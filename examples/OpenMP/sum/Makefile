
CC=gcc
CFLAGS=-fopenmp -O

all: serial_sum openmp_sum 

serial_sum: openmp_sum.c
	$(CC) -o $@ -O openmp_sum.c

openmp_sum: openmp_sum.o
	$(CC) $(CFLAGS) -o $@ $<

clean:
	/bin/rm -f openmp_sum serial_sum *.o
