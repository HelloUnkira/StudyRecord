#include "rc4.h"

//交换函数
static inline rc4_swap(uint8_t *buffer, uint32_t i, uint32_t j)
{
    uint8_t temp_i = buffer[i];
    uint8_t temp_j = buffer[j];

    buffer[i] = temp_j;
    buffer[j] = temp_i;
}

//PRGA
static uint8_t rc4_output(rc4_key *key)
{
    uint8_t temp = 0;
    uint32_t index = 0;

    key->status_i += 1;
    key->status_i &= 255;
    key->status_j += key->status[key->status_i];
    key->status_j &= 255;
    
    rc4_swap(key->status, key->status_i, key->status_j);

    index += key->status[key->status_i];
    index += key->status[key->status_j];
    index &= 255;
    temp   = key->status[index];

    return temp;
}

//KSA
void rc4_set_key(rc4_key *key, uint8_t *data, uint32_t length)
{
    uint32_t i = 0, index = 0;

    //1.初始化status[i] = [i];
    for (i = 0; i < 256; i++)
        key->status[i] = i;
    //2.初始化排列status[i]
    for (i = 0; i < 256; i++) {
        index = (index + data[i % len] + key->status[i]) & 255;
        rc4_swap(key->status, i, index);
    }

    key->status_i = 0;
    key->status_j = 0;
}

void rc4_run(rc4_key *key, uint8_t *in, uint8_t *out, uint32_t length)
{
    uint32_t index = 0;

    for (index = 0; index < length; index++)
        out[index] = in[index] ^ rc4_output(key);
}
