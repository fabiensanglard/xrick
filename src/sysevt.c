/*
 * xrick/src/sysevt.c
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

/*
 * 20021010 SDLK_n replaced by SDLK_Fn because some non-US keyboards
 *          requires that SHIFT be pressed to input numbers.
 */

#include <SDL.h>

#include "system.h"
#include "config.h"
#include "game.h"
#include "debug.h"

#include "control.h"
#include "draw.h"

#define SYSJOY_RANGE 3280

#define SETBIT(x,b) x |= (b)
#define CLRBIT(x,b) x &= ~(b)

static SDL_Event event;

/*
 * Process an event
 */
static void
processEvent()
{
	U16 key;
#ifdef ENABLE_FOCUS
	SDL_ActiveEvent *aevent;
#endif

  switch (event.type) {
  case SDL_KEYDOWN:
    key = event.key.keysym.sym;
    if (key == syskbd_up || key == SDLK_UP) {
      SETBIT(control_status, CONTROL_UP);
      control_last = CONTROL_UP;
    }
    else if (key == syskbd_down || key == SDLK_DOWN) {
      SETBIT(control_status, CONTROL_DOWN);
      control_last = CONTROL_DOWN;
    }
    else if (key == syskbd_left || key == SDLK_LEFT) {
      SETBIT(control_status, CONTROL_LEFT);
      control_last = CONTROL_LEFT;
    }
    else if (key == syskbd_right || key == SDLK_RIGHT) {
      SETBIT(control_status, CONTROL_RIGHT);
      control_last = CONTROL_RIGHT;
    }
    else if (key == syskbd_pause) {
      SETBIT(control_status, CONTROL_PAUSE);
      control_last = CONTROL_PAUSE;
    }
    else if (key == syskbd_end) {
      SETBIT(control_status, CONTROL_END);
      control_last = CONTROL_END;
    }
    else if (key == syskbd_xtra) {
      SETBIT(control_status, CONTROL_EXIT);
      control_last = CONTROL_EXIT;
    }
    else if (key == syskbd_fire) {
      SETBIT(control_status, CONTROL_FIRE);
      control_last = CONTROL_FIRE;
    }
    else if (key == SDLK_F1) {
      sysvid_toggleFullscreen();
    }
    else if (key == SDLK_F2) {
      sysvid_zoom(-1);
    }
    else if (key == SDLK_F3) {
      sysvid_zoom(+1);
    }
#ifdef ENABLE_SOUND
    else if (key == SDLK_F4) {
      syssnd_toggleMute();
    }
    else if (key == SDLK_F5) {
      syssnd_vol(-1);
    }
    else if (key == SDLK_F6) {
      syssnd_vol(+1);
    }
#endif
#ifdef ENABLE_CHEATS
    else if (key == SDLK_F7) {
      game_toggleCheat(1);
    }
    else if (key == SDLK_F8) {
      game_toggleCheat(2);
    }
    else if (key == SDLK_F9) {
      game_toggleCheat(3);
    }
#endif
    break;
  case SDL_KEYUP:
    key = event.key.keysym.sym;
    if (key == syskbd_up || key == SDLK_UP) {
      CLRBIT(control_status, CONTROL_UP);
      control_last = CONTROL_UP;
    }
    else if (key == syskbd_down || key == SDLK_DOWN) {
      CLRBIT(control_status, CONTROL_DOWN);
      control_last = CONTROL_DOWN;
    }
    else if (key == syskbd_left || key == SDLK_LEFT) {
      CLRBIT(control_status, CONTROL_LEFT);
      control_last = CONTROL_LEFT;
    }
    else if (key == syskbd_right || key == SDLK_RIGHT) {
      CLRBIT(control_status, CONTROL_RIGHT);
      control_last = CONTROL_RIGHT;
    }
    else if (key == syskbd_pause) {
      CLRBIT(control_status, CONTROL_PAUSE);
      control_last = CONTROL_PAUSE;
    }
    else if (key == syskbd_end) {
      CLRBIT(control_status, CONTROL_END);
      control_last = CONTROL_END;
    }
    else if (key == syskbd_xtra) {
      CLRBIT(control_status, CONTROL_EXIT);
      control_last = CONTROL_EXIT;
    }
    else if (key == syskbd_fire) {
      CLRBIT(control_status, CONTROL_FIRE);
      control_last = CONTROL_FIRE;
    }
    break;
  case SDL_QUIT:
    /* player tries to close the window -- this is the same as pressing ESC */
    SETBIT(control_status, CONTROL_EXIT);
    control_last = CONTROL_EXIT;
    break;
#ifdef ENABLE_FOCUS
  case SDL_ACTIVEEVENT: {
    aevent = (SDL_ActiveEvent *)&event;
    IFDEBUG_EVENTS(
      printf("xrick/events: active %x %x\n", aevent->gain, aevent->state);
      );
    if (aevent->gain == 1)
      control_active = TRUE;
    else
      control_active = FALSE;
    }
  break;
#endif
#ifdef ENABLE_JOYSTICK
  case SDL_JOYAXISMOTION:
    IFDEBUG_EVENTS(sys_printf("xrick/events: joystick\n"););
    if (event.jaxis.axis == 0) {  /* left-right */
      if (event.jaxis.value < -SYSJOY_RANGE) {  /* left */
	SETBIT(control_status, CONTROL_LEFT);
	CLRBIT(control_status, CONTROL_RIGHT);
      }
      else if (event.jaxis.value > SYSJOY_RANGE) {  /* right */
	SETBIT(control_status, CONTROL_RIGHT);
	CLRBIT(control_status, CONTROL_LEFT);
      }
      else {  /* center */
	CLRBIT(control_status, CONTROL_RIGHT);
	CLRBIT(control_status, CONTROL_LEFT);
      }
    }
    if (event.jaxis.axis == 1) {  /* up-down */
      if (event.jaxis.value < -SYSJOY_RANGE) {  /* up */
	SETBIT(control_status, CONTROL_UP);
	CLRBIT(control_status, CONTROL_DOWN);
      }
      else if (event.jaxis.value > SYSJOY_RANGE) {  /* down */
	SETBIT(control_status, CONTROL_DOWN);
	CLRBIT(control_status, CONTROL_UP);
      }
      else {  /* center */
	CLRBIT(control_status, CONTROL_DOWN);
	CLRBIT(control_status, CONTROL_UP);
      }
    }
    break;
  case SDL_JOYBUTTONDOWN:
    SETBIT(control_status, CONTROL_FIRE);
    break;
  case SDL_JOYBUTTONUP:
    CLRBIT(control_status, CONTROL_FIRE);
    break;
#endif
  default:
    break;
  }
}

/*
 * Process events, if any, then return
 */
void
sysevt_poll(void)
{
  while (SDL_PollEvent(&event))
    processEvent();
}

/*
 * Wait for an event, then process it and return
 */
void
sysevt_wait(void)
{
  SDL_WaitEvent(&event);
  processEvent();
}

/* eof */



