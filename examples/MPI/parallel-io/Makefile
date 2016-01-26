
CC=mpicc 
CFLAGS = -Wall -O
#LDFLAGS =  -llmpe -lmpe

all: io1 io2 io3 io4

io1: io1.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

io2: io2.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

io3: io3.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

io4: io4.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o a.out core io1 io2 io3 io4 testfile*
