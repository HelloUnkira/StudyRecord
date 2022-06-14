#include <stdio.h>
#include <string.h>
#include "AES128.h"
#include "RC4.h"
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
    for (int32_t Index = 0; Index < TEST_AES128TEXTLEN; Index++)
        printf("%c", Text[Index]);
    printf("\n-------------------------------------------------------------\n");
    AES128_Run(Text, TEST_AES128TEXTLEN, Key, 0);
    printf("\n-------------------------------------------------------------\n");
    printf("AES128:Encrypt Code:");
    for (int32_t Index = 0; Index < TEST_AES128TEXTLEN; Index++)
        printf("%c", Text[Index]);
    printf("\n-------------------------------------------------------------\n");
    AES128_Run(Text, TEST_AES128TEXTLEN, Key, 1);
    printf("\n-------------------------------------------------------------\n");
    printf("AES128:Decrypt Code:");
    for (int32_t Index = 0; Index < TEST_AES128TEXTLEN; Index++)
        printf("%c", Text[Index]);
    printf("\n-------------------------------------------------------------\n");
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* RC4加解密测试 */
void Test_RC4(void)
{
#define TEST_RC4TEXTLEN  64
    uint8_t Text[TEST_RC4TEXTLEN] = "hello world and my friend";
    uint8_t Key[256] = {0};
    uint8_t RC4Key[256] = {0};
    for (int32_t Index = 0; Index < 256; Index++)
        Key[Index] = Index;
    /* 初始化秘钥状态矩阵 */
    RC4Set(RC4Key, Key, 256);
    printf("\n-------------------------------------------------------------\n");
    printf("RC4:Source Code:");
    for (int32_t Index = 0; Index < TEST_RC4TEXTLEN; Index++)
        printf("%c", Text[Index]);
    printf("\n-------------------------------------------------------------\n");
    RC4Run(RC4Key, Text, TEST_RC4TEXTLEN);
    printf("\n-------------------------------------------------------------\n");
    printf("RC4:Encrypt Code:");
    for (int32_t Index = 0; Index < TEST_RC4TEXTLEN; Index++)
        printf("%c", Text[Index]);
    printf("\n-------------------------------------------------------------\n");
    RC4Run(RC4Key, Text, TEST_RC4TEXTLEN);
    printf("\n-------------------------------------------------------------\n");
    printf("RC4:Decrypt Code:");
    for (int32_t Index = 0; Index < TEST_RC4TEXTLEN; Index++)
        printf("%c", Text[Index]);
    printf("\n-------------------------------------------------------------\n");
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int main(int argc, char *argv[]) {

    Test_AES128();
    Test_RC4();

    return 0;
}






