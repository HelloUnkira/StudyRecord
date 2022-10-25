#include "SGui_Core.h"
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
/* 资源管理单元的伸缩因子和句柄数量 */
#if SGUI_INTERNAL_TEST
#define SGUI_UNIT_FACTOR    1       /* 暴力测试:1,推荐使用:5 */
#define SGUI_UNIT_LENGTH    1       /* 暴力测试:1,推荐使用:50,100 */
#else
#define SGUI_UNIT_FACTOR    5       /* 暴力测试:1,推荐使用:5 */
#define SGUI_UNIT_LENGTH    100     /* 暴力测试:1,推荐使用:50,100 */
#endif
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
    void      **Source; //句柄资源集合
    SGui_Handle Length; //句柄资源集合数量
    SGui_Handle Number; //可使用句柄数量
} SGui_HandleUnit;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SimpleGui_HandleSet {
    SGui_HandleUnit *Units;  //资源管理单元集合
    SGui_Handle      Length; //句柄资源集合数量
    SGui_Handle      Number; //可使用资源管理单元数量
} SGui_HandleSet;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 句柄管理集合 */
static SGui_HandleSet HandleSet = {0};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 静态句柄表 */
static void   **HandleStaticTable  = NULL;
static uint32_t HandleStaticLength = 0;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 获取一个空闲句柄 */
SGui_Handle SGui_HandleTake(void)
{
    SGui_Handle Index1 = 0;
    SGui_Handle Index2 = 0;
    SGui_Handle Handle = SGUI_HANDLE_INVALID;
    /* 1.遍历资源管理单元,查找一个有空闲句柄的单元 */
    for (Index1 = 0; Index1 < HandleSet.Length; Index1++) {
        if (HandleSet.Units[Index1].Source == NULL)
            break;
        if (HandleSet.Units[Index1].Number > 0)
            break;
    }
    /* 2.如果资源管理单元不足,扩张它 */
    if (Index1 == HandleSet.Length) {
        /* 生成一个新的资源管理单元集合 */
        SGui_Handle     Number = HandleSet.Number + SGUI_UNIT_FACTOR;
        SGui_Handle     Length = HandleSet.Length + SGUI_UNIT_FACTOR;
        SGui_HandleUnit *Units = SGUI_ALLOC(sizeof(SGui_HandleUnit) * Length);
        /* 拷贝原生的资源管理单元集合 */
        for (SGui_Handle Index = 0; Index < HandleSet.Length; Index++)
            Units[Index] = HandleSet.Units[Index];
        /* 初始化新生成的资源管理单元集合 */
        for (SGui_Handle Index = HandleSet.Length; Index < Length; Index++)
            Units[Index].Source = NULL;
        /* 释放原生的资源管理单元集合 */
        SGUI_FREE(HandleSet.Units);
        /* 设置新生成的资源管理单元集合 */
        HandleSet.Number = Number;
        HandleSet.Length = Length;
        HandleSet.Units  = Units;
    }
    /* 3.如果资源管理单元不存在,创建它 */
    if (HandleSet.Units[Index1].Source == NULL) {
        /* 生成单元句柄集合 */
        SGui_Handle Number = SGUI_UNIT_LENGTH;
        SGui_Handle Length = SGUI_UNIT_LENGTH;
        void      **Source = SGUI_ALLOC(sizeof(void *) * Length);
        /* 初始化新生成的单元句柄集合 */
        for (SGui_Handle Index = 0; Index < Length; Index++)
            Source[Index] = NULL;
        /* 设置新生成的资源管理单元集合 */
        HandleSet.Units[Index1].Number = Number;
        HandleSet.Units[Index1].Length = Length;
        HandleSet.Units[Index1].Source = Source;
        HandleSet.Number--;
    }
    /* 4.寻找一个空闲句柄 */
    if (HandleSet.Units[Index1].Number > 0) {
        /* 遍历单元句柄集合,寻找一个空闲句柄 */
        for (Index2 = 0; Index2 < HandleSet.Units[Index1].Length; Index2++)
            if (HandleSet.Units[Index1].Source[Index2] == NULL) {
                HandleSet.Units[Index1].Source[Index2]  = (void *)(~0);
                HandleSet.Units[Index1].Number--;
                break;
            }
    }
    /* 5.最终检查 */
    if (Index1 < HandleSet.Length && Index2 < HandleSet.Units[Index1].Length) {
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
void SGui_HandleGive(SGui_Handle Handle)
{
    int64_t     Index  = 0;
    SGui_Handle Index1 = Handle / SGUI_UNIT_LENGTH;
    SGui_Handle Index2 = Handle % SGUI_UNIT_LENGTH;
    /* 这是一个合法句柄 */
    if (Index1 < HandleSet.Length && Index2 < HandleSet.Units[Index1].Length) {
        /* 1.释放这个合法的句柄 */
        HandleSet.Units[Index1].Source[Index2] = NULL;
        HandleSet.Units[Index1].Number++;
        /* 2.句柄管理单元为空时,释放它 */
        if (HandleSet.Units[Index1].Length == HandleSet.Units[Index1].Number) {
            /* 释放这个句柄管理单元 */
            SGUI_FREE(HandleSet.Units[Index1].Source);
            /* 更新管理器的情景 */
            HandleSet.Units[Index1].Source = NULL;
            HandleSet.Number++;
            /* 如果句柄管理单元空闲过多,尝试压缩 */
            while (HandleSet.Number >= SGUI_UNIT_FACTOR) {
                /* 一级寻址依赖下标,所以只能逆向检查,中间空余部不可压缩 */
                for (Index = HandleSet.Length - 1; Index >= 0; Index--)
                    if (HandleSet.Units[Index].Source != NULL)
                        break;
                /* 如果句柄管理单元全空闲,释放本身 */
                if (Index < 0) {
                    /* 释放这个句柄管理单元集合 */
                    SGUI_FREE(HandleSet.Units);
                    HandleSet.Number = 0;
                    HandleSet.Length = 0;
                    HandleSet.Units  = NULL;
                    break;
                }
                /* 如果最后的几个连续为空闲态单元未达到伸缩标准时,放弃 */
                if (Index + SGUI_UNIT_FACTOR >= HandleSet.Length)
                    break;
                /* 如果最后的几个连续为空闲态单元达到伸缩标准时,压缩它 */
                if (Index + SGUI_UNIT_FACTOR < HandleSet.Length) {
                    /* 生成一个新的资源管理单元集合 */
                    SGui_Handle     Number = HandleSet.Number - SGUI_UNIT_FACTOR;
                    SGui_Handle     Length = HandleSet.Length - SGUI_UNIT_FACTOR;
                    SGui_HandleUnit *Units = SGUI_ALLOC(sizeof(SGui_HandleUnit) * Length);
                    /* 拷贝原生的资源管理单元集合 */
                    for (Index = 0; Index < Length; Index++)
                        Units[Index] = HandleSet.Units[Index];
                    /* 释放原生的资源管理单元集合 */
                    SGUI_FREE(HandleSet.Units);
                    /* 设置新生成的资源管理单元集合 */
                    HandleSet.Number = Number;
                    HandleSet.Length = Length;
                    HandleSet.Units  = Units;
                }
            }
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 句柄设置资源(设置,更新,清除) */
void SGui_HandleSourceSet(SGui_Handle Handle, void *Source)
{
    if (Handle >= SGUI_HANDLE_STATIC);
    if (Handle <  SGUI_HANDLE_STATIC) {
        SGui_Handle Index1 = Handle / SGUI_UNIT_LENGTH;
        SGui_Handle Index2 = Handle % SGUI_UNIT_LENGTH;
        /* 这是一个合法句柄 */
        if (Index1 < HandleSet.Length && Index2 < HandleSet.Units[Index1].Length)
            HandleSet.Units[Index1].Source[Index2] = Source;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 句柄获取资源 */
void * SGui_HandleSourceGet(SGui_Handle Handle)
{
    if (Handle >= SGUI_HANDLE_STATIC)
        return Handle < HandleStaticLength ? HandleStaticTable[Handle] : NULL;
    if (Handle <  SGUI_HANDLE_STATIC) {
        void *Source = NULL;
        SGui_Handle Index1 = Handle / SGUI_UNIT_LENGTH;
        SGui_Handle Index2 = Handle % SGUI_UNIT_LENGTH;
        /* 这是一个合法句柄 */
        if (Index1 < HandleSet.Length && Index2 < HandleSet.Units[Index1].Length)
            Source = HandleSet.Units[Index1].Source[Index2];
        
        return Source;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 是否静态句柄(静态句柄对应资源为静态资源) */
bool SGui_HandleStaticCheck(SGui_Handle Handle)
{
    return Handle >= SGUI_HANDLE_STATIC ? true :false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 静态句柄表绑定 */
void SGui_HandleStaticTableRegister(void **Resource, uint32_t Length)
{
    HandleStaticTable  = Resource;
    HandleStaticLength = Length;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_INTERNAL_TEST
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_Handle_Test(uint32_t (*Random)())
{
    #define SGUI_HANDLE_LENGTH_TEST  10
    uint32_t  Index = 0;
    uint32_t  Array[SGUI_HANDLE_LENGTH_TEST] = {0};
    uint32_t *Pointer = NULL;

    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++) {
        Array[Index] = SGui_HandleTake();
        SGui_HandleSourceSet(Array[Index], &(Array[Index]));
    }

    /* 打乱 */
    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++) {
        uint32_t Temp_Index = Random() % SGUI_HANDLE_LENGTH_TEST;
        uint32_t Temp = Array[Index];
        Array[Index] = Array[Temp_Index];
        Array[Temp_Index] = Temp;
    }

    SGUI_LOGMESSAGE("Array:");
    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++)
        SGUI_LOGMESSAGE("%d ", Array[Index]);
    SGUI_LOGMESSAGE("\n");

    /* 获取句柄资源 */
    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++) {
        Pointer = SGui_HandleSourceGet(Array[Index]);
        SGUI_LOGMESSAGE("Array[%d]:%d", Array[Index], *Pointer);
    }

    /* 释放 */
    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++)
        SGui_HandleGive(Array[Index]);

    /* 再次获取,验证 */
    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++) {
        Array[Index] = SGui_HandleTake();
        SGui_HandleSourceSet(Array[Index], &(Array[Index]));
    }

    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++) {
        Pointer = SGui_HandleSourceGet(Array[Index]);
        SGUI_LOGMESSAGE("Array[%d]:%d", Array[Index], *Pointer);
    }

    /* 释放 */
    for (Index = 0; Index < SGUI_HANDLE_LENGTH_TEST; Index++)
        SGui_HandleGive(Array[Index]);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
