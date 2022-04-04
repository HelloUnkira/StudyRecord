#ifndef CFLINT_H
#define CFLINT_H
//C std lib
//遵守C99标准
#include <stdint.h>
#include <stdbool.h>

/*Cflint(Abbr):
 *C Functions For Large Integers In Number Theory And Cryptography
 *大数理论与密码学的C函数集
 */

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 平台相关的部分配置 */
#include "CflintConfigure.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 数据类型相关的部分 */
#include "CflintData.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 基本运算相关接口 */
#include "CflintBase.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 扩展运算相关接口 */
#include "CflintExtend.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
