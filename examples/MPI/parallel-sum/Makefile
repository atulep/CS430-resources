
CC=mpicc 
CFLAGS = -Wall   -g
#CFLAGS = -Wall   -O3
#uncomment the following to enable logging and visualization
#LDFLAGS = -llmpe -lmpe

PROGS=seq_sum spmd_sum_0 spmd_sum_1 spmd_sum_2 spmd_sum_3 spmd_sum_3_64bit

all: $(PROGS)


seq_sum: seq_sum.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

spmd_sum_0: spmd_sum_0.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

spmd_sum_1: spmd_sum_1.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

spmd_sum_2: spmd_sum_2.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

spmd_sum_3: spmd_sum_3.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

spmd_sum_3_64bit: spmd_sum_3_64bit.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o a.out core $(PROGS) spmd_sum*clog2 spmd_sum*slog2 
