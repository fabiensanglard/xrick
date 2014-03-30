/*
 * xrick/src/scr_imap.c
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

#include <stdio.h>

#include "system.h"
#include "game.h"
#include "screens.h"

#include "rects.h"
#include "draw.h"
#include "control.h"
#include "maps.h"

/*
 * local vars
 */
static U16 step;              /* current step */
static U16 count;             /* number of loops for current step */
static U16 run;               /* 1 = run, 0 = no more step */
static U8 flipflop;           /* flipflop for top, bottom, left, right */
static U16 spnum;             /* sprite number */
static U16 spx, spdx;         /* sprite x position and delta */
static U16 spy, spdy;         /* sprite y position and delta */
static U16 spbase, spoffs;    /* base, offset for sprite numbers table */
static U8 seq = 0;            /* anim sequence */

static rect_t anim_rect = { 120, 16, 64, 64, NULL }; /* anim rectangle */

/*
 * prototypes
 */
static void drawtb(void);
static void drawlr(void);
static void drawsprite(void);
static void drawcenter(void);
static void nextstep(void);
static void anim(void);
static void init(void);

/*
 * Map introduction
 *
 * ASM: 1948
 *
 * return: SCREEN_RUNNING, SCREEN_DONE, SCREEN_EXIT
 */
U8
screen_introMap(void)
{
  switch (seq) {
  case 0:
    sysvid_clear();

#ifdef GFXPC
    draw_tilesBank = 1;
    draw_filter = 0xAAAA;
#endif
#ifdef GFXST
    draw_tilesBank = 0;
#endif
    draw_tllst = screen_imaptext[game_map];
    draw_setfb(32, 0);
    draw_tilesSubList();

    draw_setfb(32, 96);
#ifdef GFXPC
    draw_filter = 0x5555;
#endif
    draw_tilesList();

    game_rects = NULL;

#ifdef GFXPC
    draw_filter = 0xFFFF;
#endif

    init();
    nextstep();
    drawcenter();
    drawtb();
    drawlr();
    drawsprite();
    control_last = 0;

    game_rects = &draw_SCREENRECT;

#ifdef ENABLE_SOUND
	game_setmusic(map_maps[game_map].tune, 1);
#endif

	seq = 1;
    break;
  case 1:  /* top and bottom borders */
    drawtb();
    game_rects = &anim_rect;
    seq = 2;
    break;
  case 2:  /* background and sprite */
    anim();
    drawcenter();
    drawsprite();
    game_rects = &anim_rect;
    seq = 3;
    break;
  case 3:  /* all borders */
    drawtb();
    drawlr();
    game_rects = &anim_rect;
    seq = 1;
    break;
  case 4:  /* wait for key release */
    if (!(control_status & CONTROL_FIRE))
      seq = 5;
    else
      sys_sleep(50); /* .5s */
    break;
  }

  if (control_status & CONTROL_FIRE) {  /* end as soon as key pressed */
    seq = 4;
  }

  if (control_status & CONTROL_EXIT)  /* check for exit request */
    return SCREEN_EXIT;

  if (seq == 5) {  /* end as soon as key pressed */
    sysvid_clear();
    seq = 0;
	return SCREEN_DONE;
  }
  else
    return SCREEN_RUNNING;
}


/*
 * Display top and bottom borders (0x1B1F)
 *
 */
static void
drawtb(void)
{
  U8 i;

  flipflop++;
  if (flipflop & 0x01) {
    draw_setfb(128, 16);
    for (i = 0; i < 6; i++)
      draw_tile(0x40);
    draw_setfb(128, 72);
    for (i = 0; i < 6; i++)
      draw_tile(0x06);
  }
  else {
    draw_setfb(128, 16);
    for (i = 0; i < 6; i++)
      draw_tile(0x05);
    draw_setfb(128, 72);
    for (i = 0; i < 6; i++)
      draw_tile(0x40);
  }
}


/*
 * Display left and right borders (0x1B7C)
 *
 */
static void
drawlr(void)
{
  U8 i;

  if (flipflop & 0x02) {
    for (i = 0; i < 8; i++) {
      draw_setfb(120, 16 + i * 8);
      draw_tile(0x04);
      draw_setfb(176, 16 + i * 8);
      draw_tile(0x04);
    }
  }
  else {
    for (i = 0; i < 8; i++) {
      draw_setfb(120, 16 + i * 8);
      draw_tile(0x2B);
      draw_setfb(176, 16 + i * 8);
      draw_tile(0x2B);
    }
  }
}


/*
 * Draw the sprite (0x19C6)
 *
 */
static void
drawsprite(void)
{
  draw_sprite(spnum, 128 + ((spx << 1) & 0x1C), 24 + (spy << 1));
}


/*
 * Draw the background (0x1AF1)
 *
 */
static void
drawcenter(void)
{
  static U8 tn0[] = { 0x07, 0x5B, 0x7F, 0xA3, 0xC7 };
  U8 i, j, tn;

  tn = tn0[game_map];
  for (i = 0; i < 6; i++) {
    draw_setfb(128, (24 + 8 * i));
    for (j = 0; j < 6; j++)
      draw_tile(tn++);
  }
}


/*
 * Next Step (0x1A74)
 *
 */
static void
nextstep(void)
{
  if (screen_imapsteps[step].count) {
    count = screen_imapsteps[step].count;
    spdx = screen_imapsteps[step].dx;
    spdy = screen_imapsteps[step].dy;
    spbase = screen_imapsteps[step].base;
    spoffs = 0;
    step++;
  }
  else {
    run = 0;
  }
}


/*
 * Anim (0x1AA8)
 *
 */
static void
anim(void)
{
  U16 i;

  if (run) {
    i = screen_imapsl[spbase + spoffs];
    if (i == 0) {
      spoffs = 0;
      i = screen_imapsl[spbase];
    }
    spnum = i;
    spoffs++;
    spx += spdx;
    spy += spdy;
    count--;
    if (count == 0)
      nextstep();
  }
}


/*
 * Initialize (0x1A43)
 *
 */
static void
init(void)
{
  run = 0; run--;
  step = screen_imapsofs[game_map];
  spx = screen_imapsteps[step].dx;
  spy = screen_imapsteps[step].dy;
  step++;
  spnum = 0; /* NOTE spnum in [8728] is never initialized ? */
}

/* eof */



