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
                         CFLINT_TYPE *Temp1,  CFLINT_TYPE *Temp2,
                         CFLINT_TYPE *Temp3,  CFLINT_TYPE *Temp4, uint32_t Length)
{
    /*扩展:B次根整数部分(存档)
     *1.X = 2**[Cflint_Root2Integer(Operand) / B]
     *2.Y = [((B - 1) * X + [N / X**(B - 1)]) / B]
     *                         //X**(B - 1):求模幂,模是(CFLINT_TYPE)(~0)
     *  Y < X
     */
    
    /* 固有开销 */
    CFLINT_TYPE *X = Temp1;
    CFLINT_TYPE *Y = Temp2;
    CFLINT_TYPE *Quotient = Temp3;
    CFLINT_TYPE *Module   = Temp4;
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
                       CFLINT_TYPE *Temp1,  CFLINT_TYPE *Temp2,
                       CFLINT_TYPE *Temp3,  CFLINT_TYPE *Temp4, uint32_t Length)
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
    uint16_t Temp = 0;
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
    Cflint_SetValue(Temp1, Length, 0);
    Temp1[0] = ((uint16_t)(65 * 63 * 11)) >> 0;
    Temp1[1] = ((uint16_t)(65 * 63 * 11)) >> CFLINT_BITS;
    Cflint_Modulo(Temp3, Operand, Temp1, Length);
    Temp |= ((uint16_t)(Temp3[0])) << 0;
    Temp |= ((uint16_t)(Temp3[1])) << CFLINT_BITS;
    /* 3.计算Temp % 63 */
    if (Q63[Temp % 63] == false)
        ;//return false;
    /* 4.Temp % 65 */
    if (Q65[Temp % 65] == false)
        ;//return false;
    /* 5.temp = Temp % 11 */
    if (Q11[Temp % 11] == false)
        ;//return false;
    /* 全额检查 */
    Cflint_Root2Integer(Result, Operand, Temp1, Temp2, Temp3, Temp4, Length);
    Cflint_Square(Temp1, Result, Length);
    if (Cflint_Equal(Operand, Temp1, Length) == true)
        return true;
    return false;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 符号Jacobi(Operand1/Operand2)计算 */
int8_t Cflint_JacobiFlag(CFLINT_TYPE *Operand1, CFLINT_TYPE *Operand2,
                         CFLINT_TYPE *Temp1,    CFLINT_TYPE *Temp2,
                         CFLINT_TYPE *Temp3,       uint32_t  Length)
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
    CFLINT_TYPE *A = Temp1;
    CFLINT_TYPE *B = Temp2;
    CFLINT_TYPE *Temp = Temp3;
    /* 初始化:A = Operand1,B = Operand2 */
    Cflint_Copy(A, Operand1, Length);
    Cflint_Copy(B, Operand2, Length);
    /* 第一步:如果B == 0,(当A == 1,K = 1)(当A != 1,K = 0) */
    if (Cflint_IsZero(B, Length) == true) {
        Cflint_SetValue(Temp, Length, 0);
        Cflint_AdditionBit(Temp, Length, 1);
        bool EqualResult = Cflint_Equal(A, Temp, Length);
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
            Cflint_SetValue(Temp, Length, 0);
            Cflint_AdditionBit(Temp, Length, 1);
            int8_t CompareResult = Cflint_Compare(B, Temp, Length);
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
        /* 第四步:Temp = A, A = B % Temp, B = Temp */
        Cflint_Copy(Temp, A, Length);
        Cflint_Modulo(A, B, Temp, Length);
        Cflint_Copy(B, Temp, Length);
    } while (1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
