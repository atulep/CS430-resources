

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main() {

	int fd;
	int i, iterations;
	int count;
	unsigned int randomvalue;
	
                

	// The device /dev/urandom will always give back as many bytes
	// as we ask for but they are not as random as /dev/random
	fd = open("/dev/urandom",O_RDONLY);

	count = read(fd, &randomvalue, sizeof(unsigned int));
	printf("count = %d, random value = %u\n", count, randomvalue);
	printf("----------------------------------------------------\n");
 

	// /dev/random will stop after giving as many bytes as it has
    // accumulated...so if things aren't happening then 
    // we may not get back as many bytes as we want.
	fd = open("/dev/random",O_RDONLY);

	count = read(fd, &randomvalue, sizeof(unsigned int));
	if (count != sizeof(unsigned int)) {
		printf("Oops! Not enough randomness in this world.\n");
		printf("Needed %d bytes, got %d bytes \n", sizeof(unsigned int), count);
	} else {
		printf("count = %d, random value = %u\n", count, randomvalue);
	}
	printf("----------------------------------------------------\n");
}
 
