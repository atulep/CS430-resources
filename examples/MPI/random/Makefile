
CC=mpicc 
CFLAGS= -Wall -O
# Use the following to enable debugging
#CFLAGS= -Wall -g
LDFLAGS= -lprand
# Use the following to enable logging and visualization
#LDFLAGS = -llmpe -lmpe -lpthread


all: seq_random random

seq_random:
	cd sequential/; make

random: random.o timing.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

prand-test: prand-test.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o a.out core random prand-test
	cd sequential/; make clean
