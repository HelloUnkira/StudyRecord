/* 实现目标:
 * 设计简要的容器管理阵列
 * 容器是一个控件属性
 * 用于管理多子控件
 */

/* 核心唯一依赖项 */
#include "SGuiCore.h"

/* 资源容器集合扩张缩减因子 */
/* 数量越大,额外维护空间越小 */
/* 数量越小,内存需求弹性越高 */
#define SGUISCALFACTOR    5

typedef struct SimpleGuiContainer {
    uint32_t   index;  //当前单元编号
    uint32_t   number; //控件句柄数量
    uint32_t  *source; //控件句柄集合
} SGuiContainer;

static SGuiSList head = {0};
static SGuiSList tail = {0};

/* 获得一个容器 */
uint32_t SGuiContainerTake(void)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0;
    /* 1.生成一个空闲单元 */
    SGuiContainer *unit = SGUIMALLOC(sizeof(SGuiContainer));
    unit->number = SGUISCALFACTOR;
    unit->source = SGUIMALLOC(sizeof(uint32_t) * (unit->number));
    /* 初始化空闲单元 */
    for (index = 0; index < unit->number; index++)
        (unit->source)[index] = SGUIINVALIDHANDLE;

    /* 2.将空闲单元以最小化索引加入链表 */
    SGuiSNodeUnitAdd(&head, &tail, unit);

    SGUISYNCPRIMITOPTSYNCMUTEX(false);

    return unit->index;
}

/* 释放一个容器 */
void SGuiContainerGive(uint32_t container)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    /* 1.搜索生成的空闲单元 */
    SGuiContainer *unit = SGuiSNodeUnitSearch(&head, &tail, container);
    /* 2.释放一个空闲单元 */
    if (unit == NULL);
    if (unit != NULL) {
        /* 生成与释放顺序相反 */
        /* 虽然unit会变成野指针,但我们不会拿它干坏事 */
        SGUIFREE(unit->source);
        SGUIFREE(unit);
        /* 将空闲单元移除出以最小化索引链表 */
        SGuiSNodeUnitRemove(&head, &tail, unit);
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}

/* 容器添加一个子控件 */
void SGuiContainerAdd(uint32_t container, uint32_t handle)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0;
    /* 1.搜索生成的空闲单元 */
    SGuiContainer *unit = SGuiSNodeUnitSearch(&head, &tail, container);
    /* 2.空闲单元添加资源 */
    if (unit == NULL);
    if (unit != NULL) {
        /* 遍历容器集合,将其添加到末尾 */
        for (index = 0; index < unit->number; index++)
            if ((unit->source)[index] == SGUIINVALIDHANDLE)
                break;
    }
    if (unit != NULL) {
        /* 如果遍历到了末尾 */
        if (index == unit->number) {
            /* 生成一个扩展容器集合 */
            uint32_t  number = unit->number + SGUISCALFACTOR;
            uint32_t *source = SGUIMALLOC(sizeof(uint32_t) * number);
            /* 拷贝原生容器集合 */
            for (index = 0; index < unit->number; index++)
                source[index] = (unit->source)[index];
            /* 初始化扩展端部 */
             for (index = unit->number; index < number; index++)
                source[index] = SGUIINVALIDHANDLE;
            /* 释放原生容器集合 */
            SGUIFREE(unit->source);
            /*  */
            index = unit->number;
            /* 设置扩展容器 */
            unit->source = source;
            unit->number = number;
        }
        /* 设置子控件 */
        (unit->source)[index] = handle;
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}

/* 容器移除一个子控件 */
void SGuiContainerRemove(uint32_t container, uint32_t handle)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0;
    /* 1.搜索生成的空闲单元 */
    SGuiContainer *unit = SGuiSNodeUnitSearch(&head, &tail, container);
    /* 2.空闲单元移除资源 */
    if (unit == NULL);
    if (unit != NULL) {
        /* 遍历容器集合,寻找到目标资源 */
        for (index = 0; index < unit->number; index++)
            if ((unit->source)[index] == handle) {
                (unit->source)[index]  = SGUIINVALIDHANDLE;
                break;
            }
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}

/* 获取容器集合 */
void SGUIContainerGet(uint32_t container, uint32_t **source, uint32_t *number)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0;
    /* 1.搜索生成的空闲单元 */
    SGuiContainer *unit = SGuiSNodeUnitSearch(&head, &tail, container);
    /* 2.空闲单元获取资源 */
    if (unit == NULL);
    if (unit != NULL) {
        //*source = unit->source;
        //*number = unit->number;
        /* 注意:模组资源是内部全局保护,所以不可共享到外界 */
        *source = SGUIMALLOC(sizeof(uint32_t) * (unit->number));
        *number = unit->number;
        /* 资源拷贝到外界 */
        for (index = 0; index < unit->number; index++)
            (*source)[index] = (unit->source)[index];
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}
