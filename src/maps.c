/*
 * xrick/src/maps.c
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
 * NOTES
 *
 * A map is composed of submaps, which in turn are composed of rows of
 * 0x20 tiles. map_map contains the tiles for the current portion of the
 * current submap, i.e. a little bit more than what appear on the screen,
 * but not the whole submap.
 *
 * map_frow is map_map top row within the submap.
 *
 * Submaps are stored as arrays of blocks, each block being a 4x4 tile
 * array. map_submaps[].bnum points to the first block of the array.
 *
 * Before a submap can be played, it needs to be expanded from blocks
 * to map_map.
 */

#include "system.h"
#include "game.h"
#include "maps.h"
#include "debug.h"

#include "ents.h"
#include "draw.h"
#include "screens.h"
#include "e_sbonus.h"

/*
 * global vars
 */
U8 map_map[0x2C][0x20];
U8 map_eflg[0x100];
U8 map_frow;
U8 map_tilesBank;


/*
 * prototypes
 */
static void map_eflg_expand(U8);


/*
 * Fill in map_map with tile numbers by expanding blocks.
 *
 * add map_submaps[].bnum to map_frow to find out where to start from.
 * We need to /4 map_frow to convert from tile rows to block rows, then
 * we need to *8 to convert from block rows to block numbers (there
 * are 8 blocks per block row). This is achieved by *2 then &0xfff8.
 */
void
map_expand(void)
{
  U8 i, j, k, l;
  U8 row, col;
  U16 pbnum;

  pbnum = map_submaps[game_submap].bnum + ((2 * map_frow) & 0xfff8);
  row = col = 0;

  for (i = 0; i < 0x0b; i++) {  /* 0x0b rows of blocks */
    for (j = 0; j < 0x08; j++) {  /* 0x08 blocks per row */
      for (k = 0, l = 0; k < 0x04; k++) {  /* expand one block */
	map_map[row][col++] = map_blocks[map_bnums[pbnum]][l++];
	map_map[row][col++] = map_blocks[map_bnums[pbnum]][l++];
	map_map[row][col++] = map_blocks[map_bnums[pbnum]][l++];
	map_map[row][col]   = map_blocks[map_bnums[pbnum]][l++];
	row += 1; col -= 3;
      }
      row -= 4; col += 4;
      pbnum++;
    }
    row += 4; col = 0;
  }
}


/*
 * Initialize a new submap
 *
 * ASM 0cc3
 */
void
map_init(void)
{
  /*sys_printf("xrick/map_init: map=%#04x submap=%#04x\n", g_map, game_submap);*/
#ifdef GFXPC
  draw_filter = 0xffff;
  map_tilesBank = (map_submaps[game_submap].page == 1) ? 3 : 2;
#endif
#ifdef GFXST
  map_tilesBank = (map_submaps[game_submap].page == 1) ? 2 : 1;
#endif
  map_eflg_expand((map_submaps[game_submap].page == 1) ? 0x10 : 0x00);
  map_expand();
  ent_reset();
  ent_actvis(map_frow + MAP_ROW_SCRTOP, map_frow + MAP_ROW_SCRBOT);
  ent_actvis(map_frow + MAP_ROW_HTTOP, map_frow + MAP_ROW_HTBOT);
  ent_actvis(map_frow + MAP_ROW_HBTOP, map_frow + MAP_ROW_HBBOT);
}


/*
 * Expand entity flags for this map
 *
 * ASM 1117
 */
void
map_eflg_expand(U8 offs)
{
  U8 i, j, k;

  for (i = 0, k = 0; i < 0x10; i++) {
    j = map_eflg_c[offs + i++];
    while (j--) map_eflg[k++] = map_eflg_c[offs + i];
  }
}


/*
 * Chain (sub)maps
 *
 * ASM 0c08
 * return: TRUE/next submap OK, FALSE/map finished
 */
U8
map_chain(void)
{
  U16 c, t;

  game_chsm = 0;
  e_sbonus_counting = FALSE;

  /* find connection */
  c = map_submaps[game_submap].connect;
  t = 3;

  IFDEBUG_MAPS(
    sys_printf("xrick/maps: chain submap=%#04x frow=%#04x .connect=%#04x %s\n",
	       game_submap, map_frow, c,
	       (game_dir == LEFT ? "-> left" : "-> right"));
  );

  /*
   * look for the first connector with compatible row number. if none
   * found, then panic
   */
  for (c = map_submaps[game_submap].connect; ; c++) {
    if (map_connect[c].dir == 0xff)
      sys_panic("(map_chain) can not find connector\n");
    if (map_connect[c].dir != game_dir) continue;
    t = (ent_ents[1].y >> 3) + map_frow - map_connect[c].rowout;
    if (t < 3) break;
  }

  /* got it */
  IFDEBUG_MAPS(
    sys_printf("xrick/maps: chain frow=%#04x y=%#06x\n",
	       map_frow, ent_ents[1].y);
    sys_printf("xrick/maps: chain connect=%#04x rowout=%#04x - ",
	       c, map_connect[c].rowout);
    );

  if (map_connect[c].submap == 0xff) {
    /* no next submap - request next map */
    IFDEBUG_MAPS(
      sys_printf("chain to next map\n");
      );
    return FALSE;
  }
  else  {
    /* next submap */
    IFDEBUG_MAPS(
      sys_printf("chain to submap=%#04x rowin=%#04x\n",
		 map_connect[c].submap, map_connect[c].rowin);
      );
    map_frow = map_frow - map_connect[c].rowout + map_connect[c].rowin;
    game_submap = map_connect[c].submap;
    IFDEBUG_MAPS(
      sys_printf("xrick/maps: chain frow=%#04x\n",
		 map_frow);
      );
    return TRUE;
  }
}


/*
 * Reset all marks, i.e. make them all active again.
 *
 * ASM 0025
 *
 */
void
map_resetMarks(void)
{
  U16 i;
  for (i = 0; i < MAP_NBR_MARKS; i++)
    map_marks[i].ent &= ~MAP_MARK_NACT;
}


/* eof */
