#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int globvar = 5;

int main(void) 
{	pid_t pid;
	int var = 1, i;

	printf("fork is different with vfork\n");
	//pid = fork();
	pid = vfork();
	switch(pid) {
		case 0:
			i = 3;
			while(i-- > 0) {
				printf("child process is running\n");
				globvar++;
				var++;
				sleep(1);
			}
			printf("child globvar = %d, var = %d\n", globvar, var);
			break;
	
		case -1:
			perror("process creation failed\n");
			exit(0);
		default:
			i = 5;
			 while(i-- > 0) {
                                printf("parent process is running\n");
                                globvar++;
                                var++;
                                sleep(1);
                        }
                        printf("parent globvar = %d, var = %d\n", globvar, var);

	}
	exit(0);
}
