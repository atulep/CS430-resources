
CC=gcc
CFLAGS=-Wall -O
#CFLAGS=-g  -Wall
OBJS=monte_carlo_pi.o timing.o

smcpi: monte_carlo_pi.o timing.o
	$(CC) $(CFLAGS) -o $@ $(OBJS)


clean:
	/bin/rm -f *.o smcpi

