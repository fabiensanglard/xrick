/*
 * xrick/include/game.h
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

#ifndef _GAME_H
#define _GAME_H

#include <stddef.h> /* NULL */

#include "system.h"
#include "config.h"

#include "rects.h"
#include "data.h"

#define LEFT 1
#define RIGHT 0

#define TRUE 1
#define FALSE 0

#define GAME_PERIOD 75

#define GAME_BOMBS_INIT 6
#define GAME_BULLETS_INIT 6

typedef struct {
  U32 score;
  U8 name[10];
} hscore_t;

extern U8 game_lives;      /* lives counter */
extern U8 game_bombs;      /* bombs counter */
extern U8 game_bullets;    /* bullets counter */

extern U32 game_score;     /* score */

extern hscore_t game_hscores[8];  /* highest scores (hall of fame) */

extern U16 game_map;       /* current map */
extern U16 game_submap;    /* current submap */

extern U8 game_dir;        /* direction (LEFT, RIGHT) */
extern U8 game_chsm;       /* change submap request (TRUE, FALSE) */

extern U8 game_waitevt;    /* wait for events (TRUE, FALSE) */
extern U8 game_period;     /* time between each frame, in millisecond */

extern rect_t *game_rects; /* rectangles to redraw at each frame */

extern void game_run(void);
extern void game_setmusic(char *name, U8 loop);
extern void game_stopmusic(void);


#ifdef ENABLE_CHEATS
extern U8 game_cheat1;     /* infinite lives, bombs and bullets */
extern U8 game_cheat2;     /* never die */
extern U8 game_cheat3;     /* highlight sprites */
extern void game_toggleCheat(U8);
#endif

#ifdef ENABLE_SOUND
extern sound_t *WAV_GAMEOVER;
extern sound_t *WAV_SBONUS2;
extern sound_t *WAV_BULLET;
extern sound_t *WAV_BOMBSHHT;
extern sound_t *WAV_EXPLODE;
extern sound_t *WAV_STICK;
extern sound_t *WAV_WALK;
extern sound_t *WAV_CRAWL;
extern sound_t *WAV_JUMP;
extern sound_t *WAV_PAD;
extern sound_t *WAV_BOX;
extern sound_t *WAV_BONUS;
extern sound_t *WAV_SBONUS1;
extern sound_t *WAV_DIE;
extern sound_t *WAV_ENTITY[];
#endif

#endif

/* eof */


