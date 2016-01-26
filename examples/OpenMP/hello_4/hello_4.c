
#include <stdio.h>
#include <omp.h>

int main(int argc, char **argv)
{
	int i=100; //this becomes a shared variable

#ifdef _OPENMP
	#pragma omp parallel
	{
		int c; //private to each thread
		c = omp_get_thread_num(); //OpenMP library call
		#pragma omp critical
		{ i++; }
	}
	printf("i = %d after parallel section\n", i);
#endif
}
