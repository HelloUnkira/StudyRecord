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
    CFLINT_BYTES Result[TEST_BASE_LENGTH * 2] = {0};
    CFLINT_BYTES Operator1[TEST_BASE_LENGTH] = {0};
    CFLINT_BYTES Operator2[TEST_BASE_LENGTH] = {0};
    CFLINT_BYTES Overflow = 0;
    
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++)
        Operator1[Index] = Index;
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++)
        Operator2[Index] = Index;
    
    /* 和运算 */
    Cflint_Addition(Result, &Overflow, Operator1, Operator2, TEST_BASE_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Overflow:%d, Result:\n", Overflow);
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++)
        Operator1[Index] = Index * 2;
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++)
        Operator2[Index] = Index;
    
    /* 差运算 */
    Cflint_Subtraction(Result, &Overflow, Operator1, Operator2, TEST_BASE_LENGTH);
    printf("\n-------------------------------------------------------------\n");
    printf("Overflow:%d, Result:\n", Overflow);
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++)
        Operator1[Index] = 1;
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++)
        Operator2[Index] = 1;
    
    /* 乘运算 */
    Cflint_Multiply(Result, TEST_BASE_LENGTH, Operator1, Operator2);
    printf("\n-------------------------------------------------------------\n");
    printf("Overflow:%d, Result:\n", Overflow);
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++)
        Operator1[Index] = 1;
    for (Index = 0; Index < TEST_BASE_LENGTH; Index++)
        Operator2[Index] = 1;
    
    /* 方运算 */
    Cflint_Square(Result, TEST_BASE_LENGTH, Operator1);
    printf("\n-------------------------------------------------------------\n");
    printf("Overflow:%d, Result:\n", Overflow);
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        printf("%u ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        Result[Index] = ~0;
    
    /* 左移位运算 */
    Cflint_ShiftLeft(Result, TEST_BASE_LENGTH * 2, 33);
    printf("\n-------------------------------------------------------------\n");
    printf("Result:\n");
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        printf("%llx ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        Result[Index] = ~0;
    
    /* 右移位运算 */
    Cflint_ShiftRight(Result, TEST_BASE_LENGTH * 2, 33);
    printf("\n-------------------------------------------------------------\n");
    printf("Result:\n");
    for (Index = 0; Index < TEST_BASE_LENGTH * 2; Index++)
        printf("%llx ", Result[Index]);
    printf("\n-------------------------------------------------------------\n");
    
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* AES128加解密测试 */
void Test_AES128(void)
{
    
    uint8_t Key[16] = {
        0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
        0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x00
    };
    
    uint8_t Text[64] = "hello world and my friend";

    for (int i = 0; i < 32; i++)
        printf("%c ", Text[i]);
        printf("\n");

    AES128_Run((uint8_t *)Text, 32, Key, 0);
    
    for (int i = 0; i < 32; i++)
        printf("%c ", Text[i]);
        printf("\n");
        
    AES128_Run((uint8_t *)Text, 32, Key, 1);

    for (int i = 0; i < 32; i++)
        printf("%c ", Text[i]);
        printf("\n");

}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/



int main(int argc, uint8_t *argv[]) {

    printf("Port Endian:%d\n", Cflint_CheckEndian());

    Test_Base();
    //Test_AES128();

    return 0;
}
