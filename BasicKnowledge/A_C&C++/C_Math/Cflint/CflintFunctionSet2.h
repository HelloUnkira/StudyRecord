#ifndef CFLINT_FUNCTION_SET2_H
#define CFLINT_FUNCTION_SET2_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_FUNCTION_SET2
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 和运算:Operand += Overflow */
CFLINT_TYPE Cflint_AdditionBit(CFLINT_TYPE *Operand, uint32_t Length,
                               CFLINT_TYPE  Overflow);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 差运算:Operand -= Overflow */
CFLINT_TYPE Cflint_SubtractionBit(CFLINT_TYPE *Operand, uint32_t Length,
                                  CFLINT_TYPE  Overflow);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 翻转数:模对称翻转数 */
CFLINT_TYPE Cflint_Reserve(CFLINT_TYPE *Operand, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 和运算:Result = Operand1 + Operand2 */
CFLINT_TYPE Cflint_Addition(CFLINT_TYPE *Result, CFLINT_TYPE *Operand1, 
                            CFLINT_TYPE *Operand2,  uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 差运算:Result = Operand1 - Operand2 */
CFLINT_TYPE Cflint_Subtraction(CFLINT_TYPE *Result, CFLINT_TYPE *Operand1,
                               CFLINT_TYPE *Operand2,  uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 乘运算:Result = Operand1 * Operand2; 结果的长度实际是Length * 2 */
void Cflint_Multiply(CFLINT_TYPE *Result, CFLINT_TYPE *Operand1,
                     CFLINT_TYPE *Operand2,  uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 方运算:Result = Operand * Operand; 结果的长度实际是Length * 2 */
void Cflint_Square(CFLINT_TYPE *Result, CFLINT_TYPE *Operand, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 带余除运算:Quotient = Operand0 / Operand1; Module = Operand0 % Operand1 */
void Cflint_Devide(CFLINT_TYPE *Quotient, CFLINT_TYPE *Module,
                   CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                      uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 基础模运算:Module = Operand0 % Operand1 */
void Cflint_Modulo(CFLINT_TYPE *Module, CFLINT_TYPE *Operand0,
                   CFLINT_TYPE *Operand1,  uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模和运算:Result = (Operand0 + Operand1) % Module */
void Cflint_ModuloAddition(CFLINT_TYPE *Result,   CFLINT_TYPE *Module,
                           CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                           CFLINT_TYPE *Temp[2],     uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模差运算:Result = (Operand0 - Operand1) % Module */
void Cflint_ModuloSubtraction(CFLINT_TYPE *Result,   CFLINT_TYPE *Module,
                              CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                              CFLINT_TYPE *Temp[2],     uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模乘运算:Result = (Operand0 * Operand1) % Module */
/* 要求:Temp[0~1]的长度为Length*2 */
void Cflint_ModuloMultiply(CFLINT_TYPE *Result,   CFLINT_TYPE *Module,
                           CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                           CFLINT_TYPE *Temp[2],     uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模方运算:Result = (Operand * Operand) % Module */
/* 要求:Temp[0~1]的长度为Length*2 */
void Cflint_ModuloSquare(CFLINT_TYPE *Result,  CFLINT_TYPE *Module,
                         CFLINT_TYPE *Operand, CFLINT_TYPE *Temp[2],
                            uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模幂运算:Result = (Operand**Exponent) % Module */
/* 要求:Temp[0~4]的长度为Length*2 */
void Cflint_ModuloExponent(CFLINT_TYPE *Result,  CFLINT_TYPE *Module,
                           CFLINT_TYPE *Operand, CFLINT_TYPE *Exponent,
                           CFLINT_TYPE *Temp[4],    uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模逆运算: Result = (1 / Operand) % Module */
/* 要求:Operand < Module且互素 */
void Cflint_ModuloInverse(CFLINT_TYPE *Result, CFLINT_TYPE *Operand,
                          CFLINT_TYPE *Module, CFLINT_TYPE *Temp[4],
                             uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 带符号与运算:Result * (*ResultFlag) = Operand1 * (Operand1_Flag) + */
/*                                       Operand2 * (Operand2_Flag)   */
/* 备注:大数运算本身是无符号语义运算,这里是为某些数学运算额外扩展 */
CFLINT_TYPE Cflint_FlagSum(CFLINT_TYPE *Result,   CFLINT_TYPE *ResultFlag,
                           CFLINT_TYPE *Operand1, CFLINT_TYPE  Operand1_Flag,
                           CFLINT_TYPE *Operand2, CFLINT_TYPE  Operand2_Flag,
                              uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 带符号模运算:Result = Operand1 * (Operand1_Flag) % Operand2 */
/* 备注:大数运算本身是无符号语义运算,这里是为某些数学运算额外扩展 */
void Cflint_FlagModulo(CFLINT_TYPE *Result,   CFLINT_TYPE *Operand1,
                       CFLINT_TYPE *Operand2, CFLINT_TYPE  Operand1_Flag,
                          uint32_t  Length);
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
