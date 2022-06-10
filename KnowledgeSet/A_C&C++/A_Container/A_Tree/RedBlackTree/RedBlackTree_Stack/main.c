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

#define MAX_ELEMENT         1000000
#define HALF_MAX_ELEMENT    500000

int32_t main(int32_t argc, uint8_t *argv[]) {
    
    //获得栈大小
    printf("RBT_MAX_DEPTH: %d\n", RBT_MAX_DEPTH);
    
    //1.创建一个容器集合(树)
    RBT_Tree *Tree = malloc(sizeof(RBT_Tree));
    //2.初始化一个容器集合(树)
    RBT_SetTree(Tree, TestCompare, TestComfirm);
    //3.创建一个堆栈空间
    void *TraceStack = malloc(sizeof(RBT_Node *) * RBT_MAX_DEPTH);
    //4.为其注入到树中(它可以插入删除动作进行之前动态分配并在完成后释放)
    RBT_SetTreeStack(Tree, TraceStack, RBT_MAX_DEPTH);

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


	return 0;
}
