/*
 * Example illustrating MPI's scatter operation.
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

	/* MPI_Scatter(end_data,
	 *		int send_count,
			MPI_Datatype send_datatype,
			void* recv_data,
			int recv_count,
			MPI_Datatype recv_datatype,
		        int root,
			MPI_Comm communicator)
	*/

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

	MPI_Finalize();
	exit(0);
}
