#ifndef EXTERN_DIGIT_CALCULATE_H
#define EXTERN_DIGIT_CALCULATE_H

#define EXTERN_DIGIT_CALCULATE_CONFIGURE
#ifdef  EXTERN_DIGIT_CALCULATE_CONFIGURE
//平台层配置统一
//如果全部用sizeof(unsigned char)时
//做这些动作多此一举,但,那会丢掉性能
#define DIGIT_CHAR1   signed char
#define DIGIT_BYTE1 unsigned char
#define DIGIT_CHAR2   signed short
#define DIGIT_BYTE2 unsigned short
#define DIGIT_CHAR4   signed int
#define DIGIT_BYTE4 unsigned int
#define DIGIT_CHAR8   signed long
#define DIGIT_BYTE8 unsigned long

//平台字节对齐,它应为sizeof(void *)
//但预编译优先与sizeof执行,所以只能手动指定
#define DIGIT_BYTE    DIGIT_BYTE4
#define DIGIT_CHAR    DIGIT_CHAR4
//一个字节8位(虽然不知道为什么这个也需要指定)
#define DIGIT_BITS_BYTE1 8
#define DIGIT_BITS_BYTE  (sizeof(DIGIT_BYTE) * DIGIT_BITS_BYTE1)
//这可以优化策略,前提是真的可以优化
//存在一种整形量xxx == 2 * sizeof(DIGIT_BYTE)
//#define DIGIT_BYTE_2  DIGIT_BYTE8
//#define DIGIT_CHAR_2  DIGIT_CHAR8
#endif

/**
 *扩展数运算
 *数类型:无符号数(也就是俩个正数)
 *数特性:高位在前低位在后[0, length)
 *进制规则:一个DIGIT_BYTE为一个数据位
***/
typedef struct extern_digit_calculate_type {
    DIGIT_BYTE **operand; //1.操作数集合
    DIGIT_BYTE  *result;  //2.运算结果
    DIGIT_CHAR4  length;  //3.操作数长度:[0, length)
    DIGIT_CHAR4  temp;    //4.运算辅助位
} digit_type;

/**
 *强调:
 *无数据容错检查
 *不接受重合操作地址
***/

typedef struct extern_digit_calculate_function_base {
    //calculate: result(temp:carry) = operand[0] + operand[1]
    //calculate: result(temp:borrow) = operand[0] - operand[1]
    void (*addition)(digit_type *digit);
    void (*subtraction)(digit_type *digit);
    //calculate: result = operand[0] * operand[1]
    //calculate: result = operand[0] * operand[0]
    //满足:result[0,2*length)
    void (*multiply)(digit_type *digit);
    void (*square)(digit_type *digit);
    //calculate: result >>= temp
    //(temp / DIGIT_BITS_BYTE <= length)
    //对一个数左/右移temp个bit位(这里的位指2进制的位)
    void (*shift_left)(digit_type *digit);
    void (*shift_right)(digit_type *digit);
    //calculate: result = result % operand[0]
    //满足:operand[1],
    void (*module_base)(digit_type *digit);
} digit_function_base;

typedef struct extern_digit_calculate_function_montgomery {
    //calculate: result = (operand[0] + operand[1]) % operand[2]
    //calculate: result = (operand[0] - operand[1]) % operand[2]
    //condition: operand1[0] < operand[2] && operand[1] < operand[2]
    //满足:operand[3]
    void (*modulo_addition)(digit_type *digit);
    void (*modulo_subtraction)(digit_type *digit);
    //calculate: result = result % operand[0]
    void (*modulo_fast)(digit_type *digit);
    //calculate: result = (operand[0] * operand[1]) % operand[2]
    //calculate: result = (operand[0] * operand[0]) % operand[1]
    //满足:result[0,2*length),operand[2][0,2*length),operand[3][0,2*length)
    //满足:result[0,2*length),operand[1][0,2*length),operand[2][0,2*length)
    void (*modulo_multiply)(digit_type *digit);
    void (*modulo_square)(digit_type *digit);
    //calculate: result = (1 / operand[0]) % operand[1]
    //满足:operand[2],operand[3],operand[4],operand[5]
    void (*modulo_inverse)(digit_type *digit);
} digit_function_mentgomery;

