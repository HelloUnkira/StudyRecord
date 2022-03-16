#ifndef	REDBLACKTREE_H
#define	REDBLACKTREE_H
//C std lib
#include <stdint.h>
#include <stdbool.h>

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

#ifdef REDBLACKTREE_C
/*
 *寻找一个实现: sizeof(uintxxx_t) <==> sizeof(void *)
 *选取一个与void*字节长度一致的整形类型,它将用作空间的节约
 *如果不是严格等价定义,造成截断现象,ERROR_PRINT产生side错误,如果它被启用
 */
#define VOID_STAR_TYPE uint64_t

//红黑树内部使用,用于内部调试
#include <stdio.h>
#define ERROR_PRINT(target, str) \
    if (target) printf("%s\n",(str));

#endif



///////////////////////////////////////////////////////////////////////////////
//红黑树所需的空间大小(1byte规定为8位)/////////////////////////////////////////
//节点所需的空间大小(1byte规定为8位)///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
unsigned int RBT_GetBytes_Container(void);
unsigned int RBT_GetBytes_Set(void);

///////////////////////////////////////////////////////////////////////////////
//为节点排序所用到的回调///////////////////////////////////////////////////////
//用户数据集中需要有一个key去确认俩个节点的关系////////////////////////////////
//this_data严格小于that_data返回非零值/////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef uint8_t (*Compare)(void *ThisData, void *ThatData);

///////////////////////////////////////////////////////////////////////////////
//为查找比对所用到的回调///////////////////////////////////////////////////////
//用户数据集中需要有一个key去确认俩个节点的关系////////////////////////////////
//this_data与target_data的关键字一致返回0//////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef uint8_t (*Comfirm)(void *ThatData, void *TargetData);

///////////////////////////////////////////////////////////////////////////////
//红黑树和节点的初始化,空间由上层提供,其对应结构大小从上可得///////////////////
//它们是以平台位数为依赖项的,所以同一平台该值是固定的,可以先静态获取///////////
///////////////////////////////////////////////////////////////////////////////
void RBT_SetTree(void *Tree, Compare k_compare, Comfirm k_confirm);
void RBT_SetRoot(void *Tree, void *Node);
void RBT_GetRoot(void *Tree, void **Node);
void RBT_SetNodeData(void *Node, void *data);
void RBT_GetNodeData(void *Node, void **data);

///////////////////////////////////////////////////////////////////////////////
//核心动作/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void RBT_InsertNode(void *Tree, void *Node);
void RBT_RemoveNode(void *Tree, void *Node);

//通过含有Key的假结构去找到真节点//////////////////////////////////////////////
void * RBT_SearchNode(void *Tree, void *Data);
void * RBT_Search_Prev(void *Node);
void * RBT_Search_Next(void *Node);
void * RBT_Search_Min(void *Tree);
void * RBT_Search_Max(void *Tree);

///////////////////////////////////////////////////////////////////////////////
//查找(成功返回用户数据集索引,并可直接修改内部数据)////////////////////////////
//未完成,可以使用上述的接口做封装完成该事务,不额外添加该接口实现///////////////
///////////////////////////////////////////////////////////////////////////////
void * RBT_Search_Data(void *Tree, void *Data);

///////////////////////////////////////////////////////////////////////////////
//层序遍历(用于打印),需要提供一个足够大的队列(不超过所有节点大小)//////////////
//改变传入的print可以使层序遍历完成别的工作////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef void (*print)(void *node, void *data, int color);
void RBT_Sequence_Traversal(void *Tree, print k_print, void *queue, int length);

///////////////////////////////////////////////////////////////////////////////
//验证一棵树是否为红黑树(内部检查)/////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
uint8_t RBT_InternalCheck(void *Tree, void *stack, int *flags, int length);

#endif
