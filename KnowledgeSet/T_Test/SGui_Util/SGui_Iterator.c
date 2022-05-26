#include "SGui_Kernel.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#ifdef SGUI_ITERATOR_SINGLE_LIST
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 将一个空闲单元加入节点,并加入到链表(使用最小化索引) */
void SGui_SNodeUnitAdd(SGui_SList *List, void *Unit)
{
    /* 1.生成节点空间 */
    SGui_SNode *Node = SGUI_ALLOC(sizeof(SGui_SNode));
    /* 2.初始化空间 */
    SGui_SNodeReset(Node);
    SGui_SNodeSetMember(Node, Unit);
    /* 3.如果是第一个插入项 */
    if (SGui_SListGetHead(List) == NULL) {
        /* 索引可更新为起始点 */
        (*(uint32_t *)Unit) = 0;
        /* 插入链表头 */
        SGui_SListPrepend(List, Node);
        /*  */
        return;
    }
    /* 4.如果需要插入第一项 */
    if (SGui_SListGetHead(List) != NULL) {
        /*  */
        void *Temp = SGui_SNodeGetMember(SGui_SListGetHead(List));
        /*  */
        if ((*(uint32_t *)Temp) > 0) {
            /* 索引可更新为起始点 */
            (*(uint32_t *)Unit) = 0;
            /* 插入链表头 */
            SGui_SListPrepend(List, Node);
            /*  */
            return;
        }
    }
    /* 5.如果不是第一个插入项 */
    SGui_SNode *Current = NULL;
    /* 遍历链表 */
    SGui_SListTraserve(List, Current) {
        /*  */
        void *Temp = SGui_SNodeGetMember(Current);
        /* 1.如果是最后一项 */
        if (SGui_SNodeGetNear(Current) == NULL) {
            /* 索引可更新为下一点 */
            (*(uint32_t *)Unit) = (*(uint32_t *)Temp) + 1;
            /* 插入链表尾 */
            SGui_SListSetTail(List, Node);
            /*  */
            return;
        }
        /*  */
        void *Temp1 = SGui_SNodeGetMember(SGui_SNodeGetNear(Current));
        /* 2.检查中间项 */
        if (SGui_SNodeGetNear(Current) != NULL) {
            /* 如果之间不支持插入新单元 */
            if ((*(uint32_t *)Temp) + 1 == (*(uint32_t *)Temp1))
                continue;
            /* 如果之间支持插入新单元 */
            if ((*(uint32_t *)Temp) + 1 != (*(uint32_t *)Temp1)) {
                /* 索引可更新为下一点 */
                (*(uint32_t *)Unit) = (*(uint32_t *)Temp) + 1;
                /* 插入节点尾 */
                SGui_SListInsert(List, Current, Node);
                /*  */
            }
        }
    }
    /* 不能到达这里 */
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 将一个空闲单元移除节点,并移除出链表(使用最小化索引) */
void SGui_SNodeUnitRemove(SGui_SList *List, void *Unit)
{
    /*  */
    SGui_SNode *Current = NULL;
    SGui_SNode *Node    = NULL;
    /* 头结点要单独考虑(因为是单链表) */
    if (SGui_SNodeGetMember(SGui_SListGetHead(List)) == Unit) {
        /*  */
        Node = SGui_SListGetHead(List);
        /* 节点移除出链表 */
        SGui_SListRemove(List, Current, Node);
        /* 释放节点空间 */
        SGUI_FREE(Node);
        return;
    }
    /* 遍历链表 */
    SGui_SListTraserve(List, Current) {
        /* 跳过意外场景 */
        if (SGui_SNodeGetNear(Current) == NULL)
            break;
        /* 搜索目标 */
        if (SGui_SNodeGetMember(SGui_SNodeGetNear(Current)) == Unit) {
            Node = SGui_SNodeGetNear(Current);
            break;
        }
    }
    /* 寻找到了空闲单元 */
    if (Node != NULL) {
        /* 节点移除出链表 */
        SGui_SListRemove(List, Current, Node);
        /* 释放节点空间 */
        SGUI_FREE(Node);
    }
    /* 不能到达这里 */
    /* 没有寻找到了空闲单元 */
    if (Node == NULL)
        return;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 将一个空闲单元寻找到(使用最小化索引) */
void * SGui_SNodeUnitSearch(SGui_SList *List, uint32_t Index)
{
    void      *Unit = NULL;
    SGui_SNode *Node = NULL;
    /* 遍历链表,确认是哪一个管理单元 */
    SGui_SListTraserve(List, Node) {
        /*  */
        Unit = SGui_SNodeGetMember(Node);
        /* 寻找目标单元 */
        if ((*(uint32_t *)Unit) == Index)
            break;
        if ((*(uint32_t *)Unit) != Index)
            continue;
    }

    if (Node != NULL);
    if (Node == NULL)
        Unit = NULL;

    return Unit;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

