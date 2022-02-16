#include "extern_digit_calculate.h"

#define EXTERN_DIGIT_CALCULATE_OPERATION_AUXILIARY
#ifdef  EXTERN_DIGIT_CALCULATE_OPERATION_AUXILIARY

//copy: operand[0] = operand[1]
static void copy(digit_type *digit)
{
    DIGIT_BYTE *operand0 = (*digit).operand[0];
    DIGIT_BYTE *operand1 = (*digit).operand[1];
    DIGIT_CHAR4 length   = (*digit).length;

    while (--length >= 0)
        operand0[length] = operand1[length];
}

//set: operand[0] = value
static void set_value(digit_type *digit, DIGIT_BYTE value)
{
    DIGIT_BYTE *operand0 = (*digit).operand[0];
    DIGIT_CHAR4 length   = (*digit).length;

    while (--length >= 0)
        operand0[length] = value;
}

//compare: (operand[0] > operand[1] ?  1 :
//         (operand[0] < operand[1] ? -1 : 0))
static DIGIT_CHAR1 compare(digit_type *digit)
{
    DIGIT_BYTE *operand0 = (*digit).operand[0];
    DIGIT_BYTE *operand1 = (*digit).operand[1];
    DIGIT_CHAR4 length   = (*digit).length;
    
    DIGIT_CHAR1 temp = 0;
    while (--length >= 0)
        if (operand0[length] != operand1[length]) {
            if (operand0[length] > operand1[length])
                temp =  1;
            if (operand0[length] < operand1[length])
                temp = -1;
            break;
        }

    return temp;
}

//confirm: (operand[0] == operand[1] ? 0 : 1)
static DIGIT_CHAR1 equal(digit_type *digit)
{
    DIGIT_BYTE *operand0 = (*digit).operand[0];
    DIGIT_BYTE *operand1 = (*digit).operand[1];
    DIGIT_CHAR4 length   = (*digit).length;

    DIGIT_BYTE temp = 0;
    while(--length >= 0)
        temp |= operand0[length] ^ operand1[length];
    
    return (temp == 0 ? 0 : 1);
}

//comfirm: (operand[0] == 0) ? 1 : 0;
static DIGIT_CHAR1 is_zero(digit_type *digit)
{
    DIGIT_BYTE *operand0 = (*digit).operand[0];
    DIGIT_CHAR4 length   = (*digit).length;
    
    DIGIT_CHAR1 temp = 0;
    while (--length >= 0)
        if (operand0[length] != (DIGIT_BYTE)0) {
            temp = 1;
            break;
        }

    return ((temp == 0) ? 1 : 0);
}

#endif

#define EXTERN_DIGIT_CALCULATE_OPERATION_BASE
#ifdef  EXTERN_DIGIT_CALCULATE_OPERATION_BASE

//calculate: result(temp:carry) = operand[0] + operand[1]
static void addition(digit_type *digit)
{
    DIGIT_BYTE *result   = (*digit).result;
    DIGIT_BYTE *operand0 = (*digit).operand[0];
    DIGIT_BYTE *operand1 = (*digit).operand[1];
    DIGIT_CHAR4 length   = (*digit).length;
    DIGIT_CHAR4 temp     = (*digit).temp = 0;
    
    DIGIT_CHAR4 i = 0;
    for (i = 0; i < length; i++) {
        result[i]  = temp;
        result[i] += operand0[i];
        result[i] += operand1[i];

        if (result[i] < operand0[i])
            temp = 1;
        if (result[i] > operand0[i])
            temp = 0;
    }
    (*digit).temp = temp;
}

//calculate: result(temp:borrow) = operand[0] - operand[1]
static void subtraction(digit_type *digit)
{
    DIGIT_BYTE *result   = (*digit).result;
    DIGIT_BYTE *operand0 = (*digit).operand[0];
    DIGIT_BYTE *operand1 = (*digit).operand[1];
    DIGIT_CHAR4 length   = (*digit).length;
    DIGIT_CHAR4 temp     = (*digit).temp = 0;

    DIGIT_CHAR4 i = 0;
    for (i = 0; i < length; i++) {
        result[i]  = operand0[i];
        result[i] -= operand1[i];
        result[i] -= temp;

        if (result[i] > operand0[i])
            temp = 1;
        if (result[i] < operand0[i])
            temp = 0;
    }
    (*digit).temp = temp;
}

