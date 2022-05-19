#ifndef SGUIITERATOR_H
#define SGUIITERATOR_H

/* 实现目标:
 * 宏迭代器(这里只需要提供部分迭代策略)
 * 注意:此处使用的是普通版本的宏迭代器
 *      稳定在几个单位级别的额外字节开销
 *      内嵌版本的宏迭代器高效实现一般需要系统宏接口
 */

#define SGUIITERATORDOUBLELIST
#ifdef  SGUIITERATORDOUBLELIST

/* 双向链表宏迭代器 */
typedef struct SimpleGuiIterator1 {
    /*  */
    union {
        struct SimpleGuiIterator1 *head;//双向链表头
        struct SimpleGuiIterator1 *next;//双向链表下一节点
    };
    union {
        struct SimpleGuiIterator1 *tail;//双向链表尾
        struct SimpleGuiIterator1 *prev;//双向链表上一节点
    };
    /*  */
    void * member;
    /*  */
} SGuiIterator1;

/* 对于(list)它的成员是head和tail */
/* 对于(node)它的成员是prev和next */
typedef SGuiIterator1 SGuiDList;
typedef SGuiIterator1 SGuiDNode;

/* 遍历链表(这里不带成员量) */
#define DLIST_TRAVERSE(list, node) \
    for ((node)  = (list)->head;   \
         (node) != ((void *)0);    \
         (node)  = (node)->next)

/* 节点头追加 */
#define DLIST_PREPEND(list, node) \
do {                              \
    (node)->next = (list)->head;  \
if ((list)->head != ((void *)0))  \
    (list)->head->prev = (node);  \
if ((list)->head == ((void *)0))  \
    (list)->tail = (node);        \
    (list)->head = (node);        \
} while (0)

/* 节点尾追加 */
#define DLIST_APPEND(list, node) \
do {                             \
    (node)->prev = (list)->tail; \
if ((list)->tail != ((void *)0)) \
    (list)->tail->next = (node); \
if ((list)->tail == ((void *)0)) \
    (list)->head = (node);       \
    (list)->tail = (node);       \
} while (0)

/* 节点前插入 */
#define DLIST_PREPEND_INSERT(list, target, node) \
do {                                             \
    /* 基础插入动作 */                           \
    (node)->prev = (target)->prev;               \
    (node)->next = (target);                     \
    /* 更新后项节点 */                           \
    (target)->prev = (node);                     \
    /* 前项节点存在时 */                         \
if ((node)->prev != ((void *)0))                 \
    (node)->prev->next = (node);                 \
    /* 前项节点不存在时,(target)是首项 */        \
if ((node)->prev == ((void *)0))                 \
    (list)->head = (node);                       \
} while (0)

/* 节点后插入 */
#define DLIST_APPEND_INSERT(list, target, node) \
do {                                            \
    /* 基础插入动作 */                          \
    (node)->next = (target)->next;              \
    (node)->prev = (target);                    \
    /* 更新前项节点 */                          \
    (target)->next = (node);                    \
    /* 后项节点存在时 */                        \
if ((node)->next != ((void *)0))                \
    (node)->next->prev = (node);                \
    /* 后项节点不存在时,(target)是尾项 */       \
if ((node)->next == ((void *)0))                \
    (list)->tail = (node);                      \
} while (0)

/* 节点移除 */
#define DLIST_REMOVE(list, node)       \
do {                                   \
    /* 链表头尾节点一并检查 */         \
if ((node)->prev == ((void *)0))       \
    (list)->head  = (node)->next;      \
if ((node)->next == ((void *)0))       \
    (list)->tail  = (node)->prev;      \
    /* 接触关联索引 */                 \
if ((node)->prev != ((void *)0))       \
    (node)->prev->next = (node)->next; \
if ((node)->next != ((void *)0))       \
    (node)->next->prev = (node)->prev; \
    /* 自己节点关联项无需清除 */       \
} while (0)

#define DNODE_RESET(node)         \
do {                              \
    (node)->prev   = ((void *)0); \
    (node)->next   = ((void *)0); \
    (node)->member = ((void *)0); \
} while (0)

#define DLIST_HEAD(list) (list)->head
#define DLIST_TAIL(list) (list)->tail

#define DNODE_PREV(node) (node)->prev
#define DNODE_NEXT(node) (node)->next

#define DNODE_SET(node, target) (node)->member = (target)
#define DNODE_GET(node)         (node)->member

#endif

#define SGUIITERATORSINGLELIST
#ifdef  SGUIITERATORSINGLELIST

/* 单向链表宏迭代器 */
typedef struct SimpleGuiIterator2 {
    /*  */
    union {
        struct SimpleGuiIterator2 *head;//单向链表头
        struct SimpleGuiIterator2 *tail;//单向链表尾
        struct SimpleGuiIterator2 *next;//单向链表下一项
    };
    /*  */
    void * member;
    /*  */
} SGuiIterator2;

/* 对于(list)它的成员是head或tail */
/* 对于(node)它的成员是next */
typedef SGuiIterator2 SGuiSList;
typedef SGuiIterator2 SGuiSNode;

/* 遍历链表(这里不带成员量) */
#define SLIST_TRAVERSE(head_, tail_, node) \
    for ((node)  = (head_)->head;          \
         (node) != ((void *)0);            \
         (node)  = (node)->next)

/* 节点头追加 */
#define SLIST_PREPEND(head_, tail_, node) \
do {                                      \
    (node)->next = (head_)->head;         \
if ((head_)->head == ((void *)0))         \
    (tail_)->tail =   (node);             \
    (head_)->head =   (node);             \
} while (0)

/* 节点尾追加 */
#define SLIST_APPEND(head_, tail_, node) \
do {                                     \
if ((tail_)->tail != ((void *)0))        \
    (tail_)->tail->next = (node);        \
if ((tail_)->tail == ((void *)0))        \
    (head_)->head =   (node);            \
    (tail_)->tail =   (node);            \
} while (0)

/* 节点后插入(单链表没有节点前插入) */
#define SLIST_INSERT(head_, tail_, prev, node) \
do {                                           \
    /* 额外考虑:目标是最后一个节点时 */        \
    (node)->next = (prev)->next;               \
    (prev)->next = (node);                     \
if ((prev) == (tail_))                         \
    (tail_)->tail = (node);                    \
} while (0)

/* 节点移除(单链表移除节点需要借助前项节点) */
#define SLIST_REMOVE(head_, tail_, prev, node) \
do {                                           \
if ((prev) != ((void *)0))                     \
    (prev)->next = (node)->next;               \
    /* 链表头尾节点一并检查 */                 \
if ((head_)->head == (node))                   \
    (head_)->head  = (node)->next;             \
if ((tail_)->tail == (node))                   \
    (tail_)->tail  = (prev);                   \
} while (0)

#define SNODE_RESET(node)         \
do {                              \
    (node)->next   = ((void *)0); \
    (node)->member = ((void *)0); \
} while (0)

//这俩项并不是特别有意义
//单链表的头尾节点指针是单独管理的
#define SLIST_HEAD(head_) (head_)->head
#define SLIST_TAIL(tail_) (tail_)->tail

#define SNODE_NEXT(node) (node)->next

#define SNODE_SET(node, target) (node)->member = (target)
#define SNODE_GET(node)         (node)->member

#endif

#endif
