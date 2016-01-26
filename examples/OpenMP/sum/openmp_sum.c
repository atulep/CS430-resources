
/*
	A multithreaded parallel sum using OpenMP
	The standard malloc isn't very scalable with respect to threads, so
	we replace it with jemalloc. Install with

	yum install jemalloc jemalloc-devel

	and then we may be able to get better speedup....
*/
#include <stdlib.h>
#include <jemalloc/jemalloc.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main(int argc, char **argv) 
{
	int i;
	int n;
	int *array;
	long int sum = 0;

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <n>\n", argv[0]);
		exit(1);
	}
	n = atoi(argv[1]);
	printf("Number of elements to add = %d\n", n);
 	array = (int *) malloc(sizeof(int)*n);

	#pragma omp parallel for
	for (i=0; i<n; i++)
		array[i] = 1;

	/*#pragma omp parallel for reduction(+:sum) schedule(static, 100000)*/
	#pragma omp parallel for reduction(+:sum) 
	for (i=0; i<n; i++) {
		sum +=  array[i];
	}

	printf(" sum =  %ld\n", sum);
	exit(0);

}