//compute(����):
//����߰���λΪ0,�Ͱ����Ϊ1
static void bit_n_half_byte(DIGIT_BYTE *result)
{
    static DIGIT_BYTE  bit_n_half_byte_result = 0;
    static DIGIT_CHAR1 bit_n_half_byte_first_calculate = 0;
    //����ֻ��Ҫȥ��һ�μ���
    if (bit_n_half_byte_first_calculate == 0) {
        bit_n_half_byte_first_calculate = 1;

        DIGIT_CHAR4 i = 0;
        for (i = 0; i < DIGIT_BITS_BYTE / 2; i++) {
            bit_n_half_byte_result <<= 1;
            bit_n_half_byte_result  |= 1;
        }
    }
    *result = bit_n_half_byte_result;
}

//compute(����):
//����λ�˷���(�����λָDIGIT_TYPE�ֽ��½��Ƶ�λ)
//����:0,1,�����λ2,�����λ3,��λ4
static void bit_n_multiply_addition_1(DIGIT_BYTE *data)
{
#ifdef DIGIT_BYTE_2
//���ùؼ����Ż�����
    DIGIT_BYTE_2 result_xy = 0;
    DIGIT_BYTE_2 result_01 = 0;
    //1.����������
    result_xy |= ((DIGIT_BYTE_2)data[0]);
    result_xy *= ((DIGIT_BYTE_2)data[1]);
    //2.������һλ�Ľ�λ
    result_01 |= ((DIGIT_BYTE_2)data[2]);
    result_01 |= ((DIGIT_BYTE_2)data[3] << DIGIT_BITS_BYTE);
    //3.�������ս��
    result_01 += result_xy;
    //4.���μ����Ƿ��λ�������
    if (result_01 < result_xy)
        data[4] = 1;
    if (result_01 >= result_xy)
        data[4] = 0;
    //5.���汾�μ���ĵ�λ�͸�λ
    data[2] = (DIGIT_BYTE)(result_01 >> 0);
    data[3] = (DIGIT_BYTE)(result_01 >> DIGIT_BITS_BYTE);
#else
//�����ùؼ����Ż�����
    DIGIT_BYTE half_byte  = 0;
    DIGIT_BYTE result_xy0 = 0;
    DIGIT_BYTE result_xy1 = 0;
    bit_n_half_byte(&half_byte);
    //1.��ó����ĸ�λ���λ
    DIGIT_BYTE x0 = data[0] & half_byte;
    DIGIT_BYTE x1 = data[0] >> (DIGIT_BITS_BYTE / 2);
    DIGIT_BYTE y0 = data[1] & half_byte;
    DIGIT_BYTE y1 = data[1] >> (DIGIT_BITS_BYTE / 2);
    //2.�ֱ�����λ���λ�Ľ���
    DIGIT_BYTE i0 = x0 * y0;
    DIGIT_BYTE i1 = x0 * y1;
    DIGIT_BYTE i2 = x1 * y0;
    DIGIT_BYTE i3 = x1 * y1;
    //3.�������
    DIGIT_BYTE overflow = i0 >> (DIGIT_BITS_BYTE / 2); //��λ�Ľ�λ
    DIGIT_BYTE result_0 = i1 + i2 + overflow;          //��λ��ֵ
    DIGIT_BYTE result_1 = i3;                          //��λ��ֵ
    if (result_0 < i1)                                 //��λ���
        result_1 += (1 << (DIGIT_BITS_BYTE / 2));
    //4.�����յ�ֵ
    result_xy0 |= (i0 & half_byte);
    result_xy0 |= (result_0 << (DIGIT_BITS_BYTE / 2));
    result_xy1 |= (result_1);
    result_xy1 += (result_0 >> (DIGIT_BITS_BYTE / 2));
    //5.���汾�μ���ĵ�λ�͸�λ
    data[2] += result_xy0;
    data[3] += result_xy1;
    //6.���μ����Ƿ��λ�������
    if (data[2] < result_xy0)
        data[3] += 1;
    //7.���μ����Ƿ��λ�������
    if ((data[3]  < result_xy1) ||
        (data[3] == result_xy1 && data[2] < result_xy0))
         data[4] += 1;
#endif
}

