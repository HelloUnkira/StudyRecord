#ifndef CFLINTDATA_H
#define CFLINTDATA_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 大小端序配置项 */
#if 0
#elif   CFLINT_ENDIAN_PORT == 0
#define CFLINT_ENDIAN_BIG
#elif   CFLINT_ENDIAN_PORT == 1
#define CFLINT_ENDIAN_LITTLE
#else
#error  "Error Configure:Port Endian"
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 数据存储类型 */
#define CFLINT_TYPE1    uint8_t     /* 1字节类型 */
#define CFLINT_TYPE2    uint16_t    /* 2字节类型 */
#define CFLINT_TYPE4    uint32_t    /* 4字节类型 */
#define CFLINT_TYPE8    uint64_t    /* 8字节类型 */
/* 数据存储类型 */
#define CFLINT_BYTE1    1   /* 1字节类型 */
#define CFLINT_BYTE2    2   /* 2字节类型 */
#define CFLINT_BYTE4    4   /* 4字节类型 */
#define CFLINT_BYTE8    8   /* 8字节类型 */
/* 数据存储类型 */
#define CFLINT_BITS1    8   /* 对应数据进制: 2^8 */
#define CFLINT_BITS2    16  /* 对应数据进制: 2^16 */
#define CFLINT_BITS4    32  /* 对应数据进制: 2^32 */
#define CFLINT_BITS8    64  /* 对应数据进制: 2^64 */
/* 扩展数据 */
#define CFLINT_BYTE1ALL 0xFF
#define CFLINT_BYTE2ALL 0xFFFF
#define CFLINT_BYTE4ALL 0xFFFFFFFF
#define CFLINT_BYTE8ALL 0xFFFFFFFFFFFFFFFF
/* 扩展数据 */
#define CFLINT_BYTE1HALFLOW 0x0F
#define CFLINT_BYTE2HALFLOW 0x00FF
#define CFLINT_BYTE4HALFLOW 0x0000FFFF
#define CFLINT_BYTE8HALFLOW 0x00000000FFFFFFFF
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 平台字节对齐类型定义 */
#if 0
#define CFLINT_BYTEHALFLOW  /* 平台字:低半字0,高半字1 */
#define CFLINT_BYTEALL      /* 平台字:全字为1 */
#define CFLINT_BITS         /* 平台字占有二进制位数 */
#define CFLINT_TYPE         /* 平台字占有字节数 */
#define CFLINT_TYPE_2       /* 关键字优化 */
#elif   CFLINT_TYPE_PORT == 1
#define CFLINT_BYTEHALFLOW  CFLINT_BYTE1HALFLOW
#define CFLINT_BYTEALL      CFLINT_BYTE1ALL
#define CFLINT_BITS         CFLINT_BITS1
#define CFLINT_TYPE         CFLINT_TYPE1
#define CFLINT_TYPE_2       CFLINT_TYPE2
#elif   CFLINT_TYPE_PORT == 2
#define CFLINT_BYTEHALFLOW  CFLINT_BYTE2HALFLOW
#define CFLINT_BYTEALL      CFLINT_BYTE2ALL
#define CFLINT_BITS         CFLINT_BITS2
#define CFLINT_TYPE         CFLINT_TYPE2
#define CFLINT_TYPE_2       CFLINT_TYPE4
#elif   CFLINT_TYPE_PORT == 4
#define CFLINT_BYTEHALFLOW  CFLINT_BYTE4HALFLOW
#define CFLINT_BYTEALL      CFLINT_BYTE4ALL
#define CFLINT_BITS         CFLINT_BITS4
#define CFLINT_TYPE         CFLINT_TYPE4
#define CFLINT_TYPE_2       CFLINT_TYPE8
#elif   CFLINT_TYPE_PORT == 8
#define CFLINT_BYTEHALFLOW  CFLINT_BYTE8HALFLOW
#define CFLINT_BYTEALL      CFLINT_BYTE8ALL
#define CFLINT_BITS         CFLINT_BITS8
#define CFLINT_TYPE         CFLINT_TYPE8
//#define CFLINT_TYPE_2        CFLINT_TYPE16
#else
#error  "Error Configure:Port Byte Alignment"
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
