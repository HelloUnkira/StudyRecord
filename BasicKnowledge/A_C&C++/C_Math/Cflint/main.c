/* 例: */

#include <stdio.h>
#include <windows.h>
#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 测试左右移位 */
void Test_CflintBase1(void)
{
    uint32_t Index = 0;
    #define TEST_CFLINTBASE1_LENGTH    10
    CFLINT_TYPE Result[TEST_CFLINTBASE1_LENGTH] = {0};
    
    /* 左移位运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_CFLINTBASE1_LENGTH; Index++)
        Result[Index] = ~0;
    Cflint_ShiftLeft2(Result, TEST_CFLINTBASE1_LENGTH, 1);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_ShiftLeft2:::Result:\n");
    for (Index = 0; Index < TEST_CFLINTBASE1_LENGTH; Index++)
        printf("%llx ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 右移位运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_CFLINTBASE1_LENGTH; Index++)
        Result[Index] = ~0;
    Cflint_ShiftRight2(Result, TEST_CFLINTBASE1_LENGTH, 1);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_ShiftRight2:::Result:\n");
    for (Index = 0; Index < TEST_CFLINTBASE1_LENGTH; Index++)
        printf("%llx ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 测试加减乘方 */
void Test_CflintBase2(void)
{
    uint32_t Index = 0;
    #define TEST_CFLINTBASE2_LENGTH    10
    CFLINT_TYPE Result  [TEST_CFLINTBASE2_LENGTH * 2] = {0};
    CFLINT_TYPE Operand1[TEST_CFLINTBASE2_LENGTH] = {0};
    CFLINT_TYPE Operand2[TEST_CFLINTBASE2_LENGTH] = {0};
    CFLINT_TYPE Overflow = 0;
    
    /* 和运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_CFLINTBASE2_LENGTH; Index++) {
        Operand1[Index] = (~0) - Index;
        Operand2[Index] = (~0) - Index;
    }
    Overflow =
    Cflint_Addition(Result, Operand1, Operand2, TEST_CFLINTBASE2_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Addition:::Overflow:%d, Result:\n", Overflow);
    for (Index = 0; Index < TEST_CFLINTBASE2_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 差运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_CFLINTBASE2_LENGTH; Index++) {
        Operand1[Index] = TEST_CFLINTBASE2_LENGTH - Index;
        Operand2[Index] = Index;
    }
    Overflow =
    Cflint_Subtraction(Result, Operand1, Operand2, TEST_CFLINTBASE2_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Subtraction:::Overflow:%d, Result:\n", Overflow);
    for (Index = 0; Index < TEST_CFLINTBASE2_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/

    /* 乘运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_CFLINTBASE2_LENGTH; Index++) {
        Operand1[Index] = Index;
        Operand2[Index] = Index;
    }
    Cflint_Multiply(Result, Operand1, Operand2, TEST_CFLINTBASE2_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Multiply:::Result:\n");
    for (Index = 0; Index < TEST_CFLINTBASE2_LENGTH * 2; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/

    /* 方运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_CFLINTBASE2_LENGTH; Index++) {
        Operand1[Index] = Index;
        Operand2[Index] = Index;
    }
    Cflint_Square(Result, Operand1, TEST_CFLINTBASE2_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Square:::Result:\n");
    for (Index = 0; Index < TEST_CFLINTBASE2_LENGTH * 2; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/

}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 测试除模 */
void Test_CflintBase3(void)
{
    uint32_t Index = 0;
    #define TEST_CFLINTBASE3_LENGTH    10
    CFLINT_TYPE Result  [TEST_CFLINTBASE3_LENGTH * 2] = {0};
    CFLINT_TYPE Quotient[TEST_CFLINTBASE3_LENGTH * 2] = {0};
    CFLINT_TYPE Module  [TEST_CFLINTBASE3_LENGTH * 2] = {0};
    CFLINT_TYPE Operand1[TEST_CFLINTBASE3_LENGTH * 2] = {0};
    CFLINT_TYPE Operand2[TEST_CFLINTBASE3_LENGTH * 2] = {0};
    CFLINT_TYPE Overflow = 0;
    
    /* 除运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_CFLINTBASE3_LENGTH * 2; Index++) {
        if (Index < TEST_CFLINTBASE3_LENGTH) {
            Operand1[Index] = Index;
            Operand2[Index] = TEST_CFLINTBASE3_LENGTH - Index;
            Module[Index]   = TEST_CFLINTBASE3_LENGTH - Index - 1;
        } else {
            Operand1[Index] = 0;
            Operand2[Index] = 0;
            Module  [Index] = 0;
        }
    }
    /* 先用乘法运算计算一下值 */
    Cflint_Multiply(Result, Operand1, Operand2, TEST_CFLINTBASE3_LENGTH);
    /* 为其添加一个余数(这里不让其溢出) */
    Cflint_Addition(Result, Result, Module, TEST_CFLINTBASE3_LENGTH * 2);
    /* 再计算除数结果 */
    Cflint_Devide(Quotient, Module, Result, Operand1,
                  TEST_CFLINTBASE3_LENGTH * 2);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Devide:::");
    printf("\n---------------------------------------------------------------");
    printf("\nResult:");
    for (Index = 0; Index < TEST_CFLINTBASE3_LENGTH * 2; Index++)
        printf("%llx ", Result[Index]);
    printf("\nOperand1:");
    for (Index = 0; Index < TEST_CFLINTBASE3_LENGTH; Index++)
        printf("%llx ", Operand1[Index]);
    printf("\nOperand2:");
    for (Index = 0; Index < TEST_CFLINTBASE3_LENGTH; Index++)
        printf("%llx ", Operand2[Index]);
    printf("\nQuotient:");
    for (Index = 0; Index < TEST_CFLINTBASE3_LENGTH * 2; Index++)
        printf("%llx ", Quotient[Index]);
    printf("\nModule:");
    for (Index = 0; Index < TEST_CFLINTBASE3_LENGTH * 2; Index++)
        printf("%llx ", Module[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 模运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_CFLINTBASE3_LENGTH * 2; Index++) {
        if (Index < TEST_CFLINTBASE3_LENGTH) {
            Operand1[Index] = Index;
            Operand2[Index] = TEST_CFLINTBASE3_LENGTH - Index;
            Module  [Index] = TEST_CFLINTBASE3_LENGTH - Index - 1;
        } else {
            Operand1[Index] = 0;
            Operand2[Index] = 0;
            Module  [Index] = 0;
        }
    }
    /* 先用乘法运算计算一下值 */
    Cflint_Multiply(Result, Operand1, Operand2, TEST_CFLINTBASE3_LENGTH);
    /* 为其添加一个余数(这里不让其溢出) */
    Cflint_Addition(Result, Result, Module, TEST_CFLINTBASE3_LENGTH * 2);
    /* 再计算模结果 */
    Cflint_Modulo(Module, Result, Operand1, TEST_CFLINTBASE3_LENGTH * 2);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Modulo:::");
    printf("\n---------------------------------------------------------------");
    printf("\nResult:");
    for (Index = 0; Index < TEST_CFLINTBASE3_LENGTH * 2; Index++)
        printf("%llx ", Result[Index]);
    printf("\nOperand1:");
    for (Index = 0; Index < TEST_CFLINTBASE3_LENGTH; Index++)
        printf("%llx ", Operand1[Index]);
    printf("\nOperand2:");
    for (Index = 0; Index < TEST_CFLINTBASE3_LENGTH; Index++)
        printf("%llx ", Operand2[Index]);
    printf("\nModule:");
    for (Index = 0; Index < TEST_CFLINTBASE3_LENGTH * 2; Index++)
        printf("%llx ", Module[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 模运算2 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_CFLINTBASE3_LENGTH * 2; Index++)
        Result[Index] = ~0;
    Cflint_Modulo2(Result, TEST_CFLINTBASE3_LENGTH * 2, CFLINT_BITS * 4 + 3);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Modulo2:::Result:\n");
    for (Index = 0; Index < TEST_CFLINTBASE3_LENGTH * 2; Index++)
        printf("%llx ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 测试模幂模逆 */
void Test_CflintBase4(void)
{
    uint32_t Index = 0;
    #define TEST_CFLINTBASE4_LENGTH    3
    CFLINT_TYPE Result  [TEST_CFLINTBASE4_LENGTH] = {0};
    CFLINT_TYPE Operand [TEST_CFLINTBASE4_LENGTH] = {0};
    CFLINT_TYPE Exponent[TEST_CFLINTBASE4_LENGTH] = {0};
    CFLINT_TYPE Module  [TEST_CFLINTBASE4_LENGTH * 2] = {0};
    CFLINT_TYPE Temp1   [TEST_CFLINTBASE4_LENGTH * 2] = {0};
    CFLINT_TYPE Temp2   [TEST_CFLINTBASE4_LENGTH * 2] = {0};
    CFLINT_TYPE Temp3   [TEST_CFLINTBASE4_LENGTH * 2] = {0};
    CFLINT_TYPE Temp4   [TEST_CFLINTBASE4_LENGTH * 2] = {0};
    
    /* 模幂运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_CFLINTBASE4_LENGTH; Index++) {
        Operand[Index] = TEST_CFLINTBASE4_LENGTH - Index;
        Module[Index]  = TEST_CFLINTBASE4_LENGTH - Index;
    }
    Module[0]   = 0; 
    Exponent[0] = 5;
    /* 模幂运算 */
    Cflint_ModuloExponent(Result, Module, Operand, Exponent,
                          Temp1, Temp2, Temp3, Temp4,
                          TEST_CFLINTBASE4_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_ModuloExponent:::");
    printf("\n---------------------------------------------------------------");
    printf("\nOperand:");
    for (Index = 0; Index < TEST_CFLINTBASE4_LENGTH; Index++)
        printf("%u ", Operand[Index]);
    printf("\nModule:");
    for (Index = 0; Index < TEST_CFLINTBASE4_LENGTH; Index++)
        printf("%u ", Module[Index]);
    printf("\nResult:");
    for (Index = 0; Index < TEST_CFLINTBASE4_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/

    /* 模逆运算 */
    /*************************************************************************/
    Cflint_SetValue(Module, TEST_CFLINTBASE4_LENGTH * 2, 0);
    for (Index = 0; Index < TEST_CFLINTBASE4_LENGTH; Index++) {
        Operand[Index] = TEST_CFLINTBASE4_LENGTH - Index;
        Module[Index]  = TEST_CFLINTBASE4_LENGTH - Index;
    }
    Module[0] = 1;
    /* 模逆运算 */
    Cflint_ModuloInverse(Result, Module, Operand,
                         Temp1, Temp2, Temp3, Temp4,
                         TEST_CFLINTBASE4_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_ModuloInverse:::");
    printf("\n---------------------------------------------------------------");
    printf("\nOperand:");
    for (Index = 0; Index < TEST_CFLINTBASE4_LENGTH; Index++)
        printf("%u ", Operand[Index]);
    printf("\nModule:");
    for (Index = 0; Index < TEST_CFLINTBASE4_LENGTH * 2; Index++)
        printf("%u ", Module[Index]);
    printf("\nResult:");
    for (Index = 0; Index < TEST_CFLINTBASE4_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /* 注:Module和Operand如果不互素,模逆不存在 */
    printf("(Result * Operand) % Module:::Result:\n");
    Cflint_Multiply(Temp1, Result, Operand, TEST_CFLINTBASE4_LENGTH);
    Cflint_Modulo(Temp1, Temp1, Module, TEST_CFLINTBASE4_LENGTH * 2);
    Cflint_Copy(Result, Temp1, TEST_CFLINTBASE4_LENGTH);
    for (Index = 0; Index < TEST_CFLINTBASE4_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 测试扩展欧几里得 */
void Test_GCD(void)
{
    uint32_t Index = 0;
    #define TEST_GCD_LENGTH     5
    CFLINT_TYPE Result[TEST_GCD_LENGTH] = {0};
    CFLINT_TYPE A[TEST_GCD_LENGTH] = {0};
    CFLINT_TYPE B[TEST_GCD_LENGTH] = {0};
    CFLINT_TYPE X[(TEST_GCD_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Y[(TEST_GCD_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Temp1[(TEST_GCD_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Temp2[(TEST_GCD_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Temp3[(TEST_GCD_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Temp4[(TEST_GCD_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Temp5[(TEST_GCD_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Temp6[(TEST_GCD_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Temp7[(TEST_GCD_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Temp8[(TEST_GCD_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Temp_0[(TEST_GCD_LENGTH + 1) * 4] = {0};
    CFLINT_TYPE Temp_1[(TEST_GCD_LENGTH + 1) * 4] = {0};
    CFLINT_TYPE X_Flag = 0, Y_Flag = 0;
    bool Check = 0;

    /* 欧几里得运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_GCD_LENGTH; Index++) {
        A[Index] = TEST_GCD_LENGTH - Index;
        B[Index] = TEST_GCD_LENGTH - Index;
    }
    B[0] = 1;
    /*************************************************************************/
    /* 欧几里得运算 */
    Check = Cflint_GCDCheck(A, B, TEST_GCD_LENGTH, Temp1, Temp2, Temp3);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_GCDCheck:::");
    printf("\n---------------------------------------------------------------");
    printf("\nA:");
    for (Index = 0; Index < TEST_GCD_LENGTH; Index++)
        printf("%u ", A[Index]);
    printf("\nB:");
    for (Index = 0; Index < TEST_GCD_LENGTH; Index++)
        printf("%u ", B[Index]);
    printf("\nResult:%u", Check);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 扩展欧几里得运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_GCD_LENGTH; Index++) {
        A[Index] = TEST_GCD_LENGTH - Index;
        B[Index] = TEST_GCD_LENGTH - Index;
    }
    B[0] = 1;
    /*************************************************************************/
    /* 扩展欧几里得运算 */
    Cflint_ExtendGCD(A, B, X, Y, Result, &X_Flag, &Y_Flag,
                     Temp1, Temp2, Temp3, Temp4, Temp5, TEST_GCD_LENGTH);
    /*************************************************************************/
    Cflint_SetValue(Temp1, (TEST_GCD_LENGTH + 1) * 2, 0);
    Cflint_Copy(Temp1, A, TEST_GCD_LENGTH);
    Cflint_Multiply(Temp_0, Temp1, X, (TEST_GCD_LENGTH + 1) * 2);
    Cflint_Copy(Temp1, B, TEST_GCD_LENGTH);
    Cflint_Multiply(Temp_1, Temp1, Y, (TEST_GCD_LENGTH + 1) * 2);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_ExtendGCD:::");
    printf("\n---------------------------------------------------------------");
    printf("\nA:");
    for (Index = 0; Index < TEST_GCD_LENGTH; Index++)
        printf("%u ", A[Index]);
    printf("\nB:");
    for (Index = 0; Index < TEST_GCD_LENGTH; Index++)
        printf("%u ", B[Index]);
    printf("\nResult:");
    for (Index = 0; Index < TEST_GCD_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\nX_Flag:%d, X:", X_Flag);
    for (Index = 0; Index < (TEST_GCD_LENGTH + 1) * 2; Index++)
        printf("%u ", X[Index]);
    printf("\nY_Flag:%d, Y:", Y_Flag);
    for (Index = 0; Index < (TEST_GCD_LENGTH + 1) * 2; Index++)
        printf("%u ", Y[Index]);
    printf("\nA*X:");
    for (Index = 0; Index < (TEST_GCD_LENGTH + 1) * 2; Index++)
        printf("%u ", Temp_0[Index]);
    printf("\nB*Y:");
    for (Index = 0; Index < (TEST_GCD_LENGTH + 1) * 2; Index++)
        printf("%u ", Temp_1[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 扩展欧几里得运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_GCD_LENGTH; Index++) {
        A[Index] = TEST_GCD_LENGTH - Index;
        B[Index] = TEST_GCD_LENGTH - Index;
    }
    B[0] = 1;
    /*************************************************************************/
    /* 扩展欧几里得运算 */
    Cflint_ExGCD(A, B, X, Y, &X_Flag, &Y_Flag,
                 Temp1, Temp2, Temp3, Temp4,
                 Temp5, Temp6, Temp7, Temp8, TEST_GCD_LENGTH);
    /*************************************************************************/
    Cflint_SetValue(Temp1, (TEST_GCD_LENGTH + 1) * 2, 0);
    Cflint_Copy(Temp1, A, TEST_GCD_LENGTH);
    Cflint_Multiply(Temp_0, Temp1, X, (TEST_GCD_LENGTH + 1) * 2);
    Cflint_Copy(Temp1, B, TEST_GCD_LENGTH);
    Cflint_Multiply(Temp_1, Temp1, Y, (TEST_GCD_LENGTH + 1) * 2);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_ExGCD:::");
    printf("\n---------------------------------------------------------------");
    printf("\nA:");
    for (Index = 0; Index < TEST_GCD_LENGTH; Index++)
        printf("%u ", A[Index]);
    printf("\nB:");
    for (Index = 0; Index < TEST_GCD_LENGTH; Index++)
        printf("%u ", B[Index]);
    printf("\nResult:");
    for (Index = 0; Index < TEST_GCD_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\nX_Flag:%d, X:", X_Flag);
    for (Index = 0; Index < (TEST_GCD_LENGTH + 1) * 2; Index++)
        printf("%u ", X[Index]);
    printf("\nY_Flag:%d, Y:", Y_Flag);
    for (Index = 0; Index < (TEST_GCD_LENGTH + 1) * 2; Index++)
        printf("%u ", Y[Index]);
    printf("\nA*X:");
    for (Index = 0; Index < (TEST_GCD_LENGTH + 1) * 2; Index++)
        printf("%u ", Temp_0[Index]);
    printf("\nB*Y:");
    for (Index = 0; Index < (TEST_GCD_LENGTH + 1) * 2; Index++)
        printf("%u ", Temp_1[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 测试蒙哥马利模(未完成,测试未通过) */
void Test_Mentgomery(void)
{
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
    if (Cflint_GCDCheck(R, N, TEST_MENTGOMERY_LENGTH * 2,
                        Temp1, Temp2, Temp3) == false) {
        Cflint_SetValue(Result, TEST_MENTGOMERY_LENGTH, 0);
        return;
    }
    /* 4.计算RR' + NN' = GCD(R,N) == 1 */
    // Cflint_ModuloInverse(RP, R, N, Temp1, Temp2, Temp3, Temp4,
    //                      TEST_MENTGOMERY_LENGTH);
    // Cflint_ModuloInverse(NP, N, R, Temp1, Temp2, Temp3, Temp4,
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
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int main(int argc, char *argv[]) {
    /* 这里强转函数指针类型 */
    Cflint_PortInfoCheck((Cflint_PortInfoPrint)printf);

    //Test_CflintBase1();
    //Test_CflintBase2();
    //Test_CflintBase3();
    //Test_CflintBase4();
    //Test_GCD();
    //未完成,测试不通过
    //Test_Mentgomery();
    
    return 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
