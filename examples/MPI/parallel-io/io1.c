/* example of sequential Unix write into a common file */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define BUFSIZE 1024
#define BUFFER_TAG 99
/*#define DEBUG 1*/

int main(int argc, char *argv[])
{
    int i, j, myrank, numprocs, buf[BUFSIZE];
	int count;
    MPI_Status status;
	char *filename;
    FILE *myfile = NULL;


    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	if (argc < 3) {
		fprintf(stderr, "Usage: %s <count> <output file>\n", argv[0]);
		exit(1);
	}
	count = atoi(argv[1]);
	filename = argv[2];

    for (i=0; i<BUFSIZE; i++)	
		buf[i] = myrank * BUFSIZE + i;
	if (myrank == 0)
		myfile = fopen(filename, "w");

	for (i=0; i<count; i++) {
    	if (myrank != 0)
			MPI_Send(buf, BUFSIZE, MPI_INT, 0, BUFFER_TAG, MPI_COMM_WORLD);
    	else {
#ifdef DEBUG
			printf("Round %d\n", i);
#endif
			fwrite(buf, sizeof(int), BUFSIZE, myfile);
			for (j=1; j<numprocs; j++) {
	    		MPI_Recv(buf, BUFSIZE, MPI_INT, j, BUFFER_TAG, MPI_COMM_WORLD, &status);
	    		fwrite(buf, sizeof(int), BUFSIZE, myfile);
			}
    	}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	if (myrank == 0)
		fclose(myfile);

    MPI_Finalize();
    return 0;
}
