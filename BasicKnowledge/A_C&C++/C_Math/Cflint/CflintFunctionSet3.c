#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_FUNCTION_SET3
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 最大公因数为1检查(互素检查) */
bool Cflint_GCDCheck(CFLINT_TYPE *A,     CFLINT_TYPE *B,
                     CFLINT_TYPE *Temp1, CFLINT_TYPE *Temp2,
                     CFLINT_TYPE *Temp3,    uint32_t  Length)
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
/* 最大公因数 */
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
        return;
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
/* 最小公倍数 */
void Cflint_LCM(CFLINT_TYPE *Result, CFLINT_TYPE *A,     CFLINT_TYPE *B,
                CFLINT_TYPE *Temp1,  CFLINT_TYPE *Temp2, CFLINT_TYPE *Temp3,
                CFLINT_TYPE *Temp4,  CFLINT_TYPE *Temp5,    uint32_t  Length)
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
/* 扩展欧几里得算法 */
void Cflint_GCDExtend(CFLINT_TYPE *A,      CFLINT_TYPE *B,
                      CFLINT_TYPE *X,      CFLINT_TYPE *Y,
                      CFLINT_TYPE *X_Flag, CFLINT_TYPE *Y_Flag,
                      CFLINT_TYPE *Temp1,  CFLINT_TYPE *Temp2,
                      CFLINT_TYPE *Temp3,  CFLINT_TYPE *Temp4,
                      CFLINT_TYPE *Temp5,  CFLINT_TYPE *Temp6,
                      CFLINT_TYPE *Temp7,  CFLINT_TYPE *Temp8, uint32_t Length)
{
    /* 固有开销 */
    CFLINT_TYPE *Quotient = Temp1;
    CFLINT_TYPE *Module   = Temp2;
    CFLINT_TYPE *Dividend = Temp3;
    CFLINT_TYPE *Divisor  = Temp4;
    CFLINT_TYPE *VV = Temp5; CFLINT_TYPE VV_Flag = 0;
    CFLINT_TYPE *TT = Temp6; CFLINT_TYPE TT_Flag = 0;
    /* 1.初始化:X=1,Y=0 */
    *X_Flag = 0; *Y_Flag = 0;
    Cflint_SetValue(Y, (Length + 1) * 2, 0);
    Cflint_SetValue(X, (Length + 1) * 2, 0);
    Cflint_AdditionBit(X, (Length + 1) * 2, 1);
    /* 2.初始化:Dividend=A,Divisor=B,VV=0 */
    Cflint_SetValue(VV, (Length + 1) * 2, 0);
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
        /* TT = X * (*X_Flag) - Temp7(Quotient * VV) * (VV_Flag) */
        Cflint_FlagSum(TT, &TT_Flag, X, *X_Flag,
                       Temp7, VV_Flag == 1 ? 0 : 1, (Length + 1) * 2);
        /* X = VV, *X_Flag = VV_Flag */
        Cflint_Copy(X, VV, (Length + 1) * 2);
        *X_Flag = VV_Flag;
        /* VV = TT, VV_Flag = TT_Flag */
        Cflint_Copy(VV, TT, (Length + 1) * 2);
        VV_Flag = TT_Flag;
        /* Dividend = Divisor */
        /* Divisor  = Module  */
        Cflint_Copy(Dividend, Divisor, Length);
        Cflint_Copy(Divisor,  Module,  Length);
    }
    /* Y = (Dividend - A * X) / B */
    Cflint_SetValue(Temp7, (Length + 1) * 2, 0);
    Cflint_Copy(Temp7, Dividend, Length);
    Cflint_Multiply(Temp8, A, X, Length);
    /* Temp7 = Temp7(Dividend) - Temp8(A * X) * (*X_Flag) */
    Cflint_FlagSum(Temp7, Y_Flag, Temp7, 0,
                   Temp8, *X_Flag == 1 ? 0 : 1, (Length + 1) * 2);
    /* Temp8 = B */
    Cflint_SetValue(Temp8, (Length + 1) * 2, 0);
    Cflint_Copy(Temp8, B, Length);
    /* Y = Temp7 / Temp8 */
    Cflint_Devide(VV, TT, Temp7, Temp8, (Length + 1) * 2);
    Cflint_Copy(Y, VV, (Length + 1) * 2);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 欧几里得乘法逆 */
