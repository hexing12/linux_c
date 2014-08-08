/*验证父子进程交替进行*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main()
{       pid_t pid;
	char *msg;
	int k;

        printf("Process Creation Study\n");
        pid = fork();
	printf("%d\n", pid);
        switch(pid) {
		case 0:  msg = "child process is running";
			 k = 3;
			 break;
		case -1: perror("process creation failed\n");
			 break;
		default: msg = "parent process is running";
			 k = 5;
			 break;
        }

	while(k > 0) {
		puts(msg);
		sleep(1);
		k--;
	}

        exit(0);
}

/* 出现的问题
**	父子进程交替出现的内幕，详情见刘瑜琀的博客
*/                                    
