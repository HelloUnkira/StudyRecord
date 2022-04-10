#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_MENTGOMERY
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 蒙哥马利约减(X,R,N,NP) */
/* X,Temp1,Temp2长度为Length*2 */
void Cflint_M_ModuloReduction(CFLINT_TYPE *Result, uint32_t Length, int64_t R,
                              CFLINT_TYPE *X, CFLINT_TYPE *N, CFLINT_TYPE *NP,
                              CFLINT_TYPE *Temp1, CFLINT_TYPE *Temp2)
{
    /* 1.计算:m = ((X * NP) % R */
    Cflint_Multiply(Temp1, X, NP, Length);
    Cflint_Modulo2(Temp1, Length * 2, R);
    /* 2.计算:y = (X + m * N) / R */
    Cflint_Multiply(Temp2, Temp1, N, Length);
    Cflint_Addition(Temp1, X, Temp2, Length * 2);
    {
        uint32_t Index = 0;
        printf("\n---------------------------------------------------------------");
        printf("\n!!Temp1:");
        for (Index = 0; Index < Length * 2; Index++)
            printf("%u ", Temp1[Index]);
        printf("\n---------------------------------------------------------------");
    }
    Cflint_ShiftRight2(Temp1, Length * 2, R);
    /* 3.y > N, y -= N */
    if (Cflint_Compare(Temp1, N, Length) == 1)
        Cflint_Subtraction(Temp1, Temp1, N, Length);
    /* 4.拷贝结果即可 */
    Cflint_Copy(Result, Temp1, Length);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 蒙哥马利模乘(X,Y,N,NP,R_N) */
/* Temp1,Temp2,Temp3,Temp4长度为Length*2 */
void Cflint_M_ModuloMultiply(CFLINT_TYPE *Result, CFLINT_TYPE *X, CFLINT_TYPE *Y,
                             CFLINT_TYPE *N, CFLINT_TYPE *NP, CFLINT_TYPE *R_N,
                             CFLINT_TYPE *Temp1, CFLINT_TYPE *Temp2,
                             CFLINT_TYPE *Temp3, CFLINT_TYPE *Temp4,
                             int64_t R, uint32_t Length)
{
    /* 1.计算X' = (X * R_N) % R,Y' = (Y * R_N) % R */
    Cflint_Multiply(Temp3, X, R_N, Length);
    Cflint_Multiply(Temp4, Y, R_N, Length);
    Cflint_Modulo2(Temp3, Length * 2, R);
    Cflint_Modulo2(Temp4, Length * 2, R);
    /* 3.计算Z = X’·Y’ */
    Cflint_Multiply(Temp1, Temp3, Temp4, Length);
    /* 4.计算Z’=REDC(Z),Z’’=REDC(Z’) */
    Cflint_M_ModuloReduction(Temp2, Length, R, Temp1, N, NP, Temp3, Temp4);
    Cflint_M_ModuloReduction(Temp1, Length, R, Temp2, N, NP, Temp3, Temp4);
    /* 5.拷贝结果 */
    Cflint_Copy(Result, Temp1, Length);
    
    {
        uint32_t Index = 0;
        printf("\n---------------------------------------------------------------");
        printf("\nResult:");
        for (Index = 0; Index < Length * 2; Index++)
            printf("%u ", Result[Index]);
        printf("\nX:");
        for (Index = 0; Index < Length * 2; Index++)
            printf("%u ", X[Index]);
        printf("\nY:");
        for (Index = 0; Index < Length * 2; Index++)
            printf("%u ", Y[Index]);
        printf("\nN:");
        for (Index = 0; Index < Length * 2; Index++)
            printf("%u ", N[Index]);
        printf("\nNP:");
        for (Index = 0; Index < Length * 2; Index++)
            printf("%u ", NP[Index]);
        printf("\nR_N:");
        for (Index = 0; Index < Length * 2; Index++)
            printf("%u ", R_N[Index]);
        printf("\nR:%d", R);
        printf("\n-------------------------------------------------------------\n");
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 蒙哥马利模幂 */
bool Cflint_M_ModuloExponent(CFLINT_TYPE *Result,  CFLINT_TYPE *Module,
                             CFLINT_TYPE *Operand, CFLINT_TYPE *Exponent,
                             CFLINT_TYPE *Temp1,   CFLINT_TYPE *Temp2,
                             CFLINT_TYPE *Temp3,   CFLINT_TYPE *Temp4,
                             CFLINT_TYPE *Temp5,   CFLINT_TYPE *Temp6,
                             CFLINT_TYPE *Temp7,   CFLINT_TYPE *Temp8,
                             CFLINT_TYPE *Temp9,   CFLINT_TYPE *Temp10,
                                uint32_t  Length)
{
    /* 固有空间开销 */
    CFLINT_TYPE *NP  = Temp5;
    CFLINT_TYPE *R_N = Temp6;
    /* 特例:除数为0检查 */
    if (Cflint_IsZero(Module, Length) == true) {
        Cflint_SetValue(Result, Length, 0);
        return false;
    }
    /* 1.计算R */
    int64_t R = Cflint_Numbers2(Module, Length) + 1;
    /* 一个数的0次幂为1 */
    if (R == 0) {
        Cflint_SetValue(Result, Length, 0);
        Cflint_AdditionBit(Result, Length, 1);
        return true;
    }
    /* 2.生成R */
    Cflint_SetValue(R_N, Length * 2, 0);
    Cflint_AdditionBit(R_N, Length * 2, 1);
    Cflint_ShiftLeft2(R_N, Length * 2, R);
    /* 3.检查R与Module的互素情况 */
    Cflint_SetValue(Temp1, Length * 2, 0);
    Cflint_Copy(Temp1, Module, Length);
    if (Cflint_GCD(R_N, Temp1, Length * 2, Temp2, Temp3, Temp4) == false) {
        Cflint_SetValue(Result, Length, 0);
        return false;
    }
    /* 4.计算NP */
    Cflint_ModuloInverse(NP, Module, R_N, Temp1, Temp2, Temp3, Temp4, Length);
    
    {
        Cflint_Multiply(Temp1, NP, R_N, Length);
        Cflint_SetValue(Temp2, Length * 2, 0);
        Cflint_Copy(Temp2, Module, Length);
        
        
        uint32_t Index = 0;
        printf("\n---------------------------------------------------------------");
        printf("\nNP:");
        for (Index = 0; Index < Length * 2; Index++)
            printf("%u ", NP[Index]);
        printf("\nR_N:");
        for (Index = 0; Index < Length * 2; Index++)
            printf("%u ", R_N[Index]);
        printf("\nModule:");
        for (Index = 0; Index < Length * 2; Index++)
            printf("%u ", Module[Index]);
        printf("\n(NP * R_N):");
        for (Index = 0; Index < Length * 2; Index++)
            printf("%u ", Temp1[Index]);
        Cflint_Modulo(Temp1, Temp1, Temp2, Length * 2);
        printf("\n(NP * R_N)%Module:");
        for (Index = 0; Index < Length * 2; Index++)
            printf("%u ", Temp1[Index]);
        printf("\n---------------------------------------------------------------");
    }
    
    /* 5.计算R%N */
    Cflint_SetValue(Temp2, Length * 2, 0);
    Cflint_Copy(Temp2, Module, Length);
    Cflint_Modulo(R_N, R_N, Temp2, Length * 2);
    /* 6.快速平方模乘 */
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
            Cflint_M_ModuloMultiply(Temp4, Temp3, Temp3, Module, NP, R_N,
                                    Temp7, Temp8, Temp9, Temp10, R, Length);
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
                Cflint_M_ModuloMultiply(Temp4, Temp1, Temp3, Module, NP, R_N,
                                        Temp7, Temp8, Temp9, Temp10, R, Length);
                Cflint_Modulo(Temp1, Temp4, Temp2, Length * 2);
            }
            Cflint_M_ModuloMultiply(Temp4, Temp3, Temp3, Module, NP, R_N,
                                    Temp7, Temp8, Temp9, Temp10, R, Length);
            Cflint_Modulo(Temp3, Temp4, Temp2, Length * 2);
        }
        Cflint_Copy(Result, Temp1, Length);
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
