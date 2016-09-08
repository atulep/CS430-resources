DEBUG = 0
CC=mpicc 
CFLAGS = -g -Wall 
LDFLAGS =  -llmpe -lmpe -lpthread

all: token_ring_mpi

token_ring_mpi: token_ring_mpi.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o a.out core token_ring_mpi
