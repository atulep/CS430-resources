/*
 file_writing_mpi.c: A SPMD style MPI program to write a file in the current
 directory (PHASE I), then each process writes a file in the /tmp directory 
 on their node (PHASE II). Each phase was timed to show writing on local 
 /tmp is much faster than writing to the shared home directory that resides 
 on the master node. A barrier has been used to make sure all nodes have 
 started the PHASE I, the second one to ensure all nodes have finished the 
 PHASE I and the third one to ensure that they have finished the PHASE II 
 
 author: Hongyi Hu, Amit Jain
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <sys/resource.h>
#include <unistd.h>
#include <mpi.h>

double getMilliseconds();

#define LENGTH 512
#define NAME_LEN 128
char groupname[] = "worker";
char  *out_put;

int main(int argc, char *argv[]){

  double startTime = 0, totalTime = 0;
  char *curr_dir, *tmp_dir;
  int N, i;
  FILE *fopen(), *fp;

  int myId, nprocs;
  MPI_Group worker_group;

  if (argc < 2) {
    perror("Usage: file_writing_pvm <size> ");
    exit(1);
  }
  sscanf(argv[1],"%d",&N);

  /* Initialize MPI */
  MPI_Init(&argc, &argv);
  /* Find out how many processes there are in the default communicator */
  MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
  /* Find out my identity in the default communicator */
  MPI_Comm_rank(MPI_COMM_WORLD, &myId);
  /* Declare the group all processes belong to */  
  MPI_Comm_group(MPI_COMM_WORLD, &worker_group);

  curr_dir = (char *)malloc(sizeof(char)*LENGTH);
  tmp_dir = (char *)malloc(sizeof(char)*LENGTH);
  out_put = (char *)malloc(sizeof(char)*NAME_LEN);

  if(getcwd(curr_dir, LENGTH)==NULL){
    fprintf(stderr,"Trouble getcwd()\n");
    exit(3);  
  }
  
  MPI_Barrier(MPI_COMM_WORLD);  
 
  if(myId ==0){
  	startTime = getMilliseconds();
  } 
  
  sprintf(out_put, "%d", myId);   
  strcat(curr_dir,"/out.");
  strcat(curr_dir,out_put);

  
  if ((fp = fopen(curr_dir,"w")) == NULL) {
    perror("Unable to open output file in current working directory.");
  } else {
    int len = strlen("Hello there ! ");
    int temp = (int)N/len;
    for(i=0; i<temp; i++)
      fputs("Hello there ! ", fp);
    fclose(fp);
  }
  
  
  MPI_Barrier(MPI_COMM_WORLD);  

  if(myId==0){
    totalTime = getMilliseconds() - startTime;
    printf("Time of phase I = %f sec(s)\n",totalTime/1000.0);

  	startTime = getMilliseconds();
  }
     
  strcpy(tmp_dir,"/tmp\0");
  strcat(tmp_dir,"/out.");
  strcat(tmp_dir,out_put);
 
  if ((fp = fopen(tmp_dir,"w")) == NULL) {
    perror("Unable to open output file in /tmp directory.");
  } else {
    int len = strlen("Hello where ! ");
    int temp = (int)N/len;
    for(i=0; i<temp; i++)
      fputs("Hello where ! ", fp);
    fclose(fp);
  }
  
  
  MPI_Barrier(MPI_COMM_WORLD);

  if(myId==0){
    totalTime = getMilliseconds() - startTime;
    printf("Time for  phase II = %f sec(s)\n",totalTime/1000.0);

  }
  /* Leave the group */    
  MPI_Group_free(&worker_group);
  /* Program finished, exit MPI */
  MPI_Finalize();
  exit(0);

}

