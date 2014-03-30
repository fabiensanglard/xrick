/*
 * xrick/include/ents.h
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

#ifndef _ENTS_H
#define _ENTS_H

#include "system.h"
#include "rects.h"

#define ENT_XRICK ent_ents[1]

#define ENT_NBR_ENTDATA 0x4a
#define ENT_NBR_SPRSEQ 0x88
#define ENT_NBR_MVSTEP 0x310

#define ENT_ENTSNUM 0x0c

/*
 * flags for ent_ents[e].n  ("yes" when set)
 *
 * ENT_LETHAL: is entity lethal?
 */
#define ENT_LETHAL 0x80

/*
 * flags for ent_ents[e].flag  ("yes" when set)
 *
 * ENT_FLG_ONCE: should the entity run once only?
 * ENT_FLG_STOPRICK: does the entity stops rick (and goes to slot zero)?
 * ENT_FLG_LETHALR: is entity lethal when restarting?
 * ENT_FLG_LETHALI: is entity initially lethal?
 * ENT_FLG_TRIGBOMB: can entity be triggered by a bomb?
 * ENT_FLG_TRIGBULLET: can entity be triggered by a bullet?
 * ENT_FLG_TRIGSTOP: can entity be triggered by rick stop?
 * ENT_FLG_TRIGRICK: can entity be triggered by rick?
 */
#define ENT_FLG_ONCE 0x01
#define ENT_FLG_STOPRICK 0x02
#define ENT_FLG_LETHALR 0x04
#define ENT_FLG_LETHALI 0x08
#define ENT_FLG_TRIGBOMB 0x10
#define ENT_FLG_TRIGBULLET 0x20
#define ENT_FLG_TRIGSTOP 0x40
#define ENT_FLG_TRIGRICK 0x80

typedef struct {
  U8 n;          /* b00 */
  /*U8 b01;*/    /* b01 in ASM code but never used */
  S16 x;         /* b02 - position */
  S16 y;         /* w04 - position */
  U8 sprite;     /* b08 - sprite number */
  /*U16 w0C;*/   /* w0C in ASM code but never used */
  U8 w;          /* b0E - width */
  U8 h;          /* b10 - height */
  U16 mark;      /* w12 - number of the mark that created the entity */
  U8 flags;      /* b14 */
  S16 trig_x;    /* b16 - position of trigger box */
  S16 trig_y;    /* w18 - position of trigger box */
  S16 xsave;     /* b1C */
  S16 ysave;     /* w1E */
  U16 sprbase;   /* w20 */
  U16 step_no_i; /* w22 */
  U16 step_no;   /* w24 */
  S16 c1;        /* b26 */
  S16 c2;        /* b28 */
  U8 ylow;       /* b2A */
  S16 offsy;     /* w2C */
  U8 latency;    /* b2E */
  U8 prev_n;     /* new */
  S16 prev_x;    /* new */
  S16 prev_y;    /* new */
  U8 prev_s;     /* new */
  U8 front;      /* new */
  U8 trigsnd;    /* new */
} ent_t;

typedef struct {
  U8 w, h;
  U16 spr, sni;
  U8 trig_w, trig_h;
  U8 snd;
} entdata_t;

typedef struct {
  U8 count;
  S8 dx, dy;
} mvstep_t;

extern ent_t ent_ents[ENT_ENTSNUM + 1];
extern entdata_t ent_entdata[ENT_NBR_ENTDATA];
extern rect_t *ent_rects;
extern U8 ent_sprseq[ENT_NBR_SPRSEQ];
extern mvstep_t ent_mvstep[ENT_NBR_MVSTEP];

extern void ent_reset(void);
extern void ent_actvis(U8, U8);
extern void ent_draw(void);
extern void ent_clprev(void);
extern void ent_action(void);

#endif

/* eof */
