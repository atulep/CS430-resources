
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*#define DEBUG 0*/

#define MAX 8192
float u[MAX][MAX];
float uold[MAX][MAX];
float unew[MAX][MAX];
char  pbmfile[32] = "dump.pbm";

void print_wave(int N)
{
	int i,j;

	for (i=0; i<N; i++) {
		for (j=0; j<N; j++) {
			printf("%+2.2f ",u[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

void pbmdump(int N)
{

	float min, max, range;
	int i,j;
	FILE *fopen(), *fp;

	min=max=u[0][0];
	for (i=0; i<N; i++) 
		for (j=0; j<N; j++) {
			if (min > u[i][j]) min=u[i][j];
			if (max < u[i][j]) max=u[i][j];
		}
	range = max-min;

	if ((fp = fopen(pbmfile,"wb")) == NULL) {
		perror("Unable to open PBM dump file");
	} else {
		fprintf(fp,"P5\n%d %d\n255\n",N,N);
		for (i=0; i<N; i++) 
			for (j=0; j<N; j++) 
				fputc((char) (((u[i][j]-min)/range)*255.0), fp);
		fclose(fp);
	}
}


int main(int argc, char *argv[])
{
	float rho,h,dt;
	int i,j,t;
	int N,steps;
	int iN,iS,jE,jW;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s <size> <steps> [<pbmdump file>]\n", argv[0]);
		exit(1);
	}
	sscanf(argv[1],"%d",&N);
	if (N > MAX) {
		fprintf(stderr,"Maximum size allowed is %d\n",MAX);
		exit(2);
	}
	sscanf(argv[2],"%d",&steps);
	printf ("N = %d steps = %d \n",N,steps);

	if (argc == 4) {
		sscanf(argv[3],"%s",pbmfile);
	}
	printf("Output will be dumped to <%s> \n\n",pbmfile);



	h = 1/(float) N;
	dt =  h/(float) sqrt((double)2.0);
	rho = dt/h;

	for (i=0; i<N-1; i++) {
		uold[i][i]=u[i][i]=1.0;
		u[i][i+1]=1.0;
	}
	uold[N-1][N-1]=u[N-1][N-1]=1.0;
	u[N-1][0]=1.0;

	for (t=0; t<steps; t++) {
#pragma omp parallel
{
	    #pragma omp for private(i, j, iS, iN, jE, jW, rho) 
		for (i=0; i<N; i++) {
			iS = (i<N-1 ? (i+1):(0));
			iN = (i>0 ? (i-1):(N-1));	
			for (j=0; j<N; j++) {
				jE = (j<N-1 ? (j+1):(0));
				jW = (j>0 ? (j-1):(N-1));	
				unew[i][j] = 2.0*(1-2.0*rho*rho)*u[i][j] + rho*rho*(u[iS][j] + 
								u[iN][j] + u[i][jW] + u[i][jE]) - uold[i][j];
			}
		}
}
		for (i=0; i<N; i++) {
			for (j=0; j<N; j++) {
				uold[i][j]=u[i][j];
				u[i][j]=unew[i][j];
			}
		}
	}

	/* ------------------------------------------------------------ */

#ifdef DEBUG
	print_wave(N);
#endif
	pbmdump(N);
	exit(0);
}

