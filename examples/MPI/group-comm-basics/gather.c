/*
 * Example illustrating MPI's gather operation.
 *
 * Written by Aza Tulepbergenov, CS 430, Fall 2016
 */


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <mpi.h>
#include <prand.h>

#define prog_name argv[0]
#define angry_face "\\(>_<)/"

#define SEED 7777777


/*
 * main expects two args in the following order: element_per_proc, root.
 */
int main(int argc, char** argv)
{
	if (argc != 3)
	{
		printf("%s\nusage: %s num_per_proc root\n", angry_face, prog_name);
		exit(1);
	}

	int nproc, who_am_i, num_per_proc, root;

	num_per_proc = atoi(argv[1]);
	root = atoi(argv[2]);

	/* Regular MPI routines */
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &who_am_i);

	int my_rand_nums[num_per_proc];

	// generate random numbers for my share
	srandom(SEED);
	unrankRand(who_am_i * num_per_proc);
	for (int i = 0; i < num_per_proc; i++)
	{
		my_rand_nums[i] = random();
	}

	/*
	MPI_Gather(void* send_data,
			int send_count,
			MPI_Datatype send_datatype,
			void* recv_data,
			int recv_count,
			MPI_Datatype recv_datatype,
			int root,
			MPI_Comm communicator)
	Note, recv_data must be NULL for everybody but the root.
	*/

	int* recv_data = NULL;

	if (who_am_i == root)
	{
		recv_data = malloc(sizeof(int) * nproc * num_per_proc);
		for (int i = 0; i < num_per_proc; i++)
		{
			recv_data[i] = my_rand_nums[i];
		}
	}

	printf("Gathering...\n");
	MPI_Gather(my_rand_nums, num_per_proc, MPI_INT, recv_data, num_per_proc,
		   MPI_INT, root, MPI_COMM_WORLD);

	if (recv_data) printf("Received on %d!\n", root);

	if (who_am_i == root)
	{
		printf("Here are the elements:\n");
		int n = nproc * num_per_proc;
		for (int i = 0; i < n; i++)
		{
			printf("%d ", recv_data[i]);
			if (i == n - 1) printf("\n");
		}
	}

	MPI_Finalize();
	if (recv_data) free(recv_data);
	exit(0);
}
