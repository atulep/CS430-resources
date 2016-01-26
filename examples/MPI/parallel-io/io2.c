/* example of parallel MPI write into separate files */
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define BUFSIZE 1024
/*#define DEBUG 1*/

int main(int argc, char *argv[])
{
    int i, myrank, buf[BUFSIZE];
    char *filename;
    MPI_File myfile;
	int count;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s <count> <output file>\n", argv[0]);
		exit(1);
	}
	count = atoi(argv[1]);
	filename = (char *) malloc(1024); 

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    for (i=0; i<BUFSIZE; i++)
        buf[i] = myrank * BUFSIZE + i;
    sprintf(filename, "%s.%d",  argv[2], myrank);
    MPI_File_open(MPI_COMM_SELF, filename, MPI_MODE_WRONLY | MPI_MODE_CREATE,
		  MPI_INFO_NULL, &myfile);

	for (i=0; i<count; i++) {
#ifdef DEBUG
		printf("Wrote round %d\n", i);
#endif
    	MPI_File_write(myfile, buf, BUFSIZE, MPI_INT, MPI_STATUS_IGNORE);
	}
    MPI_File_close(&myfile);
    MPI_Finalize();
    return 0;
}
