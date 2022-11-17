#include <stdio.h>
#include <stdlib.h>
#include "SR.h"

/* Ϊ�˲����������ȼ������Ȳ㼶��������� */

/* Application */
static uint8_t Application2Parameter = 0x22;
bool Application2Loading(const void *Parameter)
{
    printf("%s:%x\n", __func__, *(uint8_t *)Parameter);
}
static uint8_t Application1Parameter = 0x21;
bool Application1Loading(const void *Parameter)
{
    printf("%s:%x\n", __func__, *(uint8_t *)Parameter);
}
/* ɢ���ڸ����ļ��е�Module�㾲̬����� */
SR_DECLARE(Application2, SR_Level_Application, 2,
           Application2Loading, &Application2Parameter, NULL);
SR_DECLARE(Application1, SR_Level_Application, 1,
           Application1Loading, &Application1Parameter, NULL);
/* Application */

/* Module */
static uint8_t Module2Parameter = 0x12;
bool Module2Loading(const void *Parameter)
{
    printf("%s:%x\n", __func__, *(uint8_t *)Parameter);
}
static uint8_t Module1Parameter = 0x11;
bool Module1Loading(const void *Parameter)
{
    printf("%s:%x\n", __func__, *(uint8_t *)Parameter);
}
/* ɢ���ڸ����ļ��е�Module�㾲̬����� */
SR_DECLARE(Module2, SR_Level_Module, 2,
           Module2Loading, &Module2Parameter, NULL);
SR_DECLARE(Module1, SR_Level_Module, 1,
           Module1Loading, &Module1Parameter, NULL);
/* Module */

/* System */
static uint8_t System2Parameter = 0x02;
bool System2Loading(const void *Parameter)
{
    printf("%s:%x\n", __func__, *(uint8_t *)Parameter);
}
static uint8_t System1Parameter = 0x01;
bool System1Loading(const void *Parameter)
{
    printf("%s:%x\n", __func__, *(uint8_t *)Parameter);
}
/* ɢ���ڸ����ļ��е�System�㾲̬����� */
SR_DECLARE(System2, SR_Level_System, 2,
           System2Loading, &System2Parameter, NULL);
SR_DECLARE(System1, SR_Level_System, 1,
           System1Loading, &System1Parameter, NULL);
/* System */



int main(int argc, char *argv[])
{
    SR_EntryLoading();
    
    /* ��������,���Է������ӽű��Ѿ������ǰ����ȼ������� */
    printf("\n\n");
    SR_EntryIterator(Entry) {
        // printf("Entry:%p\n",                Entry);
        // printf("Entry->Name:%s\n",          Entry->Name);
        // printf("Entry->Function:%p\n",      Entry->Function);
        // printf("Entry->Parameter:%p\n",     Entry->Parameter);
        // printf("Entry->Resource:%p\n",      Entry->Resource);
    }
    
	return 0;
}

