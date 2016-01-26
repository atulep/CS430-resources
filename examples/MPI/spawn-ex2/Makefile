
CC=mpicc 
CFLAGS = -Wall -g
LDFLAGS =

all: pcp pcp_slave 

pcp: pcp-spawn-master.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

pcp_slave: pcp-spawn-slave.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.o a.out core pcp pcp_slave
