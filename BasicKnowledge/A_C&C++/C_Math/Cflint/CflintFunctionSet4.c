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
    if (Cflint_IsZero(Operand, Length) == true) {
        Cflint_SetValue(Result, Length, 0);
        Cflint_AdditionBit(Result, Length, 1);
        return true;
    }
    /* 1.计算Operand % 64 == Operand[0] & 63 */
    if (Q64[Operand[0] & 63] == false)
        ;//return false;
    /* 2.计算Temp = Operand % (65 * 63 * 11) */
    Cflint_SetValue(Temp[0], Length, 0);
    Temp[0][0] = ((uint16_t)(65 * 63 * 11)) >> 0;
    Temp[0][1] = ((uint16_t)(65 * 63 * 11)) >> CFLINT_BITS;
    Cflint_Modulo(Temp[1], Operand, Temp[0], Length);
    TT |= ((uint16_t)(Temp[1][0])) << 0;
    TT |= ((uint16_t)(Temp[1][1])) << CFLINT_BITS;
    /* 3.计算Temp % 63 */
    if (Q63[TT % 63] == false)
        ;//return false;
    /* 4.TT % 65 */
    if (Q65[TT % 65] == false)
        ;//return false;
    /* 5.TT = TT % 11 */
    if (Q11[TT % 11] == false)
        ;//return false;
    /* 全额检查 */
    Cflint_Root2Integer(Result, Operand, Temp, Length);
    Cflint_Square(Temp[0], Result, Length);
    if (Cflint_Equal(Operand, Temp[0], Length) == true)
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
    if (Cflint_IsZero(B, Length) == true) {
        Cflint_SetValue(T, Length, 0);
        Cflint_AdditionBit(T, Length, 1);
        bool EqualResult = Cflint_Equal(A, T, Length);
        if (EqualResult == true)
            return 1;
        if (EqualResult == false)
            return 0;
    }
    /* 第二步:如果A % 2 == B % 2 == 0 */
    if (Cflint_IsEven(A, Length) == true &&
        Cflint_IsEven(B, Length) == true)
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
        if (Cflint_IsZero(A, Length) == true) {
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
    if (Cflint_IsZero(P, Length) == true)
        return false;
    if (Cflint_IsEven(P, Length) == true)
        return false;
    /* 性质:A == 0 */
    if (Cflint_IsZero(A, Length) == true) {
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
    while (Cflint_Equal(B, Z, Length) == false)
    {
        bool LoopStatus = true;
        for (M = 0; LoopStatus; M++) {
            /* 计算:Q = Q**2 % P */
            Cflint_ModuloSquare(Q, P, Q, TT, Length);
            /* 检查 */
            if (Cflint_Equal(B, Z, Length) == false)
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
    if (Cflint_ModuloP1Root2(A, P, X, TT, Length) == false)
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
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
