#ifndef SGUICOMMON_H
#define SGUICOMMON_H
/* ��׼������ */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
/* ƽ̨���� */
#include "SGuiPort.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* ����������(�ɸ�дΪ��̬������������Ч��) */
#include "SGuiIterator.h"
/* ���������������Ӧ��Դ���Ա�������ȡ��module�� */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* ��һ�����е�Ԫ����ڵ�,�����뵽����(ʹ����С������) */
void SGuiSNodeUnitAdd(SGuiSNode *head, SGuiSNode *tail, void *unit);
/* ��һ�����е�Ԫ�Ƴ��ڵ�,���Ƴ�������(ʹ����С������) */
void SGuiSNodeUnitRemove(SGuiSNode *head, SGuiSNode *tail, void *unit);
/* ��һ�����е�ԪѰ�ҵ�(ʹ����С������) */
void * SGuiSNodeUnitSearch(SGuiSNode *head, SGuiSNode *tail, uint32_t index);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* ͬ��ԭ�ﻥ����Ҫ���뵽Դ����ʹ�õı��� */
/* ͬ��ԭ��ĳ�ʼ��,����,��ֹ,��ͳһ�ӵ��˴� */
static bool             SGuiSyncPrimitOptSyncMutex_Init     = false;
static bool             SGuiSyncPrimitOptSyncMutex_DeInit   = false;
static uint8_t          SGuiSyncPrimitOptSyncMutex_Variable = 0xA5;
static SGuiSyncPrimit  *SGuiSyncPrimitOptSyncMutex_Mutex    = NULL;
/* ͬ��ԭ�Ｏ�ɻ������ӿ� */
void SGuiSyncPrimitOptSyncMutex(SGuiSyncPrimit **Mutex,
                                void            *Variable,
                                bool            *Init,
                                bool            *DeInit,
                                bool             StartOrEnd);
/* ����ͬ��ԭ��(true:���ñ���,false:���ñ���) */
#define SGUISYNCPRIMITOPTSYNCMUTEX(StartOrEnd)                              \
        SGuiSyncPrimitOptSyncMutex(&SGuiSyncPrimitOptSyncMutex_Mutex,       \
                                   &SGuiSyncPrimitOptSyncMutex_Variable,    \
                                   &SGuiSyncPrimitOptSyncMutex_Init,        \
                                   &SGuiSyncPrimitOptSyncMutex_DeInit,      \
                                    StartOrEnd)
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*ʵ��Ŀ��:
 *һЩͨ�õĺ����û���������
 */
#define SGUIMIN(data1, data2) ((data1) <= (data2) ? (data1) : (data2))
#define SGUIMAX(data1, data2) ((data1) >= (data2) ? (data1) : (data2))
#define SGUIDIS(data1, data2) (SGUIMAX(data1, data2) - SGUIMIN(data1, data2))
#endif
