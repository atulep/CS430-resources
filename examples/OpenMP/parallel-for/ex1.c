#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

/* 
 * Multiple calls to sqr are independent, so
 * this parallelization works.
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

    #pragma omp parallel for 
    for (i = 0; i < n; i++)
       	a[i] = sqrt(2 * i * 3.14159) ;
 
    return 0;
}

