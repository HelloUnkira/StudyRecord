/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "Cflint.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#ifdef CFLINT_USE_FUNCTION_SET1
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 交换:Operand0 <=> Operand1 */
void Cflint_Swap(Cflint_Type *Operand0, Cflint_Type *Operand1, uint32_t Length)
{
    Cflint_Type Temp0 = 0, Temp1 = 0;
    for (uint32_t Index = 0; Index < Length; Index++) {
        Temp0 = Operand0[Index]; Temp1 = Operand1[Index];
        Operand0[Index] = Temp1; Operand1[Index] = Temp0;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 拷贝:Operand0 = Operand1 */
void Cflint_Copy(Cflint_Type *Operand0, Cflint_Type *Operand1, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Operand0[Index] = Operand1[Index];
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 比较:((>:1);(==:0);(<:-1)) */
int8_t Cflint_Compare(Cflint_Type *Operand0, Cflint_Type *Operand1, uint32_t Length)
{
    for (int64_t Index = (int64_t)Length - 1; Index >= 0; Index--)
        if (Operand0[Index] != Operand1[Index]) {
            if (Operand0[Index] > Operand1[Index])
                return 1;
            if (Operand0[Index] < Operand1[Index])
                return -1;
        }
    return 0;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 相等判断:((==:1);(!=:0)) */
bool Cflint_Equal(Cflint_Type *Operand0, Cflint_Type *Operand1, uint32_t Length)
{
    Cflint_Type Result = 0;
    for (uint32_t Index = 0; Index < Length; Index++)
        Result |= Operand0[Index] ^ Operand1[Index];
    return Result == 0 ? true : false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 为0判断:Operand == 0 */
bool Cflint_IsZero(Cflint_Type *Operand, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        if (Operand[Index] != 0)
            return false;
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 设值:类似memset */
void Cflint_SetValue(Cflint_Type *Operand, uint32_t Length, Cflint_Type Value)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Operand[Index] = Value;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 检查一个数是否为2的幂次方 */
bool Cflint_IsExponent2(Cflint_Type *Operand, uint32_t Length)
{
    uint32_t UnZeroWord = 0;
    /* 1.每一个字都满足2的幂次方;2.只有一个非0字 */
    for (uint32_t Index = 0; Index < Length; Index++) {
        if ((Operand[Index] == 0))
            continue;
        if ((Operand[Index] & (Operand[Index] - 1)) != 0)
            return false;
        if (UnZeroWord++ != 0)
            return false;
    }
    if (UnZeroWord == 1)
        return true;
    return false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 计算2进制最高位索引 */
int64_t Cflint_Numbers2(Cflint_Type *Operand, uint32_t Length)
{
    /* 先计算前导0,这里使用普通的方式,使用折半查找效果会更好 */
    int64_t Index1 = 0, Index2 = 0;
    /* 粗粒度计算 */
    for (Index1 = (int64_t)Length - 1; Index1 >= 0; Index1--)
        if (Operand[Index1] != 0)
            break;
    /* 查验,全空时最高位索引为-1 */
    if (Index1 < 0) return -1;
    /* 不存在全空时,查验落点位 */
    for (Index2 = (int64_t)Cflint_Bits - 1; Index2 >= 0; Index2--)
        if ((Operand[Index1] & (1 << Index2)) != 0)
            break;
    /* 计算最高位 */
    return (Index1 * Cflint_Bits + Index2);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 位或运算 */
void Cflint_OR(Cflint_Type *Result, Cflint_Type *Operand1, Cflint_Type *Operand2, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Result[Index] = Operand1[Index] | Operand2[Index];
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 位与运算 */
void Cflint_AND(Cflint_Type *Result, Cflint_Type *Operand1, Cflint_Type *Operand2, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Result[Index] = Operand1[Index] & Operand2[Index];
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 位异或运算 */
void Cflint_XOR(Cflint_Type *Result, Cflint_Type *Operand1, Cflint_Type *Operand2, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Result[Index] = Operand1[Index] ^ Operand2[Index];
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 位反运算 */
void Cflint_NOT(Cflint_Type *Operand, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Operand[Index] = ~Operand[Index];
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 位检查 */
bool Cflint_CheckBit2(Cflint_Type *Operand, uint32_t Length, int64_t Bits2)
{
    int64_t Bits_N = Bits2 / Cflint_Bits;
    int64_t Bits_2 = Bits2 % Cflint_Bits;
    
    return Bits_N < Length ? ((Operand[Bits_N] & (1 << Bits_2)) != 0) : false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 位设置 */
void Cflint_SetBit2(Cflint_Type *Operand, uint32_t Length, int64_t Bits2)
{
    int64_t Bits_N = Bits2 / Cflint_Bits;
    int64_t Bits_2 = Bits2 % Cflint_Bits;
    
    if (Bits_N < Length)
        Operand[Bits_N] |= (1 << Bits_2);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 位清除 */
void Cflint_ClearBit2(Cflint_Type *Operand, uint32_t Length, int64_t Bits2)
{
    int64_t Bits_N = Bits2 / Cflint_Bits;
    int64_t Bits_2 = Bits2 % Cflint_Bits;
    
    if (Bits_N < Length)
        Operand[Bits_N] &= ~(1 << Bits_2);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 偶数判断 */
bool Cflint_IsEven(Cflint_Type *Operand, uint32_t Length)
{
    return ((Operand[0] & 1) == 0);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 左移位运算 */
void Cflint_ShiftLeft2(Cflint_Type *Operand, uint32_t Length, uint64_t Bits2)
{
    /* 特殊检查:空移位 */
    if (Bits2 == 0)
        return;
    /* 一个DIGIT_TYPE字节下进制的位有多少个2进制位 */
    int64_t Index  = 0;
    int64_t Bits_N = Bits2 / Cflint_Bits;
    int64_t Bits_2 = Bits2 % Cflint_Bits;
    int64_t Last_2 = Cflint_Bits - Bits_2;
    Cflint_Type Bit_N1 = 0, Bit_High = 0;
    Cflint_Type Bit_N2 = 0, Bit_Low  = 0;
    
    for (Index = (int64_t)Length - 1; Index - Bits_N - 1 >= 0; Index--) {
        Bit_N1    = Operand[Index - Bits_N];
        Bit_N2    = Operand[Index - Bits_N - 1];
        Bit_High  = (Bit_N1 << Bits_2);
        Bit_Low   = (Bit_N2 >> Last_2);
        Bit_Low   = (Last_2 >= Cflint_Bits) ? 0 : Bit_Low;
        Operand[Index] = Bit_High | Bit_Low;
    }
    {
        Bit_N1 = Operand[Index - Bits_N];
        //Bit_N2 = Operand[Index - Bits_N - 1];
        Bit_High  = (Bit_N1 << Bits_2);
        //Bit_Low   = (Bit_N2 >> Last_2);
        //Bit_Low   = (Last_2 >= Cflint_Bits) ? 0 : Bit_Low;
        Operand[Index] = Bit_High;// | (Bit_N2 >> Last_2);
    }
    
    for (Index = 0; Index - Bits_N < 0; Index++)
        Operand[Index] = 0;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 右移位运算 */
void Cflint_ShiftRight2(Cflint_Type *Operand, uint32_t Length, uint64_t Bits2)
{
    /* 特殊检查:空移位 */
    if (Bits2 == 0)
        return;
    /* 一个DIGIT_TYPE字节下进制的位有多少个2进制位 */
    int64_t Index  = 0;
    int64_t Bits_N = Bits2 / Cflint_Bits;
    int64_t Bits_2 = Bits2 % Cflint_Bits;
    int64_t Last_2 = Cflint_Bits - Bits_2;
    Cflint_Type Bit_N1 = 0, Bit_Low  = 0;
    Cflint_Type Bit_N2 = 0, Bit_High = 0;
    
    for (Index = 0; Index + Bits_N + 1 < (int64_t)Length; Index++) {
        Bit_N1    = Operand[Index + Bits_N];
        Bit_N2    = Operand[Index + Bits_N + 1];
        Bit_Low   = (Bit_N1 >> Bits_2);
        Bit_High  = (Bit_N2 << Last_2);
        Bit_High  = (Last_2 >= Cflint_Bits) ? 0 : Bit_High;
        Operand[Index] = Bit_Low | Bit_High;
    }
    {
        Bit_N1 = Operand[Index + Bits_N];
        //Bit_N2 = Operand[Index + Bits_N + 1];
        Bit_Low = (Bit_N1 >> Bits_2);
        //Bit_High = (Bit_N2 << Last_2);
        //Bit_High = (Last_2 >= Cflint_Bits) ? 0 : Bit_High;
        Operand[Index] = Bit_Low;// | (Bit_N2 << Last_2);
    }
    
    for (Index = (int64_t)Length - Bits_N; Index < (int64_t)Length; Index++)
        Operand[Index] = 0;
}
 /*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 左移位运算 */
void Cflint_ShiftLeftN(Cflint_Type *Operand, uint32_t Length, uint32_t BitsN)
{
    /* 超出限制时,直接清空 */
    if (BitsN >= Length) {
        Cflint_SetValue(Operand, Length, 0);
        return;
    }
    
    for (int64_t Index = 0; Index + BitsN < Length; Index++)
        Operand[Index] = Operand[Index + BitsN];
    for (int64_t Index = (int64_t)Length - BitsN; Index < Length; Index++)
        Operand[Index] = 0;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 右移位运算 */
void Cflint_ShiftRightN(Cflint_Type *Operand, uint32_t Length, uint32_t BitsN)
{
    /* 超出移位限制时,直接清空 */
    if (BitsN >= Length) {
        Cflint_SetValue(Operand, Length, 0);
        return;
    }
    
    for (int64_t Index = (int64_t)Length - 1; Index - BitsN >= 0; Index--)
        Operand[Index] = Operand[Index - BitsN];
    for (int64_t Index = (int64_t)BitsN - 1; Index >= 0; Index--)
        Operand[Index] = 0;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 数分解运算 */
int64_t Cflint_Factor2(Cflint_Type *Operand1, Cflint_Type *Operand2, uint32_t Length)
{
    /* 初始化Operand2=Operand1 */
    if (Operand2 != Operand1)
        Cflint_Copy(Operand2, Operand1, Length);
    /* 初始化Bits2=0 */
    int64_t Bits2 = 0;
    /* 特殊检查,Operand1==0 */
    if (Cflint_IsZero(Operand2, Length))
        return 0;
    /* 计算最低非0位所在位置 */
    while (!Cflint_CheckBit2(Operand2, Length, Bits2))
        Bits2++;
    /* 再次检查移位可行性 */
    Cflint_ShiftRight2(Operand2, Length, Bits2);
    /* 返回K */
    return Bits2;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 掩码运算 */
void Cflint_Mask2(Cflint_Type *Operand, uint32_t Length, int64_t Bits2)
{
    int64_t Bits_N = Bits2 / Cflint_Bits;
    int64_t Bits_2 = Bits2 % Cflint_Bits;
    Cflint_Type Bit_N = 0;
    /* 简要检查 */
    if (Bits2 > Length *Cflint_Bits || Bits2 == 0)
        return;
    /* 移除高位 */
    for (int64_t Index = Bits_N + 1; Index < Length; Index++)
        Operand[Index] = 0;
    /* 移出位内高位 */
    Bit_N = Operand[Bits_N];
    Operand[Bits_N] <<= (Cflint_Bits - Bits_2);
    Operand[Bits_N] >>= (Cflint_Bits - Bits_2);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Cflint_Native1ToBytes(uint8_t *Bytes, uint8_t *Native, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Bytes[Index] = Native[Length - 1 - Index];
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Cflint_BytesToNative1(uint8_t *Bytes, uint8_t *Native, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Native[Index] = Bytes[Length - 1 - Index];
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Cflint_Native2ToBytes(uint8_t *Bytes, uint16_t *Native, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++) {
        Bytes[Index * 2 + 0] = (uint8_t)(Native[Length - 1 - Index] >> 8);
        Bytes[Index * 2 + 1] = (uint8_t)(Native[Length - 1 - Index] >> 0);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Cflint_BytesToNative2(uint8_t *Bytes, uint16_t *Native, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++) {
        Native[Index] |= (uint16_t)Bytes[(Length - 1 - Index) * 2 + 0] << 8;
        Native[Index] |= (uint16_t)Bytes[(Length - 1 - Index) * 2 + 1] << 0;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Cflint_Native4ToBytes(uint8_t *Bytes, uint32_t *Native, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++) {
        Bytes[Index * 4 + 0] = (uint8_t)(Native[Length - 1 - Index] >> 24);
        Bytes[Index * 4 + 1] = (uint8_t)(Native[Length - 1 - Index] >> 16);
        Bytes[Index * 4 + 2] = (uint8_t)(Native[Length - 1 - Index] >>  8);
        Bytes[Index * 4 + 3] = (uint8_t)(Native[Length - 1 - Index] >>  0);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Cflint_BytesToNative4(uint8_t *Bytes, uint32_t *Native, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++) {
        Native[Index] |= (uint32_t)Bytes[(Length - 1 - Index) * 4 + 0] << 24;
        Native[Index] |= (uint32_t)Bytes[(Length - 1 - Index) * 4 + 1] << 16;
        Native[Index] |= (uint32_t)Bytes[(Length - 1 - Index) * 4 + 2] <<  8;
        Native[Index] |= (uint32_t)Bytes[(Length - 1 - Index) * 4 + 3] <<  0;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Cflint_Native8ToBytes(uint8_t *Bytes, uint64_t *Native, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++) {
        Bytes[Index * 8 + 0] = (uint8_t)(Native[Length - 1 - Index] >> 56);
        Bytes[Index * 8 + 1] = (uint8_t)(Native[Length - 1 - Index] >> 48);
        Bytes[Index * 8 + 2] = (uint8_t)(Native[Length - 1 - Index] >> 40);
        Bytes[Index * 8 + 3] = (uint8_t)(Native[Length - 1 - Index] >> 32);
        Bytes[Index * 8 + 4] = (uint8_t)(Native[Length - 1 - Index] >> 24);
        Bytes[Index * 8 + 5] = (uint8_t)(Native[Length - 1 - Index] >> 16);
        Bytes[Index * 8 + 6] = (uint8_t)(Native[Length - 1 - Index] >>  8);
        Bytes[Index * 8 + 7] = (uint8_t)(Native[Length - 1 - Index] >>  0);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Cflint_BytesToNative8(uint8_t *Bytes, uint64_t *Native, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++) {
        Native[Index] |= (uint64_t)Bytes[(Length - 1 - Index) * 8 + 0] << 56;
        Native[Index] |= (uint64_t)Bytes[(Length - 1 - Index) * 8 + 1] << 48;
        Native[Index] |= (uint64_t)Bytes[(Length - 1 - Index) * 8 + 2] << 40;
        Native[Index] |= (uint64_t)Bytes[(Length - 1 - Index) * 8 + 3] << 32;
        Native[Index] |= (uint64_t)Bytes[(Length - 1 - Index) * 8 + 4] << 24;
        Native[Index] |= (uint64_t)Bytes[(Length - 1 - Index) * 8 + 5] << 16;
        Native[Index] |= (uint64_t)Bytes[(Length - 1 - Index) * 8 + 6] <<  8;
        Native[Index] |= (uint64_t)Bytes[(Length - 1 - Index) * 8 + 7] <<  0;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 逆序操作数,操作数与NativeX互转(X==Cflint_Byte) */
void Cflint_OperandReverse(Cflint_Type *Operand, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length / 2; Index++) {
        uint32_t Index1 = Index;
        uint32_t Index2 = Length - 1 - Index;
        /* 交换俩个数 */
        Operand[Index1] ^= Operand[Index2];
        Operand[Index2] ^= Operand[Index1];
        Operand[Index1] ^= Operand[Index2];
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 操作数转化为NativeX(X==Type) */
void Cflint_OperandToNative(Cflint_Type *Operand, uint32_t Length, uint8_t Type)
{
    if (Type != 1 && Type != 2 && Type != 4 && Type != 8)
        return;
    /* 跳过无必要的转换 */
    if (Cflint_Byte == Type) {
        Cflint_Type *Native = Operand;
        Cflint_OperandReverse(Operand, Length);
        return;
    }
    /* 开始转换流程 */
    Cflint_Type *Native = Operand;
    uint32_t  Length1 = Length * Cflint_Byte / 1;
    uint32_t  Length2 = Length * Cflint_Byte / 2;
    uint32_t  Length4 = Length * Cflint_Byte / 4;
    uint32_t  Length8 = Length * Cflint_Byte / 8;
     uint8_t *Native1 = ( uint8_t *)Native;
    uint16_t *Native2 = (uint16_t *)Native;
    uint32_t *Native4 = (uint32_t *)Native;
    uint64_t *Native8 = (uint64_t *)Native;
    /* 操作数转Native */
    Cflint_OperandReverse(Operand, Length);
    /* 将Native转化为Native1 */
    for (uint32_t Index = 0; Index < Length; Index++) {
#if 0
#elif Cflint_Byte == 1
        continue;
         uint8_t Value = Native[Index];
        Native1[Index * 1 + 0] = Value >>  0;
#elif Cflint_Byte == 2
        uint16_t Value = Native[Index];
        Native1[Index * 2 + 0] = Value >>  8;
        Native1[Index * 2 + 1] = Value >>  0;
#elif Cflint_Byte == 4
        uint32_t Value = Native[Index];
        Native1[Index * 4 + 0] = Value >> 24;
        Native1[Index * 4 + 1] = Value >> 16;
        Native1[Index * 4 + 2] = Value >>  8;
        Native1[Index * 4 + 3] = Value >>  0;
#elif Cflint_Byte == 8
        uint64_t Value = Native[Index];
        Native1[Index * 8 + 0] = Value >> 56;
        Native1[Index * 8 + 1] = Value >> 48;
        Native1[Index * 8 + 2] = Value >> 40;
        Native1[Index * 8 + 3] = Value >> 32;
        Native1[Index * 8 + 4] = Value >> 24;
        Native1[Index * 8 + 5] = Value >> 16;
        Native1[Index * 8 + 6] = Value >>  8;
        Native1[Index * 8 + 7] = Value >>  0;
#else
#endif
    }
    /* 将Native1转化为目标Native */
    switch(Type) {
    case 1:
        break;
        for (uint32_t Index = 0; Index < Length1; Index++) {
             uint8_t Value = 0;
            Value |= Native1[Index * 1 + 0];
            Native1[Index] = Value;
        }
        break;
    case 2:
        for (uint32_t Index = 0; Index < Length2; Index++) {
            uint16_t Value = 0;
            Value |= Native1[Index * 2 + 0]; Value <<= 8;
            Value |= Native1[Index * 2 + 1];
            Native2[Index] = Value;
        }
        break;
    case 4:
        for (uint32_t Index = 0; Index < Length4; Index++) {
            uint32_t Value = 0;
            Value |= Native1[Index * 4 + 0]; Value <<= 8;
            Value |= Native1[Index * 4 + 1]; Value <<= 8;
            Value |= Native1[Index * 4 + 2]; Value <<= 8;
            Value |= Native1[Index * 4 + 3];
            Native4[Index] = Value;
        }
        break;
    case 8:
        for (uint32_t Index = 0; Index < Length8; Index++) {
            uint64_t Value = 0;
            Value |= Native1[Index * 8 + 0]; Value <<= 8;
            Value |= Native1[Index * 8 + 1]; Value <<= 8;
            Value |= Native1[Index * 8 + 2]; Value <<= 8;
            Value |= Native1[Index * 8 + 3]; Value <<= 8;
            Value |= Native1[Index * 8 + 4]; Value <<= 8;
            Value |= Native1[Index * 8 + 5]; Value <<= 8;
            Value |= Native1[Index * 8 + 6]; Value <<= 8;
            Value |= Native1[Index * 8 + 7];
            Native8[Index] = Value;
        }
        break;
    }
    /*  */
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* NativeX转化为操作数(X==Type) */
void Cflint_NativeToOperand(Cflint_Type *Operand, uint32_t Length, uint8_t Type)
{
    if (Type != 1 && Type != 2 && Type != 4 && Type != 8)
        return;
    /* 跳过无必要的转换 */
    if (Cflint_Byte == Type) {
        Cflint_Type *Native = Operand;
        Cflint_OperandReverse(Operand, Length);
        return;
    }
    /* 开始转换流程 */
    Cflint_Type *Native = Operand;
    uint32_t  Length1 = Length * Cflint_Byte / 1;
    uint32_t  Length2 = Length * Cflint_Byte / 2;
    uint32_t  Length4 = Length * Cflint_Byte / 4;
    uint32_t  Length8 = Length * Cflint_Byte / 8;
     uint8_t *Native1 = ( uint8_t *)Native;
    uint16_t *Native2 = (uint16_t *)Native;
    uint32_t *Native4 = (uint32_t *)Native;
    uint64_t *Native8 = (uint64_t *)Native;
    /* 将目标Native转化为Native1 */
    switch(Type) {
    case 1:
        break;
        for (uint32_t Index = 0; Index < Length1; Index++) {
             uint8_t Value = Native1[Index];
            Native1[Index * 1 + 0] = Value >>  0;
        }
        break;
    case 2:
        for (uint32_t Index = 0; Index < Length2; Index++) {
            uint16_t Value = Native2[Index];
            Native1[Index * 2 + 0] = Value >>  8;
            Native1[Index * 2 + 1] = Value >>  0;
        }
        break;
    case 4:
        for (uint32_t Index = 0; Index < Length4; Index++) {
            uint32_t Value = Native4[Index];
            Native1[Index * 4 + 0] = Value >> 24;
            Native1[Index * 4 + 1] = Value >> 16;
            Native1[Index * 4 + 2] = Value >>  8;
            Native1[Index * 4 + 3] = Value >>  0;
        }
        break;
    case 8:
        for (uint32_t Index = 0; Index < Length8; Index++) {
            uint64_t Value = Native8[Index];
            Native1[Index * 8 + 0] = Value >> 56;
            Native1[Index * 8 + 1] = Value >> 48;
            Native1[Index * 8 + 2] = Value >> 40;
            Native1[Index * 8 + 3] = Value >> 32;
            Native1[Index * 8 + 4] = Value >> 24;
            Native1[Index * 8 + 5] = Value >> 16;
            Native1[Index * 8 + 6] = Value >>  8;
            Native1[Index * 8 + 7] = Value >>  0;
        }
        break;
    }
    /* 将Native1转化为Native */
    for (uint32_t Index = 0; Index < Length; Index++) {
#if 0
#elif Cflint_Byte == 1
        continue;
         uint8_t Value = 0;
        Value |= Native1[Index * 1 + 0];
        Native[Index] = Value;
#elif Cflint_Byte == 2
        uint16_t Value = 0;
        Value |= Native1[Index * 2 + 0]; Value <<= 8;
        Value |= Native1[Index * 2 + 1];
        Native[Index] = Value;
#elif Cflint_Byte == 4
        uint32_t Value = 0;
        Value |= Native1[Index * 4 + 0]; Value <<= 8;
        Value |= Native1[Index * 4 + 1]; Value <<= 8;
        Value |= Native1[Index * 4 + 2]; Value <<= 8;
        Value |= Native1[Index * 4 + 3];
        Native[Index] = Value;
#elif Cflint_Byte == 8
        uint64_t Value = 0;
        Value |= Native1[Index * 8 + 0]; Value <<= 8;
        Value |= Native1[Index * 8 + 1]; Value <<= 8;
        Value |= Native1[Index * 8 + 2]; Value <<= 8;
        Value |= Native1[Index * 8 + 3]; Value <<= 8;
        Value |= Native1[Index * 8 + 4]; Value <<= 8;
        Value |= Native1[Index * 8 + 5]; Value <<= 8;
        Value |= Native1[Index * 8 + 6]; Value <<= 8;
        Value |= Native1[Index * 8 + 7];
        Native[Index] = Value;
#else
#endif
    }
    /* Native转操作数 */
    Cflint_OperandReverse(Operand, Length);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Cflint_NumToHex(uint8_t *Hex, uint8_t *Num, uint8_t *Temp[2], uint32_t Len)
{
    uint32_t NumLen1 = 0;
    uint32_t NumLen2 = 0;
    uint8_t *Swap = NULL;
    uint8_t *Buffer1 = Temp[0]; //被除数
    uint8_t *Buffer2 = Temp[1]; //商
    /* 清零缓冲区数据,初始化Buffer1,Buffer2 */
    for (uint32_t Index = 0; Index < Len; Index++) {
        Hex[Index] = 0;
        Buffer1[Index] = 0;
        Buffer2[Index] = 0;
    }
    /* 拷贝十进制数据到Buffer1 */
    for (uint32_t Index = 0; Index < Len; Index++) {
        /* 这里只能接受纯数字字符串 */
        if (Num[Index] == 0)
            break;
        if (Num[Index] <= '0' && Num[Index] >= '9')
            return;
        Buffer1[Index] = Num[Index] - '0';
        NumLen1 = Index + 1;
    }
    /* 进行迭代循环 Module = Buffer1 % 255 */
    uint32_t Index0 = 0;
    while (1) {
        /* 前缀0滤除锁 */
        uint32_t PrefixLock = 0;
        /* 进行迭代循环 */
        uint32_t Index1 = 0;
        uint16_t Module = 0;
        /* Buffer2 = Buffer1 / 255 */
        /* Module  = Buffer1 % 255 */
        do {
            /* 数据不够移动到下一个位 */
            if (Index1 < NumLen1)
                Module  = Module * 10 + Buffer1[Index1++];
            /* 数据不够除时,商补0 */
            /* 数据足够除时,计算商 */
            if (Module >> 8 != 0)
                PrefixLock  = 1;
            /* 前缀0滤除锁(这是必备的,它会限制商以期靠近0) */
            if (PrefixLock != 0)
                Buffer2[NumLen2++] = Module >> 8;
            Module &= 0xFF;
        } while (Index1 < NumLen1);
        /* 保存当次最后除余 */
        Hex[Index0++] = Module;
        /* 辗转迭代直到商为0 */
        if (NumLen2 == 0)
            break;
        /* 商更新成被除数(交换指针即可) */
        Swap    = Buffer1;
        Buffer1 = Buffer2;
        Buffer2 = Swap;
        NumLen1 = NumLen2;
        NumLen2 = 0;
        /* 开始下一次迭代循环 */
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Cflint_HexToNum(uint8_t *Hex, uint8_t *Num, uint8_t *Temp[3], uint32_t Len)
{
    uint32_t NumLen1 = 0;
    uint32_t NumLen2 = 0;
    uint8_t *Swap = NULL;
    uint8_t *Buffer1 = Temp[0]; //被除数
    uint8_t *Buffer2 = Temp[1]; //商
    uint8_t *Buffer3 = Temp[2]; //累增除数
    /* 清零缓冲区数据,初始化Buffer1,Buffer2 */
    for (uint32_t Index = 0; Index < Len; Index++) {
        Num[Index] = 0;
        Buffer1[Index] = 0;
        Buffer2[Index] = 0;
        Buffer3[Index] = 0;
    }
    /* 拷贝十进制数据到Buffer1 */
    for (uint32_t Index = 0; Index < Len; Index++) {
        Buffer1[Index] = Hex[Index];
        if (Hex[Index] != 0)
            NumLen1 = Index + 1;
    }
    /* 进行迭代循环 Module = Buffer1 % 10 */
    uint32_t Index0 = 0;
    while (1) {
        /* 进行迭代循环 */
        uint32_t Index1 = 0;
        uint16_t Module = 0;
        /* 退出条件 */
        if (NumLen1 == 0)
            break;
        /* 注意:这里只能使用原始的除转减(计算机不支持跨进制除法) */
        /* 生成一个累赠除数,该除数恰好小于被除数偏移1位 */
        for (uint32_t Index = 0; Index < Len; Index++)
            Buffer3[Index] = 0;
        Buffer3[0] = 10;
        /* 这个累赠除数数学表示为10 * 2 ** k */
        int64_t Offset = 0;
        while (1) {
            /* 比较Buffer1和Buffer3(必须大于才可以是true) */
            int8_t Compare = 0;
            for (int64_t Index = NumLen1 - 1; Index >= 0; Index--) {
                if (Buffer1[Index] > Buffer3[Index]) {
                    Compare = +1;
                    break;
                }
                if (Buffer1[Index] < Buffer3[Index]) {
                    Compare = -1;
                    break;
                }
            }
            if (Compare == 0)
                break;
            /* 如果Buffer1 > Buffer3则Buffer3左移一位(*2) */
            if (Compare == +1) {
                /* 如果Buffer3移位达到限制,再移位会产生截断时,强行中止 */
                if (Offset >= (NumLen1 - 1) * 8 + 4)
                    break;
                uint8_t Overflow = 0;
                for (uint32_t Index = 0; Index < NumLen1; Index++) {
                    uint8_t Value  = Buffer3[Index];
                    Buffer3[Index] = ((Value & 0x7F) << 1) | Overflow;
                    Overflow = (Value & 0x80) >> 7;
                }
                Offset++;
            }
            /* 如果Buffer1 < Buffer3则Buffer3右移一位(/2) */
            if (Compare == -1) {
                uint8_t Overflow = 0;
                for (int64_t Index = NumLen1 - 1; Index >= 0; Index--) {
                    uint8_t Value  = Buffer3[Index];
                    Buffer3[Index] = ((Value & 0xFE) >> 1) | Overflow;
                    Overflow = (Value & 0x01) << 7;
                }
                Offset--;
                break;
            }
        }
        /* 如果第一次小于比较都不通过(踩空了) */
        if (Offset == -1) {
            Num[Index0++] = Buffer1[0];
            /* Buffer1 = Buffer2 */
            for (uint32_t Index = 0; Index < Len; Index++)
                Buffer1[Index] = Buffer2[Index];
            NumLen1 = NumLen2;
            /* Buffer2 = 0 */
            for (uint32_t Index = 0; Index < Len; Index++)
                Buffer2[Index] = 0;
            NumLen2 = 0;
            continue;
        }
        /* 现在的Buffer3 <= Buffer1了 */
        /* Buffer1 -= Buffer3 */
        /* Buffer2 += Buffer3 / 10 */
        {
            uint8_t Overflow = 0;
            /* Buffer1 -= Buffer3 */
            for (uint32_t Index = 0; Index < NumLen1; Index++) {
                /* 计算差结果 */
                uint8_t Value = Buffer1[Index] - Buffer3[Index] - Overflow;
                /* 检查借位溢出 */
                if (Value > Buffer1[Index])
                    Overflow = 1;
                else
                    Overflow = 0;
                /* 保存运算结果 */
                Buffer1[Index] = Value;
            }
            /* 关键: */
            /* Buffer3 == 10  * 2 ** K(K == Offset) */
            /* Buffer3 /= 10 == 2 ** K(K == Offset) */
            for (uint32_t Index = 0; Index < Len; Index++)
                Buffer3[Index] = 0;
            Buffer3[Offset / 8] = 1 << (Offset % 8);
            Overflow = 0;
            /* Buffer2 += Buffer3 */
            for (uint32_t Index = 0; Index < Len; Index++) {
                /* 计算和结果 */
                uint8_t Value = Buffer2[Index] + Buffer3[Index] + Overflow;
                /* 检查进位溢出 */
                if (Value < Buffer2[Index] || Value < Buffer3[Index])
                    Overflow = 1;
                if (Value > Buffer2[Index] && Value > Buffer3[Index])
                    Overflow = 0;
                /* 保存运算结果 */
                Buffer2[Index] = Value;
                if (Buffer2[Index] != 0)
                    NumLen2 = Index + 1;
            }
        }
    }
    /* 逆序Num成Native模式 */
    for (uint32_t Index = 0; Index < Len; Index++)
        if (Num[Index] != 0)
            NumLen1 = Index + 1;
    for (uint32_t Index = 0; Index < NumLen1; Index++)
        Num[Index] += '0';
    
    for (uint32_t Index = 0; Index < NumLen1 / 2; Index++) {
        uint32_t Index1 = Index;
        uint32_t Index2 = NumLen1 - 1 - Index;
        /* 交换俩个数 */
        Num[Index1] ^= Num[Index2];
        Num[Index2] ^= Num[Index1];
        Num[Index1] ^= Num[Index2];
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
