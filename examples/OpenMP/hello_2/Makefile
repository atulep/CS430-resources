
#CC=pgcc
#CFLAGS=-mp -c9x
CC=gcc
CFLAGS=-fopenmp 

hello_2: hello_2.o
	$(CC) $(CFLAGS) -o $@ $<

clean:
	/bin/rm -f hello_2 *.o
