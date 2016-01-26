/* from SUT, in MPI */
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
    MPI_Info hostinfo;
    MPI_Comm workers, all_processes;
	char *filename;

    MPI_Init(&argc, &argv);

	filename = getenv("PBS_NODEFILE");
	if (filename == NULL) {
		fprintf(stderr, "Not running under PBS control. Exiting!\n");
		MPI_Abort(MPI_COMM_SELF, 1);
	}
    MPI_Info_create(&hostinfo);
    MPI_Info_set(hostinfo, "file", filename);
    MPI_Info_set(hostinfo, "soft", "0-100");
	printf("Calling spawn.\n");
    MPI_Comm_spawn("worker", MPI_ARGV_NULL, 7, hostinfo, 0, MPI_COMM_SELF, &workers,
		    MPI_ERRCODES_IGNORE);
	printf("After calling spawn.\n");
    MPI_Info_free(&hostinfo);
	MPI_Intercomm_merge(workers, 0, &all_processes);

	allstatus = 0;
	mystatus = 1;
    MPI_Allreduce(&mystatus, &allstatus, 1, MPI_INT, MPI_SUM, all_processes);
    if (allstatus == -1) {
		MPI_Finalize();
		return(-1);
    }
	printf("allstatus = %d\n", allstatus);


    MPI_Comm_free(&workers);
    MPI_Comm_free(&all_processes);
    MPI_Finalize();
	exit(0);
}



