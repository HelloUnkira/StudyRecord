#ifndef MISC_FUNCTION_H
#define MISC_FUNCTION_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
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
/* 移动指定字符到尾部(Reverse时移动到头部) */
static inline void Misc_StringMoveCharToEnd(char *String, char Char, bool Reverse)
{
    uint32_t Length = strlen(String);
    /* 逆序,移动字符到头部使用 */
    if (Reverse) {
        char    *Arr = String;
        uint32_t Len = Length;
        do {/* 通用组件:逆序数组 */
            for (uint32_t Idx0 = 0; Idx0 < Len / 2; Idx0++) {
                 uint32_t Idx1 = Len - 1 - Idx0;
                 char Tmp = Arr[Idx0];
                Arr[Idx0] = Arr[Idx1];
                Arr[Idx1] = Tmp;
            }
        } while (0);
    }
    /* 双指针移动字符 */
    if (true) {
        uint32_t Pos1 = 0, Pos2 = 0;
        for (; Pos2 < Length; Pos2++)
            if (String[Pos2]  != Char)
                String[Pos1++] = String[Pos2];
        for (; Pos1 < Length; Pos1++)
                String[Pos1] = Char;
    }
    /* 逆序,移动字符到头部使用 */
    if (Reverse) {
        char    *Arr = String;
        uint32_t Len = Length;
        do {/* 通用组件:逆序数组 */
            for (uint32_t Idx0 = 0; Idx0 < Len / 2; Idx0++) {
                 uint32_t Idx1 = Len - 1 - Idx0;
                 char Tmp = Arr[Idx0];
                Arr[Idx0] = Arr[Idx1];
                Arr[Idx1] = Tmp;
            }
        } while(0);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 循环左旋转字符串(Reverse时循环右旋转) */
static inline void Misc_StringRotateLeft(char *String, uint32_t Offset, bool Reverse)
{
    uint32_t Length = strlen(String);
    
    if (Reverse)
        Offset  = Length - Offset;
        Offset %= Length;
    if (Offset == 0 || Length == 0)
        return;
    
    uint32_t Pos[3][2] = {{0, Offset - 1}, {Offset, Length - 1}, {0, Length - 1},};
    
    for (uint8_t I = 0; I < 3; I++) {
        for (uint32_t Pos1 = Pos[I][0], Pos2 = Pos[I][1]; Pos1 < Pos2; Pos1++, Pos2--) {
            char   Char  = String[Pos1];
            String[Pos1] = String[Pos2];
            String[Pos2] = Char;
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 指定字符为分割点逆序字符串 */
static inline void Misc_StringReverse(char *String, char Char)
{
    uint32_t Length = strlen(String);
    
    if (Length == 0)
        return;
    
    uint32_t Pos = 0, Pos1 = 0, Pos2 = 0;
    
    while(Pos < Length) {
        for (Pos1 = Pos;  String[Pos1] == Char && String[Pos1] != 0; Pos1++);
        for (Pos2 = Pos1; String[Pos2] != Char && String[Pos2] != 0; Pos2++);
        for (Pos  = Pos2--; Pos1 < Pos2; Pos1++, Pos2--) {
                char Val = String[Pos1];
            String[Pos1] = String[Pos2];
            String[Pos2] = Val;
        }
    }
    for (Pos1 = 0, Pos2 = Length - 1; Pos1 < Pos2; Pos1++, Pos2--) {
            char Val = String[Pos1];
        String[Pos1] = String[Pos2];
        String[Pos2] = Val;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
