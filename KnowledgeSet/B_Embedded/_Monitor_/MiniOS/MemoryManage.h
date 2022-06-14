#ifndef MEMORYMANAGE_H
#define MEMORYMANAGE_H
//C std lib
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
//接口层提供部分明细
#include "MemoryManageInterface.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 平台配置 */
//平台字节对齐
#define MemoryManageAlignment   8
#define MemoryManageAlign       __attribute__ ((aligned (MemoryManageAlignment)))
//如需指定地址(添加指定地址修饰)
#define MemoryManageAddr(addr)  __attribute__ ((at (addr)))
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef MemoryManageStrategySlab
/* Slab分配器 */
typedef struct MemoryManageSlab {
    uint8_t *MemoryStart;  //管理区起始地址
    uint8_t *MemoryEnd;    //管理区结束地址
    uint8_t *BlockList;    //空闲块链表集
    uint32_t BlockSize;    //块大小
    uint32_t BlockNumber;  //块数量
#if MemoryManageCheck == 1
    uint32_t BlockUsed;    //使用字节明细
#endif
} SlabDistributor;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//配置分配器集合
#define SlabDistributorDefineAll(type)              \
    static SlabDistributor SlabSet[type] = {0};     \
    static uint32_t SlabDistributorType  = type;    \
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//配置单个分配器
#define SlabDistributorDefine(index, size, number)          \
{                                                           \
    static uint8_t MemoryManageAlign                        \
    SlabDistributor##index[size * number] = {0};            \
    SlabSet[index].BlockList   = SlabDistributor##index;    \
    SlabSet[index].BlockSize   = size;                      \
    SlabSet[index].BlockNumber = number;                    \
}                                                           \
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//配置单个分配器(指定地址)
#define SlabDistributorDefineOrient(index, size, number, addr)  \
{                                                               \
    static uint8_t MemoryManageAddr(addr)                       \
    SlabDistributor##index[size * number] = {0};                \
    SlabSet[index].BlockList   = SlabDistributor##index;        \
    SlabSet[index].BlockSize   = size;                          \
    SlabSet[index].BlockNumber = number;                        \
}                                                               \
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef MemoryManageStrategyBuddy
/* Buddy分配器 */
typedef struct MemoryManageBuddy {
    uint8_t  *MemoryStart; //管理区起始地址
    uint8_t  *MemoryEnd;   //管理区结束地址
    uint8_t  *BlockOffset; //管理区基地址偏移
    uint8_t **BlockList;   //空闲块链表集合
    uint32_t  BlockSize;   //最小块大小因子
    uint32_t  BlockFactor; //倍频因子
    uint32_t  BlockNumber; //最小块数量
#if MemoryManageCheck == 1
    uint32_t  BlockUsed;   //使用字节明细
#endif
} BuddyDistributor;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//配置分配器集合
#define BuddyDistributorDefineAll(type)             \
    static BuddyDistributor BuddySet[type] = {0};   \
    static uint32_t BuddyDistributorType  = type;   \
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//配置单个分配器
#define BuddyDistributorDefine(index, min, max, number)                 \
{                                                                       \
    static uint8_t  MemoryManageAlign                                   \
    BuddyDistributor##index[(1 << (max)) * number] = {0};               \
    static uint8_t *BuddyDistributorList##index[max - min + 1] = {0};   \
    BuddySet[index].BlockOffset = BuddyDistributor##index;              \
    BuddySet[index].BlockList   = BuddyDistributorList##index;          \
    BuddySet[index].BlockSize   = min;                                  \
    BuddySet[index].BlockNumber = number;                               \
    BuddySet[index].BlockFactor = max - min;                            \
}                                                                       \
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//配置单个分配器(指定地址)
#define BuddyDistributorDefineOrient(index, min, max, number, addr)     \
{                                                                       \
    static uint8_t  MemoryManageAddr(addr)                              \
    BuddyDistributor##index[(1 << (max)) * number]  = {0};              \
    static uint8_t *BuddyDistributorList##index[max - min + 1] = {0};   \
    BuddySet[index].BlockOffset = BuddyDistributor##index;              \
    BuddySet[index].BlockList   = BuddyDistributorList##index;          \
    BuddySet[index].BlockSize   = min;                                  \
    BuddySet[index].BlockNumber = number;                               \
    BuddySet[index].BlockFactor = max - min;                            \
}                                                                       \

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef MemoryManageStrategyHeap
/* Heap分配器 */
typedef struct MemoryManageHeap {
    uint8_t *MemoryStart; //管理区起始地址
    uint8_t *MemoryEnd;   //管理区结束地址
    uint8_t *BlockList;   //空闲块链表集合
    uint64_t BlockSize;   //最大块大小
#if MemoryManageCheck == 1
    uint64_t BlockUsed;   //使用字节明细
#endif
} HeapDistributor;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//配置分配器集合
#define HeapDistributorDefineAll(type)              \
    static HeapDistributor HeapSet[type] = {0};     \
    static uint32_t HeapDistributorType  = type;    \
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//配置单个分配器
#define HeapDistributorDefine(index, size)              \
{                                                       \
    static uint8_t MemoryManageAlign                    \
    HeapDistributor##index[size] = {0};                 \
    HeapSet[index].BlockList = HeapDistributor##index;  \
    HeapSet[index].BlockSize = size;                    \
}                                                       \
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//配置单个分配器(指定地址)
#define HeapDistributorDefineOrient(index, size, addr)  \
{                                                       \
    static uint8_t MemoryManageAddr(addr)               \
    HeapDistributor##index[size] = {0};                 \
    HeapSet[index].BlockList = HeapDistributor##index;  \
    HeapSet[index].BlockSize = size;                    \
}                                                       \
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif