
CC=gcc
CFLAGS=-fopenmp

all: lb-demo-1 lb-demo-2

lb-demo-1: lb-demo-1.o
	$(CC) $(CFLAGS) -o $@ $< -lm

lb-demo-2: lb-demo-2.o
	$(CC) $(CFLAGS) -o $@ $< -lm

clean:
	/bin/rm -f lb-demo-1 lb-demo-2 *.o
