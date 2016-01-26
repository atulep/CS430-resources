
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*#define DEBUG 0*/

float **u;
float **uold;
float **unew;
char  pbmfile[32] = "dump.pbm";

void print_wave(float **u, int N)
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

void pbmdump(float **u, int N)
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


void allocate_arrays(int N)
{
	int i;

	uold = (float **) calloc(N, sizeof(float *));
	u = (float **) calloc(N, sizeof(float *));
	unew = (float **) calloc(N, sizeof(float *));
	for (i=0; i<N; i++) {
		uold[i] = (float *) calloc(N, sizeof(float));
		u[i] = (float *) calloc(N, sizeof(float));
		unew[i] = (float *) calloc(N, sizeof(float));
	}

}



int main(int argc, char *argv[])
{
	float rho,h,dt;
	int i,j,t;
	int N,steps;
	int iN,iS,jE,jW;
	float **p_uold, **p_u, **p_unew, **tmp;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s <size> <steps> [<pbmdump file>]\n", argv[0]);
		exit(1);
	}
	sscanf(argv[1],"%d",&N);
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

	for (i=0; i<N-1; i++) {
		uold[i][i]=u[i][i]=1.0;
		u[i][i+1]=1.0;
	}
	uold[N-1][N-1]=u[N-1][N-1]=1.0;
	u[N-1][0]=1.0;
		

	p_uold = uold;
	p_u = u;
	p_unew = unew;

	for (t=0; t<steps; t++) 
	{
#pragma omp parallel for private (i, j, iS, iN, jE, jW)
		for (i=0; i<N; i++) {
			iS = (i<N-1 ? (i+1):(0));
			iN = (i>0 ? (i-1):(N-1));	
			for (j=0; j<N; j++) {
				jE = (j<N-1 ? (j+1):(0));
				jW = (j>0 ? (j-1):(N-1));	
				p_unew[i][j] = 2.0*(1-2.0*rho*rho)*p_u[i][j] + rho*rho*(p_u[iS][j] + 
								p_u[iN][j] + p_u[i][jW] + p_u[i][jE]) - p_uold[i][j];
			}
		}
		tmp = p_u; p_u = p_unew; p_unew = p_uold; p_uold = tmp;
	}

#ifdef DEBUG
	print_wave(p_u, N);
#endif

	pbmdump(p_u, N);
	exit(0);
}

