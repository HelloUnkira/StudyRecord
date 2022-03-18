#ifndef RING_BUFFER_H
#define RING_BUFFER_H
//C std lib
#include <stdint.h>
#include <stdbool.h>

/*实现目标:
 *可兼容1,2,4,8字节对齐的拷贝式环形队列
 *该实现参考于zephyr\lib\os
 */

typedef struct RingBufferBody {
    /* 类型缓冲区: */
    union {
        void     *Buffer;   /* 抽象缓冲区 */
        uint8_t  *Buffer1;  /* 缓冲区(1字节对齐) */
        uint16_t *Buffer2;  /* 缓冲区(2字节对齐) */
        uint32_t *Buffer4;  /* 缓冲区(4字节对齐) */
        uint64_t *Buffer8;  /* 缓冲区(8字节对齐) */
    };
    uint8_t  Type;      /* 字节对齐类型(1,2,4,8) */
    uint32_t Size;      /* 最大数据条目 */
    uint32_t Mask;      /* 最大条目掩码(加速运算) */
    uint32_t Head;      /* 队列起始位置(进动点,非实际下标) */
    uint32_t Tail;      /* 队列结束位置(进动点,非实际下标) */
} RingBuffer;

//环形队列为空判断
static inline bool RingBuffer_IsEmpty(RingBuffer *Instance)
{
    return (Instance->Head == Instance->Tail) ? true : false;
}

//重置环形队列
static inline void RingBuffer_Reset(RingBuffer *Instance)
{
    Instance->Head = 0;
    Instance->Tail = 0;
}

//获取环形队列已有条目
static inline uint32_t RingBuffer_GetItem(RingBuffer *Instance)
{
    return Instance->Tail - Instance->Head;
}

//获取环形队列空闲条目
static inline uint32_t RingBuffer_GetSpace(RingBuffer *Instance)
{
    return Instance->Size - (Instance->Tail - Instance->Head);
}

/*@brief        初始化环形队列(无参数检查)
 *              当满足Buffer为字节对齐且Size为2的次方达到最大效率
 *@param[in]    Instance    实例
 *@param[in]    Type        数据单元类型:(1,2,4,8)(字节对齐)
 *@param[in]    Buffer      指定的缓冲区,为对齐的字流(不是字节流)(如下)
 *@param[in]    Size        对齐字流的长度
 */
static inline void RingBuffer_Init(RingBuffer *Instance, uint8_t Type, void *Buffer, uint32_t Size)
{
    /* 简要的字节对齐修正 */
    /* 地址截断,通过最后几位确定是否字节对齐 */
    uint8_t Offset = (uint8_t)Buffer & (Type - 1);
    
    if (Offset != 0) {
        Buffer = (((uint8_t *)Buffer) + Type - Offset);
        Size--;
    }

    Instance->Head   = 0;
    Instance->Tail   = 0;
    Instance->Type   = Type;
	Instance->Size   = Size;
	Instance->Buffer = Buffer;
    /* 缓冲区大小字节对齐加速 */
	Instance->Mask = ((Size != 0) && ((Size & (Size - 1)) == 0)) ? (Size - 1) : 0;
}

/*@brief        从环形队列获取数据(无参数检查)
 *@param[in]    Instance    实例
 *@param[out]   Data        指定对齐类型数据的存储地址(非对齐可能会导致截断的情况)
 *@param[in]    Length      所需获取数据长度
 *@retval       -1          数据不足
 *@retval       -2          实例类型错误
 */
int32_t RingBuffer_Gets(RingBuffer *Instance, void *Data, uint32_t Length);

/*@brief        向环形队列推送数据(无参数检查)
 *@param[in]    Instance    实例
 *@param[out]   Data        指定对齐类型数据的存储地址(非对齐可能会导致截断的情况)
 *@param[in]    Length      所需推送数据长度
 *@retval       -1:         空间不足
 */
int32_t RingBuffer_Puts(RingBuffer *Instance, void *Data, uint32_t Length);

#endif
