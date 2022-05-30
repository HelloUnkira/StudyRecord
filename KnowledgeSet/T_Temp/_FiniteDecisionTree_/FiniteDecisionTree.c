#include "FiniteDecisionTree.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void FiniteLoserTree_Adjust(FiniteLoserTree *Tree, uint32_t Index)
{
    bool Loop = true;
    
    uint32_t Parent = (Index + Tree->Length) >> 1;
    
    do {
        /* 如果到了根结点,退出循环 */
        if (Parent == 0)
            Loop = false;
        /* 进行一次比赛 */
        if (Tree->Leave[Index] >= Tree->Leave[Tree->Node[Parent]])
            break;
        /* 如果当前结点小于父结点,交换父亲和自己 */
        Tree->Node[Parent] = Index ^ Tree->Node[Parent];
        Index              = Index ^ Tree->Node[Parent];
        Tree->Node[Parent] = Index ^ Tree->Node[Parent];
        /* 迭代到祖先 */
        Parent -=  1;
        Parent >>= 1;
    } while (Loop);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void FiniteWinnerTree_Adjust(FiniteWinnerTree *Tree, uint32_t Index)
{
    bool Loop = true;
    
    uint32_t Parent = (Index + Tree->Length) >> 1;
    
    do {
        /* 如果到了根结点,退出循环 */
        if (Parent == 0)
            Loop = false;
        /* 左孩子:(2 * Parent + 1), 右孩子:(2 * Parent + 2) */
        uint32_t L = 2 * Parent + 1;
        uint32_t R = 2 * Parent + 2;
        /* 获取左右孩子的值存储位点 */
        L = (L < Tree->Length) ? (Tree->Node[L]) : ((L - 1) >> 1);
        R = (R < Tree->Length) ? (Tree->Node[R]) : ((R - 1) >> 1);
        /* 进行一次比赛 */
        Tree->Node[Parent] = Tree->Leave[L] > Tree->Leave[R] ? L : R;
        /* 迭代到祖先 */
        Parent -=  1;
        Parent >>= 1;
    } while (Loop);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
