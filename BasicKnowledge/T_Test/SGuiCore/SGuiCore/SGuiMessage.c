/*实现目标:
 *消息引擎,用于记录不同类别的消息
 *多种不同类别的消息被归为特殊部分
 *而对外而言它仅仅是一个集成化的仓库
 */

/* 核心唯一依赖项 */
#include "SGuiCore.h"

typedef struct SimpleGuiMessageQueue {
    uint32_t  index;    //队列编号
    uint32_t  number;   //队列消息集数量
    uint32_t  length;   //队列消息集长度
    void    **source;   //消息集
} SGuiMessageQueue;

/* 唯一队列集实例 */
static SGuiSList head = {0};
static SGuiSList tail = {0};

/* 生成一个消息队列 */
uint32_t SGuiMessageQueueTake(uint32_t length)
{
    SGuiSyncPrimitOptSync(true);

    uint32_t index = 0;
    /* 1.生成一个消息队列 */
    SGuiMessageQueue *unit = SGUIMALLOC(sizeof(SGuiMessageQueue));
    unit->number = 0;
    unit->length = length;
    unit->queue  = SGUIMALLOC(sizeof(void *) * queue->length);
    /* 初始化消息队列 */
    for (index = 0; index < length; index++)
        (unit->queue)[index] = NULL;

    /* 2.将空闲单元以最小化索引加入链表 */
    SGuiSNodeUnitAdd(&head, &tail, unit);

    SGuiSyncPrimitOptSync(false);

    return unit->index;
}

/* 释放一个消息队列 */
uint32_t SGuiMessageQueueGive(uint32_t queue)
{
    SGuiSyncPrimitOptSync(true);

    uint32_t index = 0, value = 0;
    /* 1.搜索生成的空闲单元 */
    SGuiMessageQueue *unit = SGuiSNodeUnitSearch(&head, &tail, queue);
    /* 做一个简单的检查,如果仍然存在消息,不做释放 */
    for (index = 0; index < unit->length; index++)
        if ((unit->queue)[index] != NULL) {
            value = -1;
            unit  = NULL;
            break;
        }
    /* 2.释放一个空闲单元 */
    if (unit == NULL);
    if (unit != NULL) {
        /* 生成与释放顺序相反 */
        /* 虽然unit会变成野指针,但我们不会拿它干坏事 */
        SGUIFREE(unit->queue);
        SGUIFREE(unit);
        /* 将空闲单元移除出以最小化索引链表 */
        SGuiSNodeUnitRemove(&head, &tail, unit);
    }

    SGuiSyncPrimitOptSync(false);

    return value;
}

/* 消息队列添加一个消息 */
void SGuiMessageQueueAdd(uint32_t queue, SGuiMessage message)
{
    SGuiSyncPrimitOptSync(true);

    uint32_t index = 0;
    /* 1.搜索生成的空闲单元 */
    SGuiMessageQueue *unit = SGuiSNodeUnitSearch(&head, &tail, queue);
    if (unit == NULL);
    if (unit != NULL) {
        if (unit->number < unit->length) {
            for (index = 0; index < unit->length; index++) {
                //空指针在非空指针之后
                if ((unit->queue)[index] == NULL) {

                    

                    break;
                }

                if ((unit->queue)[index]->type != message.type)
                    continue;
                if (message.merge == NULL)
                    continue;

                SGUIMSGPARAMTYPE parameter[SGUIMSGPARAMLEN] = {0};

                message.merge(parameter,  message->parameter,
                             (unit->queue)[index]->parameter);
                
                for (index = 0; index < SGUIMSGPARAMLEN; index++)
                    (unit->queue)[index]->parameter[index] = parameter[index];
                    
                    break;
            }
        }
    }

    SGuiSyncPrimitOptSync(false);
}





