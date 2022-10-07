#include <stdio.h>
#include <stdlib.h>
#include "Heap.h"

typedef struct DataStructForTest {
    uint8_t  Data1;
    uint16_t Data2;
    uint32_t Data4;
    uint64_t Data8;
    double DataD;
    float DataF;
} DataTest;

bool Heap_Compare(void *Node1, void *Node2)
{
    return ((DataTest *)Node1)->Data4 >
           ((DataTest *)Node2)->Data4 ? true : false;
}

int main(int argc, char *argv[]) {
    
    #define LENGTH  10
    Heap  Instance = {0};
    void *Array[LENGTH] = {NULL};
    
    Heap_Reset(&Instance, Array, LENGTH, Heap_Compare);
    
    printf("\n");
    for (uint32_t I = 0; I < LENGTH; I++) {
        DataTest *Data = malloc(sizeof(DataTest));
        Data->Data4 = rand() % 100;
        printf("%d ", Data->Data4);
        Heap_Push(&Instance, Data);
    }
    printf("\n");
    DataTest *Data = NULL;
    while (Heap_Pop(&Instance, &Data) == true) {
        printf("%d ", Data->Data4);
        free(Data);
    }
    printf("\n");
}
