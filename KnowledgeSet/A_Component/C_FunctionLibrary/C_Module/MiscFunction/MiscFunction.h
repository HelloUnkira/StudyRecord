#ifndef MISC_FUNCTION_H
#define MISC_FUNCTION_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 将一个索引下标数组随机打乱 */
static inline void Misc_Shuffle(uint32_t  *Array,  uint32_t Length,
                                uint32_t (*Random)(uint32_t Number))
{
    /* 1.先将其序列化 */
    for (uint32_t Index = 0; Index < Length; Index++)
        Array[Index] = Index;
    /* 2.逆向乱序 */
    for (uint32_t Index = 0; Index < Length; Index++) {
        /* 取一个随机下标 */
        uint32_t Index1 = Random(Length - Index);
        uint32_t Index2 = Length - 1 - Index;
        /* 交换Index1 与 Length - 1 - Index */
        uint32_t Temp = Array[Index1];
        Array[Index1] = Array[Index2];
        Array[Index2] = Temp;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
