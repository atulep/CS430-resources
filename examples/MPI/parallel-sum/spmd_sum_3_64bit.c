/*
 spmd_sum_3.c: A SPMD style parallel sum program in MPI, it has each 
 process generate its share of numbers independently. 
                
 Author: Hongyi Hu, Amit Jain
 
*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>  
#include <mpi.h>

const int PARTIAL_SUM = 1;

int add(int id, int n, int *data, int nproc, int share)
{
	int i;
 	int sum = 0;

  	for(i=0; i<share; i++)
	  sum += data[i];

  	return(sum);
}
  


int main(int argc, char **argv)
{
    int i;
    long long total = 0;                 /* total sum of data elements */
    long long *partial_sums = NULL;
    int *data;
	int id;
	long long n;
    int nproc;
	long long share;
    long long tempsum; 
	int source;
    MPI_Status status;
	double startTime, totalTime;

  	if (argc != 2) {
		fprintf(stderr,"Usage: %s  <number of elements>\n",argv[0]); 
		exit(1);
	} 
	n = atoll(argv[1]);      // number of data elements

  	/* Start up MPI */
  	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);

	share = n/nproc;
	if (id == nproc-1) share += n%nproc;
	data = (int *) malloc(sizeof(int)*share);     // data array

  	for (i=0; i<share; i++)
	  data[i] = 1;
	
	startTime = MPI_Wtime();
  	long long result = add(id, n, data, nproc, share);

	if (id == 0) {
  	        partial_sums = (long long *)malloc(sizeof(long long)*nproc);
		/* Process 0 gets its partial sum from local variable result */
		partial_sums[0] = result;
	} else {
  		/* Other processes send partial sum to the process 0 */ 	
		MPI_Send(&result, 1, MPI_LONG, 0, PARTIAL_SUM, MPI_COMM_WORLD );
	}

	if (id == 0) {
		printf(" I got %lld from %d\n", partial_sums[id], id); 
  		/* Wait for results from other processes */
  		for (i=0; i<nproc-1; i++) {
			MPI_Recv(&tempsum, 1, MPI_LONG, MPI_ANY_SOURCE, PARTIAL_SUM, MPI_COMM_WORLD, &status);
            source = status.MPI_SOURCE;
            partial_sums[source] = tempsum;
            printf("I got %lld from %d\n", partial_sums[source], source);
    	}

  		/* Compute the global sum */
  		for (i=0; i<nproc; i++)
    		total += partial_sums[i];
		totalTime = MPI_Wtime() - startTime;
		printf("The total is %lld. Time taken = %lf seconds\n", total, totalTime);
	}

  	/* Program finished. Exit MPI */
  	MPI_Finalize();
  	exit(0);
}

/* vim: set ts=4: */
