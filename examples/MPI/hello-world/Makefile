
CC=mpicc 
CFLAGS= -Wall
# Use the following to enable debugging
#CFLAGS= -Wall -g
LDFLAGS = 
# Use the following to enable logging and visualization
#LDFLAGS = -llmpe -lmpe


all: spmd_hello_world

spmd_hello_world: spmd_hello_world.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o a.out core spmd_hello_world
