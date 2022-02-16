#include "symple_aes128.h"

#define BASIC_DEFINE_AND_HELP_FUNCTION
#ifdef  BASIC_DEFINE_AND_HELP_FUNCTION

static void change_data_0(UNSIGNED_BYTE1 matrix[4][4], UNSIGNED_BYTE1 key[16])
{
    UNSIGNED_BYTE1 i = 0, j = 0;
    //秘钥输入状态矩阵
    for (i = 0; i <= 3; i++)
        for (j = 0; j <= 3; j++)
            matrix[j][i] = key[i * 4 + j];
}

static void change_data_1(UNSIGNED_BYTE1 matrix[4][4], UNSIGNED_BYTE1 key[16])
{
    UNSIGNED_BYTE1 i = 0, j = 0;
    //状态矩阵输入秘钥
    for (i = 0; i <= 3; i++)
        for (j = 0; j <= 3; j++)
            key[i * 4 + j] = matrix[j][i];
}

#define KEY_TO_MATRIX(matrix, key) change_data_0(matrix, key)
#define MATRIX_TO_KEY(matrix, key) change_data_1(matrix, key)

#endif

//1.字节代换
#define STEP1_FOR_BYTE_CHANGE
#ifdef  STEP1_FOR_BYTE_CHANGE

static const UNSIGNED_BYTE1 aes128_s_1[16][16] = {  //aes128_s_1 -->S盒
    {0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5
    ,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76},
    {0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0
    ,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0},
    {0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc
    ,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15},
    {0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a
    ,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75},
    {0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0
    ,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84},
    {0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b
    ,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf},
    {0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85
    ,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8},
    {0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5
    ,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2},
    {0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17
    ,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73},
    {0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88
    ,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb},
    {0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c
    ,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79},
    {0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9
    ,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08},
    {0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6
    ,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a},
    {0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e
    ,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e},
    {0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94
    ,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf},
    {0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68
    ,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16}
};

static const UNSIGNED_BYTE1 aes128_s_2[16][16] = {  //aes128_s_2 -->逆S盒
    {0x52,0x09,0x6a,0xd5,0x30,0x36,0xa5,0x38
    ,0xbf,0x40,0xa3,0x9e,0x81,0xf3,0xd7,0xfb},
    {0x7c,0xe3,0x39,0x82,0x9b,0x2f,0xff,0x87
    ,0x34,0x8e,0x43,0x44,0xc4,0xde,0xe9,0xcb},
    {0x54,0x7b,0x94,0x32,0xa6,0xc2,0x23,0x3d
    ,0xee,0x4c,0x95,0x0b,0x42,0xfa,0xc3,0x4e},
    {0x08,0x2e,0xa1,0x66,0x28,0xd9,0x24,0xb2
    ,0x76,0x5b,0xa2,0x49,0x6d,0x8b,0xd1,0x25},
    {0x72,0xf8,0xf6,0x64,0x86,0x68,0x98,0x16
    ,0xd4,0xa4,0x5c,0xcc,0x5d,0x65,0xb6,0x92},
    {0x6c,0x70,0x48,0x50,0xfd,0xed,0xb9,0xda
    ,0x5e,0x15,0x46,0x57,0xa7,0x8d,0x9d,0x84},
    {0x90,0xd8,0xab,0x00,0x8c,0xbc,0xd3,0x0a
    ,0xf7,0xe4,0x58,0x05,0xb8,0xb3,0x45,0x06},
    {0xd0,0x2c,0x1e,0x8f,0xca,0x3f,0x0f,0x02
    ,0xc1,0xaf,0xbd,0x03,0x01,0x13,0x8a,0x6b},
    {0x3a,0x91,0x11,0x41,0x4f,0x67,0xdc,0xea
    ,0x97,0xf2,0xcf,0xce,0xf0,0xb4,0xe6,0x73},
    {0x96,0xac,0x74,0x22,0xe7,0xad,0x35,0x85
    ,0xe2,0xf9,0x37,0xe8,0x1c,0x75,0xdf,0x6e},
    {0x47,0xf1,0x1a,0x71,0x1d,0x29,0xc5,0x89
    ,0x6f,0xb7,0x62,0x0e,0xaa,0x18,0xbe,0x1b},
    {0xfc,0x56,0x3e,0x4b,0xc6,0xd2,0x79,0x20
    ,0x9a,0xdb,0xc0,0xfe,0x78,0xcd,0x5a,0xf4},
    {0x1f,0xdd,0xa8,0x33,0x88,0x07,0xc7,0x31
    ,0xb1,0x12,0x10,0x59,0x27,0x80,0xec,0x5f},
    {0x60,0x51,0x7f,0xa9,0x19,0xb5,0x4a,0x0d
    ,0x2d,0xe5,0x7a,0x9f,0x93,0xc9,0x9c,0xef},
    {0xa0,0xe0,0x3b,0x4d,0xae,0x2a,0xf5,0xb0
    ,0xc8,0xeb,0xbb,0x3c,0x83,0x53,0x99,0x61},
    {0x17,0x2b,0x04,0x7e,0xba,0x77,0xd6,0x26
    ,0xe1,0x69,0x14,0x63,0x55,0x21,0x0c,0x7d}
};

#define BYTE_CHANGE_0(byte) \
    (aes128_s_1[(byte & 0xF0) >> 4][(byte & 0x0F)])
#define BYTE_CHANGE_1(byte) \
    (aes128_s_2[(byte & 0xF0) >> 4][(byte & 0x0F)])

static void byte_change(UNSIGNED_BYTE1 matrix[4][4], UNSIGNED_BYTE1 flag)
{
    UNSIGNED_BYTE1 i = 0, j = 0;
    if (flag == 0) {
        for (i = 0; i <= 3; i++)
            for (j = 0; j <= 3; j++)
                matrix[i][j] = BYTE_CHANGE_0(matrix[i][j]);
    }
    if (flag == 1) {
        for (i = 0; i <= 3; i++)
            for (j = 0; j <= 3; j++)
                matrix[i][j] = BYTE_CHANGE_1(matrix[i][j]);
    }
}

#endif

//2.行位移(状态矩阵按行位移:0移位(左移)1逆移位(右移))
#define STEP2_FOR_LINE_SHIFT
#ifdef  STEP2_FOR_LINE_SHIFT

static void line_shift(UNSIGNED_BYTE1 matrix[4][4], UNSIGNED_BYTE1 flag)
{   //不要用循环,实际循环的效率应低于批量赋值
    //(且批量赋值不增加太多代码量,可接受范围内)
    UNSIGNED_BYTE1 byte[4] = {0};
    //第0行不移位
    if (flag == 0) {//第1行左移一位
        byte[0] = matrix[1][0];
        matrix[1][0] = matrix[1][1];
        matrix[1][1] = matrix[1][2];
        matrix[1][2] = matrix[1][3];
        matrix[1][3] = byte[0];
    }
    if (flag == 1) {//第1行右移一位
        byte[0] = matrix[1][3];
        matrix[1][3] = matrix[1][2];
        matrix[1][2] = matrix[1][1];
        matrix[1][1] = matrix[1][0];
        matrix[1][0] = byte[0];
    }
    {   //第2行左右移俩位(右左移俩位)
        byte[0] = matrix[2][0];
        byte[1] = matrix[2][1];
        matrix[2][0] = matrix[2][2];
        matrix[2][1] = matrix[2][3];
        matrix[2][2] = byte[0];
        matrix[2][3] = byte[1];
    }
    if (flag == 0) {//第3行左移三位(右移一位)
        byte[3] = matrix[3][3];
        matrix[3][3] = matrix[3][2];
        matrix[3][2] = matrix[3][1];
        matrix[3][1] = matrix[3][0];
        matrix[3][0] = byte[3];
    }
    if (flag == 1) {//第3行右移三位(左移一位)
        byte[3] = matrix[3][0];
        matrix[3][0] = matrix[3][1];
        matrix[3][1] = matrix[3][2];
        matrix[3][2] = matrix[3][3];
        matrix[3][3] = byte[3];
    }
}

