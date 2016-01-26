
CC=mpicc 
CFLAGS = -Wall -g
LDFLAGS =

all: coordinator worker 

coordinator: spawn-coordinator.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

worker: spawn-worker.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o a.out core coordinator worker
