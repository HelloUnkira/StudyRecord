#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H
// C std lib
#include <stdint.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define Unused(Expression)  do { (volatile void *)(Expression) } while (0)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define MemR_F32(Address)   (*(volatile    float *)(Address))
#define MemR_F64(Address)   (*(volatile   double *)(Address))
#define MemR_I8( Address)   (*(volatile   int8_t *)(Address))
#define MemR_I16(Address)   (*(volatile  int16_t *)(Address))
#define MemR_I32(Address)   (*(volatile  int32_t *)(Address))
#define MemR_I64(Address)   (*(volatile  int64_t *)(Address))
#define MemR_U8( Address)   (*(volatile  uint8_t *)(Address))
#define MemR_U16(Address)   (*(volatile uint16_t *)(Address))
#define MemR_U32(Address)   (*(volatile uint32_t *)(Address))
#define MemR_U64(Address)   (*(volatile uint64_t *)(Address))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define MemW_F32(Address, Data) ((*(volatile    float *)(Address)) = (Data))
#define MemW_F64(Address, Data) ((*(volatile   double *)(Address)) = (Data))
#define MemW_I8( Address, Data) ((*(volatile   int8_t *)(Address)) = (Data))
#define MemW_I16(Address, Data) ((*(volatile  int16_t *)(Address)) = (Data))
#define MemW_I32(Address, Data) ((*(volatile  int32_t *)(Address)) = (Data))
#define MemW_I64(Address, Data) ((*(volatile  int64_t *)(Address)) = (Data))
#define MemW_U8( Address, Data) ((*(volatile  uint8_t *)(Address)) = (Data))
#define MemW_U16(Address, Data) ((*(volatile uint16_t *)(Address)) = (Data))
#define MemW_U32(Address, Data) ((*(volatile uint32_t *)(Address)) = (Data))
#define MemW_U64(Address, Data) ((*(volatile uint64_t *)(Address)) = (Data))
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
static inline uintptr_t P_To_V(void * X) {return (uintptr_t)X;}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define KeepAdding(...)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
