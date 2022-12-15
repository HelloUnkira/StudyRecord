//C std lib
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
//内源依赖项
#include "RingBuffer.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 环形缓冲区所使用到的算法 */
#define RINGBUFFER_MAX_LEN     0x80000000
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//索引求模运算
static uint32_t RingBuffer_Modulo(RingBuffer *Instance, uint32_t Value)
{
    return (Instance->Mask != 0) ? (Value & Instance->Mask) : (Value % Instance->Size);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//索引回退
static void RingBuffer_RewindIndex(RingBuffer *Instance)
{
    if (Instance->Head >= RINGBUFFER_MAX_LEN) {
        
        uint32_t Rewind = RINGBUFFER_MAX_LEN;
        
        Rewind /= Instance->Size;
        Rewind *= Instance->Size;

        Instance->Head -= Rewind;
        Instance->Tail -= Rewind;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/*@brief        从环形队列获取数据(无参数检查)
 *@param[in]    Instance    实例
 *@param[out]   Data        指定对齐类型数据的存储地址(非对齐可能会导致截断的情况)
 *@param[in]    Length      所需获取数据长度
 *@retval       -1          数据不足
 *@retval       -2          实例类型错误
 */
int32_t RingBuffer_Gets(RingBuffer *Instance, void *Data, uint32_t Length)
{
    uint8_t  *Buffer1 = Data;
    uint16_t *Buffer2 = Data;
    uint32_t *Buffer4 = Data;
    uint64_t *Buffer8 = Data;
    
    if (RingBuffer_IsEmpty(Instance) == true)
        return -1;
    if (RingBuffer_GetItem(Instance) < Length)
        return -1;

    switch (Instance->Type) {
    case 1:
        if (Instance->Mask != 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Head) & Instance->Mask;
                 Buffer1[Index] = Instance->Buffer1[Index1];
            }
        if (Instance->Mask == 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Head) % Instance->Size;
                 Buffer1[Index] = Instance->Buffer1[Index1];
            }
        break;
    case 2:
        if (Instance->Mask != 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Head) & Instance->Mask;
                 Buffer2[Index] = Instance->Buffer2[Index1];
            }
        if (Instance->Mask == 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Head) % Instance->Size;
                 Buffer2[Index] = Instance->Buffer2[Index1];
            }
        break;
    case 4:
        if (Instance->Mask != 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Head) & Instance->Mask;
                 Buffer4[Index] = Instance->Buffer4[Index1];
            }
        if (Instance->Mask == 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Head) % Instance->Size;
                 Buffer4[Index] = Instance->Buffer4[Index1];
            }
        break;
    case 8:
        if (Instance->Mask != 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Head) & Instance->Mask;
                 Buffer8[Index] = Instance->Buffer8[Index1];
            }
        if (Instance->Mask == 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Head) % Instance->Size;
                 Buffer8[Index] = Instance->Buffer8[Index1];
            }
        break;
    default:
        return -2;
    }

    Instance->Head += Length;
    
    //索引回退
    RingBuffer_RewindIndex(Instance);
    
    return 0;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/*@brief        向环形队列推送数据(无参数检查)
 *@param[in]    Instance    实例
 *@param[out]   Data        指定对齐类型数据的存储地址(非对齐可能会导致截断的情况)
 *@param[in]    Length      所需推送数据长度
 *@retval       -1:         空间不足
 */
int32_t RingBuffer_Puts(RingBuffer *Instance, void *Data, uint32_t Length)
{
    uint8_t  *Buffer1 = Data;
    uint16_t *Buffer2 = Data;
    uint32_t *Buffer4 = Data;
    uint64_t *Buffer8 = Data;
    
    if (RingBuffer_GetSpace(Instance) < Length)
        return -1;

    switch (Instance->Type) {
    case 1:
        if (Instance->Mask != 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Tail) & Instance->Mask;
                 Instance->Buffer1[Index1] = Buffer1[Index];
            }
        if (Instance->Mask == 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Tail) % Instance->Size;
                 Instance->Buffer1[Index1] = Buffer1[Index];
            }
        break;
    case 2:
        if (Instance->Mask != 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Tail) & Instance->Mask;
                 Instance->Buffer2[Index1] = Buffer2[Index];
            }
        if (Instance->Mask == 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Tail) % Instance->Size;
                 Instance->Buffer2[Index1] = Buffer2[Index];
            }
        break;
    case 4:
        if (Instance->Mask != 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Tail) & Instance->Mask;
                 Instance->Buffer4[Index1] = Buffer4[Index];
            }
        if (Instance->Mask == 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Tail) % Instance->Size;
                 Instance->Buffer4[Index1] = Buffer4[Index];
            }
        break;
    case 8:
        if (Instance->Mask != 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Tail) & Instance->Mask;
                 Instance->Buffer8[Index1] = Buffer8[Index];
            }
        if (Instance->Mask == 0)
            for (uint32_t Index = 0; Index < Length; Index++) {
                 uint32_t Index1 = (Index + Instance->Tail) % Instance->Size;
                 Instance->Buffer8[Index1] = Buffer8[Index];
            }
        break;
    default:
        return -2;
    }

    Instance->Tail += Length;
    
    //索引回退
    RingBuffer_RewindIndex(Instance);
    
    return 0;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
