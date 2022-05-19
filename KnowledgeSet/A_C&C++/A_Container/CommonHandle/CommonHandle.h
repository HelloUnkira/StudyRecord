#ifndef COMMONHANDLE_H
#define COMMONHANDLE_H
//C std lib
#include <stdint.h>
#include <stdbool.h>

/* 实现目标:
 * 设计简要的句柄资源管理器
 * 采用二级数组实现
 * 在保证查找效率为O(1)的条件下
 * 降低额外空间开销
 * 将时间成本转移到获取与释放部分
 * 因为句柄通常不需要频繁获取与释放
 */

/* 配置通用句柄,用于优化内存空间占用 */
#ifdef COMMONHANDLE_C
/* 资源管理单元的伸缩因子 */

/* 暴力测试:1,推荐使用:5 */
#define COMMON_HANDLE_UINT_SCAL_FACTOR     5
/* 暴力测试:1,推荐使用:50,100 */
#define COMMON_HANDLE_UINT_LENGTH          100
/* 句柄计算规则:
 * index1 = handle / SGUIUNITLENGTH;
 * index2 = handle % SGUIUNITLENGTH;
 * handle = index1 * SGUIUNITLENGTH + index2;
 */
/* 资源管理单元必备的分配策略抽象 */
#include <stdlib.h>
#define COMMON_HANDLE_MALLOC(size)  malloc(size)
#define COMMON_HANDLE_FREE(pointer) free(pointer)
/*  */
#endif

/* 非法句柄 */
#define COMMON_HANDLE_INVALID               (~0)

/* 获取一个空闲句柄 */
uint32_t CommonHandleTake(void);
/* 释放一个句柄 */
void CommonHandleGive(uint32_t Handle);
/* 句柄绑定或更新资源 */
void CommonHandleSet(uint32_t Handle, void *Source);
/* 句柄获取资源 */
void * CommonHandleGet(uint32_t Handle);

#endif





