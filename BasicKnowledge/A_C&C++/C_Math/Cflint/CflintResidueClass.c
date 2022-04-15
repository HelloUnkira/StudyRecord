#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_RESIDUECLASS
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 欧几里得算法:GCD(A,B) == 1 */
bool Cflint_GCDCheck(CFLINT_TYPE *A, CFLINT_TYPE *B, uint32_t Length,
                     CFLINT_TYPE *Temp1, CFLINT_TYPE *Temp2, CFLINT_TYPE *Temp3)
{
    /* 固有开销 */
    CFLINT_TYPE *Module   = Temp1;
    CFLINT_TYPE *Dividend = Temp2;
    CFLINT_TYPE *Divisor  = Temp3;
    /* 将A,B拷贝到缓冲区,数据计算在缓冲区内 */
    Cflint_Copy(Dividend, A, Length);
    Cflint_Copy(Divisor,  B, Length);
    /* 当B为0时 */
    if (Cflint_IsZero(B, Length) == true)
        return false;
    /* 进行向下递归 */
    do {
        /* Module = Dividend % Divisor */
        Cflint_Modulo(Module, Dividend, Divisor, Length);
        /* 更新被除数和除数 */
        Cflint_Copy(Dividend, Divisor, Length);
        Cflint_Copy(Divisor,  Module,  Length);
        /* 当模为0的时候结束,用于回溯递归 */
        if (Cflint_IsZero(Divisor, Length) == true) {
            /* 处理这个模为0的时候的结果 */
            Cflint_AdditionBit(Divisor, Length, 1);
            /* 检查除数是否为1 */
            return Cflint_Equal(Dividend, Divisor, Length);
        }
    } while (1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 欧几里得算法:GCD(A,B) */
void Cflint_GCD(CFLINT_TYPE *Result, CFLINT_TYPE *A,     CFLINT_TYPE *B,
                CFLINT_TYPE *Temp1,  CFLINT_TYPE *Temp2, CFLINT_TYPE *Temp3,
                   uint32_t  Length)
{
    /* 固有开销 */
    CFLINT_TYPE *Module   = Temp1;
    CFLINT_TYPE *Dividend = Temp2;
    CFLINT_TYPE *Divisor  = Temp3;
    /* 将A,B拷贝到缓冲区,数据计算在缓冲区内 */
    Cflint_Copy(Dividend, A, Length);
    Cflint_Copy(Divisor,  B, Length);
    /* 当B为0时 */
    if (Cflint_IsZero(B, Length) == true)
        return false;
    /* 进行向下递归 */
    do {
        /* Module = Dividend % Divisor */
        Cflint_Modulo(Module, Dividend, Divisor, Length);
        /* 更新被除数和除数 */
        Cflint_Copy(Dividend, Divisor, Length);
        Cflint_Copy(Divisor,  Module,  Length);
        /* 当模为0的时候结束,用于回溯递归 */
        if (Cflint_IsZero(Divisor, Length) == true) {
            /* 拷贝被除数 */
            Cflint_Copy(Result, Dividend, Length);
            return;
        }
    } while (1);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 扩展欧几里得算法:A*X - B*Y = 1 */
/* Flag为0表示正数,Flag为1表示负数 */
void Cflint_ExtendGCD(CFLINT_TYPE *A, CFLINT_TYPE *B, CFLINT_TYPE *X, CFLINT_TYPE *Y,
                      CFLINT_TYPE *Result, CFLINT_TYPE *X_Flag, CFLINT_TYPE *Y_Flag,
                      CFLINT_TYPE *Temp1, CFLINT_TYPE *Temp2, CFLINT_TYPE *Temp3,
                      CFLINT_TYPE *Temp4, CFLINT_TYPE *Temp5, uint32_t Length)
{
    /*案例代码:
     *uint64_t extend_gcd(uint64_t a, uint64_t b, uint64_t &x, uint64_t &y)
     *{
     *    if (b == 0) {
     *        x = 1;
     *        y = 0;
     *        return a;
     *    } else {
     *        uint64_t q   = a / b;
     *        uint64_t m   = a % b;
     *        uint64_t gcd = extend_gcd(b, m, x, y);
     *        uint64_t t = x; x = y; y = t;
     *        y = y - q * x;
     *        return gcd;
     *    }
     *}
    */
    
    /* 固有开销 */
    CFLINT_TYPE *Quotient = Temp1;
    CFLINT_TYPE *Module   = Temp2;
    CFLINT_TYPE *Dividend = Temp3;
    CFLINT_TYPE *Divisor  = Temp4;
    CFLINT_TYPE *Temp     = Temp5;
    /* 初始化:X=1,Y=0 */
    *X_Flag = 0; *Y_Flag = 0;
    Cflint_SetValue(Y, (Length + 1) * 2, 0);
    Cflint_SetValue(X, (Length + 1) * 2, 0);
    Cflint_AdditionBit(X, (Length + 1) * 2, 1);
    /* 当B为0时 */
    if (Cflint_IsZero(B, Length) == true)
        return;
    /* 将A,B拷贝到缓冲区,数据计算在缓冲区内 */
    Cflint_Copy(Dividend, A, Length);
    Cflint_Copy(Divisor,  B, Length);
    /* 先进行向下递归 */
    do {
        /* Module = Dividend % Divisor */
        Cflint_Modulo(Module, Dividend, Divisor, Length);
        /* 当模为0的时候结束,停在此处的除数用于回溯 */
        if (Cflint_IsZero(Module, Length) == true) {
            Cflint_Copy(Result, Divisor, Length);
            break;
        }
        /* 更新被除数和除数 */
        Cflint_Copy(Dividend, Divisor, Length);
        Cflint_Copy(Divisor,  Module,  Length);
    } while (1);
    /* 再进行向上回溯 */
    do {
        /* 在已知除数和模的情况下无法解析出被除数和商 */
        /* 为了降低其栈开销,暂时只能从上往下再算一遍 */
        /* 这是以CPU换内存的做法,临时想不到别的策略 */
        Cflint_Copy(Temp, Divisor, Length);
        /* 将A,B拷贝到缓冲区,数据计算在缓冲区内 */
        Cflint_Copy(Dividend, A, Length);
        Cflint_Copy(Divisor,  B, Length);
        do {
            /* Quotient= Dividend / Divisor */
            /* Module  = Dividend % Divisor */
            Cflint_Devide(Quotient, Module, Dividend, Divisor, Length);
            /* 当除数匹配时退出 */
            if (Cflint_Equal(Temp, Divisor, Length) == 1)
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
        Cflint_Swap(X, Y, (Length + 1) * 2);
        Cflint_Multiply(Temp, Quotient, X, Length);
        /* 合并场景:符号向异(合并成和运算) */
        if ((*X_Flag == 1 && *Y_Flag == 0) ||
            (*X_Flag == 0 && *Y_Flag == 1)) {
            Cflint_Addition(Temp, Y, Temp, (Length + 1) * 2);
        }
        /* 分立场景:符号向同(分立成差运算) */
        if (*X_Flag == 1 && *Y_Flag == 1) {
            if (Cflint_Compare(Temp, Y, Length) != -1) {
                Cflint_Subtraction(Temp, Temp, Y, (Length + 1) * 2);
                *Y_Flag = 0;
            } else {
                Cflint_Subtraction(Temp, Y, Temp, (Length + 1) * 2);
            }
        }
        /* 分立场景:符号向同(分立成差运算) */
        if (*X_Flag == 0 && *Y_Flag == 0) {
            if (Cflint_Compare(Y, Temp, Length) != -1) {
                Cflint_Subtraction(Temp, Y, Temp, (Length + 1) * 2);
            } else {
                Cflint_Subtraction(Temp, Temp, Y, (Length + 1) * 2);
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
/* 备注:我看不懂这个顺向推导的数学逻辑,只知道其逆向推导 */
/* 扩展欧几里得算法(Euclidean):A*X + B*Y = 1 */
void Cflint_ExGCD(CFLINT_TYPE *A,      CFLINT_TYPE *B,
                  CFLINT_TYPE *X,      CFLINT_TYPE *Y,
                  CFLINT_TYPE *X_Flag, CFLINT_TYPE *Y_Flag,
                  CFLINT_TYPE *Temp1,  CFLINT_TYPE *Temp2, CFLINT_TYPE *Temp3,
                  CFLINT_TYPE *Temp4,  CFLINT_TYPE *Temp5, CFLINT_TYPE *Temp6,
                  CFLINT_TYPE *Temp7,  CFLINT_TYPE *Temp8,    uint32_t  Length)
{
    /* 固有开销 */
    CFLINT_TYPE *Quotient = Temp1;
    CFLINT_TYPE *Module   = Temp2;
    CFLINT_TYPE *Dividend = Temp3;
    CFLINT_TYPE *Divisor  = Temp4;
    CFLINT_TYPE *VV = Temp5; CFLINT_TYPE VV_Flag = 0;
    CFLINT_TYPE *TT = Temp6; CFLINT_TYPE TT_Flag = 0;
    int8_t CompareResult = 0;
    /* 1.初始化:X=1,Y=0 */
    *X_Flag = 0; *Y_Flag = 0;
    Cflint_SetValue(Y, (Length + 1) * 2, 0);
    Cflint_SetValue(X, (Length + 1) * 2, 0);
    Cflint_AdditionBit(X, (Length + 1) * 2, 1);
    /* 2.初始化:Dividend=A,Divisor=B,VV=1 */
    Cflint_SetValue(VV, (Length + 1) * 2, 0);
    Cflint_SetValue(Dividend, (Length + 1) * 2, 0);
    Cflint_SetValue(Divisor,  (Length + 1) * 2, 0);
    Cflint_Copy(Dividend, A, Length);
    Cflint_Copy(Divisor,  B, Length);
    /* 3.初始化除数为0检查 */
    if (Cflint_IsZero(Divisor, Length) == true)
        return;
    /* 开始主循环直到除数为0 */
    while (Cflint_IsZero(Divisor, Length) == false) {
        /* Quotient = Dividend / Divisor */
        /* Module   = Dividend % Divisor */
        Cflint_Devide(Quotient, Module, Dividend, Divisor, Length);
        /* TT = X - Quotient * VV */
        Cflint_Multiply(Temp7, Quotient, VV, Length);
        /* Temp7 = X * (*X_Flag) - Temp7(Quotient * VV) * (VV_Flag) */
        {
            if (*X_Flag == 0 && VV_Flag == 0) {
                /* TT = X - Temp7(Quotient * VV) */
                CompareResult = Cflint_Compare(X, Temp7, Length * 2);
                if (CompareResult != -1) {
                    Cflint_Subtraction(Temp7, X, Temp7, (Length + 1) * 2);
                    TT_Flag = 0;
                }
                if (CompareResult == -1) {
                    Cflint_Subtraction(Temp7, Temp7, X, (Length + 1) * 2);
                    TT_Flag = 1;
                }
            }
            if (*X_Flag == 1 && VV_Flag == 1) {
                /* TT = Temp7(Quotient * VV) - X */
                CompareResult = Cflint_Compare(Temp7, X, Length * 2);
                if (CompareResult != -1) {
                    Cflint_Subtraction(Temp7, Temp7, X, (Length + 1) * 2);
                    TT_Flag = 0;
                }
                if (CompareResult == -1) {
                    Cflint_Subtraction(Temp7, X, Temp7, (Length + 1) * 2);
                    TT_Flag = 1;
                }
            }
            if ((*X_Flag == 1 && VV_Flag == 0) ||
                (*X_Flag == 0 && VV_Flag == 1)) {
                /* TT = (*X_Flag)*(X + Temp7(Quotient * VV)) */
                Cflint_Addition(Temp7, X, Temp7, (Length + 1) * 2);
                TT_Flag = *X_Flag;
            }
        }
        /* TT = Temp7 */
        Cflint_Copy(TT, Temp7, (Length + 1) * 2);
        /* X = VV, *X_Flag = VV_Flag */
        Cflint_Copy(X, VV, (Length + 1) * 2);
        *X_Flag = VV_Flag;
        /* Dividend = Divisor */
        /* Divisor  = Module  */
        Cflint_Copy(Dividend, Divisor, Length);
        Cflint_Copy(Divisor,  Module,  Length);
        /* VV = TT, VV_Flag = TT_Flag */
        Cflint_Copy(VV, TT, (Length + 1) * 2);
        VV_Flag = TT_Flag;
    }
    /* Y = (Dividend - A * X) / B */
    Cflint_SetValue(Temp7, (Length + 1) * 2, 0);
    Cflint_Copy(Temp7, Dividend, Length);
    Cflint_Multiply(Temp8, A, X, Length);
    /* Y = Temp7(Dividend) - Temp8(A * X) * (*X_Flag) */
    {
        if (*X_Flag == 0) {
            CompareResult = Cflint_Compare(Temp7, Temp8, Length * 2);
            if (CompareResult != -1) {
                /* Temp7 = +(Temp7(Dividend) - Temp8(A * X)) */
                Cflint_Subtraction(Temp7, Temp7, Temp8, (Length + 1) * 2);
                *Y_Flag = 0;
            }
            if (CompareResult == -1) {
                /* Temp7 = -(Temp8(A * X) - Temp7(Dividend)) */
                Cflint_Subtraction(Temp7, Temp8, Temp7, (Length + 1) * 2);
                *Y_Flag = 1;
            }
        }
        if (*X_Flag == 1) {
            /* Temp7 = +(Temp7(Dividend) + Temp8(A * X)) */
            Cflint_Addition(Temp7, Temp7, Temp8, (Length + 1) * 2);
            *Y_Flag = 0;
        }
    }
    /* Temp8 = B */
    Cflint_SetValue(Temp8, (Length + 1) * 2, 0);
    Cflint_Copy(Temp8, B, Length);
    /* Y = Temp7 / Temp8 */
    Cflint_Devide(Quotient, Module, Temp7, Temp8, (Length + 1) * 2);
    Cflint_Copy(Y, Quotient, (Length + 1) * 2);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 最小公倍数:LCM(A,B) = A * B / GCD(A,B) */
void Cflint_LCM(CFLINT_TYPE *Result, CFLINT_TYPE *A, CFLINT_TYPE *B,
                CFLINT_TYPE *Temp1,  CFLINT_TYPE *Temp2,
                CFLINT_TYPE *Temp3,  CFLINT_TYPE *Temp4,
                CFLINT_TYPE *Temp5,     uint32_t  Length)
{
    /* 1.计算A*B */
    Cflint_Multiply(Temp4, A, B, Length);
    Cflint_SetValue(Temp5, Length * 2, 0);
    /* 2.计算GCD(A,B); */
    Cflint_GCD(Temp5, A, B, Temp1, Temp2, Temp3, Length);
    /* 3.计算A * B / GCD(A,B) */
    Cflint_Devide(Temp1, Temp2, Temp4, Temp5, Length * 2);
    /* 4.得出结果 */
    Cflint_Copy(Result, Temp1, Length * 2);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