/**
 *获取操作集:
 *返回值获得操作集首地址
 *参数获得操作集地址拷贝(参数 != (void *)0)
***/
void * get_digit_function_base(digit_function_base *function);
void * get_digit_function_mentgomery(digit_function_mentgomery *function);

#endif

/**


//计算扩展欧几里得算法: a * x + b * y == gcd(a, b)
static void gcd_external(UNSIGNED_TYPE a,
                         UNSIGNED_TYPE b,
                         UNSIGNED_TYPE *gcd,
                         UNSIGNED_TYPE *x,
                         UNSIGNED_TYPE *y)
{
    if (b == 0) {
        *x   = 1;
        *y   = 0;
        *gcd = a;
    } else {
        calculate_extern_gcd(b, a % b, x, y);
        //当且仅当回溯才可计算,也即,非递归仅仅堆栈策略
        UNSIGNED_TYPE temp = *y;
        *y = (*x) - (a / b) * (*y);
        *x = temp;
    }
}

***/

//测试用例
#ifdef EXTERN_DIGIT_CALCULATE_TEST

#include <stdio.h>
#define LENGTH 3

static void extern_digit_calculate_base_test(void)
{
    digit_type data;
    digit_function_base *func_b = 0;
    //操作数
    DIGIT_BYTE  result[LENGTH * 2] = {0};
    DIGIT_BYTE  operand0[LENGTH] = {0};
    DIGIT_BYTE  operand1[LENGTH] = {0};
    DIGIT_BYTE  operand2[LENGTH] = {0};
    DIGIT_BYTE *operand[3]  = {0};
    //绑定操作数
    operand[0] = operand0;
    operand[1] = operand1;
    operand[2] = operand2;

    //绑定集合
    data.result  = result;
    data.operand = operand;
    data.length  = LENGTH;
    //获取操作集
    func_b = get_digit_function_base(NULL);
    //
    DIGIT_CHAR4 i = 0;
    //测试

    //测试1:左移位
    for (i = 0; i < LENGTH; i++)
        result[i] = (DIGIT_BYTE)(~0);
    data.temp = 33;
    (*func_b).shift_left(&data);
    printf("\nresult:\n");
    for (i = 0; i < LENGTH; i++)
        printf("%x ", result[i]);
    //测试2:右移位
    for (i = 0; i < LENGTH; i++)
        result[i] = (DIGIT_BYTE)(~0);
    data.temp = 33;
    (*func_b).shift_right(&data);
    printf("\nresult:\n");
    for (i = 0; i < LENGTH; i++)
        printf("%x ", result[i]);

    //测试3:加法
    for (i = 0; i < LENGTH; i++) {
        operand0[i] = (DIGIT_BYTE)(~0);
        operand1[i] = (DIGIT_BYTE)(~0);
    }
    (*func_b).addition(&data);
    printf("\nresult:%d\n",data.temp);
    for (i = 0; i < LENGTH; i++)
        printf("%x ", result[i]);
    //测试4:减法
    for (i = 0; i < LENGTH; i++) {
        operand0[i] = (DIGIT_BYTE)(0);
        operand1[i] = (DIGIT_BYTE)(~0);
    }
    (*func_b).subtraction(&data);
    printf("\nresult:%d\n",data.temp);
    for (i = 0; i < LENGTH; i++)
        printf("%x ", result[i]);
    //测试5:乘法
    for (i = 0; i < LENGTH; i++) {
        operand0[i] = (DIGIT_BYTE)(1);
        operand1[i] = (DIGIT_BYTE)(1);
    }
    (*func_b).multiply(&data);
    printf("\nresult:\n");
    for (i = 0; i < LENGTH * 2; i++)
        printf("%x ", result[i]);
    //测试6:乘方
    for (i = 0; i < LENGTH; i++) {
        operand0[i] = (DIGIT_BYTE)(1);
    }
    (*func_b).square(&data);
    printf("\nresult:\n");
    for (i = 0; i < LENGTH * 2; i++)
        printf("%x ", result[i]);
    printf("\n");
    //测试6:模
    for (i = 0; i < LENGTH; i++) {
        result[i]   = (DIGIT_BYTE)(i);
        operand0[i] = (DIGIT_BYTE)(1);
    }
    (*func_b).module_base(&data);
    printf("\nresult:\n");
    for (i = 0; i < LENGTH; i++)
        printf("%x ", result[i]);
}

