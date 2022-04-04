#include "Cflint.h"

/* 备注:实现来源于uECC.c.h */

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 和运算 */
CFLINT_TYPE Cflint_Addition(CFLINT_TYPE *Result,    CFLINT_TYPE *Operand1, 
                            CFLINT_TYPE *Operand2, uint32_t Length)
{
    CFLINT_TYPE Overflow = 0;
    /* 计算俩数和 */
    for (uint32_t Index = 0; Index < Length; Index++) {
        /* 计算和结果,暂存到临时区 */
        CFLINT_TYPE Temp = Operand1[Index] + Operand2[Index] + Overflow;
        /* 检查进位溢出 */
        if (Temp < Operand1[Index] || Temp < Operand2[Index])
            Overflow = 1;
        if (Temp > Operand1[Index] && Temp > Operand2[Index])
            Overflow = 0;
        /* 保存运算结果 */
        Result[Index] = Temp;
    }
    return Overflow;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 差运算 */
CFLINT_TYPE Cflint_Subtraction(CFLINT_TYPE *Result,    CFLINT_TYPE *Operand1,
                               CFLINT_TYPE *Operand2, uint32_t Length)
{
    CFLINT_TYPE Overflow = 0;
    /* 计算俩数差 */
    for (uint32_t Index = 0; Index < Length; Index++) {
        /* 计算和结果,暂存到临时区 */
        CFLINT_TYPE Temp = Operand1[Index] - Operand2[Index] - Overflow;
        /* 检查借位溢出 */
        if (Temp > Operand1[Index] && Temp > Operand2[Index])
            Overflow = 1;
        if (Temp < Operand1[Index] || Temp < Operand2[Index])
            Overflow = 0;
        /* 保存运算结果 */
        Result[Index] = Temp;
    }
    return Overflow;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 乘运算(计算位乘法器) */
static void Cflint_Multiply1(CFLINT_TYPE  Data1,  CFLINT_TYPE  Data2,
                             CFLINT_TYPE *Carry1, CFLINT_TYPE *Carry2,
                             CFLINT_TYPE *Overflow)
{
#ifdef CFLINT_TYPE_2
//利用关键字优化策略
    CFLINT_TYPE_2 Result_XY = 0;
    CFLINT_TYPE_2 Result_01 = 0;
    //1.计算乘数结果
    Result_XY |= ((CFLINT_TYPE_2)Data1);
    Result_XY *= ((CFLINT_TYPE_2)Data2);
    //2.计算上一位的进位
    Result_01 |= ((CFLINT_TYPE_2)(*Carry1));
    Result_01 |= ((CFLINT_TYPE_2)(*Carry2) << CFLINT_BITS);
    //3.计算最终结果
    Result_01 += Result_XY;
    //4.本次计算是否高位产生溢出
    if (Result_01 <  Result_XY) (*Overflow) = 1;
    if (Result_01 >= Result_XY) (*Overflow) = 0;
    //5.保存本次计算的低位和高位
    (*Carry1) = (CFLINT_TYPE)(Result_01 >> 0);
    (*Carry2) = (CFLINT_TYPE)(Result_01 >> CFLINT_BITS);
#else
//不利用关键字优化策略
    CFLINT_TYPE Result_XY0 = 0;
    CFLINT_TYPE Result_XY1 = 0;
    //1.获得乘数的高位与低位
    CFLINT_TYPE X0 = Data1 & CFLINT_BYTEHALFLOW;
    CFLINT_TYPE X1 = Data1 >> (CFLINT_BITS / 2);
    CFLINT_TYPE Y0 = Data2 & CFLINT_BYTEHALFLOW;
    CFLINT_TYPE Y1 = Data2 >> (CFLINT_BITS / 2);
    //2.分别计算高位与低位的交乘
    CFLINT_TYPE I0 = X0 * Y0;
    CFLINT_TYPE I1 = X0 * Y1;
    CFLINT_TYPE I2 = X1 * Y0;
    CFLINT_TYPE I3 = X1 * Y1;
    //3.计算溢出
    CFLINT_TYPE Overflow0 = I0 >> (CFLINT_BITS / 2); //低位的进位
    CFLINT_TYPE Result_0  = I1 + I2 + Overflow0;        //低位的值
    CFLINT_TYPE Result_1  = I3;                         //高位的值
    if (Result_0 < I1)                                   //低位溢出
        Result_1 += ((CFLINT_TYPE)1 << (CFLINT_BITS / 2));
    //4.计算终点值
    Result_XY0 |= (I0 & CFLINT_BYTEHALFLOW);
    Result_XY0 |= (Result_0 << (CFLINT_BITS / 2));
    Result_XY1 |= (Result_1);
    Result_XY1 += (Result_0 >> (CFLINT_BITS / 2));
    //5.保存本次计算的低位和高位
    (*Carry1) += Result_XY0;
    (*Carry2) += Result_XY1;
    //6.本次计算是否低位产生溢出
    if ((*Carry1) < Result_XY0)
        (*Carry2) += 1;
    //7.本次计算是否高位产生溢出
    if (((*Carry2)  < Result_XY1) ||
        ((*Carry2) == Result_XY1 && (*Carry1) < Result_XY0))
         (*Overflow) += 1;
#endif
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 方运算(计算位乘方器) */
static void Cflint_Square1(CFLINT_TYPE  Data1,  CFLINT_TYPE  Data2,
                           CFLINT_TYPE *Carry1, CFLINT_TYPE *Carry2,
                           CFLINT_TYPE *Overflow)
{
#ifdef CFLINT_TYPE_2
//利用关键字优化策略
    CFLINT_TYPE_2 Result_XY = 0;
    CFLINT_TYPE_2 Result_01 = 0;
    //1.计算乘数结果
    Result_XY |= ((CFLINT_TYPE_2)Data1);
    Result_XY *= ((CFLINT_TYPE_2)Data2);
    //2.计算上一位的进位
    Result_01 |= ((CFLINT_TYPE_2)(*Carry1));
    Result_01 |= ((CFLINT_TYPE_2)(*Carry2) << CFLINT_BITS);
    //3.计算进位
    (*Overflow) += (Result_XY >> (CFLINT_BITS * 2 - 1));
    //4.计算最终结果
    Result_XY *= 2;
    Result_01 += Result_XY;
    //4.本次计算是否高位产生溢出
    if (Result_01 <  Result_XY) (*Overflow) += 1;
    if (Result_01 >= Result_XY) (*Overflow) += 0;
    //5.保存本次计算的低位和高位
    (*Carry1) = (CFLINT_TYPE)(Result_01 >> 0);
    (*Carry2) = (CFLINT_TYPE)(Result_01 >> CFLINT_BITS);
#else
//不利用关键字优化策略
    CFLINT_TYPE Result_XY0 = 0;
    CFLINT_TYPE Result_XY1 = 0;
    //1.获得乘数的高位与低位
    CFLINT_TYPE X0 = Data1 & CFLINT_BYTEHALFLOW;
    CFLINT_TYPE X1 = Data1 >> (CFLINT_BITS / 2);
    CFLINT_TYPE Y0 = Data2 & CFLINT_BYTEHALFLOW;
    CFLINT_TYPE Y1 = Data2 >> (CFLINT_BITS / 2);
    //2.分别计算高位与低位的交乘
    CFLINT_TYPE I0 = X0 * Y0;
    CFLINT_TYPE I1 = X0 * Y1;
    CFLINT_TYPE I2 = X1 * Y0;
    CFLINT_TYPE I3 = X1 * Y1;
    //3.计算溢出
    CFLINT_TYPE Overflow0 = I0 >> (CFLINT_BITS / 2); //低位的进位
    CFLINT_TYPE Result_0  = I1 + I2 + Overflow0;        //低位的值
    CFLINT_TYPE Result_1  = I3;                         //高位的值
    if (Result_0 < I1)                                   //低位溢出
        Result_1 += ((CFLINT_TYPE)1 << (CFLINT_BITS / 2));
    //4.计算终点值
    Result_XY0 |= (I0 & CFLINT_BYTEHALFLOW);
    Result_XY0 |= (Result_0 << (CFLINT_BITS / 2));
    Result_XY1 |= (Result_1);
    Result_XY1 += (Result_0 >> (CFLINT_BITS / 2));
    //5.计算进位
    (*Overflow) += (Result_XY1 >> (CFLINT_BITS - 1));
    //5.保存本次计算的低位和高位
    Result_XY1 <<= 1;
    Result_XY1  |= (Result_XY0 >> (CFLINT_BITS - 1));
    Result_XY0 <<= 1;

    (*Carry1) += Result_XY0;
    (*Carry2) += Result_XY1;
    //6.本次计算是否低位产生溢出
    if ((*Carry1) < Result_XY0)
        (*Carry2) += 1;
    //7.本次计算是否高位产生溢出
    if (((*Carry2)  < Result_XY1) ||
        ((*Carry2) == Result_XY1 && (*Carry1) < Result_XY0))
         (*Overflow) += 1;
#endif
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 乘运算 */
void Cflint_Multiply(CFLINT_TYPE *Result,   CFLINT_TYPE *Operand1,
                     CFLINT_TYPE *Operand2, uint32_t Length)
{
    CFLINT_TYPE Carry1 = 0;
    CFLINT_TYPE Carry2 = 0;
    CFLINT_TYPE Overflow = 0;

    for (uint32_t K = 0; K < Length; K++) {
        for (uint32_t I = 0; I <= K; I++)
            /* 使用位乘法器 */
            Cflint_Multiply1(Operand1[I], Operand2[K - I],
                             &Carry1, &Carry2, &Overflow);
        /* 结果保留 */
        Result[K]   = Carry1;   //保留低位
        Carry1      = Carry2;   //高位变低位
        Carry2      = Overflow; //进位变高位
        Overflow    = 0;        //进位清0
    }
    for (uint32_t K = Length; K < Length * 2 - 1; K++) {
        for (uint32_t I = (K + 1) - Length; I < Length; I++)
            /* 使用位乘法器 */
            Cflint_Multiply1(Operand1[I], Operand2[K - I],
                             &Carry1, &Carry2, &Overflow);
        /* 结果保留 */
        Result[K]   = Carry1;   //保留低位
        Carry1      = Carry2;   //高位变低位
        Carry2      = Overflow; //进位变高位
        Overflow    = 0;        //进位清0
    }

    Result[Length * 2 - 1] = Carry1;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 方运算 */
void Cflint_Square(CFLINT_TYPE *Result, CFLINT_TYPE *Operand, uint32_t Length)
{
    CFLINT_TYPE Carry1 = 0;
    CFLINT_TYPE Carry2 = 0;
    CFLINT_TYPE Overflow = 0;

    for (uint32_t K = 0; K < Length * 2 - 1; K++) {
         uint32_t Min = (K < Length ? 0 : ((K + 1) - Length));
        for (uint32_t I = Min; I <= K && I <= K - I; I++) {
            /* 使用位乘法器 */
            if (I <  K - I)
                Cflint_Square1(Operand[I], Operand[K - I],
                               &Carry1, &Carry2, &Overflow);
            if (I >= K - I)
                Cflint_Multiply1(Operand[I], Operand[K - I],
                                 &Carry1, &Carry2, &Overflow);
        }
        /* 结果保留 */
        Result[K]   = Carry1;   //保留低位
        Carry1      = Carry2;   //高位变低位
        Carry2      = Overflow; //进位变高位
        Overflow    = 0;        //进位清0
    }

    Result[Length * 2 - 1] = Carry1;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 左移位运算 */
void Cflint_ShiftLeft(CFLINT_TYPE *Operand, uint32_t Length, uint64_t Bits2)
{
    //一个DIGIT_TYPE字节下进制的位有多少个2进制位
    int64_t Bits_N = Bits2 / CFLINT_BITS;
    int64_t Bits_2 = Bits2 % CFLINT_BITS;
    int64_t Last_2 = CFLINT_BITS - Bits_2;
    CFLINT_TYPE Bit_N1 = 0, Bit_High = 0;
    CFLINT_TYPE Bit_N2 = 0, Bit_Low  = 0;

    int64_t Index = 0;

    for (Index = (int64_t)Length - 1; Index - Bits_N - 1 >= 0; Index--) {
        Bit_N1    = Operand[Index - Bits_N];
        Bit_N2    = Operand[Index - Bits_N - 1];
        Bit_High  = (Bit_N1 << Bits_2);
        Bit_Low   = (Bit_N2 >> Last_2);
        Bit_Low   = (Last_2 >= CFLINT_BITS) ? 0 : Bit_Low;
        Operand[Index] = Bit_High | Bit_Low;
    }
    Bit_N1 = Operand[Index - Bits_N];
    //Bit_N2 = Operand[Index - Bits_N - 1];
    Bit_High  = (Bit_N1 << Bits_2);
    //Bit_Low   = (Bit_N2 >> Last_2);
    //Bit_Low   = (Last_2 >= CFLINT_BITS) ? 0 : Bit_Low;
    Operand[Index] = Bit_High;// | (Bit_N2 >> Last_2);

    for (Index = 0; Index - Bits_N < 0; Index++)
        Operand[Index] = 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 右移位运算 */
void Cflint_ShiftRight(CFLINT_TYPE *Operand, uint32_t Length, uint64_t Bits2)
{
    //一个DIGIT_TYPE字节下进制的位有多少个2进制位
    int64_t Bits_N = Bits2 / CFLINT_BITS;
    int64_t Bits_2 = Bits2 % CFLINT_BITS;
    int64_t Last_2 = CFLINT_BITS - Bits_2;
    CFLINT_TYPE Bit_N1 = 0, Bit_Low  = 0;
    CFLINT_TYPE Bit_N2 = 0, Bit_High = 0;

    int64_t Index = 0;

    for (Index = 0; Index + Bits_N + 1 < (int64_t)Length; Index++) {
        Bit_N1    = Operand[Index + Bits_N];
        Bit_N2    = Operand[Index + Bits_N + 1];
        Bit_Low   = (Bit_N1 >> Bits_2);
        Bit_High  = (Bit_N2 << Last_2);
        Bit_High  = (Last_2 >= CFLINT_BITS) ? 0 : Bit_High;
        Operand[Index] = Bit_Low | Bit_High;
    }
    Bit_N1 = Operand[Index + Bits_N];
    //Bit_N2 = Operand[Index + Bits_N + 1];
    Bit_Low = (Bit_N1 >> Bits_2);
    //Bit_High = (Bit_N2 << Last_2);
    //Bit_High = (Last_2 >= CFLINT_BITS) ? 0 : Bit_High;
    Operand[Index] = Bit_Low;// | (Bit_N2 << Last_2);

    for (Index = (int64_t)Length - Bits_N; Index < (int64_t)Length; Index++)
        Operand[Index] = 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
