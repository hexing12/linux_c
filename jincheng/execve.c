/*exec函数实例，这里使用execve函数*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[], char **environ)
{	pid_t pid;
	int stat_val;

	printf("exec example!\n");
	pid = fork();
	switch(pid) {
		case -1: 	perror("process creation failed\n");
				exit(1);
		case 0:
				printf("child process is running\n");
				printf("my pid = %d, parentpid = %d\n", getpid(), getppid());
				printf("uid = %d, gid = %d\n", getuid(), getgid());
				execve("processimage", argv, environ);
				printf("process never go to here!\n");
				exit(0);
		default:
				printf("parent process is running\n");
				break;
	}
	wait(&stat_val);
	exit(0);
}
