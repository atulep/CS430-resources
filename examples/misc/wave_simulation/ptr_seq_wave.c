#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*#define DEBUG 1*/

#ifdef DISPLAY
#include "gldisplay.h"
#else
#define MAX 8192
#endif

#define M   (MAX+2)
float u[MAX+2][MAX+2], uold[MAX+2][MAX+2], unew[MAX+2][MAX+2];
char data[MAX][MAX];
char  pbmfile[32] = "dump";
	

void print_wave(int N, float *p_u)
{
	int i,j, ij;

	for (i = 1; i <= N; i++) {
		ij = i*M;
		for (j = 1; j <= N; j++) {
			ij++;
			printf("%+2.2f ", *(p_u + ij));
		}
		printf("\n");
	}
	printf("\n\n");
}


void create_bitmap(int N, float *p_u)
{
	float min, max, range;
	int i,j;
	int ij;

	min = max = *(p_u+M+1);
	for (i=1; i<=N; i++) {
		ij = i*M;
		for (j=1; j<=N; j++) {
			ij++;
			if (min > *(p_u+ij)) min = *(p_u+ij);
			if (max < *(p_u+ij)) max = *(p_u+ij);
		}
	}
	range = max-min;

	for (i=1; i<=N; i++) {
		ij = i*M;
		for (j=1; j<=N; j++) {
			ij++;
			data[i-1][j-1]= (((*(p_u+ij)-min)*255.0)/range);
		}
	}
}


void pbmdump(int N, float *p_u)
{

	float min, max, range;
	int i,j;
	int ij;
	FILE *fopen(), *fp;

	min = max = *(p_u + M + 1);
	for (i=1; i<=N; i++) {
		ij = i*M;
		for (j=1; j<=N; j++) {
			ij++;
			if (min > *(p_u + ij)) min = *(p_u + ij);
			if (max < *(p_u + ij)) max = *(p_u + ij);
		}
	}
	range = max-min;

	if ((fp = fopen(pbmfile,"wb")) == NULL) {
		perror("Unable to open PBM dump file");
	} else {
		fprintf(fp,"P5\n%d %d\n255\n",N,N);
		for (i=1; i<=N; i++) {
			ij = i*M;
			for (j=1; j<=N; j++) {
				ij++;
				fputc((char) (((*(p_u + ij)-min)/range)*255.0), fp);
			}
		}
		fclose(fp);
	}
}


int main(int argc, char *argv[])
{
	float rho,h,dt;
	int i,j,k,t;
	int N,steps;
#ifdef DISPLAY
	int jump;
#endif
	float *p_u, *p_uold, *p_unew, *tmp1;
	int i1,i2,i3,i4;
	int ij,ij1,ij2;

	if (argc < 3) {
#ifdef DISPLAY
		fprintf(stderr, "Usage: %s <size> <steps> [<jump>] [<pbmdump file>]\n", argv[0]);
#else
		fprintf(stderr, "Usage: %s <size> <steps> [<pbmdump file>]\n", argv[0]);
#endif
		exit(1);
	}
	sscanf(argv[1],"%d",&N);
	if (N > MAX) {
		fprintf(stderr,"Maximum size allowed is %d\n",MAX);
		exit(2);
	}

	sscanf(argv[2],"%d",&steps);
#ifdef DISPLAY
	if (argc > 3) {
		sscanf(argv[3],"%d",&jump);
	}
	if (jump < 1) jump = 1;
	if (jump > steps) jump = steps;
	printf ("N = %d steps = %d jump = %d \n",N,steps,jump);

	if (argc == 5) {
		sscanf(argv[4],"%s",pbmfile);
	}
	printf("Output will be dumped to <%s> \n\n",pbmfile);
	/* initialize window */
	initwin(N,(unsigned char *)data, argc, argv);
#else
	printf ("N = %d steps = %d\n",N,steps);
	if (argc ==  4) {
		sscanf(argv[3],"%s",pbmfile);
	}
	printf("Output will be dumped to <%s> \n\n",pbmfile);
#endif

	h = 1/(float) N;
	dt =  h/(float) sqrt((double)2.0);
	rho = dt/h;

	for (i=1; i<N; i++) {
		uold[i][i]=u[i][i]=1.0;
		u[i][i+1]=1.0;
	}
	uold[N][N]=u[N][N]=1.0;
	u[N][1]=1.0;

	// copy the ghost rows and columns
	for (k=1; k<=N; k++) {
		u[0][k] = u[N][k];
	 	u[N+1][k] = u[1][k];
		u[k][0] = u[k][N];
		u[k][N+1] = u[k][1];

		uold[0][k] = uold[N][k];
	 	uold[N+1][k] = uold[1][k];
		uold[k][0] = uold[k][N];
		uold[k][N+1] = uold[k][1];
	}

	/* initialize pointers */
	p_u  = &u[0][0];
	p_uold = &uold[0][0];
	p_unew = &unew[0][0];


	for (t=0; t<steps; t++) {
#ifdef DISPLAY
		if ((t % jump) == 0) {
			create_bitmap(N,p_u);
			display_once();
		}
#endif
#pragma omp parallel for private(i, j, ij, ij1, ij2, rho) 
		for (i=1; i<=N; i++) {
			ij = i*M;
			ij1 = ij+M;
			ij2 = ij-M;
			for (j=1; j<=N; j++) {
				ij++;ij1++;ij2++;
				*(p_unew+ij) = 2.0*(1-2.0*rho*rho)* ( *(p_u+ij)) +
					rho*rho*(*(p_u+ij1) + *(p_u+ij2) + 
					*(p_u+ij-1) + *(p_u+ij+1)) - *(p_uold+ij);
			}
		}

		// copy the ghost rows and columns
		i1=N*M;
		i2=i1+M;
		i3=M;
		i4=M+N;
		for (i=1; i<=N; i++) {
			i1++; i2++;
			*(p_unew+i) = *(p_unew+i1);
			*(p_unew+i2) = *(p_unew+M+i);
			*(p_unew+i3)= *(p_unew+i4);
			*(p_unew+i4+1)= *(p_unew+i3+1);
			i3 += M; i4 += M;
		}
		tmp1 = p_u; p_u = p_unew; p_unew = p_uold; p_uold = tmp1; 
	}



#ifdef DISPLAY
	closewin();
#endif

#ifdef DEBUG
	print_wave(N, p_u);
#endif

	pbmdump(N, p_u);
	exit(0);
}

/* vim: set ts=4: */

