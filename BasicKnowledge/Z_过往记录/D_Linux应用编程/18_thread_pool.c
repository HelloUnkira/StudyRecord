#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct pthread_pool
{
	//������,����������������С��Ļ������
	pthread_mutex_t lock;
	//��������,��ʾ��������С��Ƿ��������
	pthread_cond_t cond;
	//��ʾ�����Ƿ��˳�
	bool shutdown;	
	//ָ���߳�ID������,�̳߳��������̵߳�tid
	pthread_t *tids;
	//�̳߳������ڷ��۵��߳�����,��ǰ�߳�����
	unsigned int active_threads;
	//�������(����),ָ���һ����Ҫִ�е�����
	//���е��̶߳��Ǵ�����������������ȡ���񡰹�����Դ��	
	struct task *task_list;
	//�̳߳���������е�������������
	unsigned int max_task;	
	//�̳߳��е�ǰ������е���������
	unsigned int cur_task;
}pthread_pool;

//����һ������
typedef struct task  //������
{
	//�����㱾���Ͼ���������ʾһ������(����)cp_file,"����ָ��"ָ����Ҫ��ɵĺ���
	void *(*do_task)(void *arg); //�������е�����,"����ָ��"
	void *arg;//ָ��,ִ����������Ҫ�Ĳ���(�ļ�������)
	struct task *next; 
}task;	

void * start_routine(void *arg)
{
	while (1) {	
	
		//��ȡ�̻߳�����,����
		pthread_mutex_lock(&((struct pthread_pool *)arg)->lock);
		
		//���̳߳�δ������ʱ��,�����������ȥ������
		if(((struct pthread_pool *)arg)->cur_task != 0 && \
			((struct pthread_pool *)arg)->shutdown == 0) {	
	
			struct task* current_thread = NULL;					
			//��(struct pthread_pool *)arg)->task_list��ժ��һ����
			current_thread = ((struct pthread_pool *)arg)->task_list;
			if (((struct pthread_pool *)arg)->task_list != NULL)
				((struct pthread_pool *)arg)->task_list = \
				((struct pthread_pool *)arg)->task_list->next;
			current_thread->next = NULL;
			((struct pthread_pool *)arg)->cur_task--;
			
			//ִ���������ʾ�ľ�������
			current_thread->do_task(current_thread->arg);			
			//�ͷ�������
			free(current_thread);			
			//��ȡ�̻߳�����,����
			pthread_mutex_unlock(&((struct pthread_pool *)arg)->lock);				
			
		} else if(((struct pthread_pool *)arg)->cur_task == 0 && \
			((struct pthread_pool *)arg)->shutdown == 0) {
			
			//��ȡ�̻߳�����,����
			pthread_mutex_unlock(&((struct pthread_pool *)arg)->lock);				
			pthread_cond_wait(&((struct pthread_pool *)arg)->cond,			
				&((struct pthread_pool *)arg)->lock);	
				
		} else if(((struct pthread_pool *)arg)->cur_task == 0 && \
			((struct pthread_pool *)arg)->shutdown == 1) {
				
			//��ȡ�̻߳�����,����
			pthread_mutex_unlock(&((struct pthread_pool *)arg)->lock);				
			pthread_exit(NULL);
			
		} else if(((struct pthread_pool *)arg)->cur_task != 0 && \
			((struct pthread_pool *)arg)->shutdown == 1) {
				
			struct task* current_thread = NULL;				
			//��(struct pthread_pool *)arg)->	task_list��ժ��һ�����
			current_thread = ((struct pthread_pool *)arg)->task_list;
			((struct pthread_pool *)arg)->task_list = \
			((struct pthread_pool *)arg)->task_list->next;
			current_thread->next = NULL;	
			((struct pthread_pool *)arg)->cur_task--;	
			//ִ���������ʾ�ľ�������
			current_thread->do_task(current_thread->arg);			
			//�ͷ�������
			free(current_thread);
			//��ȡ�̻߳�����,����
			pthread_mutex_unlock(&((struct pthread_pool *)arg)->lock);				
			
		}	
	}
}

