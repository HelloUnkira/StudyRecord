#ifndef FINITE_DECISION_TREE_H
#define FINITE_DECISION_TREE_H
//C std lib
#include <stdint.h>
#include <stdbool.h>
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 当前决策树只提供胜者树与败者树 */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
typedef struct FiniteLoserTreeBody {
    int32_t *Node;     /* 长度:Length, 0号元素为最小值索引 */
    int32_t *Leave;    /* 长度:Length + 1, 0号元素填充假值, 为数据下界 */
    int32_t  Length;
} FiniteLoserTree;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 每修改Tree->Leave[Index]的值都需要执行下面的动作 */
void FiniteLoserTree_Adjust(FiniteLoserTree *Tree, int32_t Index);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 当Tree->Leave[X]被批量更新时执行该动作刷新 */
static void FiniteLoserTree_Reflush(FiniteLoserTree *Tree)
{
    for (int32_t Index = 0; Index < Tree->Length; Index++)
        Tree->Node[Index] = 0;
    for (int32_t Index = Tree->Length; Index > 0; Index--)
        FiniteLoserTree_Adjust(Tree, Index);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
typedef struct FiniteWinnerTreeBody {
    uint32_t *Node;     /* 长度:Length, 1号元素为最大值索引 */
    uint32_t *Leave;    /* 长度:Length + 1 */
    uint32_t  Length;
} FiniteWinnerTree;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 每修改Tree->Leave[Index]的值都需要执行下面的动作 */
void FiniteWinnerTree_Adjust(FiniteWinnerTree *Tree, int32_t Index);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 当Tree->Leave[X]被批量更新时执行该动作刷新 */
static void FiniteWinnerTree_Reflush(FiniteWinnerTree *Tree)
{
    void FiniteWinnerTree_AdjustOnly(FiniteWinnerTree *Tree, int32_t Index);
    for (uint32_t Index = Tree->Length - 1; Index > 0; Index--)
        FiniteWinnerTree_AdjustOnly(Tree, Index);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif