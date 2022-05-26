/* ����Ψһ������ */
#include <stdio.h>
#include <stdlib.h>
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#include "SGui_Kernel.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void SGui_Timer_Handler_Test(void)
{
    static uint64_t CurrentTimer = 0;

    if (CurrentTimer++ % 1000 == 0)
        printf("1s cb\n");
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void SGui_Timer_Test(void)
{
    SGui_TimerConfigure(SGui_Timer_Handler_Test);
    SGui_TimerInit();
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void SGui_Handle_Test(void)
{
    #define SGUI_HANDLE_LENGTH_TEST  10
    uint32_t  Index = 0;
    uint32_t  Array[SGUI_HANDLE_LENGTH_TEST] = {0};
    uint32_t *Pointer = NULL;

    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++) {
        Array[Index] = SGui_HandleTake();
        SGui_HandleSourceSet(Array[Index], &(Array[Index]));
    }

    /* ���� */
    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++) {
        uint32_t Temp_Index = random() % SGUI_HANDLE_LENGTH_TEST;
        uint32_t Temp = Array[Index];
        Array[Index] = Array[Temp_Index];
        Array[Temp_Index] = Temp;
    }

    printf("Array:");
    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++)
        printf("%d ", Array[Index]);
    printf("\n");

    /* ��ȡ�����Դ */
    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++) {
        Pointer = SGui_HandleSourceGet(Array[Index]);
        printf("Array[%d]:%d\n", Array[Index], *Pointer);
    }

    /* �ͷ� */
    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++)
        SGui_HandleGive(Array[Index]);

    /* �ٴλ�ȡ,��֤ */
    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++) {
        Array[Index] = SGui_HandleTake();
        SGui_HandleSourceSet(Array[Index], &(Array[Index]));
    }

    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++) {
        Pointer = SGui_HandleSourceGet(Array[Index]);
        printf("Array[%d]:%d\n", Array[Index], *Pointer);
    }

    /* �ͷ� */
    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++)
        SGui_HandleGive(Array[Index]);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define SGuiClipRegionTest(Clip)        \
    printf("Left:%d\n",   (Clip).LU.X); \
    printf("Up:%d\n",     (Clip).LU.Y); \
    printf("Right:%d\n",  (Clip).RB.X); \
    printf("Bottom:%d\n", (Clip).RB.Y); \
    printf("\n");
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void SGui_ClipRegion1_Test(void)
{
    /* ������������� */
    SGui_Area_Define(Clip0);
    SGui_Area_Define(Clip1);
    SGui_Area_Define(Clip2);
    SGui_Area_Define(Clip3);

    SGUI_Area_Set(Clip1, 100, 100, 300, 300);
    SGUI_Area_Set(Clip2, 150, 150, 250, 250);
    SGUI_Area_Set(Clip3, 150, 100, 350, 300);

    SGuiClipRegionTest(Clip1);
    SGuiClipRegionTest(Clip2);
    SGuiClipRegionTest(Clip3);

    /* ���������򽻼� */
    printf("Rect1 & Rect2:\n");
    Clip0 = SGui_ClipRectAnd(Clip1, Clip2);
    SGuiClipRegionTest(Clip0);

    /* ���������򲢼� */
    printf("Rect1 | Rect2:\n");
    Clip0 = SGui_ClipRectOr(Clip1, Clip2);
    SGuiClipRegionTest(Clip0);

    /* �������������� */
    printf("Rect1 U Rect2:\n");
    Clip0 = SGui_ClipRectUnion(Clip1, Clip3);
    SGuiClipRegionTest(Clip0);

    /* �����������ϵ���1:(�����) */
    printf("Rect1 have Rect2:%d\n", SGui_ClipRectCheck1(Clip2, Clip1));

    /* �������������(Ҫ��:������ڽ���) */
    SGui_Area Result[4] = {0};
    int Index = 0;
    int Count = SGui_ClipRectSub(Result, Clip1, Clip2);
    
    printf("Rect1 - Rect2:\n");
    for (int Index = 0; Index < Count; Index++) {
        SGuiClipRegionTest(Result[Index]);
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void SGui_ClipRegion2_Test(void)
{
    SGui_Area *Slave = NULL;
    uint32_t Length = 0;
    uint32_t Index  = 0;

    SGui_Area_Define(master);
    SGUI_Area_Set(master, 0, 0, 400, 400);
    SGui_Area_Define(Slave1);
    SGUI_Area_Set(Slave1, 150, 150, 250, 250);
    SGui_Area_Define(Slave2);
    SGUI_Area_Set(Slave2, 50, 50 , 100, 100);
    /* 1.���һ�������򼯺� */
    uint32_t Rects = SGui_ClipRectsTake();
    /* 2.Ϊ�����򼯺��������� */
    SGui_ClipRectsMasterSet(Rects, master);
    /* 3.�����򼯺���Ӽ����� */
    SGui_ClipRectsSlaveAdd(Rects, master);
    /* 4.�����򼯺��Ƴ������� */
    SGui_ClipRectsSlaveRemove(Rects, Slave1);
    /* ��ȡ�����򼯺� */
    printf("Current Rects:\n");
    SGui_ClipRectsSlaveLengthGet(Rects, &Length);
    Slave = SGUI_ALLOC(sizeof(SGui_Area) * Length);
    SGui_ClipRectsSlaveSetGet(Rects, Slave);
    if (Slave != NULL)
        for (Index = 0; Index < Length; Index++)
            if (SGui_ClipRectIsVaild(Slave[Index]) == true) {
                printf("Index:%d\n", Index);
                SGuiClipRegionTest(Slave[Index]);
            }
    SGUI_FREE(Slave);
    /* 4.�����򼯺��Ƴ������� */
    SGui_ClipRectsSlaveRemove(Rects, Slave2);
    /* ��ȡ�����򼯺� */
    printf("Current Rects:\n");
    SGui_ClipRectsSlaveLengthGet(Rects, &Length);
    Slave = SGUI_ALLOC(sizeof(SGui_Area) * Length);
    SGui_ClipRectsSlaveSetGet(Rects, Slave);
    if (Slave != NULL)
        for (Index = 0; Index < Length; Index++)
            if (SGui_ClipRectIsVaild(Slave[Index]) == true) {
                printf("Index:%d\n", Index);
                SGuiClipRegionTest(Slave[Index]);
            }
    SGUI_FREE(Slave);
    /* 3.�����򼯺���Ӽ����� */
    SGui_ClipRectsSlaveAdd(Rects, Slave1);
    /* ��ȡ�����򼯺� */
    printf("Current Rects:\n");
    SGui_ClipRectsSlaveLengthGet(Rects, &Length);
    Slave = SGUI_ALLOC(sizeof(SGui_Area) * Length);
    SGui_ClipRectsSlaveSetGet(Rects, Slave);
    if (Slave != NULL)
        for (Index = 0; Index < Length; Index++)
            if (SGui_ClipRectIsVaild(Slave[Index]) == true) {
                printf("Index:%d\n", Index);
                SGuiClipRegionTest(Slave[Index]);
            }
    SGUI_FREE(Slave);
    /* 3.�����򼯺���Ӽ����� */
    SGui_ClipRectsSlaveAdd(Rects, Slave2);
    /* ��ȡ�����򼯺� */
    printf("Current Rects:\n");
    SGui_ClipRectsSlaveLengthGet(Rects, &Length);
    Slave = SGUI_ALLOC(sizeof(SGui_Area) * Length);
    SGui_ClipRectsSlaveSetGet(Rects, Slave);
    if (Slave != NULL)
        for (Index = 0; Index < Length; Index++)
            if (SGui_ClipRectIsVaild(Slave[Index]) == true) {
                printf("Index:%d\n", Index);
                SGuiClipRegionTest(Slave[Index]);
            }
    SGUI_FREE(Slave);
    /* �ͷ�һ�������򼯺� */
    SGui_ClipRectsGive(Rects);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void SGui_Container_Test(void)
{
    #define SGUI_SOURCE_LENGTH_TEST  10
    uint32_t Index = 0;
    uint32_t Array[SGUI_SOURCE_LENGTH_TEST]  = {0,1,2,3,4,5,6,7,8,9};
    
    /* ���һ������ */
    uint32_t Container = SGui_ContainerTake();
    /* ����Ĳ���ʱ,���ֱ���õ������±���� */
    uint32_t *Source = NULL;
    uint32_t  Number = 0;
    
    for (Index = 0; Index < SGUI_SOURCE_LENGTH_TEST; Index++)
        SGui_ContainerAdd(Container, Index);
    
    /* ���� */
    for (Index = 0; Index < SGUI_SOURCE_LENGTH_TEST; Index++) {
        uint32_t TempIndex = random() % SGUI_SOURCE_LENGTH_TEST;
        uint32_t Temp = Array[Index];
        Array[Index] = Array[TempIndex];
        Array[TempIndex] = Temp;
    }

    printf("Array:");
    for (Index = 0; Index < SGUI_SOURCE_LENGTH_TEST; Index++)
        printf("%d ", Array[Index]);
    printf("\n");

    /* ��ȡ��Դ */
    SGui_ContainerLengthGet(Container, &Number);
    Source = SGUI_ALLOC(sizeof(uint32_t) * Number);
    SGui_ContainerSetGet(Container, Source);

    for (Index = 0; Index < SGUI_SOURCE_LENGTH_TEST; Index++)
        printf("Array[%d]:%d\n", Source[Index], Array[Source[Index]]);

    printf("Number:%d\n", Number);
    for (Index = 0; Index < SGUI_SOURCE_LENGTH_TEST; Index++)
        printf("Source[%d]:%d\n", Index, Source[Index]);

    for (Index = 0; Index < SGUI_SOURCE_LENGTH_TEST; Index++)
        SGui_ContainerRemove(Container, Source[Index]);
    SGUI_FREE(Source);

    /* ��ȡ��Դ */
    SGui_ContainerLengthGet(Container, &Number);
    Source = SGUI_ALLOC(sizeof(uint32_t) * Number);
    SGui_ContainerSetGet(Container, Source);
    
    for (Index = 0; Index < SGUI_SOURCE_LENGTH_TEST; Index++)
        printf("Source[%d]:%d\n", Index, Source[Index]);
    SGUI_FREE(Source);
    /* �ͷ� */
    SGui_ContainerGive(Container);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int main(int argc, char *argv[])
{
    // SGui_Timer_Test();
    // while (1);
    
    // SGui_Handle_Test();
    // SGui_ClipRegion1_Test();
    // SGui_ClipRegion2_Test();
    // SGui_Container_Test();

    return 0;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
