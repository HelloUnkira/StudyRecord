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

#define MAX_ELEMENT  30

int32_t main(int32_t argc, uint8_t *argv[]) {
    
    //获得容器和容器集的空间大小(仅仅测试)
    printf("sizeof: \n");
    printf("container:%d set:%d UINT:%d ULONG:%d VOID* %d Stack %d\n", 
           RBT_GetBytes_Container(),
           RBT_GetBytes_Set(),
           sizeof(uint32_t),
           sizeof(uint64_t),
           sizeof(void *),
           RBT_MAX_DEPTH);
    
    //1.创建一个容器集合(树)
    void *Tree = malloc(RBT_GetBytes_Set());
    //2.初始化一个容器集合(树)
    RBT_SetTree(Tree, TestCompare, TestComfirm);
    //3.创建一个堆栈空间
    void *Stack = malloc(sizeof(void *) * RBT_MAX_DEPTH);
    //4.为其注入到树中(它可以插入删除动作进行之前动态分配并在完成后释放)
    RBT_SetTreeStack(Tree, Stack, RBT_MAX_DEPTH);
    
    
    //测试:随机生成key
    int32_t i = 0;
    srand(100);
    for (i = 0; i < MAX_ELEMENT; i++)
    {
        void *Node = malloc(RBT_GetBytes_Container());
        void *Data = malloc(sizeof(DataTest));
        ((DataTest *)Data)->Key  = rand() % 100;
        ((DataTest *)Data)->Rand = rand() % 100;
        RBT_SetNodeData(Node, Data);
        //插入到树中
        RBT_InsertNode(Tree, Node);
    }
    
    void *Queue = malloc(RBT_GetBytes_Container() * MAX_ELEMENT);
    
    RBT_Sequence_Traversal(Tree,TestPrint,Queue,MAX_ELEMENT);//插入检查
    printf("\n");
    
    //查找测试
    void *Data = malloc(sizeof(DataTest));
    ((DataTest *)Data)->Key  = 10;
    RBT_RemoveNode(Tree, Data);
    
    ((DataTest *)Data)->Key  = 5;
    RBT_RemoveNode(Tree, Data);
  
    ((DataTest *)Data)->Key  = 4;
    RBT_RemoveNode(Tree, Data);
    
    ((DataTest *)Data)->Key  = 9;
    RBT_RemoveNode(Tree, Data);

    ((DataTest *)Data)->Key  = 15;
    RBT_RemoveNode(Tree, Data);
 
    ((DataTest *)Data)->Key  = 28;
    RBT_RemoveNode(Tree, Data);
    
    ((DataTest *)Data)->Key  = 16;
    RBT_RemoveNode(Tree, Data);
    
    ((DataTest *)Data)->Key  = 29;
    RBT_RemoveNode(Tree, Data);
    
    ((DataTest *)Data)->Key  = 29;
    RBT_RemoveNode(Tree, Data);

    RBT_Sequence_Traversal(Tree,TestPrint,Queue,MAX_ELEMENT);//删除检查
    printf("\n");  
    
	return 0;
}
