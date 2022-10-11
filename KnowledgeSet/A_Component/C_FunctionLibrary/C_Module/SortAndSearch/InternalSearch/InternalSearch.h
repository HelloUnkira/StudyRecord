#ifndef INTERNALSORT_H
#define INTERNALSORT_H
//C std lib
#include <stdint.h>
#include <stdbool.h>
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 搜索数据抽象 */
struct InternalSearch_DataSet {
    void     *Parameter1[2];
    uint32_t  Parameter2[3];
};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 核心转义 */
typedef struct InternalSearch_DataSet InternalSearch_Data;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* Data1严格小于Data2返回0 */
/* Data1严格等于Data2返回1 */
/* Data1严格大于Data2返回2 */
typedef uint8_t (*KeyCompare)(void *Data1, void *Data2);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int64_t InternalSearch_Binary(void *Basic, void *Target);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif