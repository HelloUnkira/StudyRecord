#ifndef RC4_H
#define RC4_H
//C std lib
#include <stdint.h>
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
typedef struct RC4KeyBody {
    //状态矩阵
    uint8_t  Status[256];
    uint32_t StatusI;
    uint32_t StatusJ;
} RC4Key;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void RC4Set(RC4Key *Key, uint8_t *Data,  uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void RC4Run(RC4Key *Key, uint8_t *Input, uint8_t *Output, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
