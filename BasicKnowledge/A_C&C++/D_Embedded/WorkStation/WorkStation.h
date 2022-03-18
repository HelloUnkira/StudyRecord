#ifndef WORKSTATION_H
#define WORKSTATION_H
//C std lib
#include <stdint.h>
#include <stdbool.h>



/* 支持静态工作队列,定义此宏,否则注释 */
#define WORKSTATION_SUPPORT_QUEUE_STATIC
/* 支持动态工作队列,定义此宏,否则注释 */
#define WORKSTATION_SUPPORT_QUEUE_DYNAMIC
/* 如果上述功能都不支持,工作队列不参与工程编译 */


#ifndef WORKSTATION_SUPPORT_QUEUE_STATIC
#ifndef WORKSTATION_SUPPORT_QUEUE_DYNAMIC
/* 不使用工作队列时,用于清理源 */
#define WORKSTATION_SUPPORT_NULL
#endif
#endif

/* 头文件提供相应源文件的静态或动态配置项 */
#ifdef  WORKSTATION_C
/*  */
/* 平台字节对齐类型,用于高效内存交互 */
#define WORKSTATION_BYTES_ALIGN   8
#define WORKSTATION_ALIGN       __attribute__ ((aligned (WORKSTATION_BYTES_ALIGN)))

/* 支持静态工作队列时 */
/* 它至少是数据投送时的最大平台字节对齐长度 */
#ifdef  WORKSTATION_SUPPORT_QUEUE_STATIC
#define WORKSTATION_SQUEUE_DATAMAXLEN     1024
#endif

/* 支持动态工作队列时 */
/* 必须需要额外支持数据动态生成接口 */
#ifdef  WORKSTATION_SUPPORT_QUEUE_DYNAMIC
/* 需要配置相应接口的重定向 */
#include <stdlib.h>
#define WORKSTATION_DQUEUE_MALLOC(size)   malloc(size)
#define WORKSTATION_DQUEUE_FREE(pointer)  free(pointer)
#endif
/* 需裁剪项,紧化程序,内部调试时临时使用 */
#include <stdio.h>
#define ERROR_PRINT(target, str) \
    if (target) printf("%s\n",(str));
/*  */
#endif

/* 工作类型:Ready的Type字段使用 */
enum {
    WorkStation_Work1 = 1,   //void Work(void);
    WorkStation_Work2 = 2,   //woid Work(void *Data);
    WorkStation_Work3 = 3,   //woid Work(void *Data, uint32_t Size);
};

#ifndef WORKSTATION_SUPPORT_NULL
/* 获得工作空间占用,用于空间优化配置,正式场景不使用 */
uint32_t WS_Worker_BytesGet(void);
/* 将字节数据流长度转成平台对齐数据流 */
uint32_t WS_Worker_SizesCaculate(uint32_t Size);
#endif

#ifdef WORKSTATION_SUPPORT_QUEUE_STATIC

/* 静态工作队列空间占用预估 */
#define WORKSTATION_SQUEUE_SIZE (24)
/* 获得工作空间占用,用于配置上一参数,正式场景不使用 */
uint32_t WS_SQueue_BytesGet(void);

/* 静态工作队列准备 */
uint32_t WS_SQueue_Ready(void *Queue, void *Zone, uint32_t Size);
/* 静态工作队列就绪一份工作 */
uint32_t WS_SQueue_WorkReady(void *Queue, uint8_t Type, void *Work, void *Data,
                             uint32_t Size);
/* 静态工作队列处理一份工作 */
uint32_t WS_SQueue_WorkRun(void *Queue, void **Work);
#endif

#ifdef WORKSTATION_SUPPORT_QUEUE_DYNAMIC

/* 动态工作队列空间占用预估 */
#define WORKSTATION_DQUEUE_SIZE (20)
/* 获得工作空间占用,用于配置上一参数,正式场景不使用 */
uint32_t WS_DQueue_BytesGet(void);

/* 动态工作队列准备 */
void WS_DQueue_Ready(void *Queue);
/* 动态工作队列就绪一份工作 */
uint32_t WS_DQueue_WorkReady(void *Queue, uint8_t Type, void *Work, void *Data,
                             uint32_t Size);
/* 动态工作队列处理一份工作 */
void * WS_DQueue_WorkRun(void *Queue, void **Work);
#endif

#endif
