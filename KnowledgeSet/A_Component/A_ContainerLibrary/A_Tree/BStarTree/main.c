/* 例: */

#include <stdio.h>
#include <windows.h>
#include "BStarTree.h"

typedef struct DataStructForTest {
	int32_t Key;
} DataTest;

void TestPrint(uint32_t CurrentNumber, uint32_t *KeySet, void **DataSet)
{
	uint32_t Index = 0;
	for (Index = 0; Index < CurrentNumber; Index++)
		printf("%d ", KeySet[Index]);
	printf("\n");
}

void TestDelete(uint32_t CurrentNumber, uint32_t *KeySet, void **DataSet)
{
	uint32_t Index = 0;
	for (Index = 0; Index < CurrentNumber; Index++) {
		printf("%d ", KeySet[Index]);
        if (DataSet != NULL)
            free(DataSet[Index]);
    }
	printf("\n");
}

uint32_t GetKey(void *Data)
{
	return (uint32_t)(((DataTest *)Data)->Key);
}

#define MAX_ELEMENT 500000
#define HALF_MAX_ELEMENT 250000

int main(int argc, uint8_t *argv[]) {

	//1.创建一个容器集合(树)
	void *Tree = malloc(BST_GetBytes_Set());
	//2.初始化一个容器集合(树)
	BST_SetTree(Tree, 10, GetKey);

	void *Queue = malloc(sizeof(void *) * MAX_ELEMENT);
	printf("-------------------------------------------------\n");

	//测试:随机生成key
	int Index = 0;
	srand(150);
	for (Index = 0; Index < MAX_ELEMENT; Index++) {
		void *Data = malloc(sizeof(DataTest));
		((DataTest *)Data)->Key = rand() % 5;
		//插入到树中
		//printf("%d ", ((DataTest *)Data)->Key);
		BST_InsertNode(Tree, Data);
	}

	printf("-------------------------------------------------\n");
	printf("%u", BST_Sequence_Traversal(Tree, TestPrint, 0));
	printf("\n----------------------------------------------------\n");
 
	Sleep(2000);

    //测试:随机生成key
    Index = 0;
    srand(100);
    for (Index = 0; Index < HALF_MAX_ELEMENT; Index++) {
        void *Data;
        //移除出树中
        BST_RemoveNode(Tree, &Data, rand() % 5);
        if (Data != NULL);
			//printf("%d ",((DataTest *)Data)->Key);

		if (0) {
        // if (1) {
        // if (Index > 0 && Index < 150) {
		    printf("-------------------------------------------------\n");
        	printf("%u", BST_Sequence_Traversal(Tree, TestPrint, 0));
        	printf("\n----------------------------------------------------\n");
		}
    }

    printf("-------------------------------------------------\n");
    printf("%u", BST_Sequence_Traversal(Tree, TestDelete, 1));
    printf("\n----------------------------------------------------\n");
    
	return 0;
}