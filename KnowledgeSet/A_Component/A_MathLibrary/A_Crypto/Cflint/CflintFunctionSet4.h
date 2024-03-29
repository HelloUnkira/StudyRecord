#ifndef CFLINT_FUNCTION_SET4_H
#define CFLINT_FUNCTION_SET4_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#ifdef CFLINT_USE_FUNCTION_SET4
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 2次方根整数部运算:[Sqrt(Operand)] */
void Cflint_Root2Integer(Cflint_Type *Result, Cflint_Type *Operand,
                         Cflint_Type *Temp[4],   uint32_t  Length);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 2次方数判别检查:Sqrt(Operand)**2 == Operand */
/* 要求Temp[0]的长度为Length*2 */
bool Cflint_Root2Check(Cflint_Type *Result, Cflint_Type *Operand,
                       Cflint_Type *Temp[4],   uint32_t  Length);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Jacobi符号计算:Jacobi(Operand1/Operand2) */
int8_t Cflint_JacobiFlag(Cflint_Type *Operand1, Cflint_Type *Operand2,
                         Cflint_Type *Temp[3],     uint32_t  Length);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 二次剩余计算:((Result**2) % Operand2 == Operand1 % Operand2) */
/* 模为Operand2 > 2的奇素数, 要求Temp[6~9]为Length * 2 */
bool Cflint_ModuloP1Root2(Cflint_Type *Operand1,  Cflint_Type *Operand2,
                          Cflint_Type *Result,    Cflint_Type *Temp[10],
                             uint32_t  Length);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 扩展二次剩余计算:(X(K)**2) % P == A % (P**K) */
bool Cflint_ModuloPkRoot2(Cflint_Type *Operand1,  Cflint_Type *Operand2,
                          Cflint_Type *Result,    Cflint_Type *Temp[10],
                              int64_t  Exponent,     uint32_t  Length);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 二次剩余计算:((Result**2) % (Operand2 * Operand3) ==  */
/*               (Operand1)  % (Operand2 * Operand3))    */
/* 这里是求最小平方根,意外返回:当Result == 0时不满足二次剩余 */
/* 模为Operand2,Operand3 > 2的奇素数, Operand1, Result为Length * 2 */
/* 要求Temp[3~10] 为(Length+1)*2 */
/* 要求Temp[11~12]为Length * 4 */
bool Cflint_Modulo1Root2(Cflint_Type *Operand1,  Cflint_Type *Operand2,
                         Cflint_Type *Operand3,  Cflint_Type *Result,
                         Cflint_Type *Temp[13],     uint32_t  Length);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 线性同余方程组计算:X == Ai % Mi,当i != j时, GCD(Mi, Mj) == 1 */
/* Result,Temps[0~5]为LengthMax,返回值-2表示LengthMax过小 */
/* 要求Temp[3~4]为(Length+1)*2 */
/* 要求Temp[5~9]为(Length+1)*4 */
int8_t Cflint_LCE(Cflint_Type **Operands, Cflint_Type *Result,   uint64_t Number,
                  Cflint_Type  *Temps[5], Cflint_Type *Temp[10], uint32_t Length,
                     uint32_t   LengthMax);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
