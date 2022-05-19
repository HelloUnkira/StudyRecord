/* 例: */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "ListIterator.h"

typedef struct DataStructForTest {
    uint64_t Data8;
    uint32_t Data4;
    uint16_t Data2;
    uint8_t  Data1;
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
    
    DListBody *List = malloc(sizeof(DListBody));
    /* 初始化双向链表体 */
    DList_Reset(List);
    /* 批量生成并追加节点 */
    DListNode *Node = NULL;
    for (Index = 0, Node = NULL; Index < 10; Index++) {
        /* 生成数据实体并初始化 */
        TestData *Data = malloc(sizeof(TestData));
        SetTestData(Data, Index);
        DListNode *Temp = malloc(sizeof(DListNode));
        DNode_Reset(Temp);
        DNode_SetMember(Temp, Data);
        DList_InsertAppend(List, Node, Temp);
        Node = Temp;
    }
    printf("------------------------------------------------------------\r\n");
    /* 正向迭代检查 */
    DList_Traverse_Backward(List, Node)
        CheckTestData(DNode_GetMember(Node));
    printf("------------------------------------------------------------\r\n");
    /* 逆向迭代检查 */
    DList_Traverse_Forward(List, Node)
        CheckTestData(DNode_GetMember(Node));
    printf("------------------------------------------------------------\r\n");
    for (Index = 0; Index < 10; Index++) {
        /* 销毁所有节点并回收数据空间 */
        Node = DList_GetHead(List);
        DList_Remove(List, Node);
        free(DNode_GetMember(Node));
        free(Node);
    }
    printf("------------------------------------------------------------\r\n");
    /* 正向迭代检查 */
    DList_Traverse_Backward(List, Node)
        CheckTestData(DNode_GetMember(Node));
    printf("------------------------------------------------------------\r\n");
    /* 逆向迭代检查 */
    DList_Traverse_Forward(List, Node)
        CheckTestData(DNode_GetMember(Node));
    printf("------------------------------------------------------------\r\n");
    for (Index = 0, Node = NULL; Index < 10; Index++) {
        /* 生成数据实体并初始化 */
        TestData *Data = malloc(sizeof(TestData));
        SetTestData(Data, Index);
        DListNode *Temp = malloc(sizeof(DListNode));
        DNode_Reset(Temp);
        DNode_SetMember(Temp, Data);
        DList_InsertPrepend(List, Node, Temp);
        Node = Temp;
    }
    printf("------------------------------------------------------------\r\n");
    /* 正向迭代检查 */
    DList_Traverse_Backward(List, Node)
        CheckTestData(DNode_GetMember(Node));
    printf("------------------------------------------------------------\r\n");
    /* 逆向迭代检查 */
    DList_Traverse_Forward(List, Node)
        CheckTestData(DNode_GetMember(Node));
    printf("------------------------------------------------------------\r\n");
    for (Index = 0; Index < 10; Index++) {
        /* 销毁所有节点并回收数据空间 */
        Node = DList_GetTail(List);
        DList_Remove(List, Node);
        free(DNode_GetMember(Node));
        free(Node);
    }
    printf("------------------------------------------------------------\r\n");
    /* 正向迭代检查 */
    DList_Traverse_Backward(List, Node)
        CheckTestData(DNode_GetMember(Node));
    printf("------------------------------------------------------------\r\n");
    /* 逆向迭代检查 */
    DList_Traverse_Forward(List, Node)
        CheckTestData(DNode_GetMember(Node));
    printf("------------------------------------------------------------\r\n");
}

void TestSingleList(void)
{
    /* 如上类似,不做单元测试了 */
}



int main(int argc, char *argv[]) {
    
    TestDoubleList();
    TestSingleList();
}


