#include "SGui_Kernel.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 通配辅助函数:拷贝函数 */
void SGui_Copy(void *Stream1, void *Stream2, uint32_t Length)
{
#if 0
    // memcpy(Stream1, Stream2, Length);
#else
    
#define SGui_Copy_Unit(Type) {                      \
        *((Type *)Stream1) = *((Type *)Stream2);    \
        Stream1 += sizeof(Type);                    \
        Stream2 += sizeof(Type);                    \
        Length  -= sizeof(Type);                    \
}

#define SGui_Copy_Type(Type) {                      \
    while (Length >= sizeof(Type)) {                \
        *((Type *)Stream1) = *((Type *)Stream2);    \
        Stream1 += sizeof(Type);                    \
        Stream2 += sizeof(Type);                    \
        Length  -= sizeof(Type);                    \
    }                                               \
}

    /* 至少保证一个流是平台字节对齐 */
    uint8_t Alignment1 = (uint64_t)(Stream1) % sizeof(void *);
    uint8_t Alignment2 = (uint64_t)(Stream2) % sizeof(void *);
    
    while (Alignment1 != 0 && Alignment2 != 0) {
        Alignment1--;
        Alignment2--;
        SGui_Copy_Unit(uint8_t);
    }

    SGui_Copy_Type(uint64_t);
    SGui_Copy_Type(uint32_t);
    SGui_Copy_Type(uint16_t);
    SGui_Copy_Type(uint8_t);
    
#endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 通配辅助函数:交换函数 */
void SGui_Swap(void *Stream1, void *Stream2, uint32_t Length)
{
#if 0
    //
#else
    
#define SGui_Swap_Unit(Type) {                  \
        Type Temp_1 = (*((Type *)Stream1));     \
        Type Temp_2 = (*((Type *)Stream2));     \
        *((Type *)Stream1) = Temp_2;            \
        *((Type *)Stream2) = Temp_1;            \
        Stream1 += sizeof(Type);                \
        Stream2 += sizeof(Type);                \
        Length  -= sizeof(Type);                \
}

#define SGui_Swap_Type(Type) {                  \
    while (Length >= sizeof(Type)) {            \
        Type Temp_1 = (*((Type *)Stream1));     \
        Type Temp_2 = (*((Type *)Stream2));     \
        *((Type *)Stream1) = Temp_2;            \
        *((Type *)Stream2) = Temp_1;            \
        Stream1 += sizeof(Type);                \
        Stream2 += sizeof(Type);                \
        Length  -= sizeof(Type);                \
    }                                           \
}

    /* 至少保证一个流是平台字节对齐 */
    uint8_t Alignment1 = (uint64_t)(Stream1) % sizeof(void *);
    uint8_t Alignment2 = (uint64_t)(Stream2) % sizeof(void *);
    
    while (Alignment1 != 0 && Alignment2 != 0) {
        Alignment1--;
        Alignment2--;
        SGui_Swap_Unit(uint8_t);
    }
    
    SGui_Swap_Type(uint64_t);
    SGui_Swap_Type(uint32_t);
    SGui_Swap_Type(uint16_t);
    SGui_Swap_Type(uint8_t);
    
#endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
