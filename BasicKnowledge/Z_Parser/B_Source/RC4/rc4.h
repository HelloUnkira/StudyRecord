#ifndef RC4_H
#define RC4_H
//C std lib
#include <stdint.h>

typedef struct rc4_key_body {
    //×´Ì¬±äÁ¿
    uint8_t  status[256];
    uint32_t status_i;
    uint32_t status_j;
} rc4_key;

void rc4_set_key(rc4_key *key, uint8_t *data, uint32_t length);
void rc4_run(rc4_key *key, uint8_t *in, uint8_t *out, uint32_t length);

#endif
