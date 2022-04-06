#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_RESIDUECLASS
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 蒙哥马利约减 */
/* T,Temp1,Temp2长度为Length*2 */
void Cflint_M_ModuloReduction(CFLINT_TYPE *Result, uint32_t Length, int64_t R,
                              CFLINT_TYPE *T, CFLINT_TYPE *N, CFLINT_TYPE *Np,
                              CFLINT_TYPE *Temp1, CFLINT_TYPE *Temp2)
{
    /* 1.计算:m = ((T%R)*Np) % R */
    Cflint_Copy(Temp2, T, Length * 2);
    Cflint_Modulo2(Temp2, Length * 2, R);
    Cflint_Multiply(Temp1, Temp2, Np, Length);
    Cflint_Modulo2(Temp1, Length * 2, R);
    /* 2.计算:t = (T + m * N) % R */
    Cflint_Multiply(Temp2, Temp1, N, Length);
    Cflint_Addition(Temp1, T, Temp2, Length * 2);
    Cflint_Modulo2(Temp1, Length * 2, R);
    /* 3.蒙哥马利模减 */
    if (Cflint_Compare(Temp1, N, Length) != -1)
        Cflint_Subtraction(Temp1, Temp1, N, Length);
    /* 4.拷贝结果即可 */
    Cflint_Copy(Result, Temp1, Length);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 蒙哥马利模乘 */
/* Temp1,Temp2,Temp3,Temp4长度为Length*2 */
void Cflint_M_ModuloMultiply(CFLINT_TYPE *Result,   CFLINT_TYPE *Module,
                             CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                             CFLINT_TYPE *Temp1,    CFLINT_TYPE *Temp2,
                             CFLINT_TYPE *Temp3,    CFLINT_TYPE *Temp4,
                             CFLINT_TYPE *N, CFLINT_TYPE *Np, CFLINT_TYPE *RR_N,
                             int64_t R, uint32_t Length)
{
    /* 1.计算X*RR_N,Y*RR_N */
    Cflint_Multiply(Temp3, Operand0, RR_N, Length);
    Cflint_Multiply(Temp4, Operand1, RR_N, Length);
    /* 2.计算X’=REDC(X·(R·R mod N)),Y’=REDC(X·(R·R mod N)) */
    Cflint_M_ModuloReduction(Result, Length, R, Temp3, N, Np, Temp1, Temp2);
    Cflint_Copy(Temp3, Result, Length);
    Cflint_M_ModuloReduction(Result, Length, R, Temp4, N, Np, Temp1, Temp2);
    Cflint_Copy(Temp4, Result, Length);
    /* 3.计算Z’=REDC(x’·y’) */
    Cflint_Multiply(Temp1, Temp3, Temp4, Length);
    /* 4.计算Z’=REDC(x’·y’),Z=REDC(Z’) */
    Cflint_M_ModuloReduction(Temp2, Length, R, Temp1, N, Np, Temp3, Temp4);
    Cflint_M_ModuloReduction(Temp1, Length, R, Temp2, N, Np, Temp3, Temp4);
    /* 5.拷贝结果 */
    Cflint_Copy(Result, Temp1, Length);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 蒙哥马利模幂 */
void Cflint_M_ModuloExponent(CFLINT_TYPE *Result,  CFLINT_TYPE *Module,
                             CFLINT_TYPE *Operand, CFLINT_TYPE *Exponent,
                             CFLINT_TYPE *RR_N,    CFLINT_TYPE *xxx,
                             CFLINT_TYPE *Temp1,   CFLINT_TYPE *Temp2,
                             CFLINT_TYPE *Temp3,   CFLINT_TYPE *Temp4,
                                uint32_t  Length)
{
    /* 初始化清空 */
    Cflint_SetValue(RR_N, Length * 2, 0);
    /* 1.计算R */
    int64_t R = Cflint_Numbers2(Exponent, Length) + 1;
    /* 一个数的0次幂为1 */
    if (R == 0) {
        Cflint_SetValue(Result, Length, 0);
        Cflint_AdditionBit(Result, Length , 1);
        return;
    }
    /* 2.生成R */
    Cflint_AdditionBit(RR_N, Length * 2, 1);
    Cflint_ShiftLeft2(RR_N, Length * 2, R);
    /* 3.计算RR%N */
    Cflint_Square(Temp1, RR_N, Length);
    Cflint_SetValue(Temp2, Length * 2, 0);
    Cflint_Copy(Temp2, Module, Length);
    Cflint_Modulo(RR_N, Temp1, Temp2, Length * 2);

    /* 未完成,插眼!!! */
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
