#ifndef HASH_TABLE_H
#define HASH_TABLE_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//用于维护非实体外的数据结构
#include <stdlib.h>
//动态收缩Hash表需要额外的动态内存分配接口用于内部生成维护空间
#define HT_ALLOC(size)      malloc(size)
#define HT_FREE(pointer)    free(pointer)
//如果上述接口不能正确提供,Hash表不能正常工作
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 结构中关键信息散列摘要 */
typedef uint32_t (*HT_Digest)(void *Parameter);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 结构中关键信息匹配 */
typedef bool (*HT_Comfirm)(void *Parameter, void *Target);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct HashTable {
    HT_Digest  Digest;
    HT_Comfirm Comfirm;
    uint32_t   Size;
    void     **List;
    uint32_t   Number;
    uint8_t  FailCount;     /* 未命中最大尝试次数(失败自动扩张) */
    uint8_t  FactorMax;     /* 伸缩比例最大值(0,100)(超出该比例自动扩张) */
    uint8_t  FactorMin;     /* 伸缩比例最小值(0,100](低于该比例自动收缩) */
    uint8_t  FactorScale;   /* 伸缩单位比例(0,100) */
    uint32_t SearchHit;     /* 命中次数 */
    uint32_t SearchMiss;    /* 未命中次数 */
    uint32_t InsertHit;     /* 命中次数 */
    uint32_t InsertMiss;    /* 未命中次数 */
    uint32_t RemoveHit;     /* 命中次数 */
    uint32_t RemoveMiss;    /* 未命中次数 */
} HT_Table;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void HT_Configure(HT_Table *Table, HT_Digest Digest, HT_Comfirm Comfirm,
                                uint8_t FailCount, uint8_t FactorScale,
                                uint8_t FactorMax, uint8_t FactorMin)
{
    Table->Digest  = Digest;
    Table->Comfirm = Comfirm;
    Table->Size    = 0;
    Table->List    = NULL;
    Table->Number  = 0;
    Table->FailCount   = FailCount;
    Table->FactorScale = FactorScale;
    Table->FactorMax   = FactorMax;
    Table->FactorMin   = FactorMin;
    Table->SearchHit   = 0;
    Table->SearchMiss  = 0;
    Table->InsertHit   = 0;
    Table->InsertMiss  = 0;
    Table->RemoveHit   = 0;
    Table->RemoveMiss  = 0;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void HT_Ready(HT_Table *Table, uint32_t Size)
{
    Table->Size = Size;
    Table->List = HT_ALLOC(Size * sizeof(void *));
    
    for (uint32_t Index = 0; Index < Size; Table->List[Index] = NULL, Index++);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void HT_Clean(HT_Table *Table)
{
    HT_FREE(Table->List);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 通过特性值匹配的假节点获得真节点,不存在则证明未在表中 */
void * HT_Search(HT_Table *Table, void *Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void HT_Insert(HT_Table *Table, void *Node);
void HT_Remove(HT_Table *Table, void *Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 下面是摘要的来源网络的Hash散列函数 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint32_t Elf_HashFunction(uint8_t *Data, uint32_t Length)
{
    uint64_t H = 0, G = 0;
    
    for (uint32_t Index = 0; Index < Length; Index++) {
        H = (H << 4) + Data[Index];
        if (G = H & 0xf0000000)
            H ^= G >> 24;
        H &= ~G;
    }
    
    /* 散列不均匀时需要适当加点盐 */
    const uint32_t Salt = 13;
    
    return (uint32_t)(H >> Salt);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
