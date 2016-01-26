CC=gcc 
CFLAGS= -O5 -Wall
DISPLAY_CFLAGS= -DDISPLAY=1
DISPLAY_LIBS=  -lglut -lGL -lGLU -lXmu -lXi -lXext -lX11 -lm -lpthread
    

all: slow_wave fast_wave ghost_wave ptr_wave

openmp:
	$(CC) $(CFLAGS) -fopenmp -o openmp_slow  slow_seq_wave.o $<  $(LIBS)
	$(CC) $(CFLAGS) -fopenmp -o openmp_fast  fast_seq_wave.o $<  $(LIBS)
	$(CC) $(CFLAGS) -fopenmp -o openmp_ghost  ghost_seq_wave.o $<  $(LIBS)
	$(CC) $(CFLAGS) -fopenmp -o openmp_ptr  ptr_seq_wave.o $<  $(LIBS)
	


slow_wave: slow_seq_wave.o 
	$(CC) $(CFLAGS) -o $@ $<  $(LIBS)

fast_wave: fast_seq_wave.o  
	$(CC)  $(CFLAGS) -o $@ $<  $(LIBS)

ghost_wave: ghost_seq_wave.o 
	$(CC)  $(CFLAGS) -o $@  $<  $(LIBS)

ptr_wave: ptr_seq_wave.o 
	$(CC)  $(CFLAGS) -o $@ $<   $(LIBS)

ptr_wave_display: clean ptr_seq_wave.c  gldisplay.c
	$(CC) $(CFLAGS) $(DISPLAY_CFLAGS) -c ptr_seq_wave.c 
	$(CC) $(CFLAGS) $(DISPLAY_CFLAGS) -c gldisplay.c 
	$(CC)  $(CFLAGS) $(DISPLAY_CFLAGS) -o $@ ptr_seq_wave.o gldisplay.o $(LIBS) $(DISPLAY_LIBS)


clean:
	/bin/rm -f slow_wave fast_wave ghost_wave ptr_wave ptr_wave_display  *.o 
