#ifndef CFLINTRESIDUECLASS_H
#define CFLINTRESIDUECLASS_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_RESIDUECLASS
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 蒙哥马利模加:Result = (Operand0 + Operand1) % Module */
/* 要求:Operand0 < Module && Operand1 < Module */
void Cflint_ModuloAddition(CFLINT_TYPE *Result,   CFLINT_TYPE *Module,
                           CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                              uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 蒙哥马利模减:Result = (Operand0 - Operand1) % Module */
/* 要求:Operand0 < Module && Operand1 < Module */
void Cflint_ModuloSubtraction(CFLINT_TYPE *Result,   CFLINT_TYPE *Module,
                              CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                                 uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模乘运算:省略,先乘后模 */
/* 模方运算:省略,先方后模 */
/* 模幂运算:Result = Operand**Exponent % Module */
/* 要求:Temp1,Temp2,Temp3,Temp4的长度为Length*2 */
void Cflint_ModuloExp(CFLINT_TYPE *Result,  CFLINT_TYPE *Module,
                      CFLINT_TYPE *Operand, CFLINT_TYPE *Exponent,
                      CFLINT_TYPE *Temp1,   CFLINT_TYPE *Temp2,
                      CFLINT_TYPE *Temp3,   CFLINT_TYPE *Temp4,
                         uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 模逆运算: Result = (1 / Operand) % Module */
void Cflint_ModuloInv(CFLINT_TYPE *Result,  CFLINT_TYPE *Module,
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
