#include "../ring_buffer.h"

int main(int argc, char *argv[])
{
    static ring_buffer instance1;
    static ring_buffer instance2;
    static ring_buffer instance4;
    static ring_buffer instance8;
    
    static uint8_t  buffer1[10] = {0}, temp1 = 0;
    static uint16_t buffer2[10] = {0}, temp2 = 0;
    static uint32_t buffer4[10] = {0}, temp4 = 0;
    static uint64_t buffer8[10] = {0}, temp8 = 0;
    
    ring_buffer_init(&instance1, buffer1, 1, 10);
    ring_buffer_init(&instance2, buffer2, 2, 10);
    ring_buffer_init(&instance4, buffer4, 4, 10);
    ring_buffer_init(&instance8, buffer8, 8, 10);
    
    int i = 0;
    
    for (i = 0; i < 10; i++) {
        
        temp1 = (~0) - i;
        temp2 = (~0) - i;
        temp4 = (~0) - i;
        temp8 = (~0) - i;
        
        ring_buffer_puts(&instance1, &temp1, 1);
        ring_buffer_puts(&instance2, &temp2, 1);
        ring_buffer_puts(&instance4, &temp4, 1);
        ring_buffer_puts(&instance8, &temp8, 1);
        
    }
    
    for (i = 0; i < 10; i++) {
        
        ring_buffer_gets(&instance1, &temp1, 1);
        ring_buffer_gets(&instance2, &temp2, 1);
        ring_buffer_gets(&instance4, &temp4, 1);
        ring_buffer_gets(&instance8, &temp8, 1);
        
        printf("temp1:%d\ttemp2:%d\ttemp4:%d\ttemp8:%lld\n",
                temp1,temp2,temp4,temp8);
    }
    
    
    
    
	return 0;
}
