/* ʵ��Ŀ��:
 * ��Ƽ�Ҫ�ľ����Դ������
 * ���ö�������ʵ��
 * �ڱ�֤����Ч��ΪO(1)��������
 * ���Ͷ���ռ俪��
 * ��ʱ��ɱ�ת�Ƶ���ȡ���ͷŲ���
 * ��Ϊ���ͨ������ҪƵ����ȡ���ͷ�
 */

/* ����Ψһ������ */
#include "SGuiCore.h"

/* ��Դ����Ԫ���������� */
#define SGUIUNITSCALFACTOR  5       /* ��������:1,�Ƽ�ʹ��:5 */
/* ��Դ����Ԫ�ľ������ */
#define SGUIUNITLENGTH      100     /* ��������:1,�Ƽ�ʹ��:50,100 */
/* ����������:
 * index1 = handle / SGUIUNITLENGTH;
 * index2 = handle % SGUIUNITLENGTH;
 * handle = index1 * SGUIUNITLENGTH + index2;
 */

/* ��Դ����Ԫ */
typedef struct SimpleGuiHandleUnit {
    uint32_t number;    //��ʹ�þ������
    uint32_t length;    //�����Դ��������
    void   **source;    //�����Դ����
} SGuiHandleUnit;

typedef struct SimpleGuiHandle {
    uint32_t number;        //��ʹ����Դ����Ԫ����
    uint32_t length;        //�����Դ��������
    SGuiHandleUnit *units;  //��Դ����Ԫ����
} SGuiHandle;

/* ��������� */
static SGuiHandle handles = {0};

/* ��ȡһ�����о�� */
uint32_t SGuiHandleTake(void)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index  = 0;
    uint32_t index1 = 0;
    uint32_t index2 = 0;
    uint32_t handle = SGUIINVALIDHANDLE;
    /* 1.������Դ����Ԫ,����һ���п��о���ĵ�Ԫ */
    for (index1 = 0; index1 < handles.length; index1++) {
        if (handles.units[index1].source == NULL)
            break;
        if (handles.units[index1].number > 0)
            break;
    }
    /* 2.�����Դ����Ԫ����,������ */
    if (index1 == handles.length) {
        /* ����һ���µ���Դ����Ԫ���� */
        uint32_t number = handles.number + SGUIUNITSCALFACTOR;
        uint32_t length = handles.length + SGUIUNITSCALFACTOR;
        SGuiHandleUnit *units = SGUIMALLOC(sizeof(SGuiHandleUnit) * length);
        /* ����ԭ������Դ����Ԫ���� */
        for (index = 0; index < handles.length; index++)
            units[index] = handles.units[index];
        /* ��ʼ�������ɵ���Դ����Ԫ���� */
        for (index = handles.length; index < length; index++)
            units[index].source = NULL;
        /* �ͷ�ԭ������Դ����Ԫ���� */
        SGUIFREE(handles.units);
        /* ���������ɵ���Դ����Ԫ���� */
        handles.number = number;
        handles.length = length;
        handles.units = units;
    }
    /* 3.�����Դ����Ԫ������,������ */
    if (handles.units[index1].source == NULL) {
        /* ���ɵ�Ԫ������� */
        uint32_t number = SGUIUNITLENGTH;
        uint32_t length = SGUIUNITLENGTH;
        void   **source = SGUIMALLOC(sizeof(void *) * length);
        /* ��ʼ�������ɵĵ�Ԫ������� */
        for (index = 0; index < length; index++)
            source[index] = NULL;
        /* ���������ɵ���Դ����Ԫ���� */
        handles.units[index1].number = number;
        handles.units[index1].length = length;
        handles.units[index1].source = source;
        handles.number--;
    }
    /* 4.Ѱ��һ�����о�� */
    if (handles.units[index1].number > 0) {
        /* ������Ԫ�������,Ѱ��һ�����о�� */
        for (index2 = 0; index2 < handles.units[index1].length; index2++)
            if (handles.units[index1].source[index2] == NULL) {
                handles.units[index1].source[index2]  = (void *)(~0);
                handles.units[index1].number--;
                break;
            }
    }
    /* 5.���ռ�� */
    if (index1 < handles.length && index2 < handles.units[index1].length) {
        /* ������ */
        handle = index1 * SGUIUNITLENGTH + index2;
    }
    /*  */
    SGUISYNCPRIMITOPTSYNCMUTEX(false);
    /*  */
    return handle;
}

