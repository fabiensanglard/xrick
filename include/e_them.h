/*
 * xrick/include/e_them.h
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

#ifndef _E_THEM_H
#define _E_THEM_H

#include "system.h"

extern U32 e_them_rndseed;

extern void e_them_t1a_action(U8);
extern void e_them_t1b_action(U8);
extern void e_them_t2_action(U8);
extern void e_them_t3_action(U8);
extern void e_them_z_action(U8);

#endif

/* eof */
