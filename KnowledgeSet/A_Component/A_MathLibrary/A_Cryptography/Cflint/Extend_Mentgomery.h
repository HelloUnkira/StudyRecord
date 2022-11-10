#ifndef EXTEND_MENTGOMERY_H
#define EXTEND_MENTGOMERY_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef EXTEND_USE_MENTGOMERY
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 蒙哥马利约减(X,R,N,NP) */
/* X,Temp1,Temp2长度为Length*2 */
void Mentgomery_ModuloReduction(Cflint_Type *Result, uint32_t Length, int64_t R,
                                Cflint_Type *X, Cflint_Type *N, Cflint_Type *NP,
                                Cflint_Type *Temp1, Cflint_Type *Temp2);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 蒙哥马利模乘(X,Y,N,NP,R_N) */
/* Temp1,Temp2,Temp3,Temp4长度为Length*2 */
void Mentgomery_ModuloMultiply(Cflint_Type *Result, Cflint_Type *X, Cflint_Type *Y,
                               Cflint_Type *N, Cflint_Type *NP, Cflint_Type *R_N,
                               Cflint_Type *Temp1, Cflint_Type *Temp2,
                               Cflint_Type *Temp3, Cflint_Type *Temp4,
                               int64_t R, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
