#
# xrick/src/Makefile
#
# Copyright (C) 1998-2002 BigOrno (bigorno@bigorno.net). All rights reserved.
#
# The use and distribution terms for this software are contained in the file
# named README, which can be found in the root of this distribution. By
# using this software in any fashion, you are agreeing to be bound by the
# terms of this license.
#
# You must not remove this notice, or any other, from this software.
#

#
# Includes
#
#
ifeq (.depend, $(wildcard .depend))
include .depend
endif

ifeq (../Makefile.global, $(wildcard ../Makefile.global))
include ../Makefile.global
else
$(error Oops)
endif

#
# Defs
#
#
TARGET = ../xrick
OBJECTS = unzip.o data.o scr_xrick.o scr_pause.o scr_imain.o scr_imap.o scr_gameover.o scr_getname.o dat_picsPC.o dat_picsST.o dat_screens.o dat_tilesPC.o dat_tilesST.o dat_maps.o dat_ents.o dat_spritesST.o dat_spritesPC.o ents.o e_bullet.o e_bomb.o e_rick.o e_sbonus.o e_them.o e_bonus.o e_box.o rects.o util.o game.o xrick.o draw.o maps.o sysvid.o syskbd.o control.o system.o scroller.o sysevt.o sysarg.o syssnd.o sysjoy.o dat_snd.o

#
# Rules
#
#

all: $(TARGET)

$(TARGET): $(OBJECTS) $(XOBJ)
	$(CC) $(OBJECTS) $(XOBJ) $(LDFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

xrick.res: xrick.rc
	windres $< -O coff -o $@

clean:
	rm -f *~ *.o core .depend $(TARGET)
	rm -f *.exe *.core *.res
	rm -f $(LINKS)

depend:
	for i in *.c; do $(CPP) $(CFLAGS) -I$(ROOTDIR)/include -MM $$i; done > .depend

# eof




