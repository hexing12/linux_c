/*孤儿进程的验证*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{	pid_t pid;
	int k = 3;

	pid = fork();
	switch(pid) {
		case 0:
			while(1) {
				printf ("My parent id is %d \n" , getppid ()) ;
				sleep(3);			
			}
			exit(0);
		case -1:
			perror("process creation is failed\n");
			exit(-1);
		default:
			printf("i am parent process, my pid is %d\n", getpid());
			exit(0);
				
	}

	return 0;
}
