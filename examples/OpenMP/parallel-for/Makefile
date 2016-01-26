
CC=gcc
CFLAGS=-fopenmp -g
LFLAGS=-lm

all: ex1 ex2 

ex1: ex1.o
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

ex2: ex2.o
	$(CC) $(CFLAGS) -o $@ $< $(LFLAGS)

clean:
	/bin/rm -f ex1 ex2 *.o
