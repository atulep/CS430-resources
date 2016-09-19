CC = gcc
EXEC = mandSEQ
OBJS = timer.o
H_FILE = timer.h
MATHFLAG = -lm
SEQFLAGS = -O3

all: $(EXEC)

mandSEQ: mandelbrot.c $(OBJS) $(H_FILE)
	$(CC) $(SEQFLAGS) $(MATHFLAG) -o $@ mandelbrot.c $(OBJS)


timer.o: timer.c
	$(CC) $(SEQFLAGS) -o $@ -c timer.c

clean: 
	rm -f *.o *.pgm $(OBJS) $(EXEC)
