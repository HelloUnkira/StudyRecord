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
    /* 特殊:为0检查 */
    if (Cflint_IsZero(Operand, Length) == true) {
        Cflint_SetValue(Result, Length, 0);
        Cflint_AdditionBit(Result, Length, 1);
        return true;
    }
    /* 1.temp = Operand % 64 */
    Cflint_SetValue(Temp2, Length, 0);
    Cflint_AdditionBit(Temp2, Length, 64);
    Cflint_Modulo(Temp1, Operand, Temp2, Length);
    if (Q64[Temp1[0]] == false)
        ;//return false;
    /* 2.temp = Operand % (65 * 63 * 11) */
    Cflint_SetValue(Temp1, Length, 0);
    Temp1[0] = ((uint16_t)(65 * 63 * 11)) >> 0;
    Temp1[1] = ((uint16_t)(65 * 63 * 11)) >> CFLINT_BITS;
    Cflint_Modulo(Temp3, Operand, Temp1, Length);
    /* 3.temp = temp % 63 */
    Cflint_SetValue(Temp2, Length, 0);
    Cflint_AdditionBit(Temp2, Length, 63);
    Cflint_Modulo(Temp1, Temp3, Temp2, Length);
    if (Q63[Temp1[0]] == false)
        ;//return false;
    /* 4.temp = Temp % 65 */
    Cflint_SetValue(Temp2, Length, 0);
    Cflint_AdditionBit(Temp2, Length, 65);
    Cflint_Modulo(Temp1, Temp3, Temp2, Length);
    if (Q65[Temp1[0]] == false)
        ;//return false;
    /* 5.temp = Temp % 11 */
    Cflint_SetValue(Temp2, Length, 0);
    Cflint_AdditionBit(Temp2, Length, 11);
    Cflint_Modulo(Temp1, Temp3, Temp2, Length);
    if (Q11[Temp1[0]] == false)
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
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
