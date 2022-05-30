#include <stdio.h>
#include "FiniteDecisionTree.h"

void TestLoserTree(void)
{
    FiniteLoserTree LoserTree = {0};
    
    uint32_t Leave[10] = {0};
    uint32_t Node[10]  = {0};
    
    for (uint32_t Index = 0; Index < 10; Index++)
        Leave[Index] = rand() % 10;
    
    LoserTree.Leave  = Leave;
    LoserTree.Node   = Node;
    LoserTree.Length = 10;
    
    /* LoserTree */
    printf("\nNode:");
    for (uint32_t I = 0; I < 10; I++)
        printf("%2u ", LoserTree.Node[I]);
    printf("\nLeave:");
    for (uint32_t I = 0; I < 10; I++)
        printf("%2u ", LoserTree.Leave[I]);  
        
    FiniteLoserTree_Reflush(&LoserTree);
    
    for (uint32_t Index = 0; Index < 10; Index++) {
        
        /* LoserTree */
        printf("\nNode:");
        for (uint32_t I = 0; I < 10; I++)
            printf("%2u ", LoserTree.Node[I]);
        printf("\nLeave:");
        for (uint32_t I = 0; I < 10; I++)
            printf("%2u ", LoserTree.Leave[I]);        
        
        LoserTree.Leave[Index] = 0;
        
        FiniteLoserTree_Adjust(&LoserTree, Index);
    }
}

int main(int argc, char *argv[]) {
    
    TestLoserTree();
    
    
}
