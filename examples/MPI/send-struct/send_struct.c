/**
*  Demonstrates how to create a custom MPI type to send a struct via send/recv
*/
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>  
#include <mpi.h>


int main(int argc, char **argv)
{
	int pid;
    int nproc;
	int i;
	const int TEST_TAG = 111; // arbitrary number

	struct test {
		int n;
		double x[3];
		double y[3];
	};


  	/* Start up MPI */
  	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);

	const int nitems = 3;
	int blocklengths[3] = {1, 3, 3}; //lengths of i, x and y as #items
	MPI_Datatype types[3] = {MPI_INT, MPI_DOUBLE, MPI_DOUBLE};
	MPI_Aint offsets[3];
	MPI_Datatype mpi_test_type;

	offsets[0] = offsetof(struct test, n); // offsetof is a std C function declared in stddef.h
	offsets[1] = offsetof(struct test, x);
	offsets[2] = offsetof(struct test, y);

	if (MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_test_type) != MPI_SUCCESS) {
		perror("Could not create MPI data type for struct test. Bailing out!\n");
		exit(1);
	}
	MPI_Type_commit(&mpi_test_type);


	if (pid == 0) {
		struct test test1;
		test1.n = 1;
		for (i = 0; i < 3; i++) {
			test1.x[i] = i;
			test1.y[i] = i;
		}
		int dest = 1; 
		/*char buf[sizeof(struct test)];*/
		/*memcpy(buf, &test1);*/
		MPI_Send(&test1, 1, mpi_test_type, dest,  TEST_TAG, MPI_COMM_WORLD);
		printf("Process %d: sent struct test\n", pid);
	} else if (pid == 1) {
		MPI_Status status;
		const int src = 0;
		struct test test2;

		MPI_Recv(&test2, 1, mpi_test_type, src, TEST_TAG, MPI_COMM_WORLD, &status);
		printf("process %d: recvd struct test:\n", pid);
		printf(" test2.n = %d\n", test2.n);
		printf(" test2.x[0] = %f test2.x[1] = %f test2.x[2] = %f\n", test2.x[0], test2.x[1], test2.x[2]);
		printf(" test2.y[0] = %f test2.y[1] = %f test2.y[2] = %f\n", test2.y[0], test2.y[1], test2.y[2]);
	}

  	/* Program finished. Exit MPI*/
  	MPI_Finalize();
	exit(0);
}

