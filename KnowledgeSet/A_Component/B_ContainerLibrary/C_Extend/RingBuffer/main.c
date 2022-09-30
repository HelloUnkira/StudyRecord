/* 例: */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "RingBuffer.h"

//迭代位置过大,需要单独测试索引
static void Test_RewindIndex(void)
{
    uint32_t Index = 0;
    
    for (Index = 0; 1; Index++)
        if (Index >= 0x80000000) {
            
            uint32_t rewind = 0x80000000;
            rewind /= 1000;
            rewind *= 1000;
            
            printf("Index:%u Rewind:%u Max:%u\r\n", 
                    Index, rewind, 0x80000000);
            
            Index -= rewind;
            
            printf("Index:%u Rewind:%u Max:%u\r\n", 
                    Index, rewind, 0x80000000);
        }
}

int main(int argc, char *argv[]) {
    
    //暴力测试索引回退算法正确性
    //Test_RewindIndex();
    
    static RingBuffer instance1;
    static RingBuffer instance2;
    static RingBuffer instance4;
    static RingBuffer instance8;
    
    static uint8_t  Buffer1[1024] = {0}, temp1[1024] = {0};
    static uint16_t Buffer2[1024] = {0}, temp2[1024] = {0};
    static uint32_t Buffer4[1024] = {0}, temp4[1024] = {0};
    static uint64_t Buffer8[1024] = {0}, temp8[1024] = {0};
    
    RingBuffer_Init(&instance1, 1, Buffer1, 1024);
    RingBuffer_Init(&instance2, 2, Buffer2, 1024);
    RingBuffer_Init(&instance4, 4, Buffer4, 1024);
    RingBuffer_Init(&instance8, 8, Buffer8, 1024);
    
    int i = 0;
    int index = 0;
    
    for (i = 0; i < 1024; i++) {
        temp1[i] = i;
        temp2[i] = i;
        temp4[i] = i;
        temp8[i] = i;
    }
        
    for (index = 0; index < 10; index++) {
        /* 暴力测试 */
        RingBuffer_Puts(&instance1, temp1, 1024);
        RingBuffer_Puts(&instance2, temp2, 1024);
        RingBuffer_Puts(&instance4, temp4, 1024);
        RingBuffer_Puts(&instance8, temp8, 1024);

        RingBuffer_Gets(&instance1, temp1, 1024);
        RingBuffer_Gets(&instance2, temp2, 1024);
        RingBuffer_Gets(&instance4, temp4, 1024);
        RingBuffer_Gets(&instance8, temp8, 1024);

    }

	return 0;
}
