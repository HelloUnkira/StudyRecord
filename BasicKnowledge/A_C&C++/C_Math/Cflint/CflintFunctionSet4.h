#ifndef CFLINT_FUNCTION_SET4_H
#define CFLINT_FUNCTION_SET4_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_FUNCTION_SET4
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 2次方根整数部运算: */
void Cflint_Root2Integer(CFLINT_TYPE *Result, CFLINT_TYPE *Operand,
                         CFLINT_TYPE *Temp1,  CFLINT_TYPE *Temp2,
                         CFLINT_TYPE *Temp3,  CFLINT_TYPE *Temp4, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 2次方数判别检查 */
/* 要求Temp1的长度为Length*2 */
bool Cflint_Root2Check(CFLINT_TYPE *Result, CFLINT_TYPE *Operand,
                       CFLINT_TYPE *Temp1,  CFLINT_TYPE *Temp2,
                       CFLINT_TYPE *Temp3,  CFLINT_TYPE *Temp4, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 符号Jacobi(Operand1/Operand2)计算 */
int8_t Cflint_JacobiFlag(CFLINT_TYPE *Operand1, CFLINT_TYPE *Operand2,
                         CFLINT_TYPE *Temp1,    CFLINT_TYPE *Temp2,
                         CFLINT_TYPE *Temp3,       uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif