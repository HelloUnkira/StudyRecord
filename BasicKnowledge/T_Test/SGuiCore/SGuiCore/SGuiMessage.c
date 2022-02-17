/*ʵ��Ŀ��:
 *��Ϣ����,���ڼ�¼��ͬ������Ϣ
 *���ֲ�ͬ������Ϣ����Ϊ���ⲿ��
 *�����������������һ�����ɻ��Ĳֿ�
 */

/* ����Ψһ������ */
#include "SGuiCore.h"

typedef struct SimpleGuiMessageQueue {
    uint32_t  index;    //���б��
    uint32_t  number;   //������Ϣ������
    uint32_t  length;   //������Ϣ������
    void    **source;   //��Ϣ��
} SGuiMessageQueue;

/* Ψһ���м�ʵ�� */
static SGuiSList head = {0};
static SGuiSList tail = {0};

/* ����һ����Ϣ���� */
uint32_t SGuiMessageQueueTake(uint32_t length)
{
    SGuiSyncPrimitOptSync(true);

    uint32_t index = 0;
    /* 1.����һ����Ϣ���� */
    SGuiMessageQueue *unit = SGUIMALLOC(sizeof(SGuiMessageQueue));
    unit->number = 0;
    unit->length = length;
    unit->queue  = SGUIMALLOC(sizeof(void *) * queue->length);
    /* ��ʼ����Ϣ���� */
    for (index = 0; index < length; index++)
        (unit->queue)[index] = NULL;

    /* 2.�����е�Ԫ����С�������������� */
    SGuiSNodeUnitAdd(&head, &tail, unit);

    SGuiSyncPrimitOptSync(false);

    return unit->index;
}

/* �ͷ�һ����Ϣ���� */
uint32_t SGuiMessageQueueGive(uint32_t queue)
{
    SGuiSyncPrimitOptSync(true);

    uint32_t index = 0, value = 0;
    /* 1.�������ɵĿ��е�Ԫ */
    SGuiMessageQueue *unit = SGuiSNodeUnitSearch(&head, &tail, queue);
    /* ��һ���򵥵ļ��,�����Ȼ������Ϣ,�����ͷ� */
    for (index = 0; index < unit->length; index++)
        if ((unit->queue)[index] != NULL) {
            value = -1;
            unit  = NULL;
            break;
        }
    /* 2.�ͷ�һ�����е�Ԫ */
    if (unit == NULL);
    if (unit != NULL) {
        /* �������ͷ�˳���෴ */
        /* ��Ȼunit����Ұָ��,�����ǲ��������ɻ��� */
        SGUIFREE(unit->queue);
        SGUIFREE(unit);
        /* �����е�Ԫ�Ƴ�������С���������� */
        SGuiSNodeUnitRemove(&head, &tail, unit);
    }

    SGuiSyncPrimitOptSync(false);

    return value;
}

/* ��Ϣ�������һ����Ϣ */
void SGuiMessageQueueAdd(uint32_t queue, SGuiMessage message)
{
    SGuiSyncPrimitOptSync(true);

    uint32_t index = 0;
    /* 1.�������ɵĿ��е�Ԫ */
    SGuiMessageQueue *unit = SGuiSNodeUnitSearch(&head, &tail, queue);
    if (unit == NULL);
    if (unit != NULL) {
        if (unit->number < unit->length) {
            for (index = 0; index < unit->length; index++) {
                //��ָ���ڷǿ�ָ��֮��
                if ((unit->queue)[index] == NULL) {

                    

                    break;
                }

                if ((unit->queue)[index]->type != message.type)
                    continue;
                if (message.merge == NULL)
                    continue;

                SGUIMSGPARAMTYPE parameter[SGUIMSGPARAMLEN] = {0};

                message.merge(parameter,  message->parameter,
                             (unit->queue)[index]->parameter);
                
                for (index = 0; index < SGUIMSGPARAMLEN; index++)
                    (unit->queue)[index]->parameter[index] = parameter[index];
                    
                    break;
            }
        }
    }

    SGuiSyncPrimitOptSync(false);
}





