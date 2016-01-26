
CC=gcc
CFLAGS=-Wall -O
#CFLAGS=-g -gstabs+ -Wall
OBJS=seq_prefix_sums.o timing.o

seq_prefix_sums: seq_prefix_sums.o timing.o
	$(CC) $(CFLAGS) -o $@ $(OBJS)


clean:
	/bin/rm -fr *.o seq_prefix_sums LINUXI386/

