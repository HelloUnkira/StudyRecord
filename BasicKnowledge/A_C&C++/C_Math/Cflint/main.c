/* 例: */

#include <stdio.h>
#include <windows.h>
#include "Cflint.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 基础平台字节对齐进制运算 */
void Test_Base(void)
{
#define TEST_BASE_LENGTH    10
    uint32_t Index = 0;
    CFLINT_TYPE Result[TEST_BASE_LENGTH * 2]   = {0};
    CFLINT_TYPE Operand1[TEST_BASE_LENGTH * 2] = {0};
    CFLINT_TYPE Operand2[TEST_BASE_LENGTH * 2] = {0};
    CFLINT_TYPE Overflow = 0;
    
    /* 和运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++) {
        Operand1[Index] = Index;
        Operand2[Index] = Index;
    }
    Overflow = Cflint_Addition(Result, Operand1, Operand2, TEST_BASE_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Addition:::Overflow:%d, Result:\n", Overflow);
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 差运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++) {
        Operand1[Index] = Index * 2;
        Operand2[Index] = Index;
    }
    Overflow = Cflint_Subtraction(Result, Operand1, Operand2, TEST_BASE_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Subtraction:::Overflow:%d, Result:\n", Overflow);
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/

    /* 乘运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++) {
        Operand1[Index] = Index;
        Operand2[Index] = Index;
    }
    Cflint_Multiply(Result, Operand1, Operand2, TEST_BASE_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Multiply:::Result:\n");
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/

    /* 方运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++) {
        Operand1[Index] = Index;
        Operand2[Index] = Index;
    }
    Cflint_Square(Result, Operand1, TEST_BASE_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Square:::Result:\n");
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 左移位运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        Result[Index] = ~0;
    Cflint_ShiftLeft2(Result, TEST_BASE_LENGTH * 2, 1);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_ShiftLeft2:::Result:\n");
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        printf("%llx ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 右移位运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        Result[Index] = ~0;
    Cflint_ShiftRight2(Result, TEST_BASE_LENGTH * 2, 1);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_ShiftRight2:::Result:\n");
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        printf("%llx ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
    /* 除运算 */
    /*************************************************************************/
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++) {
        if (Index < TEST_BASE_LENGTH) {
            Operand1[Index] = Index;
            Operand2[Index] = TEST_BASE_LENGTH - Index;
        } else {
            Operand1[Index]  = 0;
            Operand2[Index]  = 0;
        }
    }
    CFLINT_TYPE Quotient[TEST_BASE_LENGTH * 2] = {0};
    CFLINT_TYPE Module[TEST_BASE_LENGTH * 2]   = {0};
    /* 先用乘法运算计算一下值 */
    Cflint_Multiply(Result, Operand1, Operand2, TEST_BASE_LENGTH);
    /* 再计算除数结果 */
    Cflint_Devide(Quotient, Module, TEST_BASE_LENGTH * 2, Result, Operand1);
    printf("\n-------------------------------------------------------------\n");
    printf("Cflint_Devide:::");
    printf("\n-------------------------------------------------------------\n");
    printf("Result:\n");
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        printf("%llx ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    printf("Operand1:\n");
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++)
        printf("%llx ", Operand1[Index]);
    printf("\n-------------------------------------------------------------\n");
    printf("Operand2:\n");
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++)
        printf("%llx ", Operand2[Index]);
    printf("\n-------------------------------------------------------------\n");
    printf("Quotient:\n");
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        printf("%llx ", Quotient[Index]);
    printf("\n-------------------------------------------------------------\n");
    printf("Module:\n");
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        printf("%llx ", Module[Index]);
    printf("\n-------------------------------------------------------------\n");
    /*************************************************************************/
    
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* AES128加解密测试 */
void Test_AES128(void)
{
#define TEST_AES128TEXTLEN  64
    uint8_t Text[TEST_AES128TEXTLEN] = "hello world and my friend";
    uint8_t Key[16]  = {
        0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
        0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x00
    };
    
    printf("\n-------------------------------------------------------------\n");
    printf("AES128:Source Code:");
    for (int32_t i = 0; i < TEST_AES128TEXTLEN; i++) printf("%c", Text[i]);
    printf("\n-------------------------------------------------------------\n");
    AES128_Run((uint8_t *)Text, TEST_AES128TEXTLEN, Key, 0);
    printf("\n-------------------------------------------------------------\n");
    printf("AES128:Encrypt Code:");
    for (int32_t i = 0; i < TEST_AES128TEXTLEN; i++) printf("%c", Text[i]);
    printf("\n-------------------------------------------------------------\n");
    AES128_Run((uint8_t *)Text, TEST_AES128TEXTLEN, Key, 1);
    printf("\n-------------------------------------------------------------\n");
    printf("AES128:Decrypt Code:");
    for (int32_t i = 0; i < TEST_AES128TEXTLEN; i++) printf("%c", Text[i]);
    printf("\n-------------------------------------------------------------\n");
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void PortInfoCheck(const char *Parameter, uint32_t Result)
{
    printf(Parameter, Result);
}

int main(int argc, char *argv[]) {

    Cflint_PortInfoCheck(PortInfoCheck);

    Test_Base();
    Test_AES128();

    return 0;
}
