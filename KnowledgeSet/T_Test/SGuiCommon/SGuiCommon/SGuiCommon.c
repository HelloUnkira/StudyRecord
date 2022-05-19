
#include "SGuiPort.h"
#include "SGuiCommon.h"

/* ʵ��Ŀ��:
 * ���ƻ�,ȫ�ֻ��ĸ����Ż�����
 * ����Ϊ�����ṩĳЩ��Դ
 * ��������������������Ż�����
 */

/* ���������������ƻ�(���ڼ򻯱�Ե�߼�) */
/* ��Ա���������index��������С�����ɲ��� */
/* (ע��:��������ƽ̨�����Լ�ģ������): */
/* ���ݵ�ǰʹ��������������һ��� */
/* Ҫ��ÿһ��ʹ�øù��ܵ�ģ��,unit��һ��Ԫ������������: */
/* uint32_t index;  //��ǰ��Ԫ��� */
#define SGUISNODESPECIALDESIGN
#ifdef  SGUISNODESPECIALDESIGN
/* ��һ�����е�Ԫ����ڵ�,�����뵽����(ʹ����С������) */
void SGuiSNodeUnitAdd(SGuiSNode *head, SGuiSNode *tail, void *unit)
{
    /* 1.���ɽڵ�ռ� */
    SGuiSNode *node = SGUIMALLOC(sizeof(SGuiSNode));
    /* 2.��ʼ���ռ� */
    SNODE_RESET(node);
    SNODE_SET(node, unit);
    /* 3.����ǵ�һ�������� */
    if (SLIST_HEAD(head) == NULL) {
        /* �����ɸ���Ϊ��ʼ�� */
        (*(uint32_t *)unit) = 0;
        /* ��������ͷ */
        SLIST_PREPEND(head, tail, node);
        /*  */
        return;
    }
    /* 4.�����Ҫ�����һ�� */
    if (SLIST_HEAD(head) != NULL) {
        /*  */
        void *temp = SNODE_GET(SLIST_HEAD(head));
        /*  */
        if ((*(uint32_t *)temp) > 0) {
            /* �����ɸ���Ϊ��ʼ�� */
            (*(uint32_t *)unit) = 0;
            /* ��������ͷ */
            SLIST_PREPEND(head, tail, node);
            /*  */
            return;
        }
    }
    /* 5.������ǵ�һ�������� */
    SGuiSNode *current = NULL;
    /* �������� */
    SLIST_TRAVERSE(head, tail, current) {
        /*  */
        void *temp = SNODE_GET(current);
        /* 1.��������һ�� */
        if (SNODE_NEXT(current) == NULL) {
            /* �����ɸ���Ϊ��һ�� */
            (*(uint32_t *)unit) = (*(uint32_t *)temp) + 1;
            /* ��������β */
            SLIST_APPEND(head, tail, node);
            /*  */
            return;
        }
        /*  */
        void *temp1 = SNODE_GET(SNODE_NEXT(current));
        /* 2.����м��� */
        if (SNODE_NEXT(current) != NULL) {
            /* ���֮�䲻֧�ֲ����µ�Ԫ */
            if ((*(uint32_t *)temp) + 1 == (*(uint32_t *)temp1))
                continue;
            /* ���֮��֧�ֲ����µ�Ԫ */
            if ((*(uint32_t *)temp) + 1 != (*(uint32_t *)temp1)) {
                /* �����ɸ���Ϊ��һ�� */
                (*(uint32_t *)unit) = (*(uint32_t *)temp) + 1;
                /* ����ڵ�β */
                SLIST_INSERT(head, tail, current, node);
                /*  */
            }
        }
    }
    /* ���ܵ������� */
}

/* ��һ�����е�Ԫ�Ƴ��ڵ�,���Ƴ�������(ʹ����С������) */
void SGuiSNodeUnitRemove(SGuiSNode *head, SGuiSNode *tail, void *unit)
{
    /*  */
    SGuiSNode *current = NULL;
    SGuiSNode *node    = NULL;
    /* ͷ���Ҫ��������(��Ϊ�ǵ�����) */
    if (SNODE_GET(SLIST_HEAD(head)) == unit) {
        /*  */
        node = SLIST_HEAD(head);
        /* �ڵ��Ƴ������� */
        SLIST_REMOVE(head, tail, current, node);
        /* �ͷŽڵ�ռ� */
        SGUIFREE(node);
        return;
    }
    /* �������� */
    SLIST_TRAVERSE(head, tail, current) {
        /* �������ⳡ�� */
        if (SNODE_NEXT(current) == NULL)
            break;
        /* ����Ŀ�� */
        if (SNODE_GET(SNODE_NEXT(current)) == unit) {
            node = SNODE_NEXT(current);
            break;
        }
    }
    /* Ѱ�ҵ��˿��е�Ԫ */
    if (node != NULL) {
        /* �ڵ��Ƴ������� */
        SLIST_REMOVE(head, tail, current, node);
        /* �ͷŽڵ�ռ� */
        SGUIFREE(node);
    }
    /* ���ܵ������� */
    /* û��Ѱ�ҵ��˿��е�Ԫ */
    if (node == NULL)
        return;
}

/* ��һ�����е�ԪѰ�ҵ�(ʹ����С������) */
void * SGuiSNodeUnitSearch(SGuiSNode *head, SGuiSNode *tail, uint32_t index)
{
    void      *unit    = NULL;
    SGuiSNode *current = NULL;
    /* ��������,ȷ������һ������Ԫ */
    SLIST_TRAVERSE(head, tail, current) {
        /*  */
        unit = SNODE_GET(current);
        /* Ѱ��Ŀ�굥Ԫ */
        if ((*(uint32_t *)unit) == index)
            break;
        if ((*(uint32_t *)unit) != index)
            continue;
    }

    if (current != NULL);
    if (current == NULL)
        unit     = NULL;

    return unit;
}

#endif

/* ͬ��ԭ��������ɻ�(���ڼ򻯱�Ե�߼�) */
#define SGUISYNCPRIMITSTSPECIALDESIGN
#ifdef  SGUISYNCPRIMITSTSPECIALDESIGN

/* ����ͬ��ԭ��(true:���ñ���,false:���ñ���) */
void SGuiSyncPrimitOptSyncMutex(SGuiSyncPrimit **Mutex,
                                void            *Variable,
                                bool            *Init,
                                bool            *DeInit,
                                bool             StartOrEnd)
{
    /* ͬ��ԭ��ĳ�ʼ�� */
    if (*Init == false) {
        *Init  = true;
        /* ��ʼ�� */
        *Mutex = SGuiSyncPrimitCreate(SGUIMUTEX);
        /* ����һ������Դ��ֹ����NULL */
        SGuiSyncPrimitSetResource(*Mutex, Variable);
    }
    /* ͬ��ԭ��ķ���ʼ��(ʲôʱ�������ʽ����ʹ����Դ����?) */
    if (*DeInit == false) {
        *DeInit  = true;
        //SGuiSyncPrimitDestory(*Mutex);
    }
    /* ��ȡ����Դ */
    if (StartOrEnd == true) {
        uint8_t * pointer = SGuiSyncPrimitTakeResource(*Mutex);
        /* ���Լ�Ҫ�������ȥȷ��ͬ��ԭ���Ƿ���Ч */
    }
    /* �ͷż���Դ */
    if (StartOrEnd == false) {
        SGuiSyncPrimitGiveResource(*Mutex);
    }
}

#endif


