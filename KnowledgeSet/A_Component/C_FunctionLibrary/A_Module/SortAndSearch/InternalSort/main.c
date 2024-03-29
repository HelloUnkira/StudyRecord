
/* 例: */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "InternalSort.h"

#define SIMPLE_SORT_NUMBER 1000000
#define SIMPLE_SORT_DOUBLE SIMPLE_SORT_NUMBER * 2
#define SIMPLE_SORT_TRIPLE SIMPLE_SORT_NUMBER * 3
#define SIMPLE_SORT_SRAND  978563412
#include <stdio.h>

//如果结构体被替换成普通元素,退化的过程
typedef struct DataStructForTest {
    uint32_t    xxx; //关键字
    uint8_t     yyy;
    uint16_t    ll;
    float       pp;
    uint32_t    tax;
    double      tt;
    void       *other;
} TestData;

static uint8_t TestCompare(void *Temp1, void *Temp2)
{
    if (Temp1 == NULL || Temp2 == NULL)
        printf("\nspecial error\n");
    
    if (((TestData *)Temp1)->xxx <  ((TestData *)Temp2)->xxx)
        return 0;
    if (((TestData *)Temp1)->xxx == ((TestData *)Temp2)->xxx)
        return 1;
    if (((TestData *)Temp1)->xxx >  ((TestData *)Temp2)->xxx)
        return 2;
    printf("\nspecial error\n");
    return (~0);
}

static uint32_t Hibbard(uint32_t x)
{//希尔排序用到的增量序列
    uint32_t Temp = (x + 1) / 2;
    return   Temp <= x ? Temp : 1;
}

static bool TestBitsLevel(void *Temp, uint32_t Level)
{
    return ((((TestData *)Temp)->xxx) & (1 << Level)) != 0;
}

static uint64_t TestUInt64Value(void *Temp)
{
    return ((TestData *)Temp)->xxx;
}

static void PrintAll(TestData *Test, uint32_t TrueOrFalse)
{
    if (TrueOrFalse == 0)
        return;

    uint32_t I = 0;
    while (I < SIMPLE_SORT_NUMBER)
        printf("%3d ", Test[I++].xxx);
    printf("\n");
}

static uint8_t TestCheck(TestData *Test)
{
    uint32_t I = 0;
    for (I = 1; I < SIMPLE_SORT_NUMBER; I++)
        if (Test[I - 1].xxx > Test[I].xxx)
            return 0;
    return (~0);
}

static void TestSort(void)
{
	printf("bytes:%d\n", sizeof(TestData));
    
    uint32_t I = 0;
    uint32_t Flag = 0;
    uint32_t TrueOrFalse = 0;
    TestData *Temp1 = NULL;
    TestData *Temp2 = NULL;
    TestData   Helper1 = {0};
    TestData  *Helper2 = NULL;
    uint32_t  *Helper3 = NULL;
    uint32_t  *Helper4 = NULL;
    Temp1   = (TestData *)calloc(SIMPLE_SORT_NUMBER, sizeof(TestData));
    Temp2   = (TestData *)calloc(SIMPLE_SORT_NUMBER, sizeof(TestData));
    Helper2 = (TestData *)calloc(SIMPLE_SORT_NUMBER, sizeof(TestData));
    //数据量越大log n 越小于 n,否则很接近,所以为兼容各个情况
    Helper3 = (uint32_t *)calloc(SIMPLE_SORT_DOUBLE, sizeof(uint32_t));
    Helper4 = (uint32_t *)calloc(SIMPLE_SORT_TRIPLE, sizeof(uint32_t));
    //-------------------------------------------------------------------------
    void *test_data = malloc(sizeof(InternalSort_Data));
    InternalSort_SetDataSet(test_data, (void *)Temp2,
                            sizeof(TestData), 0, SIMPLE_SORT_NUMBER - 1);
    //-------------------------------------------------------------------------
    //随机数生成器,获取随机数种子,获取随机数
    srand(SIMPLE_SORT_SRAND);
    for (I = 0; I < SIMPLE_SORT_NUMBER; I++)
        Temp1[I].xxx = rand() % 500;
    //-------------------------------------------------------------------------
    
    while (1) {
        printf("\n--------------------------------------------------------\n");
        for (I = 0; I < SIMPLE_SORT_NUMBER; I++)
            Temp2[I].xxx = Temp1[I].xxx;
        
        switch (Flag++) {
        case 0:
            break;  /* 太慢了,测试完成直接跳过 */
            InternalSort_SetCompare(test_data, TestCompare);
            InternalSort_Bubble(test_data);
            TrueOrFalse = 0;
            break;
        case 1:
            break;  /* 太慢了,测试完成直接跳过 */
            InternalSort_SetCompare(test_data, TestCompare);
            InternalSort_Selection(test_data, &Helper1);
            TrueOrFalse = 0;
            break;
        case 2:
            break;  /* 太慢了,测试完成直接跳过 */
            InternalSort_SetCompare(test_data, TestCompare);
            InternalSort_Insertion(test_data, &Helper1);
            TrueOrFalse = 0;
            break;
        case 3:
            InternalSort_SetCompare(test_data, TestCompare);
            InternalSort_SetHill(test_data, Hibbard);
            InternalSort_Shell(test_data, &Helper1);
            TrueOrFalse = 0;
            break;
        case 4:
            InternalSort_SetCompare(test_data, TestCompare);
            InternalSort_Heap(test_data);
            TrueOrFalse = 0;
            break;
        case 5:
            InternalSort_SetCompare(test_data, TestCompare);
            InternalSort_Merge(test_data, Helper2);
            TrueOrFalse = 0;
            break;
        case 6:
            InternalSort_SetCompare(test_data, TestCompare);
            InternalSort_Quick(test_data, Helper3);
            TrueOrFalse = 0;
            break;
        case 7:
            InternalSort_SetBitsLevel(test_data, TestBitsLevel);
            InternalSort_Radix1(test_data, Helper4, sizeof(uint32_t) * 8);
            TrueOrFalse = 0;
            break;
        case 8:
            InternalSort_SetUInt64Value(test_data, TestUInt64Value);
            InternalSort_Radix2(test_data, Helper2, Helper4, 10);
            TrueOrFalse = 0;
            break;
        case 9:
            break;  /* 太慢了,测试完成直接跳过 */
            InternalSort_SetCompare(test_data, TestCompare);
            InternalSort_OddEven(test_data);
            TrueOrFalse = 0;
            break;
        case 10:
            InternalSort_SetCompare(test_data, TestCompare);
            InternalSort_Bitonic(test_data);
            TrueOrFalse = 0;
            break;
        default:
            TrueOrFalse = 0;
            free(Temp1);
            free(Temp2);
            free(Helper2);
            return;
        }
        
        PrintAll(Temp2, TrueOrFalse);
        TrueOrFalse = 0;
        printf("\n %d sort result:%d\n", Flag - 1, TestCheck(Temp2));
    }
}

int main(int argc, char *argv[])
{
    TestSort();
}

