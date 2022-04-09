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
    /*案例代码:
     *uint64_t extend_gcd(uint64_t a, uint64_t b, uint64_t &x, uint64_t &y)
     *{
     *    if (b == 0) {
     *        x = 1;
     *        y = 0;
     *        return a;
     *    } else {
     *        uint64_t gcd = extend_gcd(b, a % b, x, y);
     *        uint64_t tmp = x;
     *        x = y;
     *        y = tmp - (a / b) * y;
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
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
