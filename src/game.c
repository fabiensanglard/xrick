/*
 * xrick/src/game.c
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

#include <stdlib.h>

#include "system.h"
#include "config.h"
#include "game.h"

#include "draw.h"
#include "maps.h"
#include "ents.h"
#include "e_rick.h"
#include "e_sbonus.h"
#include "e_them.h"
#include "screens.h"
#include "rects.h"
#include "scroller.h"
#include "control.h"
#include "data.h"

#ifdef ENABLE_DEVTOOLS
#include "devtools.h"
#endif


/*
 * local typedefs
 */
typedef enum {
#ifdef ENABLE_DEVTOOLS
  DEVTOOLS,
#endif
  XRICK,
  INIT_GAME, INIT_BUFFER,
  INTRO_MAIN, INTRO_MAP,
  PAUSE_PRESSED1, PAUSE_PRESSED1B, PAUSED, PAUSE_PRESSED2,
  PLAY0, PLAY1, PLAY2, PLAY3,
  CHAIN_SUBMAP, CHAIN_MAP, CHAIN_END,
  SCROLL_UP, SCROLL_DOWN,
  RESTART, GAMEOVER, GETNAME, EXIT
} game_state_t;


/*
 * global vars
 */
U8 game_period = 0;
U8 game_waitevt = FALSE;
rect_t *game_rects = NULL;

U8 game_lives = 0;
U8 game_bombs = 0;
U8 game_bullets = 0;
U32 game_score = 0;

U16 game_map = 0;
U16 game_submap = 0;

U8 game_dir = 0;
U8 game_chsm = FALSE;

U8 game_cheat1 = 0;
U8 game_cheat2 = 0;
U8 game_cheat3 = 0;

#ifdef GFXST
hscore_t game_hscores[8] = {
  { 8000, "SIMES@@@@@" },
  { 7000, "JAYNE@@@@@" },
  { 6000, "DANGERSTU@" },
  { 5000, "KEN@@@@@@@" },
  { 4000, "ROB@N@BOB@" },
  { 3000, "TELLY@@@@@" },
  { 2000, "NOBBY@@@@@" },
  { 1000, "JEZEBEL@@@" }
};
#endif
#ifdef GFXPC
hscore_t game_hscores[8] = {
  { 8000, "DANGERSTU@" },
  { 7000, "SIMES@@@@@" },
  { 6000, "KEN@T@ZEN@" },
  { 5000, "BOBBLE@@@@" },
  { 4000, "GREG@LAA@@" },
  { 3000, "TELLY@@@@@" },
  { 2000, "CHIGLET@@@" },
  { 1000, "ANDYSPLEEN" }
};
#endif
#ifdef ENABLE_SOUND
sound_t *WAV_GAMEOVER;
sound_t *WAV_SBONUS2;
sound_t *WAV_BULLET;
sound_t *WAV_BOMBSHHT;
sound_t *WAV_EXPLODE;
sound_t *WAV_STICK;
sound_t *WAV_WALK;
sound_t *WAV_CRAWL;
sound_t *WAV_JUMP;
sound_t *WAV_PAD;
sound_t *WAV_BOX;
sound_t *WAV_BONUS;
sound_t *WAV_SBONUS1;
sound_t *WAV_DIE;
sound_t *WAV_ENTITY[10];
#endif


/*
 * local vars
 */
static U8 isave_frow;
static game_state_t game_state;
#ifdef ENABLE_SOUND
static sound_t *music_snd;
#endif


/*
 * prototypes
 */
static void frame(void);
static void init(void);
static void play0(void);
static void play3(void);
static void restart(void);
static void isave(void);
static void irestore(void);
static void loaddata(void);
static void freedata(void);


/*
 * Cheats
 */
#ifdef ENABLE_CHEATS
void
game_toggleCheat(U8 nbr)
{
  if (game_state != INTRO_MAIN && game_state != INTRO_MAP &&
      game_state != GAMEOVER && game_state != GETNAME &&
#ifdef ENABLE_DEVTOOLS
      game_state != DEVTOOLS &&
#endif
      game_state != XRICK && game_state != EXIT) {
    switch (nbr) {
    case 1:
      game_cheat1 = ~game_cheat1;
      game_lives = 6;
      game_bombs = 6;
      game_bullets = 6;
      break;
    case 2:
      game_cheat2 = ~game_cheat2;
      break;
    case 3:
      game_cheat3 = ~game_cheat3;
      break;
    }
    draw_infos();
    /* FIXME this should probably only raise a flag ... */
    /* plus we only need to update INFORECT not the whole screen */
    sysvid_update(&draw_SCREENRECT);
  }
}
#endif

#ifdef ENABLE_SOUND
/*
 * Music
 */
