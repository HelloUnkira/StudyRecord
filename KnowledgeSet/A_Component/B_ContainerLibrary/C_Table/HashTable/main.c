#include <stdio.h>
#include "HashTable.h"

//哈希函数,选取部分哈希函数

typedef struct HT_NodeDataStructForTest {
    
    uint32_t Key;
    uint32_t Data;
    
} DataTest;

uint32_t HT_Test_Digest(void *Parameter)
{
    uint8_t *Data = (uint8_t *)&(((DataTest *)Parameter)->Key);
    uint32_t Length = sizeof(uint32_t);
    return Elf_HashFunction(Data, Length);
}

bool HT_Test_Comfirm(void *Parameter, void *Target)
{
    return ((DataTest *)Parameter)->Key == ((DataTest *)Target)->Key;
}

int main(int argc, char* argv[])
{
    HT_Table Test_HT_Table = {0};
    HT_Configure(&Test_HT_Table, HT_Test_Digest, HT_Test_Comfirm, 5, 50, 70, 30);
    HT_Ready(&Test_HT_Table, 20);
    
    /* 随机生成100个键值对 */
    for (uint32_t Index = 0; Index < 100; Index++) {
        DataTest *Data = malloc(sizeof(DataTest));
        Data->Key  = Index;
        Data->Data = rand() % 100;
        
        HT_Insert(&Test_HT_Table, Data);
        
        void *Target = HT_Search(&Test_HT_Table, Data);
    }
    
    printf("SearchHit:%d;SearchMiss:%d\n"
           "InsertHit:%d;InsertMiss:%d\n"
           "RemoveHit:%d;RemoveMiss:%d\n\n",
            Test_HT_Table.SearchHit,
            Test_HT_Table.SearchMiss,
            Test_HT_Table.InsertHit,
            Test_HT_Table.InsertMiss,
            Test_HT_Table.RemoveHit,
            Test_HT_Table.RemoveMiss);
    
    /* 随机移除一半的键值对 */
    for (uint32_t Index = 0; Index < 50; Index) {
        /* 查询 */
        DataTest Target = {.Key = rand() % 100,};
        DataTest *Data = HT_Search(&Test_HT_Table, &Target);
        if (Data == NULL)
            continue;
        HT_Remove(&Test_HT_Table, Data);
        free(Data);
        Index++;
    }
    
    printf("SearchHit:%d;SearchMiss:%d\n"
           "InsertHit:%d;InsertMiss:%d\n"
           "RemoveHit:%d;RemoveMiss:%d\n\n",
            Test_HT_Table.SearchHit,
            Test_HT_Table.SearchMiss,
            Test_HT_Table.InsertHit,
            Test_HT_Table.InsertMiss,
            Test_HT_Table.RemoveHit,
            Test_HT_Table.RemoveMiss);
    
    HT_Clean(&Test_HT_Table);
    return 0;
}
