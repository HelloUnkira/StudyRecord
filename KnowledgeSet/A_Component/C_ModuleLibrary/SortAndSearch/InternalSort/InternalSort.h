#ifndef INTERNALSORT_H
#define INTERNALSORT_H
//C std lib
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 排序数据抽象 */
struct InternalSort_DataSet {
    void     *Parameter1[2];
    uint32_t  Parameter2[3];
};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 核心转义 */
typedef struct InternalSort_DataSet InternalSort_Data;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//temp1严格小于temp2返回0
//temp1严格等于temp2返回1
//temp1严格大于temp2返回2
typedef uint8_t (*KeyCompare)(void *Temp1, void *Temp2);
//希尔增量序列,已知序列后一项求序列前一项,直到1
typedef uint32_t (*HillSequence)(uint32_t x);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//使用对应参数初始化basic
void InternalSort_ContainerBytesSet(void *DataSet, KeyCompare Compare,
                                    void *List, uint32_t Size,
                                    uint32_t Left, uint32_t Right);

//冒泡排序:时间复杂度:O(n**2)
void InternalSort_Bubble(void *Basic);
//选择排序:时间复杂度:O(n**2),空间复杂度:O(1) 1单位
void InternalSort_Selection(void *Basic, void *Helper);
//插入排序:时间复杂度:O(n**2),空间复杂度:O(1) 1单位
void InternalSort_Insertion(void *Basic, void *Helper);
//希尔排序:时间复杂度:O(n log n),空间复杂度:O(1) 1单位
void InternalSort_Shell(void *Basic, void *Helper, HillSequence Incremental);
//堆排序:时间复杂度:O(n log n),空间复杂度:O(1)
void InternalSort_Heap(void *Basic);
//归并排序:时间复杂度:O(n log n),空间复杂度:O(n)
void InternalSort_Merge(void *Basic, void *Helper);
//快速排序:时间复杂度:O(n log n),空间复杂度:O(n log n)
void InternalSort_Quick(void *Basic, void *Helper);

#endif