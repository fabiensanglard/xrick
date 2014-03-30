/*
 * xrick/src/system.c
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

#include <SDL.h>

#include <stdarg.h>   /* args for sys_panic */
#include <fcntl.h>    /* fcntl in sys_panic */
#include <stdio.h>    /* printf */
#include <stdlib.h>
#include <signal.h>

#include "system.h"

/*
 * Panic
 */
void
sys_panic(char *err, ...)
{
  va_list argptr;
  char s[1024];

  /* change stdin to non blocking */
  /*fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) & ~FNDELAY);*/
  /* NOTE HPUX: use ... is it OK on Linux ? */
  /* fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) & ~O_NDELAY); */

  /* prepare message */
  va_start(argptr, err);
  vsprintf(s, err, argptr);
  va_end(argptr);

  /* print message and die */
  printf("%s\npanic!\n", s);
  exit(1);
}


/*
 * Print a message
 */
void
sys_printf(char *msg, ...)
{
  va_list argptr;
  char s[1024];

  /* change stdin to non blocking */
  /*fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) & ~FNDELAY);*/
  /* NOTE HPUX: use ... is it OK on Linux ? */
  /* fcntl(0, F_SETFL, fcntl (0, F_GETFL, 0) & ~O_NDELAY); */

  /* prepare message */
  va_start(argptr, msg);
  vsprintf(s, msg, argptr);
  va_end(argptr);
  printf(s);
}

/*
 * Return number of microseconds elapsed since first call
 */
U32
sys_gettime(void)
{
  static U32 ticks_base = 0;
  U32 ticks;

  ticks = SDL_GetTicks();

  if (!ticks_base)
    ticks_base = ticks;

  return ticks - ticks_base;
}

/*
 * Sleep a number of microseconds
 */
void
sys_sleep(int s)
{
  SDL_Delay(s);
}

/*
 * Initialize system
 */
void
sys_init(int argc, char **argv)
{
	sysarg_init(argc, argv);
	sysvid_init();
#ifdef ENABLE_JOYSTICK
	sysjoy_init();
#endif
#ifdef ENABLE_SOUND
	if (sysarg_args_nosound == 0)
		syssnd_init();
#endif
	atexit(sys_shutdown);
	signal(SIGINT, exit);
	signal(SIGTERM, exit);
}

/*
 * Shutdown system
 */
void
sys_shutdown(void)
{
#ifdef ENABLE_SOUND
	syssnd_shutdown();
#endif
#ifdef ENABLE_JOYSTICK
	sysjoy_shutdown();
#endif
	sysvid_shutdown();
}

/* eof */