//compute(����):
//����λ�˷���(�����λָDIGIT_TYPE�ֽ��½��Ƶ�λ)
//����:0,1,�����λ2,�����λ3,��λ4
static void bit_n_multiply_addition_2(DIGIT_BYTE *data)
{
#ifdef DIGIT_BYTE_2
//���ùؼ����Ż�����
    DIGIT_BYTE_2 result_xy = 0;
    DIGIT_BYTE_2 result_01 = 0;
    //1.����������
    result_xy |= ((DIGIT_BYTE_2)data[0]);
    result_xy *= ((DIGIT_BYTE_2)data[1]);
    //2.������һλ�Ľ�λ
    result_01 |= ((DIGIT_BYTE_2)data[2]);
    result_01 |= ((DIGIT_BYTE_2)data[3] << DIGIT_BITS_BYTE);
    //3.�����λ
    data[4] += (result_xy >> (DIGIT_BITS_BYTE * 2 - 1));
    //4.�������ս��
    result_xy *= 2;
    result_01 += result_xy;
    //4.���μ����Ƿ��λ�������
    if (result_01 < result_xy)
        data[4] += 1;
    if (result_01 >= result_xy)
        data[4] += 0;
    //5.���汾�μ���ĵ�λ�͸�λ
    data[2] = (DIGIT_BYTE)(result_01 >> 0);
    data[3] = (DIGIT_BYTE)(result_01 >> DIGIT_BITS_BYTE);
#else
//�����ùؼ����Ż�����
    DIGIT_BYTE half_byte  = 0;
    DIGIT_BYTE result_xy0 = 0;
    DIGIT_BYTE result_xy1 = 0;
    bit_n_half_byte(&half_byte);
    //1.��ó����ĸ�λ���λ
    DIGIT_BYTE x0 = data[0] & half_byte;
    DIGIT_BYTE x1 = data[0] >> (DIGIT_BITS_BYTE / 2);
    DIGIT_BYTE y0 = data[1] & half_byte;
    DIGIT_BYTE y1 = data[1] >> (DIGIT_BITS_BYTE / 2);
    //2.�ֱ�����λ���λ�Ľ���
    DIGIT_BYTE i0 = x0 * y0;
    DIGIT_BYTE i1 = x0 * y1;
    DIGIT_BYTE i2 = x1 * y0;
    DIGIT_BYTE i3 = x1 * y1;
    //3.�������
    DIGIT_BYTE overflow = i0 >> (DIGIT_BITS_BYTE / 2); //��λ�Ľ�λ
    DIGIT_BYTE result_0 = i1 + i2 + overflow;          //��λ��ֵ
    DIGIT_BYTE result_1 = i3;                          //��λ��ֵ
    if (result_0 < i1)                                 //��λ���
        result_1 += (1 << (DIGIT_BITS_BYTE / 2));
    //4.�����յ�ֵ
    result_xy0 |= (i0 & half_byte);
    result_xy0 |= (result_0 << (DIGIT_BITS_BYTE / 2));
    result_xy1 |= (result_1);
    result_xy1 += (result_0 >> (DIGIT_BITS_BYTE / 2));
    //5.�����λ
    data[4] += (result_xy1 >> (DIGIT_BITS_BYTE - 1));
    //5.���汾�μ���ĵ�λ�͸�λ
    result_xy1 <<= 1;
    result_xy1  |= (result_xy0 >> (DIGIT_BITS_BYTE - 1));
    result_xy0 <<= 1;

    data[2] += result_xy0;
    data[3] += result_xy1;
    //6.���μ����Ƿ��λ�������
    if (data[2] < result_xy0)
        data[3] += 1;
    //7.���μ����Ƿ��λ�������
    if ((data[3]  < result_xy1) ||
        (data[3] == result_xy1 && data[2] < result_xy0))
         data[4] += 1;
#endif
}

