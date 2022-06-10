/* 例: */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "CommonHandle.h"

int main(int argc, char *argv[])
{
#define HANDLE_ALL  10
    uint32_t  Index = 0;
    uint32_t  Array[HANDLE_ALL] = {0};
    uint32_t *Pointer = NULL;

    for (Index = 0; Index < HANDLE_ALL; Index++) {
        Array[Index] = CommonHandleTake();
        CommonHandleSet(Array[Index], &(Array[Index]));
    }

    /* 打乱 */
    for (Index = 0; Index < HANDLE_ALL; Index++) {
        uint32_t temp_Index = rand() % HANDLE_ALL;
        uint32_t temp = Array[Index];
        Array[Index] = Array[temp_Index];
        Array[temp_Index] = temp;
    }

    printf("Array:");
    for (Index = 0; Index < HANDLE_ALL; Index++)
        printf("%d ", Array[Index]);
    printf("\n");

    /* 获取句柄资源 */
    for (Index = 0; Index < HANDLE_ALL; Index++) {
        Pointer = CommonHandleGet(Array[Index]);
        printf("Array[%d]:%d\n", Array[Index], *Pointer);
    }

    /* 释放 */
    for (Index = 0; Index < HANDLE_ALL; Index++)
        CommonHandleGive(Array[Index]);

    /* 再次获取,验证 */
    for (Index = 0; Index < HANDLE_ALL; Index++) {
        Array[Index] = CommonHandleTake();
        CommonHandleSet(Array[Index], &(Array[Index]));
    }

    for (Index = 0; Index < HANDLE_ALL; Index++) {
        Pointer = CommonHandleGet(Array[Index]);
        printf("Array[%d]:%d\n", Array[Index], *Pointer);
    }

    /* 释放 */
    for (Index = 0; Index < HANDLE_ALL; Index++)
        CommonHandleGive(Array[Index]);
    
    return 0;
}
