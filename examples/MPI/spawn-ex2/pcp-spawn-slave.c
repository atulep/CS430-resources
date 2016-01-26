/* pcp from SUT, in MPI */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mpi.h>
#define BUFSIZE    256*1024
#define CMDSIZE    80
int main(int argc, char *argv[])
{
    int mystatus, allstatus, done, numread;
    char controlmsg[80];
    int outfd;
    char buf[BUFSIZE];
    char hostname[64];
    MPI_Comm slavecomm, all_processes;

    MPI_Init(&argc, &argv);
	gethostname(hostname, 64);
	printf("Spawned process on machine: %s\n", hostname);

    MPI_Comm_get_parent(&slavecomm);
	MPI_Intercomm_merge(slavecomm, 1, &all_processes);
    MPI_Bcast(controlmsg, CMDSIZE, MPI_CHAR, 0, slavecomm);
    if (strcmp(controlmsg, "exit") == 0) {
        MPI_Finalize();
		return(-1);
    }

    MPI_Bcast(controlmsg, CMDSIZE, MPI_CHAR, 0, all_processes);
    if ((outfd = open(controlmsg, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU)) == -1) 
        mystatus = -1;
    else
        mystatus = 0;
    MPI_Allreduce(&mystatus, &allstatus, 1, MPI_INT, MPI_MIN, all_processes);
    if (allstatus == -1) {
		MPI_Finalize();
		return(-1);
    }

    /* at this point all files have been successfully opened */
    
	printf("File opened successfully\n");
    done = 0;
    while (!done) {
		MPI_Bcast(&numread, 1, MPI_INT, 0, all_processes);
		if (numread > 0) {
	    	MPI_Bcast(buf, numread, MPI_BYTE, 0, all_processes);
	    	write(outfd, buf, numread); 
		} else {	  
	    	close(outfd);
	    	done = 1;
		}
    }
    MPI_Comm_free(&slavecomm);
    MPI_Comm_free(&all_processes);
    MPI_Finalize();
    exit(0);
}
