#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

#define CHECKPOINT (1024 * 1024)


void usage(char *prgname)
{
	fprintf(stderr, "usage: %s r|w </dev/node> <nbytes> [skip]\n", prgname);
}

int main(int argc, char **argv)
{

	int prot;
	int fd;
	int ret;
	long nbytes;
	long nelements;
	long skip;
	long i;
	unsigned long *diskp;

	if (argc < 4) {
		usage(argv[0]);
		return 1;
	}

	nbytes = atol(argv[3]);
	if (nbytes <= 0) {
		printf("How much is the fish?\n");
		return 0;
	}

	nelements = nbytes / sizeof(unsigned long);

	if (argc == 5) {
		skip = atol(argv[4]) /  sizeof(unsigned long);
	} else {
		skip = 0;
	}

	if (!strcmp(argv[1], "r")) {
		prot = PROT_READ;
	} else if (!strcmp(argv[1], "w")) {
		prot = PROT_WRITE;
	} else {
		usage(argv[0]);
		return 1;
	}

	fd = open(argv[2], O_RDWR);
	if (fd < 0) {
		perror("Can't open() device");
		return 1;
	}

	printf("nbytes = %ld\n", nbytes);
	if (skip)
		printf("skip = %ld\n", skip);


	diskp = (unsigned long *) mmap(NULL, nbytes, prot, MAP_SHARED, fd, 0);
	if (diskp == MAP_FAILED) {
		perror("Can't mmap device");
		return 1;
	}

	if (prot == PROT_WRITE) {
		printf("Writing...\n");
		for (i = skip; i < nelements; i++) {
			diskp[i] = i;
			if (!((i * sizeof(unsigned long) % (CHECKPOINT * 100)))) {
				printf("%ld\n", (i * sizeof (unsigned long) / CHECKPOINT));
			}
		}
		printf("\n");
		printf("Done\n");
	} else {
		for (i = skip; i < nelements; i++) {
			if (diskp[i] != i) {
				printf("%ld: %ld\n", i, diskp[i]);
			}
			if (!((i * sizeof(unsigned long) % (CHECKPOINT * 100))))
				printf("%ld\n", (i * sizeof (unsigned long) / CHECKPOINT));

		}
	}

	printf("Unmapping...\n");
	munmap((void *)diskp, nbytes);
	printf("Syncing...\n");
 	fsync(fd);
	printf("Closing...\n");
	close(fd);

	printf("Bye.\n");

	return 0;

}
