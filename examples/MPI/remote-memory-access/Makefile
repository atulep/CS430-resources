
CC=mpicc 
CFLAGS = -Wall
LDFLAGS =

all: cpi-rma

cpi-rma: cpi-rma.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o a.out core cpi-rma
