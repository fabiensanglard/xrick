/*
 * xrick/src/scr_pause.c
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

#include <stddef.h> /* NULL */

#include "system.h"
#include "game.h"
#include "screens.h"

#include "draw.h"
#include "control.h"
#include "ents.h"


/*
 * Display the pause indicator
 */
void
screen_pause(U8 pause)
{
  if (pause == TRUE) {
    draw_tilesBank = 0;
    draw_tllst = screen_pausedtxt;
    draw_setfb(120, 80);
#ifdef GFXPC
    draw_filter = 0xAAAA;
#endif
    draw_tilesList();
  }
  else {
#ifdef GFXPC
    draw_filter = 0xFFFF;
#endif
    draw_map();
    ent_draw();
    draw_drawStatus();
  }
  game_rects = &draw_SCREENRECT;
}


/* eof */