int init_pool(struct pthread_pool *pool, unsigned int thread_num)
{
	//��ʼ���̳߳�ָ��poolִ�еĽṹ��		
	//��ʼ���̻߳�����
	//��ʼ���߳���������
	//...	
	
	//������,����������������С��Ļ������
	pthread_mutex_init(&pool->lock, NULL);
	//��������,��ʾ��������С��Ƿ��������
	pthread_cond_init(&pool->cond, NULL);	
	pool->shutdown = 0;//��ʾ�����Ƿ��˳�
	//ָ���߳�ID������,�̳߳��������̵߳�tid
	pool->tids = (pthread_t*)malloc(sizeof(pthread_t) * thread_num);
	//�̳߳������ڷ��۵��߳�����,��ǰ�߳�����
	pool->active_threads = thread_num;	
	//�������(����),ָ���һ����Ҫִ�е�����
	//���е��̶߳��Ǵ�����������������ȡ���񡰹�����Դ��	
	pool->task_list = NULL;
	//�̳߳���������е�������������
	pool->max_task = 10;	
	//�̳߳��е�ǰ������е���������
	pool->cur_task = 0;
	
	//����thread_num���߳�,�����е��̶߳���ִ�С�������亯����,���Ҽ�¼ÿһ���̵߳�tid
	int i = 0;
	for (i = 0; i < pool->active_threads; i++)
		pthread_create(&pool->tids[i], NULL, start_routine, (void *)pool);
	
}

int destroy_pool(struct pthread_pool *pool)
{
	//��ȡ�̻߳�����,����
	//�ȴ������������,��shutdown��Ϊ��	
	pthread_mutex_lock(&pool->lock);			
	pool->shutdown = 1;
	pthread_mutex_unlock(&pool->lock);
	
	int i = 0;
	//�����Լ����֣�������ʳ
	//�ͷ����ж�̬�ռ�	
	pthread_mutex_lock(&pool->lock);				
	while (pool->task_list != NULL) {
		struct task* current_thread = pool->task_list;
		pool->task_list = pool->task_list->next;
		current_thread->do_task(current_thread->arg);	
		free(current_thread);
	}
	for (i = 0; i < pool->active_threads; i++)
		pthread_cancel(pool->tids[i]);	
	free(pool->tids);
	pthread_cond_destroy(&pool->cond);	
	pthread_mutex_unlock(&pool->lock);
	pthread_mutex_destroy(&pool->lock);
}

int add_task(struct pthread_pool *pool, void *(*do_task)(void *arg), void *arg)
{
	//�ѵڶ��������͵�����������װ��һ��������(struct task)
	struct task * buf = (struct task*)malloc(sizeof(struct task));
	buf->do_task = do_task;
	buf->arg = arg;
	buf->next = NULL;
	int flag = 0;
	while (flag == 0) {
		//����
		pthread_mutex_lock(&pool->lock);
		if (pool->cur_task <= pool->max_task) {
			//����������뵽�������
			//���������һ��������Դ			
			buf->next = pool->task_list;
			pool->task_list = buf;	
			pool->cur_task++;
			flag = 1;
		} 
		//����			
		pthread_mutex_unlock(&pool->lock);
		//���ѵȴ����߳�			
		pthread_cond_signal(&pool->cond);		
	}			
	return 0;
}

//�������̳߳�������߳�
int add_threads(struct pthread_pool *pool, unsigned int add_threads_num)
{	
	//ԭ���̶߳�Ҫ�ص�
	pthread_mutex_lock(&pool->lock);		
	int i = 0;
	for (i = 0; i < pool->active_threads; i++)
		pthread_cancel(pool->tids[i]);
	pthread_t * new_tids=(pthread_t *)realloc(pool->tids, \
		sizeof(pthread_t) * (pool->active_threads + add_threads_num));
	pool->tids = new_tids;
	pool->active_threads += add_threads_num;
	//����thread_num���߳�,�����е��̶߳���ִ�С�������亯����,���Ҽ�¼ÿһ���̵߳�tid
	for (i = 0; i < pool->active_threads; i++)
		pthread_create(&pool->tids[i], NULL, start_routine, (void *)pool);
	pthread_mutex_unlock(&pool->lock);
}
	
//����һ���̳߳��е��߳�����
int remove_threads(struct pthread_pool *pool, unsigned int remove_threads_num)
{
	//ԭ���̹߳ص�һ����������
	pthread_mutex_lock(&pool->lock);		
	int i = pool->active_threads - 1;
	for (i = pool->active_threads - 1; i > 0 && \
		pool->active_threads - i <= remove_threads_num; i++)
		pthread_cancel(pool->tids[i]);
	pool->active_threads -= remove_threads_num;
	pthread_mutex_unlock(&pool->lock);		
}


void* test_thread1(void*arg)
{
	printf("now ! test_thread1 is start\n");
}

void* test_thread2(void*arg)
{
	printf("now ! test_thread2 is start\n");
}

void* test_thread3(void*arg)
{
	printf("now ! test_thread3 is start\n");
}

int main()
{
	struct pthread_pool pool;
	init_pool(&pool, 10);
	sleep(1);
	
	remove_threads(&pool, 5);
	add_threads(&pool, 15);
	
	add_task(&pool, test_thread1, NULL);
	add_task(&pool, test_thread2, NULL);
	add_task(&pool, test_thread3, NULL);
	
	destroy_pool(&pool);
	return 0;
}

