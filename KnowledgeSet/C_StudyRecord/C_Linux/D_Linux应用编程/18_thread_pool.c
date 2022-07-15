#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct pthread_pool
{
	//互斥锁,用来保护“任务队列”的互斥访问
	pthread_mutex_t lock;
	//条件变量,表示“任务队列”是否存在任务
	pthread_cond_t cond;
	//表示程序是否退出
	bool shutdown;	
	//指向线程ID的数组,线程池中所有线程的tid
	pthread_t *tids;
	//线程池中正在服役的线程数量,当前线程数量
	unsigned int active_threads;
	//任务队列(链表),指向第一个需要执行的任务
	//所有的线程都是从这个任务链表上面获取任务“共享资源”	
	struct task *task_list;
	//线程池中任务队列的最大的任务数量
	unsigned int max_task;	
	//线程池中当前任务队列的任务数量
	unsigned int cur_task;
}pthread_pool;

//描述一个任务
typedef struct task  //任务结点
{
	//任务结点本质上就是用来表示一个任务(函数)cp_file,"函数指针"指向你要完成的函数
	void *(*do_task)(void *arg); //任务结点中的任务,"函数指针"
	void *arg;//指针,执行任务函数需要的参数(文件描述符)
	struct task *next; 
}task;	

void * start_routine(void *arg)
{
	while (1) {	
	
		//获取线程互斥锁,上锁
		pthread_mutex_lock(&((struct pthread_pool *)arg)->lock);
		
		//当线程池未结束的时候,从任务队列中去任务结点
		if(((struct pthread_pool *)arg)->cur_task != 0 && \
			((struct pthread_pool *)arg)->shutdown == 0) {	
	
			struct task* current_thread = NULL;					
			//从(struct pthread_pool *)arg)->task_list中摘出一个结
			current_thread = ((struct pthread_pool *)arg)->task_list;
			if (((struct pthread_pool *)arg)->task_list != NULL)
				((struct pthread_pool *)arg)->task_list = \
				((struct pthread_pool *)arg)->task_list->next;
			current_thread->next = NULL;
			((struct pthread_pool *)arg)->cur_task--;
			
			//执行任务结点表示的具体任务
			current_thread->do_task(current_thread->arg);			
			//释放任务结点
			free(current_thread);			
			//获取线程互斥锁,解锁
			pthread_mutex_unlock(&((struct pthread_pool *)arg)->lock);				
			
		} else if(((struct pthread_pool *)arg)->cur_task == 0 && \
			((struct pthread_pool *)arg)->shutdown == 0) {
			
			//获取线程互斥锁,解锁
			pthread_mutex_unlock(&((struct pthread_pool *)arg)->lock);				
			pthread_cond_wait(&((struct pthread_pool *)arg)->cond,			
				&((struct pthread_pool *)arg)->lock);	
				
		} else if(((struct pthread_pool *)arg)->cur_task == 0 && \
			((struct pthread_pool *)arg)->shutdown == 1) {
				
			//获取线程互斥锁,解锁
			pthread_mutex_unlock(&((struct pthread_pool *)arg)->lock);				
			pthread_exit(NULL);
			
		} else if(((struct pthread_pool *)arg)->cur_task != 0 && \
			((struct pthread_pool *)arg)->shutdown == 1) {
				
			struct task* current_thread = NULL;				
			//从(struct pthread_pool *)arg)->	task_list中摘出一个结点
			current_thread = ((struct pthread_pool *)arg)->task_list;
			((struct pthread_pool *)arg)->task_list = \
			((struct pthread_pool *)arg)->task_list->next;
			current_thread->next = NULL;	
			((struct pthread_pool *)arg)->cur_task--;	
			//执行任务结点表示的具体任务
			current_thread->do_task(current_thread->arg);			
			//释放任务结点
			free(current_thread);
			//获取线程互斥锁,解锁
			pthread_mutex_unlock(&((struct pthread_pool *)arg)->lock);				
			
		}	
	}
}

int init_pool(struct pthread_pool *pool, unsigned int thread_num)
{
	//初始化线程池指针pool执行的结构体		
	//初始化线程互斥锁
	//初始化线程条件变量
	//...	
	
	//互斥锁,用来保护“任务队列”的互斥访问
	pthread_mutex_init(&pool->lock, NULL);
	//条件变量,表示“任务队列”是否存在任务
	pthread_cond_init(&pool->cond, NULL);	
	pool->shutdown = 0;//表示程序是否退出
	//指向线程ID的数组,线程池中所有线程的tid
	pool->tids = (pthread_t*)malloc(sizeof(pthread_t) * thread_num);
	//线程池中正在服役的线程数量,当前线程数量
	pool->active_threads = thread_num;	
	//任务队列(链表),指向第一个需要执行的任务
	//所有的线程都是从这个任务链表上面获取任务“共享资源”	
	pool->task_list = NULL;
	//线程池中任务队列的最大的任务数量
	pool->max_task = 10;	
	//线程池中当前任务队列的任务数量
	pool->cur_task = 0;
	
	//创建thread_num个线程,让所有的线程都是执行“任务分配函数”,并且记录每一个线程的tid
	int i = 0;
	for (i = 0; i < pool->active_threads; i++)
		pthread_create(&pool->tids[i], NULL, start_routine, (void *)pool);
	
}

int destroy_pool(struct pthread_pool *pool)
{
	//获取线程互斥锁,上锁
	//等待所有任务完成,把shutdown置为真	
	pthread_mutex_lock(&pool->lock);			
	pool->shutdown = 1;
	pthread_mutex_unlock(&pool->lock);
	
	int i = 0;
	//还是自己动手，丰衣足食
	//释放所有动态空间	
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
	//把第二个参数和第三个参数封装成一个任务结点(struct task)
	struct task * buf = (struct task*)malloc(sizeof(struct task));
	buf->do_task = do_task;
	buf->arg = arg;
	buf->next = NULL;
	int flag = 0;
	while (flag == 0) {
		//上锁
		pthread_mutex_lock(&pool->lock);
		if (pool->cur_task <= pool->max_task) {
			//把任务结点加入到任务队列
			//任务队列是一个共享资源			
			buf->next = pool->task_list;
			pool->task_list = buf;	
			pool->cur_task++;
			flag = 1;
		} 
		//解锁			
		pthread_mutex_unlock(&pool->lock);
		//唤醒等待的线程			
		pthread_cond_signal(&pool->cond);		
	}			
	return 0;
}

//可以往线程池中添加线程
int add_threads(struct pthread_pool *pool, unsigned int add_threads_num)
{	
	//原有线程都要关掉
	pthread_mutex_lock(&pool->lock);		
	int i = 0;
	for (i = 0; i < pool->active_threads; i++)
		pthread_cancel(pool->tids[i]);
	pthread_t * new_tids=(pthread_t *)realloc(pool->tids, \
		sizeof(pthread_t) * (pool->active_threads + add_threads_num));
	pool->tids = new_tids;
	pool->active_threads += add_threads_num;
	//创建thread_num个线程,让所有的线程都是执行“任务分配函数”,并且记录每一个线程的tid
	for (i = 0; i < pool->active_threads; i++)
		pthread_create(&pool->tids[i], NULL, start_routine, (void *)pool);
	pthread_mutex_unlock(&pool->lock);
}
	
//减少一个线程池中的线程数量
int remove_threads(struct pthread_pool *pool, unsigned int remove_threads_num)
{
	//原有线程关掉一定数量即可
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

