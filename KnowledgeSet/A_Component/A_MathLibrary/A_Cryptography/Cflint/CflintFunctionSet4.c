#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_FUNCTION_SET4
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 2次方根整数部运算: */
void Cflint_Root2Integer(CFLINT_TYPE *Result, CFLINT_TYPE *Operand,
                         CFLINT_TYPE *Temp[4],   uint32_t  Length)
{
    /*扩展:B次根整数部分(存档)
     *1.X = 2**[Cflint_Root2Integer(Operand) / B]
     *2.Y = [((B - 1) * X + [N / X**(B - 1)]) / B]
     *                         //X**(B - 1):求模幂,模是(CFLINT_TYPE)(~0)
     *  Y < X
     */
    
    /* 固有开销 */
    CFLINT_TYPE *X = Temp[0];
    CFLINT_TYPE *Y = Temp[1];
    CFLINT_TYPE *Quotient = Temp[2];
    CFLINT_TYPE *Module   = Temp[3];
    /* 计算E = [log2(Operand)] */
    int64_t Numbers2 = Cflint_Numbers2(Operand, Length);
    /* 生成Y = [2**(E + 2) / 2] */
    Cflint_SetValue(Y, Length, 0);
    Cflint_SetValue(Y, Length, 0);
    Cflint_AdditionBit(Y, Length, 1);
    Cflint_ShiftLeft2(Y, Length, ((Numbers2 + 2) >> 1));
    do {
        /* X = Y */
        Cflint_Copy(X, Y, Length);
        /* Quotient = N / X */
        Cflint_Devide(Quotient, Module, Operand, X, Length);
        /* Y = X + Quotient */
        Cflint_Addition(Y, X, Quotient, Length);
        /* Y /= 2 */
        Cflint_ShiftRight2(Y, Length, 1);
        /* Y < X */
    } while (Cflint_Compare(Y, X, Length) == -1);
    /* Result = X */
    Cflint_Copy(Result, X, Length);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 2次方数判别检查 */
bool Cflint_Root2Check(CFLINT_TYPE *Result, CFLINT_TYPE *Operand,
                       CFLINT_TYPE *Temp[4],   uint32_t  Length)
{
    /* 备注:当前未知算法工作异常,临时屏蔽 */
    /*
    uint32_t K = 0;
    uint32_t Q[100] = {0};
    printf("\n-------------------------------------------------------------\n");
    for (K = 0; K <= 10; K++) Q[K] = 0;
    for (K = 0; K <= 5;  K++) Q[(K * K) % 11] = 1;
    printf("static bool Q11[11] = {");
    for (K = 0; K <= 10; K++) printf("%d, ",Q[K]);
    printf("};");
    printf("\n-------------------------------------------------------------\n");
    for (K = 0; K <= 62; K++) Q[K] = 0;
    for (K = 0; K <= 31; K++) Q[(K * K) % 63] = 1;
    printf("static bool Q63[63] = {");
    for (K = 0; K <= 62; K++) printf("%d, ",Q[K]);
    printf("};");
    printf("\n-------------------------------------------------------------\n");
    for (K = 0; K <= 63; K++) Q[K] = 0;
    for (K = 0; K <= 31; K++) Q[(K * K) % 64] = 1;
    printf("static bool Q64[64] = {");
    for (K = 0; K <= 63; K++) printf("%d, ",Q[K]);
    printf("};");
    printf("\n-------------------------------------------------------------\n");
    for (K = 0; K <= 64; K++) Q[K] = 0;
    for (K = 0; K <= 32; K++) Q[(K * K) % 65] = 1;
    printf("static bool Q65[65] = {");
    for (K = 0; K <= 64; K++) printf("%d, ",Q[K]);
    printf("};");
    printf("\n-------------------------------------------------------------\n");
     */
    
    static bool Q11[11] = {1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0};
    static bool Q63[63] = {1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0,
                           1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0,
                           0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0,
                           0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0};
    static bool Q64[64] = {1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                           1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                           0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
                           0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
    static bool Q65[65] = {1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0,
                           1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0,
                           0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
                           0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1};
    
    uint16_t TT = 0;
    /* 特殊:为0检查 */
    if (Cflint_IsZero(Operand, Length)) {
        Cflint_SetValue(Result, Length, 0);
        Cflint_AdditionBit(Result, Length, 1);
        return true;
    }
    /* 1.计算Operand % 64 == Operand[0] & 63 */
    if (!Q64[Operand[0] & 63])
        ;//return false;
    /* 2.计算Temp = Operand % (65 * 63 * 11) */
    Cflint_SetValue(Temp[0], Length, 0);
    Temp[0][0] = (65 * 63 * 11) >> 0;
    Temp[0][1] = (65 * 63 * 11) >> CFLINT_BITS;
    Cflint_Modulo(Temp[1], Operand, Temp[0], Length);
    TT |= ((uint16_t)(Temp[1][0])) << 0;
    TT |= ((uint16_t)(Temp[1][1])) << CFLINT_BITS;
    /* 3.计算Temp % 63 */
    if (!Q63[TT % 63])
        ;//return false;
    /* 4.TT % 65 */
    if (!Q65[TT % 65])
        ;//return false;
    /* 5.TT = TT % 11 */
    if (!Q11[TT % 11])
        ;//return false;
    /* 全额检查 */
    Cflint_Root2Integer(Result, Operand, Temp, Length);
    Cflint_Square(Temp[0], Result, Length);
    if (Cflint_Equal(Operand, Temp[0], Length))
        return true;
    return false;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 符号Jacobi(Operand1/Operand2)计算 */
int8_t Cflint_JacobiFlag(CFLINT_TYPE *Operand1, CFLINT_TYPE *Operand2,
                         CFLINT_TYPE *Temp[3],     uint32_t  Length)
{
    /* 优化1: */
    /* K = (-1)**((X**2 - 1) / 8) ==变形优化后==>> */
    /* K = JacobiTable2[(X % 8) == (X & 7)] */
    static int8_t JacobiTable2[8] = {0, 1, 0, -1, 0, -1, 0, 1};
    /* 优化2: */
    /* K = (-1)**((A - 1)(B - 1) / 4) ==变形优化后==>> */
    /* if (A & B & 2) K = -K */
    true == true;
    /* 固定开销 */
    int8_t  K = 0;
    int64_t V = 0;
    CFLINT_TYPE *A = Temp[0];
    CFLINT_TYPE *B = Temp[1];
    CFLINT_TYPE *T = Temp[2];
    /* 初始化:A = Operand1,B = Operand2 */
    Cflint_Copy(A, Operand1, Length);
    Cflint_Copy(B, Operand2, Length);
    /* 第一步:如果B == 0,(当A == 1,K = 1)(当A != 1,K = 0) */
    if (Cflint_IsZero(B, Length)) {
        Cflint_SetValue(T, Length, 0);
        Cflint_AdditionBit(T, Length, 1);
        bool EqualResult = Cflint_Equal(A, T, Length);
        if ( EqualResult)
            return 1;
        if (!EqualResult)
            return 0;
    }
    /* 第二步:如果A % 2 == B % 2 == 0 */
    if (Cflint_IsEven(A, Length) && Cflint_IsEven(B, Length))
        return 0;
    /* 第二步:如果B % 2 == 0,分解B */
    V = Cflint_Factor2(B, B, Length);
    /* 第二步:如果V % 2 == 0, K = 1 */
    if ((V & 1) == 0)
        K = 1;
    /* 第二步:如果V % 2 != 0, K = (-1)((A**2 - 1) / 8) */
    if ((V & 1) != 0)
        K = JacobiTable2[A[0] & 7];
    do {
        /* 第三步:如果A == 0,(当B > 1,K = 0)(当B <= 1, K = K) */
        if (Cflint_IsZero(A, Length)) {
            Cflint_SetValue(T, Length, 0);
            Cflint_AdditionBit(T, Length, 1);
            int8_t CompareResult = Cflint_Compare(B, T, Length);
            if (CompareResult == 1)
                return 0;
            if (CompareResult != 1)
                return K;
        }
        /* 第三步:如果A % 2 == 0,分解A */
        V = Cflint_Factor2(A, A, Length);
        /* 第三步:如果V % 2 != 0, K = (-1)((A**2 - 1) / 8) */
        if ((V & 1) != 0)
            K = JacobiTable2[B[0] & 7];
        /* 第四步:K = (-1)**((A - 1)(B - 1) / 4)*K */
        if ((A[0] & B[0] & 2) != 0)
            K = -K;
        /* 第四步:T = A, A = B % T, B = T */
        Cflint_Copy(T, A, Length);
        Cflint_Modulo(A, B, T, Length);
        Cflint_Copy(B, T, Length);
    } while (1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 二次剩余计算:((Result**2) % Operand2 == Operand1 % Operand2) */
bool Cflint_ModuloP1Root2(CFLINT_TYPE *Operand1,  CFLINT_TYPE *Operand2,
                          CFLINT_TYPE *Result,    CFLINT_TYPE *Temp[10],
                             uint32_t  Length)
{
    CFLINT_TYPE *A = Operand1;
    CFLINT_TYPE *P = Operand2;
    CFLINT_TYPE *X = Result;
    /* 固有开销 */
    CFLINT_TYPE *B   = Temp[0];
    CFLINT_TYPE *Q   = Temp[1];
    CFLINT_TYPE *T   = Temp[2];
    CFLINT_TYPE *Y   = Temp[3];
    CFLINT_TYPE *Z   = Temp[4];
    CFLINT_TYPE *T1  = Temp[5];
    CFLINT_TYPE **TT = Temp + 6;
    
    /*  */
    int64_t R = 0, M = 0;
    /* 性质:P == 0 || P % 2 == 0 */
    if (Cflint_IsZero(P, Length))
        return false;
    if (Cflint_IsEven(P, Length))
        return false;
    /* 性质:A == 0 */
    if (Cflint_IsZero(A, Length)) {
        Cflint_SetValue(X, Length, 0);
        return true;
    }
    /* 第一步:找到一个二次非剩余 */
    Cflint_Copy(Q, P, Length);
    Cflint_SubtractionBit(Q, Length, 1);
    /* 第一步:(2**R) * Q = P - 1 */
    R = Cflint_Factor2(Q, Q, Length);
    /* 第一步:寻找一个随机数Z = N, 满足Jacobi(N/P) == -1 */
    Cflint_SetValue(Z, Length, 0);
    Cflint_AdditionBit(Z, Length, 2);
    while (Cflint_JacobiFlag(Z, P, TT, Length) == 1)
        Cflint_AdditionBit(Z, Length, 1);
    /* 第二步:计算Z = (N**Q) % P */
    Cflint_ModuloExponent(Z, P, Z, Q, TT, Length);
    /* 第二步:获得Y = Z = (N**Q) % P */
    Cflint_Copy(Y, Z, Length);
    /* 第二步:计算Q = (Q - 1) / 2 */
    Cflint_SubtractionBit(Q, Length, 1);
    Cflint_ShiftRight2(Q, Length, 1);
    /* 第二步:计算X = A ** ((P - 1) / 2) % P = A ** Q % P */
    Cflint_ModuloExponent(X, P, A, Q, TT, Length);
    /* 第二步:计算B = A * (B = X ** 2) % P */
    Cflint_ModuloSquare(B, P, X, TT, Length);
    Cflint_ModuloMultiply(B, P, A, B, TT, Length);
    /* 第二步:计算X = A * X % P */
    Cflint_ModuloMultiply(X, P, A, X, TT, Length);
    /* 第二步:计算Q = B, Z = 1 */
    Cflint_Copy(Q, B, Length);
    Cflint_SetValue(Z, Length, 0);
    Cflint_AdditionBit(Z, Length, 1);
    /* 第三步:寻找Z**(2**M) % P === 1的最小M或结束 */
    while (!Cflint_Equal(B, Z, Length))
    {
        bool LoopStatus = true;
        for (M = 0; LoopStatus; M++) {
            /* 计算:Q = Q**2 % P */
            Cflint_ModuloSquare(Q, P, Q, TT, Length);
            /* 检查 */
            if (!Cflint_Equal(B, Z, Length))
                LoopStatus = false;
        }
        if (M != R) {
            /* 计算:T1 = 2**(R - M - 1) */
            Cflint_SetValue(T1, Length, 0);
            Cflint_AdditionBit(T1, Length, 1);
            Cflint_ShiftLeft2(T1, Length, R - M - 1);
            /* 计算:T = Y**T1 % P */
            Cflint_ModuloExponent(T, P, Y, T1, TT, Length);
            /* 计算:Y = T**2 % P */
            Cflint_ModuloSquare(Y, P, T, TT, Length);
            /* 计算:X = X * T % P */
            Cflint_ModuloMultiply(X, P, X, T, TT, Length);
            /* 计算:B = B * Y % P */
            Cflint_ModuloMultiply(B, P, B, Y, TT, Length);
            /* 计算:Q = B, R = M */
            Cflint_Copy(Q, B, Length);
            R = M;
            continue;
        }
        return false;
    }
    return true;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 扩展二次剩余计算:(X(K)**2) % P == A % (P**K) */
bool Cflint_ModuloPkRoot2(CFLINT_TYPE *Operand1,  CFLINT_TYPE *Operand2,
                          CFLINT_TYPE *Result,    CFLINT_TYPE *Temp[10],
                              int64_t  Exponent,     uint32_t  Length)
{
    /* 递推公式如下:
     * (X(K) * 2 * X(K - 1)) + (X(K - 1)**2 - A) / P == 0 % P
     * ============>
     * X(K) == ((A % P - (X(K - 1)**2) % P) % P) / ((2 * P * X(K - 1)) % P)
     * X(K) == ((A % P - (X(K - 1)**2) % P) % P) / (X(K - 1) % P)
     */
    
    CFLINT_TYPE  *A  = Operand1;
    CFLINT_TYPE  *P  = Operand2;
    CFLINT_TYPE  *X  = Result;
    CFLINT_TYPE **TT = Temp;
    CFLINT_TYPE **TX = Temp + 6;
    CFLINT_TYPE  *T1 = Temp[0];
    CFLINT_TYPE  *T2 = Temp[1];
    CFLINT_TYPE  *T3 = Temp[2];
    CFLINT_TYPE  *T4 = Temp[3];
    /* 解算:X(K - 1)**2 = A % P**(K - 1) */
    if (!Cflint_ModuloP1Root2(A, P, X, TT, Length))
        return false;
    /* 逆向递归 */
    for (uint32_t K = 0; K < Exponent; K++) {
        /* 计算:T1 = (X(K - 1)**2) % P */
        Cflint_ModuloSquare(T1, X, P, TX, Length);
        /* 计算:T1 = ((A - (X(K - 1)**2)) % P) */
        Cflint_ModuloSubtraction(T1, P, A, T1, TX, Length);
        /* 计算:T2 = X(K - 1) % P */
        Cflint_Modulo(T2, X, P ,Length);
        /* 计算:Quotient = T1 / T2 */
        Cflint_Devide(T3, T4, T1, T2, Length);
        Cflint_Modulo(X, T3, P, Length);
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 二次剩余计算:((Result**2) % (Operand2 * Operand3) ==  */
/*               (Operand1)  % (Operand2 * Operand3))    */
bool Cflint_Modulo1Root2(CFLINT_TYPE *Operand1,  CFLINT_TYPE *Operand2,
                         CFLINT_TYPE *Operand3,  CFLINT_TYPE *Result,
                         CFLINT_TYPE *Temp[13],     uint32_t  Length)
{
    CFLINT_TYPE  *A  = Operand1;
    CFLINT_TYPE  *P  = Operand2;
    CFLINT_TYPE  *Q  = Operand3;
    CFLINT_TYPE  *X  = Result;
    CFLINT_TYPE  *T  = Temp[0];
    CFLINT_TYPE  *XP = Temp[1];
    CFLINT_TYPE  *XQ = Temp[2];
    CFLINT_TYPE **TT = Temp + 3;
    CFLINT_TYPE  *U  = Temp[3]; CFLINT_TYPE U_Flag = 0;  //(Length+1)*2
    CFLINT_TYPE  *V  = Temp[4]; CFLINT_TYPE V_Flag = 0;  //(Length+1)*2
    CFLINT_TYPE  *N  = Temp[5];  //(Length+1)*2
    CFLINT_TYPE **TX = Temp + 6;
    CFLINT_TYPE  *X0 = Temp[6]; CFLINT_TYPE X0_Flag = 0; //(Length+1)*2
    CFLINT_TYPE  *X1 = Temp[7]; CFLINT_TYPE X1_Flag = 0; //(Length+1)*2
    CFLINT_TYPE  *X2 = Temp[8]; CFLINT_TYPE X2_Flag = 0; //(Length+1)*2
    CFLINT_TYPE  *X3 = Temp[9]; CFLINT_TYPE X3_Flag = 0; //(Length+1)*2
    CFLINT_TYPE  *T0 = Temp[10]; //(Length+1)*2
    CFLINT_TYPE  *T1 = Temp[11]; //Length * 4
    CFLINT_TYPE  *T2 = Temp[12]; //Length * 4
    /* 查验:A == 0 */
    if (Cflint_IsZero(A, Length * 2)) {
        Cflint_SetValue(X, Length * 2, 0);
        return true;
    }
    /* 计算:T = A % P */
    Cflint_SetValue(T0, Length * 2, 0);
    Cflint_Copy(T0, P, Length);
    Cflint_Modulo(T1, A, T0, Length * 2);
    Cflint_Copy(T, T1, Length);
    /* 解算:((XP**2) % P == A % P) */
    if (!Cflint_ModuloP1Root2(T, P, XP, TT, Length))
        return false;
    /* 计算:T = A % Q */
    Cflint_SetValue(T0, Length * 2, 0);
    Cflint_Copy(T0, Q, Length);
    Cflint_Modulo(T1, A, T0, Length * 2);
    Cflint_Copy(T, T1, Length);
    /* 解算:((XQ**2) % Q == A % Q) */
    if (!Cflint_ModuloP1Root2(T, Q, XQ, TT, Length))
        return false;
    /* 解算:P * U + Q * V = GCD(P, Q) */
    Cflint_GCDExtend(P, Q, N, U, &U_Flag, V, &V_Flag, TX, Length);
    /* 计算:N = P * Q */
    N[Length * 2 + 0] = 0;
    N[Length * 2 + 1] = 0;
    Cflint_Multiply(N, P, Q, Length);
    Cflint_SetValue(T2, Length * 4, 0);
    Cflint_Copy(T2, N, Length * 2);
    /* 计算:U = P * XQ * U % N */
    Cflint_Modulo(U, U, N, (Length + 1) * 2);
    Cflint_Multiply(T0, P, XQ, Length);
    Cflint_Multiply(T1, T0, U, Length * 2);
    Cflint_Modulo(T1, T1, T2, Length * 4);
    Cflint_Copy(U, T1, Length * 2);
    /* 计算:V = Q * XP * V % N */
    Cflint_Modulo(V, V, N, (Length + 1) * 2);
    Cflint_Multiply(T0, Q, XP, Length);
    Cflint_Multiply(T1, T0, V, Length * 2);
    Cflint_Modulo(T1, T1, T2, Length * 4);
    Cflint_Copy(V, T1, Length * 2);
    /* 计算4个X: */
    CFLINT_TYPE Overflow = 0;
    /* 计算:X0 = (U + V) % N, X1 = -X0 % N = N - X0 */
    Overflow = Cflint_FlagSum(X0, &X0_Flag, U, U_Flag, V, V_Flag, Length * 2);
    Cflint_FlagModulo(X0, X0, N, X0_Flag, Length * 2);
    Cflint_Subtraction(X1, N, X0, Length * 2);
    /* 计算:U_Flag~~ */
    U_Flag = (U_Flag == 0) ? 1 : 0;
    /* 计算:X2 = (V - U) % N, X3 = -X2 % N = N - X2 */
    Overflow = Cflint_FlagSum(X2, &X2_Flag, U, U_Flag, V, V_Flag, Length * 2);
    Cflint_FlagModulo(X2, X2, N, X2_Flag, Length * 2);
    Cflint_Subtraction(X3, N, X2, Length * 2);
    /* X = Min(X0, X1, X2, X3) */
    CFLINT_TYPE *X_Min0 = NULL;
    CFLINT_TYPE *X_Min1 = NULL;
    CFLINT_TYPE *X_Min2 = NULL;
    int8_t CompareResult1 = Cflint_Compare(X0, X1, Length * 2);
    int8_t CompareResult2 = Cflint_Compare(X2, X3, Length * 2);
    X_Min1 = (CompareResult1 == -1) ? X0 : X1;
    X_Min2 = (CompareResult1 == -1) ? X2 : X3;
    int8_t CompareResult0 = Cflint_Compare(X_Min1, X_Min2, Length * 2);
    X_Min0 = (CompareResult1 == -1) ? X_Min1 : X_Min2;
    /* 返回最小的根 */
    Cflint_Copy(X, X_Min0, Length * 2);
    return true;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 线性同余方程组计算:X == Ai % Mi,当i != j时, GCD(Mi, Mj) == 1 */
int8_t Cflint_LCE(CFLINT_TYPE **Operands, CFLINT_TYPE *Result,   uint64_t Number,
                  CFLINT_TYPE  *Temps[5], CFLINT_TYPE *Temp[10], uint32_t Length,
                     uint32_t   LengthMax)
{
    /* 固有开销1:不可控变长开销 */
    uint32_t L_MX  = 0;
    uint32_t L_UV  = 0;
    CFLINT_TYPE  *X   = Result;  CFLINT_TYPE X_Flag = 0;
    CFLINT_TYPE  *M   = Temps[0];
    CFLINT_TYPE  *UL  = Temps[1];
    CFLINT_TYPE  *VL  = Temps[2];
    CFLINT_TYPE  *TL0 = Temps[3];
    CFLINT_TYPE  *TL1 = Temps[4];
    /* 固有开销1:定长开销 */
    CFLINT_TYPE  *GCD = Temp[0];
    CFLINT_TYPE  *U   = Temp[1]; CFLINT_TYPE U_Flag = 0; //(Length+1)*2
    CFLINT_TYPE  *V   = Temp[2]; CFLINT_TYPE V_Flag = 0; //(Length+1)*2
    CFLINT_TYPE **TT  = Temp + 3;
    CFLINT_TYPE  *T0  = Temp[3];
    CFLINT_TYPE  *T1  = Temp[4];
    /* 无同余方程容错 */
    if (Number == 0)
        return true;
    /* 第一步:载入第一个同余方程 */
    L_MX = Length;
    Cflint_Copy(X, Operands[0 * 2 + 0], Length);
    Cflint_Copy(M, Operands[0 * 2 + 1], Length);
    /* 第二步:循环解算同余方程 */
    for (uint32_t Index = 1; Index < Number; Index++) {
        /* 载入其余同余方程 */
        CFLINT_TYPE *Ai = Operands[Index * 2 + 0];
        CFLINT_TYPE *Mi = Operands[Index * 2 + 1];
        /* GCD(M, Mi) = GCD(M % Mi, Mi),这里将M化为Length长度 */
        if (LengthMax < L_MX * 2)
            return -2;
        Cflint_SetValue(TL1, L_MX, 0);
        Cflint_Copy(TL1, Mi, Length);
        Cflint_Modulo(TL0, M, TL1, L_MX);
        /* 解算扩展欧几里得方程 */
        Cflint_GCDExtend(TL0, Mi, GCD, U, &U_Flag, V, &V_Flag, TT, Length);
        /* 非素检查:GCD == 1 */
        if (Cflint_SubtractionBit(GCD, Length, 1) != 0)
            return -1;
        if (!Cflint_IsZero(GCD, Length))
            return -1;
        /* 计算U = U * Ai, U的长度为理论最大值(Length + 1) * 4 */
        Cflint_Copy(T0, U, (Length + 1) * 2);
        Cflint_SetValue(T1, (Length + 1) * 2, 0);
        Cflint_Copy(T1, Ai, Length);
        Cflint_Multiply(U, T0, T1, (Length + 1) * 2);
        /* 计算V = V * Mi, V的长度为理论最大值(Length + 1) * 4 */
        Cflint_Copy(T0, V, (Length + 1) * 2);
        Cflint_SetValue(T1, (Length + 1) * 2, 0);
        Cflint_Copy(T1, Mi, Length);
        Cflint_Multiply(V, T0, T1, (Length + 1) * 2);
        /* 计算最大范围: */
        L_UV = (L_MX > (Length + 1) * 4) ? L_MX : (Length + 1) * 4;
        if (LengthMax <= L_UV + 1)
            return -2;
        /* 计算变长乘:UL = U * M */
        Cflint_SetValue(T0, L_UV, 0);
        Cflint_SetValue(T1, L_UV, 0);
        Cflint_Copy(T0, U, (Length + 1) * 4);
        Cflint_Copy(T0, M, L_MX);
        Cflint_Multiply(UL, T0, T1, L_MX);
        /* 计算变长乘:VL = V * X */
        Cflint_SetValue(T0, L_UV, 0);
        Cflint_SetValue(T1, L_UV, 0);
        Cflint_Copy(T0, V, (Length + 1) * 4);
        Cflint_Copy(T0, X, L_MX);
        Cflint_Multiply(VL, T0, T1, L_MX);
        /* 计算和:X * X_Flag = U * Ai * M * U_Flag + V * Mi * X * V_Flag */
        X[L_UV] = Cflint_FlagSum(X, &X_Flag, UL, U_Flag, VL, V_Flag, L_UV);
        /* 计算乘:M *= Mi,这里需要对其归化 */
        Cflint_SetValue(T1, L_MX, 0);
        Cflint_Copy(T1, Mi, Length);
        Cflint_Multiply(T0, M, T1, L_MX);
        L_MX = (Cflint_Numbers2(T0, L_MX * 2) + 1) / CFLINT_BITS;
        /* 计算模:X*X_Flag %= M */
        uint32_t L_MXUV = L_MX > (L_UV + 1) ? L_MX : (L_UV + 1);
        Cflint_SetValue(T0, L_MXUV, 0);
        Cflint_SetValue(T1, L_MXUV, 0);
        Cflint_Copy(T0, X, L_UV + 1);
        Cflint_Copy(T1, M, L_MX);
        Cflint_FlagModulo(X, T0, T1, X_Flag, L_MXUV);
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
