/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdlib.h>
#include <pthread.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct task {
    void  *next;
    void (*exec)(void *args);
    void  *args;
} ptask_t;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct thread_pool {
    unsigned char   shutdown;
    unsigned int    nums;
    pthread_t      *tids;
    pthread_cond_t  cond;
    pthread_mutex_t lock;
    ptask_t        *head;
    ptask_t        *tail;
    unsigned int    curr;
} pthread_pool_t;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void * ptask_exec(void *args)
{
    pthread_pool_t *pool = args;
    while (1) {
        if (1) {
            pthread_mutex_lock(&pool->lock);
        }
        /* scene 1: if task queue is empty and thread pool is not over, wait condition variable */
        if (pool->curr == 0 && pool->shutdown == 0) {
            pthread_cond_wait(&pool->cond, &pool->lock);
        }
        /* scene 2: if task queue is not empty, take task from task queue and execute */
        if (pool->curr != 0) {
            ptask_t *task = pool->head;
            pool->head = task->next;
            if (pool->head == NULL)
                pool->tail = NULL;
            pool->curr--;
            pthread_mutex_unlock(&pool->lock);
            task->exec(task->args);
            free(task);
        }
        /* scene 3: if task queue is empty and thread pool is over, close current thread */
        if (pool->curr == 0 && pool->shutdown == 1) {
            pthread_mutex_unlock(&pool->lock);
            pthread_exit(NULL);
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void * pthread_pool_create(unsigned int nums)
{
    pthread_pool_t *pool = malloc(sizeof(pthread_pool_t));
    if (pool == NULL)
        return NULL;
    pool->shutdown = 0;
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->cond,  NULL);
    pool->tids = malloc(sizeof(pthread_t) * nums);
    pool->nums = nums;
    pool->head = NULL;
    pool->tail = NULL;
    pool->curr = 0;
    if (pool->tids == NULL) {
        free(pool);
        return NULL;
    }
    for (int index = 0; index < nums; index++)
        pthread_create(&pool->tids[index], NULL, ptask_exec, (void *)pool);
    return pool;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void pthread_pool_destroy(void *instance)
{
    pthread_pool_t *pool = instance;
    pthread_mutex_lock(&pool->lock);
    pool->shutdown = 1;
    pthread_mutex_unlock(&pool->lock);
    for (int index = 0; index < pool->nums; index++)
        pthread_join(pool->tids[index], NULL);
	pthread_cond_destroy(&pool->cond);
	pthread_mutex_destroy(&pool->lock);
    free(pool->tids);
    free(pool);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void pthread_pool_commit(void *instance, void (*exec)(void *args), void *args)
{
    pthread_pool_t *pool = instance;
    ptask_t *task = malloc(sizeof(ptask_t));
    task->exec = exec;
    task->args = args;
    task->next = NULL;
    pthread_mutex_lock(&pool->lock);
    if (pool->tail != NULL)
        pool->tail->next = task;
    if (pool->tail == NULL)
        pool->head = task;
    pool->tail = task;
    pool->curr++;
    pthread_mutex_unlock(&pool->lock);
    pthread_cond_signal(&pool->cond);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
