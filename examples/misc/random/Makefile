CC = gcc 
CFLAGS =-O -Wall
    

all: test-random get-random-seed


test-random: test-random.o 
	$(CC) $(CFLAGS) -o $@ test-random.o  $(LIBS)

get-random-seed: get-random-seed.o  
	$(CC)  $(CFLAGS) -o $@ get-random-seed.o $(LIBS)

clean:
	/bin/rm -f test-random get-random-seed *.o 