#endif

//3.列混合(状态矩阵按列混合:0混合1逆混合),基于GF(2^8)
#define STEP3_COLUMN_MIXTURE
#ifdef  STEP3_COLUMN_MIXTURE

//伽罗瓦域运算:GF(2^8)
#define GF_ADDITION(data1, data2) \
    ((data1) ^ (data2))
#define GF_MULTIPLY_2(data) \
    ((((data) & 0x80) == 0 ? 0x00 : 0x1b) ^ ((data) << 1))

static void column_mixture(UNSIGNED_BYTE1 matrix[4][4], UNSIGNED_BYTE1 flag)
{
    UNSIGNED_BYTE1 i = 0, j = 0, k = 0;
    UNSIGNED_BYTE1 byte[4][4] = {0};

    if (flag == 0) {
        //针对列混合矩阵进行定向优化
        for (i = 0; i <= 3; i++)
            for (j = 0; j <= 3; j++) {//第0列:
                UNSIGNED_BYTE1 result = 0;
                result ^= GF_MULTIPLY_2(matrix[(0 + i) % 4][j]);    //0x02
                result ^= GF_MULTIPLY_2(matrix[(1 + i) % 4][j]);    //0x03
                result ^= matrix[(1 + i) % 4][j];
                result ^= matrix[(2 + i) % 4][j];                   //0x01
                result ^= matrix[(3 + i) % 4][j];                   //0x01
                byte[i][j] = result;
            }
        for (i = 0; i <= 3; i++)
            for (j = 0; j <= 3; j++)
                matrix[i][j] = byte[i][j];
    }
    if (flag == 1) {
        UNSIGNED_BYTE1 temp = 0;
        //针对列混合矩阵进行定向优化
        for (i = 0; i <= 3; i++)
            for (j = 0; j <= 3; j++) {//第0列:
                UNSIGNED_BYTE1 result = 0;
                temp = matrix[(0 + i) % 4][j];          //0x0e
                result ^= (temp = GF_MULTIPLY_2(temp));
                result ^= (temp = GF_MULTIPLY_2(temp));
                result ^= (temp = GF_MULTIPLY_2(temp));
                temp = matrix[(1 + i) % 4][j];          //0x0b
                result ^= (temp);
                result ^= (temp = GF_MULTIPLY_2(temp));
                          (temp = GF_MULTIPLY_2(temp));
                result ^= (temp = GF_MULTIPLY_2(temp));
                temp = matrix[(2 + i) % 4][j];          //0x0d
                result ^= (temp);
                          (temp = GF_MULTIPLY_2(temp));
                result ^= (temp = GF_MULTIPLY_2(temp));
                result ^= (temp = GF_MULTIPLY_2(temp));
                temp = matrix[(3 + i) % 4][j];          //0x09
                result ^= (temp);
                          (temp = GF_MULTIPLY_2(temp));
                          (temp = GF_MULTIPLY_2(temp));
                result ^= (temp = GF_MULTIPLY_2(temp));
                byte[i][j] = result;
            }
        for (i = 0; i <= 3; i++)
            for (j = 0; j <= 3; j++)
                matrix[i][j] = byte[i][j];
    }

}

