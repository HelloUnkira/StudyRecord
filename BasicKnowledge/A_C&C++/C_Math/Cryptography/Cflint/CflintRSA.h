#ifndef CFLINT_RSA_H
#define CFLINT_RSA_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_RSA
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 备注:这里并没有提供完整的方案,只是表现一些流程 */
/*RSA流程:
 *1.根据找寻素数对(P, Q),使用大随机奇素数接口
 *2.计算公共模数:N = P * Q
 *3.根据欧拉公式计算欧拉数:R = (P - 1) * (Q - 1)
 *4.找寻一个小于R的整数:E满足E与R互质
 *5.计算乘法逆:D = (1 / E)(Mod R)
 *6.销毁P和Q,生成公钥(N, E), 生成私钥(N, D)
 *RSA加解密:
 *密文 = (明文**E) % N
 *明文 = (密文**D) % N
 */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 通过目标素数对(P, Q)构造公钥(N, E)和私钥(N, D) */
/* 要求Temp[0~7]为Length*2 */
/* 要求Temp[7~9]为Length*4 */
void Cflint_RSA_Make(CFLINT_TYPE *P, CFLINT_TYPE *Q, CFLINT_TYPE *N,
                     CFLINT_TYPE *E, CFLINT_TYPE *D, CFLINT_TYPE *R,
                     CFLINT_TYPE *Temp[9], uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void Cflint_RSA_Encrypt(CFLINT_TYPE *N, CFLINT_TYPE *E, CFLINT_TYPE *I,
                        CFLINT_TYPE *O, CFLINT_TYPE *Temp[4], uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void Cflint_RSA_Decrypt(CFLINT_TYPE *N, CFLINT_TYPE *D, CFLINT_TYPE *I,
                        CFLINT_TYPE *O, CFLINT_TYPE *Temp[4], uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif