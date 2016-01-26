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
    char outfilename[128], controlmsg[80];
    int infd, outfd;
    char buf[BUFSIZE];
    MPI_Info hostinfo;
    MPI_Comm pcpslaves, all_processes;
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
    MPI_Comm_spawn("pcp_slave", MPI_ARGV_NULL, 7, hostinfo, 0, MPI_COMM_SELF, &pcpslaves,
		    MPI_ERRCODES_IGNORE);
	printf("After calling spawn.\n");
    MPI_Info_free(&hostinfo);
	MPI_Intercomm_merge(pcpslaves, 0, &all_processes);

    strcpy(outfilename, argv[2]);
    if ((infd = open(argv[1], O_RDONLY)) == -1) {
        fprintf(stderr, "input %s does not exist\n", argv[2]);
		sprintf(controlmsg, "exit");
		MPI_Bcast(controlmsg, CMDSIZE, MPI_CHAR, 0, all_processes);
		MPI_Finalize();
		return(-1);
    } else {
        sprintf(controlmsg, "ready");
        MPI_Bcast(controlmsg, CMDSIZE, MPI_CHAR, 0, all_processes);
    }
	printf("Opened input file.\n");

    sprintf(controlmsg, outfilename);
    MPI_Bcast(controlmsg, CMDSIZE, MPI_CHAR, 0, all_processes);
    if ((outfd = open(outfilename, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU)) == -1)
        mystatus = -1;
    else
        mystatus = 0;
    MPI_Allreduce(&mystatus, &allstatus, 1, MPI_INT, MPI_MIN, all_processes);
    if (allstatus == -1) {
        fprintf(stderr, "output file %s could not be opened\n", outfilename);
		MPI_Finalize();
		return(-1);
    }
	printf("All processes have opened  output file.\n");

    /* at this point all files have been successfully opened */
    
    done = 0;
    while (!done) {
        numread = read(infd, buf, BUFSIZE);
		MPI_Bcast(&numread, 1, MPI_INT, 0, all_processes);
		if (numread > 0) {
	    	MPI_Bcast(buf, numread, MPI_BYTE, 0, all_processes);
	    	write(outfd, buf, numread);
		} else {	  
	    	close(outfd);
	    	done = 1;
		}
    }
    MPI_Comm_free(&pcpslaves);
    MPI_Comm_free(&all_processes);
    MPI_Finalize();
	exit(0);
}



