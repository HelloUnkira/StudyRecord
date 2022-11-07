#include <stdio.h>
#include <string.h>
#include "AES128.h"
#include "RC4.h"
#include "MD5.h"
#include "SHA256.h"
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
    for (int32_t Index = 0; Index < TEST_AES128TEXTLEN; Index)
        printf("%c", Text[Index++]);
    printf("\n-------------------------------------------------------------\n");
    printf("AES128:Encrypt Code:");
    AES128_Run(Text, TEST_AES128TEXTLEN, Key, 0);
    for (int32_t Index = 0; Index < TEST_AES128TEXTLEN; Index++)
        printf("%2x ", Text[Index]);
    printf("\n-------------------------------------------------------------\n");
    printf("AES128:Decrypt Code:");
    AES128_Run(Text, TEST_AES128TEXTLEN, Key, 1);
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
    uint8_t RC4Key1[256] = {0};
    uint8_t RC4Key2[256] = {0};
    /* 初始化秘钥状态矩阵 */
    for (int32_t Index = 0; Index < 256; Index++)
        Key[Index] = Index;
    RC4Set(RC4Key, Key, 256);
    for (uint32_t Index = 0; Index < 256; Index++)
        RC4Key1[Index] = RC4Key[Index];
    for (uint32_t Index = 0; Index < 256; Index++)
        RC4Key2[Index] = RC4Key[Index];
    printf("\n-------------------------------------------------------------\n");
    printf("RC4:Source Code:");
    for (int32_t Index = 0; Index < TEST_RC4TEXTLEN; Index++)
        printf("%c", Text[Index]);
    printf("\n-------------------------------------------------------------\n");
    printf("RC4:Encrypt Code:");
    RC4Run(RC4Key1, Text, TEST_RC4TEXTLEN); /* 加密解密使用相同的密钥 */
    for (int32_t Index = 0; Index < TEST_RC4TEXTLEN; Index++)
        printf("%2x ", Text[Index]);
    printf("\n-------------------------------------------------------------\n");
    printf("RC4:Decrypt Code:");
    RC4Run(RC4Key2, Text, TEST_RC4TEXTLEN); /* 加密解密使用相同的密钥 */
    for (int32_t Index = 0; Index < TEST_RC4TEXTLEN; Index++)
        printf("%c", Text[Index]);
    printf("\n-------------------------------------------------------------\n");
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* MD5信息摘要测试 */
void Test_MD5(void)
{
    #define TEST_MD5TEXTLEN     6
    uint8_t Text[TEST_MD5TEXTLEN] = "123456";
    uint8_t Digest[16] = {0};
    printf("\n-------------------------------------------------------------\n");
    printf("MD5:Source Code:");
    for (int32_t Index = 0; Index < TEST_MD5TEXTLEN; Index++)
        printf("%c", Text[Index]);
    printf("\n-------------------------------------------------------------\n");
    printf("MD5:Digest:");
    MD5_Run(Text, TEST_MD5TEXTLEN, Digest);
    for (int32_t Index = 0; Index < 16; Index++)
        printf("%02x", Digest[Index]);
    printf("\n-------------------------------------------------------------\n");
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* MD5信息摘要测试 */
void Test_SHA256(void)
{
    #define TEST_SHA256TEXTLEN      5
    uint8_t Text[TEST_SHA256TEXTLEN] = "hello";
    uint8_t Digest[32] = {0};
    printf("\n-------------------------------------------------------------\n");
    printf("SHA256:Source Code:");
    for (int32_t Index = 0; Index < TEST_SHA256TEXTLEN; Index++)
        printf("%c", Text[Index]);
    printf("\n-------------------------------------------------------------\n");
    printf("SHA256:Digest:");
    SHA256_Run(Text, TEST_SHA256TEXTLEN, Digest);
    for (int32_t Index = 0; Index < 32; Index++)
        printf("%02x", Digest[Index]);
    printf("\n-------------------------------------------------------------\n");
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int main(int argc, char *argv[]) {

    Test_AES128();
    Test_RC4();
    Test_MD5();
    Test_SHA256();
    
    //03AC674216F3E15C761EE1A5E255F067953623C8B388B4459E13F978D7C846F4
    
    return 0;
}






