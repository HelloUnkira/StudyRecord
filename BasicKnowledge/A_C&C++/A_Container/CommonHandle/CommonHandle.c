//C std lib
#include <stdint.h>
#include <stdbool.h>

/* ����Ψһ������ */
#define   COMMONHANDLE_C
#include "CommonHandle.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* ��Դ����Ԫ */
struct CommonHandleUnit {
    uint32_t Number;    //��ʹ�þ������
    uint32_t Length;    //�����Դ��������
    void   **Source;    //�����Դ����
};
/* ��Դ���� */
struct CommonHandleBody {
    struct CommonHandleUnit *Unit;  //��Դ����Ԫ����
    uint32_t Number;                //��ʹ����Դ����Ԫ����
    uint32_t Length;                //�����Դ��������
};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* ����ת�� */
typedef struct CommonHandleUnit CHandleUnit;
typedef struct CommonHandleBody CHandleBody;
/* ��������� */
static CHandleBody CHandle = {0};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* ��ȡһ�����о�� */
uint32_t CommonHandleTake(void)
{
    uint32_t Index  = 0;
    uint32_t Index1 = 0;
    uint32_t Index2 = 0;
    uint32_t Handle = COMMON_HANDLE_INVALID;
    /* 1.������Դ����Ԫ,����һ���п��о���ĵ�Ԫ */
    for (Index1 = 0; Index1 < CHandle.Length; Index1++) {
        if (CHandle.Unit[Index1].Source == NULL)
            break;
        if (CHandle.Unit[Index1].Number > 0)
            break;
    }
    /* 2.�����Դ����Ԫ����,������ */
    if (Index1 == CHandle.Length) {
        /* ����һ���µ���Դ����Ԫ���� */
        uint32_t Number = CHandle.Number + COMMON_HANDLE_UINT_SCAL_FACTOR;
        uint32_t Length = CHandle.Length + COMMON_HANDLE_UINT_SCAL_FACTOR;
        CHandleUnit *Unit = COMMON_HANDLE_MALLOC(sizeof(CHandleUnit) * Length);
        /* ����ԭ������Դ����Ԫ���� */
        for (Index = 0; Index < CHandle.Length; Index++)
            Unit[Index] = CHandle.Unit[Index];
        /* ��ʼ�������ɵ���Դ����Ԫ���� */
        for (Index = CHandle.Length; Index < Length; Index++)
            Unit[Index].Source = NULL;
        /* �ͷ�ԭ������Դ����Ԫ���� */
        COMMON_HANDLE_FREE(CHandle.Unit);
        /* ���������ɵ���Դ����Ԫ���� */
        CHandle.Number = Number;
        CHandle.Length = Length;
        CHandle.Unit   = Unit;
    }
    /* 3.�����Դ����Ԫ������,������ */
    if (CHandle.Unit[Index1].Source == NULL) {
        /* ���ɵ�Ԫ������� */
        uint32_t Number = COMMON_HANDLE_UINT_LENGTH;
        uint32_t Length = COMMON_HANDLE_UINT_LENGTH;
        void   **Source = COMMON_HANDLE_MALLOC(sizeof(void *) * Length);
        /* ��ʼ�������ɵĵ�Ԫ������� */
        for (Index = 0; Index < Length; Index++)
            Source[Index] = NULL;
        /* ���������ɵ���Դ����Ԫ���� */
        CHandle.Unit[Index1].Number = Number;
        CHandle.Unit[Index1].Length = Length;
        CHandle.Unit[Index1].Source = Source;
        CHandle.Number--;
    }
    /* 4.Ѱ��һ�����о�� */
    if (CHandle.Unit[Index1].Number > 0) {
        /* ������Ԫ�������,Ѱ��һ�����о�� */
        for (Index2 = 0; Index2 < CHandle.Unit[Index1].Length; Index2++)
            if (CHandle.Unit[Index1].Source[Index2] == NULL) {
                CHandle.Unit[Index1].Source[Index2]  = (void *)(~0);
                CHandle.Unit[Index1].Number--;
                break;
            }
    }
    /* 5.���ռ�� */
    if (Index1 < CHandle.Length && Index2 < CHandle.Unit[Index1].Length) {
        /* ������ */
        Handle = Index1 * COMMON_HANDLE_UINT_LENGTH + Index2;
    }
    /*  */
    return Handle;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* �ͷ�һ����� */
void CommonHandleGive(uint32_t Handle)
{
    int32_t  Index  = 0;
    uint32_t Index1 = Handle / COMMON_HANDLE_UINT_LENGTH;
    uint32_t Index2 = Handle % COMMON_HANDLE_UINT_LENGTH;
    /* 1.����һ���Ƿ���� */
    if (Index1 >= CHandle.Length || Index2 >= CHandle.Unit[Index1].Length)
        return;
    /* 2.�ͷ�����Ϸ��ľ�� */
    CHandle.Unit[Index1].Source[Index2] = NULL;
    CHandle.Unit[Index1].Number++;
    /* 3.�������Ԫ�ǿ�ʱ,���ͷ��� */
    if (CHandle.Unit[Index1].Length != CHandle.Unit[Index1].Number)
        return;
    /* 4.�ͷ�����������Ԫ */
    COMMON_HANDLE_FREE(CHandle.Unit[Index1].Source);
    /* 5.���¹��������龰 */
    CHandle.Unit[Index1].Source = NULL;
    CHandle.Number++;
    /* 6.����������Ԫ���й���,����ѹ�� */
    while (CHandle.Number >= COMMON_HANDLE_UINT_SCAL_FACTOR) {
        /* 1.һ��Ѱַ�����±�,����ֻ��������,�м���ಿ����ѹ�� */
        for (Index = CHandle.Length - 1; Index >= 0; Index--)
            if (CHandle.Unit[Index].Source != NULL)
                break;
        /* 2.����������Ԫȫ����,�ͷű��� */
        if (Index < 0) {
            /* �ͷ�����������Ԫ���� */
            COMMON_HANDLE_FREE(CHandle.Unit);
            CHandle.Number = 0;
            CHandle.Length = 0;
            CHandle.Unit   = NULL;
            break;
        }
        /* 3.������ļ�������Ϊ����̬��Ԫδ�ﵽ������׼ʱ,���� */
        if (Index + COMMON_HANDLE_UINT_SCAL_FACTOR >= CHandle.Length)
            break;
        /* 4.������ļ�������Ϊ����̬��Ԫ�ﵽ������׼ʱ,ѹ���� */
        if (Index + COMMON_HANDLE_UINT_SCAL_FACTOR <  CHandle.Length) {
            /* 1.����һ���µ���Դ����Ԫ���� */
            uint32_t Number = CHandle.Number - COMMON_HANDLE_UINT_SCAL_FACTOR;
            uint32_t Length = CHandle.Length - COMMON_HANDLE_UINT_SCAL_FACTOR;
            CHandleUnit *Unit = COMMON_HANDLE_MALLOC(sizeof(CHandleUnit) * Length);
            /* 2.����ԭ������Դ����Ԫ���� */
            for (Index = 0; Index < Length; Index++)
                Unit[Index] = CHandle.Unit[Index];
            /* 3.�ͷ�ԭ������Դ����Ԫ���� */
            COMMON_HANDLE_FREE(CHandle.Unit);
            /* 4.���������ɵ���Դ����Ԫ���� */
            CHandle.Number = Number;
            CHandle.Length = Length;
            CHandle.Unit   = Unit;
        }
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* ����󶨻������Դ */
void CommonHandleSet(uint32_t Handle, void *Source)
{
    uint32_t Index1 = Handle / COMMON_HANDLE_UINT_LENGTH;
    uint32_t Index2 = Handle % COMMON_HANDLE_UINT_LENGTH;
    /* ����һ���Ϸ���� */
    if (Index1 < CHandle.Length && Index2 < CHandle.Unit[Index1].Length)
        CHandle.Unit[Index1].Source[Index2] = Source;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* �����ȡ��Դ */
void * CommonHandleGet(uint32_t Handle)
{
    void *Source = NULL;
    
    uint32_t Index1 = Handle / COMMON_HANDLE_UINT_LENGTH;
    uint32_t Index2 = Handle % COMMON_HANDLE_UINT_LENGTH;
    /* ����һ���Ϸ���� */
    if (Index1 < CHandle.Length && Index2 < CHandle.Unit[Index1].Length)
        Source = CHandle.Unit[Index1].Source[Index2];

    return Source;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
