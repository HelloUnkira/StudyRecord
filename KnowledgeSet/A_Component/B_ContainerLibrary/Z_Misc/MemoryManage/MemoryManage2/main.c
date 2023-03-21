#include <stdio.h>
#include "HeapStrategy.h"

int main(int argc, char *argv[]) {
    
    HeapStrategy Instance;
    uint8_t Memory[16 * 1024] = {0};
    printf("MMHS_Init: %d\n", MMHS_Init(&Instance, Memory + 1, 16 * 1024));
    
    void *Buffer[10] = {0};
    
    for (uintptr_t Index = 0; Index < 10; Index++) {
        Buffer[Index] = MMHS_Alloc(&Instance, 1024);
        printf("MMHS_Alloc: %p\n", Buffer[Index]);
    }
    
    for (uintptr_t Index = 0; Index < 10; Index++) {
        printf("MMHS_Free:%p, %d\n", Buffer[Index], MMHS_Free(&Instance, Buffer[Index]));
    }
    
    MMHS_LayoutCheck(&Instance);
    
    return 0;
}


