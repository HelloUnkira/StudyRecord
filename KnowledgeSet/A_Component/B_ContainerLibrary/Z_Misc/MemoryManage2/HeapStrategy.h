#ifndef HEAP_STRATEGY_H
#define HEAP_STRATEGY_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 该实现参考于zephyr\lib\os */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if 1
/* 内部使用的裁剪宏策略 */
/* 及时关注该信息有助于找到内存泄漏和溢出问题 */
#include <stdio.h>
#define MMHS_Visit                          printf
#define MMHS_Print(File, Line, Express)     printf("Error:[%s][%d][%s]\n", File, Line, Express)
#define MMHS_Check(Express)                 if (Express) MMHS_Print(__FILE__, __LINE__, #Express)
#else
#define MMHS_Visit
#define MMHS_Check(Express)
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct HeapStrategyDistributor {
    uintptr_t Manage;
    uintptr_t Memory;
    uintptr_t Bytes;
} HeapStrategy;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
int8_t MMHS_Free(HeapStrategy *Heap, void *Memory);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void * MMHS_Alloc(HeapStrategy *Heap, uintptr_t Bytes);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
int8_t MMHS_Init(HeapStrategy *Instance, void *Memory, uintptr_t Bytes);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void MMHS_LayoutCheck(HeapStrategy *Instance);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
