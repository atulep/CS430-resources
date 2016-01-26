#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/* example of parallel MPI write into a single file 
 * Bug in NFS lock causes a hang, try on PVFS file system
 */
#define BUFSIZE 1024
/*#define DEBUG 1*/

int main(int argc, char *argv[])
{
    int i, myrank, buf[BUFSIZE];
	int count, nprocs;
    char *filename;
    MPI_File thefile;
    MPI_Offset offset;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);

	if (argc < 3) {
		fprintf(stderr, "Usage: %s <count> <output file>\n", argv[0]);
		exit(1);
	}
	count = atoi(argv[1]);
	filename = argv[2];

    for (i = 0; i < BUFSIZE; i++)
        buf[i] = myrank * BUFSIZE + i;
    MPI_File_open(MPI_COMM_WORLD, filename, (MPI_MODE_WRONLY | MPI_MODE_CREATE),
		  MPI_INFO_NULL, &thefile);
    MPI_File_set_view(thefile, 0, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);


    offset = myrank * BUFSIZE;
	for (i=0; i<count; i++) {
#ifdef DEBUG
		printf("Wrote round %d\n", i);
#endif
    	MPI_File_write_at(thefile, offset, buf, BUFSIZE, MPI_INT, MPI_STATUS_IGNORE);
		offset += nprocs*BUFSIZE;

	}
    MPI_File_close(&thefile);
    MPI_Finalize();
    return 0;
}
