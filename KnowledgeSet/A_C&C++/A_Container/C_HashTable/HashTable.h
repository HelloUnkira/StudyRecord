#ifndef HASH_TABLE_H
#define HASH_TABLE_H
//C std lib
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "DoubleList.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 从当前节点节点地址获得它的所有者地址(编译时解析) */
#define HT_GetOwner(Type, Member, MemberAddress)    \
    ((Type *)((uint8_t *)(MemberAddress) - ((uint64_t) &((Type *)0)->Member)))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct HashTable_Node {
    DL_Node Node;
} HT_Node;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct HashTable_Nodes {
    DL_List List;
} HT_List;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct HashTable_Table {
    void    *Parameter1[3];
    uint32_t Parameter2;
} HT_Table;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef uint32_t (*HT_HashFunction)(HT_Node *Node);
typedef bool     (*HT_IsEqual)(HT_Node *Node1, HT_Node *Node2);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void HT_Node_Reset(HT_Node *Node);
void HT_List_Reset(HT_List *Array, uint32_t Length);
void HT_Table_SetZone(HT_Table *Table, HT_List *Array, uint32_t Length);
void HT_Table_Set(HT_Table *Table, HT_HashFunction HashFunction, HT_IsEqual IsEqual);
void HT_Node_Add(HT_Table *Table, HT_Node *Node);
void HT_Node_Remove(HT_Table *Table, HT_Node *Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 通过特性值匹配的假节点获得真节点,不存在则证明未在表中 */
HT_Node * HT_Node_Search(HT_Table *Table, HT_Node *Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef void (*HT_NodeCheck)(char *String, HT_Node *Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void HT_TableNodeCheck(HT_Table *Table, HT_NodeCheck Check);
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
