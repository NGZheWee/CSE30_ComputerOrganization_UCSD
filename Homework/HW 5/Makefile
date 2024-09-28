#
# CSE30 Makefile
#
# we declare the object we want to build in the variable EXE
# we list the objects we want to compile (from c) in OBJS
#
# we provide a phony target clean to rm all the .o files
#

EXE = bannerbuild
OBJS = main.o fontBuf.o displayBuf.o
CC = gcc
DEBUG = -g
WARN = -Wall -Wextra
CFLAGS = -std=c11 -I. $(DEBUG) $(WARN)

.PHONY: clean

$(EXE) : $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(OBJS) : globals.h

clean :
	rm -f $(EXE) $(OBJS)
