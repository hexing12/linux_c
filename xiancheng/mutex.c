/*验证互斥锁的过程*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mutex;
int globalnumber = 9;


void *read(void *arg);
void *write(void *arg);

void *write(void *arg)
{	pthread_mutex_lock(&mutex);
	globalnumber++;
	printf("wtite: %d\n", globalnumber);
	pthread_mutex_unlock(&mutex);
	return (void *)0;
}

void *read(void *arg)
{	printf("here\n");
	pthread_mutex_lock(&mutex);
	printf("read: %d\n", globalnumber);
	pthread_mutex_unlock(&mutex);

	return (void *)0;
}

int main()
{	pthread_t tid1, tid2;
	pthread_mutex_init(&mutex, NULL);
	
	pthread_create(&tid1, NULL, write, NULL);
	sleep(5);
	printf("haha!\n");
	pthread_create(&tid2, NULL, read, NULL);
	sleep(3);

	exit(0);
}

/*出现的问题:
**	主线程先于子线程结束
**/
