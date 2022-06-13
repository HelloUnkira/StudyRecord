/* 例: */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "DoubleList.h"
#include "SingleList.h"

typedef struct TestDataStruct {
    uint64_t Data8;
    uint32_t Data4;
    uint16_t Data2;
    uint8_t  Data1;
    /* 为了简化将其合并书写 */
    union {
        /* 我们将双向链表侵入到该数据集合内部 */
        DL_Node DL_N;
        /* 我们将单向链表侵入到该数据集合内部 */
        SL_Node SL_N;
    };
    /*  */
    float    DataF;
    double   DataD;
    char     DataS[20];
    /* ...... */
} TestData;

void SetTestData(TestData *Data, uint8_t Value)
{
    Data->Data8 = Value;
    Data->Data4 = Value;
    Data->Data2 = Value;
    Data->Data1 = Value;
    Data->DataF = Value;
    Data->DataD = Value;
    sprintf(Data->DataS,"Index:%u", Value);
}

void CheckTestData(TestData *Data)
{
    printf("Data8:%6llu,",  Data->Data8);
    printf("Data4:%6llu,",  Data->Data4);
    printf("Data2:%6llu,",  Data->Data2);
    printf("Data1:%6llu,",  Data->Data1);
    printf("DataF:%6f,",    Data->DataF);
    printf("DataD:%8f,",    Data->DataD);
    printf("DataS:%s,",     Data->DataS);
    printf("\r\n");
}

void TestDoubleList(void)
{
    uint32_t Index = 0;
    
    DL_Node *Target = NULL;
    DL_Node *Node = NULL;
    DL_List *DList = malloc(sizeof(DL_List));
    /* 初始化双向链表体 */
    DL_List_Reset(DList);
    printf("------------------------------------------------------------\r\n");
    /* 批量生成并追加节点 */
    for (Index = 0, Target = NULL; Index < 10; Index++) {
        /* 生成数据实体并初始化 */
        TestData *Data = malloc(sizeof(TestData));
        SetTestData(Data, Index);
        Node = &(Data->DL_N);
        DL_Node_Reset(Node);
        DL_List_InsertAppend(DList, Target, Node);
        Target = Node;
    }
    printf("------------------------------------------------------------\r\n");
    /* 正向迭代检查 */
    DL_List_Traverse_Backward(DList, Node)
        CheckTestData(DL_GetOwner(TestData, DL_N, Node));
    printf("------------------------------------------------------------\r\n");
    /* 逆向迭代检查 */
    DL_List_Traverse_Forward(DList, Node)
        CheckTestData(DL_GetOwner(TestData, DL_N, Node));
    printf("------------------------------------------------------------\r\n");
    for (Index = 0; Index < 10; Index++) {
        /* 销毁所有节点并回收数据空间 */
        Node = DL_List_GetHead(DList);
        DL_List_Remove(DList, Node);
        free(DL_GetOwner(TestData, DL_N, Node));
    }
    printf("------------------------------------------------------------\r\n");
    /* 正向迭代检查 */
    DL_List_Traverse_Backward(DList, Node)
        CheckTestData(DL_GetOwner(TestData, DL_N, Node));
    printf("------------------------------------------------------------\r\n");
    /* 逆向迭代检查 */
    DL_List_Traverse_Forward(DList, Node)
        CheckTestData(DL_GetOwner(TestData, DL_N, Node));
    printf("------------------------------------------------------------\r\n");
    /* 批量生成并追加节点 */
    for (Index = 0, Target = NULL; Index < 10; Index++) {
        /* 生成数据实体并初始化 */
        TestData *Data = malloc(sizeof(TestData));
        SetTestData(Data, Index);
        Node = &(Data->DL_N);
        DL_Node_Reset(Node);
        DL_List_InsertPrepend(DList, Target, Node);
        Target = Node;
    }
    printf("------------------------------------------------------------\r\n");
    /* 正向迭代检查 */
    DL_List_Traverse_Backward(DList, Node)
        CheckTestData(DL_GetOwner(TestData, DL_N, Node));
    printf("------------------------------------------------------------\r\n");
    /* 逆向迭代检查 */
    DL_List_Traverse_Forward(DList, Node)
        CheckTestData(DL_GetOwner(TestData, DL_N, Node));
    printf("------------------------------------------------------------\r\n");
    for (Index = 0; Index < 10; Index++) {
        /* 销毁所有节点并回收数据空间 */
        Node = DL_List_GetHead(DList);
        DL_List_Remove(DList, Node);
        free(DL_GetOwner(TestData, DL_N, Node));
    }
    printf("------------------------------------------------------------\r\n");
    /* 正向迭代检查 */
    DL_List_Traverse_Backward(DList, Node)
        CheckTestData(DL_GetOwner(TestData, DL_N, Node));
    printf("------------------------------------------------------------\r\n");
    /* 逆向迭代检查 */
    DL_List_Traverse_Forward(DList, Node)
        CheckTestData(DL_GetOwner(TestData, DL_N, Node));
    printf("------------------------------------------------------------\r\n");
}

