#ifndef CFLINT_FUNCTION_SET5_H
#define CFLINT_FUNCTION_SET5_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_FUNCTION_SET5
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 孪生素数判别检查 */
bool Cflint_IsPrime1(CFLINT_TYPE *X, CFLINT_TYPE *Temp[3], uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 素数生成器:不实现,给出流程
 * uint64_t B = (1 + [sqrt[N]]) >> 1;
 * uint64_t L = (N >> 1);
 *          L = (L - ((N > 0) && ((N & 1) == 0)) ? 1 : 0);
 * bool F[L + 1] = {1};
 * uint64_t P = 3;
 * uint64_t S = 4;
 * for (uint64_t I = 1; I <= B; I++) {
 *   if (F[I] == true)
 *       for (uint64_t K = S; K <= L; F[K] = 0, K += P);
 *   S += P + P + 2;
 *   P += 2;
 * }
 * 现在if (F[I] == 1)的(I * 2 + 1)就是一个素数,素数2额外计算
 */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
