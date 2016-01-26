#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

int main(int argc, char **argv)
{
	int i, j;
	int iterations;
	int value;
	const int MAX_ITERATIONS = 1000000;
	const int MAX_ROUNDS = 50;

	printf("\n\nstatic\n\n");

#pragma omp parallel for private (value, j, iterations) schedule(static, 5)
	for (i=0; i<MAX_ROUNDS; i++) {
		value = 0;
		for (j=0; j<MAX_ITERATIONS; j++) {
			value += (int) sqrt(j) % 11;
		}
		printf("finished round %d on thread %d\n", i, omp_get_thread_num());
	}

	printf("\n\ndynamic\n\n");

#pragma omp parallel for private (value, j, iterations) schedule(dynamic, 5)
	for (i=0; i<MAX_ROUNDS; i++) {
		value = 0;
		for (j=0; j<MAX_ITERATIONS; j++) {
			value += (int) sqrt(j) % 11;
		}
		printf("finished round %d on thread %d\n", i, omp_get_thread_num());
	}

	printf("\n\nguided\n\n");

#pragma omp parallel for private (value, j, iterations) schedule(guided, 5)
	for (i=0; i<MAX_ROUNDS; i++) {
		value = 0;
		for (j=0; j<MAX_ITERATIONS; j++) {
			value += (int) sqrt(j) % 11;
		}
		printf("finished round %d on thread %d\n", i, omp_get_thread_num());
	}

	exit(0);
}
