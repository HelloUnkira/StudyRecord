/* 例: */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "RedBlackTree.h"

//这是我要镶嵌入容器的数据结构
typedef struct DataStructForTest {
    int32_t Key;
    int32_t Rand;
} DataTest;

uint8_t TestCompare(void *ThisData, void *ThatData)
{
    if (((DataTest *)ThisData)->Key <
        ((DataTest *)ThatData)->Key)
        return 1;
    else
        return 0;
}

uint8_t TestComfirm(void *ThatData, void *TargetData)
{
    if (((DataTest *)ThatData)->Key ==
        ((DataTest *)TargetData)->Key)
        return 0;
    else
        return 1;
}

void TestPrint(void *Node, void *Data, int32_t Color)
{
    printf("Key:%d val:%d Color:%d\n",
            ((DataTest *)Data)->Key,
            ((DataTest *)Data)->Rand, Color);
    
}

#define MAX_ELEMENT 400
#define HALF_MAX_ELEMENT 200

int main(int argc, char *argv[]) {
    
    //获得容器和容器集的空间大小(仅仅测试)
    printf("sizeof: \n");
    printf("container:%d set:%d UINT:%d ULONG:%d VOID* %d\n", 
           RBT_GetBytes_Container(),
           RBT_GetBytes_Set(),
           sizeof(uint32_t),
           sizeof(uint64_t),
           sizeof(void *));
    
    //1.创建一个容器集合(树)
    void *Tree = malloc(RBT_GetBytes_Set());
    //2.初始化一个容器集合(树)
    RBT_SetTree(Tree, TestCompare, TestComfirm);

    void *Queue = malloc(sizeof(void *) * MAX_ELEMENT);
    void *Stack = malloc(sizeof(void *) * MAX_ELEMENT);
    void *Flags = malloc(sizeof(int32_t) * MAX_ELEMENT);
    
    printf("1\n");
    
    //测试:随机生成key
    int32_t Index = 0;
    srand(100);
    //随机插入俩百个 0-4 的数
    for (Index = 0; Index < MAX_ELEMENT; Index++)
    {
        void *Node = malloc(RBT_GetBytes_Container());
        void *Data = malloc(sizeof(DataTest));
        ((DataTest *)Data)->Key  = rand() % 5;
        ((DataTest *)Data)->Rand = rand() % 5;
        RBT_SetNodeData(Node, Data);
        RBT_InsertNode(Tree, Node);//插入到树中
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
        void *Data = malloc(sizeof(DataTest));
        ((DataTest *)Data)->Key  = rand() % 5;
        ((DataTest *)Data)->Rand = rand() % 5;
        //从树中删除,先查找再删除
        RBT_RemoveNode(Tree, RBT_SearchNode(Tree, Data));
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
    
    //从剩下一百个数中,先找到最小的数
    void *Node = RBT_Search_Min(Tree);
    //最小项没有前驱
    printf("\n----------------------------------------------------------\n");
    printf("check:%d\n",RBT_Search_Prev(Node) != NULL);
    Sleep(2000);
    //从树的最左叶子节点遍历到最右叶子节点
    while (Node != NULL)
    {
        void *TempData;
        //打印该节点(node和color是无效值)
        RBT_GetNodeData(Node, &TempData);
        TestPrint(NULL, TempData, 9);
        //迭代获得指定节点的后继
        Node = RBT_Search_Next(Node);
    }
    printf("\n----------------------------------------------------------\n");
    
    //从剩下数中,先找到最大的数
    Node = RBT_Search_Max(Tree);
    //最大项没有后继
    printf("\n----------------------------------------------------------\n");
    printf("check:%d\n",RBT_Search_Next(Node) != NULL);
    Sleep(2000);
    //从树的最右叶子节点遍历到最左叶子节点
    while (Node != NULL)
    {
        void *TempData;
        //打印该节点(node和color是无效值)
        RBT_GetNodeData(Node, &TempData);
        TestPrint(NULL, TempData, 9);
        //迭代获得指定节点的前驱
        Node = RBT_Search_Prev(Node);
    }
    printf("\n----------------------------------------------------------\n");
    
	return 0;
}