static void extern_digit_calculate_mentgomery_test(void)
{
    digit_type data;
    digit_function_mentgomery *func_b = 0;
    //操作数
    DIGIT_BYTE  result[LENGTH * 2]         = {0};
    DIGIT_BYTE  operand_module[LENGTH * 2] = {0};
    DIGIT_BYTE  operand_module_1[LENGTH * 2] = {0};
    DIGIT_BYTE  operand0[LENGTH] = {0};
    DIGIT_BYTE  operand1[LENGTH] = {0};
    DIGIT_BYTE  operand2[LENGTH] = {0};
    DIGIT_BYTE  operand3[LENGTH] = {0};
    DIGIT_BYTE  operand4[LENGTH] = {0};
    DIGIT_BYTE  operand5[LENGTH] = {0};
    DIGIT_BYTE *operand[6]  = {0};
    //绑定操作数
    operand[0] = operand0;
    operand[1] = operand1;
    operand[2] = operand2;
    operand[3] = operand3;
    operand[4] = operand4;
    operand[5] = operand5;

    //绑定集合
    data.result  = result;
    data.operand = operand;
    data.length  = LENGTH;
    //获取操作集
    func_b = get_digit_function_mentgomery(NULL);
    //
    DIGIT_CHAR4 i = 0;
    //测试

    //测试1:模加
    //测试2:模减
    for (i = 0; i < LENGTH; i++) {
        operand0[i] = (DIGIT_BYTE)(LENGTH - 1 - i);
        operand1[i] = (DIGIT_BYTE)(LENGTH - 1 - i);
        operand2[i] = (DIGIT_BYTE)(1);
    }
    //(*func_b).modulo_addition(&data);
    //(*func_b).modulo_subtraction(&data);
    printf("\nresult:\n");
    for (i = 0; i < LENGTH; i++)
        printf("%x ", result[i]);
    //测试3:求模逆
    for (i = 0; i < LENGTH; i++) {
       operand0[i] = (DIGIT_BYTE)(1);
       operand1[i] = (DIGIT_BYTE)(1);
    }
    operand0[0] = (DIGIT_BYTE)(1);
    (*func_b).modulo_inverse(&data);
    printf("\nresult:\n");
    for (i = 0; i < LENGTH; i++)
        printf("%x ", result[i]);
    //测试4:先乘后模,先方后模
    for (i = 0; i < LENGTH; i++) {
        operand0[i] = (DIGIT_BYTE)(1);
        operand1[i] = result[i];
        operand_module[i] = (DIGIT_BYTE)(1);
    }
    operand0[0] = (DIGIT_BYTE)(0);
    data.operand[2] = operand_module;
    data.operand[3] = operand_module_1;
    (*func_b).modulo_multiply(&data);
    data.operand[2] = operand2;
    data.operand[3] = operand3;
    //
    //data.operand[1] = operand_module;
    //data.operand[2] = operand_module_1;
    //(*func_b).modulo_square(&data);
    //data.operand[1] = operand1;
    //data.operand[2] = operand2;
    printf("\nresult:\n");
    for (i = 0; i < LENGTH * 2; i++)
        printf("%x ", result[i]);

}


#endif


