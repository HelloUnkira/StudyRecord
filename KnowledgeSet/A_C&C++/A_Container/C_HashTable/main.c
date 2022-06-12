#include <stdio.h>
#include "HashTable.h"

//哈希函数，选取部分哈希函数 
int Elf_HashFunction(char *name, int mod)
{
    unsigned long h = 0, g;
    while (*name) {
        h = (h << 4) + *name++;
        if (g = h & 0xf0000000)
            h ^= g >> 24;
        h &= ~g;
    }
    return (mod - (int)h % mod) % mod;
    
    
    
}

int main(int argc, char *argv[]) {
    
    /* 暂未测试 */
    
    return 0;
}
