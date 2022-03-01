/* 实现目标:
 * 设计简要的剪切域管理阵列
 * 剪切域管理阵列服务于多剪切域控件
 * 剪切域统一相对父窗口的偏移做计算
 */

/* 核心唯一依赖项 */
#include "SGuiCore.h"

/* 资源单元剪切域集合扩张缩减因子 */
/* 数量越大,额外维护空间越小 */
/* 数量越小,内存需求弹性越高 */
#define SGUISCALFACTOR    5

/* 资源管理单元 */
typedef struct SimpleGuiClipRectsUnit {
    uint32_t    index;  //当前单元编号
    uint32_t    number; //可使用剪切域数量
    uint32_t    length; //总剪切域数量
    SGuiDot2    master; //剪切域主域
    SGuiDot2   *slave;  //剪切域资源集合
} SGuiClipRectsUnit;

static SGuiSList head = {0};
static SGuiSList tail = {0};

/* 简化1: 剪切域阵列递归添加剪切域 */
static void SGuiClipRectsUnitRecurveMerge(SGuiClipRectsUnit *unit, SGuiDot2 rect)
{
    /* 如果剪切域可以合并 */
    /* 合并后的剪切域要继续检查 */
    /* 如果走到末尾,则无法继续合并 */
    int32_t index = 0, recover = unit->length;
    /* 合并剪切域 */
    for (index = 0; index < unit->length; index++) {
        /* 出现一个有效的剪切域 */
        if (SGuiClipRectIsVaild((unit->slave)[index]) == true) {
            /* 创建一个临时剪切域 */
            SGUIDOT2DEFINE(temp);
            /* 尝试联合剪切域 */
            temp = SGuiClipRectUnion(rect, (unit->slave)[index]);
            /* 联合剪切域成功时 */
            if (SGuiClipRectIsVaild(temp) == true) {
                /* 移除原生剪切域 */
                SGUIDOT2RESET((unit->slave)[index]);
                /* 扣除记录 */
                unit->number++;
                /* 获得合并后剪切域 */
                rect = temp;
                /* 这个位点的剪切域已经失效,考虑纪录索引 */
                recover = recover < index ? recover : index;
                /* 将索引拉到最开始,重制整个流程 */
                index = -1;
            }
            continue;
        }
        /* 这个位点的剪切域已经失效,考虑纪录索引 */
        recover = recover < index ? recover : index;
    }
    /* 2.将最终剪切域载入到剪切域阵列单元中 */
    if (recover == unit->length);
    if (recover != unit->length) {
        /* 载入合并后的剪切域(如果可以合并) */
        (unit->slave)[recover] = rect;
        /* 添加记录 */
        unit->number--;
    }
}

/* 简化2: 合并生成剪切域阵列 */
static void SGuiClipRectsUnitMerge(SGuiClipRectsUnit *unit1, SGuiClipRectsUnit *unit2)
{
    /* 1.生成剪切域阵列单元 */
    SGuiClipRectsUnit *unit = SGUIMALLOC(sizeof(SGuiClipRectsUnit));
    /* 2.计算合并空间 */
    unit->number = unit1->number + unit2->number;
    unit->length = unit1->length + unit2->length;
    /* 如果合并空间存在大量空闲,则取就近值(此循环应稳定执行0~1次) */
    while (unit->number >= SGUISCALFACTOR) {
           unit->number -= SGUISCALFACTOR;
           unit->length -= SGUISCALFACTOR;
    }

    /* 3.生成合并空间 */
    unit->slave = SGUIMALLOC(sizeof(SGuiDot2) * (unit->length));
    /*  */
    uint32_t index = 0, count = 0;
    /* 4.拷贝剪切域资源 */
    for (index = 0; index < unit1->length; index++)
        if (SGuiClipRectIsVaild((unit1->slave)[index]) == true)
            (unit->slave)[count++] = (unit1->slave)[index];
    for (index = 0; index < unit2->length; index++)
        if (SGuiClipRectIsVaild((unit2->slave)[index]) == true)
            (unit->slave)[count++] = (unit2->slave)[index];
    /* 5.检查失败:销毁剪切域阵列单元,返回空 */
    if (count + unit->number != unit->length) {
        SGUIFREE(unit->slave);
    }
    /* 5.检查成功:返回剪切域阵列单元 */
    if (count + unit->number == unit->length) {
        /* 先释放原生剪切域 */
        SGUIFREE(unit1->slave);
        /* 新剪切域覆盖原生剪切域 */
        unit1->number = unit->number;
        unit1->length = unit->length;
        unit1->slave  = unit->slave;
    }
    SGUIFREE(unit);
}

