#include "SGui_Kernel.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if 0
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#elif SGUI_USE_LINUX
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
pthread_t       ThreadEngine;
pthread_mutex_t EventSyncMutex;
sem_t           EventSyncSem;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void SGui_ThreadEventSyncLock(void)
{
    pthread_mutex_lock(&EventSyncMutex);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void SGui_ThreadEventSyncUnlock(void)
{
    pthread_mutex_unlock(&EventSyncMutex);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void SGui_ThreadEventSyncNotify(void)
{
    sem_post(&EventSyncSem);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void SGui_ThreadEventSyncWait(void)
{
    sem_wait(&EventSyncSem);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void * SGui_ThreadExecuteTask(void *Pointer)
{
    SGui_EngineExecute();
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ThreadReady(void)
{
    SGui_EventSyncRegister(SGui_ThreadEventSyncLock,
                           SGui_ThreadEventSyncUnlock,
                           SGui_ThreadEventSyncNotify,
                           SGui_ThreadEventSyncWait);
    SGui_EngineReady();
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ThreadExecute(void)
{
    #if SGUI_INTERNAL_TEST
    // SGui_Handle_Test();
    // SGui_ClipRegion1_Test();
    // SGui_ClipRegion2_Test();
    // SGui_Event_Test();
    // SGui_Animation_Test();
    #endif

    pthread_mutex_init(&EventSyncMutex, NULL);
    sem_init(&EventSyncSem, 0, 5);
    
    pthread_create(&ThreadEngine, NULL, SGui_ThreadExecuteTask, NULL);
    pthread_join(ThreadEngine, NULL);
    
    sem_destroy(&EventSyncSem);
    pthread_mutex_destroy(&EventSyncMutex);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//继续添加其他平台
//#elif SGUI_USE_UNKONWN
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#else
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 事件同步锁(互斥锁:上锁) */
static void SGui_ThreadEventSyncLock(void)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 事件同步锁(互斥锁:解锁) */
static void SGui_ThreadEventSyncUnlock(void)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 事件同步信号(信号量:通知) */
static void SGui_ThreadEventSyncNotify(void)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 事件同步信号(信号量:等待) */
static void SGui_ThreadEventSyncWait(void)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ThreadReady(void)
{
    SGui_EventSyncRegister(SGui_ThreadEventSyncLock,
                           SGui_ThreadEventSyncUnlock,
                           SGui_ThreadEventSyncNotify,
                           SGui_ThreadEventSyncWait);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ThreadExecute(void)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
