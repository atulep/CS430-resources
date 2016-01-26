
#CC=pgcc
#CFLAGS=-mp -c9x
CC=gcc
CFLAGS=-fopenmp

hello_4: hello_4.o
	$(CC) $(CFLAGS) -o $@ $<

clean:
	/bin/rm -f hello_4 *.o
