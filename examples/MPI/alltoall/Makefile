
CC=mpicc 
CFLAGS = -Wall  
#uncomment the following to enable logging and visualization
#LDFLAGS = -llmpe -lmpe

all: time_alltoallv


time_alltoallv: time_alltoallv.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f time_alltoallv *.o a.out core 
