#ifndef EXTERN_DIGIT_CALCULATE_H
#define EXTERN_DIGIT_CALCULATE_H

#define EXTERN_DIGIT_CALCULATE_CONFIGURE
#ifdef  EXTERN_DIGIT_CALCULATE_CONFIGURE
//ƽ̨������ͳһ
//���ȫ����sizeof(unsigned char)ʱ
//����Щ�������һ��,��,�ǻᶪ������
#define DIGIT_CHAR1   signed char
#define DIGIT_BYTE1 unsigned char
#define DIGIT_CHAR2   signed short
#define DIGIT_BYTE2 unsigned short
#define DIGIT_CHAR4   signed int
#define DIGIT_BYTE4 unsigned int
#define DIGIT_CHAR8   signed long
#define DIGIT_BYTE8 unsigned long

//ƽ̨�ֽڶ���,��ӦΪsizeof(void *)
//��Ԥ����������sizeofִ��,����ֻ���ֶ�ָ��
#define DIGIT_BYTE    DIGIT_BYTE4
#define DIGIT_CHAR    DIGIT_CHAR4
//һ���ֽ�8λ(��Ȼ��֪��Ϊʲô���Ҳ��Ҫָ��)
#define DIGIT_BITS_BYTE1 8
#define DIGIT_BITS_BYTE  (sizeof(DIGIT_BYTE) * DIGIT_BITS_BYTE1)
//������Ż�����,ǰ������Ŀ����Ż�
//����һ��������xxx == 2 * sizeof(DIGIT_BYTE)
//#define DIGIT_BYTE_2  DIGIT_BYTE8
//#define DIGIT_CHAR_2  DIGIT_CHAR8
#endif

/**
 *��չ������
 *������:�޷�����(Ҳ������������)
 *������:��λ��ǰ��λ�ں�[0, length)
 *���ƹ���:һ��DIGIT_BYTEΪһ������λ
***/
typedef struct extern_digit_calculate_type {
    DIGIT_BYTE **operand; //1.����������
    DIGIT_BYTE  *result;  //2.������
    DIGIT_CHAR4  length;  //3.����������:[0, length)
    DIGIT_CHAR4  temp;    //4.���㸨��λ
} digit_type;

/**
 *ǿ��:
 *�������ݴ���
 *�������غϲ�����ַ
***/

typedef struct extern_digit_calculate_function_base {
    //calculate: result(temp:carry) = operand[0] + operand[1]
    //calculate: result(temp:borrow) = operand[0] - operand[1]
    void (*addition)(digit_type *digit);
    void (*subtraction)(digit_type *digit);
    //calculate: result = operand[0] * operand[1]
    //calculate: result = operand[0] * operand[0]
    //����:result[0,2*length)
    void (*multiply)(digit_type *digit);
    void (*square)(digit_type *digit);
    //calculate: result >>= temp
    //(temp / DIGIT_BITS_BYTE <= length)
    //��һ������/����temp��bitλ(�����λָ2���Ƶ�λ)
    void (*shift_left)(digit_type *digit);
    void (*shift_right)(digit_type *digit);
    //calculate: result = result % operand[0]
    //����:operand[1],
    void (*module_base)(digit_type *digit);
} digit_function_base;

typedef struct extern_digit_calculate_function_montgomery {
    //calculate: result = (operand[0] + operand[1]) % operand[2]
    //calculate: result = (operand[0] - operand[1]) % operand[2]
    //condition: operand1[0] < operand[2] && operand[1] < operand[2]
    //����:operand[3]
    void (*modulo_addition)(digit_type *digit);
    void (*modulo_subtraction)(digit_type *digit);
    //calculate: result = result % operand[0]
    void (*modulo_fast)(digit_type *digit);
    //calculate: result = (operand[0] * operand[1]) % operand[2]
    //calculate: result = (operand[0] * operand[0]) % operand[1]
    //����:result[0,2*length),operand[2][0,2*length),operand[3][0,2*length)
    //����:result[0,2*length),operand[1][0,2*length),operand[2][0,2*length)
    void (*modulo_multiply)(digit_type *digit);
    void (*modulo_square)(digit_type *digit);
    //calculate: result = (1 / operand[0]) % operand[1]
    //����:operand[2],operand[3],operand[4],operand[5]
    void (*modulo_inverse)(digit_type *digit);
} digit_function_mentgomery;

/**
 *��ȡ������:
 *����ֵ��ò������׵�ַ
 *������ò�������ַ����(���� != (void *)0)
***/
void * get_digit_function_base(digit_function_base *function);
void * get_digit_function_mentgomery(digit_function_mentgomery *function);

