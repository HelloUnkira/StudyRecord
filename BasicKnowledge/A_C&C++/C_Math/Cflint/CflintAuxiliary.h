#ifndef CFLINTAUXILIARY_H
#define CFLINTAUXILIARY_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 拷贝:Operand0 = Operand1 */
static inline void Cflint_Copy(CFLINT_TYPE *Operand0, CFLINT_TYPE *Operand1,
                               uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Operand0[Index] = Operand1[Index];
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 比较:((>:1);(==:0);(<:-1)) */
static inline int8_t Cflint_Compare(CFLINT_TYPE *Operand0,
                                    CFLINT_TYPE *Operand1, uint32_t Length)
{
    for (int64_t Index = Length; Index >= 0; Index--)
        if (Operand0[Index] != Operand1[Index]) {
            if (Operand0[Index] > Operand1[Index])
                return 1;
            if (Operand0[Index] < Operand1[Index])
                return -1;
        }
    return 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 相等判断:((==:1);(!=:0)) */
static inline bool Cflint_Equal(CFLINT_TYPE *Operand0,
                                CFLINT_TYPE *Operand1, uint32_t Length)
{
    CFLINT_TYPE Result = 0;
    for (uint32_t Index = 0; Index < Length; Index++)
        Result |= Operand0[Index] ^ Operand1[Index];
    return (Result == 0 ? true : false);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 为0判断:Operand == 0 */
static inline bool Cflint_IsZero(CFLINT_TYPE *Operand, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        if (Operand[Index] != 0)
            return false;
    return true;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 设值:类似memset */
static inline void Cflint_SetValue(CFLINT_TYPE *Operand, uint32_t Length,
                                   CFLINT_TYPE  Value)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        Operand[Index] = Value;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
