/*
 * xrick/include/syssnd.h
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

#ifndef _SYSSND_H
#define _SYSSND_H

#include "config.h"

#ifdef ENABLE_SOUND

#include "system.h"

/* 8-bit mono at 22050Hz */
#define SYSSND_FREQ 22050
#define SYSSND_CHANNELS 1
#define SYSSND_MAXVOL 10
#define SYSSND_MIXCHANNELS 8
/* MIXSAMPLES: 256 is too low on Windows. 512 means ~20 mix per second at 11025Hz */
/* MIXSAMPLES: ?? at 22050Hz */
#define SYSSND_MIXSAMPLES 1024

typedef struct {
	sound_t *snd;
	U8 *buf;
	U32 len;
	S8 loop;
} channel_t;

#endif /* ENABLE_SOUND */

#endif /* _SYSSND_H */

/* eof */


