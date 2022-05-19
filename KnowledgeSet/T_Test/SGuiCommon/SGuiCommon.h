#ifndef SGUICOMMON_H
#define SGUICOMMON_H
/* 标准库依赖 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
/* 平台依赖 */
#include "SGuiPort.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 链表宏迭代器(可改写为静态内联函数提升效率) */
#include "SGuiIterator.h"
/* 上述的声明及其对应的源可以被单独提取到module中 */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 将一个空闲单元加入节点,并加入到链表(使用最小化索引) */
void SGuiSNodeUnitAdd(SGuiSNode *head, SGuiSNode *tail, void *unit);
/* 将一个空闲单元移除节点,并移除出链表(使用最小化索引) */
void SGuiSNodeUnitRemove(SGuiSNode *head, SGuiSNode *tail, void *unit);
/* 将一个空闲单元寻找到(使用最小化索引) */
void * SGuiSNodeUnitSearch(SGuiSNode *head, SGuiSNode *tail, uint32_t index);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 同步原语互斥锁要导入到源中所使用的变量 */
/* 同步原语的初始化,启用,终止,都统一加到此处 */
static bool             SGuiSyncPrimitOptSyncMutex_Init     = false;
static bool             SGuiSyncPrimitOptSyncMutex_DeInit   = false;
static uint8_t          SGuiSyncPrimitOptSyncMutex_Variable = 0xA5;
static SGuiSyncPrimit  *SGuiSyncPrimitOptSyncMutex_Mutex    = NULL;
/* 同步原语集成化操作接口 */
void SGuiSyncPrimitOptSyncMutex(SGuiSyncPrimit **Mutex,
                                void            *Variable,
                                bool            *Init,
                                bool            *DeInit,
                                bool             StartOrEnd);
/* 操作同步原语(true:启用保护,false:禁用保护) */
#define SGUISYNCPRIMITOPTSYNCMUTEX(StartOrEnd)                              \
        SGuiSyncPrimitOptSyncMutex(&SGuiSyncPrimitOptSyncMutex_Mutex,       \
                                   &SGuiSyncPrimitOptSyncMutex_Variable,    \
                                   &SGuiSyncPrimitOptSyncMutex_Init,        \
                                   &SGuiSyncPrimitOptSyncMutex_DeInit,      \
                                    StartOrEnd)
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*实现目标:
 *一些通用的宏配置或内联功能
 */
#define SGUIMIN(data1, data2) ((data1) <= (data2) ? (data1) : (data2))
#define SGUIMAX(data1, data2) ((data1) >= (data2) ? (data1) : (data2))
#define SGUIDIS(data1, data2) (SGUIMAX(data1, data2) - SGUIMIN(data1, data2))
#endif
