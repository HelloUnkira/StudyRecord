#ifndef RING_BUFFER_H
#define RING_BUFFER_H
//C std lib
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*实现目标:
 *可兼容1,2,4,8字节对齐的拷贝式环形队列
 *该实现参考于zephyr\lib\os
 */

typedef struct ring_buffer_body {
    /* 类型缓冲区: */
    union {
        void     *buffer;   /* 抽象缓冲区 */
        uint8_t  *buffer1;  /* 缓冲区(1字节对齐) */
        uint16_t *buffer2;  /* 缓冲区(2字节对齐) */
        uint32_t *buffer4;  /* 缓冲区(4字节对齐) */
        uint64_t *buffer8;  /* 缓冲区(8字节对齐) */
    };
    uint8_t  type;      /* 字节对齐类型(1,2,4,8) */
    uint32_t size;      /* 最大数据条目 */
    uint32_t mask;      /* 最大条目掩码 */
    uint32_t head;      /* 队列起始位置(进动点,非实际下标) */
    uint32_t tail;      /* 队列结束位置(进动点,非实际下标) */
} ring_buffer;

/* 环形缓冲区所使用到的算法 */
#define RING_BUFFER_MAX_LEN     0x80000000

//环形队列为空判断
static inline bool ring_buffer_is_empty(ring_buffer *instance)
{
    return (instance->head == instance->tail) ? true : false;
}

//重置环形队列
static inline void ring_buffer_reset(ring_buffer *instance)
{
    instance->head = 0;
    instance->tail = 0;
}

//获取环形队列已有条目
static inline uint32_t ring_buffer_get_item(ring_buffer *instance)
{
    return instance->tail - instance->head;
}

//获取环形队列空闲条目
static inline uint32_t ring_buffer_get_space(ring_buffer *instance)
{
    return instance->size - ring_buffer_get_item(instance);
}

/*@brief        初始化环形队列(无参数检查)
 *@param[in]    instance    实例
 *@param[in]    buffer      指定的缓冲区,为对齐的字流(不是字节流)(如下)
 *@param[in]    type        数据单元类型:(1,2,4,8)(字节对齐)
 *@param[in]    size        对齐字流的长度
 */
static inline void ring_buffer_init(ring_buffer *instance, void *buffer, uint8_t type, uint32_t size)
{
    /* 简要的字节对齐修正 */
    /* 地址截断,通过最后几位确定是否字节对齐 */
    uint8_t offset = (uint64_t)(uint8_t *)buffer;
    
    if (offset % type != 0) {
        buffer = (((uint8_t *)buffer) + type - offset);
        size--;
    }

    instance->head = 0;
    instance->tail = 0;
    instance->type = type;
	instance->size = size;
	instance->buffer = buffer;
    /* 缓冲区大小2字节对齐加速 */
	instance->mask = ((size != 0) && ((size & (size - 1)) == 0)) ? (size - 1) : 0;
}

/*@brief        从环形队列获取数据(无参数检查)
 *@param[in]    instance    实例
 *@param[out]   data        指定对齐类型数据的存储地址(非对齐可能会导致截断的情况)
 *@param[in]    length      所需获取数据长度
 *@retval       -1          数据不足
 *@retval       -2          实例类型错误
 */
int32_t ring_buffer_gets(ring_buffer *instance, void *data, uint32_t length);

/*@brief        向环形队列推送数据(无参数检查)
 *@param[in]    instance    实例
 *@param[out]   data        指定对齐类型数据的存储地址(非对齐可能会导致截断的情况)
 *@param[in]    length      所需推送数据长度
 *@retval       -1:         空间不足
 */
int32_t ring_buffer_puts(ring_buffer *instance, void *data, uint32_t length);

#endif
