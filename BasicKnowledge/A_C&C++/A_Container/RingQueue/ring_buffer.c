//内源依赖项
#include "ring_buffer.h"

//索引求模运算
static void ring_buffer_modulo(ring_buffer *instance, uint32_t value)
{
    return (instance->mask != 0) ? (value & instance->mask) : (value % instance->size);
}

//索引回退
static void ring_buffer_rewind_index(ring_buffer *instance)
{
    if (instance->head >= RING_BUFFER_MAX_LEN) {
        
        uint32_t rewind = RING_BUFFER_MAX_LEN;
        rewind -= instance->size;
        rewind *= instance->size;
        
        instance->head -= rewind;
        instance->tail -= rewind;
    }
}


/*@brief        从环形队列获取数据(无参数检查)
 *@param[in]    instance    实例
 *@param[out]   data        指定对齐类型数据的存储地址(非对齐可能会导致截断的情况)
 *@param[in]    length      所需获取数据长度
 *@retval       -1          数据不足
 *@retval       -2          实例类型错误
 */
int32_t ring_buffer_gets(ring_buffer *instance, void *data, uint32_t length)
{
    uint32_t index = 0, index1 = 0;
    uint8_t  *buffer1 = data;
    uint16_t *buffer2 = data;
    uint32_t *buffer4 = data;
    uint64_t *buffer8 = data;
    
    if (ring_buffer_is_empty(instance))
        return -1;
    if (ring_buffer_get_item(instance) < length)
        return -1;

    switch (instance->type) {
    case 1:
        if (instance->mask != 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->head) & instance->mask;
                buffer1[index] = instance->buffer1[index1];
            }
        if (instance->mask == 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->head) % instance->size;
                buffer1[index] = instance->buffer1[index1];
            }
        break;
    case 2:
        if (instance->mask != 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->head) & instance->mask;
                buffer2[index] = instance->buffer2[index1];
            }
        if (instance->mask == 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->head) % instance->size;
                buffer2[index] = instance->buffer2[index1];
            }
        break;
    case 4:
        if (instance->mask != 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->head) & instance->mask;
                buffer4[index] = instance->buffer4[index1];
            }
        if (instance->mask == 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->head) % instance->size;
                buffer4[index] = instance->buffer4[index1];
            }
        break;
    case 8:
        if (instance->mask != 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->head) & instance->mask;
                buffer8[index] = instance->buffer8[index1];
            }
        if (instance->mask == 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->head) % instance->size;
                buffer8[index] = instance->buffer8[index1];
            }
        break;
    default:
        return -2;
    }

    instance->head += length;
    
    //索引回退
    ring_buffer_rewind_index(instance);
    
    return 0;
}

/*@brief        向环形队列推送数据(无参数检查)
 *@param[in]    instance    实例
 *@param[out]   data        指定对齐类型数据的存储地址(非对齐可能会导致截断的情况)
 *@param[in]    length      所需推送数据长度
 *@retval       -1:         空间不足
 */
int32_t ring_buffer_puts(ring_buffer *instance, void *data, uint32_t length)
{
    uint32_t index = 0, index1 = 0;
    uint8_t  *buffer1 = data;
    uint16_t *buffer2 = data;
    uint32_t *buffer4 = data;
    uint64_t *buffer8 = data;
    
    if (ring_buffer_get_space(instance) < length)
        return -1;

    switch (instance->type) {
    case 1:
        if (instance->mask != 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->tail) & instance->mask;
                instance->buffer1[index1] = buffer1[index];
            }
        if (instance->mask == 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->tail) % instance->size;
                instance->buffer1[index1] = buffer1[index];
            }
        break;
    case 2:
        if (instance->mask != 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->tail) & instance->mask;
                instance->buffer2[index1] = buffer2[index];
            }
        if (instance->mask == 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->tail) % instance->size;
                instance->buffer2[index1] = buffer2[index];
            }
        break;
    case 4:
        if (instance->mask != 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->tail) & instance->mask;
                instance->buffer4[index1] = buffer4[index];
            }
        if (instance->mask == 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->tail) % instance->size;
                instance->buffer4[index1] = buffer4[index];
            }
        break;
    case 8:
        if (instance->mask != 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->tail) & instance->mask;
                instance->buffer8[index1] = buffer8[index];
            }
        if (instance->mask == 0)
            for (index = 0; index < length; index++) {
                index1 = (index + instance->tail) % instance->size;
                instance->buffer8[index1] = buffer8[index];
            }
        break;
    default:
        return -2;
    }

    instance->tail += length;
    
    return 0;
}


