#include <stdio.h>
#include <stdlib.h>
#include "GeneralHeap.h"

typedef struct DataStructForTest {
    uint8_t  Data1;
    uint16_t Data2;
    uint32_t Data4;
    uint64_t Data8;
    double DataD;
    float DataF;
} DataTest;

bool GH_HeapCompare(void *Node1, void *Node2)
{
    return ((DataTest *)Node1)->Data4 >
           ((DataTest *)Node2)->Data4 ? true : false;
}

int main(int argc, char *argv[]) {
    
    #define LENGTH  10
    GH_Heap Heap = {0};
    void   *Array[LENGTH] = {NULL};
    
    GH_HeapReset(&Heap, Array, LENGTH, GH_HeapCompare);
    
    printf("\n");
    for (uint32_t I = 0; I < LENGTH; I++) {
        DataTest *Data = malloc(sizeof(DataTest));
        Data->Data4 = rand() % 100;
        printf("%d ", Data->Data4);
        GH_HeapPush(&Heap, Data);
    }
    printf("\n");
    DataTest *Data = NULL;
    while (GH_HeapPop(&Heap, &Data) == true) {
        printf("%d ", Data->Data4);
        free(Data);
    }
    printf("\n");
}