//calculate: result = operand[0] * operand[1]
//����:result[0,2*length)
static void multiply(digit_type *digit)
{
    DIGIT_BYTE *result   = (*digit).result;
    DIGIT_BYTE *operand0 = (*digit).operand[0];
    DIGIT_BYTE *operand1 = (*digit).operand[1];
    DIGIT_CHAR4 length   = (*digit).length;

    DIGIT_BYTE bit_data[5] = {0};
    DIGIT_CHAR4 i = 0;
    DIGIT_CHAR4 k = 0;

    for (k = 0; k < length; k++) {
        for (i = 0; i <= k; i++) {
            bit_data[0] = operand0[i];
            bit_data[1] = operand1[k - i];
            //ʹ��λ�˷���
            bit_n_multiply_addition_1(bit_data);
        }
        result[k]   = bit_data[2];  //������λ
        bit_data[2] = bit_data[3];  //��λ���λ
        bit_data[3] = bit_data[4];  //��λ���λ
        bit_data[4] = 0;            //��λ��0
    }
    for (k = length; k < length * 2 - 1; k++) {
        for (i = (k + 1) - length; i < length; i++) {
            bit_data[0] = operand0[i];
            bit_data[1] = operand1[k - i];
            //ʹ��λ�˷���
            bit_n_multiply_addition_1(bit_data);
        }
        result[k]   = bit_data[2];  //������λ
        bit_data[2] = bit_data[3];  //��λ���λ
        bit_data[3] = bit_data[4];  //��λ���λ
        bit_data[4] = 0;            //��λ��0
    }

    result[length * 2 - 1] = bit_data[2];
}

//calculate: result = operand[0] * operand[0]
//����:result[0,2*length)
static void square(digit_type *digit)
{
    DIGIT_BYTE *result   = (*digit).result;
    DIGIT_BYTE *operand0 = (*digit).operand[0];
    DIGIT_CHAR4 length   = (*digit).length;

    DIGIT_BYTE bit_data[5] = {0};
    DIGIT_CHAR4 minimum = 0;
    DIGIT_CHAR4 i = 0;
    DIGIT_CHAR4 k = 0;

    for (k = 0; k < length * 2 - 1; k++) {
        minimum = (k < length ? 0 : ((k + 1) - length));
        for (i = minimum; i <= k && i <= k - i; i++) {
            bit_data[0] = operand0[i];
            bit_data[1] = operand0[k - i];
            if (i <  k - i)  //ʹ��λ�˷���
                bit_n_multiply_addition_2(bit_data);
            if (i >= k - i)
                bit_n_multiply_addition_1(bit_data);
        }
        result[k]   = bit_data[2];  //������λ
        bit_data[2] = bit_data[3];  //��λ���λ
        bit_data[3] = bit_data[4];  //��λ���λ
        bit_data[4] = 0;            //��λ��0
    }

    result[length * 2 - 1] = bit_data[2];
}

//calculate: result <<= temp
//(temp / DIGIT_BITS_BYTE <= length)
//��һ��������temp��bitλ(�����λָ2���Ƶ�λ)
static void shift_left(digit_type *digit)
{
    DIGIT_BYTE *result = (*digit).result;
    DIGIT_CHAR4 length = (*digit).length;
    DIGIT_CHAR4 temp   = (*digit).temp;

    //һ��DIGIT_TYPE�ֽ��½��Ƶ�λ�ж��ٸ�2����λ
    DIGIT_CHAR4 bits_n = temp / DIGIT_BITS_BYTE;
    DIGIT_CHAR4 bits_2 = temp % DIGIT_BITS_BYTE;
    DIGIT_CHAR4 last_2 = DIGIT_BITS_BYTE - bits_2;
    DIGIT_BYTE  bit_n1 = 0, bit_high = 0;
    DIGIT_BYTE  bit_n2 = 0, bit_low = 0;

    DIGIT_CHAR4 i = 0;

    for (i = length - 1; i - bits_n - 1 >= 0; i--) {
        bit_n1    = result[i - bits_n];
        bit_n2    = result[i - bits_n - 1];
        bit_high  = (bit_n1 << bits_2);
        bit_low   = (bit_n2 >> last_2);
        bit_low   = (last_2 >= DIGIT_BITS_BYTE) ? 0 : bit_low;
        result[i] = bit_high | bit_low;
    }
    bit_n1 = result[i - bits_n];
    //bit_n2 = result[i - bits_n - 1];
    bit_high  = (bit_n1 << bits_2);
    //bit_low   = (bit_n2 >> last_2);
    //bit_low   = (last_2 >= DIGIT_BITS_BYTE) ? 0 : bit_low;
    result[i] = bit_high;// | (bit_n2 >> last_2);

    for (i = 0; i - bits_n < 0; i++)
        result[i] = 0;
}

