#ifndef	SPECIAL_REDBLACKTREE_H
#define	SPECIAL_REDBLACKTREE_H

///////////////////////////////////////////////////////////////////////////////
//容器(红黑树)/////////////////////////////////////////////////////////////////
//该树实现由Linux源码改编//////////////////////////////////////////////////////
//注:红黑树规则////////////////////////////////////////////////////////////////
//1)节点为红色或黑色///////////////////////////////////////////////////////////
//2)根结点是黑色的/////////////////////////////////////////////////////////////
//3)所有叶(NULL)是黑色的///////////////////////////////////////////////////////
//4)每个红结点的两个子结点都是黑色的///////////////////////////////////////////
//5)从根到叶的每条简单路径包含相同数量的黑节点/////////////////////////////////
//4和5保证了O(log n)///////////////////////////////////////////////////////////
//因为4意味着一条路径上不可能有两个连续的红结点////////////////////////////////
//因此每个红结点后面都有一个黑结点/////////////////////////////////////////////
//所以如果B是每条简单路径上的黑结点数//////////////////////////////////////////
//那么从4出发的最长可能路径就是2B//////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#ifdef SPECIAL_REDBLACKTREE_C
///////////////////////////////////////////////////////////////////////////////
//选取一个与void*长度一致的整形类型,它将用作空间的节约/////////////////////////
//如果不是严格等价定义,造成截断现象,则会导致其ERROR_PRINT产生side错误//////////
//注:程序可能意外的错误,此时应该释放下述定义取消优化///////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define VOID_STAR_CHANGE unsigned long long
//#define REDBLACKTREE_ACCIDENTAL_ERROR

//红黑树涉及到的引用

#include "stdio.h"
#define ERROR_PRINT(target, str) \
    if (target) printf("%s\n",(str));

#define ERROR_RETURN(target) \
    do {                     \
        if (target)          \
            return;          \
    } while (0)
        

#define ERROR_RETURN_0(target) \
    do {                       \
        if (target)            \
            return 0;          \
    } while (0)


#define ERROR_RETURN_NULL(target) \
    do {                          \
        if (target)               \
            return NULL;          \
    } while (0)

#endif



///////////////////////////////////////////////////////////////////////////////
//红黑树所需的空间大小(1byte规定为8位)/////////////////////////////////////////
//节点所需的空间大小(1byte规定为8位)///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
unsigned int get_rb_container_bytes(void);
unsigned int get_rb_set_bytes(void);

///////////////////////////////////////////////////////////////////////////////
//为节点排序所用到的回调///////////////////////////////////////////////////////
//用户数据集中需要有一个key去确认俩个节点的关系////////////////////////////////
//this_data严格小于that_data返回非零值/////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef unsigned char (*compare)(void *this_data, void *that_data);

///////////////////////////////////////////////////////////////////////////////
//为查找比对所用到的回调///////////////////////////////////////////////////////
//用户数据集中需要有一个key去确认俩个节点的关系////////////////////////////////
//this_data与target_data的关键字一致返回0//////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef unsigned char (*comfirm)(void *that_data, void *target_data);

///////////////////////////////////////////////////////////////////////////////
//红黑树和节点的初始化,空间由上层提供,其对应结构大小从上可得///////////////////
//它们是以平台位数为依赖项的,所以同一平台该值是固定的,可以先静态获取///////////
///////////////////////////////////////////////////////////////////////////////
void set_rb_tree(void *tree_mem, compare k_compare, comfirm k_confirm);
void set_rb_root(void *tree, void *node);
void get_rb_root(void *tree, void **node);
void set_rb_node(void *node, void *data);
void get_rb_data(void *node, void **data);


///////////////////////////////////////////////////////////////////////////////
//核心动作/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void   insert_node(void *tree, void *node);
void   remove_node(void *tree, void *node);
void * search_node(void *tree, void *data);

