DEBUG = 0
CC=mpicc 
CFLAGS = -O -Wall 
#LDFLAGS = -llmpe -lmpe -lm
LDFLAGS =  -lmpe -lm

all: barrier_demo_mpi lb_demo_mpi

barrier_demo_mpi: barrier_demo_mpi.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

lb_demo_mpi: lb_demo_mpi.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o a.out core lb_demo_mpi barrier_demo_mpi
