#ifndef	SPLAY_TREE_H
#define	SPLAY_TREE_H
//C std lib
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 实现目标:侵入式伸展树 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 从当前伸展树节点地址获得它的所有者地址(编译时解析) */
#define ST_GetOwner(Type, Member, MemberAddress)    \
    ((Type *)((uint8_t *)(MemberAddress) - ((uint64_t) &((Type *)0)->Member)))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
struct SplayTree {
    void *Parameter[3];
};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SplayTree ST_Node;
typedef struct SplayTree ST_Tree;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 为节点排序所用到的回调
 * 用户数据集中需要有一个Key去确认俩个节点的关系
 * ThisNode严格小于ThatNode返回非零值
 */
typedef uint8_t (*ST_KeyCompare)(ST_Node *ThisNode, ST_Node *ThatNode);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 为查找比对所用到的回调
 * 用户数据集中需要有一个Key去确认俩个节点的关系
 * ThisNode与ThatNode的关键字一致返回0
 */
typedef uint8_t (*ST_KeyConfirm)(ST_Node *ThisNode, ST_Node *ThatNode);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void ST_InsertNode(ST_Tree *Tree, ST_Node *Node);
void ST_RemoveNode(ST_Tree *Tree, ST_Node *Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 这里需要生成一个含有Key的假结构,然后使用它去找到真节点 */
ST_Node * ST_SearchNode(ST_Tree *Tree, ST_Node *Node);
ST_Node * ST_SearchPrev(ST_Tree *Tree, ST_Node *Node);
ST_Node * ST_SearchNext(ST_Tree *Tree, ST_Node *Node);
ST_Node * ST_SearchMin(ST_Tree *Tree);
ST_Node * ST_SearchMax(ST_Tree *Tree);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void ST_ResetTree(ST_Tree *Tree);
void ST_ResetNode(ST_Node *Node);
void ST_SetTree(ST_Tree *Tree, ST_KeyCompare Compare, ST_KeyConfirm Confirm);
void ST_SetRoot(ST_Tree *Tree, ST_Node *Node);
void ST_GetRoot(ST_Tree *Tree, ST_Node **Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void ST_JoinTree(ST_Tree *Tree, ST_Tree *Tree1, ST_Tree *Tree2);
void ST_SplitTree(ST_Tree *Tree, ST_Node *Node, ST_Tree *Tree1, ST_Tree *Tree2);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 向后遍历伸展树 */
#define ST_Traverse_Backward(Tree, Node)   \
    for (Node = ST_SearchMin(Tree); Node != NULL; Node = ST_SearchNext(Node))
/* 向前遍历伸展树 */
#define ST_Traverse_Forward(Tree, Node)    \
    for (Node = ST_SearchMax(Tree); Node != NULL; Node = ST_SearchPrev(Node))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 层序遍历(用于打印),需要提供一个足够大的队列(不超过所有节点大小) */
/* 改变传入的Print可以使层序遍历完成别的工作(遍历过程不可以做修改动作,因为队列的状态不一致) */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef void (*ST_Print)(ST_Node *Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void ST_Sequence_Traversal(ST_Tree *Tree, ST_Print Printf, ST_Node **Queue, int32_t Length);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
