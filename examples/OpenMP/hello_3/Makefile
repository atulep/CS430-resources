
#CC=pgcc
#CFLAGS=-mp -c9x
CC=gcc
CFLAGS=-fopenmp

hello_3: hello_3.o
	$(CC) $(CFLAGS) -o $@ $<

clean:
	/bin/rm -f hello_3 *.o
