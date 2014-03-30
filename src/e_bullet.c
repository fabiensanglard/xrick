/*
 * xrick/src/e_bullet.c
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

#include "system.h"
#include "game.h"
#include "ents.h"
#include "e_bullet.h"

#include "maps.h"

/*
 * public vars (for performance reasons)
 */
S8 e_bullet_offsx;
S16 e_bullet_xc, e_bullet_yc;

/*
 * Initialize bullet
 */
void
e_bullet_init(U16 x, U16 y)
{
  E_BULLET_ENT.n = 0x02;
  E_BULLET_ENT.x = x;
  E_BULLET_ENT.y = y + 0x0006;
  if (game_dir == LEFT) {
    e_bullet_offsx = -0x08;
    E_BULLET_ENT.sprite = 0x21;
  }
  else {
    e_bullet_offsx = 0x08;
    E_BULLET_ENT.sprite = 0x20;
  }
#ifdef ENABLE_SOUND
  syssnd_play(WAV_BULLET, 1);
#endif
}


/*
 * Entity action
 *
 * ASM 1883, 0F97
 */
void
e_bullet_action(UNUSED(U8 e))
{
  /* move bullet */
  E_BULLET_ENT.x += e_bullet_offsx;

  if (E_BULLET_ENT.x <= -0x10 || E_BULLET_ENT.x > 0xe8) {
    /* out: deactivate */
    E_BULLET_ENT.n = 0;
  }
  else {
    /* update bullet center coordinates */
    e_bullet_xc = E_BULLET_ENT.x + 0x0c;
    e_bullet_yc = E_BULLET_ENT.y + 0x05;
    if (map_eflg[map_map[e_bullet_yc >> 3][e_bullet_xc >> 3]] &
	MAP_EFLG_SOLID) {
      /* hit something: deactivate */
      E_BULLET_ENT.n = 0;
    }
  }
}


/* eof */


