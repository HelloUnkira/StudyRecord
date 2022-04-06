#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_BASE
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 蒙哥马利模加 */
void Cflint_ModuloAddition(CFLINT_TYPE *Result,   CFLINT_TYPE *Module,
                           CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                              uint32_t  Length)
{
    /* 模为0检查 */
    if (Cflint_IsZero(Module, Length) == true)
        return;
    
    /*蒙哥马利模加:
     *当Operand0 <= Module && Operand1 <= Module时
     *Result = (Operand0 + Operand1) % Module;
     */

    /* 计算和运算 */
    CFLINT_TYPE Overflow = Cflint_Addition(Result, Operand0, Operand1, Length);
    if (Overflow != 0)
        Cflint_Subtraction(Result, Result, Module, Length);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 蒙哥马利模减 */
void Cflint_ModuloSubtraction(CFLINT_TYPE *Result,   CFLINT_TYPE *Module,
                              CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                                 uint32_t  Length)
{
    /* 模为0检查 */
    if (Cflint_IsZero(Module, Length) == true)
        return;
    /*蒙哥马利模减:
     *当Operand0 <= Module && Operand1 <= Module时
     *Result = (Operand0 - Operand1) % Module;
     */
    CFLINT_TYPE Overflow = Cflint_Subtraction(Result, Operand0, Operand1, Length);
    if (Overflow != 0)
            Cflint_Addition(Result, Result, Result, Length);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 蒙哥马利约减 */
/* T,Temp1,Temp2长度为Length*2 */
void Cflint_ModuloReduction(CFLINT_TYPE *Result, uint32_t Length, int64_t R,
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
void Cflint_ModuloMultiply(CFLINT_TYPE *Result,   CFLINT_TYPE *Module,
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
    Cflint_ModuloReduction(Result, Length, R, Temp3, N, Np, Temp1, Temp2);
    Cflint_Copy(Temp3, Result, Length);
    Cflint_ModuloReduction(Result, Length, R, Temp4, N, Np, Temp1, Temp2);
    Cflint_Copy(Temp4, Result, Length);
    /* 3.计算Z’=REDC(x’·y’) */
    Cflint_Multiply(Temp1, Temp3, Temp4, Length);
    /* 4.计算Z’=REDC(x’·y’),Z=REDC(Z’) */
    Cflint_ModuloReduction(Temp2, Length, R, Temp1, N, Np, Temp3, Temp4);
    Cflint_ModuloReduction(Temp1, Length, R, Temp2, N, Np, Temp3, Temp4);
    /* 5.拷贝结果 */
    Cflint_Copy(Result, Temp1, Length);
}
/* 蒙哥马利模幂 */
void Cflint_ModuloExponent(CFLINT_TYPE *Result,  CFLINT_TYPE *Module,
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







}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模幂运算 */
void Cflint_ModuloExp(CFLINT_TYPE *Result,  CFLINT_TYPE *Module,
                      CFLINT_TYPE *Operand, CFLINT_TYPE *Exponent,
                      CFLINT_TYPE *Temp1,   CFLINT_TYPE *Temp2,
                      CFLINT_TYPE *Temp3,   CFLINT_TYPE *Temp4,
                         uint32_t  Length)
{
    /* 特例:除数为0检查 */
    if (Cflint_IsZero(Module, Length) == true)
        return;
    /* 初始化Temp,清除空间 */
    Cflint_SetValue(Temp1, Length * 2, 0);  //保留Result
    Cflint_SetValue(Temp2, Length * 2, 0);  //保留Module
    Cflint_SetValue(Temp3, Length * 2, 0);  //保留Operand
    Cflint_SetValue(Temp4, Length * 2, 0);  //保留Operand
    /* 初始化Result,数值为1 */
    Cflint_AdditionBit(Temp1, Length, 1);
    Cflint_Copy(Temp2, Module, Length);
    Cflint_Modulo(Temp3, Operand, Module, Length);
    /* 计算有效位数个数 */
    int64_t Numbers = Cflint_Numbers2(Exponent, Length);
    /* 计算1的个数用于确定,是否存在2的幂次方 */
    bool NumbersOnlyOne = Cflint_IsExponent2(Exponent, Length);
    /* 特殊优化场景:指数为2的幂 */
    if (NumbersOnlyOne == true) {
        /* 一个数的0次幂为1 */
        if (Numbers == -1) {
            Cflint_Copy(Result, Temp1, Length);
            return;
        }
        /* 一个数的1次幂为本身 */
        if (Numbers == 0) {
            Cflint_Modulo(Temp1, Temp3, Temp2, Length * 2);
            Cflint_Copy(Result, Temp1, Length);
            return;
        }
        for (int64_t Bits2 = 1; Bits2 <= Numbers; Bits2++) {
            Cflint_Square(Temp4, Temp3, Length);
            Cflint_Copy(Temp3, Temp4, Length);
            Cflint_Modulo(Temp1, Temp3, Temp2, Length * 2);
        }
        Cflint_Copy(Result, Temp1, Length);
        return;
    }
    /* 普通场景 */
    if (NumbersOnlyOne == false) {
        for (int64_t Bits2 = 0; Bits2 <= Numbers; Bits2++) {
            /* 索引计算 */
            uint32_t BitN = Bits2 / CFLINT_BITS;
            uint32_t Bit2 = Bits2 % CFLINT_BITS;
            /* 逆向迭代 */
            if ((Exponent[BitN] & (1 << Bit2)) != 0) {
                Cflint_Multiply(Temp4, Temp1, Temp3, Length);
                Cflint_Modulo(Temp1, Temp4, Temp2, Length * 2);
            }
            Cflint_Square(Temp4, Temp3, Length);
            Cflint_Modulo(Temp3, Temp4, Temp2, Length * 2);
        }
        Cflint_Copy(Result, Temp1, Length);
        return;
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模逆运算 */
void Cflint_ModuloInv(CFLINT_TYPE *Result,  CFLINT_TYPE *Module,
                      CFLINT_TYPE *Operand, CFLINT_TYPE *Temp1,
                      CFLINT_TYPE *Temp2,   CFLINT_TYPE *Temp3,
                      CFLINT_TYPE *Temp4,      uint32_t  Length)
{
    /* 固有额外空间开销 */
    CFLINT_TYPE *A = Temp1;
    CFLINT_TYPE *B = Temp2;
    CFLINT_TYPE *U = Temp3;
    CFLINT_TYPE *V = Temp4;

    /* 1.除数为0检查 */
    if (Cflint_IsZero(Operand, Length) == true) {
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
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
