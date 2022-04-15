#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_AUXILIARY
/* 交换:Operand0 <=> Operand1 */
void Cflint_Swap(CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1, uint32_t Length)
{
    CFLINT_TYPE Temp0 = 0, Temp1 = 0;
    for (uint32_t Index = 0; Index < Length; Index++) {
        Temp0 = Operand0[Index]; Temp1 = Operand1[Index];
        Operand0[Index] = Temp1; Operand1[Index] = Temp0;
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 拷贝:Operand0 = Operand1 */
void Cflint_Copy(CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Operand0[Index] = Operand1[Index];
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 比较:((>:1);(==:0);(<:-1)) */
int8_t Cflint_Compare(CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1, uint32_t Length)
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
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 相等判断:((==:1);(!=:0)) */
bool Cflint_Equal(CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1, uint32_t Length)
{
    CFLINT_TYPE Result = 0;
    for (uint32_t Index = 0; Index < Length; Index++)
        Result |= Operand0[Index] ^ Operand1[Index];
    return (Result == 0 ? true : false);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 为0判断:Operand == 0 */
bool Cflint_IsZero(CFLINT_TYPE *Operand, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        if (Operand[Index] != 0)
            return false;
    return true;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 设值:类似memset */
void Cflint_SetValue(CFLINT_TYPE *Operand, uint32_t Length,  CFLINT_TYPE  Value)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Operand[Index] = Value;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 检查一个数是否为2的幂次方 */
bool Cflint_IsExponent2(CFLINT_TYPE *Operand, uint32_t Length)
{
    bool NumbersOnlyOne = false;
    for (uint32_t Index = 0; Index < Length; Index++)
    for (uint32_t Bit2 = 0; Bit2 < CFLINT_BITS; Bit2++)
        if ((Operand[Index] & (1 << Bit2)) != 0) {
            if (NumbersOnlyOne == false) {
                NumbersOnlyOne  = true;
                continue;
            }
            if (NumbersOnlyOne == true) {
                NumbersOnlyOne  = false;
                break;
            }
        }
    return NumbersOnlyOne;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 计算2进制最高位索引 */
int64_t Cflint_Numbers2(CFLINT_TYPE *Operand, uint32_t Length)
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
    for (Index2 = (int64_t)CFLINT_BITS - 1; Index2 >= 0; Index2--)
        if ((Operand[Index1] & (1 << Index2)) != 0)
            break;
    /* 计算最高位 */
    return (Index1 * CFLINT_BITS + Index2);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 位与运算 */
void Cflint_AND(CFLINT_TYPE *Result, CFLINT_TYPE *Operand1, 
                CFLINT_TYPE *Operand2,  uint32_t  Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Result[Index] = Operand1[Index] & Operand2[Index];
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 位或运算 */
void Cflint_OR(CFLINT_TYPE *Result, CFLINT_TYPE *Operand1, 
               CFLINT_TYPE *Operand2,  uint32_t  Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Result[Index] = Operand1[Index] | Operand2[Index];
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 位异或运算 */
void Cflint_XOR(CFLINT_TYPE *Result, CFLINT_TYPE *Operand1, 
                CFLINT_TYPE *Operand2,  uint32_t  Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Result[Index] = Operand1[Index] ^ Operand2[Index];
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 位反运算 */
void Cflint_NOT(CFLINT_TYPE *Operand, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Operand[Index] = ~Operand[Index];
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 位检查 */
bool Cflint_CheckBit2(CFLINT_TYPE *Operand, uint32_t Length, int64_t Bits2)
{
    int64_t Bits_N = Bits2 / CFLINT_BITS;
    int64_t Bits_2 = Bits2 % CFLINT_BITS;
    
    if (Bits_N < Length)
        return ((Operand[Bits_N] & (1 << Bits_2)) != 0);
    return false;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 位设置 */
void Cflint_SetBit2(CFLINT_TYPE *Operand, uint32_t Length, int64_t Bits2)
{
    int64_t Bits_N = Bits2 / CFLINT_BITS;
    int64_t Bits_2 = Bits2 % CFLINT_BITS;
    
    if (Bits_N < Length)
        Operand[Bits_N] |= (1 << Bits_2);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 位清除 */
void Cflint_ClearBit2(CFLINT_TYPE *Operand, uint32_t Length, int64_t Bits2)
{
    int64_t Bits_N = Bits2 / CFLINT_BITS;
    int64_t Bits_2 = Bits2 % CFLINT_BITS;
    
    if (Bits_N < Length)
        Operand[Bits_N] &= ~(1 << Bits_2);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
