/*
 * xrick/include/debug.h
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

#ifndef _DEBUG_H
#define _DEBUG_H

/* enable/disable subsystem debug */
#undef DEBUG_ENTS
#undef DEBUG_SCROLLER
#undef DEBUG_MAPS
#undef DEBUG_JOYSTICK
#undef DEBUG_EVENTS
#undef DEBUG_AUDIO
#undef DEBUG_AUDIO2
#undef DEBUG_VIDEO
#undef DEBUG_VIDEO2

/* define global debug option */
#ifdef DEBUG
#define DEBUG_VIDEO
#define DEBUG_VIDEO2
#endif

/* define IFDEBUG macros */
#ifdef DEBUG_ENTS
#define IFDEBUG_ENTS(X); X
#else
#define IFDEBUG_ENTS(X);
#endif

#ifdef DEBUG_SCROLLER
#define IFDEBUG_SCROLLER(X); X
#else
#define IFDEBUG_SCROLLER(X);
#endif

#ifdef DEBUG_MAPS
#define IFDEBUG_MAPS(X); X
#else
#define IFDEBUG_MAPS(X);
#endif

#ifdef DEBUG_JOYSTICK
#define IFDEBUG_JOYSTICK(X); X
#else
#define IFDEBUG_JOYSTICK(X);
#endif

#ifdef DEBUG_EVENTS
#define IFDEBUG_EVENTS(X); X
#else
#define IFDEBUG_EVENTS(X);
#endif

#ifdef DEBUG_AUDIO
#define IFDEBUG_AUDIO(X); X
#else
#define IFDEBUG_AUDIO(X);
#endif

#ifdef DEBUG_AUDIO2
#define IFDEBUG_AUDIO2(X); X
#else
#define IFDEBUG_AUDIO2(X);
#endif

#ifdef DEBUG_VIDEO
#define IFDEBUG_VIDEO(X); X
#else
#define IFDEBUG_VIDEO(X);
#endif

#ifdef DEBUG_VIDEO2
#define IFDEBUG_VIDEO2(X); X
#else
#define IFDEBUG_VIDEO2(X);
#endif

#endif

/* eof */


