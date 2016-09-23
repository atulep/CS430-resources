DEBUG = 0
CC=mpicc
CFLAGS = -g -Wall
# The below params are for producing clog2 file.
# LDFLAGS =  -llmpe -lmpe -lpthread
RDFLAGS = -lprand

all: scatter gather reduce

scatter: scatter.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

gather: gather.o
	$(CC) $(CFLAGS) -o $@ $< $(RDFLAGS)

reduce: reduce.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o scatter gather reduce
