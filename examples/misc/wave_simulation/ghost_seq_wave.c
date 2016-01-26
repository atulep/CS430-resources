
#include <stdio.h>
#include <stdlib.h>
#include <jemalloc/jemalloc.h>
#include <math.h>

/*#define DEBUG 0*/

#define MAX 8192
float **u;
float **uold;
float **unew;
char  pbmfile[32] = "dump.pbm";

void print_wave(float **u, int N)
{
	int i,j;

	for (i=1 ; i <=N ; i++) {
		for (j=1 ; j <= N; j++) {
			printf("%+2.2f ",u[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

void pbmdump(float **u, int N)
{

	float min, max, range;
	int i,j;
	FILE *fopen(), *fp;

	min=max=u[0][0];
	for (i=1; i<=N; i++) 
		for (j=1; j<=N; j++) {
			if (min > u[i][j]) min=u[i][j];
			if (max < u[i][j]) max=u[i][j];
		}
	range = max-min;

	if ((fp = fopen(pbmfile,"wb")) == NULL) {
		perror("Unable to open PBM dump file");
	} else {
		fprintf(fp,"P5\n%d %d\n255\n",N,N);
		for (i=1; i<=N; i++) 
			for (j=1; j<=N; j++) 
				fputc((char) (((u[i][j]-min)/range)*255.0), fp);
		fclose(fp);
	}
}


void allocate_arrays(int N)
{
	int i;
    int M = N+2; //for ghost rows and columns

	uold = (float **) calloc(M, sizeof(float *));
	u = (float **) calloc(M, sizeof(float *));
	unew = (float **) calloc(M, sizeof(float *));
	for (i=0; i<M ; i++) {
		uold[i] = (float *) calloc(M, sizeof(float));
		u[i] = (float *) calloc(M, sizeof(float));
		unew[i] = (float *) calloc(M, sizeof(float));
	}
}



int main(int argc, char *argv[])
{
	float rho,h,dt;
	int i,j,k,t;
	int N,steps;
	int iN,iS,jE,jW;
	float **p_uold, **p_u, **p_unew, **tmp;

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

	allocate_arrays(N);

	h = 1/(float) N;
	dt =  h/(float) sqrt((double)2.0);
	rho = dt/h;

	for (i=1; i<N; i++) {
		uold[i][i] = u[i][i] = 1.0;
		u[i][i+1] = 1.0;
	}
	uold[N][N] = u[N][N] = 1.0;
	u[N][1]=1.0;

	p_uold = uold;
	p_u = u;
	p_unew = unew;
	// copy the ghost rows and columns
	for (k = 1; k <= N; k++) {
		p_u[0][k] = p_u[N][k];
	 	p_u[N+1][k] = p_u[1][k];
		p_u[k][0] = p_u[k][N];
		p_u[k][N+1] = p_u[k][1];

		p_uold[0][k] = p_uold[N][k];
	 	p_uold[N+1][k] = p_uold[1][k];
		p_uold[k][0] = p_uold[k][N];
		p_uold[k][N+1] = p_uold[k][1];
	}

	for (t=0; t<steps; t++) 
	{
		#pragma omp parallel for private (i, j, iS, iN, jE, jW) 
		for (i=1; i <= N; i++) {
			iS = i + 1;
			iN = i - 1;
			for (j=1; j <= N; j++) {
				jE = j + 1;
				jW = j - 1;
				p_unew[i][j] = 2.0*(1-2.0*rho*rho)*p_u[i][j] + rho*rho*(p_u[iS][j] + 
								p_u[iN][j] + p_u[i][jW] + p_u[i][jE]) - p_uold[i][j];
			}
		}

		//copy the ghost rows and columns
		for (k = 1; k <= N; k++) {
			p_unew[0][k] = p_unew[N][k];
		 	p_unew[N+1][k] = p_unew[1][k];
			p_unew[k][0] = p_unew[k][N];
			p_unew[k][N+1] = p_unew[k][1];
		}
		tmp = p_u; p_u = p_unew; p_unew = p_uold; p_uold = tmp;
	}

#ifdef DEBUG
	print_wave(p_u, N);
#endif

	pbmdump(p_u, N);
	exit(0);
}

