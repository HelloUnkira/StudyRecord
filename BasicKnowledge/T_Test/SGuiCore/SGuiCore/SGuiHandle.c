/* 实现目标:
 * 设计简要的句柄资源管理器
 * 采用二级数组实现
 * 在保证查找效率为O(1)的条件下
 * 降低额外空间开销
 * 将时间成本转移到获取与释放部分
 * 因为句柄通常不需要频繁获取与释放
 */

/* 核心唯一依赖项 */
#include "SGuiCore.h"

/* 资源管理单元的伸缩因子 */
#define SGUIUNITSCALFACTOR  5       /* 暴力测试:1,推荐使用:5 */
/* 资源管理单元的句柄数量 */
#define SGUIUNITLENGTH      100     /* 暴力测试:1,推荐使用:50,100 */
/* 句柄计算规则:
 * index1 = handle / SGUIUNITLENGTH;
 * index2 = handle % SGUIUNITLENGTH;
 * handle = index1 * SGUIUNITLENGTH + index2;
 */

/* 资源管理单元 */
typedef struct SimpleGuiHandleUnit {
    uint32_t number;    //可使用句柄数量
    uint32_t length;    //句柄资源集合数量
    void   **source;    //句柄资源集合
} SGuiHandleUnit;

typedef struct SimpleGuiHandle {
    uint32_t number;        //可使用资源管理单元数量
    uint32_t length;        //句柄资源集合数量
    SGuiHandleUnit *units;  //资源管理单元集合
} SGuiHandle;

/* 句柄管理集合 */
static SGuiHandle handles = {0};

/* 获取一个空闲句柄 */
uint32_t SGuiHandleTake(void)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index  = 0;
    uint32_t index1 = 0;
    uint32_t index2 = 0;
    uint32_t handle = SGUIINVALIDHANDLE;
    /* 1.遍历资源管理单元,查找一个有空闲句柄的单元 */
    for (index1 = 0; index1 < handles.length; index1++) {
        if (handles.units[index1].source == NULL)
            break;
        if (handles.units[index1].number > 0)
            break;
    }
    /* 2.如果资源管理单元不足,扩张它 */
    if (index1 == handles.length) {
        /* 生成一个新的资源管理单元集合 */
        uint32_t number = handles.number + SGUIUNITSCALFACTOR;
        uint32_t length = handles.length + SGUIUNITSCALFACTOR;
        SGuiHandleUnit *units = SGUIMALLOC(sizeof(SGuiHandleUnit) * length);
        /* 拷贝原生的资源管理单元集合 */
        for (index = 0; index < handles.length; index++)
            units[index] = handles.units[index];
        /* 初始化新生成的资源管理单元集合 */
        for (index = handles.length; index < length; index++)
            units[index].source = NULL;
        /* 释放原生的资源管理单元集合 */
        SGUIFREE(handles.units);
        /* 设置新生成的资源管理单元集合 */
        handles.number = number;
        handles.length = length;
        handles.units = units;
    }
    /* 3.如果资源管理单元不存在,创建它 */
    if (handles.units[index1].source == NULL) {
        /* 生成单元句柄集合 */
        uint32_t number = SGUIUNITLENGTH;
        uint32_t length = SGUIUNITLENGTH;
        void   **source = SGUIMALLOC(sizeof(void *) * length);
        /* 初始化新生成的单元句柄集合 */
        for (index = 0; index < length; index++)
            source[index] = NULL;
        /* 设置新生成的资源管理单元集合 */
        handles.units[index1].number = number;
        handles.units[index1].length = length;
        handles.units[index1].source = source;
        handles.number--;
    }
    /* 4.寻找一个空闲句柄 */
    if (handles.units[index1].number > 0) {
        /* 遍历单元句柄集合,寻找一个空闲句柄 */
        for (index2 = 0; index2 < handles.units[index1].length; index2++)
            if (handles.units[index1].source[index2] == NULL) {
                handles.units[index1].source[index2]  = (void *)(~0);
                handles.units[index1].number--;
                break;
            }
    }
    /* 5.最终检查 */
    if (index1 < handles.length && index2 < handles.units[index1].length) {
        /* 计算句柄 */
        handle = index1 * SGUIUNITLENGTH + index2;
    }
    /*  */
    SGUISYNCPRIMITOPTSYNCMUTEX(false);
    /*  */
    return handle;
}

