/* 例: */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "RedBlackTree.h"

//这是我要获得红黑树语义的数据结构
typedef struct DataStructForTest {
    int32_t Key;
    /* 将红黑树侵入到这个容器中 */
    RBT_Node Node;
    /*  */
    int32_t Rand;
} DataTest;

DataTest * GetDataTest(RBT_Node *NodeAddress)
{
    return RBT_GetOwner(DataTest, Node, NodeAddress);
}

uint8_t TestCompare(RBT_Node *ThisNode, RBT_Node *ThatNode)
{
    if (((DataTest *)GetDataTest(ThisNode))->Key <
        ((DataTest *)GetDataTest(ThatNode))->Key)
        return 1;
    else
        return 0;
}

uint8_t TestComfirm(RBT_Node *ThisNode, RBT_Node *ThatNode)
{
    if (((DataTest *)GetDataTest(ThisNode))->Key ==
        ((DataTest *)GetDataTest(ThatNode))->Key)
        return 0;
    else
        return 1;
}

void TestPrint(RBT_Node *Node, RBT_Color Color)
{
    printf("Key:%d val:%d Color:%s\n",
           ((DataTest *)GetDataTest(Node))->Key,
           ((DataTest *)GetDataTest(Node))->Rand,
           Color == BLACK ? "BLACK" :
           Color == RED ?   "RED"   : "ERROR");
}

#define MAX_ELEMENT 1000000
#define HALF_MAX_ELEMENT 500000

int main(int argc, char *argv[]) {
    
    //1.创建一个容器集合(树)
    RBT_Tree *Tree = malloc(sizeof(RBT_Tree));
    //2.初始化一个容器集合(树)
    RBT_SetTree(Tree, TestCompare, TestComfirm);

    RBT_Node **Queue = malloc(sizeof(RBT_Node *)  * MAX_ELEMENT);
    RBT_Node **Stack = malloc(sizeof(RBT_Node *)  * MAX_ELEMENT);
    int32_t   *Flags = malloc(sizeof(int32_t)     * MAX_ELEMENT);
    
    printf("1\n");
    
    //测试:随机生成key
    int32_t Index = 0;
    srand(100);
    //随机插入俩百个 0-4 的数
    for (Index = 0; Index < MAX_ELEMENT; Index++)
    {
        DataTest *Data = malloc(sizeof(DataTest));
        Data->Key  = rand() % 5;
        Data->Rand = rand() % 5;
        RBT_ResetNode(&(Data->Node));
        RBT_InsertNode(Tree, &(Data->Node));//插入到树中
    }
    
    printf("\n----------------------------------------------------------\n");
    RBT_Sequence_Traversal(Tree,TestPrint,Queue,MAX_ELEMENT);//插入检查
    printf("\ncheck:%d", RBT_InternalCheck(Tree, Stack, Flags, MAX_ELEMENT));
    printf("\n----------------------------------------------------------\n");
    Sleep(2000);
    
    //随机删除一百个 0-4 的数 (事实上基于key的红黑树理论上不支持重复key)
    //这里的删除是删除最先遇到的项(即不是首项也不是末项)
    //因为对于红黑树来说,项相同的节点是完全等价的,它们的数据并不是关心的点
    for (Index = 0; Index < HALF_MAX_ELEMENT; Index++)
    {
        DataTest *Data = malloc(sizeof(DataTest));
        Data->Key  = rand() % 5;
        Data->Rand = rand() % 5;
        RBT_ResetNode(&(Data->Node));
        //从树中删除,先查找再删除
        RBT_Node *Node = RBT_SearchNode(Tree, &(Data->Node));
        //从树中删除,先查找再删除
        RBT_RemoveNode(Tree, Node);
    }
    
    printf("\n----------------------------------------------------------\n");
    RBT_Sequence_Traversal(Tree,TestPrint,Queue,MAX_ELEMENT);//删除检查
    printf("\ncheck:%d", RBT_InternalCheck(Tree, Stack, Flags, MAX_ELEMENT));
    printf("\n----------------------------------------------------------\n");
    Sleep(2000);
    
    //更新 == 删除 + 插入
    //因为删除指定的节点顶多只是将其移除出树
    //其数据是不会被做任何修改的,可以只修改关键字或其他,重新插入
    //如果出现一堆重复key,那么它们可以被视作为一个集合
    //反复的移除可以获得指定集合中全部的数据项
    
    RBT_Node *Node = NULL;
    
    //从剩下一百个数中,先找到最小的数
    Node = RBT_SearchMin(Tree);
    //最小项没有前驱
    printf("\n----------------------------------------------------------\n");
    printf("check:%d\n",RBT_SearchPrev(Node) != NULL);
    Sleep(2000);
    //从树的最左叶子节点遍历到最右叶子节点
    while (Node != NULL)
    {
        //打印该节点(color是无效值)
        TestPrint(Node, 9);
        //迭代获得指定节点的后继
        Node = RBT_SearchNext(Node);
    }
    printf("\n----------------------------------------------------------\n");
    
    //从剩下数中,先找到最大的数
    Node = RBT_SearchMax(Tree);
    //最大项没有后继
    printf("\n----------------------------------------------------------\n");
    printf("check:%d\n",RBT_SearchNext(Node) != NULL);
    Sleep(2000);
    //从树的最右叶子节点遍历到最左叶子节点
    while (Node != NULL)
    {
        //打印该节点(color是无效值)
        TestPrint(Node, 9);
        //迭代获得指定节点的前驱
        Node = RBT_SearchPrev(Node);
    }
    printf("\n----------------------------------------------------------\n");
    
	return 0;
}
