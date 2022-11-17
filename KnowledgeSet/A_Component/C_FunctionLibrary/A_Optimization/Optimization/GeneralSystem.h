#ifndef GENERAL_SYSTEM_H
#define GENERAL_SYSTEM_H
// C std lib
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define MemR(Addr, Type)        ((*(volatile Type *)(Addr)))
#define MemW(Addr, Data, Type)  ((*(volatile Type *)(Addr)) = (Data))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline    float MemR_F32(void *Addr) {return MemR(Addr, float   );}
static inline   double MemR_F64(void *Addr) {return MemR(Addr, double  );}
static inline   int8_t MemR_I8(void  *Addr) {return MemR(Addr, int8_t  );}
static inline  int16_t MemR_I16(void *Addr) {return MemR(Addr, int16_t );}
static inline  int32_t MemR_I32(void *Addr) {return MemR(Addr, int32_t );}
static inline  int64_t MemR_I64(void *Addr) {return MemR(Addr, int64_t );}
static inline  uint8_t MemR_U8(void  *Addr) {return MemR(Addr, uint8_t );}
static inline uint16_t MemR_U16(void *Addr) {return MemR(Addr, uint16_t);}
static inline uint32_t MemR_U32(void *Addr) {return MemR(Addr, uint32_t);}
static inline uint64_t MemR_U64(void *Addr) {return MemR(Addr, uint64_t);}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void MemW_F32(void *Addr,    float Data) {MemW(Addr, Data,    float);}
static inline void MemW_F64(void *Addr,   double Data) {MemW(Addr, Data,   double);}
static inline void MemW_I8( void *Addr,   int8_t Data) {MemW(Addr, Data,   int8_t);}
static inline void MemW_I16(void *Addr,  int16_t Data) {MemW(Addr, Data,  int16_t);}
static inline void MemW_I32(void *Addr,  int32_t Data) {MemW(Addr, Data,  int32_t);}
static inline void MemW_I64(void *Addr,  int64_t Data) {MemW(Addr, Data,  int64_t);}
static inline void MemW_U8( void *Addr,  uint8_t Data) {MemW(Addr, Data,  uint8_t);}
static inline void MemW_U16(void *Addr, uint16_t Data) {MemW(Addr, Data, uint16_t);}
static inline void MemW_U32(void *Addr, uint32_t Data) {MemW(Addr, Data, uint32_t);}
static inline void MemW_U64(void *Addr, uint64_t Data) {MemW(Addr, Data, uint64_t);}
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
    
    MemW_U16(Addr, ((uint16_t)MemR_U8((void *)((uint8_t *)Addr + 0)) << 8) |
                   ((uint16_t)MemR_U8((void *)((uint8_t *)Addr + 1)) >> 8));
}
static inline void MemRev_B4(void *Addr)
{
    MemRev_B2((void *)((uint16_t *)Addr + 0));
    MemRev_B2((void *)((uint16_t *)Addr + 1));
    
    MemW_U32(Addr, ((uint32_t)MemR_U16((void *)((uint16_t *)Addr + 0)) << 16) |
                   ((uint32_t)MemR_U16((void *)((uint16_t *)Addr + 1)) >> 16));
}
static inline void MemRev_B8(void *Addr)
{
    MemRev_B4((void *)((uint32_t *)Addr + 0));
    MemRev_B4((void *)((uint32_t *)Addr + 1));
    
    MemW_U64(Addr, ((uint64_t)MemR_U32((void *)((uint32_t *)Addr + 0)) << 32) |
                   ((uint64_t)MemR_U32((void *)((uint32_t *)Addr + 1)) >> 32));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define BitG(Addr, Pos, Type) (MemR_U##Type(Addr) & ((1 << (Pos)) % Type)) != 0
#define BitS(Addr, Pos, Type) (MemW_U##Type(Addr, MemR_U##Type(Addr) | ( (1 << (Pos % Type)))))
#define BitR(Addr, Pos, Type) (MemW_U##Type(Addr, MemR_U##Type(Addr) & (~(1 << (Pos % Type)))))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define EBitG(Addr, Pos, Type) (BitG(&((Addr)[(Pos) / Type]), (Pos) % Type), Type)
#define EBitS(Addr, Pos, Type) (BitS(&((Addr)[(Pos) / Type]), (Pos) % Type), Type)
#define EBitR(Addr, Pos, Type) (BitR(&((Addr)[(Pos) / Type]), (Pos) % Type), Type)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline bool EBitG_U8(uint8_t   *Addr, uintptr_t Pos) {return EBitG(Addr, Pos,  8);}
static inline bool EBitG_U16(uint16_t *Addr, uintptr_t Pos) {return EBitG(Addr, Pos, 16);}
static inline bool EBitG_U32(uint32_t *Addr, uintptr_t Pos) {return EBitG(Addr, Pos, 32);}
static inline bool EBitG_U64(uint64_t *Addr, uintptr_t Pos) {return EBitG(Addr, Pos, 64);}
static inline bool EBitS_U8(uint8_t   *Addr, uintptr_t Pos) {return EBitS(Addr, Pos,  8);}
static inline bool EBitS_U16(uint16_t *Addr, uintptr_t Pos) {return EBitS(Addr, Pos, 16);}
static inline bool EBitS_U32(uint32_t *Addr, uintptr_t Pos) {return EBitS(Addr, Pos, 32);}
static inline bool EBitS_U64(uint64_t *Addr, uintptr_t Pos) {return EBitS(Addr, Pos, 64);}
static inline bool EBitR_U8(uint8_t   *Addr, uintptr_t Pos) {return EBitR(Addr, Pos,  8);}
static inline bool EBitR_U16(uint16_t *Addr, uintptr_t Pos) {return EBitR(Addr, Pos, 16);}
static inline bool EBitR_U32(uint32_t *Addr, uintptr_t Pos) {return EBitR(Addr, Pos, 32);}
static inline bool EBitR_U64(uint64_t *Addr, uintptr_t Pos) {return EBitR(Addr, Pos, 64);}
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
    uint16_t Data_16 = MemR_U16(Addr);
    MemW_U8((void *)((uintptr_t)Addr + 0), (Data_16 >> 0) && 0xFF);
    MemW_U8((void *)((uintptr_t)Addr + 1), (Data_16 >> 8) && 0xFF);
    ToLe_U8((void *)((uintptr_t)Addr + 0));
    ToLe_U8((void *)((uintptr_t)Addr + 1));
#endif
}
static inline void ToLe_U32(void *Addr)
{
#ifndef ARCH_IS_BE
    uint32_t Data_32 = MemR_U32(Addr);
    MemW_U16((void *)((uintptr_t)Addr + 0), (Data_32 >>  0) && 0xFFFF);
    MemW_U16((void *)((uintptr_t)Addr + 2), (Data_32 >> 16) && 0xFFFF);
    ToLe_U16((void *)((uintptr_t)Addr + 0));
    ToLe_U16((void *)((uintptr_t)Addr + 2));
#endif
}
static inline void ToLe_U64(void *Addr)
{
#ifndef ARCH_IS_BE
    uint64_t Data_64 = MemR_U64(Addr);
    MemW_U32((void *)((uintptr_t)Addr + 0), (Data_64 >>  0) && 0xFFFFFFFF);
    MemW_U32((void *)((uintptr_t)Addr + 4), (Data_64 >> 32) && 0xFFFFFFFF);
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
    uint16_t Data_16 = MemR_U16(Addr);
    MemW_U8((void *)((uintptr_t)Addr + 1), (Data_16 >> 0) && 0xFF);
    MemW_U8((void *)((uintptr_t)Addr + 0), (Data_16 >> 8) && 0xFF);
    ToBe_U8((void *)((uintptr_t)Addr + 1));
    ToBe_U8((void *)((uintptr_t)Addr + 0));
#endif
}
static inline void ToBe_U32(void *Addr)
{
#ifndef ARCH_IS_LE
    uint32_t Data_32 = MemR_U32(Addr);
    MemW_U16((void *)((uintptr_t)Addr + 2), (Data_32 >>  0) && 0xFFFF);
    MemW_U16((void *)((uintptr_t)Addr + 0), (Data_32 >> 16) && 0xFFFF);
    ToBe_U16((void *)((uintptr_t)Addr + 2));
    ToBe_U16((void *)((uintptr_t)Addr + 0));
#endif
}
static inline void ToBe_U64(void *Addr)
{
#ifndef ARCH_IS_LE
    uint64_t Data_64 = MemR_U64(Addr);
    MemW_U32((void *)((uintptr_t)Addr + 4), (Data_64 >>  0) && 0xFFFFFFFF);
    MemW_U32((void *)((uintptr_t)Addr + 0), (Data_64 >> 32) && 0xFFFFFFFF);
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
static inline bool IsAlpha(char C) {return ('A' <= C && C <= 'Z') || ('a' <= C && C <= 'z');}
static inline bool IsAlNum(char C) {return ('A' <= C && C <= 'Z') || ('a' <= C && C <= 'z') ||
                                           ('0' <= C && C <= '9');}
static inline bool IsADHex(char C) {return ('A' <= C && C <= 'F') || ('a' <= C && C <= 'f') ||
                                           ('0' <= C && C <= '9');}
static inline bool IsBlank(char C) {return (' ' == C || '\t' == C || '\n' == C);}
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
#define ArrLen(Arr)     (sizeof(Arr) / sizeof(Arr[0]))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define ArrCpy(Arr1, Arr2, Len)     \
    for (uint32_t Idx = 0; Idx < Len; Arr1[Idx] = Arr2[Idx], Idx++)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
#define ArrRSft(Arr, Len, Cnt)                             \
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
#define KeepSupplement(...)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
