/*实现目标:
 *消息引擎,用于记录不同类别的消息
 *多种不同类别的消息被归为特殊部分
 *而对外而言它仅仅是一个集成化的仓库
 */

/* 核心唯一依赖项 */
#include "SGuiCore.h"

typedef struct SimpleGuiMessageSet {
    uint32_t  index;    //集合编号
    uint32_t  number;   //集合消息集长度
    void    **source;   //集合消息集实体
} SGuiMessageSet;

/* 唯一集合集实例 */
static SGuiSList head = {0};
static SGuiSList tail = {0};

/* 生成一个消息集合 */
uint32_t SGuiMessageSetTake(uint32_t number)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0;
    /* 1.生成一个消息集合 */
    SGuiMessageSet *unit = SGUIMALLOC(sizeof(SGuiMessageSet));
    unit->number = number;
    unit->source = SGUIMALLOC(sizeof(void *) * unit->number);
    /* 初始化消息集合 */
    for (index = 0; index < number; index++)
        (unit->source)[index] = NULL;

    /* 2.将空闲单元以最小化索引加入链表 */
    SGuiSNodeUnitAdd(&head, &tail, unit);

    SGUISYNCPRIMITOPTSYNCMUTEX(false);

    return unit->index;
}

/* 释放一个消息集合 */
uint32_t SGuiMessageSetGive(uint32_t set)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0, value = 0;
    /* 1.搜索生成的空闲单元 */
    SGuiMessageSet *unit = SGuiSNodeUnitSearch(&head, &tail, set);
    /* 做一个简单的检查,如果仍然存在消息,不做释放 */
    for (index = 0; index < unit->number; index++)
        if ((unit->source)[index] != NULL) {
            value = -1;
            unit  = NULL;
            break;
        }
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

    return value;
}

/* 消息集合添加一个消息(先向) */
uint32_t SGuiMessageSetAdd(uint32_t set, SGuiMessage message)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0, value = -1;
    /* 1.搜索生成的空闲单元 */
    SGuiMessageSet *unit = SGuiSNodeUnitSearch(&head, &tail, set);
    if (unit == NULL);
    if (unit != NULL)
        for (index = 0; index < unit->number; index++)
            //找到一个空闲位置将消息投掷
            if ((unit->source)[index] == NULL) {
                (unit->source)[index]  = SGUIMALLOC(sizeof(SGuiMessage));
                //消息写入到指定位置
                *((SGuiMessage *)((unit->source)[index])) = message;
                value = 0;
                break;
            }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);

    return value;
}

/* 消息集合移除一个消息(先向, 先定向) */
uint32_t SGuiMessageSetRemove(uint32_t set, SGuiMessage *message)
{
    SGUISYNCPRIMITOPTSYNCMUTEX(true);

    uint32_t index = 0, value = -1;
    /* 1.搜索生成的空闲单元 */
    SGuiMessageSet *unit = SGuiSNodeUnitSearch(&head, &tail, set);
    if (unit == NULL);
    if (unit != NULL) {
        if (message->type == SGUIMSGFAKERTYPE)
            for (index = 0; index < unit->number; index++)
                //找到一个非空闲位置将消息提取
                if ((unit->source)[index] != NULL) {
                    //消息读取从指定位置
                    *message = *((SGuiMessage *)((unit->source)[index]));
                    SGUIFREE((unit->source)[index]);
                    (unit->source)[index] = NULL;
                    value = 0;
                    break;
                }
        if (message->type != SGUIMSGFAKERTYPE)
            for (index = 0; index < unit->number; index++)
                //找到一个非空闲位置将消息提取
                if ((unit->source)[index] != NULL && 
                    ((SGuiMessage *)((unit->source)[index]))->type == message->type) {
                    //消息读取从指定位置
                    *message = *((SGuiMessage *)((unit->source)[index]));
                    SGUIFREE((unit->source)[index]);
                    (unit->source)[index] = NULL;
                    value = 0;
                    break;
                }
    }

    SGUISYNCPRIMITOPTSYNCMUTEX(false);

    return value;
}
