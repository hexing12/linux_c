/*创建子进程*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main()
{	pid_t pid;
	
	printf("Process Creation Study\n");
	pid = fork();
	switch(pid) {
		case 0: 	printf("Child process is running, CurPid is %d, Parentpid is %d\n", pid, getppid());
			    	break;
		case -1: 	perror("Process creation failed\n");
				break;
		default:	printf("Parent process is running, Childpid is %d, parentpid is %d\n", pid, getpid());
				break;
	}

	exit(0);
}
