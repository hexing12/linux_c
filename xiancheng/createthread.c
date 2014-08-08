/*自编的创建线程函数*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int *thread(void *arg)
{	pthread_t newthid;

	newthid = pthread_self();
	printf("this is a new thread, thread id = %lu\n", newthid);
	return NULL;
}

int main(void) 
{	pthread_t thid;

	printf("main thread, id is %lu\n", pthread_self());
	if(pthread_create(&thid, NULL, (void *)thread, NULL) != 0) {
		printf("thread creation failed\n");
		exit(1);
	}
	//sleep(1);
	//printf("%lu\n", thid);
	sleep(1);
	exit(0);
}

/*出现的问题
**	注释行对多线程的深入理解
*/

