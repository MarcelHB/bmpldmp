CC = gcc
OUTFILE = bmpldmp
CFLAGS =

main: main.c
	$(CC) $^ $(CFLAGS) -o $(OUTFILE)

default: main
