DEBUG = 0
CC=mpicc 
CFLAGS = -O -Wall 
LDFLAGS =

all: file_writing_mpi

file_writing_mpi: file_writing_mpi.o timing.o
	$(CC) $(CFLAGS) -o $@ $< timing.o  $(LDFLAGS)

clean:
	rm -f *.o a.out core file_writing_mpi out.*
