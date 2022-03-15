#ifndef MEMORYMANAGEUSERCONFIGURE_H
#define MEMORYMANAGEUSERCONFIGURE_H
//C std lib
#include <stdint.h>
#include <stdbool.h>

/*实现目标:
 *这是管理单元的用户配置部,用于将静态配置
 *与实现分离开来,这会导致部分内容向上层暴露
 *但相对而言,这部分内容并不具有实现的私密性
 */

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef MemoryManageStrategySlab
/* 配置slab分配器集合数 */
SlabDistributorDefineAll(3);
/* 函数内配置各个分配器明细 */
static inline void MemoryManageSlabConfigure(void)
{
    /*备注:
     *编号     : 从 0 到 集合数-1 的唯一系列
     *块大小   : >= sizeof(void *), 建议块大小不可以重复!
     *块数量   : >= 1
     *定向地址 : 如需使用,对应谓词结构修改Port对应部分
     */
    /* {编号, 块大小, 块数量} */
    SlabDistributorDefine(0, 8, 5);
    SlabDistributorDefine(1, 8, 5);
    SlabDistributorDefine(2, 8, 5);
    // SlabDistributorDefine(0, 32,  10);
    // SlabDistributorDefine(1, 64,  10);
    // SlabDistributorDefine(2, 128, 10);
    // SlabDistributorDefine(3, 256, 10);
    // SlabDistributorDefine(4, 512, 10);
    /* {编号, 块大小, 块数量, 定向地址} */
    //SlabDistributorDefineOrient(0, 32, 10, 0x4000f000);
}
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef MemoryManageStrategyBuddy
/* 配置buddy分配器集合数,建议使用1个分配器 */
BuddyDistributorDefineAll(1);
/* 函数内配置各个分配器明细 */
static inline void MemoryManageBuddyConfigure(void)
{
    /*备注:
     *编号      : 从 0 到 集合数-1 的唯一系列
     *最小块因子: 最小块大小 == (1 << min), 要求 >= sizeof(void *)
     *最大块因子: 最大块大小 == (1 << max), 要求 >= sizeof(void *)
     *块数量    : 最大块数量, 要求 >= 1
     *定向地址  : 如需使用,对应谓词结构修改Port对应部分
     */
    /* {编号, 最小块大小偏移, 倍频因子, 块数量} */
    BuddyDistributorDefine(0, 3, 5, 5);
    /* {编号, 块大小, 倍频因子, 块数量, 定向地址} */
    //BuddyDistributorDefineOrient(0, 3, 5, 5, 0x4000f000);
}
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef MemoryManageStrategyHeap
/* 配置buddy分配器集合数,建议使用1个分配器 */
HeapDistributorDefineAll(1);
/* 函数内配置各个分配器明细 */
static inline void MemoryManageHeapConfigure(void)
{
    /*备注:
     *编号      : 从 0 到 集合数-1 的唯一系列
     *最小块因子: 最小块大小 == (1 << min), 要求 >= sizeof(void *)
     *最大块因子: 最大块大小 == (1 << max), 要求 >= sizeof(void *)
     *块数量    : 最大块数量, 要求 >= 1
     *定向地址  : 如需使用,对应谓词结构修改Port对应部分
     */
    /* {编号, 最大块大小} */
    HeapDistributorDefine(0, 1024 * 256);
    /* {编号, 最大块大小, 定向地址} */
    //HeapDistributorDefineOrient(0, 1024 * 256, 0x4000f000);
}
#endif

#endif
