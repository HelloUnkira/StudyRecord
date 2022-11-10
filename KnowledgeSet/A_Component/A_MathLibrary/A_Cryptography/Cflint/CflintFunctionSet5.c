#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_FUNCTION_SET5
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 大随机数生成 */
void Cflint_Random(CFLINT_TYPE  *Operand, uint32_t Length,
                   CFLINT_TYPE (*Random)(void))
{
    /* 这里直接以最简单的方式生成即可,但要逆向生成 */
    /* 因为随机数的目标是每一个bit位都随机 */
    /* 那么大小随机数均可完成该功能 */
    for (uint32_t Index = 0; Index < Length; Index++)
        Operand[Length - Index - 1] = Random();
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 强素数判别检查(Miller_Rabin测试) */
bool Cflint_IsPrime(CFLINT_TYPE *X, CFLINT_TYPE *Temp[7], uint32_t Length,
                    CFLINT_TYPE (*Random)(void), bool Weak)
{
    CFLINT_TYPE  *A  = Temp[0];
    CFLINT_TYPE  *R  = Temp[1];
    CFLINT_TYPE  *E  = Temp[2];
    CFLINT_TYPE **TT = Temp + 3;   //Temp[3~6]:Length * 2
    /* 小素数筛查 */
    if (Weak) {
        /* 数据量决定了当次小筛选的强度 */
        uint32_t SizerLength  = 50;
        uint32_t SizerTable[] = {
              3,   5,   7,  11,  13,  17,  19,  23,  29,  31,
             37,  41,  43,  47,  53,  59,  61,  67,  71,  73,
             79,  83,  89,  97, 101, 103, 107, 109, 113, 127,
            131, 137, 139, 149, 151, 157, 163, 167, 173, 179,
            181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
        };
        for (uint32_t Index = 0; Index < SizerLength; Index++) {
            Cflint_SetValue(A, Length, 0);
            /* 设置除数 */
#if 0
#elif CFLINT_BYTE == 1
            A[0] = SizerTable[Index] >>  0;
            A[1] = SizerTable[Index] >>  8;
            A[2] = SizerTable[Index] >> 16;
            A[3] = SizerTable[Index] >> 24;
#elif CFLINT_BYTE == 2
            A[0] = SizerTable[Index] >>  0;
            A[1] = SizerTable[Index] >> 16;
#else
            Cflint_AdditionBit(A, Length, SizerTable[Index]);
#endif
            /* 计算模,如果能被整除则未通过当次素性测试 */
            Cflint_Modulo(R, X, A, Length);
            if (Cflint_IsZero(R, Length))
                return false;
        }
    }
    /* 素数筛查(Miller_Rabin测试) */
    //位长度: 512;  轮数: 7
    //位长度: 1024; 轮数: 4
    //位长度: 1536; 轮数: 3
    uint32_t Wheel = 10;
    uint32_t Bits2 = Length * CFLINT_BITS;
    if (Bits2 >= 1536) Wheel = 3; else
    if (Bits2 >= 1024) Wheel = 4; else
    if (Bits2 >=  512) Wheel = 7; else Wheel = 10;
    for (uint32_t Count = 0; Count < Wheel; Count++) {
        /* 随机生成一个数并满足它小于: P - 1 */
        Cflint_Random(A, Length, Random);
        Cflint_SubtractionBit(X, Length, 1);
        Cflint_Modulo(A, A, X, Length);
        Cflint_AdditionBit(X, Length, 1);
        /* 设置初始指数为:D(奇数) = (P - 1) / (2 ** K) */
        uint64_t Bit2_K = 0;
        Cflint_Copy(E, X, Length);
        Cflint_SubtractionBit(E, Length, 1);
        while (Cflint_IsEven(E, Length)) {
            Cflint_ShiftRight2(E, Length, 1);
            Bit2_K++;
        }
        /* 初始测试:R = A ^ D Mod X */
        Cflint_ModuloExponent(R, X, A, E, TT, Length);
        /* 检查模是否为 X - 1 */
        Cflint_AdditionBit(R, Length, 1);
        if (Cflint_Equal(R, X, Length))
            continue;
        /* 检查模是否为 1 */
        Cflint_SubtractionBit(R, Length, 2);
        if (Cflint_IsZero(R, Length))
            continue;
        Cflint_AdditionBit(R, Length, 1);
        bool BreakLoop = false;
        /* 循环测试:R = R ^ 2 Mod X */
        for (uint32_t Index1 = 0; Index1 < Bit2_K; Index1++) {
            Cflint_Copy(A, R, Length);
            Cflint_ModuloSquare(R, X, A, TT, Length);
            /* 检查模是否为 X - 1 */
            Cflint_AdditionBit(R, Length, 1);
            if (Cflint_Equal(R, X, Length)) {
                BreakLoop = true;
                break;
            }
            /* 检查模是否为 1 */
            Cflint_SubtractionBit(R, Length, 2);
            if (Cflint_IsZero(R, Length))
                return false;
            Cflint_AdditionBit(R, Length, 1);
        }
        if (BreakLoop)
            continue;
        return false;
    }
    return true;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 指定查找次数,尝试找到一个强素数(Min和Max同时为空则取值范围在全域内) */
uint32_t Cflint_RandomPrime(CFLINT_TYPE *X, CFLINT_TYPE *Temp[7],
                            CFLINT_TYPE *Min, CFLINT_TYPE *Max, uint32_t Length,
                            CFLINT_TYPE (*Random)(void), uint32_t CountMax)
{
    for (uint32_t Count = 0; Count < CountMax; Count++) {
        /* 生成随机数 */
        Cflint_Random(X, Length, Random);
        if (Cflint_IsEven(X, Length))
            Cflint_AdditionBit(X, Length, 1);
        if (Min != NULL && Max != NULL) {
            Cflint_Subtraction(Temp[0], Max, Min, Length);
            Cflint_Modulo(Temp[1], X, Temp[0], Length);
            Cflint_Addition(X, Min, Temp[1], Length);
            if (Cflint_IsEven(X, Length))
                Cflint_AdditionBit(X, Length, 1);
            if (Cflint_Compare(X, Max, Length) == +1)
                Cflint_SubtractionBit(X, Length, 2);
            if (Cflint_Compare(X, Min, Length) == -1)
                return 0;
        }
        
        if (Cflint_IsPrime(X, Temp, Length, Random, false))
            return Count;
    }
    return CountMax;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 孪生素数判别检查 */
bool Cflint_IsPrime1(CFLINT_TYPE *X, CFLINT_TYPE *Temp[3], uint32_t Length)
{
    /* 固有开销 */
    CFLINT_TYPE *Module   = Temp[1];
    CFLINT_TYPE *Divisor  = Temp[2];
    /* 单独检查:2, 3 */
    Cflint_SetValue(Module, Length, 0);
    /* 单独检查:生成2,匹配2 */
    Module[0] = 2;
    if(Cflint_Equal(X, Module, Length))
        return true;
    /* 单独检查:生成3,匹配3 */
    Module[0] = 3;
    if(Cflint_Equal(X, Module, Length))
        return true;
    /* 解算:Module = X % 6 */
    Cflint_SetValue(Divisor, Length, 0);
    Divisor[0] = 6;
    Cflint_Modulo(Module, X, Divisor, Length);
    /* 验证是否为孪生素数1:(6 * N - 1) % 6 = 5 */
    Divisor[0] = 5;
    if(Cflint_Equal(Divisor, Module, Length))
        return true;
    /* 验证是否为孪生素数2:(6 * N + 1) % 6 = 1 */
    Divisor[0] = 1;
    if(Cflint_Equal(Divisor, Module, Length))
        return true;
    return false;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/