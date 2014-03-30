/*
 * xrick/src/sysjoy.c
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

#include "config.h"

#ifdef ENABLE_JOYSTICK

#include "system.h"
#include "debug.h"

static SDL_Joystick *j = NULL;

void
sysjoy_init(void)
{
  U8 i, jcount;

  if (SDL_InitSubSystem(SDL_INIT_JOYSTICK) < 0) {
    IFDEBUG_JOYSTICK(
      sys_printf("xrick/joystick: can not initialize joystick subsystem\n");
      );
    return;
  }

  jcount = SDL_NumJoysticks();
  if (!jcount) {  /* no joystick on this system */
    IFDEBUG_JOYSTICK(sys_printf("xrick/joystick: no joystick available\n"););
    return;
  }

  /* use the first joystick that we can open */
  for (i = 0; i < jcount; i++) {
    j = SDL_JoystickOpen(i);
    if (j)
      break;
  }

  /* enable events */
  SDL_JoystickEventState(SDL_ENABLE);
}

void
sysjoy_shutdown(void)
{
  if (j)
    SDL_JoystickClose(j);
}

#endif /* ENABLE_JOYSTICK */

/* eof */

