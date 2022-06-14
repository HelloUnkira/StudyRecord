#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"

typedef struct DataStructForTest {
    uint8_t  Data1;
    uint16_t Data2;
    uint32_t Data4;
    uint64_t Data8;
    
    /**/
    Stack StackNode;
    /**/
    
    double DataD;
    float DataF;
    
} DataTest;

int main(int argc, char *argv[]) {
    
    Stack StackTop;
    Stack_ResetTop(&StackTop);
    
    printf("\n");
    for (uint32_t I = 0; I < 10; I++) {
        DataTest *Data = malloc(sizeof(DataTest));
        Data->Data4 = I;
        Stack_ResetNode(&(Data->StackNode));
        Stack_Push(&StackTop, &(Data->StackNode));
        printf("%d ", Data->Data4);
    }
    printf("\n");
    for (Stack *Node = Stack_Pop(&StackTop); Node != NULL;
                Node = Stack_Pop(&StackTop)) {
        DataTest *Data = Stack_GetOwner(DataTest, StackNode, Node);
        printf("%d ", Data->Data4);
        free(Data);
    }
    printf("\n");
}
