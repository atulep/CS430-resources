CC = g++ 
CFLAGS =-O -Wall
    

all: eratosthenes


eratosthenes: eratosthenes.o 
	$(CC) $(CFLAGS) -o $@ eratosthenes.o  $(LIBS)

clean:
	/bin/rm -f eratosthenes *.o 
