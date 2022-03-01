
#include "SGuiPort.h"
#include "SGuiCommon.h"

/* 实现目标:
 * 定制化,全局化的辅助优化策略
 * 用于为核心提供某些资源
 * 它必须得以依赖编译器优化策略
 */

/* 单链表宏迭代器定制化(用于简化边缘逻辑) */
/* 成员首项带索引index的索引最小化生成策略 */
/* (注意:它引入了平台依赖以及模组依赖): */
/* 根据当前使用情况额外引入的一项定制 */
/* 要求每一个使用该功能的模组,unit第一个元素是以下类型: */
/* uint32_t index;  //当前单元编号 */
#define SGUISNODESPECIALDESIGN
#ifdef  SGUISNODESPECIALDESIGN
/* 将一个空闲单元加入节点,并加入到链表(使用最小化索引) */
void SGuiSNodeUnitAdd(SGuiSNode *head, SGuiSNode *tail, void *unit)
{
    /* 1.生成节点空间 */
    SGuiSNode *node = SGUIMALLOC(sizeof(SGuiSNode));
    /* 2.初始化空间 */
    SNODE_RESET(node);
    SNODE_SET(node, unit);
    /* 3.如果是第一个插入项 */
    if (SLIST_HEAD(head) == NULL) {
        /* 索引可更新为起始点 */
        (*(uint32_t *)unit) = 0;
        /* 插入链表头 */
        SLIST_PREPEND(head, tail, node);
        /*  */
        return;
    }
    /* 4.如果需要插入第一项 */
    if (SLIST_HEAD(head) != NULL) {
        /*  */
        void *temp = SNODE_GET(SLIST_HEAD(head));
        /*  */
        if ((*(uint32_t *)temp) > 0) {
            /* 索引可更新为起始点 */
            (*(uint32_t *)unit) = 0;
            /* 插入链表头 */
            SLIST_PREPEND(head, tail, node);
            /*  */
            return;
        }
    }
    /* 5.如果不是第一个插入项 */
    SGuiSNode *current = NULL;
    /* 遍历链表 */
    SLIST_TRAVERSE(head, tail, current) {
        /*  */
        void *temp = SNODE_GET(current);
        /* 1.如果是最后一项 */
        if (SNODE_NEXT(current) == NULL) {
            /* 索引可更新为下一点 */
            (*(uint32_t *)unit) = (*(uint32_t *)temp) + 1;
            /* 插入链表尾 */
            SLIST_APPEND(head, tail, node);
            /*  */
            return;
        }
        /*  */
        void *temp1 = SNODE_GET(SNODE_NEXT(current));
        /* 2.检查中间项 */
        if (SNODE_NEXT(current) != NULL) {
            /* 如果之间不支持插入新单元 */
            if ((*(uint32_t *)temp) + 1 == (*(uint32_t *)temp1))
                continue;
            /* 如果之间支持插入新单元 */
            if ((*(uint32_t *)temp) + 1 != (*(uint32_t *)temp1)) {
                /* 索引可更新为下一点 */
                (*(uint32_t *)unit) = (*(uint32_t *)temp) + 1;
                /* 插入节点尾 */
                SLIST_INSERT(head, tail, current, node);
                /*  */
            }
        }
    }
    /* 不能到达这里 */
}

/* 将一个空闲单元移除节点,并移除出链表(使用最小化索引) */
void SGuiSNodeUnitRemove(SGuiSNode *head, SGuiSNode *tail, void *unit)
{
    /*  */
    SGuiSNode *current = NULL;
    SGuiSNode *node    = NULL;
    /* 头结点要单独考虑(因为是单链表) */
    if (SNODE_GET(SLIST_HEAD(head)) == unit) {
        /*  */
        node = SLIST_HEAD(head);
        /* 节点移除出链表 */
        SLIST_REMOVE(head, tail, current, node);
        /* 释放节点空间 */
        SGUIFREE(node);
        return;
    }
    /* 遍历链表 */
    SLIST_TRAVERSE(head, tail, current) {
        /* 跳过意外场景 */
        if (SNODE_NEXT(current) == NULL)
            break;
        /* 搜索目标 */
        if (SNODE_GET(SNODE_NEXT(current)) == unit) {
            node = SNODE_NEXT(current);
            break;
        }
    }
    /* 寻找到了空闲单元 */
    if (node != NULL) {
        /* 节点移除出链表 */
        SLIST_REMOVE(head, tail, current, node);
        /* 释放节点空间 */
        SGUIFREE(node);
    }
    /* 不能到达这里 */
    /* 没有寻找到了空闲单元 */
    if (node == NULL)
        return;
}

/* 将一个空闲单元寻找到(使用最小化索引) */
void * SGuiSNodeUnitSearch(SGuiSNode *head, SGuiSNode *tail, uint32_t index)
{
    void      *unit    = NULL;
    SGuiSNode *current = NULL;
    /* 遍历链表,确认是哪一个管理单元 */
    SLIST_TRAVERSE(head, tail, current) {
        /*  */
        unit = SNODE_GET(current);
        /* 寻找目标单元 */
        if ((*(uint32_t *)unit) == index)
            break;
        if ((*(uint32_t *)unit) != index)
            continue;
    }

    if (current != NULL);
    if (current == NULL)
        unit     = NULL;

    return unit;
}

#endif

/* 同步原语操作集成化(用于简化边缘逻辑) */
#define SGUISYNCPRIMITSTSPECIALDESIGN
#ifdef  SGUISYNCPRIMITSTSPECIALDESIGN

/* 操作同步原语(true:启用保护,false:禁用保护) */
void SGuiSyncPrimitOptSyncMutex(SGuiSyncPrimit **Mutex,
                                void            *Variable,
                                bool            *Init,
                                bool            *DeInit,
                                bool             StartOrEnd)
{
    /* 同步原语的初始化 */
    if (*Init == false) {
        *Init  = true;
        /* 初始化 */
        *Mutex = SGuiSyncPrimitCreate(SGUIMUTEX);
        /* 设置一个假资源防止返回NULL */
        SGuiSyncPrimitSetResource(*Mutex, Variable);
    }
    /* 同步原语的反初始化(什么时候才算正式放弃使用资源了呢?) */
    if (*DeInit == false) {
        *DeInit  = true;
        //SGuiSyncPrimitDestory(*Mutex);
    }
    /* 获取假资源 */
    if (StartOrEnd == true) {
        uint8_t * pointer = SGuiSyncPrimitTakeResource(*Mutex);
        /* 可以简要添加内容去确认同步原语是否有效 */
    }
    /* 释放假资源 */
    if (StartOrEnd == false) {
        SGuiSyncPrimitGiveResource(*Mutex);
    }
}

#endif


