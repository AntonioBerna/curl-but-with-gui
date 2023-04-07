CC=gcc

CFLAGS=-Wall -Wextra -Wpedantic -std=c99 `pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0` -lcurl
ASMFLAGS=-S # for macbook M1: -arch i386

CTARGET=main
ASMTARGET=main.s

SRCS=main.c
OBJS=$(SRCS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) $< -c -o $@ $(LIBS)

all: $(OBJS)
	$(CC) $(CFLAGS) $< -o $(CTARGET) $(LIBS)

.PHONY: clean asm

asm: 
	$(CC) $(CFLAGS) $(ASMFLAGS) $(SRCS) -o $(ASMTARGET) $(LIBS)

clean:
	$(RM) $(CTARGET) $(ASMTARGET) $(OBJS)
