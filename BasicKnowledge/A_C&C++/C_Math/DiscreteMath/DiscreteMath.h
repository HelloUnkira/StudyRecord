#ifndef DISCRETE_MATH_H
#define DISCRETE_MATH_H
//C std lib
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
#define DISCRETE_MATH_BITS   (8)
/* 抽象类型集合:使用时指定(如果模组需要指定) */
#define DISCRETE_MATH_INT8   (0) //int8_t
#define DISCRETE_MATH_INT16  (1) //int16_t
#define DISCRETE_MATH_INT32  (2) //int32_t
#define DISCRETE_MATH_INT64  (3) //int64_t
#define DISCRETE_MATH_UINT8  (4) //uint8_t
#define DISCRETE_MATH_UINT16 (5) //uint16_t
#define DISCRETE_MATH_UINT32 (6) //uint32_t
#define DISCRETE_MATH_UINT64 (7) //uint64_t
#define DISCRETE_MATH_FLOAT  (8) //float(32)
#define DISCRETE_MATH_DOUBLE (9) //double(64)
/*************************************************************************************************/
/* 抽象类型集合1 */
typedef union DiscreteMathTypeBody {
    void     *None;
    int8_t   *INT8;
    int16_t  *INT16;
    int32_t  *INT32;
    int64_t  *INT64;
    uint8_t  *UINT8;
    uint16_t *UINT16;
    uint32_t *UINT32;
    uint64_t *UINT64;
    float    *FLOAT32;
    double   *DOUBLE64;
} DiscreteMathType;
/*************************************************************************************************/
#endif
