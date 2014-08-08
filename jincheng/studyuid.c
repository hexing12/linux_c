#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

extern int errno;

int main()
{	int fd;
	
	printf("uid study:\n");
	printf("process's uid = %d, euid = %d\n", getuid(), geteuid());

	if((fd = open("test.c", O_RDWR)) == -1) {
		printf("open failure, errno is %d: %s\n", errno, strerror(errno));
		exit(1);
	} else {
		printf("open successfully!\n");
	}

	close(fd);
	exit(0);
}
