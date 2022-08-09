#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H
// C std lib
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define Unused(Expr)  do {(volatile void *)(Expr);} while (0)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define MemR_F32(Addr)   (*(volatile    float *)(Addr))
#define MemR_F64(Addr)   (*(volatile   double *)(Addr))
#define MemR_I8( Addr)   (*(volatile   int8_t *)(Addr))
#define MemR_I16(Addr)   (*(volatile  int16_t *)(Addr))
#define MemR_I32(Addr)   (*(volatile  int32_t *)(Addr))
#define MemR_I64(Addr)   (*(volatile  int64_t *)(Addr))
#define MemR_U8( Addr)   (*(volatile  uint8_t *)(Addr))
#define MemR_U16(Addr)   (*(volatile uint16_t *)(Addr))
#define MemR_U32(Addr)   (*(volatile uint32_t *)(Addr))
#define MemR_U64(Addr)   (*(volatile uint64_t *)(Addr))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define MemW_F32(Addr, Data) ((*(volatile    float *)(Addr)) = (Data))
#define MemW_F64(Addr, Data) ((*(volatile   double *)(Addr)) = (Data))
#define MemW_I8( Addr, Data) ((*(volatile   int8_t *)(Addr)) = (Data))
#define MemW_I16(Addr, Data) ((*(volatile  int16_t *)(Addr)) = (Data))
#define MemW_I32(Addr, Data) ((*(volatile  int32_t *)(Addr)) = (Data))
#define MemW_I64(Addr, Data) ((*(volatile  int64_t *)(Addr)) = (Data))
#define MemW_U8( Addr, Data) ((*(volatile  uint8_t *)(Addr)) = (Data))
#define MemW_U16(Addr, Data) ((*(volatile uint16_t *)(Addr)) = (Data))
#define MemW_U32(Addr, Data) ((*(volatile uint32_t *)(Addr)) = (Data))
#define MemW_U64(Addr, Data) ((*(volatile uint64_t *)(Addr)) = (Data))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define BitG_U8( Addr, Pos) ((MemR_U8(Addr)  & (1 << (Pos % 8)))  == 0 ? false : true)
#define BitG_U16(Addr, Pos) ((MemR_U16(Addr) & (1 << (Pos % 16))) == 0 ? false : true)
#define BitG_U32(Addr, Pos) ((MemR_U32(Addr) & (1 << (Pos % 32))) == 0 ? false : true)
#define BitG_U64(Addr, Pos) ((MemR_U64(Addr) & (1 << (Pos % 64))) == 0 ? false : true)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define BitS_U8( Addr, Pos) (MemW_U8(Addr,  (MemR_U8(Addr)  | (1 << (Pos % 8)))))
#define BitS_U16(Addr, Pos) (MemW_U16(Addr, (MemR_U16(Addr) | (1 << (Pos % 16)))))
#define BitS_U32(Addr, Pos) (MemW_U32(Addr, (MemR_U32(Addr) | (1 << (Pos % 32)))))
#define BitS_U64(Addr, Pos) (MemW_U64(Addr, (MemR_U64(Addr) | (1 << (Pos % 64)))))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define BitR_U8( Addr, Pos) (MemW_U8(Addr,  (MemR_U8(Addr)  & (~(1 << (Pos % 8))))))
#define BitR_U16(Addr, Pos) (MemW_U16(Addr, (MemR_U16(Addr) & (~(1 << (Pos % 16))))))
#define BitR_U32(Addr, Pos) (MemW_U32(Addr, (MemR_U32(Addr) & (~(1 << (Pos % 32))))))
#define BitR_U64(Addr, Pos) (MemW_U64(Addr, (MemR_U64(Addr) & (~(1 << (Pos % 64))))))
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
#define LShift(Data, Offset)    ((Data) << (Offset))
#define RShift(Data, Offset)    ((Data) >> (Offset))
#define ArrLen(Array)   (sizeof(Array) / sizeof(Array[0]))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline bool IsSupper(char C) {return ('A' <= C && C <= 'Z');}
static inline bool IsLower(char C)  {return ('a' <= C && C <= 'z');}
static inline bool IsDigit(char C)  {return ('0' <= C && C <= '9');}
static inline bool IsAlpha(char C)  {return ('A' <= C && C <= 'Z') || ('a' <= C && C <= 'z');}
static inline bool IsAlNum(char C)  {return ('A' <= C && C <= 'Z') || ('a' <= C && C <= 'z') ||
                                            ('0' <= C && C <= '9');}
static inline bool IsHex(char C)    {return ('A' <= C && C <= 'F') || ('a' <= C && C <= 'f') ||
                                            ('0' <= C && C <= '9');}
static inline bool IsBlank(char C)  {return (' ' == C || '\t' == C || '\n' == C);}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uintptr_t P_To_V(void * X) {return (uintptr_t)X;}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define KeepSupplement(...)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
