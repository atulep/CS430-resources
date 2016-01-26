DEBUG = 0
CC=mpicc 
CFLAGS = -O -Wall 
#LDFLAGS = -lmpe  -lm /usr/lib64/libX11.so.6
LDFLAGS = -lmpe  -lm -lX11

all: monte monte-anim

monte: monte.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

monte-anim: monte-anim.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o a.out core monte monte-anim
