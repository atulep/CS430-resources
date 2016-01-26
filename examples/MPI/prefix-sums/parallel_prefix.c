

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


const int PARTIAL_SUM = 1;
const int PREVIOUS_SUM = 2;

int *initialize_input_array(int size)
{
	int i;
	int *array;
	
	array = (int *) malloc(sizeof(int)*size);
	for (i=0; i<size; i++)
		array[i] = 1;
	return array;
}

void parallel_prefix(int *input, int share, int p, int me)
{
	int i;
	int *temp;
	int source;
	int value, prevsum;
	MPI_Status status;

	fprintf(stderr, "p=%d, me=%d, share=%d\n", p, me, share);
	
	// each process does its share of prefix sums
	for (i=1; i<share; i++)
		input[i] = input[i-1] + input[i];
		
	
	if (me != 0) {
		fprintf(stderr, "Process %d sending value %d\n", me, input[share-1]);
		MPI_Send(&input[share-1], 1, MPI_INT, 0, PARTIAL_SUM, MPI_COMM_WORLD);
	} else {
		temp = (int *) malloc(sizeof(int)*p);
		temp[0] = input[share-1];
		for (i=1; i<p; i++) {
			MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, PARTIAL_SUM, MPI_COMM_WORLD, &status);
            source = status.MPI_SOURCE;
            temp[source] = value;
            fprintf(stderr, "I got %d from %d\n", temp[source], source);
			temp[source] = value;
		}
	}


	if (me == 0) {
		fprintf(stderr, "Process %d computing partial sums of sums\n", me);
		for (i=1; i<p; i++)
			temp[i] = temp[i-1] + temp[i];
		for (i=0; i<p-1; i++)
			MPI_Send(&temp[i], 1, MPI_INT, i+1, PREVIOUS_SUM, MPI_COMM_WORLD);		
	} else {
		MPI_Recv(&prevsum, 1, MPI_INT, 0, PREVIOUS_SUM, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		fprintf(stderr, "Process %d updating its share of partial sums with prevsum=%d\n", me, prevsum);
		for (i=0; i<share; i++)
			input[i] = input[i] + prevsum;

	}

	if (me == 0) {
		free(temp);
	}
	
	if (share < 100) {
		for (i=0; i<share; i++)
			printf("%d ", input[i]);
		printf("\n");
	}
}


int main(int argc, char **argv)
{
	int *input;
	int n;
	int share;
	double startTime, totalTime;
    int me, nproc;



	if (argc != 2) {
		fprintf(stderr, "Usage: %s <n>\n", argv[0]);
		exit(1);
	}

	n = atoi(argv[1]);


    /* Start up MPI */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);
    
	share = n/nproc;
	if (me == nproc - 1) share = share + n % nproc;
	printf("n=%d nproc=%d share=%d\n", n, nproc, share);
	
	input = initialize_input_array(share);

	startTime = MPI_Wtime();
	parallel_prefix(input, share, nproc, me);
	MPI_Barrier(MPI_COMM_WORLD);
	totalTime = MPI_Wtime() - startTime;
	if (me == 0) printf("Process %d: time taken = %lf seconds\n", me, totalTime);

	MPI_Finalize();
	exit(0);
}
