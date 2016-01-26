

/**
	monte_carlo_pi.c

	Calculate the value of Pi using a Monte Carlo Simulation.

	To build: 
		make

	To run: 
		smcpi <number of iterations> <random seed> <checkpoint interval>
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

double getMilliSeconds();
double report_cpu_time();
void   print_state(int);
int interval = 10;
long long count;
long long n;


/** run_random_experiments:

	Throw random darts on the square [-1..1][-1..1] on the real number plane and
	count how many end up inside the circle centered at (0,0) with
	radius of 1.

	return: the count of successful throws (as a double to allow larger counts)
*/
double run_random_experiments()
{
	long long  m;
	double x, y;

	m = 0;
	for (count=0; count<n; count++) {
		x = -1 +  (2.0 * (random() / (RAND_MAX + 1.0)));
		y = -1 +  (2.0 * (random() / (RAND_MAX + 1.0)));
		/* check if (x,y) is within the unit circle */
		if ((x*x + y*y) < 1)  {
			m++;
		}
	}
	return m;


}


/** 
	estimate_pi:

	Estimate the value of Pi using a Monte carlo experiment.

	return: the value of Pi as a double
*/

double estimate_pi(unsigned int seed)
{
	long long m;

	srandom(seed);

	m = run_random_experiments();

	return (4 * (double) m / n);
}


int main(int argc, char **argv)
{
	unsigned int seed;
	double pi;
	double startTime, totalTime;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s <number of iterations> <random seed> <checkpoint interval>\n", argv[0]);
		exit(1);
	}

	n = atoll(argv[1]);
	seed = atoi(argv[2]);
	if (argc == 4) {
		interval = atoi(argv[3]);
	}

	signal(SIGALRM, print_state);
	alarm(interval);

	startTime = getMilliSeconds();

	pi = estimate_pi(seed);

	totalTime = getMilliSeconds() - startTime;

	printf("Simulation progress: %04.1lf%% done\n", ((double)count/n)*100);

	printf("PI = %1.16lf\n", pi);
	printf("Time taken = %6.2lf seconds\n", totalTime/1000.0);

	exit(0);
}


/**
  print_state:
 
  Invoked on receiving an alarm signal. Prints the current
  state of the computation to standard output. 
 
*/
void print_state(int signo)
{
	printf("Simulation progress: %04.1lf%% done\n", ((double)count/n)*100);
	fflush(NULL);
    // reset alarm for next time
    alarm(interval);
}