//calculate: result >>= temp
//(temp / DIGIT_BITS_BYTE <= length)
//��һ��������temp��bitλ(�����λָ2���Ƶ�λ)
static void shift_right(digit_type *digit)
{
    DIGIT_BYTE *result = (*digit).result;
    DIGIT_CHAR4 length = (*digit).length;
    DIGIT_CHAR4 temp   = (*digit).temp;

    //һ��DIGIT_TYPE�ֽ��½��Ƶ�λ�ж��ٸ�2����λ
    DIGIT_CHAR4 bits_n = temp / DIGIT_BITS_BYTE;
    DIGIT_CHAR4 bits_2 = temp % DIGIT_BITS_BYTE;
    DIGIT_CHAR4 last_2 = DIGIT_BITS_BYTE - bits_2;
    DIGIT_BYTE  bit_n1 = 0, bit_low = 0;
    DIGIT_BYTE  bit_n2 = 0, bit_high = 0;

    DIGIT_CHAR4 i = 0;

    for (i = 0; i + bits_n + 1 < length; i++) {
        bit_n1    = result[i + bits_n];
        bit_n2    = result[i + bits_n + 1];
        bit_low   = (bit_n1 >> bits_2);
        bit_high  = (bit_n2 << last_2);
        bit_high  = (last_2 >= DIGIT_BITS_BYTE) ? 0 : bit_high;
        result[i] = bit_low | bit_high;
    }
    bit_n1 = result[i + bits_n];
    //bit_n2 = result[i + bits_n + 1];
    bit_low = (bit_n1 >> bits_2);
    //bit_high = (bit_n2 << last_2);
    //bit_high = (last_2 >= DIGIT_BITS_BYTE) ? 0 : bit_high;
    result[i] = bit_low;// | (bit_n2 << last_2);

    for (i = length - bits_n; i < length; i++)
        result[i] = 0;
}

//calculate: result = result % operand[0]
//����:operand[1]
static void module_base(digit_type *digit)
{
    //����һ���������Ż�����ģ����
    DIGIT_BYTE *result   = (*digit).result;
    DIGIT_BYTE *operand0 = (*digit).operand[0];
    DIGIT_BYTE *operand1 = (*digit).operand[1];
    DIGIT_CHAR4 length   = (*digit).length;


    //��ȡ��λ
    DIGIT_CHAR4 offset = 0;
    while (1) {
        //result <=> operand0
        (*digit).operand[0] = result;
        (*digit).operand[1] = operand0;
        if (compare(digit) < 0)
            break;
        
        //���λ��Ϊ0,��λһ�����
        if(operand0[length - 1] != 0)
            break;

        //operand0 <<= 1(DIGIT_TYPE�ֽ��½��Ƶ�λ)
        (*digit).temp   = 1;
        (*digit).result = operand0;
        shift_left(digit);
        //��λ��¼
        offset++;
    }

    while (offset >= 0) {
        //result <=> operand0
        (*digit).operand[0] = result;
        (*digit).operand[1] = operand0;
        if (compare(digit) < 0) {
            //����Ѿ����˵�����״̬
            if (--offset < 0)
                break;
            //operand0 >>= 1(DIGIT_TYPE�ֽ��½��Ƶ�λ)
            (*digit).temp   = 1;
            (*digit).result = operand0;
            shift_right(digit);
            continue;
        }

        //result->operand1
        (*digit).operand[0] = operand1;
        (*digit).operand[1] = result;
        copy(digit);

        //result = operand1 - operand0
        (*digit).operand[0] = operand1;
        (*digit).operand[1] = operand0;
        (*digit).result     = result;
        subtraction(digit);

    }

    //���꼯�ϻ�ԭ
    (*digit).operand[0] = operand0;
    (*digit).operand[1] = operand1;
    (*digit).result     = result;
}

#endif

#define EXTERN_DIGIT_CALCULATE_OPERATION_MONTGOMERY
#ifdef  EXTERN_DIGIT_CALCULATE_OPERATION_MONTGOMERY

