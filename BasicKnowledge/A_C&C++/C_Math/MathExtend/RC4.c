#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* ��ע:�ò���ժҪ������ */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* KSA */
void RC4Set(RC4Key *Key, uint8_t *Data, uint32_t Length)
{
    uint32_t Index = 0;
    //1.��ʼ��status[Index1] = Index1;
    for (uint32_t Index1 = 0; Index1 < 256; Index1++)
        Key->Status[Index1] = Index1;
    //2.��ʼ������status[Index1]
    for (uint32_t Index1 = 0; Index1 < 256; Index1++) {
        Index = (Index + Data[Index1 % Length] + Key->Status[Index1]) & 255;
        /* ����Index1��Index */
        uint8_t Temp0 = Key->Status[Index1];
        uint8_t Temp1 = Key->Status[Index];
        Key->Status[Index1] = Temp1;
        Key->Status[Index]  = Temp0;
    }
    /* 3.��ʼ��״̬�� */
    Key->StatusI = 0;
    Key->StatusJ = 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* PRGA */
static uint8_t RC4Output(RC4Key *Key)
{
    /* ����״̬�� */
    Key->StatusI += 1;
    Key->StatusI &= 255;
    Key->StatusJ += Key->Status[Key->StatusI];
    Key->StatusJ &= 255;
    /* ����״̬�� */
    uint8_t Temp0 = Key->Status[Key->StatusI];
    uint8_t Temp1 = Key->Status[Key->StatusJ];
    Key->Status[Key->StatusI] = Temp1;
    Key->Status[Key->StatusJ] = Temp0;
    /* ����������� */
    uint32_t Index = 0;
    Index += Key->Status[Key->StatusI];
    Index += Key->Status[Key->StatusJ];
    Index &= 255;
    return Key->Status[Index];
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void RC4Run(RC4Key *Key, uint8_t *Input, uint8_t *Output, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Output[Index] = Input[Index] ^ RC4Output(Key);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
