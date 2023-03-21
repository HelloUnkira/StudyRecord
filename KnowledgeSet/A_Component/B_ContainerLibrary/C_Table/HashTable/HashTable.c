/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//内源依赖项
#include "HashTable.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void * HT_Search(HT_Table *Table, void *Node)
{
    void *Target = NULL;
    uint32_t Salt = 0;
    /* 摘取散列值 */
    uint32_t Digest = Table->Digest(Node);
    /* 散列匹配 */
    for (uint8_t FailCount = 0; FailCount <Table->FailCount; FailCount++) {
        /* 更新盐(再散列值...) */
        uint32_t HashValue = (Salt += ~Digest) % Table->Size;
        /* 获得散列目标 */
        if ((Target = Table->List[HashValue]) == NULL)
            continue;
        /* 匹配散列目标 */
        if (Table->Comfirm(Node, Target)) {
            Table->SearchHit++;
            return Target;
        }
    }
    /* 常规匹配 */
    for (uint32_t Index = 0; Index < Table->Size; Index++) {
        /* 摘取散列值 */
        uint32_t HashValue = (Index + Salt % Table->Size) % Table->Size;
        /* 获得常规目标 */
        if ((Target = Table->List[HashValue]) == NULL)
            continue;
        /* 匹配常规目标 */
        if (Table->Comfirm(Node, Target)) {
            Table->SearchMiss++;
            return Target;
        }
    }
    /* 匹配失败,散列表不存在 */
    return NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void HT_Insert(HT_Table *Table, void *Node)
{
    uint32_t Salt = 0;
    bool NotHitYet = true;
    /* 摘取散列值 */
    uint32_t Digest = Table->Digest(Node);
    /* 散列匹配 */
    if (NotHitYet)
    for (uint8_t FailCount = 0; FailCount <Table->FailCount; FailCount++) {
        /* 更新盐(再散列值...) */
        uint32_t HashValue = (Salt += ~Digest) % Table->Size;
        /* 检查散列目标 */
        if (Table->List[HashValue] == NULL) {
            Table->List[HashValue]  = Node;
            Table->Number++;
            Table->InsertHit++;
            NotHitYet = false;
            break;
        }
    }
    /* 常规匹配 */
    if (NotHitYet)
    for (uint32_t Index = 0; Index < Table->Size; Index++) {
        /* 摘取散列值 */
        uint32_t HashValue = (Index + Salt % Table->Size) % Table->Size;
        /* 检查散列目标 */
        if (Table->List[HashValue] == NULL) {
            Table->List[HashValue]  = Node;
            Table->Number++;
            Table->InsertMiss++;
            NotHitYet = false;
            break;
        }
    }
    /* 伸缩因子达到最大值,进行本地扩张 */
    if (Table->Number * 100 >= Table->Size * Table->FactorMax) {
        uint32_t Size = Table->Size;
        void   **List = Table->List;
        Table->Size   = Size * (100 + Table->FactorScale) / 100;
        Table->List   = HT_ALLOC(Table->Size * sizeof(void *));
        Table->Number = 0;
        
        for (uint32_t Index = 0; Index < Table->Size; Table->List[Index] = NULL, Index++);
        for (uint32_t Index = 0; Index < Size; Index++)
            if (List[Index] != NULL)
                HT_Insert(Table, List[Index]);
        HT_FREE(List);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void HT_Remove(HT_Table *Table, void *Node)
{
    uint32_t Salt = 0;
    bool NotHitYet = true;
    /* 摘取散列值 */
    uint32_t Digest = Table->Digest(Node);
    /* 散列匹配 */
    if (NotHitYet)
    for (uint8_t FailCount = 0; FailCount <Table->FailCount; FailCount++) {
        /* 更新盐(再散列值...) */
        uint32_t HashValue = (Salt += ~Digest) % Table->Size;
        /* 检查散列目标 */
        if (Table->List[HashValue] == Node) {
            Table->List[HashValue]  = NULL;
            Table->Number--;
            Table->RemoveHit++;
            NotHitYet = false;
            break;
        }
    }
    /* 常规匹配 */
    if (NotHitYet)
    for (uint32_t Index = 0; Index < Table->Size; Index++) {
        /* 摘取散列值 */
        uint32_t HashValue = (Index + Salt % Table->Size) % Table->Size;
        /* 检查散列目标 */
        if (Table->List[HashValue] == Node) {
            Table->List[HashValue]  = NULL;
            Table->Number--;
            Table->RemoveMiss++;
            NotHitYet = false;
            break;
        }
    }
    /* 伸缩因子达到最大值,进行本地收缩 */
    if (Table->Number * 100 <= Table->Size * Table->FactorMin) {
        uint32_t Size = Table->Size;
        void   **List = Table->List;
        Table->Size   = Size * (100 - Table->FactorScale) / 100;
        Table->List   = HT_ALLOC(Table->Size * sizeof(void *));
        Table->Number = 0;
        
        for (uint32_t Index = 0; Index < Table->Size; Table->List[Index] = NULL, Index++);
        for (uint32_t Index = 0; Index < Size; Index++)
            if (List[Index] != NULL)
                HT_Insert(Table, List[Index]);
        HT_FREE(List);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
