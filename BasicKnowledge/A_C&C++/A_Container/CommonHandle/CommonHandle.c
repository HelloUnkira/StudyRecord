//C std lib
#include <stdint.h>
#include <stdbool.h>

/* 核心唯一依赖项 */
#define   COMMONHANDLE_C
#include "CommonHandle.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 资源管理单元 */
struct CommonHandleUnit {
    uint32_t Number;    //可使用句柄数量
    uint32_t Length;    //句柄资源集合数量
    void   **Source;    //句柄资源集合
};
/* 资源管理 */
struct CommonHandleBody {
    struct CommonHandleUnit *Unit;  //资源管理单元集合
    uint32_t Number;                //可使用资源管理单元数量
    uint32_t Length;                //句柄资源集合数量
};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 核心转义 */
typedef struct CommonHandleUnit CHandleUnit;
typedef struct CommonHandleBody CHandleBody;
/* 句柄管理集合 */
static CHandleBody CHandle = {0};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 获取一个空闲句柄 */
uint32_t CommonHandleTake(void)
{
    uint32_t Index  = 0;
    uint32_t Index1 = 0;
    uint32_t Index2 = 0;
    uint32_t Handle = COMMON_HANDLE_INVALID;
    /* 1.遍历资源管理单元,查找一个有空闲句柄的单元 */
    for (Index1 = 0; Index1 < CHandle.Length; Index1++) {
        if (CHandle.Unit[Index1].Source == NULL)
            break;
        if (CHandle.Unit[Index1].Number > 0)
            break;
    }
    /* 2.如果资源管理单元不足,扩张它 */
    if (Index1 == CHandle.Length) {
        /* 生成一个新的资源管理单元集合 */
        uint32_t Number = CHandle.Number + COMMON_HANDLE_UINT_SCAL_FACTOR;
        uint32_t Length = CHandle.Length + COMMON_HANDLE_UINT_SCAL_FACTOR;
        CHandleUnit *Unit = COMMON_HANDLE_MALLOC(sizeof(CHandleUnit) * Length);
        /* 拷贝原生的资源管理单元集合 */
        for (Index = 0; Index < CHandle.Length; Index++)
            Unit[Index] = CHandle.Unit[Index];
        /* 初始化新生成的资源管理单元集合 */
        for (Index = CHandle.Length; Index < Length; Index++)
            Unit[Index].Source = NULL;
        /* 释放原生的资源管理单元集合 */
        COMMON_HANDLE_FREE(CHandle.Unit);
        /* 设置新生成的资源管理单元集合 */
        CHandle.Number = Number;
        CHandle.Length = Length;
        CHandle.Unit   = Unit;
    }
    /* 3.如果资源管理单元不存在,创建它 */
    if (CHandle.Unit[Index1].Source == NULL) {
        /* 生成单元句柄集合 */
        uint32_t Number = COMMON_HANDLE_UINT_LENGTH;
        uint32_t Length = COMMON_HANDLE_UINT_LENGTH;
        void   **Source = COMMON_HANDLE_MALLOC(sizeof(void *) * Length);
        /* 初始化新生成的单元句柄集合 */
        for (Index = 0; Index < Length; Index++)
            Source[Index] = NULL;
        /* 设置新生成的资源管理单元集合 */
        CHandle.Unit[Index1].Number = Number;
        CHandle.Unit[Index1].Length = Length;
        CHandle.Unit[Index1].Source = Source;
        CHandle.Number--;
    }
    /* 4.寻找一个空闲句柄 */
    if (CHandle.Unit[Index1].Number > 0) {
        /* 遍历单元句柄集合,寻找一个空闲句柄 */
        for (Index2 = 0; Index2 < CHandle.Unit[Index1].Length; Index2++)
            if (CHandle.Unit[Index1].Source[Index2] == NULL) {
                CHandle.Unit[Index1].Source[Index2]  = (void *)(~0);
                CHandle.Unit[Index1].Number--;
                break;
            }
    }
    /* 5.最终检查 */
    if (Index1 < CHandle.Length && Index2 < CHandle.Unit[Index1].Length) {
        /* 计算句柄 */
        Handle = Index1 * COMMON_HANDLE_UINT_LENGTH + Index2;
    }
    /*  */
    return Handle;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 释放一个句柄 */
void CommonHandleGive(uint32_t Handle)
{
    int32_t  Index  = 0;
    uint32_t Index1 = Handle / COMMON_HANDLE_UINT_LENGTH;
    uint32_t Index2 = Handle % COMMON_HANDLE_UINT_LENGTH;
    /* 1.这是一个非法句柄 */
    if (Index1 >= CHandle.Length || Index2 >= CHandle.Unit[Index1].Length)
        return;
    /* 2.释放这个合法的句柄 */
    CHandle.Unit[Index1].Source[Index2] = NULL;
    CHandle.Unit[Index1].Number++;
    /* 3.句柄管理单元非空时,不释放它 */
    if (CHandle.Unit[Index1].Length != CHandle.Unit[Index1].Number)
        return;
    /* 4.释放这个句柄管理单元 */
    COMMON_HANDLE_FREE(CHandle.Unit[Index1].Source);
    /* 5.更新管理器的情景 */
    CHandle.Unit[Index1].Source = NULL;
    CHandle.Number++;
    /* 6.如果句柄管理单元空闲过多,尝试压缩 */
    while (CHandle.Number >= COMMON_HANDLE_UINT_SCAL_FACTOR) {
        /* 1.一级寻址依赖下标,所以只能逆向检查,中间空余部不可压缩 */
        for (Index = CHandle.Length - 1; Index >= 0; Index--)
            if (CHandle.Unit[Index].Source != NULL)
                break;
        /* 2.如果句柄管理单元全空闲,释放本身 */
        if (Index < 0) {
            /* 释放这个句柄管理单元集合 */
            COMMON_HANDLE_FREE(CHandle.Unit);
            CHandle.Number = 0;
            CHandle.Length = 0;
            CHandle.Unit   = NULL;
            break;
        }
        /* 3.如果最后的几个连续为空闲态单元未达到伸缩标准时,放弃 */
        if (Index + COMMON_HANDLE_UINT_SCAL_FACTOR >= CHandle.Length)
            break;
        /* 4.如果最后的几个连续为空闲态单元达到伸缩标准时,压缩它 */
        if (Index + COMMON_HANDLE_UINT_SCAL_FACTOR <  CHandle.Length) {
            /* 1.生成一个新的资源管理单元集合 */
            uint32_t Number = CHandle.Number - COMMON_HANDLE_UINT_SCAL_FACTOR;
            uint32_t Length = CHandle.Length - COMMON_HANDLE_UINT_SCAL_FACTOR;
            CHandleUnit *Unit = COMMON_HANDLE_MALLOC(sizeof(CHandleUnit) * Length);
            /* 2.拷贝原生的资源管理单元集合 */
            for (Index = 0; Index < Length; Index++)
                Unit[Index] = CHandle.Unit[Index];
            /* 3.释放原生的资源管理单元集合 */
            COMMON_HANDLE_FREE(CHandle.Unit);
            /* 4.设置新生成的资源管理单元集合 */
            CHandle.Number = Number;
            CHandle.Length = Length;
            CHandle.Unit   = Unit;
        }
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 句柄绑定或更新资源 */
void CommonHandleSet(uint32_t Handle, void *Source)
{
    uint32_t Index1 = Handle / COMMON_HANDLE_UINT_LENGTH;
    uint32_t Index2 = Handle % COMMON_HANDLE_UINT_LENGTH;
    /* 这是一个合法句柄 */
    if (Index1 < CHandle.Length && Index2 < CHandle.Unit[Index1].Length)
        CHandle.Unit[Index1].Source[Index2] = Source;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 句柄获取资源 */
void * CommonHandleGet(uint32_t Handle)
{
    void *Source = NULL;
    
    uint32_t Index1 = Handle / COMMON_HANDLE_UINT_LENGTH;
    uint32_t Index2 = Handle % COMMON_HANDLE_UINT_LENGTH;
    /* 这是一个合法句柄 */
    if (Index1 < CHandle.Length && Index2 < CHandle.Unit[Index1].Length)
        Source = CHandle.Unit[Index1].Source[Index2];

    return Source;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
