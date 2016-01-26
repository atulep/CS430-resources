
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>  
#include <prand.h>		//header files for the random number system
#include <mpi.h>


#define SEED   12345678		//All processes start with the same seed  
#define MAXHOSTNAMELEN 64
#define DEBUG 1


/* The main work routine for all processes */
void generateRandomNumbers(int me, long long int num_per_processor)  
{
	long long int startingPosition = 0;
	long long int i;
	long int x;

	srandom(SEED);
	startingPosition = me * num_per_processor; 
    unrankRand(startingPosition);	//Move forward to the correct starting position
    
    for (i = 0; i < num_per_processor; i++) {
		x = random();               
#ifdef DEBUG
		printf("%ld\n", x);
#endif
  	}
}


int main(int argc, char **argv)
{
    long long numRands = 0;
    long long numRandsPerNode = 0;      
	long long totNums = 0;
	long long count;
	char hostname[MAXHOSTNAMELEN];

    double timeStart = 0;
    double timeElapsed = 0;
	MPI_Status status;

	int i;
    int nproc;
	int remainder;
	int me;
	int who;
	const int JOB_COMPLETED = 1;

  	if (argc < 2) {
        fprintf(stderr, "Usage: %s <n>\n" ,argv[0]);
        exit(1);
    }
    sscanf(argv[1],"%lld",&numRands); /* lld for long long int */

  	/* Start up MPI */
  	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &me);

	gethostname(hostname, MAXHOSTNAMELEN);

	printf("Hello! I am %d of %d running on %s.\n", me, nproc, hostname);


	numRandsPerNode = numRands/nproc;
	remainder = numRands % nproc;
	/* divide remainder  numbers into remainder processes */
	if (me == nproc -1) {
		numRandsPerNode += remainder;
	}


	if (me == 0) {
    	timeStart = MPI_Wtime(); /* and we are off! */
	}
    generateRandomNumbers(me, numRandsPerNode);
#ifdef DEBUG
	printf("Process %d generated %lld random numbers\n", me, numRandsPerNode);
#endif

	if (me == 0) {
		printf("Process %d generated %lld random numbers\n", me, numRandsPerNode);
   		/* Wait for acknowledgemnt from process 1 ..nproc-1 */ 
    	for (i=1; i<nproc; i++) {
			MPI_Recv(&count, 1, MPI_LONG_LONG_INT, MPI_ANY_SOURCE, JOB_COMPLETED, MPI_COMM_WORLD, &status);
            who = status.MPI_SOURCE;
        	totNums += count;                 //Added to the results so far
			printf("Received message from process %d (generated %lld numbers)\n", who, count);
    	}
	} else {
     /* Send acknowledgemnt to process 0 of job completed */  
	 	MPI_Send(&numRandsPerNode, 1, MPI_LONG_LONG_INT, 0, JOB_COMPLETED, MPI_COMM_WORLD);
	}

	if (me == 0) {
    	timeElapsed = MPI_Wtime() - timeStart;
    	printf("Elapsed time:  %lf seconds\n",timeElapsed);
    	fflush(stdout);
	}

  	MPI_Finalize();
    exit(0);
}