/* �ͷ�һ����� */
void SGuiHandleGive(uint32_t handle)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    int32_t  index  = 0;
    uint32_t index1 = handle / SGUIUNITLENGTH;
    uint32_t index2 = handle % SGUIUNITLENGTH;
    /* ����һ���Ϸ���� */
    if (index1 < handles.length && index2 < handles.units[index1].length) {
        /* 1.�ͷ�����Ϸ��ľ�� */
        handles.units[index1].source[index2] = NULL;
        handles.units[index1].number++;
        /* 2.�������ԪΪ��ʱ,�ͷ��� */
        if (handles.units[index1].length == handles.units[index1].number) {
            /* �ͷ�����������Ԫ */
            SGUIFREE(handles.units[index1].source);
            /* ���¹��������龰 */
            handles.units[index1].source = NULL;
            handles.number++;
            /* ����������Ԫ���й���,����ѹ�� */
            while (handles.number >= SGUIUNITSCALFACTOR) {
                /* һ��Ѱַ�����±�,����ֻ��������,�м���ಿ����ѹ�� */
                for (index = handles.length - 1; index >= 0; index--)
                    if (handles.units[index].source != NULL)
                        break;
                /* ����������Ԫȫ����,�ͷű��� */
                if (index < 0) {
                    /* �ͷ�����������Ԫ���� */
                    SGUIFREE(handles.units);
                    handles.number = 0;
                    handles.length = 0;
                    handles.units = NULL;
                    break;
                }
                /* ������ļ�������Ϊ����̬��Ԫδ�ﵽ������׼ʱ,���� */
                if (index + SGUIUNITSCALFACTOR >= handles.length)
                    break;
                /* ������ļ�������Ϊ����̬��Ԫ�ﵽ������׼ʱ,ѹ���� */
                if (index + SGUIUNITSCALFACTOR < handles.length) {
                    /* ����һ���µ���Դ����Ԫ���� */
                    uint32_t number = handles.number - SGUIUNITSCALFACTOR;
                    uint32_t length = handles.length - SGUIUNITSCALFACTOR;
                    SGuiHandleUnit *units = SGUIMALLOC(sizeof(SGuiHandleUnit) * length);
                    /* ����ԭ������Դ����Ԫ���� */
                    for (index = 0; index < length; index++)
                        units[index] = handles.units[index];
                    /* �ͷ�ԭ������Դ����Ԫ���� */
                    SGUIFREE(handles.units);
                    /* ���������ɵ���Դ����Ԫ���� */
                    handles.number = number;
                    handles.length = length;
                    handles.units = units;
                }
            }
        }
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}

/* ����󶨻������Դ */
void SGuiHandleSet(uint32_t handle, void *source)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index1 = handle / SGUIUNITLENGTH;
    uint32_t index2 = handle % SGUIUNITLENGTH;
    /* ����һ���Ϸ���� */
    if (index1 < handles.length && index2 < handles.units[index1].length)
        handles.units[index1].source[index2] = source;

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}

/* �����ȡ��Դ */
void * SGuiHandleGet(uint32_t handle)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    void *source = NULL;
    uint32_t index1 = handle / SGUIUNITLENGTH;
    uint32_t index2 = handle % SGUIUNITLENGTH;
    /* ����һ���Ϸ���� */
    if (index1 < handles.length && index2 < handles.units[index1].length)
        source = handles.units[index1].source[index2];

    SGUISYNCPRIMITOPTSYNCMUTEX(false);

    return source;
}
