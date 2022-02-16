#ifndef	SPECIAL_REDBLACKTREE_H
#define	SPECIAL_REDBLACKTREE_H

//深度为K的二叉树,最多有2^k - 1个节点

#ifdef SPECIAL_REDBLACKTREE_C
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
//为动态配置红黑树堆栈最大空间的理论参考值/////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#define PBITS(t) (8 * sizeof(t))
#define TBITS(t) ((sizeof(t)) < 8 ? 2 : 3)
#define MAX_RBTREE_DEPTH (2 * (PBITS(int *) - TBITS(int *) - 1) + 1)



///////////////////////////////////////////////////////////////////////////////
//选取一个与void*长度一致的整形类型,它将用作空间的节约/////////////////////////
//如果不是严格等价定义,造成截断现象,则会导致其ERROR_PRINT产生side错误//////////
///////////////////////////////////////////////////////////////////////////////
//#define REPLACE_VOID_STAR unsigned long long

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
void initialize_tree(void *tree_mem, compare k_compare, comfirm k_confirm);
void initialize_node(void *node, void *data);

///////////////////////////////////////////////////////////////////////////////
//追踪维护红黑树使用到的堆栈,它是get_rb_container_bytes()类型的数组////////////
///////////////////////////////////////////////////////////////////////////////
void config_stack(void *tree, void *stack, unsigned int length);

///////////////////////////////////////////////////////////////////////////////
//核心动作:插入,删除///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void insert_node(void *tree, void *node);
void remove_node(void *tree, void *data);

///////////////////////////////////////////////////////////////////////////////
//查找(成功返回用户数据集索引,并可直接修改内部数据)////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void * search_data(void *tree, void *target_data);

///////////////////////////////////////////////////////////////////////////////
//获取树深/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
unsigned int get_tree_max_depth(void *tree);

///////////////////////////////////////////////////////////////////////////////
//层序遍历(用于打印),需要提供一个足够大的队列(不超过所有节点大小)//////////////
//改变传入的print可以使层序遍历完成别的工作////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef void (*print)(void *data, int color);
void internal_check_for_level(void *tree, print k_print, 
                              void *queue, int max_length);

#endif

///////////////////////////////////////////////////////////////////////////////
/*例:
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

void t_print(void *data, int color)
{
    printf("key:%d val:%d color:%d\n",
        ((test_data *)data)->key,
        ((test_data *)data)->rand,
        color);
    
}

#define MAX_ELEMENT  30

int main(int argc, char *argv[]) {
    
    //获得容器和容器集的空间大小(仅仅测试)
    printf("sizeof: container:%d set:%d UINT:%d ULONG:%d VOID* %d STACK %d\n", 
           get_rb_container_bytes(),get_rb_set_bytes(),
           sizeof(unsigned int),sizeof(unsigned long long),sizeof(void *),MAX_RBTREE_DEPTH);
    
    //1.创建一个容器集合(树)
    void *tree = malloc(get_rb_set_bytes());
    //2.初始化一个容器集合(树)
    initialize_tree(tree, t_compare, t_comfirm);
    //3.创建一个堆栈空间
    void *stack = malloc(sizeof(void *) * MAX_RBTREE_DEPTH);
    //4.为其注入到树中(它可以插入删除动作进行之前动态分配并在完成后释放)
    config_stack(tree, stack, MAX_RBTREE_DEPTH);
    
    
    //测试:随机生成key
    int i = 0;
    srand(100);
    for (i = 0; i < MAX_ELEMENT; i++)
    {
        void *node = malloc(get_rb_container_bytes());
        void *data = malloc(sizeof(test_data));
        ((test_data *)data)->key  = rand() % 100;
        ((test_data *)data)->rand = rand() % 100;
        initialize_node(node, data);
        //插入到树中
        insert_node(tree, node);
    }
    
    void *queue = malloc(get_rb_container_bytes() * MAX_ELEMENT);
    
    internal_check_for_level(tree,t_print,queue,MAX_ELEMENT);//插入检查
    printf("\n");
    
    //查找测试
    void *data = malloc(sizeof(test_data));
    ((test_data *)data)->key  = 10;
    remove_node(tree, data);
    
    ((test_data *)data)->key  = 5;
    remove_node(tree, data);
  
    ((test_data *)data)->key  = 4;
    remove_node(tree, data);
    
    ((test_data *)data)->key  = 9;
    remove_node(tree, data);

    ((test_data *)data)->key  = 15;
    remove_node(tree, data);
 
    ((test_data *)data)->key  = 28;
    remove_node(tree, data);
    
    ((test_data *)data)->key  = 16;
    remove_node(tree, data);
    
    ((test_data *)data)->key  = 29;
    remove_node(tree, data);
    
    ((test_data *)data)->key  = 29;
    remove_node(tree, data);

    internal_check_for_level(tree,t_print,queue,MAX_ELEMENT);//删除检查
    printf("\n");  
    
	return 0;
}


*/
///////////////////////////////////////////////////////////////////////////////




























