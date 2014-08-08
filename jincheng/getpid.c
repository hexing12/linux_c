#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{	pid_t pid;
	
	if((pid = fork()) == -1) {
		printf("fork error!\n");
		exit(1);
	}

	if(pid == 0)
		printf("getpid return %d\n", getpid());

	exit(0);
}