#if 0
/*
static const UNSIGNED_BYTE1 aes128_column_1[4][4] = {   //列混合矩阵
    {0x2,0x3,0x1,0x1},
    {0x1,0x2,0x3,0x1},
    {0x1,0x1,0x2,0x3},
    {0x3,0x1,0x1,0x2}
};

static const UNSIGNED_BYTE1 aes128_column_2[4][4] = {   //列混合逆矩阵
    {0xe,0xb,0xd,0x9},
    {0x9,0xe,0xb,0xd},
    {0xd,0x9,0xe,0xb},
    {0xb,0xd,0x9,0xe}
};

static UNSIGNED_BYTE1 gf_multiply(UNSIGNED_BYTE1 data1, UNSIGNED_BYTE1 data2)
{
    UNSIGNED_BYTE1 i = 1;
    UNSIGNED_BYTE1 temp = data1;
    UNSIGNED_BYTE1 result = 0;

    if ((data2 & 0x01) != 0)
        result ^= data1;
    for (i = 1; i < 8; i++) {
        temp = GF_MULTIPLY_2(temp);
        if ((data2 & (1 << i)) != 0)
            result ^= temp;
    }
    return result;
}

static void column_mixture(UNSIGNED_BYTE1 matrix[4][4], UNSIGNED_BYTE1 flag)
{
    UNSIGNED_BYTE1 i = 0, j = 0, k = 0;
    UNSIGNED_BYTE1 byte[4][4] = {0};

    UNSIGNED_BYTE1 temp = 0;
    if (flag == 0) {
        for (i = 0; i <= 3; i++)
            for (j = 0; j <= 3; j++) {
                UNSIGNED_BYTE1 result = 0;
                for (k = 0; k <= 3; k++) {
                    temp = gf_multiply(matrix[k][j],aes128_column_1[i][k]);
                    result = GF_ADDITION(result, temp);
                }
                byte[i][j] = result;
            }
        for (i = 0; i <= 3; i++)
            for (j = 0; j <= 3; j++)
                matrix[i][j] = byte[i][j];
    }
    if (flag == 1) {
        for (i = 0; i <= 3; i++)
            for (j = 0; j <= 3; j++) {
                UNSIGNED_BYTE1 result = 0;
                for (k = 0; k <= 3; k++) {
                    temp = gf_multiply(matrix[k][j],aes128_column_2[i][k]);
                    result = GF_ADDITION(result, temp);
                }
                byte[i][j] = result;
            }
        for (i = 0; i <= 3; i++)
            for (j = 0; j <= 3; j++)
                matrix[i][j] = byte[i][j];
    }
}
*/
#endif

#endif

//4.秘钥轮加(状态矩阵从上往下,从左往右:轮加,逆轮加一致)
#define STEP4_KEY_WHEEL
#ifdef  STEP4_KEY_WHEEL

static void key_wheel(UNSIGNED_BYTE1 matrix0[4][4],
                      UNSIGNED_BYTE1 matrix1[4][4])
{
    UNSIGNED_BYTE1 i = 0, j = 0;

    for (i = 0; i <= 3; i++)
        for (j = 0; j <= 3; j++)
            matrix0[i][j] ^= matrix1[i][j];
}


#endif

//5.秘钥扩展
#define STEP5_KEY_EXTERN
#ifdef  STEP5_KEY_EXTERN

static const UNSIGNED_BYTE1 aes128_round[10][4] = { //轮常量
    {0x01,0x00,0x00,0x00},
    {0x02,0x00,0x00,0x00},
    {0x04,0x00,0x00,0x00},
    {0x08,0x00,0x00,0x00},
    {0x10,0x00,0x00,0x00},
    {0x20,0x00,0x00,0x00},
    {0x40,0x00,0x00,0x00},
    {0x80,0x00,0x00,0x00},
    {0x1b,0x00,0x00,0x00},
    {0x36,0x00,0x00,0x00}
};

static void function_t(UNSIGNED_BYTE1 key[4], UNSIGNED_BYTE1 round)
{
    UNSIGNED_BYTE1 temp = 0;
    //1.字循环
    temp   = key[0];
    key[0] = key[1];
    key[1] = key[2];
    key[2] = key[3];
    key[3] = temp;
    //2.字节代换
    key[0] = BYTE_CHANGE_0(key[0]);
    key[1] = BYTE_CHANGE_0(key[1]);
    key[2] = BYTE_CHANGE_0(key[2]);
    key[3] = BYTE_CHANGE_0(key[3]);
    //3.轮常量异或
    if (round < 9)
        key[0] ^= (1 << (round - 1));
    if (round == 9)
        key[0] ^= 0x1b;
    if (round == 10)
        key[0] ^= 0x36;
}

