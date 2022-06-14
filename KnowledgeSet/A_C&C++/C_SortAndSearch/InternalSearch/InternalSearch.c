//C std lib
#include <stdint.h>
#include <stdbool.h>

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 搜索数据抽象 */
struct InternalSearch_DataSet {
    //元素大小比较回调
    uint8_t (*Compare)(void *Data1, void *Data2);
    void     *List;    //通配元素集合
    uint32_t  Size;    //通配元素字节大小
    uint32_t  Left;    //排序起始点(相对于通配元素集合位置)
    uint32_t  Right;   //排序结束点(相对于通配元素集合位置)
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
/* 结果状态记录 */
enum {LESS = 0, EQUAL = 1, GREATER = 2} IS_CompareState;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 宏定义简化及优化: */
static inline void InternalSearch_Simplification(void)
{
/* 数据错误检查 */
#define INTERNALSEARCH_DATACHECK(DataSet)   \
    if ((DataSet)          == NULL ||       \
        (DataSet)->Compare == NULL ||       \
        (DataSet)->List    == NULL ||       \
        (DataSet)->Size    == 0    ||       \
        (DataSet)->Left    >=               \
        (DataSet)->Right)                   \
        return;
    
/* 数据所需参数定义 */
#define INTERNALSEARCH_DATADEFINE(DataSet)      \
    KeyCompare Compare  = (DataSet)->Compare;   \
    uint8_t *List       = (DataSet)->List;      \
    uint32_t Size       = (DataSet)->Size;      \
    uint32_t Left       = (DataSet)->Left;      \
    uint32_t Right      = (DataSet)->Right;     \


#define RETURN_EMPTY(target) if (target) return;

}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int64_t InternalSearch_Binary(void *Basic, void *Target)
{
    INTERNALSEARCH_DATACHECK((InternalSort_Data *)DataSet);
    INTERNALSEARCH_DATADEFINE((InternalSort_Data *)DataSet);
    
    int64_t BLeft  = Left;
    int64_t BRight = Right;
    
    while (BLeft <= BRight) {
        int64_t Middle = (BLeft + BRight) / 2;
        /* 计算比较结果 */
        uint8_t Result = Compare(Target, List + Size * Middle);
        
        if (Result == EQUAL)
            return Middle;
        
        if (Result == GREATER)
            BLeft  = Middle + 1;
        if (Result == LESS)
            BRight = Middle - 1;
    }
    return -1;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
