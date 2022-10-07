#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

typedef struct DataStructForTest {
    uint8_t  Data1;
    uint16_t Data2;
    uint32_t Data4;
    uint64_t Data8;
    
    /**/
    Queue QueueNode;
    /**/
    
    double DataD;
    float DataF;
    
} DataTest;

int main(int argc, char *argv[]) {
    
    Queue QueueHead;
    Queue QueueTail;
    Queue_ResetQueue(&QueueHead, &QueueTail);
    
    printf("\n");
    for (uint32_t I = 0; I < 10; I++) {
        DataTest *Data = malloc(sizeof(DataTest));
        Data->Data4 = I;
        Queue_ResetNode(&(Data->QueueNode));
        Queue_EnQueue(&QueueHead, &QueueTail, &(Data->QueueNode));
        printf("%d ", Data->Data4);
    }
    printf("\n");
    for (Queue *Node = Queue_DeQueue(&QueueHead, &QueueTail); Node != NULL;
                Node = Queue_DeQueue(&QueueHead, &QueueTail)) {
        DataTest *Data = Queue_GetOwner(DataTest, QueueNode, Node);
        printf("%d ", Data->Data4);
        free(Data);
    }
    printf("\n");
}
