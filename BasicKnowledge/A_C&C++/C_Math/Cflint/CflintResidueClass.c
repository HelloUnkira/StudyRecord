#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_RESIDUECLASS
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 扩展欧几里得算法:A*X - B*Y = 1 */
/* Flag为0表示正数,Flag为1表示负数 */
void Cflint_GCD(CFLINT_TYPE *A, CFLINT_TYPE *B, CFLINT_TYPE *X, CFLINT_TYPE *Y,
                CFLINT_TYPE *Result, CFLINT_TYPE *X_Flag, CFLINT_TYPE *Y_Flag,
                CFLINT_TYPE *Temp1, CFLINT_TYPE *Temp2, CFLINT_TYPE *Temp3,
                CFLINT_TYPE *Temp4, CFLINT_TYPE *Temp5, uint32_t Length)
{
    /* 固有开销 */
    CFLINT_TYPE *Quotient = Temp1;
    CFLINT_TYPE *Module   = Temp2;
    CFLINT_TYPE *Dividend = Temp3;
    CFLINT_TYPE *Divisor  = Temp4;    
    CFLINT_TYPE *Temp     = Temp5;
    /* 初始化:X=1,Y=0 */
    *X_Flag = 0; *Y_Flag = 0;
    Cflint_SetValue(Y, Length, 0);
    Cflint_SetValue(X, Length, 0);
    Cflint_AdditionBit(X, Length, 1);
    /* 当A或B为0时 */
    if (Cflint_IsZero(B, Length) == true)
        return;
    /* 将A,B拷贝到缓冲区,数据计算在缓冲区内 */
    Cflint_Copy(Dividend, A, Length);
    Cflint_Copy(Divisor,  B, Length);
    /* 先进行向下递归 */
    do {
        /* Quotient= Dividend / Divisor */
        /* Module  = Dividend % Divisor */
        Cflint_Devide(Quotient, Module, Dividend, Divisor, Length);
        /* 更新被除数和除数 */
        Cflint_Copy(Dividend, Divisor, Length);
        Cflint_Copy(Divisor,  Module,  Length);
        /* 当模为0的时候结束,用于回溯递归 */
        if (Cflint_IsZero(Module, Length) == true) {
            /* 此时单独处理这个模为0的时候的结果 */
            Cflint_Copy(Result, Dividend, Length);
            break;
        }
    } while (1);
    /* 再进行向上回溯 */
    do {
        /* 在已知除数和模的情况下无法解析出被除数和商 */
        /* 为了降低其栈开销,暂时只能从上往下再算一遍 */
        /* 那么这是以CPU换内存的做法,临时想不到别的策略 */
        Cflint_Copy(Temp, Divisor, Length);
        /* 将A,B拷贝到缓冲区,数据计算在缓冲区内 */
        Cflint_Copy(Dividend, A, Length);
        Cflint_Copy(Divisor,  B, Length);
        do {
            /* Quotient= Dividend / Divisor */
            /* Module  = Dividend % Divisor */
            Cflint_Devide(Quotient, Module, Dividend, Divisor, Length);
            /* 当除数匹配时退出 */
            if (Cflint_Compare(Temp, Divisor, Length) == 0)
                break;
            /* 更新被除数和除数 */
            Cflint_Copy(Dividend, Divisor, Length);
            Cflint_Copy(Divisor,  Module,  Length);
        } while (1);
        /* 使用本级的商去更新X和Y,使用标志位单独表示符号 */
        *X_Flag = *X_Flag ^ *Y_Flag;
        *Y_Flag = *X_Flag ^ *Y_Flag;
        *X_Flag = *X_Flag ^ *Y_Flag;
        /* X<=>Y,X_Flag<=>Y_Flag */
        Cflint_Swap(X, Y, Length);
        Cflint_Multiply(Temp, Quotient, X, Length);
        /* 合并场景:符号向异(合并成和运算) */
        if ((*X_Flag == 1 && *Y_Flag == 0) ||
            (*X_Flag == 0 && *Y_Flag == 1)) {
            Cflint_Addition(Temp, Y, Temp, Length);
        }
        /* 分立场景:符号向同(分立成差运算) */
        if (*X_Flag == 1 && *Y_Flag == 1) {
            if (Cflint_Compare(Temp, Y, Length) == 1) {
                Cflint_Subtraction(Temp, Temp, Y, Length);
                *Y_Flag = 0;
            } else {
                Cflint_Subtraction(Temp, Y, Temp, Length);
            }
        }
        /* 分立场景:符号向同(分立成差运算) */
        if (*X_Flag == 0 && *Y_Flag == 0) {
            if (Cflint_Compare(Y, Temp, Length) != -1) {
                Cflint_Subtraction(Temp, Y, Temp, Length);
            } else {
                Cflint_Subtraction(Temp, Temp, Y, Length);
                *Y_Flag = 1;
            }
        }
        /* 场景结束,更新Y */
        Cflint_Copy(Y, Temp, Length);
        /* 如果达到退出条件,退出 */
        if (Cflint_Equal(A, Dividend, Length) == 1 &&
            Cflint_Equal(B, Divisor,  Length) == 1)
            break;
        /* 还原上一级的除数 */
        Cflint_Copy(Divisor, Dividend, Length);
    } while (1);
}
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
