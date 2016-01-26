ALL: default
##### User configurable options #####

MPI_CC      = mpicc
MPI_CLINKER = $(MPI_CC)
#
MPI_INC     = 
MPI_LIBS    = 
CLFGAS      = 
MPE_GRAPH   = @MPE_GRAPHICS@
MPE_INCDIR  = /usr/local/src/mpich2-1.0.4p1/src/mpe2/include
MPE_LIBDIR  = /usr/local/src/mpich2-1.0.4p1/src/mpe2/lib
PROF_LIBS   = -L$(MPE_LIBDIR) -lmpe -lX11 $(MPI_LIBS) -lm
#
INCLUDE_DIR = -I$(MPE_INCDIR) $(MPI_INC)
LDFLAGS     =  -lpthread

### End User configurable options ###

RM          = rm

VPATH=.:$(srcdir)

srcdir      = .

CC_FLAGS    = $(CFLAGS) $(INCLUDE_DIR) $(MPE_COPTS)  -DHAVE_STDLIB_H=1
CC_LDFLAGS  = $(LDFLAGS) $(CFLAGS) $(MPE_LDOPTS)
EXECS       = pmandel

.SUFFIXES: .c .o 

.c.o:
	$(MPI_CC) $(CC_FLAGS) -c $<

default: $(EXECS)

fract_gen.o: fract_gen.h pmandel.h

pm_genproc.o: pmandel.h fract_gen.h

pmandel.o: pmandel.h fract_gen.h lists.h pm_genproc.h

pm_separate_rect.o: pmandel.h pm_genproc.h

pmandel: pmandel.o pm_genproc.o fract_gen.o pm_separate_rect.o lists.o args.o
	$(MPI_CLINKER) $(CC_LDFLAGS) -o $@ $? $(PROF_LIBS)

clean:
	@-$(RM) -f *.o *~ PI* $(EXECS) *.log *.out *.ln

distclean: clean
	@-$(RM) -f Makefile

lint:
	lint -chv $(CFLAGS) $(MPE_GRAPH) $(INCLUDE_DIR) pmandel.c \
	          pm_genproc.c fract_gen.c pm_separate_rect.c lists.c args.c
