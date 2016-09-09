
CC=mpicc 
CFLAGS = -Wall   -g
#uncomment the following to enable logging and visualization
#LDFLAGS = -L/usr/lib -lpthread -llmpe -lmpe

PROGS=exchange

all: $(PROGS)



exchange: exchange.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)


clean:
	rm -f *.o a.out core $(PROGS) 