void
game_setmusic(char *name, U8 loop)
{
	U8 channel;

	if (music_snd)
		game_stopmusic();
	music_snd = syssnd_load(name);
	if (music_snd)
	{
		music_snd->dispose = TRUE; /* music is always "fire and forget" */
		channel = syssnd_play(music_snd, loop);
	}
}

void
game_stopmusic(void)
{
	syssnd_stopsound(music_snd);
	music_snd = NULL;
}
#endif

/*
 * Main loop
 */
void
game_run(void)
{
  U32 tm, tmx;

	loaddata(); /* load cached data */

	game_period = sysarg_args_period ? sysarg_args_period : GAME_PERIOD;
	tm = sys_gettime();
	game_state = XRICK;

	/* main loop */
	while (game_state != EXIT) {

		/* timer */
		tmx = tm; tm = sys_gettime(); tmx = tm - tmx;
		if (tmx < game_period) sys_sleep(game_period - tmx);

		/* video */
		/*DEBUG*//*game_rects=&draw_SCREENRECT;*//*DEBUG*/
		sysvid_update(game_rects);
		draw_STATUSRECT.next = NULL;  /* FIXME freerects should handle this */

		/* sound */
		/*snd_mix();*/

		/* events */
		if (game_waitevt)
			sysevt_wait();  /* wait for an event */
		else
			sysevt_poll();  /* process events (non-blocking) */

		/* frame */
		frame();
	}

	freedata(); /* free cached data */
}

/*
 * Prepare frame
 *
 * This function loops forever: use 'return' when a frame is ready.
 * When returning, game_rects must contain every parts of the buffer
 * that have been modified.
 */
static void
frame(void)
{
	while (1) {

		switch (game_state) {



#ifdef ENABLE_DEVTOOLS
		case DEVTOOLS:
			switch (devtools_run()) {
			case SCREEN_RUNNING:
				return;
			case SCREEN_DONE:
				game_state = INIT_GAME;
				break;
			case SCREEN_EXIT:
				game_state = EXIT;
				return;
			}
		break;
#endif



		case XRICK:
			switch(screen_xrick()) {
			case SCREEN_RUNNING:
				return;
			case SCREEN_DONE:
#ifdef ENABLE_DEVTOOLS
				game_state = DEVTOOLS;
#else
				game_state = INIT_GAME;
#endif
				break;
			case SCREEN_EXIT:
				game_state = EXIT;
				return;
			}
		break;



		case INIT_GAME:
			init();
			game_state = INTRO_MAIN;
			break;



		case INTRO_MAIN:
			switch (screen_introMain()) {
			case SCREEN_RUNNING:
				return;
			case SCREEN_DONE:
				game_state = INTRO_MAP;
				break;
			case SCREEN_EXIT:
				game_state = EXIT;
				return;
			}
		break;



		case INTRO_MAP:
			switch (screen_introMap()) {
			case SCREEN_RUNNING:
				return;
			case SCREEN_DONE:
				game_waitevt = FALSE;
				game_state = INIT_BUFFER;
				break;
			case SCREEN_EXIT:
				game_state = EXIT;
				return;
			}
		break;



		case INIT_BUFFER:
			sysvid_clear();                 /* clear buffer */
			draw_map();                     /* draw the map onto the buffer */
			draw_drawStatus();              /* draw the status bar onto the buffer */
#ifdef ENABLE_CHEATS
			draw_infos();                   /* draw the info bar onto the buffer */
#endif
			game_rects = &draw_SCREENRECT;  /* request full buffer refresh */
			game_state = PLAY0;
			return;



		case PAUSE_PRESSED1:
			screen_pause(TRUE);
			game_state = PAUSE_PRESSED1B;
			break;



		case PAUSE_PRESSED1B:
			if (control_status & CONTROL_PAUSE)
				return;
			game_state = PAUSED;
			break;



		case PAUSED:
			if (control_status & CONTROL_PAUSE)
				game_state = PAUSE_PRESSED2;
			if (control_status & CONTROL_EXIT)
				game_state = EXIT;
			return;



		case PAUSE_PRESSED2:
			if (!(control_status & CONTROL_PAUSE)) {
				game_waitevt = FALSE;
				screen_pause(FALSE);
#ifdef ENABLE_SOUND
				syssnd_pause(FALSE, FALSE);
#endif
				game_state = PLAY2;
			}
		return;



		case PLAY0:
			play0();
			break;



		case PLAY1:
			if (control_status & CONTROL_PAUSE) {
#ifdef ENABLE_SOUND
				syssnd_pause(TRUE, FALSE);
#endif
				game_waitevt = TRUE;
				game_state = PAUSE_PRESSED1;
			}
			else if (control_active == FALSE) {
#ifdef ENABLE_SOUND
				syssnd_pause(TRUE, FALSE);
#endif
				game_waitevt = TRUE;
				screen_pause(TRUE);
				game_state = PAUSED;
			}
			else
				game_state = PLAY2;
			break;



		case PLAY2:
			if E_RICK_STTST(E_RICK_STDEAD) {  /* rick is dead */
				if (game_cheat1 || --game_lives) {
					game_state = RESTART;
				} else {
					game_state = GAMEOVER;
				}
			}
			else if (game_chsm)  /* request to chain to next submap */
				game_state = CHAIN_SUBMAP;
			else
				game_state = PLAY3;
			break;



    case PLAY3:
      play3();
      return;



    case CHAIN_SUBMAP:
      if (map_chain())
	game_state = CHAIN_END;
      else {
	game_bullets = 0x06;
	game_bombs = 0x06;
	game_map++;

	if (game_map == 0x04) {
	  /* reached end of game */
	  /* FIXME @292?*/
	}

	game_state = CHAIN_MAP;
      }
      break;



    case CHAIN_MAP:                             /* CHAIN MAP */
      switch (screen_introMap()) {
      case SCREEN_RUNNING:
	return;
      case SCREEN_DONE:
	if (game_map >= 0x04) {  /* reached end of game */
	  sysarg_args_map = 0;
	  sysarg_args_submap = 0;
	  game_state = GAMEOVER;
	}
	else {  /* initialize game */
	  ent_ents[1].x = map_maps[game_map].x;
	  ent_ents[1].y = map_maps[game_map].y;
	  map_frow = (U8)map_maps[game_map].row;
	  game_submap = map_maps[game_map].submap;
	  game_state = CHAIN_END;
	}
	break;
      case SCREEN_EXIT:
	game_state = EXIT;
	return;
      }
      break;



    case CHAIN_END:
      map_init();                     /* initialize the map */
      isave();                        /* save data in case of a restart */
      ent_clprev();                   /* cleanup entities */
      draw_map();                     /* draw the map onto the buffer */
      draw_drawStatus();              /* draw the status bar onto the buffer */
      game_rects = &draw_SCREENRECT;  /* request full screen refresh */
      game_state = PLAY3;
      return;



    case SCROLL_UP:
      switch (scroll_up()) {
      case SCROLL_RUNNING:
	return;
      case SCROLL_DONE:
	game_state = PLAY0;
	break;
      }
      break;



    case SCROLL_DOWN:
      switch (scroll_down()) {
      case SCROLL_RUNNING:
	return;
      case SCROLL_DONE:
	game_state = PLAY0;
	break;
      }
      break;



    case RESTART:
      restart();
      game_state = PLAY0;
      return;



    case GAMEOVER:
      switch (screen_gameover()) {
      case SCREEN_RUNNING:
	return;
      case SCREEN_DONE:
	game_state = GETNAME;
	break;
      case SCREEN_EXIT:
	game_state = EXIT;
	break;
      }
      break;



    case GETNAME:
      switch (screen_getname()) {
      case SCREEN_RUNNING:
	return;
      case SCREEN_DONE:
	game_state = INIT_GAME;
	return;
      case SCREEN_EXIT:
	game_state = EXIT;
	break;
      }
      break;



    case EXIT:
      return;

    }
  }
}


