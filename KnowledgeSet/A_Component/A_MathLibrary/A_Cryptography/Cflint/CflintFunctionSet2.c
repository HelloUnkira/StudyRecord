#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_FUNCTION_SET2
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*备注:和,差,乘,方,逆运算实现来源于uECC.c.h */
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
    /* 计算数累减一个位 */
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
    /* 计算数累减一个模 */
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
        /* 计算差结果,暂存到临时区 */
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
    if (Cflint_IsZero(Operand, Length))
        return;
    /* 除数移位记录 */
    int64_t MovedBits2 = 0;
    /* 主迭代运行 */
    do {
        /* 比较被除数和除数 */
        int8_t CompareResult = Cflint_Compare(Module, Operand, Length);
        /* 被除数等于除数,恰好被整除时 */
        if (CompareResult == 0) {
            /* 商累加1个单元 */
            Cflint_AdditionBit(Quotient, Length, 1);
            if (MovedBits2 != 0) {
                /* 商进所有位,除数回退所有位 */
                Cflint_ShiftLeft2(Quotient, Length, MovedBits2);
                Cflint_ShiftRight2(Operand, Length, MovedBits2);
            }
            Cflint_SetValue(Module, Length, 0);
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
    if (Cflint_IsZero(Operand, Length))
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
                /* 除数回退所有位 */
                Cflint_ShiftRight2(Operand, Length, MovedBits2);
            }
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
/* 模和运算 */
void Cflint_ModuloAddition(CFLINT_TYPE *Result,   CFLINT_TYPE *Module,
                           CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                           CFLINT_TYPE *Temp[2],     uint32_t  Length)
{
    Cflint_Modulo(Temp[0], Operand0, Module, Length);
    Cflint_Modulo(Temp[1], Operand1, Module, Length);
    
    if (Cflint_Addition(Temp[0], Temp[0], Temp[1], Length) != 0 ||
        Cflint_Compare(Temp[0], Module, Length) != -1)
        Cflint_Subtraction(Temp[0], Temp[0], Module, Length);
    Cflint_Copy(Result, Temp[0], Length);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模差运算 */
void Cflint_ModuloSubtraction(CFLINT_TYPE *Result,   CFLINT_TYPE *Module,
                              CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                              CFLINT_TYPE *Temp[2],     uint32_t  Length)
{
    Cflint_Modulo(Temp[0], Operand0, Module, Length);
    Cflint_Modulo(Temp[1], Operand1, Module, Length);
    
    if (Cflint_Subtraction(Temp[0], Temp[0], Temp[1], Length) != 0)
        Cflint_Addition(Temp[0], Temp[0], Module, Length);
    Cflint_Copy(Result, Temp[0], Length);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模乘运算 */
void Cflint_ModuloMultiply(CFLINT_TYPE *Result,   CFLINT_TYPE *Module,
                           CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                           CFLINT_TYPE *Temp[2],     uint32_t  Length)
{
    Cflint_Multiply(Temp[0], Operand0, Operand1, Length);
    Cflint_SetValue(Temp[1], Length * 2, 0);
    Cflint_Copy(Temp[1], Module, Length);
    Cflint_Modulo(Temp[0], Temp[0], Temp[1], Length * 2);
    Cflint_Copy(Result, Temp[0], Length);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模方运算 */
void Cflint_ModuloSquare(CFLINT_TYPE *Result,  CFLINT_TYPE *Module,
                         CFLINT_TYPE *Operand, CFLINT_TYPE *Temp[2],
                            uint32_t  Length)
{
    Cflint_Square(Temp[0], Operand, Length);
    Cflint_SetValue(Temp[1], Length * 2, 0);
    Cflint_Copy(Temp[1], Module, Length);
    Cflint_Modulo(Temp[0], Temp[0], Temp[1], Length * 2);
    Cflint_Copy(Result, Temp[0], Length);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模幂运算 */
void Cflint_ModuloExponent(CFLINT_TYPE *Result,  CFLINT_TYPE *Module,
                           CFLINT_TYPE *Operand, CFLINT_TYPE *Exponent,
                           CFLINT_TYPE *Temp[4],    uint32_t  Length)
{
    /* 固有开销 */
    CFLINT_TYPE *X = Temp[0];  //保留Result
    CFLINT_TYPE *Y = Temp[1];  //保留Module
    CFLINT_TYPE *A = Temp[2];  //保留Operand
    CFLINT_TYPE *B = Temp[3];  //保留Operand
    
    /* 特例:除数为0检查 */
    if (Cflint_IsZero(Module, Length))
        return;
    /* 初始化:X == 1, Y == Module */
    Cflint_SetValue(X, Length * 2, 0);
    Cflint_AdditionBit(X, Length * 2, 1);
    Cflint_SetValue(Y, Length * 2, 0);
    Cflint_Copy(Y, Module, Length);
    /* 初始化:A == Operand % Module, B == 0 */
    Cflint_SetValue(A, Length * 2, 0);
    Cflint_Modulo(A, Operand, Module, Length);
    Cflint_SetValue(B, Length * 2, 0);
    /* 计算:2**Numbers <= Exponent < 2**(Numbers + 1) */
    int64_t Numbers = Cflint_Numbers2(Exponent, Length);
    /* 检查:2**K == Exponent */
    bool NumbersOnlyOne = Cflint_IsExponent2(Exponent, Length);
    /* 特殊优化场景:指数为2的幂 */
    if ( NumbersOnlyOne) {
        /* 一个数的0次幂为1 */
        if (Numbers == -1) {
            Cflint_Copy(Result, X, Length);
            return;
        }
        /* 一个数的1次幂为本身 */
        if (Numbers == 0) {
            Cflint_Modulo(X, A, Y, Length * 2);
            Cflint_Copy(Result, X, Length);
            return;
        }
        /* 顺向迭代 */
        for (int64_t Bits2 = 1; Bits2 <= Numbers; Bits2++) {
            /* 计算:X = A**2 % Y */
            Cflint_Square(B, A, Length);
            Cflint_Copy(A, B, Length);
            Cflint_Modulo(X, A, Y, Length * 2);
        }
        Cflint_Copy(Result, X, Length);
        return;
    }
    /* 普通场景 */
    if (!NumbersOnlyOne) {
        for (int64_t Bits2 = 0; Bits2 <= Numbers; Bits2++) {
            /* 索引计算 */
            uint32_t BitN = Bits2 / CFLINT_BITS;
            uint32_t Bit2 = Bits2 % CFLINT_BITS;
            /* 逆向迭代 */
            if ((Exponent[BitN] & (1 << Bit2)) != 0) {
                /* 计算:X = X * A % Y */
                Cflint_Multiply(B, X, A, Length);
                Cflint_Modulo(X, B, Y, Length * 2);
            }
            /* 计算:A = A**2 % Y */
            Cflint_Square(B, A, Length);
            Cflint_Modulo(A, B, Y, Length * 2);
        }
        Cflint_Copy(Result, X, Length);
        return;
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模逆运算 */
void Cflint_ModuloInverse(CFLINT_TYPE *Result, CFLINT_TYPE *Operand,
                          CFLINT_TYPE *Module, CFLINT_TYPE *Temp[4],
                             uint32_t  Length)
{
    /* 固有额外空间开销 */
    CFLINT_TYPE *A = Temp[0];
    CFLINT_TYPE *B = Temp[1];
    CFLINT_TYPE *U = Temp[2];
    CFLINT_TYPE *V = Temp[3];

    /* 1.除数为0检查 */
    if (Cflint_IsZero(Operand, Length)) {
        Cflint_SetValue(Result, Length, 0);
        return;
    }
    /* 2.初始化:A = Operand,B = Module */
    Cflint_Copy(A, Operand, Length);
    Cflint_Copy(B, Module,  Length);
    /* 3.初始化:V = 0,U = 1 */
    Cflint_SetValue(V, Length, 0);
    Cflint_SetValue(U, Length, 0);
    Cflint_AdditionBit(U, Length, 1);
    /* 4.准备状态量 */
    int8_t CompareResult = 0;
    /* 5.开始主流程 */
    while ((CompareResult = Cflint_Compare(A, B, Length)) != 0) {
        if ((A[0] & 1) == 0 || (B[0] & 1) == 0) {
            /* 场景1:A最低位为0,或,B最低位为0 */
            CFLINT_TYPE *AB = NULL, *UV = NULL;
            /* 合并场景,这里先查B后查A */
            if ((B[0] & 1) == 0) { AB = B; UV = V;}
            if ((A[0] & 1) == 0) { AB = A; UV = U;}
            /* AB >>= 1 */
            Cflint_ShiftRight2(AB, Length, 1);
            /* UV最低位不为0:UV += Module */
            CFLINT_TYPE Overflow = 0;
            if ((UV[0] & 1) != 0)
                Overflow = Cflint_Addition(UV, UV, Module, Length);
            /* UV >>= 1 */
            Cflint_ShiftRight2(UV, Length, 1);
            /* 溢出检查 */
            if (Overflow != 0)
                UV[Length - 1] |= CFLINT_BYTEHIGHEST;
            continue;
        }
        /* 场景2.A>B,或,A<=B */
        {
            CFLINT_TYPE *AB_Max = NULL, *UV_Max = NULL;
            CFLINT_TYPE *AB_Min = NULL, *UV_Min = NULL;
            /* 合并场景 */
            if (CompareResult == 1) {
                AB_Max = A; UV_Max = U;
                AB_Min = B; UV_Min = V;
            }
            if (CompareResult != 1) {
                AB_Max = B; UV_Max = V;
                AB_Min = A; UV_Min = U;
            }
            /* AB_Max -= AB_Min, AB_Max >>= 1 */
            Cflint_Subtraction(AB_Max, AB_Max, AB_Min, Length);
            Cflint_ShiftRight2(AB_Max, Length, 1);
            /* 对UV进行蒙哥马利约减:(UV_Max - UV_Min) % Module */
            if (Cflint_Compare(UV_Max, UV_Min, Length) == -1)
                Cflint_Addition(UV_Max, UV_Max, Module, Length);
            Cflint_Subtraction(UV_Max, UV_Max, UV_Min, Length);
            /* Max1最低位不为0:UV_Max += Module */
            CFLINT_TYPE Overflow = 0;
            if ((UV_Max[0] & 1) != 0)
                Overflow = Cflint_Addition(UV_Max, UV_Max, Module, Length);
            /* UV_Max >>= 1 */
            Cflint_ShiftRight2(UV_Max, Length, 1);
            /* 溢出检查 */
            if (Overflow != 0)
                UV_Max[Length - 1] |= CFLINT_BYTEHIGHEST;
            continue;
        }
    }
    /* 拷贝数据结果 */
    Cflint_Copy(Result, U, Length);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 带符号与运算:Result * (*ResultFlag) = Operand1 * (Operand1_Flag) + */
/*                                       Operand2 * (Operand2_Flag)   */
/* 备注:大数运算本身是无符号语义运算,这里是为某些数学运算额外扩展 */
CFLINT_TYPE Cflint_FlagSum(CFLINT_TYPE *Result,   CFLINT_TYPE *ResultFlag,
                           CFLINT_TYPE *Operand1, CFLINT_TYPE  Operand1_Flag,
                           CFLINT_TYPE *Operand2, CFLINT_TYPE  Operand2_Flag,
                              uint32_t  Length)
{
    if (Operand1_Flag == 0 && Operand2_Flag == 0) {
        *ResultFlag = 0;
        return Cflint_Addition(Result, Operand1, Operand2, Length);
    }
    if (Operand1_Flag == 1 && Operand2_Flag == 1) {
        *ResultFlag = 1;
        return Cflint_Addition(Result, Operand1, Operand2, Length);
    }
    int8_t CompareResult = 0;
    if (Operand1_Flag == 0 && Operand2_Flag == 1) {
        CompareResult = Cflint_Compare(Operand1, Operand2, Length);
        if (CompareResult != -1) {
            *ResultFlag = 0;
            return Cflint_Subtraction(Result, Operand1, Operand2, Length);
        }
        if (CompareResult == -1) {
            *ResultFlag = 1;
            return Cflint_Subtraction(Result, Operand2, Operand1, Length);
        }
    }
    if (Operand1_Flag == 1 && Operand2_Flag == 0) {
        CompareResult = Cflint_Compare(Operand2, Operand1, Length);
        if (CompareResult != -1) {
            *ResultFlag = 0;
            return Cflint_Subtraction(Result, Operand2, Operand1, Length);
        }
        if (CompareResult == -1) {
            *ResultFlag = 1;
            return Cflint_Subtraction(Result, Operand1, Operand2, Length);
        }
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 带符号模运算:Result = Operand1 * (Operand1_Flag) % Operand2 */
/* 备注:大数运算本身是无符号语义运算,这里是为某些数学运算额外扩展 */
void Cflint_FlagModulo(CFLINT_TYPE *Result,   CFLINT_TYPE *Operand1,
                       CFLINT_TYPE *Operand2, CFLINT_TYPE  Operand1_Flag,
                          uint32_t  Length)
{
    if (Operand1_Flag == 0) {
        Cflint_Modulo(Result, Operand1, Operand2, Length);
        return;
    }
    if (Operand1_Flag == 1) {
        Cflint_Modulo(Result, Operand1, Operand2, Length);
        Cflint_Subtraction(Result, Operand1, Result, Length);
        return;
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
