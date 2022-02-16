#ifndef SIMPLE_SORT_H
#define SIMPLE_SORT_H

//temp1严格小于temp2返回0
//temp1严格等于temp2返回1
//temp1严格大于temp2返回2
typedef unsigned char (*key_compare)(void *temp1, void *temp2);
//希尔增量序列,已知序列后一项求序列前一项,直到1
typedef unsigned int (*hill_sequence)(unsigned int x);

//冒泡排序:时间复杂度:O(n**2)
void bubble_sort(void *basic);
//选择排序:时间复杂度:O(n**2),空间复杂度:O(1) 1单位
void selection_sort(void *basic, void *helper);
//插入排序:时间复杂度:O(n**2),空间复杂度:O(1) 1单位
void insertion_sort(void *basic, void *helper);
//希尔排序:时间复杂度:O(n log n),空间复杂度:O(1) 1单位
void shell_sort(void *basic, void *helper, hill_sequence incremental);
//堆排序:时间复杂度:O(n log n),空间复杂度:O(1)
void heap_sort(void *basic);
//归并排序:时间复杂度:O(n log n),空间复杂度:O(n)
void merge_sort(void *basic, void *helper);
//快速排序:时间复杂度:O(n log n),空间复杂度:O(n log n)
void quick_sort(void *basic, void *helper);

//注意:以下功能
//获取basic的空间大小(用于动态分配)
unsigned char get_sort_container_byte(void);

//使用对应参数初始化basic
void set_sort_container(void        *basic,
                        key_compare  compare,
                        void        *list,
                        unsigned int size,
                        unsigned int left,
                        unsigned int right);

#endif

//测试用例,头文件包含前定义该宏
#ifdef SIMPLE_SORT_H_TEST
#define SIMPLE_SORT_NUMBER 30000
#define SIMPLE_SORT_DOUBLE SIMPLE_SORT_NUMBER * 2
#define SIMPLE_SORT_SRAND  978563412
#include <stdio.h>

//如果结构体被替换成普通元素,退化的过程
typedef struct test_data_struct {
    unsigned int   xxx; //关键字
    unsigned char  yyy;
    unsigned short ll;
    float          pp;
    unsigned int   tax;
    double         tt;
    void          *other;
} test_struct_t;

static unsigned char t_compare(void *temp1, void *temp2)
{
    if (temp1 == NULL || temp2 == NULL)
        printf("\nspecial error\n");
    
    if (((test_struct_t *)temp1)->xxx < ((test_struct_t *)temp2)->xxx)
        return 0;
    if (((test_struct_t *)temp1)->xxx == ((test_struct_t *)temp2)->xxx)
        return 1;
    if (((test_struct_t *)temp1)->xxx > ((test_struct_t *)temp2)->xxx)
        return 2;
    printf("\nspecial error\n");
    return (~0);
}

static unsigned int Hibbard(unsigned int x)
{//希尔排序用到的增量序列
    unsigned int temp = (x + 1) / 2;
    return temp <= x ? temp : 1;
}

static void printf_all(test_struct_t *test, unsigned int true_or_false)
{
    if (true_or_false == 0)
        return;

    unsigned int i = 0;
    while (i < SIMPLE_SORT_NUMBER)
        printf("%3d ", test[i++].xxx);
    printf("\n");
}

static unsigned char simple_sort_check(test_struct_t *test)
{
    unsigned int i = 0;
    for (i = 1; i < SIMPLE_SORT_NUMBER; i++)
        if (test[i - 1].xxx > test[i].xxx)
            return 0;
    return (~0);
}

static void simple_sort_test(void)
{
	printf("%d\t%d\t%d\t%d\t\n",
           sizeof(unsigned short),
           sizeof(unsigned int),
           sizeof(unsigned long long),
           sizeof(test_struct_t));
    
    unsigned int i = 0;
    unsigned int flag = 0;
    unsigned int true_or_false = 0;
    test_struct_t *temp1 = NULL;
    test_struct_t *temp2 = NULL;
    test_struct_t  helper1 = {0};
    test_struct_t *helper2 = NULL;
    unsigned int  *helper3 = NULL;
    temp1   = (test_struct_t *)calloc(SIMPLE_SORT_NUMBER, sizeof(test_struct_t));
    temp2   = (test_struct_t *)calloc(SIMPLE_SORT_NUMBER, sizeof(test_struct_t));
    helper2 = (test_struct_t *)calloc(SIMPLE_SORT_NUMBER, sizeof(test_struct_t));
    //数据量越大log n 越小于 n,否则很接近,所以为兼容各个情况
    helper3 = (unsigned int *)calloc(SIMPLE_SORT_DOUBLE, sizeof(unsigned int));
    //-------------------------------------------------------------------------
    void *test_data = malloc(get_sort_container_byte());
    set_sort_container(test_data,
                       t_compare,
                       (void *)temp2,
                       sizeof(test_struct_t),
                       0,
                       SIMPLE_SORT_NUMBER - 1);
    //-------------------------------------------------------------------------
    //随机数生成器,获取随机数种子,获取随机数
    srand(SIMPLE_SORT_SRAND);
    for (i = 0; i < SIMPLE_SORT_NUMBER; i++)
        temp1[i].xxx = rand() % 500;
    //-------------------------------------------------------------------------

    while (1) {
        printf("\n--------------------------------------------------------\n");
        for (i = 0; i < SIMPLE_SORT_NUMBER; i++)
            temp2[i].xxx = temp1[i].xxx;
        
        switch (flag++) {
        case 0:
            bubble_sort(test_data);
            true_or_false = 0;
            break;
        case 1:
            selection_sort(test_data, &helper1);
            true_or_false = 0;
            break;
        case 2:
            insertion_sort(test_data, &helper1);
            true_or_false = 0;
            break;
        case 3:
            shell_sort(test_data, &helper1, Hibbard);
            true_or_false = 0;
            break;
        case 4:
            heap_sort(test_data);
            true_or_false = 0;
            break;
        case 5:
            merge_sort(test_data, (void *)helper2);
            true_or_false = 0;
            break;
        case 6:
            quick_sort(test_data, helper3);
            true_or_false = 0;
            break;
        default:
            true_or_false = 0;
            free(temp1);
            free(temp2);
            free(helper2);
            return;
        }
        
        printf_all(temp2, true_or_false);
        printf("\n %d sort result:%d\n", flag - 1, simple_sort_check(temp2));
    }
}

#endif
