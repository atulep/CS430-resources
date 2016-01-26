/*
 * lb_demo_mpi.c: A SPMD style MPI program to demonstrate barriers
 *
 * Author of MPI version: Hongyi Hu, Amit Jain 
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/types.h>
#include <mpi.h>


const int MAX_ITERATIONS = 20000000;
const int MIN_ITERATIONS = 10000;
const int CHECK_IN = 1;


int main(int argc, char **argv)
{
	int nproc;                 /* number of processes */
	int myId;                  /* my process number in old original group*/ 
	int myNewId;               /* my process number in new group*/
	int i, j;
	int *my_ranks;
	MPI_Group orig_group, new_group;
	MPI_Comm new_comm;
	MPI_Request req;

	/* Initialize MPI */
	MPI_Init(&argc, &argv);
	/* Find out how many processes there are in the default communicator */
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	/* Find out my identity in the default communicator */
	MPI_Comm_rank(MPI_COMM_WORLD, &myId);

	my_ranks = (int *)malloc(sizeof(int)*nproc);
	for(i=0; i<nproc; i++)
		my_ranks[i]=nproc-1-i;

	/* Declare the group all processes belong to */ 
	MPI_Comm_group(MPI_COMM_WORLD, &orig_group);
	/* Create the new group from orig_group and specifies member processes */
	MPI_Group_incl(orig_group, nproc, my_ranks, &new_group );
	/* Create a new communicator */
	MPI_Comm_create(MPI_COMM_WORLD, new_group, &new_comm);
	/* Return the rank of this process in the given group */
	MPI_Group_rank(new_group, &myNewId);
	printf("In new group, process %d has new process number %d\n", myId, myNewId);

	srandom(myId); // different seed for each process
     
	for (i=0; i<50; i++)
  	{
    	int iterations = random() % MAX_ITERATIONS + MIN_ITERATIONS;
    
    	if (myId == 0) {
			int load;

			for (j=0; j<nproc-1; j++) {
	  			MPI_Irecv( &load, 1, MPI_INT, MPI_ANY_SOURCE, CHECK_IN, MPI_COMM_WORLD, &req);
			}
    	} else {
		MPI_Issend(&iterations,1, MPI_INT, 0, CHECK_IN, MPI_COMM_WORLD, &req); 
    	}
   
    	MPI_Barrier(new_comm);
    	printf("process %d reached barrier number %d\n", myId, i);
  	}

	/* Leave the new group */
	MPI_Group_free(&new_group);

	/* Program finished, exit MPI */
	MPI_Finalize();
	exit(0);
}