void * search_node_prev(void *node);
void * search_node_next(void *node);
void * search_min_node(void *tree);
void * search_max_node(void *tree);

///////////////////////////////////////////////////////////////////////////////
//查找(成功返回用户数据集索引,并可直接修改内部数据)////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void * search_data(void *tree, void *target_data);

///////////////////////////////////////////////////////////////////////////////
//层序遍历(用于打印),需要提供一个足够大的队列(不超过所有节点大小)//////////////
//改变传入的print可以使层序遍历完成别的工作////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef void (*print)(void *node, void *data, int color);
void sequence_traversal(void *tree, print k_print, void *queue, int length);

///////////////////////////////////////////////////////////////////////////////
//验证一棵树是否为红黑树(内部检查)/////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
unsigned char check_rb_tree(void *tree, void *stack, int *flags, int length);

#endif

///////////////////////////////////////////////////////////////////////////////
/*例:

#include <stdlib.h>
#include <windows.h>
#include "red_black_tree.h"

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
           get_rb_container_bytes(),get_rb_set_bytes(),
           sizeof(unsigned int),sizeof(unsigned long long),sizeof(void *));
    
    //1.创建一个容器集合(树)
    void *tree = malloc(get_rb_set_bytes());
    //2.初始化一个容器集合(树)
    set_rb_tree(tree, t_compare, t_comfirm);

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
        void *node = malloc(get_rb_container_bytes());
        void *data = malloc(sizeof(test_data));
        ((test_data *)data)->key  = rand() % 5;
        ((test_data *)data)->rand = rand() % 5;
        set_rb_node(node, data);
        insert_node(tree, node);//插入到树中
    }
    
    printf("\n----------------------------------------------------------\n");
    sequence_traversal(tree,t_print,queue,MAX_ELEMENT);//插入检查
    printf("\ncheck:%d", check_rb_tree(tree, stack, flags, MAX_ELEMENT));
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
        remove_node(tree, search_node(tree, data));
    }
    
    printf("\n----------------------------------------------------------\n");
    sequence_traversal(tree,t_print,queue,MAX_ELEMENT);//删除检查
    printf("\ncheck:%d", check_rb_tree(tree, stack, flags, MAX_ELEMENT));
    printf("\n----------------------------------------------------------\n");
    Sleep(2000);
    
    //更新 == 删除 + 插入
    //因为删除指定的节点顶多只是将其移除出树
    //其数据是不会被做任何修改的,可以只修改关键字或其他,重新插入
    //如果出现一堆重复key,那么它们可以被视作为一个集合
    //反复的移除可以获得指定集合中全部的数据项
    
    //从剩下一百个数中,先找到最小的数
    void *node = search_min_node(tree);
    //最小项没有前驱
    printf("\n----------------------------------------------------------\n");
    printf("check:%d\n",search_node_prev(node) != NULL);
    Sleep(2000);
    //从树的最左叶子节点遍历到最右叶子节点
    while (node != NULL)
    {
        void *temp_data;
        //打印该节点(node和color是无效值)
        get_rb_data(node, &temp_data);
        t_print(NULL, temp_data, 9);
        //迭代获得指定节点的后继
        node = search_node_next(node);
    }
    printf("\n----------------------------------------------------------\n");
    
    //从剩下数中,先找到最大的数
    node = search_max_node(tree);
    //最大项没有后继
    printf("\n----------------------------------------------------------\n");
    printf("check:%d\n",search_node_next(node) != NULL);
    Sleep(2000);
    //从树的最右叶子节点遍历到最左叶子节点
    while (node != NULL)
    {
        void *temp_data;
        //打印该节点(node和color是无效值)
        get_rb_data(node, &temp_data);
        t_print(NULL, temp_data, 9);
        //迭代获得指定节点的前驱
        node = search_node_prev(node);
    }
    printf("\n----------------------------------------------------------\n");
    
	return 0;
}
*/
///////////////////////////////////////////////////////////////////////////////




