//calculate: result = (operand[0] + operand[1]) % operand[2]
//condition: operand1[0] < operand[2] && operand[1] < operand[2]
//����:operand[3]
static void modulo_addition(digit_type *digit)
{
    DIGIT_BYTE *result   = (*digit).result;
    DIGIT_BYTE *operand0 = (*digit).operand[0];
    DIGIT_BYTE *operand1 = (*digit).operand[1];
    DIGIT_BYTE *operand2 = (*digit).operand[2];
    DIGIT_BYTE *operand3 = (*digit).operand[3];

    addition(digit);
    if ((*digit).temp != 0) {
        //result->operand3
        (*digit).operand[0] = operand3;
        (*digit).operand[1] = result;
        copy(digit);
        
        //result = operand3 - operand2
        (*digit).operand[0] = operand3;
        (*digit).operand[1] = operand2;
        subtraction(digit);
    } else {
        //result <=> operand2
        (*digit).operand[0] = result;
        (*digit).operand[1] = operand2;
        if (compare(digit) >= 0) {
            //result->operand3
            (*digit).operand[0] = operand3;
            (*digit).operand[1] = result;
            copy(digit);
            
            //result = operand3 - operand2
            (*digit).operand[0] = operand3;
            (*digit).operand[1] = operand2;
            subtraction(digit);
        }
    }

    (*digit).operand[0] = operand0;
    (*digit).operand[1] = operand1;
}

//calculate: result = (operand[0] - operand[1]) % operand[2]
//condition: operand1[0] < operand[2] && operand[1] < operand[2]
//����:operand[3]
static void modulo_subtraction(digit_type *digit)
{
    DIGIT_BYTE *result   = (*digit).result;
    DIGIT_BYTE *operand0 = (*digit).operand[0];
    DIGIT_BYTE *operand1 = (*digit).operand[1];
    DIGIT_BYTE *operand2 = (*digit).operand[2];
    DIGIT_BYTE *operand3 = (*digit).operand[3];

    subtraction(digit);
    if ((*digit).temp != 0) {
        //result->operand3
        (*digit).operand[0] = operand3;
        (*digit).operand[1] = result;
        copy(digit);
        
        //result = operand3 + operand2
        (*digit).operand[0] = operand3;
        (*digit).operand[1] = operand2;
        addition(digit);

        (*digit).operand[0] = operand0;
        (*digit).operand[1] = operand1;
        return;
    }
}

//calculate: result = result % operand[0]
static void modulo_fast(digit_type *digit)
{
    //�ȴ����
    //uECCԴ��Ϊ����������Բ�������˶��Ƶ��Ż�
    //��ͦͺȻ��
    //��������ô˵
    //�����Ѿ��ṩ���㹻�Ľӿ�Ϊ֮ʹ��
    module_base(digit);
}

//calculate: result = (operand[0] * operand[1]) % operand[2]
//����:result[0,2*length),operand[2][0,2*length),operand[3][0,2*length)
static void modulo_multiply(digit_type *digit)
{   //�ȳ˺�ģ
    DIGIT_BYTE *operand0 = (*digit).operand[0];
    DIGIT_BYTE *operand1 = (*digit).operand[1];
    DIGIT_BYTE *operand2 = (*digit).operand[2];
    DIGIT_BYTE *operand3 = (*digit).operand[3];
    multiply(digit);
    (*digit).length    *= 2;
    (*digit).operand[0] = operand2;
    (*digit).operand[1] = operand3;
    modulo_fast(digit);
    //��ԭ������
    (*digit).operand[0] = operand0;
    (*digit).operand[1] = operand1;
    (*digit).length    /= 2;
}

//calculate: result = (operand[0] * operand[0]) % operand[1]
//����:result[0,2*length),operand[1][0,2*length),operand[2][0,2*length)
static void modulo_square(digit_type *digit)
{   //�ȳ˺�ģ
    DIGIT_BYTE *operand0 = (*digit).operand[0];
    DIGIT_BYTE *operand1 = (*digit).operand[1];
    DIGIT_BYTE *operand2 = (*digit).operand[2];
    square(digit);
    (*digit).length    *= 2;
    (*digit).operand[0] = operand1;
    (*digit).operand[1] = operand2;
    modulo_fast(digit);
    //��ԭ������
    (*digit).operand[0] = operand0;
    (*digit).operand[1] = operand1;
    (*digit).length    /= 2;
}

