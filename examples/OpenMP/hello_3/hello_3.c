
#include <stdio.h>
#include <omp.h>

int main(int argc, char **argv)
{
	int id, numThreads;

	#pragma omp parallel private (id, numThreads)
	{
		id = omp_get_thread_num(); //OpenMP library call
		numThreads = omp_get_num_threads();
		printf("I am thread %d out of  %d threads\n", id, numThreads);
	}
}
