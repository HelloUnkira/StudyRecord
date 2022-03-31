#include "Cflint.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 辅助功能:测试阶段所用于确定的大小端序检查 */
uint8_t Cflint_CheckEndian(void)
{
    union {
        uint8_t  Byte;
        uint32_t Integer;
    } Endian;
    
    Endian.Integer = 1;
    
    return (Endian.Byte == 1 ? 1 : 0);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/






