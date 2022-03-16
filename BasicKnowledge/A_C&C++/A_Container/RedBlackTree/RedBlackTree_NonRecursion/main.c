/* 例: */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "RedBlackTree.h"

//这是我要镶嵌入容器的数据结构
typedef struct data_struct_for_test {
    int key;
    int rand;
} test_data;

unsigned char t_compare(void *this_data, void *that_data)
{
    if (((test_data *)this_data)->key < ((test_data *)that_data)->key)
        return 1;
    return 0;
}

unsigned char t_comfirm(void *that_data, void *target_data)
{
    if (((test_data *)that_data)->key == ((test_data *)target_data)->key)
        return 0;
    return 1;
}

void t_print(void *node, void *data, int color)
{
    printf("key:%d val:%d color:%d\n",
        ((test_data *)data)->key,
        ((test_data *)data)->rand,
        color);
    
}

#define MAX_ELEMENT 400
#define HALF_MAX_ELEMENT 200

int main(int argc, char *argv[]) {
    
    //获得容器和容器集的空间大小(仅仅测试)
    printf("sizeof: container:%d set:%d UINT:%d ULONG:%d VOID* %d\n", 
           RBT_GetBytes_Container(),RBT_GetBytes_Set(),
           sizeof(unsigned int),sizeof(unsigned long long),sizeof(void *));
    
    //1.创建一个容器集合(树)
    void *tree = malloc(RBT_GetBytes_Set());
    //2.初始化一个容器集合(树)
    RBT_SetTree(tree, t_compare, t_comfirm);

    void *queue = malloc(sizeof(void *) * MAX_ELEMENT);
    void *stack = malloc(sizeof(void *) * MAX_ELEMENT);
    void *flags = malloc(sizeof(int) * MAX_ELEMENT);
    
    printf("1\n");
    
    //测试:随机生成key
    int i = 0;
    srand(100);
    //随机插入俩百个 0-4 的数
    for (i = 0; i < MAX_ELEMENT; i++)
    {
        void *node = malloc(RBT_GetBytes_Container());
        void *data = malloc(sizeof(test_data));
        ((test_data *)data)->key  = rand() % 5;
        ((test_data *)data)->rand = rand() % 5;
        RBT_SetNodeData(node, data);
        RBT_InsertNode(tree, node);//插入到树中
    }
    
    printf("\n----------------------------------------------------------\n");
    RBT_Sequence_Traversal(tree,t_print,queue,MAX_ELEMENT);//插入检查
    printf("\ncheck:%d", RBT_InternalCheck(tree, stack, flags, MAX_ELEMENT));
    printf("\n----------------------------------------------------------\n");
    Sleep(2000);
    
    //随机删除一百个 0-4 的数 (事实上基于key的红黑树理论上不支持重复key)
    //这里的删除是删除最先遇到的项(即不是首项也不是末项)
    //因为对于红黑树来说,项相同的节点是完全等价的,它们的数据并不是关心的点
    for (i = 0; i < HALF_MAX_ELEMENT; i++)
    {
        void *data = malloc(sizeof(test_data));
        ((test_data *)data)->key  = rand() % 5;
        ((test_data *)data)->rand = rand() % 5;
        //从树中删除,先查找再删除
        RBT_RemoveNode(tree, RBT_SearchNode(tree, data));
    }
    
    printf("\n----------------------------------------------------------\n");
    RBT_Sequence_Traversal(tree,t_print,queue,MAX_ELEMENT);//删除检查
    printf("\ncheck:%d", RBT_InternalCheck(tree, stack, flags, MAX_ELEMENT));
    printf("\n----------------------------------------------------------\n");
    Sleep(2000);
    
    //更新 == 删除 + 插入
    //因为删除指定的节点顶多只是将其移除出树
    //其数据是不会被做任何修改的,可以只修改关键字或其他,重新插入
    //如果出现一堆重复key,那么它们可以被视作为一个集合
    //反复的移除可以获得指定集合中全部的数据项
    
    //从剩下一百个数中,先找到最小的数
    void *node = RBT_Search_Min(tree);
    //最小项没有前驱
    printf("\n----------------------------------------------------------\n");
    printf("check:%d\n",RBT_Search_Prev(node) != NULL);
    Sleep(2000);
    //从树的最左叶子节点遍历到最右叶子节点
    while (node != NULL)
    {
        void *temp_data;
        //打印该节点(node和color是无效值)
        RBT_GetNodeData(node, &temp_data);
        t_print(NULL, temp_data, 9);
        //迭代获得指定节点的后继
        node = RBT_Search_Next(node);
    }
    printf("\n----------------------------------------------------------\n");
    
    //从剩下数中,先找到最大的数
    node = RBT_Search_Max(tree);
    //最大项没有后继
    printf("\n----------------------------------------------------------\n");
    printf("check:%d\n",RBT_Search_Next(node) != NULL);
    Sleep(2000);
    //从树的最右叶子节点遍历到最左叶子节点
    while (node != NULL)
    {
        void *temp_data;
        //打印该节点(node和color是无效值)
        RBT_GetNodeData(node, &temp_data);
        t_print(NULL, temp_data, 9);
        //迭代获得指定节点的前驱
        node = RBT_Search_Prev(node);
    }
    printf("\n----------------------------------------------------------\n");
    
	return 0;
}