void Cflint_GCDInverse(CFLINT_TYPE *A,       CFLINT_TYPE *N,
                       CFLINT_TYPE *Inverse, CFLINT_TYPE *GCD,    
                       CFLINT_TYPE *Temp1,   CFLINT_TYPE *Temp2,
                       CFLINT_TYPE *Temp3,   CFLINT_TYPE *Temp4,
                       CFLINT_TYPE *Temp5,   CFLINT_TYPE *Temp6,
                       CFLINT_TYPE *Temp7,   CFLINT_TYPE *Temp8, uint32_t Length)
{
    /* 固有开销 */
    CFLINT_TYPE *Quotient = Temp1;
    CFLINT_TYPE *Module   = Temp2;
    CFLINT_TYPE *Dividend = Temp3;
    CFLINT_TYPE *Divisor  = Temp4;
    CFLINT_TYPE *VV = Temp5;
    CFLINT_TYPE *TT = Temp6;
    /* 1.初始化Inverse=1 */
    Cflint_SetValue(Inverse, Length, 0);
    Cflint_AdditionBit(Inverse, Length, 1);
    /* 2.初始化Devidend=A,Divisor=N,VV=0 */
    Cflint_SetValue(VV, Length, 0);
    Cflint_Copy(Dividend, A, Length);
    Cflint_Copy(Divisor,  N, Length);
    /* 3.Temp7===N */
    Cflint_SetValue(Temp7, Length * 2, 0);
    Cflint_Copy(Temp7, N, Length);
    /* 4.初始化除数为0检查 */
    if (Cflint_IsZero(Divisor, Length) == true)
        return;
    /* 开始主循环直到除数为0 */
    while (Cflint_IsZero(Divisor, Length) == false) {
        /* Quotient = Dividend / Divisor */
        /* Module   = Dividend % Divisor */
        Cflint_Devide(Quotient, Module, Dividend, Divisor, Length);
        /* TT = (Inverse - Quotient * VV)%N */
        Cflint_Multiply(Temp8, Quotient, VV, Length);
        Cflint_Modulo(Temp8, Temp8, Temp7, Length * 2);
        /* 蒙哥马利模减 */
        CFLINT_TYPE Overflow = Cflint_Subtraction(TT, Inverse, Temp8, Length);
        if (Overflow != 0)
            Cflint_Addition(TT, TT, Temp7, Length);
        /* Inverse = VV */
        Cflint_Copy(Inverse, VV, Length);
        /* VV = TT */
        Cflint_Copy(VV, TT, Length);
        /* Dividend = Divisor */
        /* Divisor  = Module  */
        Cflint_Copy(Dividend, Divisor, Length);
        Cflint_Copy(Divisor,  Module,  Length);
    }
    /* GCD=Dividend */
    Cflint_Copy(GCD, Dividend, Length);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 扩展欧几里得算法 */
void Cflint_ExtendGCD(CFLINT_TYPE *A,       CFLINT_TYPE *B,
                      CFLINT_TYPE *X,       CFLINT_TYPE *Y,
                      CFLINT_TYPE *X_Flag,  CFLINT_TYPE *Y_Flag,
                      CFLINT_TYPE *Temp1,   CFLINT_TYPE *Temp2,
                      CFLINT_TYPE *Temp3,   CFLINT_TYPE *Temp4,
                      CFLINT_TYPE *Temp5,   CFLINT_TYPE *Result, uint32_t Length)
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
        /* Y = Y - Quotient * X */
        Cflint_FlagSum(Y, Y_Flag, Y, *Y_Flag,
                       Temp, *X_Flag == 1 ? 0 : 1, (Length + 1) * 2);
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
