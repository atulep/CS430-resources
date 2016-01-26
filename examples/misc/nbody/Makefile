CC		= 	gcc
CFLAGS		=	-O3 -Wall -I.
LIBS		=	$(PVMLIB) $(ARCHLIB)
LIBS = -L.  -lg2 -L/usr/X11R6/lib -lX11 -lgd -lm -lpthread

all: seqNbody2

seqNbody2: seqNbody2.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS) $(LIBS) $(LDFLAGS)

clean:
	rm -f *.o seqNbody2

