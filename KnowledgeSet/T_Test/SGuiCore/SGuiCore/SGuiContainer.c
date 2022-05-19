/* ʵ��Ŀ��:
 * ��Ƽ�Ҫ��������������
 * ������һ���ؼ�����
 * ���ڹ�����ӿؼ�
 */

/* ����Ψһ������ */
#include "SGuiCore.h"

/* ��Դ�������������������� */
/* ����Խ��,����ά���ռ�ԽС */
/* ����ԽС,�ڴ�������Խ�� */
#define SGUISCALFACTOR    5

typedef struct SimpleGuiContainer {
    uint32_t   index;  //��ǰ��Ԫ���
    uint32_t   number; //�ؼ��������
    uint32_t  *source; //�ؼ��������
} SGuiContainer;

static SGuiSList head = {0};
static SGuiSList tail = {0};

/* ���һ������ */
uint32_t SGuiContainerTake(void)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0;
    /* 1.����һ�����е�Ԫ */
    SGuiContainer *unit = SGUIMALLOC(sizeof(SGuiContainer));
    unit->number = SGUISCALFACTOR;
    unit->source = SGUIMALLOC(sizeof(uint32_t) * (unit->number));
    /* ��ʼ�����е�Ԫ */
    for (index = 0; index < unit->number; index++)
        (unit->source)[index] = SGUIINVALIDHANDLE;

    /* 2.�����е�Ԫ����С�������������� */
    SGuiSNodeUnitAdd(&head, &tail, unit);

    SGUISYNCPRIMITOPTSYNCMUTEX(false);

    return unit->index;
}

/* �ͷ�һ������ */
void SGuiContainerGive(uint32_t container)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    /* 1.�������ɵĿ��е�Ԫ */
    SGuiContainer *unit = SGuiSNodeUnitSearch(&head, &tail, container);
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
}

/* �������һ���ӿؼ� */
void SGuiContainerAdd(uint32_t container, uint32_t handle)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0;
    /* 1.�������ɵĿ��е�Ԫ */
    SGuiContainer *unit = SGuiSNodeUnitSearch(&head, &tail, container);
    /* 2.���е�Ԫ�����Դ */
    if (unit == NULL);
    if (unit != NULL) {
        /* ������������,������ӵ�ĩβ */
        for (index = 0; index < unit->number; index++)
            if ((unit->source)[index] == SGUIINVALIDHANDLE)
                break;
    }
    if (unit != NULL) {
        /* �����������ĩβ */
        if (index == unit->number) {
            /* ����һ����չ�������� */
            uint32_t  number = unit->number + SGUISCALFACTOR;
            uint32_t *source = SGUIMALLOC(sizeof(uint32_t) * number);
            /* ����ԭ���������� */
            for (index = 0; index < unit->number; index++)
                source[index] = (unit->source)[index];
            /* ��ʼ����չ�˲� */
             for (index = unit->number; index < number; index++)
                source[index] = SGUIINVALIDHANDLE;
            /* �ͷ�ԭ���������� */
            SGUIFREE(unit->source);
            /*  */
            index = unit->number;
            /* ������չ���� */
            unit->source = source;
            unit->number = number;
        }
        /* �����ӿؼ� */
        (unit->source)[index] = handle;
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}

/* �����Ƴ�һ���ӿؼ� */
void SGuiContainerRemove(uint32_t container, uint32_t handle)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0;
    /* 1.�������ɵĿ��е�Ԫ */
    SGuiContainer *unit = SGuiSNodeUnitSearch(&head, &tail, container);
    /* 2.���е�Ԫ�Ƴ���Դ */
    if (unit == NULL);
    if (unit != NULL) {
        /* ������������,Ѱ�ҵ�Ŀ����Դ */
        for (index = 0; index < unit->number; index++)
            if ((unit->source)[index] == handle) {
                (unit->source)[index]  = SGUIINVALIDHANDLE;
                break;
            }
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}

/* ��ȡ�������� */
void SGUIContainerGet(uint32_t container, uint32_t **source, uint32_t *number)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0;
    /* 1.�������ɵĿ��е�Ԫ */
    SGuiContainer *unit = SGuiSNodeUnitSearch(&head, &tail, container);
    /* 2.���е�Ԫ��ȡ��Դ */
    if (unit == NULL);
    if (unit != NULL) {
        //*source = unit->source;
        //*number = unit->number;
        /* ע��:ģ����Դ���ڲ�ȫ�ֱ���,���Բ��ɹ������ */
        *source = SGUIMALLOC(sizeof(uint32_t) * (unit->number));
        *number = unit->number;
        /* ��Դ��������� */
        for (index = 0; index < unit->number; index++)
            (*source)[index] = (unit->source)[index];
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}
