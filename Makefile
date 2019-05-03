CC=gcc
CFLAGS=-O3 -Wall -I/homes/tdettbar/local/include -DBIGENDIAN=0
LDFLAGS=-L/homes/tdettbar/local/lib
LIBS=-lncurses

CFILES=ui.c gpl.c main.c 
HFILES=ui.h gpl.h data.h
OFILES=ui.o gpl.o main.o
all:	nMicrocoder

nMicrocoder:	$(OFILES) $(HFILES)
		$(CC) $(LDFLAGS) -o$@ $(OFILES) $(LIBS)

.c.o:
		$(CC) $(CFLAGS) $< -c -I. $(OPTIONS)

clean:
		rm -f nMicrocoder $(OFILES)
