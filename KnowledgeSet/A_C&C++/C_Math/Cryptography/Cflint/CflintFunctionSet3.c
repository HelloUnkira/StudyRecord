#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_FUNCTION_SET3
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 最大公因数为1检查(互素检查) */
bool Cflint_GCDCheck(CFLINT_TYPE *A, CFLINT_TYPE *B, CFLINT_TYPE *Temp[3],
                        uint32_t  Length)
{
    /* 固有开销 */
    CFLINT_TYPE *Module   = Temp[0];
    CFLINT_TYPE *Dividend = Temp[1];
    CFLINT_TYPE *Divisor  = Temp[2];
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
void Cflint_GCD(CFLINT_TYPE *Result, CFLINT_TYPE *A, CFLINT_TYPE *B,
                CFLINT_TYPE *Temp[3],   uint32_t  Length)
{
    /* 固有开销 */
    CFLINT_TYPE *Module   = Temp[0];
    CFLINT_TYPE *Dividend = Temp[1];
    CFLINT_TYPE *Divisor  = Temp[2];
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
void Cflint_LCM(CFLINT_TYPE *Result, CFLINT_TYPE *A, CFLINT_TYPE *B,
                CFLINT_TYPE *Temp[5],   uint32_t  Length)
{
    /* 1.计算A*B */
    Cflint_Multiply(Temp[3], A, B, Length);
    Cflint_SetValue(Temp[4], Length * 2, 0);
    /* 2.计算GCD(A,B); */
    Cflint_GCD(Temp[4], A, B, Temp, Length);
    /* 3.计算A * B / GCD(A,B) */
    Cflint_Devide(Temp[0], Temp[1], Temp[3], Temp[4], Length * 2);
    /* 4.得出结果 */
    Cflint_Copy(Result, Temp[0], Length * 2);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 扩展欧几里得算法 */
void Cflint_GCDExtend(CFLINT_TYPE *A, CFLINT_TYPE *B, CFLINT_TYPE *GCD,
                      CFLINT_TYPE *X, CFLINT_TYPE *X_Flag,
                      CFLINT_TYPE *Y, CFLINT_TYPE *Y_Flag,
                      CFLINT_TYPE *Temp[7],   uint32_t  Length)
{
    /*案例代码:
     *uint64_t GCDExtend(uint64_t A, uint64_t B, uint64_t &X, uint64_t &Y)
     *{
     *    if (B == 0) {
     *        X = 1;
     *        Y = 0;
     *        return A;
     *    } else {
     *        uint64_t Q   = A / B;
     *        uint64_t M   = A % B;
     *        uint64_t GCD = GCDExtend(B, M, X, Y);
     *        uint64_t T = X; X = Y; Y = T; Y = Y - Q * X;
     *        return GCD;
     *    }
     *}
     */
    
    /* 固有开销 */
    CFLINT_TYPE *Dividend = GCD;
    CFLINT_TYPE *Divisor  = Temp[0];
    CFLINT_TYPE *Quotient = Temp[1];
    CFLINT_TYPE *Module   = Temp[2];
    CFLINT_TYPE *VV = Temp[3]; CFLINT_TYPE VV_Flag = 0;
    CFLINT_TYPE *TT = Temp[4]; CFLINT_TYPE TT_Flag = 0;
    CFLINT_TYPE *T1 = Temp[5];
    CFLINT_TYPE *T2 = Temp[6];
    /* 1.初始化:X=1,Y=0 */
    *X_Flag = 0; *Y_Flag = 0;
    Cflint_SetValue(Y, (Length + 1) * 2, 0);
    Cflint_SetValue(X, (Length + 1) * 2, 0);
    Cflint_AdditionBit(X, (Length + 1) * 2, 1);
    /* 2.初始化:Dividend = A, Divisor = B, VV = 0 */
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
        Cflint_Multiply(T1, Quotient, VV, Length);
        /* TT = X * (*X_Flag) - T1(Quotient * VV) * (VV_Flag) */
        Cflint_FlagSum(TT, &TT_Flag, X, *X_Flag,
                       T1, VV_Flag == 1 ? 0 : 1, (Length + 1) * 2);
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
    Cflint_SetValue(T1, (Length + 1) * 2, 0);
    Cflint_Copy(T1, Dividend, Length);
    Cflint_Multiply(T2, A, X, Length);
    /* T1 = T1(Dividend) - T2(A * X) * (*X_Flag) */
    Cflint_FlagSum(T1, Y_Flag, T1, 0,
                   T2, *X_Flag == 1 ? 0 : 1, (Length + 1) * 2);
    /* T2 = B */
    Cflint_SetValue(T2, (Length + 1) * 2, 0);
    Cflint_Copy(T2, B, Length);
    /* Y = T1 / T2 */
    Cflint_Devide(VV, TT, T1, T2, (Length + 1) * 2);
    Cflint_Copy(Y, VV, (Length + 1) * 2);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 欧几里得乘法逆 */
void Cflint_GCDInverse(CFLINT_TYPE *A,       CFLINT_TYPE *N,
                       CFLINT_TYPE *Inverse, CFLINT_TYPE *GCD,    
                       CFLINT_TYPE *Temp[8],     uint32_t Length)
{
    /* 固有开销 */
    CFLINT_TYPE *Quotient = Temp[0];
    CFLINT_TYPE *Module   = Temp[1];
    CFLINT_TYPE *Dividend = Temp[2];
    CFLINT_TYPE *Divisor  = Temp[3];
    CFLINT_TYPE *VV = Temp[4];
    CFLINT_TYPE *TT = Temp[5];
    CFLINT_TYPE *T1 = Temp[6];
    CFLINT_TYPE *T2 = Temp[7];
    /* 1.初始化Inverse=1 */
    Cflint_SetValue(Inverse, Length, 0);
    Cflint_AdditionBit(Inverse, Length, 1);
    /* 2.初始化Devidend=A,Divisor=N,VV=0 */
    Cflint_SetValue(VV, Length, 0);
    Cflint_Copy(Dividend, A, Length);
    Cflint_Copy(Divisor,  N, Length);
    /* 3.T1===N */
    Cflint_SetValue(T1, Length * 2, 0);
    Cflint_Copy(T1, N, Length);
    /* 4.初始化除数为0检查 */
    if (Cflint_IsZero(Divisor, Length) == true)
        return;
    /* 开始主循环直到除数为0 */
    while (Cflint_IsZero(Divisor, Length) == false) {
        /* Quotient = Dividend / Divisor */
        /* Module   = Dividend % Divisor */
        Cflint_Devide(Quotient, Module, Dividend, Divisor, Length);
        /* TT = (Inverse - Quotient * VV)%N */
        Cflint_Multiply(T2, Quotient, VV, Length);
        Cflint_Modulo(T2, T2, T1, Length * 2);
        /* 蒙哥马利模减 */
        CFLINT_TYPE Overflow = Cflint_Subtraction(TT, Inverse, T2, Length);
        if (Overflow != 0)
            Cflint_Addition(TT, TT, T1, Length);
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
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
