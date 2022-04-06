#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_BASE
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*备注:和差乘方运算实现来源于uECC.c.h */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 和运算:数加一个位 */
CFLINT_TYPE Cflint_AdditionBit(CFLINT_TYPE *Operand, uint32_t Length,
                               CFLINT_TYPE  Overflow)
{
    /* 计算数累加一个位 */
    for (uint32_t Index = 0; Index < Length; Index++) {
        CFLINT_TYPE Temp = Operand[Index] + Overflow;
        /* 检查进位溢出 */
        if (Temp < Operand[Index])
            Overflow = 1;
        if (Temp > Operand[Index])
            Overflow = 0;
        /* 保存运算结果 */
        Operand[Index] = Temp;
        /* 不存在进位溢出时即可退出 */
        if (Overflow == 0)
            return 0;
    }
    return 1;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 差运算:数减一个位 */
CFLINT_TYPE Cflint_SubtractionBit(CFLINT_TYPE *Operand, uint32_t Length,
                                  CFLINT_TYPE  Overflow)
{
    /* 计算数累加一个位 */
    for (uint32_t Index = 0; Index < Length; Index++) {
        CFLINT_TYPE Temp = Operand[Index] - Overflow;
        /* 检查借位溢出 */
        if (Temp > Operand[Index])
            Overflow = 1;
        if (Temp < Operand[Index])
            Overflow = 0;
        /* 保存运算结果 */
        Operand[Index] = Temp;
        /* 不存在借位溢出时即可退出 */
        if (Overflow == 0)
            return 0;
    }
    return 1;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 翻转数:模对称指定位翻转数 */
CFLINT_TYPE Cflint_Reserve(CFLINT_TYPE *Operand, uint32_t Length)
{
    CFLINT_TYPE Overflow = 0;
    /* 计算数累加一个位 */
    for (uint32_t Index = 0; Index < Length; Index++) {
        CFLINT_TYPE Temp = 0 - Operand[Index] - Overflow;
        /* 检查借位溢出 */
        if (Temp > Operand[Index])
            Overflow = 1;
        if (Temp < Operand[Index])
            Overflow = 0;
        /* 保存运算结果 */
        Operand[Index] = Temp;
    }
    return 1;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 和运算 */
CFLINT_TYPE Cflint_Addition(CFLINT_TYPE *Result, CFLINT_TYPE *Operand1, 
                            CFLINT_TYPE *Operand2,  uint32_t  Length)
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
CFLINT_TYPE Cflint_Subtraction(CFLINT_TYPE *Result, CFLINT_TYPE *Operand1,
                               CFLINT_TYPE *Operand2,  uint32_t  Length)
{
    CFLINT_TYPE Overflow = 0;
    /* 计算俩数差 */
    for (uint32_t Index = 0; Index < Length; Index++) {
        /* 计算和结果,暂存到临时区 */
        CFLINT_TYPE Temp = Operand1[Index] - Operand2[Index] - Overflow;
        /* 检查借位溢出 */
        if (Temp > Operand1[Index])
            Overflow = 1;
        else
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
void Cflint_Multiply(CFLINT_TYPE *Result, CFLINT_TYPE *Operand1,
                     CFLINT_TYPE *Operand2,  uint32_t  Length)
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
void Cflint_ShiftLeft2(CFLINT_TYPE *Operand, uint32_t Length, uint64_t Bits2)
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
void Cflint_ShiftRight2(CFLINT_TYPE *Operand, uint32_t Length, uint64_t Bits2)
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
/* 左移位运算 */
void Cflint_ShiftLeftN(CFLINT_TYPE *Operand, uint32_t Length, uint32_t BitsN)
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
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 右移位运算 */
void Cflint_ShiftRightN(CFLINT_TYPE *Operand, uint32_t Length, uint32_t BitsN)
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
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 带余除运算 */
void Cflint_Devide(CFLINT_TYPE *Quotient, CFLINT_TYPE *Module,
                   CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                      uint32_t  Length)
{
    /* 初始化时模为被除数,商为0 */
    Cflint_Copy(Module, Operand0, Length);
    Cflint_SetValue(Quotient, Length, 0);
    CFLINT_TYPE *Operand = Operand1;
    /* 特例:除数为0检查 */
    if (Cflint_IsZero(Operand, Length) == true)
        return;
    /* 除数移位记录 */
    int64_t MovedBits2 = 0;
    /* 主迭代运行 */
    do {
        /* 比较被除数和除数 */
        int8_t CompareResult = Cflint_Compare(Module, Operand, Length);
        /* 被除数等于除数,恰好被整除时 */
        if (CompareResult == 0) {
            if (MovedBits2 != 0) {
                /* 商累加1个单元 */
                Cflint_AdditionBit(Quotient, Length, 1);
                /* 模为0,商进所有位,除数回退所有位 */
                Cflint_SetValue(Module, Length, 0);
                Cflint_ShiftLeft2(Quotient, Length, MovedBits2);
                Cflint_ShiftRight2(Operand, Length, MovedBits2);
            }
            return;
        }
        /* 被除数小于除数 */
        if (CompareResult == -1) {
            if (MovedBits2 != 0) {
                int64_t Numbers0 = Cflint_Numbers2(Module, Length);
                int64_t Numbers1 = Cflint_Numbers2(Operand, Length);
                /* 在同一水位线时,,商进1个位,除数回退1个位 */
                if (Numbers0 == Numbers1) {
                    Cflint_ShiftLeft2(Quotient, Length, 1);
                    Cflint_ShiftRight2(Operand, Length, 1);
                    MovedBits2 -= 1;
                }
                /* 不在同一水位线时,缺少退位时,商进N个位,除数回退N个位 */
                if (Numbers0 != Numbers1 && Numbers1 - Numbers0 > MovedBits2) {
                    Cflint_ShiftLeft2(Quotient, Length, MovedBits2);
                    Cflint_ShiftRight2(Operand, Length, MovedBits2);
                    MovedBits2 = 0;
                }
                /* 不在同一水位线时,冗余退位时,商进N个位,除数回退N个位 */
                if (Numbers0 != Numbers1 && Numbers1 - Numbers0 <= MovedBits2) {
                    Cflint_ShiftLeft2(Quotient, Length, Numbers1 - Numbers0);
                    Cflint_ShiftRight2(Operand, Length, Numbers1 - Numbers0);
                    MovedBits2 -= Numbers1 - Numbers0;
                }
                continue;
            }
            return;
        }
        /* 被除数大于除数 */
        if (CompareResult == 1) {
            /* 计算被除数和除数有效2进制位数 */
            int64_t Numbers0 = Cflint_Numbers2(Module, Length);
            int64_t Numbers1 = Cflint_Numbers2(Operand, Length);
            /* 如果它们未处在同一级别,除数放大且小于被除数 */
            if (Numbers0 > Numbers1 + 1) {
                Cflint_ShiftLeft2(Operand, Length, Numbers0 - Numbers1);
                MovedBits2 += Numbers0 - Numbers1;
                continue;
            }
            /* 现在被除数和除数至少处于同一级别, 执行约减 */
            Cflint_Subtraction(Module, Module, Operand, Length);
            /* 商累加1个单元 */
            Cflint_AdditionBit(Quotient, Length, 1);
            continue;
        }
    } while (1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 基础模运算 */
void Cflint_Modulo(CFLINT_TYPE *Module, CFLINT_TYPE *Operand0,
                   CFLINT_TYPE *Operand1,  uint32_t  Length)
{
    /* 初始化时模为被除数,商为0 */
    if (Module != Operand0)
        Cflint_Copy(Module, Operand0, Length);
    CFLINT_TYPE *Operand = Operand1;
    /* 特例:除数为0检查 */
    if (Cflint_IsZero(Operand, Length) == true)
        return;
    /* 除数移位记录 */
    int64_t MovedBits2 = 0;
    /* 主迭代运行 */
    do {
        /* 比较被除数和除数 */
        int8_t CompareResult = Cflint_Compare(Module, Operand, Length);
        /* 被除数等于除数,恰好被整除时 */
        if (CompareResult == 0) {
            Cflint_SetValue(Module, Length, 0);
            return;
        }
        /* 被除数小于除数 */
        if (CompareResult == -1) {
            if (MovedBits2 != 0) {
                int64_t Numbers0 = Cflint_Numbers2(Module, Length);
                int64_t Numbers1 = Cflint_Numbers2(Operand, Length);
                /* 在同一水位线时,后退一个单元 */
                if (Numbers0 == Numbers1) {
                    Cflint_ShiftRight2(Operand, Length, 1);
                    MovedBits2 -= 1;
                }
                /* 不在同一水位线时,缺少退位时,除数回退N个位 */
                if (Numbers0 != Numbers1 && Numbers1 - Numbers0 > MovedBits2) {
                    Cflint_ShiftRight2(Operand, Length, MovedBits2);
                    MovedBits2 = 0;
                }
                /* 不在同一水位线时,冗余退位时,除数回退N个位 */
                if (Numbers0 != Numbers1 && Numbers1 - Numbers0 <= MovedBits2) {
                    Cflint_ShiftRight2(Operand, Length, Numbers1 - Numbers0);
                    MovedBits2 -= Numbers1 - Numbers0;
                }
                continue;
            }
            return;
        }
        /* 被除数大于除数 */
        if (CompareResult == 1) {
            /* 计算被除数和除数有效2进制位数 */
            int64_t Numbers0 = Cflint_Numbers2(Module, Length);
            int64_t Numbers1 = Cflint_Numbers2(Operand, Length);
            /* 如果它们未处在同一级别,除数放大且小于被除数 */
            if (Numbers0 > Numbers1 + 1) {
                Cflint_ShiftLeft2(Operand, Length, Numbers0 - Numbers1);
                MovedBits2 += Numbers0 - Numbers1;
                continue;
            }
            /* 现在被除数和除数至少处于同一级别, 执行约减 */
            Cflint_Subtraction(Module, Module, Operand, Length);
            continue;
        }
    } while (1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 基础模运算2 */
void Cflint_Modulo2(CFLINT_TYPE *Operand, uint32_t Length, int64_t Bits2)
{
    int64_t Bits_N = Bits2 / CFLINT_BITS;
    int64_t Bits_2 = Bits2 % CFLINT_BITS;
    CFLINT_TYPE Bit_N = 0;
    /* 简要检查 */
    if (Bits2 > Length *CFLINT_BITS || Bits2 == 0)
        return;
    /* 移除高位 */
    for (int64_t Index = Bits_N + 1; Index < Length; Index++)
        Operand[Index] = 0;
    /* 移出位内高位 */
    Bit_N = Operand[Bits_N];
    Operand[Bits_N] <<= (CFLINT_BITS - Bits_2);
    Operand[Bits_N] >>= (CFLINT_BITS - Bits_2);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
