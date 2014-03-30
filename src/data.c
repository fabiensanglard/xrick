/*
 * xrick/src/data.c
 *
 * Copyright (C) 1998-2002 BigOrno (bigorno@bigorno.net). All rights reserved.
 *
 * The use and distribution terms for this software are contained in the file
 * named README, which can be found in the root of this distribution. By
 * using this software in any fashion, you are agreeing to be bound by the
 * terms of this license.
 *
 * You must not remove this notice, or any other, from this software.
 */

#include <stdlib.h>  /* malloc */
#include <string.h>

#include "system.h"
#include "data.h"

#include "unzip.h"

/*
 * Private typedefs
 */
typedef struct {
	char *name;
	unzFile zip;
} zipped_t;

typedef struct {
	char *name;
	unzFile zip;
} path_t;

/*
 * Static variables
 */
static path_t path;

/*
 * Prototypes
 */
static int str_zipext(char *);
static char *str_dup(char *);
static char *str_slash(char *);

/*
 *
 */
void
data_setpath(char *name)
{
	unzFile zip;
	char *n;

	if (str_zipext(name)) {
		/* path has .zip extension */
		n = str_slash(str_dup(name));
		zip = unzOpen(n);
		if (!zip) {
			free(n);
			sys_panic("(data) can not open data");
		} else {
			path.zip = zip;
			path.name = n;
		}
	} else {
		/* path has no .zip extension. it should be a directory */
		/* FIXME check that it is a valid directory */
		path.zip = NULL;
		path.name = str_dup(name);
	}
}

/*
 *
 */
void
data_closepath()
{
	if (path.zip) {
		unzClose(path.zip);
		path.zip = NULL;
	}
	free(path.name);
	path.name = NULL;
}

/*
 * Open a data file.
 */
data_file_t *
data_file_open(char *name)
{
	char *n;
	FILE *fh;
	zipped_t *z;

	if (path.zip) {
	    z = malloc(sizeof(zipped_t));
	    z->name = strdup(name);
	    z->zip = unzDup(path.zip);
	    if (unzLocateFile(z->zip, name, 0) != UNZ_OK ||
	    	unzOpenCurrentFile(z->zip) != UNZ_OK) {
			unzClose(z->zip);
			z = NULL;
		}
	    return (data_file_t *)z;
	} else {
		n = malloc(strlen(path.name) + strlen(name) + 2);
		sprintf(n, "%s/%s", path.name, name);
		str_slash(n);
		fh = fopen(n, "rb");
		return (data_file_t *)fh;
	}
}

int
data_file_size(data_file_t *file)
{
	int s;
	if (path.zip) {
		/* not implemented */
	} else {
		fseek((FILE *)file, 0, SEEK_END);
		s = ftell((FILE *)file);
		fseek((FILE *)file, 0, SEEK_SET);
	}
	return s;
}

/*
 * Seek.
 */
int
data_file_seek(data_file_t *file, long offset, int origin)
{
	if (path.zip) {
		/* not implemented */
		return -1;
	} else {
		return fseek((FILE *)file, offset, origin);
	}
}

/*
 * Tell.
 */
int
data_file_tell(data_file_t *file)
{
	if (path.zip) {
		/* not implemented */
		return -1;
	} else {
		return ftell((FILE *)file);
	}
}

/*
 * Read a file within a data archive.
 */
int
data_file_read(data_file_t *file, void *buf, size_t size, size_t count)
{
	if (path.zip) {
		return unzReadCurrentFile(((zipped_t *)file)->zip, buf, size * count) / size;
	} else {
		return fread(buf, size, count, (FILE *)file);
	}
}

/*
 * Close a file within a data archive.
 */
void
data_file_close(data_file_t *file)
{
	if (path.zip) {
		unzClose(((zipped_t *)file)->zip);
		((zipped_t *)file)->zip = NULL;
		free(((zipped_t *)file)->name);
		((zipped_t *)file)->name = NULL;
	} else {
		fclose((FILE *)file);
	}
}

/*
 * Returns 1 if filename has .zip extension.
 */
static int
str_zipext(char *name)
{
	int i;

	i = strlen(name) - 1;
	if (i < 0 || name[i] != 'p' && name[i] != 'P') return 0;
	i--;
	if (i < 0 || name[i] != 'i' && name[i] != 'I') return 0;
	i--;
	if (i < 0 || name[i] != 'z' && name[i] != 'Z') return 0;
	i--;
	if (i < 0 || name[i] != '.') return 0;
	i--;
	if (i < 0) return 0;
	return 1;
}

/*
 *
 */
static char *
str_dup(char *s)
{
	char *s1;
	int i;

	i = strlen(s) + 1;
	s1 = malloc(i);
	strncpy(s1, s, i);
	return s1;
}

static char *
str_slash(char *s)
{
#ifdef __WIN32__
	int i, l;

	l = strlen(s);
	for (i = 0; i < l; i++)
		if (s[i] == '/') s[i] = '\\';
#endif
	return s;
}

/* eof */
