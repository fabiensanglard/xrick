/*
 * xrick/include/e_rick.h
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

#ifndef _E_RICK_H
#define _E_RICK_H

#include "system.h"

#define E_RICK_NO 1
#define E_RICK_ENT ent_ents[E_RICK_NO]

extern U8 e_rick_state;
extern S16 e_rick_stop_x, e_rick_stop_y;

#define E_RICK_STSTOP 0x01
#define E_RICK_STSHOOT 0x02
#define E_RICK_STCLIMB 0x04
#define E_RICK_STJUMP 0x08
#define E_RICK_STZOMBIE 0x10
#define E_RICK_STDEAD 0x20
#define E_RICK_STCRAWL 0x40

#define E_RICK_STSET(X) e_rick_state |= (X)
#define E_RICK_STRST(X) e_rick_state &= ~(X)
#define E_RICK_STTST(X) (e_rick_state & (X))

extern void e_rick_save(void);
extern void e_rick_restore(void);
extern void e_rick_action(U8);
extern void e_rick_gozombie(void);
extern U8 e_rick_boxtest(U8);

#endif

/* eof */


