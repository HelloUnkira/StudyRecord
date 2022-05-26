#include "SGui_Kernel.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 实现目标:
 * 设计简要的句柄资源管理器
 * 采用二级数组实现
 * 在保证查找效率为O(1)的条件下
 * 降低额外空间开销
 * 将时间成本转移到获取与释放部分
 * 因为句柄通常不需要频繁获取与释放
 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 资源管理单元的伸缩因子 */
#define SGUI_UNIT_FACTOR    1       /* 暴力测试:1,推荐使用:5 */
/* 资源管理单元的句柄数量 */
#define SGUI_UNIT_LENGTH    1     /* 暴力测试:1,推荐使用:50,100 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 句柄计算规则:
 * Index1 = Handle / SGUI_UNIT_LENGTH;
 * Index2 = Handle % SGUI_UNIT_LENGTH;
 * Handle = Index1 * SGUI_UNIT_LENGTH + Index2;
 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 资源管理单元 */
typedef struct SimpleGui_HandleUnit {
    void   **Source;    //句柄资源集合
    uint32_t Length;    //句柄资源集合数量
    uint32_t Number;    //可使用句柄数量
} SGui_HandleUnit;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SimpleGui_HandleSet {
    SGui_HandleUnit *Units;     //资源管理单元集合
    uint32_t Length;            //句柄资源集合数量
    uint32_t Number;            //可使用资源管理单元数量
} SGui_HandleSet;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 句柄管理集合 */
static SGui_HandleSet Handles = {0};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 获取一个空闲句柄 */
uint32_t SGui_HandleTake(void)
{
    uint32_t Index1 = 0;
    uint32_t Index2 = 0;
    uint32_t Handle = SGUI_HANDLE_INVALID;
    /* 1.遍历资源管理单元,查找一个有空闲句柄的单元 */
    for (Index1 = 0; Index1 < Handles.Length; Index1++) {
        if (Handles.Units[Index1].Source == NULL)
            break;
        if (Handles.Units[Index1].Number > 0)
            break;
    }
    /* 2.如果资源管理单元不足,扩张它 */
    if (Index1 == Handles.Length) {
        /* 生成一个新的资源管理单元集合 */
        uint32_t Number = Handles.Number + SGUI_UNIT_FACTOR;
        uint32_t Length = Handles.Length + SGUI_UNIT_FACTOR;
        SGui_HandleUnit *Units = SGUI_ALLOC(sizeof(SGui_HandleUnit) * Length);
        /* 拷贝原生的资源管理单元集合 */
        for (uint32_t Index = 0; Index < Handles.Length; Index++)
            Units[Index] = Handles.Units[Index];
        /* 初始化新生成的资源管理单元集合 */
        for (uint32_t Index = Handles.Length; Index < Length; Index++)
            Units[Index].Source = NULL;
        /* 释放原生的资源管理单元集合 */
        SGUI_FREE(Handles.Units);
        /* 设置新生成的资源管理单元集合 */
        Handles.Number = Number;
        Handles.Length = Length;
        Handles.Units  = Units;
    }
    /* 3.如果资源管理单元不存在,创建它 */
    if (Handles.Units[Index1].Source == NULL) {
        /* 生成单元句柄集合 */
        uint32_t Number = SGUI_UNIT_LENGTH;
        uint32_t Length = SGUI_UNIT_LENGTH;
        void   **Source = SGUI_ALLOC(sizeof(void *) * Length);
        /* 初始化新生成的单元句柄集合 */
        for (uint32_t Index = 0; Index < Length; Index++)
            Source[Index] = NULL;
        /* 设置新生成的资源管理单元集合 */
        Handles.Units[Index1].Number = Number;
        Handles.Units[Index1].Length = Length;
        Handles.Units[Index1].Source = Source;
        Handles.Number--;
    }
    /* 4.寻找一个空闲句柄 */
    if (Handles.Units[Index1].Number > 0) {
        /* 遍历单元句柄集合,寻找一个空闲句柄 */
        for (Index2 = 0; Index2 < Handles.Units[Index1].Length; Index2++)
            if (Handles.Units[Index1].Source[Index2] == NULL) {
                Handles.Units[Index1].Source[Index2]  = (void *)(~0);
                Handles.Units[Index1].Number--;
                break;
            }
    }
    /* 5.最终检查 */
    if (Index1 < Handles.Length && Index2 < Handles.Units[Index1].Length) {
        /* 计算句柄 */
        Handle = Index1 * SGUI_UNIT_LENGTH + Index2;
    }
    /*  */
    return Handle;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 释放一个句柄 */
void SGui_HandleGive(uint32_t Handle)
{
    int32_t  Index  = 0;
    uint32_t Index1 = Handle / SGUI_UNIT_LENGTH;
    uint32_t Index2 = Handle % SGUI_UNIT_LENGTH;
    /* 这是一个合法句柄 */
    if (Index1 < Handles.Length && Index2 < Handles.Units[Index1].Length) {
        /* 1.释放这个合法的句柄 */
        Handles.Units[Index1].Source[Index2] = NULL;
        Handles.Units[Index1].Number++;
        /* 2.句柄管理单元为空时,释放它 */
        if (Handles.Units[Index1].Length == Handles.Units[Index1].Number) {
            /* 释放这个句柄管理单元 */
            SGUI_FREE(Handles.Units[Index1].Source);
            /* 更新管理器的情景 */
            Handles.Units[Index1].Source = NULL;
            Handles.Number++;
            /* 如果句柄管理单元空闲过多,尝试压缩 */
            while (Handles.Number >= SGUI_UNIT_FACTOR) {
                /* 一级寻址依赖下标,所以只能逆向检查,中间空余部不可压缩 */
                for (Index = Handles.Length - 1; Index >= 0; Index--)
                    if (Handles.Units[Index].Source != NULL)
                        break;
                /* 如果句柄管理单元全空闲,释放本身 */
                if (Index < 0) {
                    /* 释放这个句柄管理单元集合 */
                    SGUI_FREE(Handles.Units);
                    Handles.Number = 0;
                    Handles.Length = 0;
                    Handles.Units  = NULL;
                    break;
                }
                /* 如果最后的几个连续为空闲态单元未达到伸缩标准时,放弃 */
                if (Index + SGUI_UNIT_FACTOR >= Handles.Length)
                    break;
                /* 如果最后的几个连续为空闲态单元达到伸缩标准时,压缩它 */
                if (Index + SGUI_UNIT_FACTOR < Handles.Length) {
                    /* 生成一个新的资源管理单元集合 */
                    uint32_t Number = Handles.Number - SGUI_UNIT_FACTOR;
                    uint32_t Length = Handles.Length - SGUI_UNIT_FACTOR;
                    SGui_HandleUnit *Units = SGUI_ALLOC(sizeof(SGui_HandleUnit) * Length);
                    /* 拷贝原生的资源管理单元集合 */
                    for (Index = 0; Index < Length; Index++)
                        Units[Index] = Handles.Units[Index];
                    /* 释放原生的资源管理单元集合 */
                    SGUI_FREE(Handles.Units);
                    /* 设置新生成的资源管理单元集合 */
                    Handles.Number = Number;
                    Handles.Length = Length;
                    Handles.Units  = Units;
                }
            }
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 句柄绑定或更新资源 */
void SGui_HandleSourceSet(uint32_t Handle, void *Source)
{
    uint32_t Index1 = Handle / SGUI_UNIT_LENGTH;
    uint32_t Index2 = Handle % SGUI_UNIT_LENGTH;
    /* 这是一个合法句柄 */
    if (Index1 < Handles.Length && Index2 < Handles.Units[Index1].Length)
        Handles.Units[Index1].Source[Index2] = Source;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 句柄获取资源 */
void * SGui_HandleSourceGet(uint32_t Handle)
{
    void *Source = NULL;
    uint32_t Index1 = Handle / SGUI_UNIT_LENGTH;
    uint32_t Index2 = Handle % SGUI_UNIT_LENGTH;
    /* 这是一个合法句柄 */
    if (Index1 < Handles.Length && Index2 < Handles.Units[Index1].Length)
        Source = Handles.Units[Index1].Source[Index2];
    
    return Source;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
