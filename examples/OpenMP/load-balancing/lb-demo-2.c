#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


/*
 
 Illustrates how dynamic load balancing works. Run this code as follows:
 
 make
 lb-demo-2 > log
 grep "thread 0" log | wc -l
 grep "thread 1" log | wc -l
 grep "thread 2" log | wc -l
 grep "thread 3" log | wc -l

*/

int main(int argc, char **argv)
{
	int i, j;
	int iterations;
	int value;
	const int MAX_ITERATIONS = 10000000;
	const int MAX_ROUNDS = 200;


#pragma omp parallel for private (value, j, iterations) schedule(dynamic, 4)
	for (i=0; i<MAX_ROUNDS; i++) {
		iterations = (random() % 4) * (omp_get_thread_num() +1) * MAX_ITERATIONS;
		value = 0;
		for (j=0; j<iterations; j++) {
			value += (int) sqrt(j) % 11;
		}
		printf("finished round %d on thread %d\n", i, omp_get_thread_num());
	}

	exit(0);
}
