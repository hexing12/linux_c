#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <wait.h>

int main()
{	pid_t pid;
	int stat_val = 0;
	int oldpri, newpri;

	printf("nice study:\n");
	pid = fork();
	switch(pid) {
		case 0: 	printf("child is running, curpid is %d, parentpid is %d\n", pid, getpid());
				oldpri = getpriority(PRIO_PROCESS, 0);
				printf("old priority = %d\n", oldpri);
		
				newpri = nice(2);
				printf("new priority = %d\n", newpri);
				exit(0);
		case -1: 	perror("process creation failed\n");
				break;
		default:	printf("parent is running, childpid is %d, parentpid is %d\n", pid,getpid());
				break;
	}

	wait(&stat_val);
	exit(0);
}
