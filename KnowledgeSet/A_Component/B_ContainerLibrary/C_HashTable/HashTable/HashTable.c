#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "DoubleList.h"
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
    HT_List  *Array;
    uint32_t (*HashFunction)(HT_Node *Node);
    bool     (*IsEqual)(HT_Node *Node1, HT_Node *Node2);
    uint32_t   Length;
} HT_Table;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef uint32_t (*HT_HashFunction)(HT_Node *Node);
typedef bool     (*HT_IsEqual)(HT_Node *Node1, HT_Node *Node2);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void HT_Node_Reset(HT_Node *Node)
{
    DL_Node_Reset(&(Node->Node));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void HT_List_Reset(HT_List *Array, uint32_t Length)
{
    for (uint32_t Index = 0; Index < Length; Index++)
        DL_List_Reset(&(Array[Index].List));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void HT_Table_SetZone(HT_Table *Table, HT_List *Array, uint32_t Length)
{
    Table->Array  = Array;
    Table->Length = Length;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void HT_Table_Set(HT_Table *Table, HT_HashFunction HashFunction, HT_IsEqual IsEqual)
{
    Table->HashFunction = HashFunction;
    Table->IsEqual      = IsEqual;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void HT_Node_Add(HT_Table *Table, HT_Node *Node)
{
    /* ??????,?????? */
    uint32_t Index = Table->HashFunction(Node) % Table->Length;
    /* ????????????,???????????? */
    //DL_List_InsertPrepend(&((Table->Array[Index]).List), NULL, &(Node->Node));
    DL_List_InsertAppend(&((Table->Array[Index]).List), NULL, &(Node->Node));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void HT_Node_Remove(HT_Table *Table, HT_Node *Node)
{
    /* ??????,?????? */
    uint32_t Index = Table->HashFunction(Node) % Table->Length;
    DL_List_Remove(&((Table->Array[Index]).List), &(Node->Node));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
HT_Node * HT_Node_Search(HT_Table *Table, HT_Node *Node)
{
    /* ??????,?????? */
    uint32_t Index = Table->HashFunction(Node) % Table->Length;
    /* ????????????,???????????? */
    //DL_List_Traverse_Forward(&((Table->Array[Index]).List), Current)
    DL_List_Traverse_Backward(&((Table->Array[Index]).List), Current) {
        HT_Node *Target = DL_GetOwner(HT_Node, Node, Current);
        if (Table->IsEqual(Target, Node) == true)
            return Target;
    }
    return NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef void (*HT_NodeCheck)(char *String, HT_Node *Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void HT_TableNodeCheck(HT_Table *Table, HT_NodeCheck Check)
{
    for (uint32_t Index = 0; Index < Table->Length; Index++) {
        Check("Table List:\n", NULL);
        /* ????????????,???????????? */
        uint32_t Count = 0;
        //DL_List_Traverse_Forward(&((Table->Array[Index]).List), Current)
        DL_List_Traverse_Backward(&((Table->Array[Index]).List), Current) {
            HT_Node *Target = DL_GetOwner(HT_Node, Node, Current);
            Check("    ", Target);
            if (++Count % 5 == 0)
                Check("\n", NULL);
        }
        Check("\n", NULL);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
