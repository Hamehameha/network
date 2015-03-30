CC = sh4-linux-gcc

export SYSROOT=$(shell readlink -f `$(CC) -print-prog-name=gcc` | sed -e s!/usr/sh4-linux-uclibc/.*!!)

CFLAGS = -Wall

objs = server client

all: $(objs)

taa:
	@echo $(SYSROOT)

clean:
	$(RM) $(objs)