/* 获取一个剪切域集合 */
uint32_t SGuiClipRectsTake(void)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    SGUIDOT2DEFINE(invalid);
    /* 1.生成一个剪切域阵列单元 */
    SGuiClipRectsUnit *unit = SGUIMALLOC(sizeof(SGuiClipRectsUnit));
    /* 2.初始化生成空间 */
    unit->number = SGUISCALFACTOR;
    unit->length = SGUISCALFACTOR;
    unit->master = invalid;
    unit->slave  = SGUIMALLOC(sizeof(SGuiDot2) * (unit->length));
    /*  */
    uint32_t index = 0;
    /* 3.重置全部剪切域 */
    for (index = 0; index < unit->length; index++)
        SGUIDOT2RESET((unit->slave)[index]);
    /* 4.剪切域阵列单元加入到阵列中 */
    SGuiSNodeUnitAdd(&head, &tail, unit);
    /* 5.返回剪切域阵列单元标号 */
    index = unit->index;

    SGUISYNCPRIMITOPTSYNCMUTEX(false);

    return index;
}

/* 释放一个剪切域集合 */
void SGuiClipRectsGive(uint32_t rects)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    /* 1.遍历链表,确认是哪一个管理单元 */
    SGuiClipRectsUnit *unit = SGuiSNodeUnitSearch(&head, &tail, rects);
    /* 2.剪切域阵列单元移除出阵列中 */
    if (unit == NULL);
    if (unit != NULL) {
        /* 释放一个空闲单元 */
        /* 生成与释放顺序相反 */
        /* 虽然unit会变成野指针,但我们不会拿它干坏事 */
        SGUIFREE(unit->slave);
        SGUIFREE(unit);
        /* 将空闲单元移除出以最小化索引链表 */
        SGuiSNodeUnitRemove(&head, &tail, unit);
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}

/* 为剪切域集合设置主域 */
void SGuiClipRectsMasterSet(uint32_t rects, SGuiDot2 rect)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    /* 1.遍历链表,确认是哪一个管理单元 */
    SGuiClipRectsUnit *unit = SGuiSNodeUnitSearch(&head, &tail, rects);
    /* 2.剪切域阵列单元设置主域 */
    if (unit == NULL);
    if (unit != NULL) {
        /* 设置主域 */
        unit->master = rect;

        /* 重置剪切域空间 */
        SGUIFREE(unit->slave);

        unit->number = SGUISCALFACTOR;
        unit->length = SGUISCALFACTOR;
        unit->slave  = SGUIMALLOC(sizeof(SGuiDot2) * (unit->length));
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}

/* 为剪切域集合获取主域 */
SGuiDot2 SGuiClipRectsMasterGet(uint32_t rects)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    SGUIDOT2DEFINE(rect);
    /* 1.遍历链表,确认是哪一个管理单元 */
    SGuiClipRectsUnit *unit = SGuiSNodeUnitSearch(&head, &tail, rects);
    /* 2.剪切域阵列单元设置主域 */
    if (unit == NULL);
    if (unit != NULL) {
        /* 获取主域 */
        rect = unit->master;
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);

    return rect;
}

/* 剪切域集合添加剪切域 */
void SGuiClipRectsSlaveAdd(uint32_t rects, SGuiDot2 rect)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    /* 1.遍历链表,确认是哪一个管理单元 */
    SGuiClipRectsUnit *unit = SGuiSNodeUnitSearch(&head, &tail, rects);
    /* 2.剪切域阵列单元添加剪切域 */
    if (unit == NULL);
    if (unit != NULL) {
        /* 先确认有效剪切域 */
        rect = SGuiClipRectAnd(unit->master, rect);
        /* 剪切域递归迭代到剪切域阵列中 */
        if (SGuiClipRectIsVaild(rect) == true)
            SGuiClipRectsUnitRecurveMerge(unit, rect);
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}

