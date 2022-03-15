#ifndef MEMORYMANAGEINTERFACE_H
#define MEMORYMANAGEINTERFACE_H
//C std lib
#include <stdint.h>
#include <stdbool.h>

/*提供给上层的接口:
 *用户只可以使用这里的接口
 */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
typedef void (*MemoryManageCheckPrint)(const char *title, uint32_t content);
//备注:因为编码是在notepad++中完成
//写以下宏主要是为了用于折叠代码,同样,可用于裁剪功能
#define MemoryManageStrategySlab
#define MemoryManageStrategyBuddy
#define MemoryManageStrategyHeap
//平台内存管理监控
#define MemoryManageCheck       1
//抽象打印接口,定参打印
typedef void (*MemoryManageCheckPrint)(const char *title, uint32_t content);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void MemoryManageConfigure(void);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef MemoryManageStrategySlab
//从分配器集合获取指定大小的块
bool MemoryManageSlabTake(uint32_t size, uint8_t **block);
//给分配器集合归还指定大小的块
bool MemoryManageSlabGive(uint32_t size, uint8_t *block);
//使用情况统计
#if MemoryManageCheck == 1
void MemoryManageSlabCheck(MemoryManageCheckPrint print);
void MemoryManageSlabMem(uint32_t index, uint8_t **start, uint8_t **end);
#endif
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef MemoryManageStrategyBuddy
//从分配器集合获取指定大小的块
bool MemoryManageBuddyTake(uint32_t size, uint8_t **block);
//给分配器集合归还指定大小的块
bool MemoryManageBuddyGive(uint32_t size, uint8_t *block);
//使用情况统计
#if MemoryManageCheck == 1
void MemoryManageBuddyCheck(MemoryManageCheckPrint print);
void MemoryManageBuddyMem(uint32_t index, uint8_t **start, uint8_t **end);
#endif
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef MemoryManageStrategyHeap
//从分配器集合获取指定大小的块
bool MemoryManageHeapTake(uint32_t size, uint8_t **block);
//给分配器集合归还指定大小的块
bool MemoryManageHeapGive(uint32_t size, uint8_t *block);
//使用情况统计
#if MemoryManageCheck == 1
void MemoryManageHeapCheck(MemoryManageCheckPrint print);
void MemoryManageHeapMem(uint32_t index, uint8_t **start, uint8_t **end);
#endif
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
