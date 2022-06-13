#ifndef	RED_BLACK_TREE_H
#define	RED_BLACK_TREE_H
//C std lib
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 实现目标:侵入式红黑树 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/*
 *容器(红黑树)
 *该树实现由Linux源码改编
 *注:红黑树规则
 *1)节点为红色或黑色
 *2)根结点是黑色的
 *3)所有叶(NULL)是黑色的
 *4)每个红结点的两个子结点都是黑色的
 *5)从根到叶的每条简单路径包含相同数量的黑节点
 *4和5保证了O(log n)
 *因为4意味着一条路径上不可能有两个连续的红结点
 *因此每个红结点后面都有一个黑结点
 *所以如果B是每条简单路径上的黑结点数
 *那么从4出发的最长可能路径就是2B
 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 从当前红黑树节点地址获得它的所有者地址(编译时解析) */
#define RBT_GetOwner(Type, Member, MemberAddress)   \
    ((Type *)((uint8_t *)(MemberAddress) - ((uint64_t) &((Type *)0)->Member)))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
struct RedBlackTree {
    void *Parameter[3];
};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct RedBlackTree RBT_Node;
typedef struct RedBlackTree RBT_Tree;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef enum {RED = 0, BLACK = 1} RBT_Color;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 为节点排序所用到的回调
 * 用户数据集中需要有一个Key去确认俩个节点的关系
 * ThisNode严格小于ThatNode返回非零值
 */
typedef uint8_t (*RBT_KeyCompare)(RBT_Node *ThisNode, RBT_Node *ThatNode);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 为查找比对所用到的回调
 * 用户数据集中需要有一个Key去确认俩个节点的关系
 * ThisNode与ThatNode的关键字一致返回0
 */
typedef uint8_t (*RBT_KeyConfirm)(RBT_Node *ThisNode, RBT_Node *ThatNode);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_InsertNode(RBT_Tree *Tree, RBT_Node *Node);
void RBT_RemoveNode(RBT_Tree *Tree, RBT_Node *Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 这里需要生成一个含有Key的假结构,然后使用它去找到真节点 */
RBT_Node * RBT_SearchNode(RBT_Tree *Tree, RBT_Node *Node);
RBT_Node * RBT_SearchPrev(RBT_Node *Node);
RBT_Node * RBT_SearchNext(RBT_Node *Node);
RBT_Node * RBT_SearchMin(RBT_Tree *Tree);
RBT_Node * RBT_SearchMax(RBT_Tree *Tree);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_ResetTree(RBT_Tree *Tree);
void RBT_ResetNode(RBT_Node *Node);
void RBT_SetTree(RBT_Tree *Tree, RBT_KeyCompare Compare, RBT_KeyConfirm Confirm);
void RBT_SetRoot(RBT_Tree *Tree, RBT_Node *Node);
void RBT_GetRoot(RBT_Tree *Tree, RBT_Node **Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 向后遍历红黑树 */
#define RBT_Traverse_Backward(Tree, Node)   \
    for (Node = RBT_SearchMin(Tree); Node != NULL; Node = RBT_SearchNext(Node))
/* 向前遍历红黑树 */
#define RBT_Traverse_Forward(Tree, Node)    \
    for (Node = RBT_SearchMax(Tree); Node != NULL; Node = RBT_SearchPrev(Node))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 层序遍历,需要提供一个足够大的队列(不超过所有节点大小) */
/* (遍历过程不可以做修改动作,因为队列的状态不一致) */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef void (*RBT_Visit)(RBT_Node *Node, RBT_Color Color);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_Sequence_Traversal(RBT_Tree *Tree, RBT_Visit Visit, RBT_Node **Queue, int32_t Length);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 验证一棵树是否为红黑树(内部检查) */
uint8_t RBT_InternalCheck(RBT_Tree *Tree, RBT_Node **Stack, int32_t *Flags, int32_t Length);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
