#include "MD5.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 初始状态量(小端序) */
static inline void MD5_Ready(uint32_t Buffer[4])
{
    const uint32_t MD5_Const[4] = {
        0x67452301,0xEFCDAB89,0x98BADCFE,0x10325476,
    };
    for (uint32_t Index = 0; Index < 4; Buffer[Index] = MD5_Const[Index++]);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 需要填充字节数 */
static inline uint32_t MD5_FillBytes(uint32_t Length)
{
    uint32_t FillBytes = Length % 64;
    FillBytes = FillBytes >= 56 ? 64 - FillBytes + 56 : 56 - FillBytes;
    return FillBytes;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 分组切块(小端序块) */
static inline bool MD5_Block(uint8_t *Text,      uint32_t  Length,
                             uint8_t  Block[64], uint32_t *Offset,
                             uint32_t FillBytes)
{
    bool RunToEnd = false;
    /* 处理后的数据流需满足补位到bits==448(mod512),bytes==56(mod64) */
    /* 填充规则为追加1其他补0,也即补一个0x80后补0 */
    /* 最后的bits==64(mod512),bytes==8(mod64) */
    for (uint32_t Index = 0; Index < 64; Index++)
        /* 正常的数据填充时 */
        if (*Offset + Index < Length)
            Block[Index] = Text[*Offset + Index];
        /* 补充的数据填充时 */
        /* 1.第一个要填充的字节,填充0x80 */
        else if (*Offset + Index == Length)
            Block[Index] = 0x80;
        /* 2.中间要填充的字节,填充0x00 */
        else if (*Offset + Index  < Length + FillBytes)
            Block[Index] = 0x00;
        /* 3.最后要填充数据位的长度,填充Length*8 */
        else {
            uint64_t LengthBits = Length * 8;
            for (uint32_t Nonius = 0; Nonius < 8; Nonius++)
                Block[64 - 8 + Nonius] = LengthBits >> (Nonius * 8);
            RunToEnd = true;
        }
    *Offset += 64;
    return RunToEnd;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 加密轮(小端序块) */
static inline void MD5_Wheel(uint32_t Block[16], uint32_t Buffer[4])
{
    /* 常量ti = (uint32_t)(abs(sin(i+1))*(2pow32)) */
    const uint32_t MD5_Ti[] = {
        0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,
        0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
        0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,
        0x6b901122,0xfd987193,0xa679438e,0x49b40821,
        0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,
        0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
        0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,
        0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
        0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,
        0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
        0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,
        0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
        0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,
        0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
        0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,
        0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391,
    };
    /* 移位数 */
    const uint32_t MD5_Si[] = {
         7,12,17,22, 7,12,17,22,
         7,12,17,22, 7,12,17,22,
         5, 9,14,20, 5, 9,14,20,
         5, 9,14,20, 5, 9,14,20,
         4,11,16,23, 4,11,16,23,
         4,11,16,23, 4,11,16,23,
         6,10,15,21, 6,10,15,21,
         6,10,15,21, 6,10,15,21,
    };
    /* 循环移位宏(右移N位,左移32-N位) */
    #define MD5_Shift(X, N) (((X) << (N)) | ((X) >> (32-(N))))
    /* 四个轮函数 */
    #define MD5_FuncF(X, Y, Z)  (((X) & (Y)) | ((~(X)) & (Z)))
    #define MD5_FuncG(X, Y, Z)  (((X) & (Z)) | ((~(Z)) & (Y)))
    #define MD5_FuncH(X, Y, Z)  ((X) ^ (Y) ^ (Z))
    #define MD5_FuncI(X, Y, Z)  ((Y) ^ ((~(Z)) | (X)))
    /* 暂存数 */
    uint32_t A = Buffer[0];
    uint32_t B = Buffer[1];
    uint32_t C = Buffer[2];
    uint32_t D = Buffer[3];
    uint32_t F = 0;
    uint32_t G = 0;
    
    for (uint32_t Index = 0; Index < 64; Index++) {
        
        if (Index < 16) {
            F = MD5_FuncF(B, C, D);
            G = (Index) % 16;
        } else
        if (Index < 32) {
            F = MD5_FuncG(B, C, D);
            G = (Index * 5 + 1) % 16;
        } else
        if (Index < 48) {
            F = MD5_FuncH(B, C, D);
            G = (Index * 3 + 5) % 16;
        } else
        if (Index < 64) {
            F = MD5_FuncI(B, C, D);
            G = (Index * 7) % 16;
        }
        
        uint32_t Temp = D; D = C; C = B;
        B = B + MD5_Shift(A + F + MD5_Ti[Index] + Block[G], MD5_Si[Index]);
        A = Temp;
    }
    
    Buffer[0] += A;
    Buffer[1] += B;
    Buffer[2] += C;
    Buffer[3] += D;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void MD5_Run(uint8_t *Text, uint32_t Length, uint8_t Digest[16])
{
    /* 初始化MD5缓存器,用于保存中间变量和最终结果(字节流小端序) */
    uint32_t *Result = (uint32_t *)Digest;
    MD5_Ready(Result);
    /* 计算最终需要填充的字节数 */
    uint32_t FillBytes = MD5_FillBytes(Length);
    /* 循环摘要流程 */
    bool RunToEnd = false;
    do {
        /* 1.填充分组Block */
        uint32_t Offset = 0;
        uint8_t  Block[64] = {0};
        RunToEnd = MD5_Block(Text, Length, Block, &Offset, FillBytes);
        /* 2.处理数据段轮函数 */
        MD5_Wheel((uint32_t *)Block, Result);
        /*  */
    } while (RunToEnd == false);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
