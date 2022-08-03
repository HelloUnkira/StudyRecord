#include "FiniteDecisionTree.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void FiniteLoserTree_Adjust(FiniteLoserTree *Tree, int32_t Index)
{
    int32_t Start = (Index + Tree->Length - 1) >> 1;
    for (int32_t Parent = Start; Parent > 0; Parent >>= 1)
        if (Tree->Leave[Index] > Tree->Leave[Tree->Node[Parent]]) {
            int32_t Temp = Index;
            Index = Tree->Node[Parent];
            Tree->Node[Parent] = Temp;
        }
    Tree->Node[0] = Index;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void FiniteWinnerTree_AdjustOnly(FiniteWinnerTree *Tree, int32_t Index)
{
    /* 左孩子:(2 * Index + 0), 右孩子:(2 * Index + 1) */
    uint32_t L = 2 * Index + 0;
    uint32_t R = 2 * Index + 1;
    /* 获取左右孩子的值存储位点 */
    L = (L < Tree->Length) ? (Tree->Node[L]) : (L - Tree->Length + 1);
    R = (R < Tree->Length) ? (Tree->Node[R]) : (R - Tree->Length + 1);
    /* 进行一次比赛 */
    Tree->Node[Index] = Tree->Leave[L] <= Tree->Leave[R] ? L : R;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void FiniteWinnerTree_Adjust(FiniteWinnerTree *Tree, int32_t Index)
{
    int32_t Start = (Index + Tree->Length - 1) >> 1;
    for (int32_t Parent = Start; Parent > 0; Parent >>= 1)
        FiniteWinnerTree_AdjustOnly(Tree, Parent);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
