/*
 A SPMD style Hello World MPI program.  
                
 author: Hongyi Hu
 author: Amit Jain
 
 email: ajain@boisestate.edu
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>  
#include <mpi.h>

#define MAXHOSTNAMELEN 64


int main(int argc, char **argv)
{
	int pid;
    int nproc;
	char hostname[MAXHOSTNAMELEN];

  	/* Start up MPI */
  	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);

	gethostname(hostname, MAXHOSTNAMELEN);

	printf("Hello! I am %d of %d running on %s.\n", pid, nproc, hostname);

  	/* Program finished. Exit MPI*/
  	MPI_Finalize();
	exit(0);
}

