#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>


/*
 *
 * Multiple calls to random are interdependent, so
 * this parallelization fails.
 */
int main(int argc, char *argv[]) 
{
    int n;
    int i;
	float *a;
 
	if (argc != 2) {
		fprintf(stderr, "Usage %s: <n>\n", argv[0]);
		exit(1);
	}

	n = atoi(argv[1]);
	a = (float *) malloc(sizeof(float) * n);

    #pragma omp parallel 
	{
		#pragma omp for 
    	for (i = 0; i < n; i++) {
       		a[i] = sqrt(2 * i * 3.14159) * random() ;
		}
	}
 
    return 0;
}

