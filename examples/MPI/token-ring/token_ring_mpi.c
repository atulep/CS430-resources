/*
 token_ring_mpi.c: A SPMD style MPI program to demonstrate a token ring
 
 @author: Hongyi Hu, Amit Jain 
 
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <mpi.h>

#define MAXNPROC 32

void pass_the_token();

int main(int argc, char **argv)
{
	int myid;              /* my process number */
	int i;
	int nproc;	           /* number of processes */

	/* Start up MPI */
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	if (myid == 0)
	{
		printf("Pass a token through the %3d id ring:\n", nproc);
		for (i = 0; i < nproc; i ++)
		{
			printf( "%6d -> ", i);
			if (i % 6 == 0 && i > 0)
				printf("\n");	
		}
		printf("%6d \n", myid);
	}
/*--------------------------------------------------------------------------*/
     	
	for(i=0; i<50; i++) {
	     pass_the_token( myid, nproc );
	}

    /* Program finished, exit MPI */
    MPI_Finalize();
    exit(0);
}

/* Simple example passes a token around a ring */

void pass_the_token(int me, int nproc)
{
  int token;
  int src, dest;
  int count  = 1;
  const int TOKEN = 4;
  int msgtag = TOKEN;
  MPI_Status status;

  /* Determine neighbors in the ring */
  if ( me == 0 )
    src = nproc -1;
  else
    src = me - 1;
  
  if (me == nproc - 1)
    dest = 0;
  else
    dest = me + 1;
  
  if( me == 0 )
    { 
      token = 0;
      MPI_Send(&token,count, MPI_INT, dest, msgtag, MPI_COMM_WORLD);
      MPI_Recv(&token, count, MPI_INT, src, msgtag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      printf("token ring done\n");
    }
  else
    {
      MPI_Recv(&token, count, MPI_INT, src, msgtag, MPI_COMM_WORLD, &status); 
      printf("received token ring on %d from %d \n",me, src);
      MPI_Send(&token, count, MPI_INT,dest,msgtag,MPI_COMM_WORLD);   
    }
}
