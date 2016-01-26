/* allocate_mem.c
 * Author:  Luke Hindman
 * Date:  November 10, 2006
 * Description:  This is a simple program to better understand how memory is allocated by GA
 */

#include <stdio.h>
#include <stdlib.h>
#include <ga.h>
#include <macommon.h>
#include <macdecls.h>
#include <mpi.h>
#include <sndrcv.h>
#include <error.h>



#define N 100     
#define BUF_SIZE 1024
#define ONE 1
#define ZERO 0


int main(int argc, char *argv[])
{
    /* The values for heap, stack, and global are the
     * number of elements to allocate space for, not
     * the number of bytes. */	
    int heap=1048576, stack=1048576, global=134217728;
    int me, nproc;
    int g_a;
    int ndim=2,type=MT_C_INT,dims[2];
    int status;


    MPI_Init(&argc, &argv);                       /* initialize MPI */

    GA_Initialize();                            /* initialize GA */
    me=GA_Nodeid();
    nproc=GA_Nnodes();
    if(me==0) printf("Using %ld processes\n",(long)nproc);


    /* Initialize Memory Allocator */
    if ( GA_Uses_ma() ) {
	fprintf(stderr,"Global Arrays Using Local Memory through MA\n");
	status = MA_init(MT_C_INT,stack,heap+global);
    }
    else {
	fprintf(stderr,"Global Arrays Using Shared Memory. \n");
	status = MA_init(MT_C_INT,stack,heap);
	GA_Set_memory_limit(MA_sizeof(MT_C_INT,global,MT_C_CHAR));
    }


    if(! status) 
       ga_error("MA_init failed",stack+heap);    

    dims[0]=11313; dims[1]=11313;
    g_a = NGA_Create(type, ndim, dims, "A", NULL);


    /* The memory used is on a per process basis */
    fprintf(stderr,"Node %d- Memory Used:  %dB\n",me,GA_Inquire_memory());


    fprintf(stderr,"Node %d- Memory Avail:  %dB\n",me,GA_Memory_avail());
    GA_Sync();
    GA_Summarize(1);
    

    GA_Destroy(g_a);


    if(me==0)printf("Terminating ..\n");
    GA_Terminate();

    MPI_Finalize();

    return 0;
}