//calculate: result = (1 / operand[0]) % operand[1]
//����:operand[2],operand[3],operand[4],operand[5]
static void modulo_inverse(digit_type *digit)
{
    DIGIT_BYTE *result   = (*digit).result;
    DIGIT_BYTE *operand0 = (*digit).operand[0]; //input
    DIGIT_BYTE *operand1 = (*digit).operand[1]; //module
    DIGIT_BYTE *operand2 = (*digit).operand[2]; //a
    DIGIT_BYTE *operand3 = (*digit).operand[3]; //b
    DIGIT_BYTE *operand4 = (*digit).operand[4]; //u
    DIGIT_BYTE *operand5 = (*digit).operand[5]; //v
    DIGIT_CHAR4 length   = (*digit).length;
    DIGIT_CHAR4 temp     = (*digit).temp = 0;

    //����Ϊ0���
    if (is_zero(digit) == 1) {
        (*digit).operand[0] = result;
        set_value(digit, 0);
        (*digit).operand[0] = operand0;
        return;
    }

    //����input  -->operand2
    (*digit).operand[0] = operand2;
    (*digit).operand[1] = operand0;
    copy(digit);
    //����module -->operand3
    (*digit).operand[0] = operand3;
    (*digit).operand[1] = operand1;
    copy(digit);
    //��� operand4 �� operand5
    (*digit).operand[0] = operand4;
    set_value(digit, 0);
    (*digit).operand[0] = operand5;
    set_value(digit, 0);
    //���� operand4 = 1
    operand4[0] = 1;
    //��ʼ
    DIGIT_BYTE  bit_2_highest  = 1 << (DIGIT_BITS_BYTE - 1);
    DIGIT_CHAR1 compare_result = 0;
    DIGIT_BYTE  overflow = 0;
    while (1) {
        //�Ƚϲ�����operand2,operand3
        (*digit).operand[0] = operand2;
        (*digit).operand[1] = operand3;
        compare_result = compare(digit);
        if (compare_result == 0)
            break;
        overflow = 0;

        //1.����:operand2���λΪ0
        //2.����:operand3���λΪ0
        if (operand2[0] & 1 == 0 ||
            operand3[0] & 1 == 0) {
            DIGIT_BYTE *target1 = (void *)0;
            DIGIT_BYTE *target2 = (void *)0;

            if (operand3[0] & 1 == 0) {
                target1 = operand3;
                target2 = operand5;
            }
            if (operand2[0] & 1 == 0) {
                target1 = operand2;
                target2 = operand4;
            }

            //target1 >>= 1
            (*digit).temp = 1;
            (*digit).result = target1;
            shift_right(digit);
            //target2���λ��Ϊ0
            if (target2[0] & 1 != 0) {
                (*digit).result     = result;
                (*digit).operand[0] = target2;
                (*digit).operand[1] = operand1;
                addition(digit);
                overflow = (*digit).temp;
                //result-->target2
                (*digit).operand[0] = target2;
                (*digit).operand[1] = result;
                copy(digit);
                //target2 >>= 1
                (*digit).temp = 1;
                (*digit).result = target2;
                shift_right(digit);
                if (overflow != 0)
                    target2[length - 2] |= bit_2_highest;
            }
            continue;
        }

        //3.����:operand2 >  operand3
        //4.����:operand2 <= operand3
        DIGIT_BYTE *minimum0 = (void *)0;
        DIGIT_BYTE *minimum1 = (void *)0;
        DIGIT_BYTE *maximum0 = (void *)0;
        DIGIT_BYTE *maximum1 = (void *)0;
        
        if (compare_result > 0) {
            maximum0 = operand2; //a
            maximum1 = operand4; //u
            minimum0 = operand3; //b
            minimum1 = operand5; //v
        }
        if (compare_result <= 0) {
            maximum0 = operand3; //b
            maximum1 = operand5; //v
            minimum0 = operand2; //a
            minimum1 = operand4; //u
        }

        //maximum0 -= minimum0
        (*digit).result     = result;
        (*digit).operand[0] = maximum0;
        (*digit).operand[1] = minimum0;
        subtraction(digit);
        (*digit).operand[0] = maximum0;
        (*digit).operand[1] = result;
        copy(digit);
        //maximum0 >>= 1
        (*digit).temp = 1;
        (*digit).result = maximum0;
        shift_right(digit);
        //compare(maximum1, minimum1)
        (*digit).operand[0] = maximum1;
        (*digit).operand[1] = minimum1;
        if (compare(digit) < 0) {
            //maximum1 += operand1
            (*digit).result     = result;
            (*digit).operand[0] = maximum1;
            (*digit).operand[1] = operand1;
            addition(digit);
            (*digit).operand[0] = maximum1;
            (*digit).operand[1] = result;
            copy(digit);
        }
        //maximum1 -= minimum1
        (*digit).result     = result;
        (*digit).operand[0] = maximum1;
        (*digit).operand[1] = minimum1;
        subtraction(digit);
        (*digit).operand[0] = maximum1;
        (*digit).operand[1] = result;
        copy(digit);
        //maximum1���λ��Ϊ0
        if (maximum1[0] & 1 != 0) {
            //maximum1 += operand1
            (*digit).result     = result;
            (*digit).operand[0] = maximum1;
            (*digit).operand[1] = operand1;
            addition(digit);
            overflow = (*digit).temp;
            (*digit).operand[0] = maximum1;
            (*digit).operand[1] = result;
            copy(digit);
        }
        //maximum1 >>= 1
        (*digit).temp = 1;
        (*digit).result = maximum1;
        shift_right(digit);
        if (overflow != 0)
            maximum1[length - 2] |= bit_2_highest;
        continue;
    }

    //�������ݽ��
    (*digit).operand[0] = result;
    (*digit).operand[1] = operand4;
    copy(digit);

    //��ԭ������
    (*digit).result     = result;
    (*digit).operand[0] = operand0;
    (*digit).operand[1] = operand1;
}

