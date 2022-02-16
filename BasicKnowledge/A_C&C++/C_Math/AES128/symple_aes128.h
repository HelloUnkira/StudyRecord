#ifndef SIMPLE_AES_128_H
#define SIMPLE_AES_128_H

//sizeof(BYTE1) == 1
//sizeof(BYTE4) == 4
#define SIGNED_BYTE1     signed char
#define SIGNED_BYTE4     signed int
#define UNSIGNED_BYTE1 unsigned char
#define UNSIGNED_BYTE4 unsigned int

//AES-128对称加密:
//秘钥长度(bit) :  128
//每组长度(BYTE4): 004
//加密轮数:        010

//text:明文或密文
//textlength:明文或密文长度且模16 == 0
//key:秘钥(对称加密:加密秘钥==解密秘钥)
//flag:0加密,1解密
void simple_aes128(UNSIGNED_BYTE1 *text,
                   UNSIGNED_BYTE4  textlength,
                   UNSIGNED_BYTE1  key[16],
                   UNSIGNED_BYTE1  flag);

#ifdef SIMPLE_AES_128_H_TEST

static void simple_aes128_test(void)
{
    
    UNSIGNED_BYTE1 key[16] = {
        0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
        0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x00
    };
    
    UNSIGNED_BYTE1 text[64] = "hello world and my friend";

    for (int i = 0; i < 32; i++)
        printf("%c ", text[i]);
        printf("\n");

    simple_aes128((UNSIGNED_BYTE1 *)text, 32, key, 0);
    
    for (int i = 0; i < 32; i++)
        printf("%c ", text[i]);
        printf("\n");
        
    simple_aes128((UNSIGNED_BYTE1 *)text, 32, key, 1);

    for (int i = 0; i < 32; i++)
        printf("%c ", text[i]);
        printf("\n");

}

#endif

#endif

