
/*

  Approximate PI using the trapezoid rule to integrate 4/(1+x^2) from 0 to 1.

*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

int main(int argc, char **argv) 
{
	long long i;
	long long num_steps = 1000000000;
	double x, pi, step, sum = 0.0;

	if (argc > 1) {
		num_steps = atoll(argv[1]);
	}
	printf("Number of steps = %lld\n", num_steps);
	step = 1.0/(double) num_steps;

	#pragma omp parallel for private(x) reduction(+:sum)
	for (i=0; i<num_steps; i++) {
		x = (i+0.5)*step;
		sum += 4.0/(1.0+x*x);
	}

	pi = step * sum;
	printf(" pi =  %.9lf\n", pi);
	exit(0);
}

