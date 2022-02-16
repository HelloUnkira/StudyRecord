#ifndef SPECIAL_BSTARTREE_H
#define SPECIAL_BSTARTREE_H

#ifdef  SPECIAL_BSTARTREE_C

//配置项
#define STATIC_INLINE static
#define NULL 0

//用于维护非实体外的数据结构
#include "stdlib.h"
#define BSTARTREE_MALLOC(size)  malloc(size)
#define BSTARTREE_FREE(pointer) free(pointer)

//可裁剪项,紧化程序
#include "stdio.h"
#define ERROR_PRINT(target, str) \
    if (target) printf("%s\n",(str));

#endif

///////////////////////////////////////////////////////////////////////////////
//辅助构建:获得集合和容器的大小////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
unsigned int get_bstar_set_bytes(void);
unsigned int get_bstar_container_bytes(void);
///////////////////////////////////////////////////////////////////////////////
//辅助构建:数据集转化关键字////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef unsigned int (*data_key)(void *data_struct);
///////////////////////////////////////////////////////////////////////////////
//辅助构建:设置集合的参数//////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void set_bstar_tree(void *tree, unsigned int length, data_key key);
///////////////////////////////////////////////////////////////////////////////
//核心动作:插入,删除///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void insert_node(void *tree, void *data);
void remove_node(void *tree, void **data, unsigned int key_value);
void search_node(void *tree, void **data, unsigned int key_value);
///////////////////////////////////////////////////////////////////////////////
//辅助动作:打印一个数据集合////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef void (*k_print)(unsigned int current_number,
                        unsigned int *key_set,
                        void **data_set);
///////////////////////////////////////////////////////////////////////////////
//辅助动作:层序遍历,并对所有节点操作///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
unsigned int sequence_traversal_check(void *tree, k_print print, char destory);

#endif

///////////////////////////////////////////////////////////////////////////////
/*

#include <stdio.h>
#include <windows.h>
#include "b_star_tree.h"

typedef struct data_struct_for_test {
	int key;
} test_data;

void t_print(unsigned int current_number,
             unsigned int *key_set,
             void **data_set)
{
	unsigned int i = 0;
	for (i = 0; i < current_number; i++)
		printf("%d ", key_set[i]);
	printf("\n");
}

void t_delete(unsigned int current_number,
              unsigned int *key_set,
              void **data_set)
{
	unsigned int i = 0;
	for (i = 0; i < current_number; i++) {
		printf("%d ", key_set[i]);
        if (data_set != NULL)
            free(data_set[i]);
    }
	printf("\n");
}

unsigned int d_key(void *data)
{
	return (unsigned int)(((test_data *)data)->key);
}

#define MAX_ELEMENT 500
#define HALF_MAX_ELEMENT 200

int main(int argc, char *argv[]) {

	//1.创建一个容器集合(树)
	void *tree = malloc(get_bstar_set_bytes());
	//2.初始化一个容器集合(树)
	set_bstar_tree(tree, 10, d_key);

	void *queue = malloc(sizeof(void *) * MAX_ELEMENT);
	printf("-------------------------------------------------\n");

	//测试:随机生成key
	int i = 0;
	srand(150);
	for (i = 0; i < MAX_ELEMENT; i++) {
		void *data = malloc(sizeof(test_data));
		((test_data *)data)->key = rand() % 5;
		//插入到树中
		//printf("%d ", ((test_data *)data)->key);
		insert_node(tree, data);
	}

	printf("-------------------------------------------------\n");
	printf("%u", sequence_traversal_check(tree, t_print, 0));
	printf("\n----------------------------------------------------\n");
 
	Sleep(2000);

    //测试:随机生成key
    i = 0;
    srand(100);
    for (i = 0; i < HALF_MAX_ELEMENT; i++) {
        void *data;
        //移除出树中
        remove_node(tree, &data, rand() % 5);
        if (data != NULL);
			//printf("%d ",((test_data *)data)->key);

		if (0) {
        // if (1) {
        // if (i > 0 && i < 150) {
		    printf("-------------------------------------------------\n");
        	printf("%u", sequence_traversal_check(tree, t_print, 0));
        	printf("\n----------------------------------------------------\n");
		}
    }

    printf("-------------------------------------------------\n");
    printf("%u", sequence_traversal_check(tree, t_delete, 1));
    printf("\n----------------------------------------------------\n");
    
	return 0;
}

*/
///////////////////////////////////////////////////////////////////////////////
