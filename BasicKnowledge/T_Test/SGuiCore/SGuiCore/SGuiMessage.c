/*ʵ��Ŀ��:
 *��Ϣ����,���ڼ�¼��ͬ������Ϣ
 *���ֲ�ͬ������Ϣ����Ϊ���ⲿ��
 *�����������������һ�����ɻ��Ĳֿ�
 */

/* ����Ψһ������ */
#include "SGuiCore.h"

typedef struct SimpleGuiMessageSet {
    uint32_t  index;    //���ϱ��
    uint32_t  length;   //������Ϣ������
    void    **source;   //������Ϣ��ʵ��
} SGuiMessageSet;

/* Ψһ���ϼ�ʵ�� */
static SGuiSList head = {0};
static SGuiSList tail = {0};

/* ����һ����Ϣ���� */
uint32_t SGuiMessageSetTake(uint32_t length)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0;
    /* 1.����һ����Ϣ���� */
    SGuiMessageSet *unit = SGUIMALLOC(sizeof(SGuiMessageSet));
    unit->length = length;
    unit->source = SGUIMALLOC(sizeof(void *) * unit->length);
    /* ��ʼ����Ϣ���� */
    for (index = 0; index < length; index++)
        (unit->source)[index] = NULL;

    /* 2.�����е�Ԫ����С�������������� */
    SGuiSNodeUnitAdd(&head, &tail, unit);

    SGUISYNCPRIMITOPTSYNCMUTEX(false);

    return unit->index;
}

/* �ͷ�һ����Ϣ���� */
uint32_t SGuiMessageSetGive(uint32_t set)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0, value = 0;
    /* 1.�������ɵĿ��е�Ԫ */
    SGuiMessageSet *unit = SGuiSNodeUnitSearch(&head, &tail, set);
    /* ��һ���򵥵ļ��,�����Ȼ������Ϣ,�����ͷ� */
    for (index = 0; index < unit->length; index++)
        if ((unit->source)[index] != NULL) {
            value = -1;
            unit  = NULL;
            break;
        }
    /* 2.�ͷ�һ�����е�Ԫ */
    if (unit == NULL);
    if (unit != NULL) {
        /* �������ͷ�˳���෴ */
        /* ��Ȼunit����Ұָ��,�����ǲ��������ɻ��� */
        SGUIFREE(unit->source);
        SGUIFREE(unit);
        /* �����е�Ԫ�Ƴ�������С���������� */
        SGuiSNodeUnitRemove(&head, &tail, unit);
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);

    return value;
}

/* ��Ϣ�������һ����Ϣ(����) */
bool SGuiMessageSetAdd(uint32_t set, SGuiMessage message)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0, value = -1;
    /* 1.�������ɵĿ��е�Ԫ */
    SGuiMessageSet *unit = SGuiSNodeUnitSearch(&head, &tail, set);
    if (unit == NULL);
    if (unit != NULL)
        for (index = 0; index < unit->length; index++)
            //�ҵ�һ������λ�ý���ϢͶ��
            if ((unit->source)[index] == NULL) {
                (unit->source)[index]  = SGUIMALLOC(sizeof(SGuiMessage));
                //��Ϣд�뵽ָ��λ��
                *((SGuiMessage *)((unit->source)[index])) = message;
                value = 0;
                break;
            }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);

    return value;
}

/* ��Ϣ�����Ƴ�һ����Ϣ(����) */
bool SGuiMessageSetRemove(uint32_t set, SGuiMessage *message)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0, value = -1;
    /* 1.�������ɵĿ��е�Ԫ */
    SGuiMessageSet *unit = SGuiSNodeUnitSearch(&head, &tail, set);
    if (unit == NULL);
    if (unit != NULL)
        for (index = 0; index < unit->length; index++)
            //�ҵ�һ������λ�ý���ϢͶ��
            if ((unit->source)[index] != NULL) {
                //��Ϣ��ȡ��ָ��λ��
                *message = *((SGuiMessage *)((unit->source)[index]));
                SGUIFREE((unit->source)[index]);
                (unit->source)[index] = NULL;
                value = 0;
                break;
            }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);

    return value;
}
