#ifndef SGUIPORT_H
#define SGUIPORT_H
/* ��׼������ */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ʵ��Ŀ��:
 * ��ƻ����ؼ���ƽ̨�ӿ�
 */

/* ��ƽ̨֧��:1����,0���� */
#define SGUI_USE_LINUX      1
#define SGUI_USE_XXXX       0

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* �ڴ�����ز��� */
#define SGUIMEMORYCHECK             1   /* �����ڴ�����ز���   */
#define SGUIMEMORYCHECKFILELEN      20  /* file����,����ʱ�����ȡ */
#define SGUIMEMORYCHECKLEN          100 /* ����ض�����������Ϣ */
#define SGUIMEMORYCHECKAUTO         0   /* ÿ����������������Ϣ����һ����ʾ */
//��¼����ͣ����(Ĭ�ϲ�����)
void SGuiMemoryCheckStart(void);
void SGuiMemoryCheckEnd(void);
//��¼������
void SGuiMemoryCheckReset(void);
//�ڴ����������
void SGuiMemoryCheck(void);
//�����ڴ���ͷ��ڴ�
#if SGUIMEMORYCHECK == 1
#define SGUIMALLOC(size)  SGuiMalloc(size,  __FILE__, __LINE__)
#define SGUIFREE(pointer) SGuiFree(pointer, __FILE__, __LINE__)
void * SGuiMalloc(uint32_t size, int8_t *file, uint32_t line);
void SGuiFree(void *pointer, int8_t *file, uint32_t line);
#endif
#if SGUIMEMORYCHECK == 0
#define SGUIMALLOC(size)  SGuiMalloc(size)
#define SGUIFREE(pointer) SGuiFree(pointer)
void * SGuiMalloc(uint32_t size);
void SGuiFree(void *pointer);
#endif
//����ר��Ϊ�ڴ�����׼����ͬ��ԭ��,����ģ�鲻��ʹ��,��ʹ������Ľӿ�
void SGuiMemoryCheckOptSync(bool StartOrEnd);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* ͬ��ԭ�� */
/* ����ͬ��ԭ����޶������������Ҫͬ�����¶�ӦԴ */
typedef struct SimpleGuiSynchronizationPrimitive {
    /*��union�е�˳���Ӧ: 1 ~ n */
#define SGUIMUTEX       1
#define SGUISEMAPHONE   2
    /* ԭ������,�궨������ */
    uint32_t type;
    /* ԭ��֧�ּ��� */
    union {
        /* ��һ������ΪĬ�ϱ������� */
        void *primitive;
        /* ֧�ֵ�ͬ��ԭ�� */
        void *mutex;
        void *semaphone;
        /* ... */
    };
    /* ͬ��ԭ�ﾲ̬������Դ�� */
    void *resource;
} SGuiSyncPrimit;

//����ָ�����͵�ͬ��ԭ��
SGuiSyncPrimit * SGuiSyncPrimitCreate(uint32_t type);
//����ָ�����͵�ͬ��ԭ��
void SGuiSyncPrimitDestory(SGuiSyncPrimit *primit);
//��ȡͬ��ԭ�ﱣ������Դʹ��Ȩ��
void * SGuiSyncPrimitTakeResource(SGuiSyncPrimit *primit);
//�黹ͬ��ԭ�ﱣ������Դʹ��Ȩ��
void SGuiSyncPrimitGiveResource(SGuiSyncPrimit *primit);
//����ͬ��ԭ�����豣������Դ(��ѡ��)
void SGuiSyncPrimitSetResource(SGuiSyncPrimit *primit, void *resource);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//��ʱ������
#if SGUI_USE_LINUX

typedef void (*SGuiTimerMSHandler)(void);
void SGuiTimerCfg(SGuiTimerMSHandler handler);
void SGuiTimerInit(void);

#else

#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//Lcd����
#if SGUI_USE_LINUX
void SGuiLcdInit(void);
void SGuiLcdDeinit(void);
uint32_t * SGuiLcdInstance(void);
uint32_t SGuiLcdWidth(void);
uint32_t SGuiLcdHeight(void);
#else

#endif

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#endif
