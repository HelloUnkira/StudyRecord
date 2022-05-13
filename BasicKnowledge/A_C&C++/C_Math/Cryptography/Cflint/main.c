/* 例: */

#include <stdio.h>
#include <windows.h>
#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void Test_CflintFunctionSet1(void)
{
    uint32_t Index = 0;
    #define TEST_FUNCTIONSET1_LENGTH    10
    CFLINT_TYPE Result[TEST_FUNCTIONSET1_LENGTH] = {0};
    
    /* 左移位运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET1_LENGTH; Index++)
        Result[Index] = ~0;
    Cflint_ShiftLeft2(Result, TEST_FUNCTIONSET1_LENGTH, 1);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_ShiftLeft2:::Result:\n");
    for (Index = 0; Index < TEST_FUNCTIONSET1_LENGTH; Index++)
        printf("%llx ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 右移位运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET1_LENGTH; Index++)
        Result[Index] = ~0;
    Cflint_ShiftRight2(Result, TEST_FUNCTIONSET1_LENGTH, 1);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_ShiftRight2:::Result:\n");
    for (Index = 0; Index < TEST_FUNCTIONSET1_LENGTH; Index++)
        printf("%llx ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 掩码运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET1_LENGTH * 2; Index++)
        Result[Index] = ~0;
    Cflint_Mask2(Result, TEST_FUNCTIONSET1_LENGTH * 2, CFLINT_BITS * 4 + 3);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Mask2:::Result:\n");
    for (Index = 0; Index < TEST_FUNCTIONSET1_LENGTH * 2; Index++)
        printf("%llx ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void Test_CflintFunctionSet2(void)
{
    uint32_t Index = 0;
    #define TEST_FUNCTIONSET2_LENGTH    10
    CFLINT_TYPE Result  [TEST_FUNCTIONSET2_LENGTH * 2] = {0};
    CFLINT_TYPE Operand1[TEST_FUNCTIONSET2_LENGTH] = {0};
    CFLINT_TYPE Operand2[TEST_FUNCTIONSET2_LENGTH] = {0};
    CFLINT_TYPE Overflow = 0;
    
    /* 和运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET2_LENGTH; Index++) {
        Operand1[Index] = (~0) - Index;
        Operand2[Index] = (~0) - Index;
    }
    Overflow =
    Cflint_Addition(Result, Operand1, Operand2, TEST_FUNCTIONSET2_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Addition:::Overflow:%d, Result:\n", Overflow);
    for (Index = 0; Index < TEST_FUNCTIONSET2_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 差运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET2_LENGTH; Index++) {
        Operand1[Index] = TEST_FUNCTIONSET2_LENGTH - Index;
        Operand2[Index] = Index;
    }
    Overflow =
    Cflint_Subtraction(Result, Operand1, Operand2, TEST_FUNCTIONSET2_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Subtraction:::Overflow:%d, Result:\n", Overflow);
    for (Index = 0; Index < TEST_FUNCTIONSET2_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/

    /* 乘运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET2_LENGTH; Index++) {
        Operand1[Index] = Index;
        Operand2[Index] = Index;
    }
    Cflint_Multiply(Result, Operand1, Operand2, TEST_FUNCTIONSET2_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Multiply:::Result:\n");
    for (Index = 0; Index < TEST_FUNCTIONSET2_LENGTH * 2; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/

    /* 方运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET2_LENGTH; Index++) {
        Operand1[Index] = Index;
        Operand2[Index] = Index;
    }
    Cflint_Square(Result, Operand1, TEST_FUNCTIONSET2_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Square:::Result:\n");
    for (Index = 0; Index < TEST_FUNCTIONSET2_LENGTH * 2; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void Test_CflintFunctionSet3(void)
{
    uint32_t Index = 0;
    #define TEST_FUNCTIONSET3_LENGTH    10
    CFLINT_TYPE Result  [TEST_FUNCTIONSET3_LENGTH * 2] = {0};
    CFLINT_TYPE Quotient[TEST_FUNCTIONSET3_LENGTH * 2] = {0};
    CFLINT_TYPE Module  [TEST_FUNCTIONSET3_LENGTH * 2] = {0};
    CFLINT_TYPE Operand1[TEST_FUNCTIONSET3_LENGTH * 2] = {0};
    CFLINT_TYPE Operand2[TEST_FUNCTIONSET3_LENGTH * 2] = {0};
    CFLINT_TYPE Overflow = 0;
    
    /* 除运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET3_LENGTH * 2; Index++) {
        if (Index < TEST_FUNCTIONSET3_LENGTH) {
            Operand1[Index] = Index;
            Operand2[Index] = TEST_FUNCTIONSET3_LENGTH - Index;
            Module[Index]   = TEST_FUNCTIONSET3_LENGTH - Index - 1;
        } else {
            Operand1[Index] = 0;
            Operand2[Index] = 0;
            Module  [Index] = 0;
        }
    }
    /* 先用乘法运算计算一下值 */
    Cflint_Multiply(Result, Operand1, Operand2, TEST_FUNCTIONSET3_LENGTH);
    /* 为其添加一个余数(这里不让其溢出) */
    Cflint_Addition(Result, Result, Module, TEST_FUNCTIONSET3_LENGTH * 2);
    /* 再计算除数结果 */
    Cflint_Devide(Quotient, Module, Result, Operand1,
                  TEST_FUNCTIONSET3_LENGTH * 2);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Devide:::");
    printf("\n---------------------------------------------------------------");
    printf("\nResult:");
    for (Index = 0; Index < TEST_FUNCTIONSET3_LENGTH * 2; Index++)
        printf("%llx ", Result[Index]);
    printf("\nOperand1:");
    for (Index = 0; Index < TEST_FUNCTIONSET3_LENGTH; Index++)
        printf("%llx ", Operand1[Index]);
    printf("\nOperand2:");
    for (Index = 0; Index < TEST_FUNCTIONSET3_LENGTH; Index++)
        printf("%llx ", Operand2[Index]);
    printf("\nQuotient:");
    for (Index = 0; Index < TEST_FUNCTIONSET3_LENGTH * 2; Index++)
        printf("%llx ", Quotient[Index]);
    printf("\nModule:");
    for (Index = 0; Index < TEST_FUNCTIONSET3_LENGTH * 2; Index++)
        printf("%llx ", Module[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 模运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET3_LENGTH * 2; Index++) {
        if (Index < TEST_FUNCTIONSET3_LENGTH) {
            Operand1[Index] = Index;
            Operand2[Index] = TEST_FUNCTIONSET3_LENGTH - Index;
            Module  [Index] = TEST_FUNCTIONSET3_LENGTH - Index - 1;
        } else {
            Operand1[Index] = 0;
            Operand2[Index] = 0;
            Module  [Index] = 0;
        }
    }
    /* 先用乘法运算计算一下值 */
    Cflint_Multiply(Result, Operand1, Operand2, TEST_FUNCTIONSET3_LENGTH);
    /* 为其添加一个余数(这里不让其溢出) */
    Cflint_Addition(Result, Result, Module, TEST_FUNCTIONSET3_LENGTH * 2);
    /* 再计算模结果 */
    Cflint_Modulo(Module, Result, Operand1, TEST_FUNCTIONSET3_LENGTH * 2);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Modulo:::");
    printf("\n---------------------------------------------------------------");
    printf("\nResult:");
    for (Index = 0; Index < TEST_FUNCTIONSET3_LENGTH * 2; Index++)
        printf("%llx ", Result[Index]);
    printf("\nOperand1:");
    for (Index = 0; Index < TEST_FUNCTIONSET3_LENGTH; Index++)
        printf("%llx ", Operand1[Index]);
    printf("\nOperand2:");
    for (Index = 0; Index < TEST_FUNCTIONSET3_LENGTH; Index++)
        printf("%llx ", Operand2[Index]);
    printf("\nModule:");
    for (Index = 0; Index < TEST_FUNCTIONSET3_LENGTH * 2; Index++)
        printf("%llx ", Module[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void Test_CflintFunctionSet4(void)
{
    uint32_t Index = 0;
    #define TEST_FUNCTIONSET4_LENGTH    3
    CFLINT_TYPE Result  [TEST_FUNCTIONSET4_LENGTH] = {0};
    CFLINT_TYPE Operand [TEST_FUNCTIONSET4_LENGTH] = {0};
    CFLINT_TYPE Exponent[TEST_FUNCTIONSET4_LENGTH] = {0};
    CFLINT_TYPE Module  [TEST_FUNCTIONSET4_LENGTH * 2] = {0};
    /* 模幂运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET4_LENGTH; Index++) {
        Operand[Index] = TEST_FUNCTIONSET4_LENGTH - Index;
        Module[Index]  = TEST_FUNCTIONSET4_LENGTH - Index;
    }
    Module[0]   = 0; 
    Exponent[0] = 5;
    /* 模幂运算 */
    {
        CFLINT_TYPE  Temp1 [TEST_FUNCTIONSET4_LENGTH * 2] = {0};
        CFLINT_TYPE  Temp2 [TEST_FUNCTIONSET4_LENGTH * 2] = {0};
        CFLINT_TYPE  Temp3 [TEST_FUNCTIONSET4_LENGTH * 2] = {0};
        CFLINT_TYPE  Temp4 [TEST_FUNCTIONSET4_LENGTH * 2] = {0};
        CFLINT_TYPE *Temp[4] = {Temp1, Temp2, Temp3, Temp4};
        /* 模幂运算 */
        Cflint_ModuloExponent(Result, Module, Operand, Exponent,
                              Temp, TEST_FUNCTIONSET4_LENGTH);
    }
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_ModuloExponent:::");
    printf("\n---------------------------------------------------------------");
    printf("\nOperand:");
    for (Index = 0; Index < TEST_FUNCTIONSET4_LENGTH; Index++)
        printf("%u ", Operand[Index]);
    printf("\nModule:");
    for (Index = 0; Index < TEST_FUNCTIONSET4_LENGTH; Index++)
        printf("%u ", Module[Index]);
    printf("\nResult:");
    for (Index = 0; Index < TEST_FUNCTIONSET4_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 模逆运算 */
    /*************************************************************************/
    Cflint_SetValue(Module, TEST_FUNCTIONSET4_LENGTH * 2, 0);
    for (Index = 0; Index < TEST_FUNCTIONSET4_LENGTH; Index++) {
        Operand[Index] = TEST_FUNCTIONSET4_LENGTH - Index;
        Module[Index]  = TEST_FUNCTIONSET4_LENGTH - Index;
    }
    Module[0] = 1;
    /* 模逆运算 */
    {
        CFLINT_TYPE  Temp1 [TEST_FUNCTIONSET4_LENGTH] = {0};
        CFLINT_TYPE  Temp2 [TEST_FUNCTIONSET4_LENGTH] = {0};
        CFLINT_TYPE  Temp3 [TEST_FUNCTIONSET4_LENGTH] = {0};
        CFLINT_TYPE  Temp4 [TEST_FUNCTIONSET4_LENGTH] = {0};
        CFLINT_TYPE *Temp[4] = {Temp1, Temp2, Temp3, Temp4};
        Cflint_ModuloInverse(Result, Operand, Module,
                             Temp, TEST_FUNCTIONSET4_LENGTH);
    }
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_ModuloInverse:::");
    printf("\n---------------------------------------------------------------");
    printf("\nOperand:");
    for (Index = 0; Index < TEST_FUNCTIONSET4_LENGTH; Index++)
        printf("%u ", Operand[Index]);
    printf("\nModule:");
    for (Index = 0; Index < TEST_FUNCTIONSET4_LENGTH; Index++)
        printf("%u ", Module[Index]);
    printf("\nResult:");
    for (Index = 0; Index < TEST_FUNCTIONSET4_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /* 注:Module和Operand如果不互素,模逆不存在 */
    printf("(Result * Operand) % Module:::Result:\n");
    CFLINT_TYPE Temp1 [TEST_FUNCTIONSET4_LENGTH * 2] = {0};
    Cflint_Multiply(Temp1, Result, Operand, TEST_FUNCTIONSET4_LENGTH);
    Cflint_Modulo(Temp1, Temp1, Module, TEST_FUNCTIONSET4_LENGTH * 2);
    Cflint_Copy(Result, Temp1, TEST_FUNCTIONSET4_LENGTH);
    for (Index = 0; Index < TEST_FUNCTIONSET4_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void Test_CflintFunctionSet5(void)
{
    uint32_t Index = 0;
    #define TEST_FUNCTIONSET5_LENGTH     5
    CFLINT_TYPE Result[TEST_FUNCTIONSET5_LENGTH]  = {0};
    CFLINT_TYPE Result1[TEST_FUNCTIONSET5_LENGTH] = {0};
    CFLINT_TYPE A[TEST_FUNCTIONSET5_LENGTH] = {0};
    CFLINT_TYPE B[TEST_FUNCTIONSET5_LENGTH] = {0};
    /* 因为运算中涉及到乘和差运算,所以这是理论最大空间 */
    CFLINT_TYPE X[(TEST_FUNCTIONSET5_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Y[(TEST_FUNCTIONSET5_LENGTH + 1) * 2] = {0};
    /* 开辟出来的用于解算结果正确性的空间,本身无用 */
    CFLINT_TYPE Temp5[(TEST_FUNCTIONSET5_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Temp6[(TEST_FUNCTIONSET5_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Temp7[(TEST_FUNCTIONSET5_LENGTH + 1) * 4] = {0};
    CFLINT_TYPE Temp8[(TEST_FUNCTIONSET5_LENGTH + 1) * 4] = {0};
    CFLINT_TYPE X_Flag = 0, Y_Flag = 0;
    bool Check = 0;
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET5_LENGTH; Index++) {
        A[Index] = TEST_FUNCTIONSET5_LENGTH - Index;
        B[Index] = TEST_FUNCTIONSET5_LENGTH - Index;
    }
    B[0] = 1;
    /*************************************************************************/
    
    /*************************************************************************/
    /* 素数判别检查 */
    {
        CFLINT_TYPE Result[TEST_FUNCTIONSET5_LENGTH] = {1, 0, 1};
        CFLINT_TYPE  Temp1[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE  Temp2[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE  Temp3[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE *Temp[3] = {Temp1, Temp2, Temp3};
        Check = Cflint_IsPrime(Result, Temp, TEST_FUNCTIONSET5_LENGTH);
    }
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_IsPrime:::%d", Check);
    printf("\n---------------------------------------------------------------");
    /*************************************************************************/
    /* 素数判别检查 */
    {
        CFLINT_TYPE Result[TEST_FUNCTIONSET5_LENGTH] = {1, 1, 1};
        CFLINT_TYPE  Temp1[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE  Temp2[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE  Temp3[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE *Temp[3] = {Temp1, Temp2, Temp3};
        Check = Cflint_IsPrime(Result, Temp, TEST_FUNCTIONSET5_LENGTH);
    }
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_IsPrime:::%d", Check);
    printf("\n---------------------------------------------------------------");
    /*************************************************************************/
    
    /* 欧几里得互素检查运算 */
    /*************************************************************************/
    {
        CFLINT_TYPE  Temp1[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE  Temp2[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE  Temp3[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE *Temp[3] = {Temp1, Temp2, Temp3};
        Check = Cflint_GCDCheck(A, B, Temp, TEST_FUNCTIONSET5_LENGTH);
    }
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_GCDCheck:::");
    printf("\n---------------------------------------------------------------");
    printf("\nA:");
    for (Index = 0; Index < TEST_FUNCTIONSET5_LENGTH; Index++)
        printf("%u ", A[Index]);
    printf("\nB:");
    for (Index = 0; Index < TEST_FUNCTIONSET5_LENGTH; Index++)
        printf("%u ", B[Index]);
    printf("\nResult:%u", Check);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 扩展欧几里得运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET5_LENGTH; Index++) {
        A[Index] = TEST_FUNCTIONSET5_LENGTH - Index;
        B[Index] = TEST_FUNCTIONSET5_LENGTH - Index;
    }
    B[0] = 1;
    /*************************************************************************/
    /* 扩展欧几里得运算 */
    {
        CFLINT_TYPE  Temp1[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE  Temp2[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE  Temp3[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE  Temp4[(TEST_FUNCTIONSET5_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE  Temp5[(TEST_FUNCTIONSET5_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE  Temp6[(TEST_FUNCTIONSET5_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE  Temp7[(TEST_FUNCTIONSET5_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE *Temp[7] = {Temp1, Temp2, Temp3, Temp4,
                                Temp5, Temp6, Temp7};
        Cflint_GCDExtend(A, B, Result, X, &X_Flag, Y, &Y_Flag,
                         Temp, TEST_FUNCTIONSET5_LENGTH);
    }
    /*************************************************************************/
    Cflint_SetValue(Temp5, (TEST_FUNCTIONSET5_LENGTH + 1) * 2, 0);
    Cflint_SetValue(Temp6, (TEST_FUNCTIONSET5_LENGTH + 1) * 2, 0);
    Cflint_SetValue(Temp7, (TEST_FUNCTIONSET5_LENGTH + 1) * 4, 0);
    Cflint_SetValue(Temp8, (TEST_FUNCTIONSET5_LENGTH + 1) * 4, 0);
    Cflint_Copy(Temp5, A, TEST_FUNCTIONSET5_LENGTH);
    Cflint_Copy(Temp6, B, TEST_FUNCTIONSET5_LENGTH);
    Cflint_Multiply(Temp7, Temp5, X, (TEST_FUNCTIONSET5_LENGTH + 1) * 2);
    Cflint_Multiply(Temp8, Temp6, Y, (TEST_FUNCTIONSET5_LENGTH + 1) * 2);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_GCDExtend:::");
    printf("\n---------------------------------------------------------------");
    printf("\nA:");
    for (Index = 0; Index < TEST_FUNCTIONSET5_LENGTH; Index++)
        printf("%u ", A[Index]);
    printf("\nB:");
    for (Index = 0; Index < TEST_FUNCTIONSET5_LENGTH; Index++)
        printf("%u ", B[Index]);
    printf("\nResult:");
    for (Index = 0; Index < TEST_FUNCTIONSET5_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\nX_Flag:%d, X:", X_Flag);
    for (Index = 0; Index < (TEST_FUNCTIONSET5_LENGTH + 1) * 2; Index++)
        printf("%u ", X[Index]);
    printf("\nY_Flag:%d, Y:", Y_Flag);
    for (Index = 0; Index < (TEST_FUNCTIONSET5_LENGTH + 1) * 2; Index++)
        printf("%u ", Y[Index]);
    printf("\nA*X:");
    for (Index = 0; Index < (TEST_FUNCTIONSET5_LENGTH + 1) * 2; Index++)
        printf("%u ", Temp7[Index]);
    printf("\nB*Y:");
    for (Index = 0; Index < (TEST_FUNCTIONSET5_LENGTH + 1) * 2; Index++)
        printf("%u ", Temp8[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 欧几里得乘法逆 */
    /*************************************************************************/
    {
        CFLINT_TYPE  Temp1[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE  Temp2[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE  Temp3[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE  Temp4[TEST_FUNCTIONSET5_LENGTH] = {0};
        CFLINT_TYPE  Temp5[(TEST_FUNCTIONSET5_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE  Temp6[(TEST_FUNCTIONSET5_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE  Temp7[(TEST_FUNCTIONSET5_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE  Temp8[(TEST_FUNCTIONSET5_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE *Temp[8] = {Temp1, Temp2, Temp3, Temp4,
                                Temp5, Temp6, Temp7, Temp8};
        Cflint_GCDInverse(A, B, Result, Result1,
                          Temp, TEST_FUNCTIONSET5_LENGTH);
    }
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_GCDInverse:::");
    printf("\n---------------------------------------------------------------");
    printf("\nA:");
    for (Index = 0; Index < TEST_FUNCTIONSET5_LENGTH; Index++)
        printf("%u ", A[Index]);
    printf("\nB:");
    for (Index = 0; Index < TEST_FUNCTIONSET5_LENGTH; Index++)
        printf("%u ", B[Index]);
    printf("\nGCD:");
    for (Index = 0; Index < TEST_FUNCTIONSET5_LENGTH; Index++)
        printf("%u ", Result1[Index]);
    printf("\nInverse:");
    for (Index = 0; Index < TEST_FUNCTIONSET5_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /* 注:Module和Operand如果不互素,模逆不存在 */
    printf("(Result * A) % B:::Result:\n");
    Cflint_Multiply(Temp5, Result, A, TEST_FUNCTIONSET5_LENGTH);
    Cflint_SetValue(Temp6, TEST_FUNCTIONSET5_LENGTH * 2, 0);
    Cflint_Copy(Temp6, B, TEST_FUNCTIONSET5_LENGTH);
    Cflint_Modulo(Temp5, Temp5, Temp6, TEST_FUNCTIONSET5_LENGTH * 2);
    Cflint_Copy(Result, Temp5, TEST_FUNCTIONSET5_LENGTH);
    for (Index = 0; Index < TEST_FUNCTIONSET5_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void Test_CflintFunctionSet6(void)
{
    uint32_t Index = 0;
    #define TEST_FUNCTIONSET6_LENGTH     5
    CFLINT_TYPE Result[TEST_FUNCTIONSET6_LENGTH] = {0};
    CFLINT_TYPE Operand[TEST_FUNCTIONSET6_LENGTH] = {0};
    int8_t Result1 = 0;
    
    /* 2次方根整数部运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET6_LENGTH; Index++)
        Operand[Index] = TEST_FUNCTIONSET6_LENGTH - Index;
    /*************************************************************************/
    /* 2次方根整数部运算 */
    {
        CFLINT_TYPE  Temp1[TEST_FUNCTIONSET6_LENGTH] = {0};
        CFLINT_TYPE  Temp2[TEST_FUNCTIONSET6_LENGTH] = {0};
        CFLINT_TYPE  Temp3[TEST_FUNCTIONSET6_LENGTH] = {0};
        CFLINT_TYPE  Temp4[TEST_FUNCTIONSET6_LENGTH] = {0};
        CFLINT_TYPE *Temp[4] = {Temp1, Temp2, Temp3, Temp4};
        Cflint_Root2Integer(Result, Operand, Temp, TEST_FUNCTIONSET6_LENGTH);
    }
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Root2Integer:::");
    printf("\n---------------------------------------------------------------");
    printf("\nResult:");
    for (Index = 0; Index < TEST_FUNCTIONSET6_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\nOperand:");
    for (Index = 0; Index < TEST_FUNCTIONSET6_LENGTH; Index++)
        printf("%u ", Operand[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 2次方数判别检查 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET6_LENGTH; Index++)
        Operand[Index] = TEST_FUNCTIONSET6_LENGTH - Index;
    Operand[0] = 1;
    Operand[1] = 2;
    /*************************************************************************/
    {
        CFLINT_TYPE  Temp1[TEST_FUNCTIONSET6_LENGTH * 2] = {0};
        CFLINT_TYPE  Temp2[TEST_FUNCTIONSET6_LENGTH] = {0};
        CFLINT_TYPE  Temp3[TEST_FUNCTIONSET6_LENGTH] = {0};
        CFLINT_TYPE  Temp4[TEST_FUNCTIONSET6_LENGTH] = {0};
        CFLINT_TYPE *Temp[4] = {Temp1, Temp2, Temp3, Temp4};
        Result1 = Cflint_Root2Check(Result, Operand, Temp,
                                    TEST_FUNCTIONSET6_LENGTH);
    }
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Root2Check:::");
    printf("\n---------------------------------------------------------------");
    printf("\nResult:");
    for (Index = 0; Index < TEST_FUNCTIONSET6_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\nOperand:");
    for (Index = 0; Index < TEST_FUNCTIONSET6_LENGTH; Index++)
        printf("%u ", Operand[Index]);
    printf("\nResult:%d", Result1);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 符号Jacobi(Operand1/Operand2)计算 */
    /*************************************************************************/
    Result[0]  = 2;
    Operand[0] = 7;
    /*************************************************************************/
    /* 符号Jacobi(Operand1/Operand2)计算 */
    {
        CFLINT_TYPE  Temp1[TEST_FUNCTIONSET6_LENGTH] = {0};
        CFLINT_TYPE  Temp2[TEST_FUNCTIONSET6_LENGTH] = {0};
        CFLINT_TYPE  Temp3[TEST_FUNCTIONSET6_LENGTH] = {0};
        CFLINT_TYPE *Temp[3] = {Temp1, Temp2, Temp3};
        Result1 = Cflint_JacobiFlag(Result, Operand, Temp, 1);
    }
    printf("\n-------------------------------------------------------------\n");
    printf("Jacobi(%d/%d):Result:%d",Result[0], Operand[0], Result1);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void Test_CflintFunctionSet7(void)
{
    uint32_t Index = 0;
    #define TEST_FUNCTIONSET7_LENGTH     3
    CFLINT_TYPE Result[TEST_FUNCTIONSET7_LENGTH] = {0};
    CFLINT_TYPE Module[TEST_FUNCTIONSET7_LENGTH] = {0};
    CFLINT_TYPE Operand[TEST_FUNCTIONSET7_LENGTH] = {0};
    /* 开辟出来的用于解算结果正确性的空间,本身无用 */
    CFLINT_TYPE Temp1[(TEST_FUNCTIONSET7_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Temp2[(TEST_FUNCTIONSET7_LENGTH + 1) * 2] = {0};
    int8_t Result1 = 0;
    
    /* 二次剩余计算:((Result**2) % Operand2 == Operand1 % Operand2) */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET7_LENGTH; Index++)
        Module[Index] = 1;
    for (Index = 0; Index < TEST_FUNCTIONSET7_LENGTH; Index++)
        Operand[Index] = 0;
    Module[1] = 0;
    /*************************************************************************/
    while (1) {
        Cflint_AdditionBit(Operand, TEST_FUNCTIONSET7_LENGTH, 1);
        /*********************************************************************/
        if (Cflint_Compare(Operand, Module, TEST_FUNCTIONSET7_LENGTH) == 1)
            break;
        /*********************************************************************/
        /* 二次剩余计算:((Result**2) % Operand2 == Operand1 % Operand2) */
        {
            CFLINT_TYPE  Temp1[TEST_FUNCTIONSET7_LENGTH] = {0};
            CFLINT_TYPE  Temp2[TEST_FUNCTIONSET7_LENGTH] = {0};
            CFLINT_TYPE  Temp3[TEST_FUNCTIONSET7_LENGTH] = {0};
            CFLINT_TYPE  Temp4[TEST_FUNCTIONSET7_LENGTH] = {0};
            CFLINT_TYPE  Temp5[TEST_FUNCTIONSET7_LENGTH] = {0};
            CFLINT_TYPE  Temp6[TEST_FUNCTIONSET7_LENGTH * 2] = {0};
            CFLINT_TYPE  Temp7[TEST_FUNCTIONSET7_LENGTH * 2] = {0};
            CFLINT_TYPE  Temp8[TEST_FUNCTIONSET7_LENGTH * 2] = {0};
            CFLINT_TYPE  Temp9[TEST_FUNCTIONSET7_LENGTH * 2] = {0};
            CFLINT_TYPE  Temp10[TEST_FUNCTIONSET7_LENGTH * 2] = {0};
            CFLINT_TYPE *Temp[10] = {Temp1, Temp2, Temp3, Temp4, Temp5,
                                     Temp6, Temp7, Temp8, Temp9, Temp10};
            /* 二次剩余计算:((Result**2) % Operand2 == Operand1 % Operand2) */
            Result1 = Cflint_ModuloP1Root2(Operand, Module, Result,
                                           Temp, TEST_FUNCTIONSET7_LENGTH);
            if (Result1 == 0)
                continue;
        }
        printf("\n---------------------------------------------------------\n");
        printf("Cflint_ModuloP1Root2:::");
        printf("\n-----------------------------------------------------------");
        printf("\nModule:");
        for (Index = 0; Index < TEST_FUNCTIONSET7_LENGTH; Index++)
            printf("%u ", Module[Index]);
        printf("\nResult:");
        for (Index = 0; Index < TEST_FUNCTIONSET7_LENGTH; Index++)
            printf("%u ", Result[Index]);
        printf("\nOperand:");
        for (Index = 0; Index < TEST_FUNCTIONSET7_LENGTH; Index++)
            printf("%u ", Operand[Index]);
        /*********************************************************************/
        Cflint_Square(Temp1, Result, TEST_FUNCTIONSET7_LENGTH);
        Cflint_SetValue(Temp2, TEST_FUNCTIONSET7_LENGTH * 2, 0);
        Cflint_Copy(Temp2, Module, TEST_FUNCTIONSET7_LENGTH);
        Cflint_Modulo(Temp1, Temp1, Temp2, TEST_FUNCTIONSET7_LENGTH * 2);
        printf("\n(Result**2)(Mod)Module:");
        for (Index = 0; Index < TEST_FUNCTIONSET7_LENGTH; Index++)
            printf("%u ", Temp1[Index]);
        /*********************************************************************/
        Cflint_Modulo(Temp1, Operand, Module, TEST_FUNCTIONSET7_LENGTH);
        printf("\n(Operand)(Mod)Module:");
        for (Index = 0; Index < TEST_FUNCTIONSET7_LENGTH; Index++)
            printf("%u ", Temp1[Index]);
        printf("\n---------------------------------------------------------\n");
        /*********************************************************************/
    }
    /*************************************************************************/
    
    /* 二次剩余计算:((Result**2) % Operand2 == Operand1 % Operand2) */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET7_LENGTH; Index++)
        Module[Index] = 1;
    for (Index = 0; Index < TEST_FUNCTIONSET7_LENGTH; Index++)
        Operand[Index] = 0;
    //Module[0] = 7;
    /*************************************************************************/
    while (1) {
        Cflint_AdditionBit(Operand, TEST_FUNCTIONSET7_LENGTH, 1);
        /*********************************************************************/
        if (Cflint_Compare(Operand, Module, TEST_FUNCTIONSET7_LENGTH) == 1)
            break;
        /*********************************************************************/
        /* 二次剩余计算:((Result**2) % Operand2 == Operand1 % Operand2) */
        {
            CFLINT_TYPE  Temp1[TEST_FUNCTIONSET7_LENGTH] = {0};
            CFLINT_TYPE  Temp2[TEST_FUNCTIONSET7_LENGTH] = {0};
            CFLINT_TYPE  Temp3[TEST_FUNCTIONSET7_LENGTH] = {0};
            CFLINT_TYPE  Temp4[TEST_FUNCTIONSET7_LENGTH] = {0};
            CFLINT_TYPE  Temp5[TEST_FUNCTIONSET7_LENGTH] = {0};
            CFLINT_TYPE  Temp6[TEST_FUNCTIONSET7_LENGTH * 2] = {0};
            CFLINT_TYPE  Temp7[TEST_FUNCTIONSET7_LENGTH * 2] = {0};
            CFLINT_TYPE  Temp8[TEST_FUNCTIONSET7_LENGTH * 2] = {0};
            CFLINT_TYPE  Temp9[TEST_FUNCTIONSET7_LENGTH * 2] = {0};
            CFLINT_TYPE  Temp10[TEST_FUNCTIONSET7_LENGTH * 2] = {0};
            CFLINT_TYPE *Temp[10] = {Temp1, Temp2, Temp3, Temp4, Temp5,
                                     Temp6, Temp7, Temp8, Temp9, Temp10};
            /* 二次剩余计算:((Result**2) % Operand2 == Operand1 % Operand2) */
            Result1 = Cflint_ModuloP1Root2(Operand, Module, Result,
                                           Temp, TEST_FUNCTIONSET7_LENGTH);
            if (Result1 == 0)
                continue;
        }
        printf("\n---------------------------------------------------------\n");
        printf("Cflint_ModuloP1Root2:::");
        printf("\n-----------------------------------------------------------");
        printf("\nModule:");
        for (Index = 0; Index < TEST_FUNCTIONSET7_LENGTH; Index++)
            printf("%u ", Module[Index]);
        printf("\nResult:");
        for (Index = 0; Index < TEST_FUNCTIONSET7_LENGTH; Index++)
            printf("%u ", Result[Index]);
        printf("\nOperand:");
        for (Index = 0; Index < TEST_FUNCTIONSET7_LENGTH; Index++)
            printf("%u ", Operand[Index]);
        /*********************************************************************/
        Cflint_Square(Temp1, Result, TEST_FUNCTIONSET7_LENGTH);
        Cflint_SetValue(Temp2, TEST_FUNCTIONSET7_LENGTH * 2, 0);
        Cflint_Copy(Temp2, Module, TEST_FUNCTIONSET7_LENGTH);
        Cflint_Modulo(Temp1, Temp1, Temp2, TEST_FUNCTIONSET7_LENGTH * 2);
        printf("\n(Result**2)(Mod)Module:");
        for (Index = 0; Index < TEST_FUNCTIONSET7_LENGTH; Index++)
            printf("%u ", Temp1[Index]);
        /*********************************************************************/
        Cflint_Modulo(Temp1, Operand, Module, TEST_FUNCTIONSET7_LENGTH);
        printf("\n(Operand)(Mod)Module:");
        for (Index = 0; Index < TEST_FUNCTIONSET7_LENGTH; Index++)
            printf("%u ", Temp1[Index]);
        printf("\n---------------------------------------------------------\n");
        /*********************************************************************/
    }
    /*************************************************************************/
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void Test_CflintFunctionSet8(void)
{
    uint32_t Index = 0;
    #define TEST_FUNCTIONSET8_LENGTH     2
    CFLINT_TYPE Operand[TEST_FUNCTIONSET8_LENGTH * 2] = {0};
    CFLINT_TYPE Result[TEST_FUNCTIONSET8_LENGTH * 2] = {0};
    CFLINT_TYPE Module1[TEST_FUNCTIONSET8_LENGTH] = {0};
    CFLINT_TYPE Module2[TEST_FUNCTIONSET8_LENGTH] = {0};
    /* 开辟出来的用于解算结果正确性的空间,本身无用 */
    CFLINT_TYPE Temp0[TEST_FUNCTIONSET8_LENGTH * 4] = {0};
    CFLINT_TYPE Temp1[TEST_FUNCTIONSET8_LENGTH * 4] = {0};
    CFLINT_TYPE Temp2[TEST_FUNCTIONSET8_LENGTH * 4] = {0};
    int8_t Result1 = 0;
    
    /* 意外备注:当Length为3时,使用Set7中的俩个数找不到一个满足要求的值 */
    
    /* 二次剩余计算:((Result**2) % Operand2 == Operand1 % Operand2) */
    /*************************************************************************/
    for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH; Index++)
        Module1[Index] = 1;
    //Module1[1] = 0;
    for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH; Index++)
        Module2[Index] = 1;
    Module2[0] = 7;
    for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
        Operand[Index] = 0;
    /*************************************************************************/
    Cflint_Multiply(Temp0, Module1, Module2, TEST_FUNCTIONSET8_LENGTH);
    /*************************************************************************/
    {
        CFLINT_TYPE Result1[3] = {1, 0, 1};
        CFLINT_TYPE Result2[3] = {7, 1, 1};
        CFLINT_TYPE  Temp1[3] = {0};
        CFLINT_TYPE  Temp2[3] = {0};
        CFLINT_TYPE  Temp3[3] = {0};
        CFLINT_TYPE *Temp[3] = {Temp1, Temp2, Temp3};
        bool Check1 = Cflint_IsPrime(Result1, Temp, 3);
        bool Check2 = Cflint_IsPrime(Result2, Temp, 3);
        printf("\n---------------------------------------------------------\n");
        printf("Cflint_IsPrime:::%d\n", Check1);
        printf("Cflint_IsPrime:::%d\n", Check2);
        printf("\n-----------------------------------------------------------");
    }
    /*************************************************************************/
    {
        CFLINT_TYPE  Temp1[TEST_FUNCTIONSET8_LENGTH] = {0};
        CFLINT_TYPE  Temp2[TEST_FUNCTIONSET8_LENGTH] = {0};
        CFLINT_TYPE  Temp3[TEST_FUNCTIONSET8_LENGTH] = {0};
        CFLINT_TYPE *Temp[3] = {Temp1, Temp2, Temp3};
        bool Result = Cflint_GCDCheck(Module1, Module2, Temp,
                                      TEST_FUNCTIONSET8_LENGTH);
        printf("\n---------------------------------------------------------\n");
        printf("Cflint_GCDCheck:::%d", Result);
        printf("\n-----------------------------------------------------------");
    }
    /*************************************************************************/
#if 0
    /*************************************************************************/
    {   /* 这是通过下面的低效查找查到的一个同时可以通过俩个二次剩余的数 */
        Operand[0] = 1;Operand[1] = 1;Operand[2] = 1;Operand[3] = 1;
    }
    /*************************************************************************/
    /* 二次剩余计算:((Result**2) % Operand2 == Operand1 % Operand2) */
    {
        CFLINT_TYPE  Temp1[TEST_FUNCTIONSET8_LENGTH] = {0};
        CFLINT_TYPE  Temp2[TEST_FUNCTIONSET8_LENGTH] = {0};
        CFLINT_TYPE  Temp3[TEST_FUNCTIONSET8_LENGTH] = {0};
        CFLINT_TYPE  Temp4[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE  Temp5[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE  Temp6[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE  Temp7[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE  Temp8[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE  Temp9[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE  Temp10[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE  Temp11[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
        CFLINT_TYPE  Temp12[TEST_FUNCTIONSET8_LENGTH * 4] = {0};
        CFLINT_TYPE  Temp13[TEST_FUNCTIONSET8_LENGTH * 4] = {0};
        CFLINT_TYPE *Temp[13] = {Temp1,  Temp2,  Temp3, Temp4, Temp5,
                                 Temp6,  Temp7,  Temp8, Temp9, Temp10,
                                 Temp11, Temp12, Temp13};
        /* 二次剩余计算:((Result**2) % (Operand2 * Operand3) ==  */
        /*               (Operand1)  % (Operand2 * Operand3))    */
        Result1 = Cflint_Modulo1Root2(Operand, Module1, Module2, Result,
                                      Temp, TEST_FUNCTIONSET8_LENGTH);
    }
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Modulo1Root2:::");
    printf("\n---------------------------------------------------------------");
    printf("\nResult:%d", Result1);
    printf("\nModule1:");
    for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH; Index++)
        printf("%u ", Module1[Index]);
    printf("\nModule2:");
    for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH; Index++)
        printf("%u ", Module2[Index]);
    printf("\nModule1*Module2:");
    for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
        printf("%u ", Temp0[Index]);
    printf("\nResult:");
    for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
        printf("%u ", Result[Index]);
    printf("\nOperand:");
    for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
        printf("%u ", Operand[Index]);
    /*************************************************************************/
    Cflint_SetValue(Temp2, TEST_FUNCTIONSET8_LENGTH * 2, 0);
    Cflint_Copy(Temp2, Module1, TEST_FUNCTIONSET8_LENGTH);
    Cflint_Modulo(Temp1, Operand, Temp2, TEST_FUNCTIONSET8_LENGTH * 2);
    printf("\n(Operand)(Mod)(Module1):");
    for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
        printf("%u ", Temp1[Index]);
    /*************************************************************************/
    Cflint_SetValue(Temp2, TEST_FUNCTIONSET8_LENGTH * 2, 0);
    Cflint_Copy(Temp2, Module2, TEST_FUNCTIONSET8_LENGTH);
    Cflint_Modulo(Temp1, Operand, Temp2, TEST_FUNCTIONSET8_LENGTH * 2);
    printf("\n(Operand)(Mod)(Module2):");
    for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
        printf("%u ", Temp1[Index]);
    /*************************************************************************/
    Cflint_Square(Temp1, Result, TEST_FUNCTIONSET8_LENGTH * 2);
    Cflint_SetValue(Temp2, TEST_FUNCTIONSET8_LENGTH * 4, 0);
    Cflint_Copy(Temp2, Temp0, TEST_FUNCTIONSET8_LENGTH * 2);
    Cflint_Modulo(Temp1, Temp1, Temp2, TEST_FUNCTIONSET8_LENGTH * 2);
    printf("\n(Result**2)(Mod)(Module1 * Module2):");
    for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
        printf("%u ", Temp1[Index]);
    /*************************************************************************/
    Cflint_SetValue(Temp2, TEST_FUNCTIONSET8_LENGTH * 4, 0);
    Cflint_Copy(Temp2, Operand, TEST_FUNCTIONSET8_LENGTH);
    Cflint_Modulo(Temp1, Temp2, Temp0, TEST_FUNCTIONSET8_LENGTH * 2);
    printf("\n(Operand)(Mod)(Module1 * Module2):");
    for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
        printf("%u ", Temp1[Index]);
    printf("\n---------------------------------------------------------\n");
    /*************************************************************************/
#else
    /*************************************************************************/
    while (1) {
        Cflint_AdditionBit(Operand, TEST_FUNCTIONSET8_LENGTH * 2, 1);
        /*********************************************************************/
        if (Cflint_Compare(Operand, Temp0, TEST_FUNCTIONSET8_LENGTH * 2) == 1)
            break;
        /*********************************************************************/
        /* 二次剩余计算:((Result**2) % Operand2 == Operand1 % Operand2) */
        {
            CFLINT_TYPE  Temp1[TEST_FUNCTIONSET8_LENGTH] = {0};
            CFLINT_TYPE  Temp2[TEST_FUNCTIONSET8_LENGTH] = {0};
            CFLINT_TYPE  Temp3[TEST_FUNCTIONSET8_LENGTH] = {0};
            CFLINT_TYPE  Temp4[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
            CFLINT_TYPE  Temp5[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
            CFLINT_TYPE  Temp6[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
            CFLINT_TYPE  Temp7[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
            CFLINT_TYPE  Temp8[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
            CFLINT_TYPE  Temp9[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
            CFLINT_TYPE  Temp10[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
            CFLINT_TYPE  Temp11[(TEST_FUNCTIONSET8_LENGTH + 1) * 2] = {0};
            CFLINT_TYPE  Temp12[TEST_FUNCTIONSET8_LENGTH * 4] = {0};
            CFLINT_TYPE  Temp13[TEST_FUNCTIONSET8_LENGTH * 4] = {0};
            CFLINT_TYPE *Temp[13] = {Temp1,  Temp2,  Temp3, Temp4, Temp5,
                                     Temp6,  Temp7,  Temp8, Temp9, Temp10,
                                     Temp11, Temp12, Temp13};
            /* 二次剩余计算:((Result**2) % (Operand2 * Operand3) ==  */
            /*               (Operand1)  % (Operand2 * Operand3))    */
            Result1 = Cflint_Modulo1Root2(Operand, Module1, Module2, Result,
                                          Temp, TEST_FUNCTIONSET8_LENGTH);
            {
                static count = 0;
                /* 事实上很难找 */
                if (count++ > 1000000) {
                    count = 0;
                printf("\nOperand:");
                for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
                    printf("%u ", Operand[Index]);
                }
            }
            if (Result1 == 0)
                continue;
            if (Cflint_IsZero(Result, TEST_FUNCTIONSET8_LENGTH) == true)
                continue;
        }
        printf("\n---------------------------------------------------------\n");
        printf("Cflint_Modulo1Root2:::");
        printf("\n-----------------------------------------------------------");
        printf("\nModule1:");
        for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH; Index++)
            printf("%u ", Module1[Index]);
        printf("\nModule2:");
        for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH; Index++)
            printf("%u ", Module2[Index]);
        printf("\nModule1*Module2:");
        for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
            printf("%u ", Temp0[Index]);
        printf("\nResult:");
        for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
            printf("%u ", Result[Index]);
        printf("\nOperand:");
        for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
            printf("%u ", Operand[Index]);
        /*********************************************************************/
        Cflint_SetValue(Temp2, TEST_FUNCTIONSET8_LENGTH * 2, 0);
        Cflint_Copy(Temp2, Module1, TEST_FUNCTIONSET8_LENGTH);
        Cflint_Modulo(Temp1, Operand, Temp2, TEST_FUNCTIONSET8_LENGTH * 2);
        printf("\n(Operand)(Mod)(Module1):");
        for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
            printf("%u ", Temp1[Index]);
        /*********************************************************************/
        Cflint_SetValue(Temp2, TEST_FUNCTIONSET8_LENGTH * 2, 0);
        Cflint_Copy(Temp2, Module2, TEST_FUNCTIONSET8_LENGTH);
        Cflint_Modulo(Temp1, Operand, Temp2, TEST_FUNCTIONSET8_LENGTH * 2);
        printf("\n(Operand)(Mod)(Module2):");
        for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
            printf("%u ", Temp1[Index]);
        /*********************************************************************/
        Cflint_Square(Temp1, Result, TEST_FUNCTIONSET8_LENGTH * 2);
        Cflint_SetValue(Temp2, TEST_FUNCTIONSET8_LENGTH * 4, 0);
        Cflint_Copy(Temp2, Temp0, TEST_FUNCTIONSET8_LENGTH * 2);
        Cflint_Modulo(Temp1, Temp1, Temp2, TEST_FUNCTIONSET8_LENGTH * 2);
        printf("\n(Result**2)(Mod)(Module1 * Module2):");
        for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
            printf("%u ", Temp1[Index]);
        /*********************************************************************/
        Cflint_SetValue(Temp2, TEST_FUNCTIONSET8_LENGTH * 4, 0);
        Cflint_Copy(Temp2, Operand, TEST_FUNCTIONSET8_LENGTH * 2);
        Cflint_Modulo(Temp1, Temp2, Temp0, TEST_FUNCTIONSET8_LENGTH * 2);
        printf("\n(Operand)(Mod)(Module1 * Module2):");
        for (Index = 0; Index < TEST_FUNCTIONSET8_LENGTH * 2; Index++)
            printf("%u ", Temp1[Index]);
        printf("\n---------------------------------------------------------\n");
        /*********************************************************************/
    }
#endif
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 测试蒙哥马利模(未完成,测试未通过) */
void Test_Mentgomery(void)
{
#if 0
    uint32_t Index = 0;
    #define TEST_MENTGOMERY_LENGTH    3
    CFLINT_TYPE Result [TEST_MENTGOMERY_LENGTH] = {0};
    CFLINT_TYPE X [TEST_MENTGOMERY_LENGTH] = {0};
    CFLINT_TYPE Y [TEST_MENTGOMERY_LENGTH] = {0};
    CFLINT_TYPE N [TEST_MENTGOMERY_LENGTH * 2] = {0};
    CFLINT_TYPE R [TEST_MENTGOMERY_LENGTH * 2] = {0};
    CFLINT_TYPE RR_N [TEST_MENTGOMERY_LENGTH] = {0};
    CFLINT_TYPE RP [(TEST_MENTGOMERY_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE NP [(TEST_MENTGOMERY_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE RP_Flag = 0, NP_Flag = 0;
    CFLINT_TYPE Temp1 [(TEST_MENTGOMERY_LENGTH + 1) * 4] = {0};
    CFLINT_TYPE Temp2 [(TEST_MENTGOMERY_LENGTH + 1) * 4] = {0};
    CFLINT_TYPE Temp3 [(TEST_MENTGOMERY_LENGTH + 1) * 4] = {0};
    CFLINT_TYPE Temp4 [(TEST_MENTGOMERY_LENGTH + 1) * 4] = {0};
    CFLINT_TYPE Temp5 [(TEST_MENTGOMERY_LENGTH + 1) * 4] = {0};
    /*************************************************************************/
    for (Index = 0; Index < TEST_MENTGOMERY_LENGTH; Index++) {
        X[Index] = TEST_MENTGOMERY_LENGTH - Index;
        Y[Index] = TEST_MENTGOMERY_LENGTH - Index;
        N[Index] = TEST_MENTGOMERY_LENGTH - Index;
    }
    Y[0] = 1;
    N[0] = 1;
    /*************************************************************************/
    
    /* 预计算及其验证 */
    /*************************************************************************/
    /* 特例:除数为0检查 */
    if (Cflint_IsZero(N, TEST_MENTGOMERY_LENGTH) == true) {
        Cflint_SetValue(Result, TEST_MENTGOMERY_LENGTH, 0);
        return;
    }
    /* 计算R_Index */
    int64_t R_Index = Cflint_Numbers2(N, TEST_MENTGOMERY_LENGTH) + 1;
    /* 一个数的0次幂为1 */
    if (R_Index == 0) {
        Cflint_SetValue(Result, TEST_MENTGOMERY_LENGTH, 0);
        Cflint_AdditionBit(Result, TEST_MENTGOMERY_LENGTH, 1);
        return;
    }
    /* 生成R */
    Cflint_SetValue(R, TEST_MENTGOMERY_LENGTH * 2, 0);
    Cflint_AdditionBit(R, TEST_MENTGOMERY_LENGTH * 2, 1);
    Cflint_ShiftLeft2(R, TEST_MENTGOMERY_LENGTH * 2, R_Index);
    /* 检查GCD(R,N) == 1 */
    if (Cflint_GCDCheck(R, N, Temp1, Temp2, Temp3,
                        TEST_MENTGOMERY_LENGTH * 2) == false) {
        Cflint_SetValue(Result, TEST_MENTGOMERY_LENGTH, 0);
        return;
    }
    /* 4.计算RR' + NN' = GCD(R,N) == 1 */
    // Cflint_ModuleInverse(RP, R, N, Temp1, Temp2, Temp3, Temp4,
    //                      TEST_MENTGOMERY_LENGTH);
    // Cflint_ModuleInverse(NP, N, R, Temp1, Temp2, Temp3, Temp4,
    //                      TEST_MENTGOMERY_LENGTH);
    Cflint_ExtendGCD(R, N, RP, NP, Result, &RP_Flag, &NP_Flag,
                     Temp1, Temp2, Temp3, Temp4, Temp5,
                     TEST_MENTGOMERY_LENGTH);
    /* 5.计算RR%N = ((R%N)*(R%N))%N = ((R-N)*(R-N))%N */
    Cflint_Subtraction(Temp1, R, N, TEST_MENTGOMERY_LENGTH * 2);
    Cflint_Square(Temp2, Temp1, TEST_MENTGOMERY_LENGTH);
    Cflint_Modulo(RR_N, Temp2, N, TEST_MENTGOMERY_LENGTH * 2);
    /* 验算预先量 */
    /*************************************************************************/
    Cflint_SetValue(Temp1, (TEST_MENTGOMERY_LENGTH + 1) * 2, 0);
    Cflint_Copy(Temp1, R, TEST_MENTGOMERY_LENGTH);
    Cflint_Multiply(Temp2, Temp1, RP, (TEST_MENTGOMERY_LENGTH + 1) * 2);
    Cflint_Copy(Temp1, N, TEST_MENTGOMERY_LENGTH);
    Cflint_Multiply(Temp3, Temp1, NP, (TEST_MENTGOMERY_LENGTH + 1) * 2);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_M_Pretractment:::Check:");
    printf("\n---------------------------------------------------------------");
    printf("\nR:");
    for (Index = 0; Index < TEST_MENTGOMERY_LENGTH; Index++)
        printf("%u ", R[Index]);
    printf("\nN:");
    for (Index = 0; Index < TEST_MENTGOMERY_LENGTH; Index++)
        printf("%u ", N[Index]);
    printf("\nResult:");
    for (Index = 0; Index < TEST_MENTGOMERY_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\nRP_Flag:%d, RP:", RP_Flag);
    for (Index = 0; Index < (TEST_MENTGOMERY_LENGTH + 1) * 2; Index++)
        printf("%u ", RP[Index]);
    printf("\nNP_Flag:%d, NP:", NP_Flag);
    for (Index = 0; Index < (TEST_MENTGOMERY_LENGTH + 1) * 2; Index++)
        printf("%u ", NP[Index]);
    printf("\nR*RP:");
    for (Index = 0; Index < (TEST_MENTGOMERY_LENGTH + 1) * 4; Index++)
        printf("%u ", Temp2[Index]);
    printf("\nN*NP:");
    for (Index = 0; Index < (TEST_MENTGOMERY_LENGTH + 1) * 4; Index++)
        printf("%u ", Temp3[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 蒙哥马利约减X%N = REDC((X*R)%N) */
    /*************************************************************************/
    Cflint_Multiply(Temp1, X, R, TEST_MENTGOMERY_LENGTH);
    Cflint_SetValue(Temp2, TEST_MENTGOMERY_LENGTH * 2, 0);
    Cflint_Modulo(Temp1, Temp1, N, TEST_MENTGOMERY_LENGTH * 2);
    Cflint_M_ModuloReduction(Result, TEST_MENTGOMERY_LENGTH, R_Index,
                             Temp1, N, NP, Temp2, Temp3);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_M_ModuloReduction:::");
    printf("\n---------------------------------------------------------------");
    printf("\nX:");
    for (Index = 0; Index < TEST_MENTGOMERY_LENGTH; Index++)
        printf("%u ", X[Index]);
    printf("\nN:");
    for (Index = 0; Index < TEST_MENTGOMERY_LENGTH; Index++)
        printf("%u ", N[Index]);
    printf("\nResult:");
    for (Index = 0; Index < TEST_MENTGOMERY_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 蒙哥马利模乘 */
    /*************************************************************************/
    Cflint_M_ModuloMultiply(Result, X, Y, N, NP, RR_N,
                            Temp1, Temp2, Temp3, Temp4,
                            R_Index, TEST_MENTGOMERY_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_M_ModuloMultiply:::");
    printf("\n---------------------------------------------------------------");
    printf("\nX:");
    for (Index = 0; Index < TEST_MENTGOMERY_LENGTH; Index++)
        printf("%u ", X[Index]);
    printf("\nY:");
    for (Index = 0; Index < TEST_MENTGOMERY_LENGTH; Index++)
        printf("%u ", Y[Index]);
    printf("\nN:");
    for (Index = 0; Index < TEST_MENTGOMERY_LENGTH; Index++)
        printf("%u ", N[Index]);
    printf("\nResult:");
    for (Index = 0; Index < TEST_MENTGOMERY_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
#endif
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int main(int argc, char *argv[]) {
    /* 这里强转函数指针类型 */
    Cflint_PortInfoCheck((Cflint_PortInfoPrint)printf);

    // Test_CflintFunctionSet1();
    // Test_CflintFunctionSet2();
    // Test_CflintFunctionSet3();
    // Test_CflintFunctionSet4();
    // Test_CflintFunctionSet5();
    // Test_CflintFunctionSet6();
    // Test_CflintFunctionSet7();
    // Test_CflintFunctionSet8();
    //未完成,测试不通过
    //Test_Mentgomery();
    


    return 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
