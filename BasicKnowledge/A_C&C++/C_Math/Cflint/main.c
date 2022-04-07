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
    printf("Cflint_ModuloExp:::");
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
    printf("Cflint_ModuloInv:::");
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
/*  */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int main(int argc, char *argv[]) {
    /* 这里强转函数指针类型 */
    Cflint_PortInfoCheck(printf);

    Test_CflintBase1();
    Test_CflintBase2();
    Test_CflintBase3();
    Test_CflintBase4();
    
    return 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
