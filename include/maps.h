/*
 * xrick/include/maps.h
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

#ifndef _MAPS_H
#define _MAPS_H

#include "system.h"

#define MAP_NBR_MAPS 0x05
#define MAP_NBR_SUBMAPS 0x2F
#define MAP_NBR_CONNECT 0x99
#define MAP_NBR_BNUMS 0x1FD8
#define MAP_NBR_BLOCKS 0x0100
#define MAP_NBR_MARKS 0x020B
#define MAP_NBR_EFLGC 0x0020

/*
 * map row definitions, for three zones : hidden top, screen, hidden bottom
 * the three zones compose map_map, which contains the definition of the
 * current portion of the submap.
 */
#define MAP_ROW_HTTOP 0x00
#define MAP_ROW_HTBOT 0x07
#define MAP_ROW_SCRTOP 0x08
#define MAP_ROW_SCRBOT 0x1F
#define MAP_ROW_HBTOP 0x20
#define MAP_ROW_HBBOT 0x27

extern U8 map_map[0x2c][0x20];

/*
 * main maps
 */
typedef struct {
  U16 x, y;		/* initial position for rick */
  U16 row;		/* initial map_map top row within the submap */
  U16 submap;	/* initial submap */
  char *tune;	/* map tune */
} map_t;

extern map_t map_maps[MAP_NBR_MAPS];

/*
 * sub maps
 */
typedef struct {
  U16 page;            /* tiles page */
  U16 bnum;            /* first block number */
  U16 connect;         /* first connection */
  U16 mark;            /* first entity mark */
} submap_t;

extern submap_t map_submaps[MAP_NBR_SUBMAPS];

/*
 * connections
 */
typedef struct {
  U8 dir;
  U8 rowout;
  U8 submap;
  U8 rowin;
} connect_t;

extern connect_t map_connect[MAP_NBR_CONNECT];

/*
 * blocks - one block is 4 by 4 tiles.
 */
typedef U8 block_t[0x10];

extern block_t map_blocks[MAP_NBR_BLOCKS];

/*
 * flags for map_marks[].ent ("yes" when set)
 *
 * MAP_MARK_NACT: this mark is not active anymore.
 */
#define MAP_MARK_NACT (0x80)

/*
 * mark structure
 */
typedef struct {
  U8 row;
  U8 ent;
  U8 flags;
  U8 xy;  /* bits XXXX XYYY (from b03) with X->x, Y->y */
  U8 lt;  /* bits XXXX XNNN (from b04) with X->trig_x, NNN->lat & trig_y */
} mark_t;

extern mark_t map_marks[MAP_NBR_MARKS];

/*
 * block numbers, i.e. array of rows of 8 blocks
 */
extern U8 map_bnums[MAP_NBR_BNUMS];

/*
 * flags for map_eflg[map_map[row][col]]  ("yes" when set)
 *
 * MAP_EFLG_VERT: vertical move only (usually on top of _CLIMB).
 * MAP_EFLG_SOLID: solid block, can't go through.
 * MAP_EFLG_SPAD: super pad. can't go through, but sends entities to the sky.
 * MAP_EFLG_WAYUP: solid block, can't go through except when going up.
 * MAP_EFLG_FGND: foreground (hides entities).
 * MAP_EFLG_LETHAL: lethal (kill entities).
 * MAP_EFLG_CLIMB: entities can climb here.
 * MAP_EFLG_01:
 */
#define MAP_EFLG_VERT (0x80)
#define MAP_EFLG_SOLID (0x40)
#define MAP_EFLG_SPAD (0x20)
#define MAP_EFLG_WAYUP (0x10)
#define MAP_EFLG_FGND (0x08)
#define MAP_EFLG_LETHAL (0x04)
#define MAP_EFLG_CLIMB (0x02)
#define MAP_EFLG_01 (0x01)

extern U8 map_eflg_c[MAP_NBR_EFLGC];  /* compressed */
extern U8 map_eflg[0x100];  /* current */

/*
 * map_map top row within the submap
 */
extern U8 map_frow;

/*
 * tiles offset
 */
extern U8 map_tilesBank;

extern void map_expand(void);
extern void map_init(void);
extern U8 map_chain(void);
extern void map_resetMarks(void);

#endif

/* eof */
