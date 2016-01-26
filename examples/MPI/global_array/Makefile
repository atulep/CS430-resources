# Makefile for building 2d-percolation code with Global Array support.
#
#

GA_PREFIX = /usr/local/ga
# GA_PREFIX = /home/lhindman/install/ga-4-0-1
MPI_PREFIX = /usr/local/mpich2
MPI_INCLUDE=$(MPI_PREFIX)/include
MPI_LIB=$(MPI_PREFIX)/lib
LIBMPI= -lmpich -lpthread
USE_MPI = yes

TARGET = LINUX
FC = "$(MPI_PREFIX)/bin/mpif77 -echo"
CC = $(MPI_PREFIX)/bin/mpicc
LIB_DISTRIB = $(GA_PREFIX)/lib
INCLUDES = -I$(GA_PREFIX)/include -I$(MPI_PREFIX)/include

include $(GA_PREFIX)/armci/config/makecoms.h
include $(GA_PREFIX)/armci/config/makemp.h
include $(GA_PREFIX)/config/makefile.h

ifndef LIBDIR
	LIBDIR = $(LIB_DISTRIB)/$(TARGET)
endif
LIBS = -L$(LIBDIR) -lm -lglobal -lma -llinalg -larmci -L$(MPI_LIB) -ltcgmsg-mpi -lm $(LIBMPI)
# LIBS = -L$(LIBDIR) -lm -lglobal -lma -larmci -L$(MPI_LIB) -ltcgmsg-mpi -lm $(LIBMPI)



CC=gcc
# CFLAGS= -pg  -O  -Wall
CFLAGS=  -O  -Wall $(INCLUDES) 
# LFLAGS= -pg -lm 
LFLAGS= $(LIBS)
OBJS=ga-mpi.o

all: ga-mpi


# I had to switch this to use mpif77 because the global arrays
# 	library contains fortran code and this is required to link it 
#	properly. It detects the c code and calls gcc to compile the 
#	.o files.  Then f77 is called to link the object code and 
#	libraries together.
ga-mpi: $(OBJS) 
	$(FC) $(CFLAGS) -o $@ $(OBJS)   $(LFLAGS)

clean:
	rm -f *.o ga-mpi *~

