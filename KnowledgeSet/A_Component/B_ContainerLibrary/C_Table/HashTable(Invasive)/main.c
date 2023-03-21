#include <stdio.h>
#include "HashTable.h"

//哈希函数，选取部分哈希函数 

typedef struct HT_NodeDataStructForTest {
    
    uint32_t Key;
    uint32_t Data;
    
    HT_Node Node;
    
} DataTest;

uint32_t HT_Digest_Test(HT_Node *Target)
{
    uint8_t *Data = (uint8_t *)(&((HT_GetOwner(DataTest, Node, Target))->Key));
    uint32_t Length = sizeof(uint32_t);
    return Elf_HashFunction(Data, Length);
}

bool HT_Comfirm_Test(HT_Node *Node1, HT_Node *Node2)
{
    return ((HT_GetOwner(DataTest, Node, Node1))->Key ==
            (HT_GetOwner(DataTest, Node, Node2))->Key);
}

void HT_NodeCheck_Test(char *String, HT_Node *Target)
{
    printf(String);
    if (Target != NULL)
        printf("<%2d, %2d>", (HT_GetOwner(DataTest, Node, Target))->Key,
                             (HT_GetOwner(DataTest, Node, Target))->Data);
}

int main(int argc, char *argv[]) {
    
#define TestLength 10
    
    HT_Table Test_HT_Table = {0};
    HT_List  Test_HT_List[TestLength] = {0};
    
    HT_List_Reset(Test_HT_List, TestLength);
    
    HT_Table_SetZone(&Test_HT_Table, Test_HT_List, TestLength);
    HT_Table_Set(&Test_HT_Table, HT_Digest_Test, HT_Comfirm_Test);
    
    /* 随机生成100个键值对 */
    for (uint32_t Index = 0; Index < 100; Index++) {
        DataTest *Data = malloc(sizeof(DataTest));
        Data->Key  = Index;
        Data->Data = rand() % 100;
        HT_Node_Reset(&(Data->Node));
        HT_Node_Insert(&Test_HT_Table, &(Data->Node));
    }
    
    printf("\n\nHT_Node_Insert:\n\n");
    HT_TableNodeCheck(&Test_HT_Table, HT_NodeCheck_Test);
    
    /* 随机移除一半的键值对 */
    for (uint32_t Index = 0; Index < 50; Index) {
        /* 查询 */
        DataTest Data;
        Data.Key = rand() % 100;
        HT_Node_Reset(&(Data.Node));
        HT_Node *Target = HT_Node_Search(&Test_HT_Table, &(Data.Node));
        if (Target == NULL)
            continue;
        HT_Node_Remove(&Test_HT_Table, Target);
        free(HT_GetOwner(DataTest, Node, Target));
        Index++;
    }
    
    printf("\n\nHT_Node_Remove:\n\n");
    HT_TableNodeCheck(&Test_HT_Table, HT_NodeCheck_Test);
    
    return 0;
}
