#include <stdio.h>
#include "FiniteDecisionTree.h"

void TestLoserTree(void)
{
    int32_t Min = -100;
    int32_t Max = +100;
    int32_t Node[10]  = {0};
    int32_t Leave[10 + 1] = {Min, };
    for (uint32_t Index = 1; Index < 11; Index++)
        Leave[Index] = rand() % 10;
    
    FiniteLoserTree LoserTree = {0};
    LoserTree.Leave  = Leave;
    LoserTree.Node   = Node;
    LoserTree.Length = 10;
    FiniteLoserTree_Reflush(&LoserTree);
    
    for (uint32_t Index = 0; Index < 10; Index++) {
        /* LoserTree */
        // printf("\nNode:");
        // for (uint32_t I = 0; I < 10; I++)
            // printf("%2u ", LoserTree.Node[I]);
        // printf("\nLeave:");
        // for (uint32_t I = 1; I < 11; I++)
            // printf("%2u ", LoserTree.Leave[I]);
        printf("%d, ", LoserTree.Leave[LoserTree.Node[0]]);
        LoserTree.Leave[LoserTree.Node[0]] = Max;
        FiniteLoserTree_Adjust(&LoserTree, LoserTree.Node[0]);
    }
}

void TestWinnerTree(void)
{
    int32_t Min = -100;
    int32_t Max = +100;
    int32_t Node[10]  = {0};
    int32_t Leave[10 + 1] = {Min, };
    for (uint32_t Index = 1; Index < 11; Index++)
        Leave[Index] = rand() % 10;
    
    FiniteWinnerTree WinnerTree = {0};
    WinnerTree.Leave  = Leave;
    WinnerTree.Node   = Node;
    WinnerTree.Length = 10;
    FiniteWinnerTree_Reflush(&WinnerTree);
    
    for (uint32_t Index = 0; Index < 10; Index++) {
        /* WinnerTree */
        // printf("\nNode:");
        // for (uint32_t I = 0; I < 10; I++)
            // printf("%2u ", WinnerTree.Node[I]);
        // printf("\nLeave:");
        // for (uint32_t I = 1; I < 11; I++)
            // printf("%2u ", WinnerTree.Leave[I]);
        printf("%d, ", WinnerTree.Leave[WinnerTree.Node[1]]);
        WinnerTree.Leave[WinnerTree.Node[1]] = Max;
        FiniteWinnerTree_Adjust(&WinnerTree, WinnerTree.Node[1]);
    }
}


int main(int argc, char *argv[]) {
    
    printf("\n-----------------------------------\n");
    TestLoserTree();
    printf("\n-----------------------------------\n");
    TestWinnerTree();
    printf("\n-----------------------------------\n");
}
