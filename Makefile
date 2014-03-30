#
# xrick/Makefile
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
# Vars
#

SDLVERSION=$(shell sdl-config --version 2>/dev/null)
ROOTDIR=$(shell pwd)
TARGET=$(shell uname -s | tr [a-z] [A-Z])

#
# Config
#

ifeq ($(strip $(SDLVERSION)),) 
$(error SDL is missing) 
else 
$(warning Detected SDL version $(SDLVERSION)) 
endif

ifeq ($(strip $(SDLVERSION)),)
$(error SDL is missing)
endif

SDL_MAJ=$(word 1,$(subst ., ,$(SDLVERSION)))
SDL_MIN=$(word 2,$(subst ., ,$(SDLVERSION)))
SDL_MIC=$(word 3,$(subst ., ,$(SDLVERSION)))

SDL_MAJ_REQ=1
SDL_MIN_REQ=2
SDL_MIC_REQ=1

SDL_CHKVER=$(shell if [ $(SDL_MAJ) -lt $(SDL_MAJ_REQ) ]; then echo "BAD"; fi)
ifeq ($(SDL_CHKVER),BAD)
$(error SDL version $(SDL_MAJ_REQ).$(SDL_MIN_REQ).$(SDL_MIC_REQ) is required)
endif

SDL_CHKVER=$(shell if [ $(SDL_MAJ) -eq $(SDL_MAJ_REQ) -a $(SDL_MIN) -lt $(SDL_MIN_REQ) ]; then echo "BAD"; fi)
ifeq ($(SDL_CHKVER),BAD)
$(error SDL version $(SDL_MAJ_REQ).$(SDL_MIN_REQ).$(SDL_MIC_REQ) is required)
endif

SDL_CHKVER=$(shell if [ $(SDL_MAJ) -eq $(SDL_MAJ_REQ) -a $(SDL_MIN) -eq $(SDL_MIN_REQ) -a $(SDL_MIC) -lt $(SDL_MIC_REQ) ]; then echo "BAD"; fi)
ifeq ($(SDL_CHKVER),BAD)
$(error SDL version $(SDL_MAJ_REQ).$(SDL_MIN_REQ).$(SDL_MIC_REQ) is required)
endif

ifneq (,$(findstring CYGWIN,$(TARGET)))
XOBJ=xrick.res
endif

ifneq (,$(findstring MINGW,$(TARGET)))
XOBJ=xrick.res
endif

#
# Rules
#

all:
	@echo "ROOTDIR=$(ROOTDIR)" > Makefile.global
	@echo "XOBJ=$(XOBJ)" >> Makefile.global
	@echo "CFLAGS=-g -ansi -pedantic -Wall -W -O2 -I $(ROOTDIR)/include $(shell sdl-config --cflags)" >> Makefile.global
	@echo "LDFLAGS=-lz $(shell sdl-config --libs)" >> Makefile.global
	@echo "CC=gcc" >> Makefile.global
	@echo "CPP=gcc -E" >> Makefile.global
	$(MAKE) -C src all

clean:
	for i in src include; do \
	  $(MAKE) -C $$i clean; \
	done
	rm -f *~ log.txt Makefile.global

depend:
	$(MAKE) -C src depend

# eof
