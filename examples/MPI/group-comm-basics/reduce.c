/*
 * Example illustrating MPI's reduce operation.
 * Program will first scatter numbers from 0 - nproc * element_per_proc-1 among
 * nproc processors. Then each processor will compute its own product and reduce
 * it to one final product at the root processor.
 *
 * Written by Aza Tulepbergenov, CS 430, Fall 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <mpi.h>

#define prog_name argv[0]
#define angry_face "\\(>_<)/"

/*
 * main expects two args in the following order: element_per_proc, root.
 */
int main(int argc, char** argv)
{
	int element_per_proc;
	int root;

	if (argc != 3)
	{
		printf("%s\nusage: %s element_per_processor, root\n", angry_face, prog_name);
		exit(1);
	}

	element_per_proc = atoi(argv[1]);
	root = atoi(argv[2]);

	int nproc, who_am_i;

	/* Initialize usual MPI routines */
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &who_am_i);

	int data[nproc*element_per_proc];

	for (int i = 0; i < sizeof(data)/sizeof(int); i++)
	{
		data[i] = i;
	}

	int recv_data[element_per_proc];

	if (who_am_i == root)
	{
		printf("Let the scattering begin. \\o/\n");
	}

	MPI_Scatter(data, element_per_proc, MPI_INT, recv_data, element_per_proc,
		    MPI_INT, root, MPI_COMM_WORLD);

	printf("Process %d received %d elements. They are:\n",
	       who_am_i, element_per_proc);

	for (int i = 0; i < element_per_proc; i++)
	{
		printf("%d ", recv_data[i]);
		if (i == element_per_proc - 1)
		{
			printf("\n");
		}
	}

	/*
	MPI_Reduce(void* send_data,
			void* recv_data,
			int count,
			MPI_Datatype datatype,
			MPI_Op op,
			int root,
			MPI_Comm communicator)
	*/

	long multiply_result;

	// will compute my result first.
	long my_result = 1;
	for (int i = 0; i < element_per_proc; i++) {
		if (recv_data[i] != 0) // otherwise, result will be 0!
			my_result *= recv_data[i];
	}

	// then reduces results of each of the processors in the group to a
	// single multiply_result.
	MPI_Reduce(&my_result, &multiply_result, 1,
		   MPI_LONG, MPI_PROD, root, MPI_COMM_WORLD);

	if (who_am_i == root)
	{
		printf("Result of multiplication is: %d\n", multiply_result);
	}

	MPI_Finalize();
	exit(0);
}
