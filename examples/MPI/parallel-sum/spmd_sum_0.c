/*
 * spmd_sum_mpi_0.c: A SPMD style parallel sum program in MPI. This program
 * gives incorrect results on purpose to illustrate a common error in parallel
 * prgorams!
 *               
 * @author: Hongyi Hu, Amit Jain 
 *
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>  
#include <mpi.h>

const int PARTIAL_SUM = 1;

int add(int id, int n, int *data, int nproc)
{
	int i;

  	int low = id *(n/nproc);
  	int high = low +(n/nproc);
  	int sum = 0;
  	for(i=low; i<high; i++)
    	sum += data[i];

  	return(sum);
}


int main(int argc, char **argv)
{
    int i;
    int total = 0;                 /* total sum of data elements */
    int *partial_sums = NULL;
    int *data = NULL;
	int id;
	int n;
    int nproc;

	if (argc != 2) {
	  fprintf(stderr,"Usage: %s  <number of elements>\n", argv[0]); 
	  exit(1);
	} 
	n = atoi(argv[1]);      // number of data elements

	data = (int *) malloc(sizeof(int)*n);     // data array

  	/* Start up MPI */
  	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	if (id == 0) 
	{
  		/* Generate numbers to be summed up: we use all 1's to make it 
		   easy to verify the result.*/
  		for (i=0; i<n; i++)
			data[i] = 1;
	}

  	/* Broadcast initial data to other processes */
	if( MPI_Bcast( data, n, MPI_INT, 0,MPI_COMM_WORLD)!= MPI_SUCCESS)
	         fprintf(stderr, "Oops! An error occured in MPI_Bcast()\n");
     
  	int result = add(id, n, data, nproc);

	if (id == 0) {
  	        partial_sums = (int *)malloc(sizeof(int)*nproc);
		/* Process 0 gets its partial sum from local variable result */
		partial_sums[0] = result;
	} else {
  		/* Other processes send partial sum to the process 0 */ 	
		MPI_Send(&result, 1, MPI_INT, 0, PARTIAL_SUM, MPI_COMM_WORLD );
	}

	if (id == 0) {
		printf(" I got %d from %d\n", partial_sums[id], id); 
  		/* Wait for results from other processes */
  		for (i=0; i<nproc-1; i++) {
			MPI_Recv(&partial_sums[i+1], 1, MPI_INT, i+1, PARTIAL_SUM, 
											MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("I got %d from %d\n", partial_sums[i+1], i+1);
    	}

  		/* Compute the global sum */
  		for (i=0; i<nproc; i++)
    		total += partial_sums[i];
		printf("The total is %d\n", total);
		free(partial_sums);
	}

  	/* Program finished. Exit MPI*/
  	MPI_Finalize();
	free(data);
  	exit(0);
}

/* vim: set ts=4: */
