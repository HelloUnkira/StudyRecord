#include <stdio.h>
#include <pthread.h>

int data = 0;
pthread_mutex_t mutex;
pthread_mutex_t * pmutex = &mutex;
//restrict

typedef unsigned long pthread_t;

void* thread1(void* arg)
{
	int i = 0;
	int ret_pthread_mutex_lock = \
		pthread_mutex_lock(pmutex);
	if (ret_pthread_mutex_lock == -1) {
		perror("pthread_mutex_lock failed\n");
		return NULL;
	}
	
	while (++i <= 1000000)
		data++;
	
	int ret_pthread_mutex_unlock = \
		pthread_mutex_unlock(pmutex);
	if (ret_pthread_mutex_unlock == -1) {
		perror("pthread_mutex_unlock failed\n");
		return NULL;
	}
	
	pthread_exit(NULL);
	return NULL;
}

void* thread2(void* arg)
{
	int i = 0;
	int ret_pthread_mutex_lock = \
		pthread_mutex_lock(pmutex);
	if (ret_pthread_mutex_lock == -1) {
		perror("pthread_mutex_lock failed\n");
		return NULL;
	}
	
	while (++i <= 1000000)
		data++;
	
	int ret_pthread_mutex_unlock = \
		pthread_mutex_unlock(pmutex);
	if (ret_pthread_mutex_unlock == -1) {
		perror("pthread_mutex_unlock failed\n");
		return NULL;
	}
	
	pthread_exit(NULL);	
	return NULL;	
}

void* thread3(void* arg)
{
	int i = 0;
	int ret_pthread_mutex_lock = \
		pthread_mutex_lock(pmutex);
	if (ret_pthread_mutex_lock == -1) {
		perror("pthread_mutex_lock failed\n");
		return NULL;
	}
	
	while (++i <= 1000000)
		data++;
	
	int ret_pthread_mutex_unlock = \
		pthread_mutex_unlock(pmutex);
	if (ret_pthread_mutex_unlock == -1) {
		perror("pthread_mutex_unlock failed\n");
		return NULL;
	}
	
	pthread_exit(NULL);	
	return NULL;	
}

int main()
{
	pthread_t pid1, pid2, pid3;
	int ret_pthread_create = 0;
	ret_pthread_create |= pthread_create(&pid1, NULL,
							  thread1, NULL);
	ret_pthread_create |= pthread_create(&pid2, NULL,
							  thread2, NULL);		
	ret_pthread_create |= pthread_create(&pid3, NULL,
							  thread3, NULL);	
	if (ret_pthread_create == -1) {
		perror("pthread_create failed\n");
		return -1;
	}
	

	int ret_pthread_mutex_init = pthread_mutex_init(\
						 pmutex, NULL);	
	if (ret_pthread_mutex_init == -1) {
		perror("pthread_mutex_init failed\n");
		return -1;			
	}
		
						
	sleep(1);	
	
	int ret_pthread_join = 0;
	ret_pthread_join |= pthread_join(pid1, NULL);	
	ret_pthread_join |= pthread_join(pid2, NULL);	
	ret_pthread_join |= pthread_join(pid3, NULL);	
	if (ret_pthread_join == -1) {
		perror("pthread_join failed\n");
		return -1;
	}	
	
		printf("%d", data);
			
	int ret_pthread_cancel = 0;
	ret_pthread_cancel |= pthread_cancel(pid1);					  
	ret_pthread_cancel |= pthread_cancel(pid2);					  
	ret_pthread_cancel |= pthread_cancel(pid3);	
	if (ret_pthread_cancel == -1) {
		perror("pthread_cancel failed\n");
		return -1;
	}
	
	int ret_pthread_mutex_destroy = \
		pthread_mutex_destroy(pmutex);
	if (ret_pthread_mutex_destroy == -1) {
		perror("pthread_mutex_destroy failed\n");
		return -1;			
	}
	
	return 0;
}







