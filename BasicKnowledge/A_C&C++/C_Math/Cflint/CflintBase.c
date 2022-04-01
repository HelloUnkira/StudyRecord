#include "Cflint.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 和运算 */
void Cflint_Addition(CFLINT_BYTES *Result,    CFLINT_BYTES *Overflow,  
                     CFLINT_BYTES *Operator1, CFLINT_BYTES *Operator2,
                     uint32_t Length)
{
    uint32_t Index = 0;
    CFLINT_BYTES Temp = 0;
    /* 初始化溢出记录 */
    *Overflow = 0;
    /* 计算俩数和 */
    for (Index = 0; Index < Length; Index++) {
        /* 计算和结果,暂存到临时区 */
        Temp = Operator1[Index] + Operator2[Index] + *Overflow;
        /* 检查进位溢出 */
        if (Temp < Operator1[Index] || Temp < Operator2[Index])
            *Overflow = 1;
        if (Temp > Operator1[Index] && Temp > Operator2[Index])
            *Overflow = 0;
        /* 保存运算结果 */
        Result[Index] = Temp;
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 差运算 */
void Cflint_Subtraction(CFLINT_BYTES *Result,    CFLINT_BYTES *Overflow,  
                        CFLINT_BYTES *Operator1, CFLINT_BYTES *Operator2,
                        uint32_t Length)
{
    uint32_t Index = 0;
    CFLINT_BYTES Temp = 0;
    /* 初始化溢出记录 */
    *Overflow = 0;
    /* 计算俩数和 */
    for (Index = 0; Index < Length; Index++) {
        /* 计算和结果,暂存到临时区 */
        Temp = Operator1[Index] - Operator2[Index] - *Overflow;
        /* 检查进位溢出 */
        if (Temp > Operator1[Index] && Temp > Operator2[Index])
            *Overflow = 1;
        if (Temp < Operator1[Index] || Temp < Operator2[Index])
            *Overflow = 0;
        /* 保存运算结果 */
        Result[Index] = Temp;
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 乘运算(计算位乘法器) */
static void Cflint_Multiply1(CFLINT_BYTES  Data1,  CFLINT_BYTES  Data2,
                             CFLINT_BYTES *Carry1, CFLINT_BYTES *Carry2,
                             CFLINT_BYTES *Overflow)
{
#ifdef CFLINT_BYTES2
//利用关键字优化策略
    CFLINT_BYTES2 Result_XY = 0;
    CFLINT_BYTES2 Result_01 = 0;
    //1.计算乘数结果
    Result_XY |= ((CFLINT_BYTES2)Data1);
    Result_XY *= ((CFLINT_BYTES2)Data2);
    //2.计算上一位的进位
    Result_01 |= ((CFLINT_BYTES2)(*Carry1));
    Result_01 |= ((CFLINT_BYTES2)(*Carry2) << CFLINT_BYTEBIT);
    //3.计算最终结果
    Result_01 += Result_XY;
    //4.本次计算是否高位产生溢出
    if (Result_01 <  Result_XY) (*Overflow) = 1;
    if (Result_01 >= Result_XY) (*Overflow) = 0;
    //5.保存本次计算的低位和高位
    (*Carry1) = (CFLINT_BYTES)(Result_01 >> 0);
    (*Carry2) = (CFLINT_BYTES)(Result_01 >> CFLINT_BYTEBIT);
#else
//不利用关键字优化策略
    CFLINT_BYTES Result_XY0 = 0;
    CFLINT_BYTES Result_XY1 = 0;
    //1.获得乘数的高位与低位
    CFLINT_BYTES X0 = Data1 & CFLINT_HALF;
    CFLINT_BYTES X1 = Data1 >> (CFLINT_BYTEBIT / 2);
    CFLINT_BYTES Y0 = Data2 & CFLINT_HALF;
    CFLINT_BYTES Y1 = Data2 >> (CFLINT_BYTEBIT / 2);
    //2.分别计算高位与低位的交乘
    CFLINT_BYTES I0 = X0 * Y0;
    CFLINT_BYTES I1 = X0 * Y1;
    CFLINT_BYTES I2 = X1 * Y0;
    CFLINT_BYTES I3 = X1 * Y1;
    //3.计算溢出
    CFLINT_BYTES Overflow0 = I0 >> (CFLINT_BYTEBIT / 2); //低位的进位
    CFLINT_BYTES Result_0  = I1 + I2 + Overflow0;        //低位的值
    CFLINT_BYTES Result_1  = I3;                         //高位的值
    if (Result_0 < I1)                                   //低位溢出
        Result_1 += ((CFLINT_BYTES)1 << (CFLINT_BYTEBIT / 2));
    //4.计算终点值
    Result_XY0 |= (I0 & CFLINT_HALF);
    Result_XY0 |= (Result_0 << (CFLINT_BYTEBIT / 2));
    Result_XY1 |= (Result_1);
    Result_XY1 += (Result_0 >> (CFLINT_BYTEBIT / 2));
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
static void Cflint_Square1(CFLINT_BYTES  Data1,  CFLINT_BYTES  Data2,
                           CFLINT_BYTES *Carry1, CFLINT_BYTES *Carry2,
                           CFLINT_BYTES *Overflow)
{
#ifdef CFLINT_BYTES2
//利用关键字优化策略
    CFLINT_BYTES2 Result_XY = 0;
    CFLINT_BYTES2 Result_01 = 0;
    //1.计算乘数结果
    Result_XY |= ((CFLINT_BYTES2)Data1);
    Result_XY *= ((CFLINT_BYTES2)Data2);
    //2.计算上一位的进位
    Result_01 |= ((CFLINT_BYTES2)(*Carry1));
    Result_01 |= ((CFLINT_BYTES2)(*Carry2) << CFLINT_BYTEBIT);
    //3.计算进位
    (*Overflow) += (Result_XY >> (CFLINT_BYTEBIT * 2 - 1));
    //4.计算最终结果
    Result_XY *= 2;
    Result_01 += Result_XY;
    //4.本次计算是否高位产生溢出
    if (Result_01 <  Result_XY) (*Overflow) += 1;
    if (Result_01 >= Result_XY) (*Overflow) += 0;
    //5.保存本次计算的低位和高位
    (*Carry1) = (CFLINT_BYTES)(Result_01 >> 0);
    (*Carry2) = (CFLINT_BYTES)(Result_01 >> CFLINT_BYTEBIT);
#else
//不利用关键字优化策略
    CFLINT_BYTES Result_XY0 = 0;
    CFLINT_BYTES Result_XY1 = 0;
    //1.获得乘数的高位与低位
    CFLINT_BYTES X0 = Data1 & CFLINT_HALF;
    CFLINT_BYTES X1 = Data1 >> (CFLINT_BYTEBIT / 2);
    CFLINT_BYTES Y0 = Data2 & CFLINT_HALF;
    CFLINT_BYTES Y1 = Data2 >> (CFLINT_BYTEBIT / 2);
    //2.分别计算高位与低位的交乘
    CFLINT_BYTES I0 = X0 * Y0;
    CFLINT_BYTES I1 = X0 * Y1;
    CFLINT_BYTES I2 = X1 * Y0;
    CFLINT_BYTES I3 = X1 * Y1;
    //3.计算溢出
    CFLINT_BYTES Overflow0 = I0 >> (CFLINT_BYTEBIT / 2); //低位的进位
    CFLINT_BYTES Result_0  = I1 + I2 + Overflow0;        //低位的值
    CFLINT_BYTES Result_1  = I3;                         //高位的值
    if (Result_0 < I1)                                   //低位溢出
        Result_1 += ((CFLINT_BYTES)1 << (CFLINT_BYTEBIT / 2));
    //4.计算终点值
    Result_XY0 |= (I0 & CFLINT_HALF);
    Result_XY0 |= (Result_0 << (CFLINT_BYTEBIT / 2));
    Result_XY1 |= (Result_1);
    Result_XY1 += (Result_0 >> (CFLINT_BYTEBIT / 2));
    //5.计算进位
    (*Overflow) += (Result_XY1 >> (CFLINT_BYTEBIT - 1));
    //5.保存本次计算的低位和高位
    Result_XY1 <<= 1;
    Result_XY1  |= (Result_XY0 >> (CFLINT_BYTEBIT - 1));
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
void Cflint_Multiply(CFLINT_BYTES *Result,    uint32_t Length,
                     CFLINT_BYTES *Operator1, CFLINT_BYTES *Operator2)
{
    CFLINT_BYTES Carry1 = 0;
    CFLINT_BYTES Carry2 = 0;
    CFLINT_BYTES Overflow = 0;
    uint32_t I = 0;
    uint32_t K = 0;

    for (K = 0; K < Length; K++) {
        for (I = 0; I <= K; I++)
            /* 使用位乘法器 */
            Cflint_Multiply1(Operator1[I], Operator2[K - I],
                             &Carry1, &Carry2, &Overflow);
        /* 结果保留 */
        Result[K]   = Carry1;   //保留低位
        Carry1      = Carry2;   //高位变低位
        Carry2      = Overflow; //进位变高位
        Overflow    = 0;        //进位清0
    }
    for (K = Length; K < Length * 2 - 1; K++) {
        for (I = (K + 1) - Length; I < Length; I++)
            /* 使用位乘法器 */
            Cflint_Multiply1(Operator1[I], Operator2[K - I],
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
void Cflint_Square(CFLINT_BYTES *Result, uint32_t Length,
                   CFLINT_BYTES *Operator)
{
    CFLINT_BYTES Carry1 = 0;
    CFLINT_BYTES Carry2 = 0;
    CFLINT_BYTES Overflow = 0;
    
    uint32_t Min = 0;
    uint32_t I = 0;
    uint32_t K = 0;

    for (K = 0; K < Length * 2 - 1; K++) {
        Min = (K < Length ? 0 : ((K + 1) - Length));
        for (I = Min; I <= K && I <= K - I; I++) {
            /* 使用位乘法器 */
            if (I <  K - I)
                Cflint_Square1(Operator[I], Operator[K - I],
                               &Carry1, &Carry2, &Overflow);
            if (I >= K - I)
                Cflint_Multiply1(Operator[I], Operator[K - I],
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
void Cflint_ShiftLeft(CFLINT_BYTES *Result, uint32_t Length, uint32_t Bits2)
{
    //一个DIGIT_TYPE字节下进制的位有多少个2进制位
    int32_t Bits_N = Bits2 / CFLINT_BYTEBIT;
    int32_t Bits_2 = Bits2 % CFLINT_BYTEBIT;
    int32_t Last_2 = CFLINT_BYTEBIT - Bits_2;
    CFLINT_BYTES Bit_N1 = 0, Bit_High = 0;
    CFLINT_BYTES Bit_N2 = 0, Bit_Low  = 0;

    int32_t I = 0;

    for (I = (int32_t)Length - 1; I - Bits_N - 1 >= 0; I--) {
        Bit_N1    = Result[I - Bits_N];
        Bit_N2    = Result[I - Bits_N - 1];
        Bit_High  = (Bit_N1 << Bits_2);
        Bit_Low   = (Bit_N2 >> Last_2);
        Bit_Low   = (Last_2 >= CFLINT_BYTEBIT) ? 0 : Bit_Low;
        Result[I] = Bit_High | Bit_Low;
    }
    Bit_N1 = Result[I - Bits_N];
    //Bit_N2 = Result[I - Bits_N - 1];
    Bit_High  = (Bit_N1 << Bits_2);
    //Bit_Low   = (Bit_N2 >> Last_2);
    //Bit_Low   = (Last_2 >= CFLINT_BYTEBIT) ? 0 : Bit_Low;
    Result[I] = Bit_High;// | (Bit_N2 >> Last_2);

    for (I = 0; I - Bits_N < 0; I++)
        Result[I] = 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 右移位运算 */
void Cflint_ShiftRight(CFLINT_BYTES *Result, uint32_t Length, uint32_t Bits2)
{
    //一个DIGIT_TYPE字节下进制的位有多少个2进制位
    int32_t Bits_N = Bits2 / CFLINT_BYTEBIT;
    int32_t Bits_2 = Bits2 % CFLINT_BYTEBIT;
    int32_t Last_2 = CFLINT_BYTEBIT - Bits_2;
    CFLINT_BYTES Bit_N1 = 0, Bit_Low  = 0;
    CFLINT_BYTES Bit_N2 = 0, Bit_High = 0;

    int32_t I = 0;

    for (I = 0; I + Bits_N + 1 < (int32_t)Length; I++) {
        Bit_N1    = Result[I + Bits_N];
        Bit_N2    = Result[I + Bits_N + 1];
        Bit_Low   = (Bit_N1 >> Bits_2);
        Bit_High  = (Bit_N2 << Last_2);
        Bit_High  = (Last_2 >= CFLINT_BYTEBIT) ? 0 : Bit_High;
        Result[I] = Bit_Low | Bit_High;
    }
    Bit_N1 = Result[I + Bits_N];
    //Bit_N2 = Result[I + Bits_N + 1];
    Bit_Low = (Bit_N1 >> Bits_2);
    //Bit_High = (Bit_N2 << Last_2);
    //Bit_High = (Last_2 >= CFLINT_BYTEBIT) ? 0 : Bit_High;
    Result[I] = Bit_Low;// | (Bit_N2 << Last_2);

    for (I = (int32_t)Length - Bits_N; I < (int32_t)Length; I++)
        Result[I] = 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