/* 剪切域集合移除剪切域 */
void SGuiClipRectsSlaveRemove(uint32_t rects, SGuiDot2 rect)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t count4    =  0;
    SGuiDot2 rects4[4] = {0};
    int32_t index = 0, index1 = 0;
    /* 1.遍历链表,确认是哪一个管理单元 */
    SGuiClipRectsUnit *unit  = SGuiSNodeUnitSearch(&head, &tail, rects);
    SGuiClipRectsUnit *unit1 = NULL;
    /* 2.先确认剪切域的有效性 */
    if (unit == NULL);
    if (unit != NULL) {
        /* 计算剪切域是否与主域相交 */
        rect = SGuiClipRectAnd(unit->master, rect);
        /* 如果剪切域与主域不相交,无法移除 */
        if (SGuiClipRectIsVaild(rect) == false)
            return;
    }
    /* 3.生成一个临时的剪切域集合,进行过程记录 */
    if (unit != NULL) {
        unit1         = SGUIMALLOC(sizeof(SGuiClipRectsUnit));
        unit1->number = SGUISCALFACTOR;
        unit1->length = SGUISCALFACTOR;
        unit1->slave  = SGUIMALLOC(sizeof(SGuiDot2) * (unit1->length));
        /*  */
        for (index = 0; index < unit1->length; index++)
            SGUIDOT2RESET((unit1->slave)[index]);
    }
    /* 4.剪切域阵列单元移除剪切域 */
    if (unit != NULL) {
        /* 迭代所有剪切域 */
        for (index = 0; index < unit->length; index++) {
            /* 1.跳过一个无效的剪切域 */
            if (SGuiClipRectIsVaild((unit->slave)[index]) == false)
                continue;
            /* 2.计算与有效剪切域的交集 */
            SGuiDot2 rect_sub = SGuiClipRectAnd(rect, (unit->slave)[index]);
            /* 3.跳过不相交的有效剪切域 */
            if (SGuiClipRectIsVaild(rect_sub) == false)
                continue;
            /* 4.计算剪切域差集 */
            count4 = SGuiClipRectSub(rects4, (unit->slave)[index], rect_sub);
            /* 5.将剪切域差集保留到本地剪切域集合中 */
            /* 5.1.空间不够记录时,扩充记录 */
            while (unit1->number < count4) {
                /* 创建扩充记录 */
                uint32_t length = unit1->length + SGUISCALFACTOR;
                SGuiDot2  *temp = SGUIMALLOC(sizeof(SGuiDot2) * length);
                /* 拷贝原生记录 */
                for (index1 = 0; index1 < unit1->length; index1++)
                    temp[index1] = (unit1->slave)[index1];
                /* 重置新增空间 */
                for (index1 = index1; index1 < length; index1++)
                    SGUIDOT2RESET((unit1->slave)[index1]);

                SGUIFREE(unit1->slave);

                unit1->number += SGUISCALFACTOR;
                unit1->length += SGUISCALFACTOR;
                unit1->slave   = temp;
            }
            /* 5.2.空间足够记录时,直接记录 */
            if (unit1->number >= count4)
                for (index1 = 0; index1 < count4; index1++)
                    SGuiClipRectsUnitRecurveMerge(unit1, rects4[index1]);
            /* 6.移除原生剪切域 */
            SGUIDOT2RESET((unit->slave)[index]);
            unit->number++;
        }
    }
    /* 5.新生成剪切域集合归并到剪切域集中 */
    if (unit != NULL) {
        SGuiClipRectsUnitMerge(unit, unit1);
        /* 销毁临时空间 */
        SGUIFREE(unit1->slave);
        SGUIFREE(unit1);
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}

/* 获取剪切域集合 */
void SGuiClipRectsSlaveGet(uint32_t rects, SGuiDot2 **slave, uint32_t *length)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0;
    /* 1.遍历链表,确认是哪一个管理单元 */
    SGuiClipRectsUnit *unit  = SGuiSNodeUnitSearch(&head, &tail, rects);
    /* 2.先确认剪切域的有效性 */
    if (unit != NULL) {
        //*slave  = unit->slave;
        //*length = unit->length;
        /* 注意:模组资源是内部全局保护,所以不可共享到外界 */
        *slave  = SGUIMALLOC(sizeof(SGuiDot2) * (unit->length));
        *length = unit->length;
        /* 资源拷贝到外界 */
        for (index = 0; index < unit->length; index++)
            (*slave)[index] = (unit->slave)[index];
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);
}
