/* compute pi using Monte Carlo method */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <mpe.h>
#include <mpe_graphics.h>
#define CHUNKSIZE      25
#define PI 3.141592653589793238462643

/* We'd like a value that gives the maximum value returned by the function
   random, but no such value is *portable*.  RAND_MAX is available on many 
   systems but is not always the correct value for random (it isn't for 
   Solaris).  The value ((unsigned(1)<<31)-1) is common but not guaranteed */
#define INT_MAX 1000000000
#define MAX_TRIALS 10000000

#define WINDOW_SIZE 512


/* message tags */
#define REQUEST  1
#define REPLY    2

int main( int argc, char *argv[] )
{
    int iter;
    int in, out, i, max, ranks[1], done;
    double x, y, Pi, error, epsilon;
    int numprocs, myid, server, totalin, totalout, workerid;
    int rands[CHUNKSIZE], request;
    MPI_Comm world, workers;
    MPI_Group world_group, worker_group;
    MPI_Status status;
	MPE_XGraph graph;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <epsilon>\n", argv[0]);
		exit(1);
	}
    MPI_Init(&argc,&argv);
    world  = MPI_COMM_WORLD;
    MPI_Comm_size(world,&numprocs);
    MPI_Comm_rank(world,&myid);
    server = numprocs-1;	/* last proc is server */
	MPE_Open_graphics(&graph, MPI_COMM_WORLD, (char *)0, -1, -1, 
					WINDOW_SIZE, WINDOW_SIZE, MPE_GRAPH_INDEPENDENT);

    if (myid == 0)
        sscanf( argv[1], "%lf", &epsilon );
    MPI_Bcast( &epsilon, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD );
    MPI_Comm_group( world, &world_group );
    ranks[0] = server;
    MPI_Group_excl( world_group, 1, ranks, &worker_group );
    MPI_Comm_create( world, worker_group, &workers );
    MPI_Group_free(&worker_group);
    if (myid == server) {	/* I am the random number server */
		do {
	    	MPI_Recv(&request, 1, MPI_INT, MPI_ANY_SOURCE, REQUEST, world, &status);
	    	if (request) {
			for (i = 0; i < CHUNKSIZE; ) {
		        rands[i] = random();
				if (rands[i] <= INT_MAX) i++;
			}
			MPI_Send(rands, CHUNKSIZE, MPI_INT, status.MPI_SOURCE, REPLY, world);
	    	}
		}
		while( request>0 );
    } else {			/* I am a worker process */
        request = 1;
		done = in = out = 0;
		max  = INT_MAX;         /* max int, for normalization */
        MPI_Send( &request, 1, MPI_INT, server, REQUEST, world );
        MPI_Comm_rank( workers, &workerid );
		iter = 0;
		while (!done) {
	    	iter++;
	    	request = 1;
	    	MPI_Recv( rands, CHUNKSIZE, MPI_INT, server, REPLY, world, &status );
	    	for (i=0; i<CHUNKSIZE-1; ) {
	        	x = (((double) rands[i++])/max) * 2 - 1;
				y = (((double) rands[i++])/max) * 2 - 1;
				if (x*x + y*y < 1.0) {
		    		in++;
					MPE_Draw_point(graph, 
								   (int) (WINDOW_SIZE/2 + x * WINDOW_SIZE/2),
								   (int) (WINDOW_SIZE/2 + y * WINDOW_SIZE/2),
								   MPE_BLUE);
				} else {
		    		out++;
					MPE_Draw_point(graph, 
								   (int) (WINDOW_SIZE/2 + x * WINDOW_SIZE/2),
								   (int) (WINDOW_SIZE/2 + y * WINDOW_SIZE/2),
								   MPE_RED);
				}
	    	}

			MPE_Update(graph);
	    	MPI_Allreduce(&in, &totalin, 1, MPI_INT, MPI_SUM, workers);
    		MPI_Allreduce(&out, &totalout, 1, MPI_INT, MPI_SUM, workers);
    		Pi = (4.0*totalin)/(totalin + totalout);
    		error = fabs( Pi - PI);
    		done = (error < epsilon || (totalin+totalout) > MAX_TRIALS);
    		request = (done) ? 0 : 1;
    		if (myid == 0) {
				printf( "\rpi = %23.20f", Pi );
				MPI_Send( &request, 1, MPI_INT, server, REQUEST, world );
    		} else {
				if (request)
	    			MPI_Send(&request, 1, MPI_INT, server, REQUEST, world);
    		}
		}
		MPI_Comm_free(&workers);
    }

    if (myid == 0) {
        printf( "\npoints: %d\nin: %d, out: %d, <ret> to exit\n",
	       totalin+totalout, totalin, totalout );
		getchar();
    }
	MPE_Close_graphics(&graph);
    MPI_Finalize();
	exit(0);
}
