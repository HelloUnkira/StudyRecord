#ifndef CFLINT_H
#define CFLINT_H
//C std lib
//遵守C99标准
#include <stdint.h>
#include <stdbool.h>

/*Cfint(Abbr):
 *C Functions For Large Integers In Number Theory And Cryptography
 *大数理论与密码学的C函数集
 */

/* 大小端序配置项 */
#define CFlint_USE_ENDIAN_BIG       1
#define CFlint_USE_ENDIAN_LITTLE    1
/* 平台大小端序检查 */
uint8_t Cflint_CheckEndian(void);



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define CFLINT_USE_AES128
/* AES128加(Flag:0)解(Flag:1)密 */
void AES128_Run(uint8_t *Text, uint32_t Length, uint8_t Key[16], uint8_t Flag);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


#endif