#endif

/**


//������չŷ������㷨: a * x + b * y == gcd(a, b)
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
        //���ҽ������ݲſɼ���,Ҳ��,�ǵݹ������ջ����
        UNSIGNED_TYPE temp = *y;
        *y = (*x) - (a / b) * (*y);
        *x = temp;
    }
}

***/

//��������
#ifdef EXTERN_DIGIT_CALCULATE_TEST

#include <stdio.h>
#define LENGTH 3

static void extern_digit_calculate_base_test(void)
{
    digit_type data;
    digit_function_base *func_b = 0;
    //������
    DIGIT_BYTE  result[LENGTH * 2] = {0};
    DIGIT_BYTE  operand0[LENGTH] = {0};
    DIGIT_BYTE  operand1[LENGTH] = {0};
    DIGIT_BYTE  operand2[LENGTH] = {0};
    DIGIT_BYTE *operand[3]  = {0};
    //�󶨲�����
    operand[0] = operand0;
    operand[1] = operand1;
    operand[2] = operand2;

    //�󶨼���
    data.result  = result;
    data.operand = operand;
    data.length  = LENGTH;
    //��ȡ������
    func_b = get_digit_function_base(NULL);
    //
    DIGIT_CHAR4 i = 0;
    //����

    //����1:����λ
    for (i = 0; i < LENGTH; i++)
        result[i] = (DIGIT_BYTE)(~0);
    data.temp = 33;
    (*func_b).shift_left(&data);
    printf("\nresult:\n");
    for (i = 0; i < LENGTH; i++)
        printf("%x ", result[i]);
    //����2:����λ
    for (i = 0; i < LENGTH; i++)
        result[i] = (DIGIT_BYTE)(~0);
    data.temp = 33;
    (*func_b).shift_right(&data);
    printf("\nresult:\n");
    for (i = 0; i < LENGTH; i++)
        printf("%x ", result[i]);

    //����3:�ӷ�
    for (i = 0; i < LENGTH; i++) {
        operand0[i] = (DIGIT_BYTE)(~0);
        operand1[i] = (DIGIT_BYTE)(~0);
    }
    (*func_b).addition(&data);
    printf("\nresult:%d\n",data.temp);
    for (i = 0; i < LENGTH; i++)
        printf("%x ", result[i]);
    //����4:����
    for (i = 0; i < LENGTH; i++) {
        operand0[i] = (DIGIT_BYTE)(0);
        operand1[i] = (DIGIT_BYTE)(~0);
    }
    (*func_b).subtraction(&data);
    printf("\nresult:%d\n",data.temp);
    for (i = 0; i < LENGTH; i++)
        printf("%x ", result[i]);
    //����5:�˷�
    for (i = 0; i < LENGTH; i++) {
        operand0[i] = (DIGIT_BYTE)(1);
        operand1[i] = (DIGIT_BYTE)(1);
    }
    (*func_b).multiply(&data);
    printf("\nresult:\n");
    for (i = 0; i < LENGTH * 2; i++)
        printf("%x ", result[i]);
    //����6:�˷�
    for (i = 0; i < LENGTH; i++) {
        operand0[i] = (DIGIT_BYTE)(1);
    }
    (*func_b).square(&data);
    printf("\nresult:\n");
    for (i = 0; i < LENGTH * 2; i++)
        printf("%x ", result[i]);
    printf("\n");
    //����6:ģ
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
    //������
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
    //�󶨲�����
    operand[0] = operand0;
    operand[1] = operand1;
    operand[2] = operand2;
    operand[3] = operand3;
    operand[4] = operand4;
    operand[5] = operand5;

    //�󶨼���
    data.result  = result;
    data.operand = operand;
    data.length  = LENGTH;
    //��ȡ������
    func_b = get_digit_function_mentgomery(NULL);
    //
    DIGIT_CHAR4 i = 0;
    //����

    //����1:ģ��
    //����2:ģ��
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
    //����3:��ģ��
    for (i = 0; i < LENGTH; i++) {
       operand0[i] = (DIGIT_BYTE)(1);
       operand1[i] = (DIGIT_BYTE)(1);
    }
    operand0[0] = (DIGIT_BYTE)(1);
    (*func_b).modulo_inverse(&data);
    printf("\nresult:\n");
    for (i = 0; i < LENGTH; i++)
        printf("%x ", result[i]);
    //����4:�ȳ˺�ģ,�ȷ���ģ
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


