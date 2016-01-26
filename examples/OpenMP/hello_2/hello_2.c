
#include <stdio.h>
#include <omp.h>

int main(int argc, char **argv)
{
	int i=100; //this becomes a shared variable

	omp_set_num_threads(4);
	#pragma omp parallel 
	{
		int c; //private to each thread
		c = omp_get_thread_num(); //OpenMP library call
		/* uncomment the following to see a race condition */
		 /*i++; */
		printf("i = %d on thread %d\n", i, c);
	}
}
