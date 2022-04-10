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
    CFLINT_TYPE A[TEST_GCD_LENGTH] = {0};
    CFLINT_TYPE B[TEST_GCD_LENGTH] = {0};
    CFLINT_TYPE X[(TEST_GCD_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Y[(TEST_GCD_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Result[TEST_GCD_LENGTH] = {0};
    CFLINT_TYPE Temp1[TEST_GCD_LENGTH] = {0};
    CFLINT_TYPE Temp2[TEST_GCD_LENGTH] = {0};
    CFLINT_TYPE Temp3[TEST_GCD_LENGTH] = {0};
    CFLINT_TYPE Temp4[TEST_GCD_LENGTH] = {0};
    CFLINT_TYPE Temp5[(TEST_GCD_LENGTH + 1) * 2] = {0};
    CFLINT_TYPE Temp6[(TEST_GCD_LENGTH + 1) * 4] = {0};
    CFLINT_TYPE Temp7[(TEST_GCD_LENGTH + 1) * 4] = {0};
    CFLINT_TYPE X_Flag = 0, Y_Flag = 0;
    bool Check = 0;

    /* 欧几里得运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_GCD_LENGTH; Index++) {
        A[Index] = TEST_GCD_LENGTH - Index;
        B[Index] = TEST_GCD_LENGTH - Index;
    }
    B[0] = 1;
    /* 欧几里得运算 */
    Check = Cflint_GCD(A, B, TEST_GCD_LENGTH, Temp1, Temp2, Temp3);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_GCD:::");
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
    /* 扩展欧几里得运算 */
    Cflint_ExtendGCD(A, B, X, Y, Result, &X_Flag, &Y_Flag,
                     Temp1, Temp2, Temp3, Temp4, Temp5, TEST_GCD_LENGTH);
    Cflint_SetValue(Temp5, TEST_GCD_LENGTH * 2 + 1, 0);
    Cflint_Copy(Temp5, A, TEST_GCD_LENGTH);
    Cflint_Multiply(Temp6, Temp5, X, (TEST_GCD_LENGTH + 1) * 2);
    Cflint_Copy(Temp5, B, TEST_GCD_LENGTH);
    Cflint_Multiply(Temp7, Temp5, Y, (TEST_GCD_LENGTH + 1) * 2);
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
        printf("%u ", Temp6[Index]);
    printf("\nB*Y:");
    for (Index = 0; Index < (TEST_GCD_LENGTH + 1) * 2; Index++)
        printf("%u ", Temp7[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 测试蒙哥马利模幂 */
void Test_Mentgomery(void)
{
    uint32_t Index = 0;
    #define TEST_MENTGOMERY_LENGTH    5
    CFLINT_TYPE Result  [TEST_MENTGOMERY_LENGTH] = {0};
    CFLINT_TYPE Operand [TEST_MENTGOMERY_LENGTH] = {0};
    CFLINT_TYPE Exponent[TEST_MENTGOMERY_LENGTH] = {0};
    CFLINT_TYPE Module  [TEST_MENTGOMERY_LENGTH * 2] = {0};
    CFLINT_TYPE Temp1   [TEST_MENTGOMERY_LENGTH * 2] = {0};
    CFLINT_TYPE Temp2   [TEST_MENTGOMERY_LENGTH * 2] = {0};
    CFLINT_TYPE Temp3   [TEST_MENTGOMERY_LENGTH * 2] = {0};
    CFLINT_TYPE Temp4   [TEST_MENTGOMERY_LENGTH * 2] = {0};
    CFLINT_TYPE Temp5   [TEST_MENTGOMERY_LENGTH * 2] = {0};
    CFLINT_TYPE Temp6   [TEST_MENTGOMERY_LENGTH * 2] = {0};
    CFLINT_TYPE Temp7   [TEST_MENTGOMERY_LENGTH * 2] = {0};
    CFLINT_TYPE Temp8   [TEST_MENTGOMERY_LENGTH * 2] = {0};
    CFLINT_TYPE Temp9   [TEST_MENTGOMERY_LENGTH * 2] = {0};
    CFLINT_TYPE Temp10  [TEST_MENTGOMERY_LENGTH * 2] = {0};
    /* 模幂运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_MENTGOMERY_LENGTH; Index++) {
        Operand[Index] = TEST_MENTGOMERY_LENGTH - Index;
        Module[Index]  = TEST_MENTGOMERY_LENGTH - Index;
    }
    Module[0]   = 1;
    Exponent[0] = 5;
    /* 模幂运算 */
    Cflint_M_ModuloExponent(Result, Module, Operand, Exponent,
                            Temp1, Temp2, Temp3, Temp4, Temp5,
                            Temp6, Temp7, Temp8, Temp9, Temp10,
                            TEST_MENTGOMERY_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_M_ModuloExponent:::");
    printf("\n---------------------------------------------------------------");
    printf("\nOperand:");
    for (Index = 0; Index < TEST_MENTGOMERY_LENGTH; Index++)
        printf("%u ", Operand[Index]);
    printf("\nModule:");
    for (Index = 0; Index < TEST_MENTGOMERY_LENGTH; Index++)
        printf("%u ", Module[Index]);
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

    Test_CflintBase1();
    Test_CflintBase2();
    Test_CflintBase3();
    Test_CflintBase4();
    Test_GCD();
    printf("\n---------------------------------------------------------------");
    printf("\n---------------------------------------------------------------");
    printf("\n---------------------------------------------------------------");
    printf("\n-------------------------------------------------------------\n");
    Test_Mentgomery();
    
    return 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