/*
 * Initialize the game
 */
static void
init(void)
{
  U8 i;

  E_RICK_STRST(0xff);

  game_lives = 6;
  game_bombs = 6;
  game_bullets = 6;
  game_score = 0;

  game_map = sysarg_args_map;

  if (sysarg_args_submap == 0) {
    game_submap = map_maps[game_map].submap;
    map_frow = (U8)map_maps[game_map].row;
  }
  else {
    /* dirty hack to determine frow */
    game_submap = sysarg_args_submap;
    i = 0;
    while (i < MAP_NBR_CONNECT &&
	   (map_connect[i].submap != game_submap ||
	    map_connect[i].dir != RIGHT))
      i++;
    map_frow = map_connect[i].rowin - 0x10;
    ent_ents[1].y = 0x10 << 3;
  }

  ent_ents[1].x = map_maps[game_map].x;
  ent_ents[1].y = map_maps[game_map].y;
  ent_ents[1].w = 0x18;
  ent_ents[1].h = 0x15;
  ent_ents[1].n = 0x01;
  ent_ents[1].sprite = 0x01;
  ent_ents[1].front = FALSE;
  ent_ents[ENT_ENTSNUM].n = 0xFF;

  map_resetMarks();

  map_init();
  isave();
}


/*
 * play0
 *
 */
static void
play0(void)
{
  if (control_status & CONTROL_END) {  /* request to end the game */
    game_state = GAMEOVER;
    return;
  }

  if (control_last == CONTROL_EXIT) {  /* request to exit the game */
    game_state = EXIT;
    return;
  }

  ent_action();      /* run entities */
  e_them_rndseed++;  /* (0270) */

  game_state = PLAY1;
}


