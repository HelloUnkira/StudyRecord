#include <stdio.h>
#include "TLSF_Strategy.h"

int main(int argc, char *argv[]) {
    
    uint8_t Memory[16 * 1024] = {0};
    uintptr_t TLSF = TLSF_Init(Memory + 1, 16 * 1024);
    TLSF_LayoutCheck(TLSF);
    
    void *Buffer[10] = {0};
    
    for (uintptr_t Index = 0; Index < 10; Index++)
        Buffer[Index] = TLSF_Alloc(TLSF, 1024);
    TLSF_LayoutCheck(TLSF);
    
    for (uintptr_t Index = 0; Index < 10; Index++)
        TLSF_Free(TLSF, Buffer[Index]);
    TLSF_LayoutCheck(TLSF);
    
    for (uintptr_t Index = 0; Index < 10; Index++)
        Buffer[Index] = TLSF_Alloc(TLSF, 1024);
    TLSF_LayoutCheck(TLSF);
    
    for (uintptr_t Index = 0; Index < 10; Index++)
        TLSF_Free(TLSF, Buffer[10 - 1 - Index]);
    TLSF_LayoutCheck(TLSF);
    
    void *BufferR = NULL;
    
    for (uintptr_t Index = 0; Index < 10; Index++)
        BufferR = TLSF_ReAlloc(TLSF, BufferR, 1024 * (Index + 1));
    TLSF_LayoutCheck(TLSF);
    
    TLSF_Free(TLSF, BufferR);
    TLSF_LayoutCheck(TLSF);
    
    return 0;
}


