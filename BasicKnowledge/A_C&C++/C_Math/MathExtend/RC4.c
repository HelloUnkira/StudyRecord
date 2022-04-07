#include "Cflint.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 备注:该部分摘要于其他 */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* KSA */
void RC4Set(RC4Key *Key, uint8_t *Data, uint32_t Length)
{
    uint32_t Index = 0;
    //1.初始化status[Index1] = Index1;
    for (uint32_t Index1 = 0; Index1 < 256; Index1++)
        Key->Status[Index1] = Index1;
    //2.初始化排列status[Index1]
    for (uint32_t Index1 = 0; Index1 < 256; Index1++) {
        Index = (Index + Data[Index1 % Length] + Key->Status[Index1]) & 255;
        /* 交换Index1和Index */
        uint8_t Temp0 = Key->Status[Index1];
        uint8_t Temp1 = Key->Status[Index];
        Key->Status[Index1] = Temp1;
        Key->Status[Index]  = Temp0;
    }
    /* 3.初始化状态量 */
    Key->StatusI = 0;
    Key->StatusJ = 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* PRGA */
static uint8_t RC4Output(RC4Key *Key)
{
    /* 更新状态量 */
    Key->StatusI += 1;
    Key->StatusI &= 255;
    Key->StatusJ += Key->Status[Key->StatusI];
    Key->StatusJ &= 255;
    /* 交换状态量 */
    uint8_t Temp0 = Key->Status[Key->StatusI];
    uint8_t Temp1 = Key->Status[Key->StatusJ];
    Key->Status[Key->StatusI] = Temp1;
    Key->Status[Key->StatusJ] = Temp0;
    /* 计算输出索引 */
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