/* 释放一个句柄 */
void SGuiHandleGive(uint32_t handle)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    int32_t  index  = 0;
    uint32_t index1 = handle / SGUIUNITLENGTH;
    uint32_t index2 = handle % SGUIUNITLENGTH;
    /* 这是一个合法句柄 */
    if (index1 < handles.length && index2 < handles.units[index1].length) {
        /* 1.释放这个合法的句柄 */
        handles.units[index1].source[index2] = NULL;
        handles.units[index1].number++;
        /* 2.句柄管理单元为空时,释放它 */
        if (handles.units[index1].length == handles.units[index1].number) {
            /* 释放这个句柄管理单元 */
            SGUIFREE(handles.units[index1].source);
            /* 更新管理器的情景 */
            handles.units[index1].source = NULL;
            handles.number++;
            /* 如果句柄管理单元空闲过多,尝试压缩 */
            while (handles.number >= SGUIUNITSCALFACTOR) {
                /* 一级寻址依赖下标,所以只能逆向检查,中间空余部不可压缩 */
                for (index = handles.length - 1; index >= 0; index--)
                    if (handles.units[index].source != NULL)
                        break;
                /* 如果句柄管理单元全空闲,释放本身 */
                if (index < 0) {
                    /* 释放这个句柄管理单元集合 */
                    SGUIFREE(handles.units);
                    handles.number = 0;
                    handles.length = 0;
                    handles.units = NULL;
                    break;
                }
                /* 如果最后的几个连续为空闲态单元未达到伸缩标准时,放弃 */
                if (index + SGUIUNITSCALFACTOR >= handles.length)
                    break;
                /* 如果最后的几个连续为空闲态单元达到伸缩标准时,压缩它 */
                if (index + SGUIUNITSCALFACTOR < handles.length) {
                    /* 生成一个新的资源管理单元集合 */
                    uint32_t number = handles.number - SGUIUNITSCALFACTOR;
                    uint32_t length = handles.length - SGUIUNITSCALFACTOR;
                    SGuiHandleUnit *units = SGUIMALLOC(sizeof(SGuiHandleUnit) * length);
                    /* 拷贝原生的资源管理单元集合 */
                    for (index = 0; index < length; index++)
                        units[index] = handles.units[index];
                    /* 释放原生的资源管理单元集合 */
                    SGUIFREE(handles.units);
                    /* 设置新生成的资源管理单元集合 */
                    handles.number = number;
                    handles.length = length;
                    handles.units = units;
                }
            }
        }
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}

/* 句柄绑定或更新资源 */
void SGuiHandleSet(uint32_t handle, void *source)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index1 = handle / SGUIUNITLENGTH;
    uint32_t index2 = handle % SGUIUNITLENGTH;
    /* 这是一个合法句柄 */
    if (index1 < handles.length && index2 < handles.units[index1].length)
        handles.units[index1].source[index2] = source;

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}

/* 句柄获取资源 */
void * SGuiHandleGet(uint32_t handle)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    void *source = NULL;
    uint32_t index1 = handle / SGUIUNITLENGTH;
    uint32_t index2 = handle % SGUIUNITLENGTH;
    /* 这是一个合法句柄 */
    if (index1 < handles.length && index2 < handles.units[index1].length)
        source = handles.units[index1].source[index2];

    SGUISYNCPRIMITOPTSYNCMUTEX(false);

    return source;
}
