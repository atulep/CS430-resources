
CC=mpicc 
#CFLAGS= -Wall -O
# Use the following to enable debugging
CFLAGS= -Wall -g
LDFLAGS = 
# Use the following to enable logging and visualization
#LDFLAGS = -llmpe -lmpe


all: send_struct

send_struct: send_struct.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o a.out core send_struct
