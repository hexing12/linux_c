/*自编的验证pthread_once函数*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_once_t once = PTHREAD_ONCE_INIT;

void run(void)
{	printf("function run is running in thread %lu\n", pthread_self());
}

void *thread1(void *arg)
{	pthread_t thid = pthread_self();
	printf("current thread's id is %lu\n", thid);
	pthread_once(&once, run);
	printf("thread1 ends\n");
}

void *thread2(void *arg)
{       pthread_t thid = pthread_self();
        printf("current thread's id is %lu\n", thid);
        pthread_once(&once, run);
        printf("thread2 ends\n");
}

int main()
{	pthread_t thid1, thid2;

	pthread_create(&thid1, NULL, thread1, NULL);
	pthread_create(&thid2, NULL, thread2, NULL);
	sleep(3);
	printf("main thread exit!\n");
	exit(0);
}

/*出现的问题:
**	pthread_once_t once的初值问题
*/                           
