#ifndef CFLINT_FUNCTION_SET1_H
#define CFLINT_FUNCTION_SET1_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef CFLINT_USE_FUNCTION_SET1
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 交换:Operand0 <=> Operand1 */
void Cflint_Swap(CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 拷贝:Operand0 = Operand1 */
void Cflint_Copy(CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 比较:((>:1);(==:0);(<:-1)) */
int8_t Cflint_Compare(CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 相等判断:((==:1);(!=:0)) */
bool Cflint_Equal(CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 为0判断:Operand == 0 */
bool Cflint_IsZero(CFLINT_TYPE *Operand, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 设值:类似memset */
void Cflint_SetValue(CFLINT_TYPE *Operand, uint32_t Length, CFLINT_TYPE Value);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 检查一个数是否为2的幂次方 */
bool Cflint_IsExponent2(CFLINT_TYPE *Operand, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 计算2进制最高位索引 */
int64_t Cflint_Numbers2(CFLINT_TYPE *Operand, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 位或运算 */
void Cflint_OR(CFLINT_TYPE *Result, CFLINT_TYPE *Operand1, 
               CFLINT_TYPE *Operand2,  uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 位与运算 */
void Cflint_AND(CFLINT_TYPE *Result, CFLINT_TYPE *Operand1, 
                CFLINT_TYPE *Operand2,  uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 位异或运算 */
void Cflint_XOR(CFLINT_TYPE *Result, CFLINT_TYPE *Operand1, 
                CFLINT_TYPE *Operand2,  uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 位反运算 */
void Cflint_NOT(CFLINT_TYPE *Operand, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 位检查 */
bool Cflint_CheckBit2(CFLINT_TYPE *Operand, uint32_t Length, int64_t Bits2);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 位设置 */
void Cflint_SetBit2(CFLINT_TYPE *Operand, uint32_t Length, int64_t Bits2);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 位清除 */
void Cflint_ClearBit2(CFLINT_TYPE *Operand, uint32_t Length, int64_t Bits2);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 偶数判断 */
bool Cflint_IsEven(CFLINT_TYPE *Operand, uint32_t Length);
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
/* 数分解运算Operand1==(2**Result)*Operand2 */
int64_t Cflint_Factor2(CFLINT_TYPE *Operand1, CFLINT_TYPE *Operand2,
                          uint32_t  Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 掩码运算:Operand &= ((2**K - 1) == ((1 << Bits2) - 1)) */
void Cflint_Mask2(CFLINT_TYPE *Operand, uint32_t Length, int64_t Bits2);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 数据转Native格式流 */
void Cflint_Native1ToBytes(uint8_t *Bytes, uint8_t  *Native, uint32_t Length);
void Cflint_BytesToNative1(uint8_t *Bytes, uint8_t  *Native, uint32_t Length);
void Cflint_Native2ToBytes(uint8_t *Bytes, uint16_t *Native, uint32_t Length);
void Cflint_BytesToNative2(uint8_t *Bytes, uint16_t *Native, uint32_t Length);
void Cflint_Native4ToBytes(uint8_t *Bytes, uint32_t *Native, uint32_t Length);
void Cflint_BytesToNative4(uint8_t *Bytes, uint32_t *Native, uint32_t Length);
void Cflint_Native8ToBytes(uint8_t *Bytes, uint64_t *Native, uint32_t Length);
void Cflint_BytesToNative8(uint8_t *Bytes, uint64_t *Native, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif