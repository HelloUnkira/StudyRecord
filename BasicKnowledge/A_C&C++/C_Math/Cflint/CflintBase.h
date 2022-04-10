#ifndef CFLINTBASE_H
#define CFLINTBASE_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_BASE
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
/* 左移位运算:Operand <<= Bits2(2进制位) */
void Cflint_ShiftLeft2(CFLINT_TYPE *Operand, uint32_t Length, uint64_t Bits2);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 右移位运算:Operand >>= Bits2(2进制位) */
void Cflint_ShiftRight2(CFLINT_TYPE *Operand, uint32_t Length, uint64_t Bits2);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 左移位运算:Operand <<= BitsN(N进制位) */
void Cflint_ShiftLeftN(CFLINT_TYPE *Operand, uint32_t Length, uint32_t BitsN);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 右移位运算:Operand >>= BitsN(N进制位) */
void Cflint_ShiftRightN(CFLINT_TYPE *Operand, uint32_t Length, uint32_t BitsN);
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
/* 基础模运算2:Operand %= (2**K == (1 << Bits2)) */
void Cflint_Modulo2(CFLINT_TYPE *Operand, uint32_t Length, int64_t Bits2);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模(乘,方)运算:省略,先(乘,方)后模 */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模加运算:Result = (Operand0 + Operand1) % Module */
/* 要求:Operand0 < Module && Operand1 < Module */
void Cflint_ModuloAddition(CFLINT_TYPE *Result,   CFLINT_TYPE *Module,
                           CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                              uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模减运算:Result = (Operand0 - Operand1) % Module */
/* 要求:Operand0 < Module && Operand1 < Module */
void Cflint_ModuloSubtraction(CFLINT_TYPE *Result,   CFLINT_TYPE *Module,
                              CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                                 uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模幂运算:Result = (Operand**Exponent) % Module */
/* 要求:Temp1,Temp2,Temp3,Temp4的长度为Length*2 */
void Cflint_ModuloExponent(CFLINT_TYPE *Result,  CFLINT_TYPE *Module,
                           CFLINT_TYPE *Operand, CFLINT_TYPE *Exponent,
                           CFLINT_TYPE *Temp1,   CFLINT_TYPE *Temp2,
                           CFLINT_TYPE *Temp3,   CFLINT_TYPE *Temp4,
                              uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模逆运算: Result = (1 / Operand) % Module */
/* 要求:Operand < Module且互素 */
void Cflint_ModuloInverse(CFLINT_TYPE *Result,  CFLINT_TYPE *Module,
                          CFLINT_TYPE *Operand, CFLINT_TYPE *Temp1,
                          CFLINT_TYPE *Temp2,   CFLINT_TYPE *Temp3,
                          CFLINT_TYPE *Temp4,      uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
