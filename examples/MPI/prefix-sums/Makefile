
CC=mpicc 
#CFLAGS= -Wall
# Use the following to enable debugging
CFLAGS= -Wall -g
LDFLAGS =
# Use the following to enable logging and visualization
#LDFLAGS = -llmpe -lmpe


all: parallel_prefix
	cd sequential/; make

parallel_prefix: parallel_prefix.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o a.out core parallel_prefix
	cd sequential/; make clean