/*
 * play3
 *
 */
static void
play3(void)
{
  static rect_t *r;

  draw_clearStatus();  /* clear the status bar */
  ent_draw();          /* draw all entities onto the buffer */
  /* sound */
  draw_drawStatus();   /* draw the status bar onto the buffer*/

  r = &draw_STATUSRECT; r->next = ent_rects;  /* refresh status bar too */
  game_rects = r;  /* take care to cleanup draw_STATUSRECT->next later! */

  if (!E_RICK_STTST(E_RICK_STZOMBIE)) {  /* need to scroll ? */
    if (ent_ents[1].y >= 0xCC) {
      game_state = SCROLL_UP;
      return;
    }
    if (ent_ents[1].y <= 0x60) {
      game_state = SCROLL_DOWN;
      return;
    }
  }

  game_state = PLAY0;
}


/*
 * restart
 *
 */
static void
restart(void)
{
  E_RICK_STRST(E_RICK_STDEAD|E_RICK_STZOMBIE);

  game_bullets = 6;
  game_bombs = 6;

  ent_ents[1].n = 1;

  irestore();
  map_init();
  isave();
  ent_clprev();
  draw_map();
  draw_drawStatus();
  game_rects = &draw_SCREENRECT;
}


/*
 * isave (0bbb)
 *
 */
static void
isave(void)
{
  e_rick_save();
  isave_frow = map_frow;
}


/*
 * irestore (0bdc)
 *
 */
static void
irestore(void)
{
  e_rick_restore();
  map_frow = isave_frow;
}

/*
 *
 */
static void
loaddata()
{
#ifdef ENABLE_SOUND
	/*
	 * Cache sounds
	 *
	 * tune[0-5].wav not cached
	 */
	WAV_GAMEOVER = syssnd_load("sounds/gameover.wav");
	WAV_SBONUS2 = syssnd_load("sounds/sbonus2.wav");
	WAV_BULLET = syssnd_load("sounds/bullet.wav");
	WAV_BOMBSHHT = syssnd_load("sounds/bombshht.wav");
	WAV_EXPLODE = syssnd_load("sounds/explode.wav");
	WAV_STICK = syssnd_load("sounds/stick.wav");
	WAV_WALK = syssnd_load("sounds/walk.wav");
	WAV_CRAWL = syssnd_load("sounds/crawl.wav");
	WAV_JUMP = syssnd_load("sounds/jump.wav");
	WAV_PAD = syssnd_load("sounds/pad.wav");
	WAV_BOX = syssnd_load("sounds/box.wav");
	WAV_BONUS = syssnd_load("sounds/bonus.wav");
	WAV_SBONUS1 = syssnd_load("sounds/sbonus1.wav");
	WAV_DIE = syssnd_load("sounds/die.wav");
	WAV_ENTITY[0] = syssnd_load("sounds/ent0.wav");
	WAV_ENTITY[1] = syssnd_load("sounds/ent1.wav");
	WAV_ENTITY[2] = syssnd_load("sounds/ent2.wav");
	WAV_ENTITY[3] = syssnd_load("sounds/ent3.wav");
	WAV_ENTITY[4] = syssnd_load("sounds/ent4.wav");
	WAV_ENTITY[5] = syssnd_load("sounds/ent5.wav");
	WAV_ENTITY[6] = syssnd_load("sounds/ent6.wav");
	WAV_ENTITY[7] = syssnd_load("sounds/ent7.wav");
	WAV_ENTITY[8] = syssnd_load("sounds/ent8.wav");
#endif
}

/*
 *
 */
static void
freedata()
{
#ifdef ENABLE_SOUND
	syssnd_stopall();
	syssnd_free(WAV_GAMEOVER);
	syssnd_free(WAV_SBONUS2);
	syssnd_free(WAV_BULLET);
	syssnd_free(WAV_BOMBSHHT);
	syssnd_free(WAV_EXPLODE);
	syssnd_free(WAV_STICK);
	syssnd_free(WAV_WALK);
	syssnd_free(WAV_CRAWL);
	syssnd_free(WAV_JUMP);
	syssnd_free(WAV_PAD);
	syssnd_free(WAV_BOX);
	syssnd_free(WAV_BONUS);
	syssnd_free(WAV_SBONUS1);
	syssnd_free(WAV_DIE);
	syssnd_free(WAV_ENTITY[0]);
	syssnd_free(WAV_ENTITY[1]);
	syssnd_free(WAV_ENTITY[2]);
	syssnd_free(WAV_ENTITY[3]);
	syssnd_free(WAV_ENTITY[4]);
	syssnd_free(WAV_ENTITY[5]);
	syssnd_free(WAV_ENTITY[6]);
	syssnd_free(WAV_ENTITY[7]);
	syssnd_free(WAV_ENTITY[8]);
#endif
}


/* eof */
