/*
 * xrick/src/e_bomb.c
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
#include "e_bomb.h"

#include "e_rick.h"

/*
 * public vars (for performance reasons)
 */
U8 e_bomb_lethal;
U8 e_bomb_xc;
U16 e_bomb_yc;

/*
 * private vars
 */
U8 e_bomb_ticker;

/*
 * Bomb hit test
 *
 * ASM 11CD
 * returns: TRUE/hit, FALSE/not
 */
U8 e_bomb_hit(U8 e)
{
	if (ent_ents[e].x > (E_BOMB_ENT.x >= 0xE0 ? 0xFF : E_BOMB_ENT.x + 0x20))
			return FALSE;
	if (ent_ents[e].x + ent_ents[e].w < (E_BOMB_ENT.x > 0x04 ? E_BOMB_ENT.x - 0x04 : 0))
			return FALSE;
	if (ent_ents[e].y > (E_BOMB_ENT.y + 0x1D))
			return FALSE;
	if (ent_ents[e].y + ent_ents[e].h < (E_BOMB_ENT.y > 0x0004 ? E_BOMB_ENT.y - 0x0004 : 0))
			return FALSE;
	return TRUE;
}

/*
 * Initialize bomb
 */
void e_bomb_init(U16 x, U16 y)
{
    E_BOMB_ENT.n = 0x03;
    E_BOMB_ENT.x = x;
    E_BOMB_ENT.y = y;
    e_bomb_ticker = E_BOMB_TICKER;
    e_bomb_lethal = FALSE;

    /*
     * Atari ST dynamite sprites are not centered the
     * way IBM PC sprites were ... need to adjust things a little bit
     */
#ifdef GFXST
    E_BOMB_ENT.x += 4;
    E_BOMB_ENT.y += 5;
#endif

}


/*
 * Entity action
 *
 * ASM 18CA
 */
void
e_bomb_action(UNUSED(U8 e))
{
	/* tick */
	e_bomb_ticker--;

	if (e_bomb_ticker == 0)
	{
		/*
		 * end: deactivate
		 */
		E_BOMB_ENT.n = 0;
		e_bomb_lethal = FALSE;
	}
	else if (e_bomb_ticker >= 0x0A)
	{
		/*
		 * ticking
		 */
#ifdef ENABLE_SOUND
		if ((e_bomb_ticker & 0x03) == 0x02)
			syssnd_play(WAV_BOMBSHHT, 1);
#endif
#ifdef GFXST
		/* ST bomb sprites sequence is longer */
		if (e_bomb_ticker < 40)
			E_BOMB_ENT.sprite = 0x99 + 19 - (e_bomb_ticker >> 1);
		else
#endif
		E_BOMB_ENT.sprite = (e_bomb_ticker & 0x01) ? 0x23 : 0x22;
	}
	else if (e_bomb_ticker == 0x09)
	{
		/*
		 * explode
		 */
#ifdef ENABLE_SOUND
		syssnd_play(WAV_EXPLODE, 1);
#endif
#ifdef GFXPC
		E_BOMB_ENT.sprite = 0x24 + 4 - (e_bomb_ticker >> 1);
#endif
#ifdef GFXST
		/* See above: fixing alignment */
		E_BOMB_ENT.x -= 4;
		E_BOMB_ENT.y -= 5;
		E_BOMB_ENT.sprite = 0xa8 + 4 - (e_bomb_ticker >> 1);
#endif
		e_bomb_xc = E_BOMB_ENT.x + 0x0C;
		e_bomb_yc = E_BOMB_ENT.y + 0x000A;
		e_bomb_lethal = TRUE;
		if (e_bomb_hit(E_RICK_NO))
			e_rick_gozombie();
	}
	else
	{
		/*
		 * exploding
		 */
#ifdef GFXPC
		E_BOMB_ENT.sprite = 0x24 + 4 - (e_bomb_ticker >> 1);
#endif
#ifdef GFXST
		E_BOMB_ENT.sprite = 0xa8 + 4 - (e_bomb_ticker >> 1);
#endif
		/* exploding, hence lethal */
		if (e_bomb_hit(E_RICK_NO))
			e_rick_gozombie();
	}
}

/* eof */


