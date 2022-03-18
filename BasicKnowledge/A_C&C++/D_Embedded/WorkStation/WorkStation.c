//C std lib
#include <stdint.h>
#include <stdbool.h>

#define   WORKSTATION_C
#include "WorkStation.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 辅助功能:泛型平台对齐字节拷贝 */
static inline void WorkStation_Copy(void *Target, void *Source, uint32_t Size)
{
    uint32_t index = 0;
    /* 首字节对齐检查 */
    if ((((uint8_t)Target) & (WORKSTATION_BYTES_ALIGN - 1)) != 0)
        ERROR_PRINT(true, "WorkStation_Copy: Target");
    if ((((uint8_t)Source) & (WORKSTATION_BYTES_ALIGN - 1)) != 0)
        ERROR_PRINT(true, "WorkStation_Copy: Source");
    /* 平台字节对齐拷贝 */
#define WORKSTATION_BYTES_ALIGN_COPY(Bits)            \
{   /* 使用宏连接写一个简要的泛型拷贝即可 */        \
    uint##Bits##_t *Target##Bits = Target;          \
    uint##Bits##_t *Source##Bits = Source;          \
    for (index = 0; index < Size; index++)          \
        Target##Bits[index] = Source##Bits[index];  \
}                                                   \


#if 0
    /* 暂时还未出现其他位平台的通用设备 */
#elif WORKSTATION_BYTES_ALIGN == 8
    WORKSTATION_BYTES_ALIGN_COPY(64);
#elif WORKSTATION_BYTES_ALIGN == 4
    WORKSTATION_BYTES_ALIGN_COPY(32);
#elif WORKSTATION_BYTES_ALIGN == 2
    WORKSTATION_BYTES_ALIGN_COPY(16);
#elif WORKSTATION_BYTES_ALIGN == 1
    WORKSTATION_BYTES_ALIGN_COPY(8);
#else
#error "WorkStation_Copy:WORKSTATION_BYTES_ALIGN  Error"
#endif 
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifndef WORKSTATION_SUPPORT_NULL
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 工作抽象(该结构需完全满足平台字节对齐): */
#pragma pack(WORKSTATION_BYTES_ALIGN)
typedef struct WorkStation_WorkerBody {
    /* 工作支持的执行处理类型 */
    union {
        /* 抽象工作 */
        void *Work;
        /* 工作类型1:无参类型 */
        void (*Work1)(void);
        /* 工作类型2:单参类型 */
        void (*Work2)(void *Data);
        /* 工作类型3:合参类型 */
        void (*Work3)(void *Data, uint32_t Size);
    };
    /* 工作基本情况 */
    void    *Data;  //工作执行时需要的数据地址
    uint32_t Size;  //工作执行时需要的数据大小
    uint8_t  Type;  //工作执行的类型(1~n)
} WS_Worker;
#pragma pack()
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 抽象部分接口 */
static inline void WS_Worker_GetSize(WS_Worker *Worker, uint32_t *Size)
{
    *Size = Worker->Size;
}
static inline void WS_Worker_SetSize(WS_Worker *Worker, uint32_t Size)
{
    Worker->Size = Size;
}
static inline void WS_Worker_GetData(WS_Worker *Worker, void **Data)
{
    *Data = Worker->Data;
}
static inline void WS_Worker_SetData(WS_Worker *Worker, void *Data)
{
    Worker->Data = Data;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 获得工作空间占用,用于空间优化配置,正式场景不使用 */
uint32_t WS_Worker_BytesGet(void)
{
    if (((sizeof(WS_Worker)) & (WORKSTATION_BYTES_ALIGN - 1)) != 0)
        ERROR_PRINT(true, "WS_Worker_BytesGet: sizeof");
    return sizeof(WS_Worker);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
uint32_t WS_Worker_SizesCaculate(uint32_t Size)
{
    return ((Size / WORKSTATION_BYTES_ALIGN)) +
           ((Size % WORKSTATION_BYTES_ALIGN) ? 1 : 0);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 配置一份工作 */
static inline void WS_Worker_Ready(WS_Worker *Worker, void *Work, uint8_t Type)
{
    Worker->Work = Work;
    Worker->Type = Type;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 完成一份工作 */
static inline void * WS_Worker_Run(WS_Worker *Worker)
{
    switch (Worker->Type) {
    case WorkStation_Work1:
        if (Worker->Work1 != NULL) {
            Worker->Work1();
            return Worker->Work;
        }
    case WorkStation_Work2:
        if (Worker->Work2 != NULL) {
            Worker->Work2(Worker->Data);
            return Worker->Work;
        }
    case WorkStation_Work3:
        if (Worker->Work3 != NULL) {
            Worker->Work3(Worker->Data, Worker->Size);
            return Worker->Work;
        }
    }
    return NULL;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#ifdef WORKSTATION_SUPPORT_QUEUE_STATIC
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 静态工作队列必须要得到该支持 */
#if 0
#elif WORKSTATION_BYTES_ALIGN == 8
    //8字节对齐静态工作队列缓冲区
    uint64_t *Temp[WORKSTATION_SQUEUE_DATAMAXLEN] WORKSTATION_ALIGN = {0};
#elif WORKSTATION_BYTES_ALIGN == 4
     //4字节对齐静态工作队列缓冲区
    uint32_t *Temp[WORKSTATION_SQUEUE_DATAMAXLEN] WORKSTATION_ALIGN = {0};
#elif WORKSTATION_BYTES_ALIGN == 2
    //2字节对齐静态工作队列缓冲区
    uint16_t *Temp[WORKSTATION_SQUEUE_DATAMAXLEN] WORKSTATION_ALIGN = {0};
#elif WORKSTATION_BYTES_ALIGN == 1
    //1字节对齐静态工作队列缓冲区
    uint8_t  *Temp[WORKSTATION_SQUEUE_DATAMAXLEN] WORKSTATION_ALIGN = {0};
#else
#error "Temp:WORKSTATION_BYTES_ALIGN  Error"
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 静态工作队列 */
typedef struct WorkStation_StaticQueueBody {
#if 0
#elif WORKSTATION_BYTES_ALIGN == 8
    uint64_t *Zone; //8字节对齐静态工作队列缓冲区
#elif WORKSTATION_BYTES_ALIGN == 4
    uint32_t *Zone; //4字节对齐静态工作队列缓冲区
#elif WORKSTATION_BYTES_ALIGN == 2
    uint16_t *Zone; //2字节对齐静态工作队列缓冲区
#elif WORKSTATION_BYTES_ALIGN == 1
    uint8_t  *Zone; //1字节对齐静态工作队列缓冲区
#else
#error "WS_SQueue:WORKSTATION_BYTES_ALIGN  Error"
#endif
    uint32_t Size;  //静态工作队列平台字节对齐条目
    uint32_t Mask;  //用于加速拷贝时的掩码
    uint32_t Head;  //读索引
    uint32_t Tail;  //写索引
} WS_SQueue;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 获得工作空间占用,用于空间优化配置,正式场景不使用 */
uint32_t WS_SQueue_BytesGet(void)
{
    return sizeof(WS_SQueue);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 内部使用的辅助 */

/* 静态工作队列使用到的算法 */
#define WQ_SQUEUE_MAXLENGTH     0x80000000

/* 索引求模运算 */
static inline uint32_t WS_SQueue_Modulo(WS_SQueue *SQueue, uint32_t Value)
{
    if (SQueue->Mask != 0)
        return (Value & SQueue->Mask);
    if (SQueue->Mask == 0)
        return (Value % SQueue->Size);
}

/* 索引回退 */
static inline void WS_SQueue_RewindIndex(WS_SQueue *SQueue)
{
    // y = a * x + b ==> b = y - a * x
    
    if (SQueue->Head >= WQ_SQUEUE_MAXLENGTH) {
        
        uint32_t Rewind = WQ_SQUEUE_MAXLENGTH;
        Rewind -= SQueue->Size;
        Rewind *= SQueue->Size;
        
        SQueue->Head -= Rewind;
        SQueue->Tail -= Rewind;
    }
}

/* 静态工作队列获取已有条目 */
static inline uint32_t WS_SQueue_GetItem(WS_SQueue *SQueue)
{
    return SQueue->Tail - SQueue->Head;
}

/* 静态工作队列获取可用条目 */
static inline uint32_t WS_SQueue_GetSpace(WS_SQueue *SQueue)
{
    return SQueue->Size - (SQueue->Tail - SQueue->Head);
}

/* 静态工作队列读取平台字节对齐数据流 */
static inline void WS_SQueue_Gets(WS_SQueue *SQueue, void *Data, uint32_t Size)
{
    uint32_t index = 0;
    uint32_t index1 = 0;
    /* 检查是否还存在数据流 */
    if (WS_SQueue_GetItem(SQueue) == 0 ||
        WS_SQueue_GetItem(SQueue)  < Size)
        return;
    /* 泛型数据拷贝 */
#define WS_SQueue_Gets_Copy(Bits, Bytes)                            \
    uint##Bits##_t *Data##Bytes = Data;                             \
    for (index = 0; index < Size; index++) {                        \
        index1 = WS_SQueue_Modulo(SQueue, SQueue->Head + index);    \
        Data##Bytes[index] = SQueue->Zone[index1];                  \
    }                                                               \


#if 0
#elif WORKSTATION_BYTES_ALIGN == 8
    WS_SQueue_Gets_Copy(64, 8);
#elif WORKSTATION_BYTES_ALIGN == 4
    WS_SQueue_Gets_Copy(32, 4);
#elif WORKSTATION_BYTES_ALIGN == 2
    WS_SQueue_Gets_Copy(16, 2);
#elif WORKSTATION_BYTES_ALIGN == 1
    WS_SQueue_Gets_Copy(8,  1);
#else
#error "WS_SQueue_Gets:WORKSTATION_BYTES_ALIGN  Error"
#endif

    SQueue->Head += Size;
    //索引回退
    WS_SQueue_RewindIndex(SQueue);
}

/* 静态工作队列写入平台字节对齐数据流 */
static inline void WS_SQueue_Puts(WS_SQueue *SQueue, void *Data, uint32_t Size)
{
    uint32_t index = 0;
    uint32_t index1 = 0;
    /* 检查是否还存在空闲空间 */
    if (WS_SQueue_GetSpace(SQueue) == 0 ||
        WS_SQueue_GetSpace(SQueue)  < Size)
        return;
    /* 泛型数据拷贝 */
#define WS_SQueue_Puts_Copy(Bits, Bytes)                            \
    uint##Bits##_t *Data##Bytes = Data;                             \
    for (index = 0; index < Size; index++) {                        \
        index1 = WS_SQueue_Modulo(SQueue, SQueue->Tail + index);    \
        SQueue->Zone[index1] = Data##Bytes[index];                  \
    }                                                               \


#if 0
#elif WORKSTATION_BYTES_ALIGN == 8
    WS_SQueue_Puts_Copy(64, 8);
#elif WORKSTATION_BYTES_ALIGN == 4
    WS_SQueue_Puts_Copy(32, 4);
#elif WORKSTATION_BYTES_ALIGN == 2
    WS_SQueue_Puts_Copy(16, 2);
#elif WORKSTATION_BYTES_ALIGN == 1
    WS_SQueue_Puts_Copy(8,  1);
#else
#error "WS_SQueue_Puts:WORKSTATION_BYTES_ALIGN  Error"
#endif

    SQueue->Tail += Size;
    //索引回退
    WS_SQueue_RewindIndex(SQueue);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 静态工作队列准备 */
uint32_t WS_SQueue_Ready(void *Queue, void *Zone, uint32_t Size)
{
    WS_SQueue *SQueue = Queue;
    /* 简要的字节对齐修正 */
    /* 地址截断,通过最后几位确定是否字节对齐 */
    uint8_t Offset = ((uint8_t)Zone) & (WORKSTATION_BYTES_ALIGN - 1);
    /* 简单检查一下字节对齐情况 */
    if (Offset != 0) {
        Zone   += WORKSTATION_BYTES_ALIGN - Offset;
        Size   -= 1;
    }

    /* 配置静态工作队列 */
    SQueue->Zone = Zone;
    SQueue->Size = Size;
    SQueue->Head = 0;
    SQueue->Tail = 0;
    SQueue->Mask = 0;
    /* 字节对齐寻址计算加速 */
    if ((Size != 0) && ((Size & (Size - 1)) == 0))
        SQueue->Mask = (Size - 1);
    
    return Size;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 静态工作队列就绪一份工作 */
uint32_t WS_SQueue_WorkReady(void *Queue, uint8_t Type, void *Work, void *Data,
                             uint32_t Size)
{
    WS_SQueue *SQueue = Queue;
    /* 检查是否还能就绪工作 */
    if (WS_SQueue_GetSpace(SQueue) == 0 ||
        WS_SQueue_GetSpace(SQueue) < Size + sizeof(WS_Worker))
        return -1;
    /* 准备一份工作 */
    WS_Worker Worker WORKSTATION_ALIGN = {0};
    WS_Worker_SetSize(&Worker, Size);
    WS_Worker_Ready(&Worker, Work, Type);
    /* 将该工作投递到队列中 */
    WS_SQueue_Puts(SQueue, &Worker, sizeof(WS_Worker) / WORKSTATION_BYTES_ALIGN);
    /* 将暂存区数据投递到队列中 */
    WS_SQueue_Puts(SQueue, Data, Size);
    /* 反馈当前负载 */
    return WS_SQueue_GetItem(SQueue);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 静态工作队列处理一份工作 */
uint32_t WS_SQueue_WorkRun(void *Queue, void **Work)
{
    uint32_t Size = 0;
    WS_SQueue *SQueue = Queue;
    /* 检查是否还存在工作 */
    if (WS_SQueue_GetSpace(SQueue) == 0)
        return -1;
    /* 准备一份工作 */
    WS_Worker Worker WORKSTATION_ALIGN = {0};
    /* 将该工作提出从队列中 */
    WS_SQueue_Gets(SQueue, &Worker, sizeof(WS_Worker) / WORKSTATION_BYTES_ALIGN);
    /* 获得工作明细 */
    WS_Worker_GetSize(&Worker, &Size);
    /* 将暂存区数据取出从队列中 */
    WS_SQueue_Gets(SQueue, Temp, Size);
    /* 更新工作明细 */
    WS_Worker_SetData(&Worker, Temp);
    /* 处理一份工作 */
    *Work = WS_Worker_Run(&Worker);
    /* 反馈当前负载 */
    return WS_SQueue_GetItem(SQueue);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#ifdef WORKSTATION_SUPPORT_QUEUE_DYNAMIC
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 动态工作队列必须要得到该支持 */
static inline void * WS_DQueue_Malloc(uint32_t size)
{
#ifndef WORKSTATION_DQUEUE_MALLOC(size)
#error  "Dynamic work queues require dynamic memory policies"
#endif
    return WORKSTATION_DQUEUE_MALLOC(size);
}
static inline void WS_DQueue_Free(void *pointer)
{
#ifndef WORKSTATION_DQUEUE_FREE(pointer)
#error  "Dynamic work queues require dynamic memory policies"
#endif
    WORKSTATION_DQUEUE_FREE(pointer);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 动态工作队列 */
typedef struct WorkStation_DynamicWorkerBody {
    struct WorkStation_DynamicWorkerBody *Next;
    struct WorkStation_WorkBody          *Self;
} WS_DWorker;

typedef struct WorkStation_DynamicQueueBody {
    struct WorkStation_DynamicWorkerBody *Head;
    struct WorkStation_DynamicWorkerBody *Tail;
    uint32_t Size;
} WS_DQueue;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 获得工作空间占用,用于空间优化配置,正式场景不使用 */
uint32_t WS_DQueue_BytesGet(void)
{
    return sizeof(WS_DQueue);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 动态工作队列准备 */
void WS_DQueue_Ready(void *Queue)
{
    WS_DQueue *DQueue = Queue;
    
    DQueue->Head = NULL;
    DQueue->Tail = NULL;
    DQueue->Size = 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 动态工作队列就绪一份工作 */
uint32_t WS_DQueue_WorkReady(void *Queue, uint8_t Type, void *Work, void *Data,
                             uint32_t Size)
{
    WS_DQueue   *DQueue = Queue;
    /* 动态生成一份工作载体 */
    WS_DWorker *DWorker = WS_DQueue_Malloc(sizeof(WS_DWorker));
    /* 动态生成一份工作 */
    WS_Worker   *Worker = WS_DQueue_Malloc(sizeof(WS_Worker));
    /* 动态生成数据暂存区 */
    void          *Temp = WS_DQueue_Malloc(WORKSTATION_BYTES_ALIGN * Size);
    /* 简要的检查 */
    if (DWorker == NULL || Worker == NULL || Temp == NULL) {
        WS_DQueue_Free(DWorker);
        WS_DQueue_Free(Worker);
        WS_DQueue_Free(Temp);
        return -1;
    }
    /* 将该工作使用到的数据放入暂存区 */
    WorkStation_Copy(Temp, Data, Size);
    /* 就绪一份工作 */
    WS_Worker_SetSize(Worker, Size);
    WS_Worker_SetData(Worker, Temp);
    WS_Worker_Ready(Worker, Work, Type);
    /* 工作与载体绑定 */
    DWorker->Self = Worker;
    DWorker->Next = NULL;
    /* 将载体加入到工作队列 */
    if (DQueue->Size == 0) {
        DQueue->Head = DWorker;
        DQueue->Tail = DWorker;
    } else {
        DQueue->Tail->Next = DWorker;
        DQueue->Tail       = DWorker;
    }
    /* 更新当前负载 */
    DQueue->Size++;
    /* 反馈当前负载 */
    return DQueue->Size;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 动态工作队列处理一份工作 */
void * WS_DQueue_WorkRun(void *Queue, void **Work)
{
    void *Data = NULL;
    WS_DQueue *DQueue = Queue;
    /* 检查是否还存在工作 */
    if (DQueue->Size == 0)
        return NULL;
    /* 动态获取一份工作载体 */
    WS_DWorker *DWorker = DQueue->Head;
    /* 动态获取一份工作 */
    WS_Worker    *Worker  = DWorker->Self;
    /* 更新工作队列 */
    DQueue->Head = DQueue->Head->Next;
    /* 更新当前负载 */
    DQueue->Size--;
    /* 处理一份工作 */
    *Work = WS_Worker_Run(Worker);
    /* 获取数据暂存区 */
    WS_Worker_GetData(Worker, &Data);
    /* 回收数据暂存区 */
    WS_DQueue_Free(Data);
    /* 回收工作 */
    WS_DQueue_Free(Worker);
    /* 回收工作载体 */
    WS_DQueue_Free(DWorker);
    /* 反馈工作标记 */
    return Work;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
