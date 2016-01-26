
#include <stdio.h>
#include <stdlib.h>

double getMilliSeconds();

void prefix_sums(int *sum, int n)
{
	int i;

	for (i=0; i<n; i++)
		sum[i] = sum[i-1] + sum[i];

}


int main(int argc, char **argv)
{
	int *sum;
	int n;
	int i;
	double startTime, totalTime;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <n>\n", argv[0]);
		exit(1);
	}

	n = atoi(argv[1]);

	sum = (int *) malloc(sizeof(int)*n);
	for (i=0; i<n; i++)
		sum[i] = 1;

	startTime = getMilliSeconds();
	prefix_sums(sum, n);
	totalTime = getMilliSeconds() - startTime;
	printf("Time taken = %lf\n", totalTime/1000.0);
	exit(0);
}
