
#CC=pgcc
#CFLAGS=-mp -c9x
CC=gcc
CFLAGS=-fopenmp

pi: pi.o
	$(CC) $(CFLAGS) -o $@ $<

clean:
	/bin/rm -f pi *.o
