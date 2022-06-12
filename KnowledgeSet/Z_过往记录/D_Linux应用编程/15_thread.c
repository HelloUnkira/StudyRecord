#include <pthread.h>
#include <stdio.h>

typedef unsigned long pthread_t;

void* thread1(void* arg)
{
	while(1) {
		printf("I am thread1\n");	
		sleep(1);
	}
}

int main()
{
	pthread_t pid;
	int ret_pthread_create = pthread_create(&pid, NULL,
		thread1, NULL);
	if (ret_pthread_create == -1) {
		perror("thread create fail\n");
		return -1;
	}
	
	while(1) {
		printf("I am main\n");	
		sleep(1);
	}	
	
	return 0;
}
