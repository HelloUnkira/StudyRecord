#ifndef FINITE_SET_H
#define FINITE_SET_H
//C std lib
#include <stdint.h>
#include <stdbool.h>
//Discrete Math Type
#include "../DiscreteMath.h"
#include "..\DiscreteMath.h"
/*************************************************************************************************/
/* 离散数学:全集集合 */
typedef struct FiniteFullSetBody {
    DiscreteMathType FullSet;   /* 集合实体 */
    uint32_t Length;            /* 集合实体长度 */
    uint32_t Number;            /* 集合当前数量 */
    uint8_t  Type;              /* 集合类型 */
} FiniteFullSet;
/*************************************************************************************************/
/* 离散数学:子集集合 */
typedef struct FiniteSubSetBody {
    FullSet *Full;      /* 集合全集 */
    uint8_t *BitMap;    /* 集合位图 */
    uint32_t Length;    /* 集合位图长度 */
} FiniteSubSet;
/*************************************************************************************************/
static inline void FiniteFullSet_Configure(FiniteFullSet *Set, uint8_t Type,
                                           DiscreteMathType FullSet, uint32_t Length)
{
    Set->Type    = Type;
    Set->FullSet = FullSet;
    Set->Length  = Length;
    Set->Number  = 0;
}
/*************************************************************************************************/

/* 离散数学:全集集合 */
typedef struct DiscreteMathFullSetBody {
    DiscreteMathType Element;   /* 集合实体 */
    uint32_t Length;            /* 集合实体长度 */
    uint32_t Number;            /* 集合当前数量 */
    uint8_t Type;               /* 集合类型 */
} DiscreteMathFullSet;

/* 离散数学:子集集合 */
typedef struct DiscreteMathSubSetBody {
    DiscreteMathFullSet *Full;  /* 集合全集 */
    uint8_t *BitMap;            /* 集合位图 */
    uint32_t Length;            /* 集合位图长度 */
} DiscreteMathSubSet;

/*基本运算:全集集合:
 *1.全集集合:创建,销毁
 *2.全集集合元素检查
 *3.全集集合元素插入(头插,尾插,指定元素前插,指定元素后插)
 *4.全集集合元素移除(指定元素移除,其余依次迁移)
 *备注:全集集合一般在初始化并配置好元素后基本上不会有大改动
 *     所以对这一部分的时间复杂度要求并不高
 */

/*基本运算:子集集合:
 *1.子集集合:创建,销毁
 *2.子集集合查验所有元素
 *3.子集集合查验指定元素
 *4.子集集合设置元素(相对于它的全集)
 *5.子集集合取消元素(相对于它的全集)
 *6.子集集合求补集(~A)       (位图非运算)
 *7.子集集合求并集(A|B)      (位图或运算),要求:相对于同一全集
 *8.子集集合求交集(A&B)      (位图与运算),要求:相对于同一全集
 *9.子集集合求差集(A-B)      (位图运算),要求:相对于同一全集
 */


/*************************************************************************************************/
#endif
