
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

  
int  main( int argc, char **argv)
{
	int *values;
	int n;
	int i;
	long sum;
     
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <n> \n", argv[0]);
		exit(1);
	}
	n = atoi(argv[1]);
	values = (int *) malloc(sizeof(int)*n);
	for (i=0; i<n; i++)
		values[i] = 1;

	sum = 0;
	for (i=0; i<n; i++) {
		sum += values[i];
	}

	printf("Total sum = %ld\n", sum);
    exit(0);
}



