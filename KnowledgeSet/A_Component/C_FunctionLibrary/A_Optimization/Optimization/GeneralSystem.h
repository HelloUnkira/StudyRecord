#ifndef GENERAL_SYSTEM_H
#define GENERAL_SYSTEM_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define MemRd(Addr, Type)       ((*(volatile Type *)(Addr)))            /* Memory Read */
#define MemWt(Addr, Data, Type) ((*(volatile Type *)(Addr)) = (Data))   /* Memory Write */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline    float MemRd_F32(void *Addr) {return MemRd(Addr,    float);}
static inline   double MemRd_F64(void *Addr) {return MemRd(Addr,   double);}
static inline   int8_t MemRd_I8(void  *Addr) {return MemRd(Addr,   int8_t);}
static inline  int16_t MemRd_I16(void *Addr) {return MemRd(Addr,  int16_t);}
static inline  int32_t MemRd_I32(void *Addr) {return MemRd(Addr,  int32_t);}
static inline  int64_t MemRd_I64(void *Addr) {return MemRd(Addr,  int64_t);}
static inline  uint8_t MemRd_U8(void  *Addr) {return MemRd(Addr,  uint8_t);}
static inline uint16_t MemRd_U16(void *Addr) {return MemRd(Addr, uint16_t);}
static inline uint32_t MemRd_U32(void *Addr) {return MemRd(Addr, uint32_t);}
static inline uint64_t MemRd_U64(void *Addr) {return MemRd(Addr, uint64_t);}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void MemWt_F32(void *Addr,    float Data) {MemWt(Addr, Data,    float);}
static inline void MemWt_F64(void *Addr,   double Data) {MemWt(Addr, Data,   double);}
static inline void MemWt_I8( void *Addr,   int8_t Data) {MemWt(Addr, Data,   int8_t);}
static inline void MemWt_I16(void *Addr,  int16_t Data) {MemWt(Addr, Data,  int16_t);}
static inline void MemWt_I32(void *Addr,  int32_t Data) {MemWt(Addr, Data,  int32_t);}
static inline void MemWt_I64(void *Addr,  int64_t Data) {MemWt(Addr, Data,  int64_t);}
static inline void MemWt_U8( void *Addr,  uint8_t Data) {MemWt(Addr, Data,  uint8_t);}
static inline void MemWt_U16(void *Addr, uint16_t Data) {MemWt(Addr, Data, uint16_t);}
static inline void MemWt_U32(void *Addr, uint32_t Data) {MemWt(Addr, Data, uint32_t);}
static inline void MemWt_U64(void *Addr, uint64_t Data) {MemWt(Addr, Data, uint64_t);}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void MemRev_B1(void *Addr)
{
}
static inline void MemRev_B2(void *Addr)
{
    MemRev_B1((void *)((uint8_t *)Addr + 0));
    MemRev_B1((void *)((uint8_t *)Addr + 1));
    
    MemWt_U16(Addr, ((uint16_t)MemRd_U8((void *)((uint8_t *)Addr + 0)) << 8) |
                    ((uint16_t)MemRd_U8((void *)((uint8_t *)Addr + 1)) >> 8));
}
static inline void MemRev_B4(void *Addr)
{
    MemRev_B2((void *)((uint16_t *)Addr + 0));
    MemRev_B2((void *)((uint16_t *)Addr + 1));
    
    MemWt_U32(Addr, ((uint32_t)MemRd_U16((void *)((uint16_t *)Addr + 0)) << 16) |
                    ((uint32_t)MemRd_U16((void *)((uint16_t *)Addr + 1)) >> 16));
}
static inline void MemRev_B8(void *Addr)
{
    MemRev_B4((void *)((uint32_t *)Addr + 0));
    MemRev_B4((void *)((uint32_t *)Addr + 1));
    
    MemWt_U64(Addr, ((uint64_t)MemRd_U32((void *)((uint32_t *)Addr + 0)) << 32) |
                    ((uint64_t)MemRd_U32((void *)((uint32_t *)Addr + 1)) >> 32));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define BitGet(Addr, Pos, Type) (MemRd_U##Type(Addr) & ((1 << (Pos)) % Type)) != 0
#define BitSet(Addr, Pos, Type) (MemWt_U##Type(Addr, MemRd_U##Type(Addr) | ( (1 << (Pos % Type)))))
#define BitRst(Addr, Pos, Type) (MemWt_U##Type(Addr, MemRd_U##Type(Addr) & (~(1 << (Pos % Type)))))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define EBitGet(Addr, Pos, Type) (BitGet(&((Addr)[(Pos) / Type]), (Pos) % Type), Type)
#define EBitSet(Addr, Pos, Type) (BitSet(&((Addr)[(Pos) / Type]), (Pos) % Type), Type)
#define EBitRst(Addr, Pos, Type) (BitRst(&((Addr)[(Pos) / Type]), (Pos) % Type), Type)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline bool EBitGet_U8(uint8_t   *Addr, uintptr_t Pos) {return EBitGet(Addr, Pos,  8);}
static inline bool EBitGet_U16(uint16_t *Addr, uintptr_t Pos) {return EBitGet(Addr, Pos, 16);}
static inline bool EBitGet_U32(uint32_t *Addr, uintptr_t Pos) {return EBitGet(Addr, Pos, 32);}
static inline bool EBitGet_U64(uint64_t *Addr, uintptr_t Pos) {return EBitGet(Addr, Pos, 64);}
static inline bool EBitSet_U8(uint8_t   *Addr, uintptr_t Pos) {return EBitSet(Addr, Pos,  8);}
static inline bool EBitSet_U16(uint16_t *Addr, uintptr_t Pos) {return EBitSet(Addr, Pos, 16);}
static inline bool EBitSet_U32(uint32_t *Addr, uintptr_t Pos) {return EBitSet(Addr, Pos, 32);}
static inline bool EBitSet_U64(uint64_t *Addr, uintptr_t Pos) {return EBitSet(Addr, Pos, 64);}
static inline bool EBitRst_U8(uint8_t   *Addr, uintptr_t Pos) {return EBitRst(Addr, Pos,  8);}
static inline bool EBitRst_U16(uint16_t *Addr, uintptr_t Pos) {return EBitRst(Addr, Pos, 16);}
static inline bool EBitRst_U32(uint32_t *Addr, uintptr_t Pos) {return EBitRst(Addr, Pos, 32);}
static inline bool EBitRst_U64(uint64_t *Addr, uintptr_t Pos) {return EBitRst(Addr, Pos, 64);}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//#define ARCH_IS_LE
//#define ARCH_IS_BE
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void ToLe_U8( void *Addr)
{
}
static inline void ToLe_U16(void *Addr)
{
#ifndef ARCH_IS_BE
    uint16_t Data_16 = MemRd_U16(Addr);
    MemWt_U8((void *)((uintptr_t)Addr + 0), (Data_16 >> 0) && 0xFF);
    MemWt_U8((void *)((uintptr_t)Addr + 1), (Data_16 >> 8) && 0xFF);
    ToLe_U8((void *)((uintptr_t)Addr + 0));
    ToLe_U8((void *)((uintptr_t)Addr + 1));
#endif
}
static inline void ToLe_U32(void *Addr)
{
#ifndef ARCH_IS_BE
    uint32_t Data_32 = MemRd_U32(Addr);
    MemWt_U16((void *)((uintptr_t)Addr + 0), (Data_32 >>  0) && 0xFFFF);
    MemWt_U16((void *)((uintptr_t)Addr + 2), (Data_32 >> 16) && 0xFFFF);
    ToLe_U16((void *)((uintptr_t)Addr + 0));
    ToLe_U16((void *)((uintptr_t)Addr + 2));
#endif
}
static inline void ToLe_U64(void *Addr)
{
#ifndef ARCH_IS_BE
    uint64_t Data_64 = MemRd_U64(Addr);
    MemWt_U32((void *)((uintptr_t)Addr + 0), (Data_64 >>  0) && 0xFFFFFFFF);
    MemWt_U32((void *)((uintptr_t)Addr + 4), (Data_64 >> 32) && 0xFFFFFFFF);
    ToLe_U32((void *)((uintptr_t)Addr + 0));
    ToLe_U32((void *)((uintptr_t)Addr + 4));
#endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void ToBe_U8( void *Addr)
{
}
static inline void ToBe_U16(void *Addr)
{
#ifndef ARCH_IS_LE
    uint16_t Data_16 = MemRd_U16(Addr);
    MemWt_U8((void *)((uintptr_t)Addr + 1), (Data_16 >> 0) && 0xFF);
    MemWt_U8((void *)((uintptr_t)Addr + 0), (Data_16 >> 8) && 0xFF);
    ToBe_U8((void *)((uintptr_t)Addr + 1));
    ToBe_U8((void *)((uintptr_t)Addr + 0));
#endif
}
static inline void ToBe_U32(void *Addr)
{
#ifndef ARCH_IS_LE
    uint32_t Data_32 = MemRd_U32(Addr);
    MemWt_U16((void *)((uintptr_t)Addr + 2), (Data_32 >>  0) && 0xFFFF);
    MemWt_U16((void *)((uintptr_t)Addr + 0), (Data_32 >> 16) && 0xFFFF);
    ToBe_U16((void *)((uintptr_t)Addr + 2));
    ToBe_U16((void *)((uintptr_t)Addr + 0));
#endif
}
static inline void ToBe_U64(void *Addr)
{
#ifndef ARCH_IS_LE
    uint64_t Data_64 = MemRd_U64(Addr);
    MemWt_U32((void *)((uintptr_t)Addr + 4), (Data_64 >>  0) && 0xFFFFFFFF);
    MemWt_U32((void *)((uintptr_t)Addr + 0), (Data_64 >> 32) && 0xFFFFFFFF);
    ToBe_U32((void *)((uintptr_t)Addr + 4));
    ToBe_U32((void *)((uintptr_t)Addr + 0));
#endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline bool IsUpper(char C) {return ('A' <= C && C <= 'Z');}
static inline bool IsLower(char C) {return ('a' <= C && C <= 'z');}
static inline bool IsDigit(char C) {return ('0' <= C && C <= '9');}
static inline bool IsUpHex(char C) {return ('A' <= C && C <= 'F');}
static inline bool IsLoHex(char C) {return ('a' <= C && C <= 'f');}
static inline bool IsPrint(char C) {return (' ' <= C && C <= '~');}  /* Printable */
static inline bool IsGraph(char C) {return ('!' <= C && C <= '~');}  /* Printable(Except Space) */
static inline bool IsBlank(char C) {return (' ' == C || '\t' == C || '\v' == C);}
static inline bool IsSpace(char C) {return (' ' == C || C - '\t' <= 4);} /* Spaceable */
static inline bool IsAlpha(char C) {return (IsUpper(C) || IsLower(C));}
static inline bool IsAlNum(char C) {return (IsUpper(C) || IsLower(C) || IsDigit(C));}
static inline bool IsADHex(char C) {return (IsUpHex(C) || IsLoHex(C) || IsDigit(C));}
static inline bool IsPunct(char C) {return (IsPrint(C) && !IsAlNum(C) && ! IsSpace(C));} /* Punctuation */
static inline char ToUpper(char C) {return (IsLower(C) ? C - 'a' + 'A' : C);}
static inline char ToLower(char C) {return (IsUpper(C) ? C - 'A' + 'a' : C);}
static inline bool IsAscii(char C) {return ((C & 0x80) == 0);}
static inline char ToAscii(char C) {return ((C & 0x7F));}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint8_t AlNumToHex(char C) {return IsDigit(C) ? C - '0' :
                                                 IsUpper(C) ? C - 'A' + 10 :
                                                 IsLower(C) ? C - 'a' + 10 : 0xFF;}
static inline char HexToAlNum(uint8_t C) {return IsADHex(C) ? C < 10 ? C + '0' :
                                                              C - 10 + 'A' : 0xFF;}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t AlignV(void)       /* Value  */ {return sizeof(uintptr_t);}
static inline uintptr_t AlignO(void *Addr) /* Offset */ {return (uintptr_t)(Addr) % AlignV();}
static inline uintptr_t AlignB(void *Addr) /* Base   */ {return (uintptr_t)(Addr) - AlignO(Addr);}
static inline uintptr_t AlignL(void *Addr) /* Low    */ {return AlignB(Addr);}
static inline uintptr_t AlignH(void *Addr) /* High   */ {return AlignB(Addr) + AlignV();}
static inline bool      AlignC(void *Addr) /* Check  */ {return AlignO(Addr) == 0;}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define Unused(Expr)  do {(volatile void *)(Expr);} while (0)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define CheckOne(Cond, Expr)  if (Cond) do {Expr;} while (0)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Gets Its(Structure) Owner Address From The Member Address(Compile Resolution) */
#define GetOwner(Type, MemName, MemAddr)    \
    ((Type *)((uint8_t *)(MemAddr) - ((uint64_t) &((Type *)0)->MemName)))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define ArrLen(Arr)     (sizeof(Arr) / sizeof(Arr[0]))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define ArrCpy(Arr1, Arr2, Len)     \
    for (uint32_t Idx = 0; Idx < Len; Arr1[Idx] = Arr2[Idx], Idx++)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Array Reverse */
#define ArrRev(Type, Arr, Len)                              \
    do {                                                    \
        for (uint32_t Idx0 = 0; Idx0 < Len / 2; Idx0++) {   \
             uint32_t Idx1 = Len - 1 - Idx0;                \
             Type Tmp = Arr[Idx0];                          \
            Arr[Idx0] = Arr[Idx1];                          \
            Arr[Idx1] = Tmp;                                \
        }                                                   \
    } while (0)                                             \
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Array Left Shift(Rest Is 0) */
#define ArrLSft(Arr, Len, Cnt)                              \
    do {                                                    \
        for (int32_t Idx = 0; Idx + Cnt < Len; Idx++)       \
            Arr[Idx] = Arr[Idx + Cnt];                      \
        for (int32_t Idx = 0; Idx < Cnt; Idx++)             \
            Arr[Idx] = 0;                                   \
    } while (0)                                             \
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* Array Right Shift(Rest Is 0) */
#define ArrRSft(Arr, Len, Cnt)                              \
    do {                                                    \
        for (int32_t Idx = Len - 1; Idx - Cnt >= 0; Idx--)  \
            Arr[Idx] = Arr[Idx - Cnt];                      \
        for (int32_t Idx = Cnt - 1; Idx >= 0; Idx--)        \
            Arr[Idx] = 0;                                   \
    } while (0)                                             \
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define Min(X, Y)   ((X) < (Y) ? (X) : (Y))
#define Max(X, Y)   ((X) > (Y) ? (X) : (Y))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline    float Min_F32(   float X,    float Y) {return Min(X, Y);}
static inline   double Min_F64(  double X,   double Y) {return Min(X, Y);}
static inline   int8_t Min_I8(   int8_t X,   int8_t Y) {return Min(X, Y);}
static inline  int16_t Min_I16( int16_t X,  int16_t Y) {return Min(X, Y);}
static inline  int32_t Min_I32( int32_t X,  int32_t Y) {return Min(X, Y);}
static inline  int64_t Min_I64( int64_t X,  int64_t Y) {return Min(X, Y);}
static inline  uint8_t Min_U8(  uint8_t X,  uint8_t Y) {return Min(X, Y);}
static inline uint16_t Min_U16(uint16_t X, uint16_t Y) {return Min(X, Y);}
static inline uint32_t Min_U32(uint32_t X, uint32_t Y) {return Min(X, Y);}
static inline uint64_t Min_U64(uint64_t X, uint64_t Y) {return Min(X, Y);}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline    float Max_F32(   float X,    float Y) {return Max(X, Y);}
static inline   double Max_F64(  double X,   double Y) {return Max(X, Y);}
static inline   int8_t Max_I8(   int8_t X,   int8_t Y) {return Max(X, Y);}
static inline  int16_t Max_I16( int16_t X,  int16_t Y) {return Max(X, Y);}
static inline  int32_t Max_I32( int32_t X,  int32_t Y) {return Max(X, Y);}
static inline  int64_t Max_I64( int64_t X,  int64_t Y) {return Max(X, Y);}
static inline  uint8_t Max_U8(  uint8_t X,  uint8_t Y) {return Max(X, Y);}
static inline uint16_t Max_U16(uint16_t X, uint16_t Y) {return Max(X, Y);}
static inline uint32_t Max_U32(uint32_t X, uint32_t Y) {return Max(X, Y);}
static inline uint64_t Max_U64(uint64_t X, uint64_t Y) {return Max(X, Y);}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define Abs(X)  ((X) < 0 ? -(X) : (X))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline    float Abs_F32(  float X) {return Abs(X);}
static inline   double Abs_F64( double X) {return Abs(X);}
static inline   int8_t Abs_I8(  int8_t X) {return Abs(X);}
static inline  int16_t Abs_I16(int16_t X) {return Abs(X);}
static inline  int32_t Abs_I32(int32_t X) {return Abs(X);}
static inline  int64_t Abs_I64(int64_t X) {return Abs(X);}
static inline  uint8_t Abs_U8(  int8_t X) {return Abs(X);}
static inline uint16_t Abs_U16(int16_t X) {return Abs(X);}
static inline uint32_t Abs_U32(int32_t X) {return Abs(X);}
static inline uint64_t Abs_U64(int64_t X) {return Abs(X);}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define Map(X, LI, RI, LO, RO)  ((((RO) - (LO)) / ((RI) - (LI))) * ((X) - (LI)) + (LO))
#define Map_Func                {return Map(X, LI, RI, LO, RO);}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline  int8_t Map_I8(  int8_t X,  int8_t LI,  int8_t LO,  int8_t RI,  int8_t RO) Map_Func
static inline int16_t Map_I16(int16_t X, int16_t LI, int16_t LO, int16_t RI, int16_t RO) Map_Func
static inline int32_t Map_I32(int32_t X, int32_t LI, int32_t LO, int32_t RI, int32_t RO) Map_Func
static inline int64_t Map_I16(int64_t X, int64_t LI, int64_t LO, int64_t RI, int64_t RO) Map_Func
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define CheckSum(Type, Buffer, Length)  \
    {Type Result = 0; for (uint32_t I = 0; I < Length; Result += Buffer[I++]); return Result;}
#define CheckSum_Func(Type)     CheckSum(Type, Buffer, Length)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline  uint8_t CheckSum_U8(  uint8_t *Buffer, uint32_t Length)  CheckSum_Func( uint8_t)
static inline uint16_t CheckSum_U16(uint16_t *Buffer, uint32_t Length)  CheckSum_Func(uint16_t)
static inline uint32_t CheckSum_U32(uint32_t *Buffer, uint32_t Length)  CheckSum_Func(uint32_t)
static inline uint64_t CheckSum_U64(uint64_t *Buffer, uint32_t Length)  CheckSum_Func(uint64_t)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define StrToNum(Str, Type, NoInt, CheckFlag, BaseFlag)         \
{                                                               \
    bool Flag = false;                                          \
    Type Num1 = 0, Num2 = 0; uint8_t Base = 10;                 \
    while (IsBlank(*Str)) Str++;                                \
    CheckFlag && *Str == '-' && (Str++, Flag = ~Flag);          \
    CheckFlag && *Str == '+' && (Str++);                        \
    BaseFlag && *Str == '0' && (Str++, Base = 8);               \
    BaseFlag && *Str == 'x' && (Str++, Base = 16);              \
    BaseFlag && *Str == 'X' && (Str++, Base = 16);              \
    while (IsDigit(*Str)) Num1 = Num1 * Base - (*Str++ - '0');  \
    if (NoInt)                                                  \
    if (*Str == '.') {Str++;}                                   \
    if (NoInt)                                                  \
    while (IsDigit(*Str)) Num2 = Num2 / Base - (*Str++ - '0');  \
    return Flag ? Num1 + Num2 : - Num1 - Num2;                  \
}                                                               \
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline    float StrToF32(char *Str) StrToNum(Str,    float,  true, true,  false)
static inline   double StrToF64(char *Str) StrToNum(Str,   double,  true, true,  false)
static inline   int8_t StrToI8( char *Str) StrToNum(Str,   int8_t, false, true,  false)
static inline  int16_t StrToI16(char *Str) StrToNum(Str,  int16_t, false, true,  false)
static inline  int32_t StrToI32(char *Str) StrToNum(Str,  int32_t, false, true,  false)
static inline  int64_t StrToI64(char *Str) StrToNum(Str,  int64_t, false, true,  false)
static inline  uint8_t StrToU8( char *Str) StrToNum(Str,  uint8_t, false, false, false)
static inline uint16_t StrToU16(char *Str) StrToNum(Str, uint16_t, false, false, false)
static inline uint32_t StrToU32(char *Str) StrToNum(Str, uint32_t, false, false, false)
static inline uint64_t StrToU64(char *Str) StrToNum(Str, uint64_t, false, false, false)
static inline  uint8_t StrToH8( char *Str) StrToNum(Str,  uint8_t, false, true,  true)
static inline uint16_t StrToH16(char *Str) StrToNum(Str, uint16_t, false, true,  true)
static inline uint32_t StrToH32(char *Str) StrToNum(Str, uint32_t, false, true,  true)
static inline uint64_t StrToH64(char *Str) StrToNum(Str, uint64_t, false, true,  true)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define CountLeadZero(Type, Data)                               \
{                                                               \
    for (uint8_t Index = 0; Index < sizeof(Type) * 8; Index++)  \
        if ((1 << (sizeof(Type) * 8 - 1 - Index)) & Data)       \
            return sizeof(Type) * 8 - 1 - Index;                \
            return sizeof(Type) * 8;                            \
}                                                               \
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline  uint8_t CLZ_B1( uint8_t Data) CountLeadZero( uint8_t, Data)
static inline uint16_t CLZ_B2(uint16_t Data) CountLeadZero(uint16_t, Data)
static inline uint32_t CLZ_B4(uint32_t Data) CountLeadZero(uint32_t, Data)
static inline uint64_t CLZ_B8(uint64_t Data) CountLeadZero(uint64_t, Data)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define CountTailZero(Type, Data)                               \
{                                                               \
    for (uint8_t Index = 0; Index < sizeof(Type) * 8; Index++)  \
        if ((1 << (Index)) & Data)                              \
            return Index;                                       \
            return sizeof(Type) * 8;                            \
}                                                               \
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline  uint8_t CTZ_B1( uint8_t Data) CountTailZero( uint8_t, Data)
static inline uint16_t CTZ_B2(uint16_t Data) CountTailZero(uint16_t, Data)
static inline uint32_t CTZ_B4(uint32_t Data) CountTailZero(uint32_t, Data)
static inline uint64_t CTZ_B8(uint64_t Data) CountTailZero(uint64_t, Data)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define CountZero(Type, Data)                                   \
{                                                               \
    uint8_t Count = 0;                                          \
    for (uint8_t Index = 0; Index < sizeof(Type) * 8; Index++)  \
        if (((1 << (Index)) & Data) == 0) Count++;              \
    return Count;                                               \
}                                                               \
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint8_t CZ_B1( uint8_t Data) CountZero( uint8_t, Data)
static inline uint8_t CZ_B2(uint16_t Data) CountZero(uint16_t, Data)
static inline uint8_t CZ_B4(uint32_t Data) CountZero(uint32_t, Data)
static inline uint8_t CZ_B8(uint64_t Data) CountZero(uint64_t, Data)
static inline uint8_t CO_B1( uint8_t Data) {return sizeof( uint8_t) - CZ_B1(Data);}
static inline uint8_t CO_B2(uint16_t Data) {return sizeof(uint16_t) - CZ_B2(Data);}
static inline uint8_t CO_B4(uint32_t Data) {return sizeof(uint32_t) - CZ_B4(Data);}
static inline uint8_t CO_B8(uint64_t Data) {return sizeof(uint64_t) - CZ_B8(Data);}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define KeepSupplement(...)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
