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
    int mystatus, allstatus;
    char hostname[64];
    MPI_Comm parent, all_processes;

    MPI_Init(&argc, &argv);
	gethostname(hostname, 64);
	printf("Spawned process on machine: %s\n", hostname);

    MPI_Comm_get_parent(&parent);
	MPI_Intercomm_merge(parent, 1, &all_processes);

	allstatus = 0;
	mystatus = 1;
    MPI_Allreduce(&mystatus, &allstatus, 1, MPI_INT, MPI_SUM, all_processes);
    if (allstatus == -1) {
		MPI_Finalize();
		return(-1);
    }
	printf("allstatus = %d\n", allstatus);

    MPI_Comm_free(&parent);
    MPI_Comm_free(&all_processes);
    MPI_Finalize();
    exit(0);
}