#endif

#define EXTERN_DIGIT_CALCULATE_OPERATION_JACOBIAN
#ifdef  EXTERN_DIGIT_CALCULATE_OPERATION_JACOBIAN
//calculate: 2 * (operand[0],operand[1],operand[2]) % operand[3]
//����:[0,2*length)           (Jacobian����)
static void multiply_double(digit_type *digit)
{
    DIGIT_BYTE *result   = (*digit).result;
    DIGIT_BYTE *operand0 = (*digit).operand[0]; //t1 = X
    DIGIT_BYTE *operand1 = (*digit).operand[1]; //t2 = Y
    DIGIT_BYTE *operand2 = (*digit).operand[2]; //t3 = Z
    DIGIT_BYTE *operand3 = (*digit).operand[3]; //module
    DIGIT_BYTE *operand4 = (*digit).operand[4]; //t4
    DIGIT_BYTE *operand5 = (*digit).operand[5]; //t5
    DIGIT_BYTE *operand6 = (*digit).operand[6]; //temp

    //���Z1 == 0
    (*digit).operand[0] = operand2;
    if (is_zero(digit) == 1) {
        (*digit).operand[0] = operand0;
        return;
    }

//���ּ��㷽ʽ(ʡ��):
#if CONDITION == 1

#endif

#if CONDITION == 2

#endif
}

#endif

//���ͷų�ȥ�Ľӿ�
void * get_digit_function_base(digit_function_base *function)
{
    static digit_function_base local_function_base = {
        .addition    = addition,
        .subtraction = subtraction,
        .multiply    = multiply,
        .square      = square,
        .shift_left  = shift_left,
        .shift_right = shift_right,
        .module_base = module_base,
    };

    if (function != (void *)0) {
        function->addition    = addition;
        function->subtraction = subtraction;
        function->multiply    = multiply;
        function->square      = square;
        function->shift_left  = shift_left;
        function->shift_right = shift_right;
        function->module_base = module_base;
    }

    return &local_function_base;
}

void * get_digit_function_mentgomery(digit_function_mentgomery *function)
{
    static digit_function_mentgomery local_function_mentgomery = {
        .modulo_addition    = modulo_addition,
        .modulo_subtraction = modulo_subtraction,
        .modulo_fast        = modulo_fast,
        .modulo_multiply    = modulo_multiply,
        .modulo_square      = modulo_square,
        .modulo_inverse     = modulo_inverse,
    };

    if (function != (void *)0) {
        function->modulo_addition    = modulo_addition;
        function->modulo_subtraction = modulo_subtraction;
        function->modulo_fast        = modulo_fast;
        function->modulo_multiply    = modulo_multiply;
        function->modulo_square      = modulo_square;
        function->modulo_inverse     = modulo_inverse;
    }

    return &local_function_mentgomery;
}
