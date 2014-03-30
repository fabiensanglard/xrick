/*
 * xrick/include/img.h
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

#ifndef _IMG_H
#define _IMG_H

#include "system.h"

typedef struct {
  U8 r, g, b, nothing;
} img_color_t;

typedef struct {
  U16 w, h;
  U16 ncolors;
  img_color_t *colors;
  U8 *pixels;
} img_t;

img_t *IMG_SPLASH;

#endif

/* eof */
