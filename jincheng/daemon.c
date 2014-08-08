/*创建守护进程*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <syslog.h>
#include <sys/param.h>
#include <sys/stat.h>

int init_daemon(void)
{	int pid;
	int i;

	/*忽略终端i/o信号，stop信号*/
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGHUP, SIG_IGN);

	pid = fork();
	if(fork > 0)
		exit(0);	//结束父进程，使得子进程成为后台进程
	else if(pid < 0)
		return -1;

	/*建立一个新进程，使子进程成为新进程的首进程，使其脱离所有终端*/
	setsid();

	/*再建一个子进程，退出父进程，保证该进程不是进程组长，同时让他无法在打开一个新的终端*/
	pid = fork();
	if(pid > 0)
		exit(0);
	else if(pid < 0)
		return -1;

	/*关闭所有从父进程继承的不再需要的文字描述符*/
	for(i = 0; i < NOFILE; close(i++));

	/*改变工作目录，使得进程不再与任何文件系统联系*/
	chdir("/");
	
	/*将文件屏蔽字设置为0*/
	umask(0);

	/*忽略SIGCHLD信号*/
	signal(SIGCHLD, SIG_IGN);
	
	return 0;
}

int main()
{	time_t now;

	init_daemon();
	syslog(LOG_USER|LOG_INFO,"测试守护进程!\n");
	while(1) {
		sleep(8);
		time(&now);
		syslog(LOG_USER|LOG_INFO,"系统时间: \t%s\t\t\n", ctime(&now));
	}
}