static void key_extern(UNSIGNED_BYTE1 matrix[11][4][4], UNSIGNED_BYTE1 key[16])
{
    UNSIGNED_BYTE1  k = 0, t = 0;
    UNSIGNED_BYTE1  temp[2][16] = {0};
    UNSIGNED_BYTE1 *key_t = (void *)0;
    UNSIGNED_BYTE1 *key_0 = temp[0];    //前一轮
    UNSIGNED_BYTE1 *key_1 = temp[1];    //当前轮

    //1.初始状态矩阵
    KEY_TO_MATRIX(matrix[0], key);

    //2.初始上一轮数据
    for (k = 0; k < 16; k++)
        key_0[k] = key[k];

    //3.轮运算
    for (k = 1; k <= 10; k++) {
        for (t = 0; t < 16; t++)
            key_1[t] = key_0[t];
        //1.求轮函数
        function_t(&(key_0[12]), k);
        //2.求记录
        for (t = 0; t < 4; t++)
            key_1[t] ^= key_0[t + 12];
        for (t = 4; t < 16; t++)
            key_1[t] ^= key_1[t - 4];
        //3.保存当前轮记录
        KEY_TO_MATRIX(matrix[1], key_1);
        //交换
        key_t = key_0;
        key_0 = key_1;
        key_1 = key_t;
    }
}

#endif

//AES加密算法
#define AES128_SYMMETRIC_ENCRYPTION
#ifdef  AES128_SYMMETRIC_ENCRYPTION

void simple_aes128(UNSIGNED_BYTE1 *text,
                   UNSIGNED_BYTE4  textlength,
                   UNSIGNED_BYTE1  key[16],
                   UNSIGNED_BYTE1  flag)
{
    //1.建立状态矩阵
    UNSIGNED_BYTE4 i = 0, k = 0;
    UNSIGNED_BYTE1 aes128_matrix[11][4][4] = {0};
    UNSIGNED_BYTE1 matrix[4][4] = {0};
    //2.扩展秘钥
    key_extern(aes128_matrix, key);
    //3.迭代加密或解密
    if (flag == 0) {
        for (i = 0; i < textlength; i += 16) {
            KEY_TO_MATRIX(matrix, &(text[i]));
            //1.起始秘钥轮加
            {
                key_wheel(matrix, aes128_matrix[0]);
            }
            //2.前九轮
            for (k = 1; k <= 9; k++) {
                //1.字节代换
                byte_change(matrix, 0);
                //2.行移位
                line_shift(matrix, 0);
                //3.列混合
                column_mixture(matrix, 0);
                //4.秘钥轮加
                key_wheel(matrix, aes128_matrix[k]);
            }
            //3.第十轮
            {
                //1.字节代换
                byte_change(matrix, 0);
                //2.行移位
                line_shift(matrix, 0);
                //4.秘钥轮加
                key_wheel(matrix, aes128_matrix[10]);
            }
            MATRIX_TO_KEY(matrix, &(text[i]));
        }
    }
    if (flag == 1) {
        //额外动作:解密时部分扩展秘钥逆列混合
        for (k = 9; k >= 1; k--)
            column_mixture(aes128_matrix[k], 1);

        for (i = 0; i < textlength; i += 16) {
            KEY_TO_MATRIX(matrix, &(text[i]));
            //1.起始秘钥轮加
            {
                key_wheel(matrix, aes128_matrix[10]);
            }
            //2.前九轮
            for (k = 9; k >= 1; k--) {
                //1.逆字节代换
                byte_change(matrix, 1);
                //2.逆行移位
                line_shift(matrix, 1);
                //3.逆列混合
                column_mixture(matrix, 1);
                //4.秘钥轮加
                key_wheel(matrix, aes128_matrix[k]);
            }
            //3.第十轮
            {
                //1.逆字节代换
                byte_change(matrix, 1);
                //2.逆行移位
                line_shift(matrix, 1);
                //4.秘钥轮加
                key_wheel(matrix, aes128_matrix[0]);
            }
            MATRIX_TO_KEY(matrix, &(text[i]));
        }
    }
}

#endif
