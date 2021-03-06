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
void Mentgomery_ModuloReduction(CFLINT_TYPE *Result, uint32_t Length, int64_t R,
                                CFLINT_TYPE *X, CFLINT_TYPE *N, CFLINT_TYPE *NP,
                                CFLINT_TYPE *Temp1, CFLINT_TYPE *Temp2);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 蒙哥马利模乘(X,Y,N,NP,R_N) */
/* Temp1,Temp2,Temp3,Temp4长度为Length*2 */
void Mentgomery_ModuloMultiply(CFLINT_TYPE *Result, CFLINT_TYPE *X, CFLINT_TYPE *Y,
                               CFLINT_TYPE *N, CFLINT_TYPE *NP, CFLINT_TYPE *R_N,
                               CFLINT_TYPE *Temp1, CFLINT_TYPE *Temp2,
                               CFLINT_TYPE *Temp3, CFLINT_TYPE *Temp4,
                               int64_t R, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
