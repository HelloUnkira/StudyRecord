//C std lib
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
//内源依赖项
#include "RingBuffer.h"

/* 环形缓冲区所使用到的算法 */
#define RINGBUFFER_MAX_LEN     0x80000000

//索引求模运算
static uint32_t RingBuffer_Modulo(RingBuffer *Instance, uint32_t Value)
{
    return (Instance->Mask != 0) ? (Value & Instance->Mask) : (Value % Instance->Size);
}

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


/*@brief        从环形队列获取数据(无参数检查)
 *@param[in]    Instance    实例
 *@param[out]   Data        指定对齐类型数据的存储地址(非对齐可能会导致截断的情况)
 *@param[in]    Length      所需获取数据长度
 *@retval       -1          数据不足
 *@retval       -2          实例类型错误
 */
int32_t RingBuffer_Gets(RingBuffer *Instance, void *Data, uint32_t Length)
{
    uint32_t index = 0, index1 = 0;
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
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Head) & Instance->Mask;
                Buffer1[index] = Instance->Buffer1[index1];
            }
        if (Instance->Mask == 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Head) % Instance->Size;
                Buffer1[index] = Instance->Buffer1[index1];
            }
        break;
    case 2:
        if (Instance->Mask != 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Head) & Instance->Mask;
                Buffer2[index] = Instance->Buffer2[index1];
            }
        if (Instance->Mask == 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Head) % Instance->Size;
                Buffer2[index] = Instance->Buffer2[index1];
            }
        break;
    case 4:
        if (Instance->Mask != 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Head) & Instance->Mask;
                Buffer4[index] = Instance->Buffer4[index1];
            }
        if (Instance->Mask == 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Head) % Instance->Size;
                Buffer4[index] = Instance->Buffer4[index1];
            }
        break;
    case 8:
        if (Instance->Mask != 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Head) & Instance->Mask;
                Buffer8[index] = Instance->Buffer8[index1];
            }
        if (Instance->Mask == 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Head) % Instance->Size;
                Buffer8[index] = Instance->Buffer8[index1];
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

/*@brief        向环形队列推送数据(无参数检查)
 *@param[in]    Instance    实例
 *@param[out]   Data        指定对齐类型数据的存储地址(非对齐可能会导致截断的情况)
 *@param[in]    Length      所需推送数据长度
 *@retval       -1:         空间不足
 */
int32_t RingBuffer_Puts(RingBuffer *Instance, void *Data, uint32_t Length)
{
    uint32_t index = 0, index1 = 0;
    uint8_t  *Buffer1 = Data;
    uint16_t *Buffer2 = Data;
    uint32_t *Buffer4 = Data;
    uint64_t *Buffer8 = Data;
    
    if (RingBuffer_GetSpace(Instance) < Length)
        return -1;

    switch (Instance->Type) {
    case 1:
        if (Instance->Mask != 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Tail) & Instance->Mask;
                Instance->Buffer1[index1] = Buffer1[index];
            }
        if (Instance->Mask == 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Tail) % Instance->Size;
                Instance->Buffer1[index1] = Buffer1[index];
            }
        break;
    case 2:
        if (Instance->Mask != 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Tail) & Instance->Mask;
                Instance->Buffer2[index1] = Buffer2[index];
            }
        if (Instance->Mask == 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Tail) % Instance->Size;
                Instance->Buffer2[index1] = Buffer2[index];
            }
        break;
    case 4:
        if (Instance->Mask != 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Tail) & Instance->Mask;
                Instance->Buffer4[index1] = Buffer4[index];
            }
        if (Instance->Mask == 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Tail) % Instance->Size;
                Instance->Buffer4[index1] = Buffer4[index];
            }
        break;
    case 8:
        if (Instance->Mask != 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Tail) & Instance->Mask;
                Instance->Buffer8[index1] = Buffer8[index];
            }
        if (Instance->Mask == 0)
            for (index = 0; index < Length; index++) {
                index1 = (index + Instance->Tail) % Instance->Size;
                Instance->Buffer8[index1] = Buffer8[index];
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


