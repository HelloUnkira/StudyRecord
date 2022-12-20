/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "Cflint.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#ifdef EXTEND_USE_MENTGOMERY
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 蒙哥马利约减(X,R,N,NP) */
/* X,Temp1,Temp2长度为Length*2 */
void Mentgomery_ModuloReduction(Cflint_Type *Result, uint32_t Length, int64_t R,
                                Cflint_Type *X, Cflint_Type *N, Cflint_Type *NP,
                                Cflint_Type *Temp1, Cflint_Type *Temp2)
{
    /* 1.计算:m = (((X % R) * NP) % R */
    Cflint_Copy(Temp2, X, Length * 2);
    Cflint_Mask2(Temp2, Length * 2, R);
    Cflint_Multiply(Temp1, Temp2, NP, Length);
    Cflint_Mask2(Temp1, Length * 2, R);
    /* 2.计算:y = (X + m * N) / R */
    Cflint_Multiply(Temp2, Temp1, N, Length);
    Cflint_Addition(Temp1, X, Temp2, Length * 2);
    Cflint_ShiftRight2(Temp1, Length * 2, R);
    /* 3.y > N, y -= N */
    if (Cflint_Compare(Temp1, N, Length) == 1)
        Cflint_Subtraction(Temp1, Temp1, N, Length);
    /* 4.拷贝结果即可 */
    Cflint_Copy(Result, Temp1, Length);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 蒙哥马利模乘(X,Y,N,NP,R_N) */
/* Temp1,Temp2,Temp3,Temp4长度为Length*2 */
void Mentgomery_ModuloMultiply(Cflint_Type *Result, Cflint_Type *X, Cflint_Type *Y,
                               Cflint_Type *N, Cflint_Type *NP, Cflint_Type *RR_N,
                               Cflint_Type *Temp1, Cflint_Type *Temp2,
                               Cflint_Type *Temp3, Cflint_Type *Temp4,
                               int64_t R, uint32_t Length)
{
    /* 1.计算X' = REDC(X * RR_N),Y' = REDC(Y * RR_N) */
    Cflint_Multiply(Temp3, X, RR_N, Length);
    Cflint_Multiply(Temp4, Y, RR_N, Length);
    Mentgomery_ModuloReduction(Temp3, Length, R, Temp3, N, NP, Temp1, Temp2);
    Mentgomery_ModuloReduction(Temp4, Length, R, Temp4, N, NP, Temp1, Temp2);
    /* 3.计算Z = X’·Y’ */
    Cflint_Multiply(Temp1, Temp3, Temp4, Length);
    /* 4.计算Z’=REDC(Z),Z’’=REDC(Z’) */
    Mentgomery_ModuloReduction(Temp2, Length, R, Temp1, N, NP, Temp3, Temp4);
    Mentgomery_ModuloReduction(Temp1, Length, R, Temp2, N, NP, Temp3, Temp4);
    /* 5.拷贝结果 */
    Cflint_Copy(Result, Temp1, Length);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
