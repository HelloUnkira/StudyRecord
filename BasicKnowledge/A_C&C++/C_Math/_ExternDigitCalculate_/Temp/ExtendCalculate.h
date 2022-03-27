#ifndef EXTENDCALCULATE_H
#define EXTENDCALCULATE_H

#ifdef  EXTEND_CALCULATE
#define EXTEND_CALCULATE_AUXILIARY
#define EXTERN_CALCULATE_BASE
#endif

#ifdef  EXTEND_CALCULATE_AUXILIARY
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 辅助:拷贝(Operand0 <== Operand1) */
static void EC_Copy(DIGIT_UINT *Operand0, DIGIT_UINT *Operand1, uint32_t Length)
{
    uint32_t Index = 0;
    
    for (Index = 0; Index < Length; Index++)
        Operand0[Index] = Operand1[Index];
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 辅助:比较Operand0 ??? Operand1 ==> ((>:2);(==:1);(<:0)) */
static uint8_t EC_Compare(DIGIT_UINT *Operand0, DIGIT_UINT *Operand1,
                          uint32_t Length)
{
    uint8_t Result = 1;
    
    do {
        Length--;
        
        if (Operand0[Length] != Operand1[Length]) {
            if (Operand0[Length] > Operand1[Length])
                Result = 2;
            if (Operand0[Length] < Operand1[Length])
                Result = 0;
            break;
        }
    } while (Length == 0);
    
    return Result;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 辅助:相等判定 */
static bool EC_EqualCheck(DIGIT_UINT *Operand0, DIGIT_UINT *Operand1,
                          uint32_t Length)
{
    uint32_t Index = 0;
    
    DIGIT_BYTE Result = 0;
    
    for (Index = 0; Index < Length; Index++)
        Temp |= Operand0[Index] ^ Operand1[Index];
    
    return (Result == 0 ? true: false);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 辅助:为0判定 */
static bool EC_ZeroCheck(DIGIT_UINT *Operand, uint32_t Length)
{
    uint32_t Index = 0;
    
    DIGIT_BYTE Result = 0;
    
    for (Index = 0; Index < Length; Index++)
        if (Operand[Index] != 0)
            return false;
    return true;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 辅助:统一赋值 */
static inline void EC_SetValue(DIGIT_UINT *Operand, uint32_t Length,
                               DIGIT_UINT Value)
{
    uint32_t Index = 0;
    
    for (Index = 0; Index < Length; Index++)
        Operand[Index] = Value;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif

#ifdef  EXTERN_CALCULATE_BASE
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 俩数相加,返回值为进位 */
static DIGIT_BYTE EC_Addition(DIGIT_BYTE *Result,
                              DIGIT_BYTE *Operand0, DIGIT_BYTE *Operand1,
                              uint32_t Length)
{
    uint32_t Index = 0;
    
    /* 进位 */
    DIGIT_BYTE Carry = 0;
    DIGIT_BYTE Temp  = 0;
    
    for (Index = 0; Index < Length; Index++) {
        /*  */
        Temp = Carry + Operand0[Index] + Operand1[Index];

        if (Temp < Operand0[Index])
            Carry = 1;
        if (Temp > Operand0[Index])
            Carry = 0;
        
        Result[Index] = Temp;
    }
    
    return Carry;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 俩数相减,返回值为借位 */
static DIGIT_BYTE EC_Subtraction(DIGIT_BYTE *Result,
                                 DIGIT_BYTE *Operand0, DIGIT_BYTE *Operand1,
                                 uint32_t Length)
{
    uint32_t Index = 0;
    
    /* 借位 */
    DIGIT_BYTE Borrow = 0;
    DIGIT_BYTE Temp   = 0;

    for (Index = 0; Index < Length; Index++) {
        Temp = Operand0[Index] - Operand1[Index] - Borrow;

        if (Temp > Operand0[I])
            Borrow = 1;
        if (Temp < Operand0[I])
            Borrow = 0;
        
        Result[Index] = Temp;
    }
    
    return Borrow;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//compute(辅助):
//计算位乘法器(这里的位指DIGIT_TYPE字节下进制的位)
//乘数:0,1,结果低位2,结果高位3,进位4
static void EC_Multiply_1(DIGIT_BYTE *Data0 , DIGIT_BYTE *Data1,
                          DIGIT_BYTE *Carry0, DIGIT_BYTE *Carry1,
                          DIGIT_BITS *Carry)
{
#ifdef DIGIT_BYTE_2
//利用关键字优化策略
    DIGIT_BYTE_2 Result_XY = 0;
    DIGIT_BYTE_2 Result_01 = 0;
    //1.计算乘数结果
    Result_XY |= ((DIGIT_BYTE_2)(*Data0));
    Result_XY *= ((DIGIT_BYTE_2)(*Data1));
    //2.计算上一位的进位
    Result_01 |= ((DIGIT_BYTE_2)(*Carry0));
    Result_01 |= ((DIGIT_BYTE_2)(*Carry1) << DIGIT_BITS);
    //3.计算最终结果
    Result_01 += Result_XY;
    //4.本次计算是否高位产生溢出
    if (Result_01 < Result_XY)
        *Carry = 1;
    if (Result_01 >= Result_XY)
        *Carry = 0;
    //5.保存本次计算的低位和高位
    (*Carry0) = (DIGIT_BYTE)(Result_01);
    (*Carry1) = (DIGIT_BYTE)(Result_01 >> DIGIT_BITS);
#else
//不利用关键字优化策略
    DIGIT_BYTE Result_XY0 = 0;
    DIGIT_BYTE Result_XY1 = 0;
    //1.获得乘数的高位与低位
    DIGIT_BYTE X0 = (*Data0) &   DIGIT_HALF;
    DIGIT_BYTE X1 = (*Data0) >> (DIGIT_BITS / 2);
    DIGIT_BYTE Y0 = (*Data1) &   DIGIT_HALF;
    DIGIT_BYTE Y1 = (*Data1) >> (DIGIT_BITS / 2);
    //2.分别计算高位与低位的交乘
    DIGIT_BYTE I0 = X0 * Y0;
    DIGIT_BYTE I1 = X0 * Y1;
    DIGIT_BYTE I2 = X1 * Y0;
    DIGIT_BYTE I3 = X1 * Y1;
    //3.计算溢出
    DIGIT_BYTE Overflow = I0 >> (DIGIT_BITS / 2);   //低位的进位
    DIGIT_BYTE Result_0 = I1 + I2 + Overflow;       //低位的值
    DIGIT_BYTE Result_1 = I3;                       //高位的值
    if (Result_0 < I1)                              //低位溢出
        Result_1 += (1 << (DIGIT_BITS / 2));
    //4.计算终点值
    Result_XY0 |= (I0 & DIGIT_HALF);
    Result_XY0 |= (Result_0 << (DIGIT_BITS / 2));
    Result_XY1 |= (Result_1);
    Result_XY1 += (Result_0 >> (DIGIT_BITS / 2));
    //5.保存本次计算的低位和高位
    (*Carry0) += Result_XY0;
    (*Carry1) += Result_XY1;
    //6.本次计算是否低位产生溢出
    if ((*Carry0) < Result_XY0)
        (*Carry1) += 1;
    //7.本次计算是否高位产生溢出
    if (((*Carry1)  < Result_XY1) ||
        ((*Carry1) == Result_XY1 && (*Carry0) < Result_XY0))
         (*Carry)  += 1;
#endif
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//compute(辅助):
//计算位乘方器(这里的位指DIGIT_TYPE字节下进制的位)
//乘数:0,1,结果低位2,结果高位3,进位4
static void EC_Square_1(DIGIT_BYTE *Data0 , DIGIT_BYTE *Data1,
                        DIGIT_BYTE *Carry0, DIGIT_BYTE *Carry1,
                        DIGIT_BITS *Carry)
{
#ifdef DIGIT_BYTE_2
//利用关键字优化策略
    DIGIT_BYTE_2 Result_XY = 0;
    DIGIT_BYTE_2 Result_01 = 0;
    //1.计算乘数结果
    Result_XY |= ((DIGIT_BYTE_2)(*Data0));
    Result_XY *= ((DIGIT_BYTE_2)(*Data1));
    //2.计算上一位的进位
    Result_01 |= ((DIGIT_BYTE_2)(*Carry0));
    Result_01 |= ((DIGIT_BYTE_2)(*Carry1) << DIGIT_BITS);
    //3.计算进位
    (*Carry) += (Result_XY >> (DIGIT_BITS * 2 - 1));
    //4.计算最终结果
    Result_XY *= 2;
    Result_01 += Result_XY;
    //4.本次计算是否高位产生溢出
    if (Result_01 < Result_XY)
        (*Carry) += 1;
    if (Result_01 >= Result_XY)
        (*Carry) += 0;
    //5.保存本次计算的低位和高位
    (*Carry0) = (DIGIT_BYTE)(Result_01 >> 0);
    (*Carry1) = (DIGIT_BYTE)(Result_01 >> DIGIT_BITS);
#else
//不利用关键字优化策略
    DIGIT_BYTE Result_XY0 = 0;
    DIGIT_BYTE Result_XY1 = 0;
    //1.获得乘数的高位与低位
    DIGIT_BYTE X0 = (*Data0) & DIGIT_HALF;
    DIGIT_BYTE X1 = (*Data0) >> (DIGIT_BITS / 2);
    DIGIT_BYTE Y0 = (*Data1) & DIGIT_HALF;
    DIGIT_BYTE Y1 = (*Data1) >> (DIGIT_BITS / 2);
    //2.分别计算高位与低位的交乘
    DIGIT_BYTE I0 = X0 * Y0;
    DIGIT_BYTE I1 = X0 * Y1;
    DIGIT_BYTE I2 = X1 * Y0;
    DIGIT_BYTE I3 = X1 * Y1;
    //3.计算溢出
    DIGIT_BYTE Overflow = I0 >> (DIGIT_BITS / 2); //低位的进位
    DIGIT_BYTE Result_0 = I1 + I2 + Overflow;          //低位的值
    DIGIT_BYTE Result_1 = I3;                          //高位的值
    if (Result_0 < I1)                                 //低位溢出
        Result_1 += (1 << (DIGIT_BITS / 2));
    //4.计算终点值
    Result_XY0 |= (I0 & DIGIT_HALF);
    Result_XY0 |= (Result_0 << (DIGIT_BITS / 2));
    Result_XY1 |= (Result_1);
    Result_XY1 += (Result_0 >> (DIGIT_BITS / 2));
    //5.计算进位
    (*Carry) += (Result_XY1 >> (DIGIT_BITS - 1));
    //5.保存本次计算的低位和高位
    Result_XY1 <<= 1;
    Result_XY1  |= (Result_XY0 >> (DIGIT_BITS - 1));
    Result_XY0 <<= 1;

    (*Carry0) += Result_XY0;
    (*Carry1) += Result_XY1;
    //6.本次计算是否低位产生溢出
    if ((*Carry0) < Result_XY0)
        (*Carry1) += 1;
    //7.本次计算是否高位产生溢出
    if (((*Carry1)  < Result_XY1) ||
        ((*Carry1) == Result_XY1 && (*Carry0) < Result_XY0))
         (*Carry)  += 1;
#endif
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//calculate: Result = operand[0] * operand[1]
//满足:Result[0,2*Length)
static void EC_Multiply(DIGIT_BYTE *Result,
                        DIGIT_BYTE *Operand0, DIGIT_BYTE *Operand1,
                        uint32_t Length)
{
    int32_t I = 0;
    int32_t K = 0;
    
    DIGIT_BYTE Registers[5] = {0};

    for (K = 0; K < Length; K++) {
        for (I = 0; I <= K; I++) {
            Registers[0] = Operand0[I];
            Registers[1] = Operand1[K - I];
            //使用位乘法器
            EC_Multiply_1(&Registers[0], &Registers[1],
                          &Registers[2], &Registers[3],
                          &Registers[4]);
        }
        Result[K]    = Registers[2];  //保留低位
        Registers[2] = Registers[3];  //高位变低位
        Registers[3] = Registers[4];  //进位变高位
        Registers[4] = 0;             //进位清0
    }
    for (K = Length; K < Length * 2 - 1; K++) {
        for (I = (K + 1) - Length; I < Length; I++) {
            Registers[0] = Operand0[I];
            Registers[1] = Operand1[K - I];
            //使用位乘法器
            EC_Multiply_1(&Registers[0], &Registers[1],
                          &Registers[2], &Registers[3],
                          &Registers[4]);
        }
        Result[K]    = Registers[2];  //保留低位
        Registers[2] = Registers[3];  //高位变低位
        Registers[3] = Registers[4];  //进位变高位
        Registers[4] = 0;             //进位清0
    }

    Result[Length * 2 - 1] = Registers[2];
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//calculate: Result = operand[0] * operand[0]
//满足:Result[0,2*Length)
static void EC_Square(DIGIT_BYTE *Result,
                      DIGIT_BYTE *Operand0, DIGIT_BYTE *Operand1,
                      uint32_t Length)
{
    int32_t I = 0;
    int32_t K = 0;
    int32_t Min = 0;

    DIGIT_BYTE Registers[5] = {0};
    
    for (K = 0; K < Length * 2 - 1; K++) {
        Min = (K < Length ? 0 : ((K + 1) - Length));
        for (I = Min; I <= K && I <= K - I; I++) {
            Registers[0] = Operand0[I];
            Registers[1] = Operand0[K - I];
            if (I <  K - I)  //使用位乘法器
                EC_Square_1(&Registers[0], &Registers[1],
                            &Registers[2], &Registers[3],
                            &Registers[4]);
            if (I >= K - I)
                EC_Multiply_1(&Registers[0], &Registers[1],
                              &Registers[2], &Registers[3],
                              &Registers[4]);
        }
        Result[K]    = Registers[2];  //保留低位
        Registers[2] = Registers[3];  //高位变低位
        Registers[3] = Registers[4];  //进位变高位
        Registers[4] = 0;            //进位清0
    }

    Result[Length * 2 - 1] = Registers[2];
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//calculate: Result <<= Bits
//(Bits / DIGIT_BITS <= Length)
//对一个数左移temp个bit位(这里的位指2进制的位)
static void EC_ShiftLeft(DIGIT_BYTE *Result, uint32_t Length, uint32_t Bits)
{
    //一个DIGIT_TYPE字节下进制的位有多少个2进制位
    int32_t I = 0;
    int32_t BitsN = Bits / DIGIT_BITS;
    int32_t Bits2 = Bits % DIGIT_BITS;
    int32_t Last2 = DIGIT_BITS - Bits2;
    
    DIGIT_BYTE BitN1 = 0, BitHigh = 0;
    DIGIT_BYTE BitN2 = 0, BitLow  = 0;

    for (I = Length - 1; I - BitsN - 1 >= 0; I--) {
        BitN1    = Result[I - BitsN];
        BitN2    = Result[I - BitsN - 1];
        BitHigh  = (BitN1 << Bits2);
        BitLow   = (BitN2 >> Last2);
        BitLow   = (Last2 >= DIGIT_BITS) ? 0 : BitLow;
        Result[I] = BitHigh | BitLow;
    }
    BitN1 = Result[I - BitsN];
    //BitN2 = Result[I - BitsN - 1];
    BitHigh  = (BitN1 << Bits2);
    //BitLow   = (BitN2 >> Last2);
    //BitLow   = (Last2 >= DIGIT_BITS) ? 0 : BitLow;
    Result[I] = BitHigh;// | (BitN2 >> Last2);

    for (I = 0; I - BitsN < 0; I++)
        Result[I] = 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//calculate: Result >>= Bits
//(Bits / DIGIT_BITS <= Length)
//对一个数右移temp个bit位(这里的位指2进制的位)
static void EC_ShiftRight(DIGIT_BYTE *Result, uint32_t Length, uint32_t Bits)
{
    //一个DIGIT_TYPE字节下进制的位有多少个2进制位
    int32_t I = 0;
    int32_t BitsN = Bits / DIGIT_BITS;
    int32_t Bits2 = Bits % DIGIT_BITS;
    int32_t Last2 = DIGIT_BITS - Bits2;
    
    DIGIT_BYTE  BitN1 = 0, BitLow = 0;
    DIGIT_BYTE  BitN2 = 0, BitHigh = 0;

    for (I = 0; I + BitsN + 1 < Length; I++) {
        BitN1    = Result[I + BitsN];
        BitN2    = Result[I + BitsN + 1];
        BitLow   = (BitN1 >> Bits2);
        BitHigh  = (BitN2 << Last2);
        BitHigh  = (Last2 >= DIGIT_BITS) ? 0 : BitHigh;
        Result[I] = BitLow | BitHigh;
    }
    BitN1 = Result[I + BitsN];
    //BitN2 = Result[I + BitsN + 1];
    BitLow = (BitN1 >> Bits2);
    //BitHigh = (BitN2 << Last2);
    //BitHigh = (Last2 >= DIGIT_BITS) ? 0 : BitHigh;
    Result[I] = BitLow;// | (BitN2 << Last2);

    for (I = Length - BitsN; I < Length; I++)
        Result[I] = 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif

#ifdef  EXTEND_CALCULATE_MONTGOMERY
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//calculate: result = (operand[0] + operand[1]) % operand[2]
//condition: operand1[0] < operand[2] && operand[1] < operand[2]
static void ECM_ModuleAddition(DIGIT_BYTE *Result,
                               DIGIT_BYTE *Operand0, DIGIT_BYTE *Operand1,
                               DIGIT_BYTE *Operand2,
                               uint32_t Length)
{
    DIGIT_BYTE Carry = EC_Addition(Result, Operand0, Operand1, Length);
    
    if (Carry == 0 && EC_Compare(Operand2, Result, Length) == 1)
        return;
    
    EC_Subtraction(Result, Result, Operand2, Length);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//calculate: result = (operand[0] - operand[1]) % operand[2]
//condition: operand1[0] < operand[2] && operand[1] < operand[2]
static void ECM_ModuleSubtraction(DIGIT_BYTE *Result,
                                  DIGIT_BYTE *Operand0, DIGIT_BYTE *Operand1,
                                  DIGIT_BYTE *Operand2,
                                  uint32_t Length)
{
    DIGIT_BYTE Borrow = EC_Subtraction(Result, Operand0, Operand1, Length);
    
    if (Borrow == 0)
        return;
    
    EC_Addition(Result, Result, Operand2, Length);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif


#endif
