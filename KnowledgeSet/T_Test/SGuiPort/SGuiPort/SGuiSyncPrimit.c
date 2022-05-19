/*实现目标:
 *一个集合化的同步原语实现
 *因为同步原语对平台要求
 *所以每一个平台都需要指定同步原语实现及其内容
 *某些不支持的同步原语,可选用支持的同步原语
 *作为临时的替代,用以保证兼容性
 */

/* 核心唯一依赖项 */
#include "SGuiPort.h"

/* 平台依赖的同步原语 */
#if SGUI_USE_LINUX

#include <pthread.h>
#include <semaphore.h>

/* 添加同步原语 */
/* 同步原语本身就依赖它去获得动态空间 */
/* 所以内部再次启用同步原语就会形成死递归 */
/* 此处直接实现系统的接口去完成该同步原语的功能 */
void SGuiMemoryCheckOptSync(bool StartOrEnd)
{
    /* 使用一个静态资源 */
    static pthread_mutex_t SGuiMemoryCheckMutex = {0};
    /* 同步原语的初始化,启用,终止,都统一加到此处 */
    static bool SGuiMemoryCheckSyncPrimitIsInit   = false;
    static bool SGuiMemoryCheckSyncPrimitIsDeInit = false;
    /* 同步原语的初始化 */
    if (SGuiMemoryCheckSyncPrimitIsInit == false) {
        SGuiMemoryCheckSyncPrimitIsInit  = true;

        pthread_mutex_init(&SGuiMemoryCheckMutex, NULL);

    }
    /* 同步原语的反初始化(什么时候才算正式放弃使用句柄资源了呢?) */
    if (SGuiMemoryCheckSyncPrimitIsDeInit == true) {

        pthread_mutex_destroy(&SGuiMemoryCheckMutex);

    }
    /* 获取互斥锁 */
    if (StartOrEnd == true) {

        pthread_mutex_lock(&SGuiMemoryCheckMutex);

    }
    /* 释放互斥锁 */
    if (StartOrEnd == false) {

        pthread_mutex_unlock(&SGuiMemoryCheckMutex);

    }
}

SGuiSyncPrimit * SGuiSyncPrimitCreate(uint32_t type)
{
    /* 生成同步原语空间 */
    SGuiSyncPrimit * primit = SGUIMALLOC(sizeof(SGuiSyncPrimit));
    /* 初始化 */
    primit->resource = NULL;
    /* 生成指定原语 */
    switch (primit->type = type) {
    case SGUIMUTEX:
        /* 生成互斥锁空间 */
        primit->mutex = SGUIMALLOC(sizeof(pthread_mutex_t));
        /* 初始化互斥锁 */
        pthread_mutex_init(primit->mutex, NULL);
        break;
    case SGUISEMAPHONE:
        /* 生成信号量空间 */
        primit->semaphone = SGUIMALLOC(sizeof(sem_t));
        /* 初始化信号量 */
        sem_init(primit->semaphone, 0, 20);
        break;
    default:
        primit->primitive = NULL;
        primit->type = 0;
    }
    /*  */
    return primit;
}

void SGuiSyncPrimitDestory(SGuiSyncPrimit *primit)
{
    /* 销毁指定原语 */
    switch (primit->type) {
    case SGUIMUTEX:
        /* 销毁互斥锁 */
        pthread_mutex_destroy(primit->mutex);
        /* 销毁互斥锁空间 */
        SGUIFREE(primit->mutex);
        break;
    case SGUISEMAPHONE:
        /* 销毁信号量 */
        sem_destroy(primit->semaphone);
        /* 销毁信号量空间 */
        SGUIFREE(primit->semaphone);
        break;
    }
    /* 销毁同步原语空间 */
    SGUIFREE(primit);
}

void * SGuiSyncPrimitTakeResource(SGuiSyncPrimit *primit)
{
    /* 获取同步原语 */
    switch (primit->type) {
    case SGUIMUTEX:
        if (pthread_mutex_lock(primit->mutex) != -1)
            return primit->resource;
    case SGUISEMAPHONE:
        if (sem_wait(primit->semaphone) != -1)
            return primit->resource;
    }
    return NULL;
}

void SGuiSyncPrimitGiveResource(SGuiSyncPrimit *primit)
{
    /* 释放同步原语 */
    switch (primit->type) {
    case SGUIMUTEX:
        pthread_mutex_unlock(primit->mutex);
        break;
    case SGUISEMAPHONE:
        sem_post(primit->semaphone);
        break;
    }
}

void SGuiSyncPrimitSetResource(SGuiSyncPrimit * primit, void *resource)
{
    SGuiSyncPrimitTakeResource(primit);
    primit->resource = resource;
    SGuiSyncPrimitGiveResource(primit);
}

/* 继续添加其余平台
#elif SGUI_USE_UNKNOWN
*/
#else

/* 给与一个简单的告知 */
#warning "unknown platform synchronization primitive"
/* 某些环境下不需要刻意使用同步原语(比如单线程) */

void SGuiMemoryCheckOptSync(bool StartOrEnd)
{
}

SGuiSyncPrimit * SGuiSyncPrimitCreate(uint32_t type)
{
    return NULL;
}

void SGuiSyncPrimitDestory(SGuiSyncPrimit * primit)
{
}

void * SGuiSyncPrimitTakeResource(SGuiSyncPrimit * primit)
{
    return NULL;
}

void SGuiSyncPrimitGiveResource(SGuiSyncPrimit * primit)
{
}

void SGuiSyncPrimitSetResource(SGuiSyncPrimit * primit, void *resource)
{
}

#endif






