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
/* 强素数判别检查 */
bool Cflint_IsPrime(CFLINT_TYPE *X, CFLINT_TYPE *Temp[8], uint32_t Length,
                    uint32_t SizerTable1[], uint32_t SizerLength1,
                    uint32_t SizerTable2[], uint32_t SizerLength2)
{
    CFLINT_TYPE *Dividend = Temp[0];
    CFLINT_TYPE *Divisor  = Temp[1];
    CFLINT_TYPE *Module   = Temp[2];
    CFLINT_TYPE *Exponent = Temp[3];
    CFLINT_TYPE *Temp_1   = Temp + 4;   //Temp[4~7]:Length * 2
    /* 1.奇偶判断检查 */
    if (Cflint_IsEven(X, Length))
        return false;
    /* 2.初级素数筛查 */
    Cflint_SetValue(Dividend, Length, 0);
    Cflint_Copy(Dividend, X, Length);
    for (uint32_t Index = 0; Index < SizerLength1; Index++) {
        Cflint_SetValue(Divisor, Length, 0);
        Cflint_SetValue(Module, Length, 0);
        /* 设置除数 */
#if 0
#elif CFLINT_BYTE == 1
        Divisor[0] = SizerTable1[Index] >>  0;
        Divisor[1] = SizerTable1[Index] >>  8;
        Divisor[2] = SizerTable1[Index] >> 16;
        Divisor[3] = SizerTable1[Index] >> 24;
#elif CFLINT_BYTE == 2
        Divisor[0] = SizerTable1[Index] >>  0;
        Divisor[1] = SizerTable1[Index] >> 16;
#else
        Cflint_AdditionBit(Divisor, Length, SizerTable1[Index]);
#endif
        /* 计算模,如果能被整除则通过当次素性测试 */
        Cflint_Modulo(Module, Dividend, Divisor, Length);
        if (Cflint_IsZero(Module, Length))
            return false;
    }
    /* 3.高级素数筛查(Miller_Rabin测试) */
    for (uint32_t Index = 0; Index < SizerLength2; Index++) {
        Cflint_SetValue(Divisor, Length, 0);
        Cflint_SetValue(Module, Length, 0);
        /* 设置除数 */
#if 0
#elif CFLINT_BYTE == 1
        Divisor[0] = SizerTable2[Index] >>  0;
        Divisor[1] = SizerTable2[Index] >>  8;
        Divisor[2] = SizerTable2[Index] >> 16;
        Divisor[3] = SizerTable2[Index] >> 24;
#elif CFLINT_BYTE == 2
        Divisor[0] = SizerTable2[Index] >>  0;
        Divisor[1] = SizerTable2[Index] >> 16;
#else
        Cflint_AdditionBit(Divisor, Length, SizerTable2[Index]);
#endif
        /* 设置初始指数为:P - 1 */
        Cflint_Copy(Exponent, X, Length);
        Cflint_SubtractionBit(Exponent, Length, 1);
        /* 开始测试流程 */
        while (1) {
            /* Module   = Result */
            /* Dividend = P */
            /* Divisor  = X */
            /* Exponent = D(奇数) * 2 ** K */
            /* 测试 X ^ ((P - 1) == D(奇数) * 2 ** K) Mod P */
            Cflint_ModuloExponent(Module, Dividend, Divisor,
                                  Exponent, Temp_1, Length);
            bool TestCheck = false;
            /* 检查模是否为 1 */
            if (!TestCheck) {
                Cflint_SubtractionBit(Module, Length, 1);
                if (Cflint_IsZero(Module, Length))
                    TestCheck = true;
                Cflint_AdditionBit(Module, Length, 1);
                if (!TestCheck)
                    return false;
            }
            /* 检查模是否为 P - 1 */
            if (!TestCheck) {
                Cflint_AdditionBit(Module, Length, 1);
                if (Cflint_Compare(Module, Dividend, Length) == 0)
                    TestCheck = true;
                Cflint_SubtractionBit(Module, Length, 1);
                if (!TestCheck)
                    return false;
            }
            /* 通过校验后检查指数,指数为奇数则退出当次校验 */
            if (!Cflint_IsEven(Exponent, Length))
                break;
            /* 指数降一个二次幂,继续进行当次校验 */
            Cflint_ShiftRight2(Exponent, Length, 1);
        }
    }
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