void TestSingleList(void)
{
    uint32_t Index = 0;
    
    SL_Node *Node = NULL;
    SL_List *SL_Head = malloc(sizeof(SL_List));
    SL_List *SL_Tail = malloc(sizeof(SL_List));
    /* 初始化单向链表体 */
    SL_List_Reset(SL_Head, SL_Tail);
    printf("------------------------------------------------------------\r\n");
    /* 批量生成并追加节点 */
    for (Index = 0; Index < 10; Index++) {
        /* 生成数据实体并初始化 */
        TestData *Data = malloc(sizeof(TestData));
        SetTestData(Data, Index);
        Node = &(Data->SL_N);
        SL_Node_Reset(Node);
        SL_List_Prepend(SL_Head, SL_Tail, Node);
    }
    printf("------------------------------------------------------------\r\n");
    /* 迭代检查 */
    SL_List_Traserve(SL_Head, SL_Tail, Node)
        CheckTestData(SL_GetOwner(TestData, SL_N, Node));
    printf("------------------------------------------------------------\r\n");
    for (Index = 0; Index < 10; Index++) {
        /* 销毁所有节点并回收数据空间 */
        Node = SL_List_GetHead(SL_Head);
        SL_List_Remove(SL_Head, SL_Tail, NULL, Node);
        free(SL_GetOwner(TestData, SL_N, Node));
    }
    printf("------------------------------------------------------------\r\n");
    /* 迭代检查 */
    SL_List_Traserve(SL_Head, SL_Tail, Node)
        CheckTestData(SL_GetOwner(TestData, SL_N, Node));
    printf("------------------------------------------------------------\r\n");
    /* 批量生成并追加节点 */
    for (Index = 0; Index < 10; Index++) {
        /* 生成数据实体并初始化 */
        TestData *Data = malloc(sizeof(TestData));
        SetTestData(Data, Index);
        Node = &(Data->SL_N);
        SL_Node_Reset(Node);
        SL_List_Append(SL_Head, SL_Tail, Node);
    }
    printf("------------------------------------------------------------\r\n");
    /* 迭代检查 */
    SL_List_Traserve(SL_Head, SL_Tail, Node)
        CheckTestData(SL_GetOwner(TestData, SL_N, Node));
    printf("------------------------------------------------------------\r\n");
    for (Index = 0; Index < 10; Index++) {
        /* 销毁所有节点并回收数据空间 */
        Node = SL_List_GetHead(SL_Head);
        SL_List_Remove(SL_Head, SL_Tail, NULL, Node);
        free(SL_GetOwner(TestData, SL_N, Node));
    }
    printf("------------------------------------------------------------\r\n");
    /* 迭代检查 */
    SL_List_Traserve(SL_Head, SL_Tail, Node)
        CheckTestData(SL_GetOwner(TestData, SL_N, Node));
    printf("------------------------------------------------------------\r\n");
}

int main(int argc, char *argv[]) {
    
    printf("\nTestDoubleList:\n");
    TestDoubleList();
    printf("\nTestSingleList:\n");
    TestSingleList();
}
