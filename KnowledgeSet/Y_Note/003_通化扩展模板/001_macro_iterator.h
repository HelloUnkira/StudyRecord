#ifndef MACRO_ITERATOR
#define MACRO_ITERATOR
//一个用于方便遍历的宏迭代器
//实现:implement
//容器:container
//节点:node
//所有者:owner

#define MACRO_ITERATOR_ _

#ifndef MACRO_ITERATOR_
#define MACRO_ITERATOR_ macro_iterator_
#endif

//从头迭代
#define MACRO_ITERATOR_FOR_EACH_NODE(implement, container, node)        \
    for (node  = MACRO_ITERATOR_ ## implement ## _peek_head(container); \
         node != NULL;                                                  \
         node  = MACRO_ITERATOR_ ## implement ## _peek_next(node))

//如果node不为空,从该节点迭代,否则从头开始迭代
#define MACRO_ITERATOR_ITERATE_FROM_NODE(implement, container, node)         \
    for (node  = node ?                                                      \
                 MACRO_ITERATOR_ ## implement ## _peek_next_no_check(node) : \
                 MACRO_ITERATOR_ ## implement ## _peek_head(container);      \
         node != NULL;                                                       \
         node  = MACRO_ITERATOR_ ## implement ## _peek_next(node))

//上述迭代的安全方式
#define MACRO_ITERATOR_FOR_EACH_NODE_SAFE(implement, container, node, nodes) \
    for (node  = MACRO_ITERATOR_ ## implement ## _peek_head(container),      \
         nodes = MACRO_ITERATOR_ ## implement ## _peek_next(node);           \
         node != NULL ; node = nodes,                                        \
         nodes = MACRO_ITERATOR_ ## implement ## _peek_next(node))

//获取节点与所有者的偏移,编译器可以提供更好的策略
#define MACRO_ITERATOR_CONTAINER_OFFSET(offset, container_type, \
                                        comtainer_member_name)  \
    do {                                                        \
        container_type container_name;                          \
        offset = &container_name -                              \
                 &container_name ## . ## comtainer_member_name; \
    } while (0)

//获取节点的所有者,编译器可以提供更好的策略
#define MACRO_ITERATOR_CONTAINER(owner, container_type,         \
                                 comtainer_member_name, node)   \
    do {                                                        \
        container_type container_name;                          \
        owner =  node -                                         \
               (&container_name -                               \
                &container_name ## . ## comtainer_member_name); \
    } while (0)
    
//获取头结点所有者
#define MACRO_ITERATOR_PEEK_HEAD_CONTAINER(owner,                 \
                                           implement, container,  \
                                           container_type,        \
                                           comtainer_member_name) \
    MACRO_ITERATOR_CONTAINER(owner, container_type,               \
                             comtainer_member_name,               \
        MACRO_ITERATOR_ ## implement ## _peek_head(container))
    
    
//获取尾节点所有者
#define MACRO_ITERATOR_PEEK_TAIL_CONTAINER(owner,                 \
                                           implement, container,  \
                                           container_type,        \
                                           comtainer_member_name) \
    MACRO_ITERATOR_CONTAINER(owner, container_type,               \
                             comtainer_member_name,               \
        MACRO_ITERATOR_ ## implement ## _peek_tail(container))

//获取下一节点所有者
#define MACRO_ITERATOR_PEEK_NEXT_CONTAINER(owner,                 \
                                           implement,             \
                                           container_node,        \
                                           container_type,        \
                                           comtainer_member_name) \
    MACRO_ITERATOR_CONTAINER(owner, container_type,               \
                             comtainer_member_name,               \
        MACRO_ITERATOR_ ## implement ## _peek_next(container_node))

//迭代全部节点的所有者
#define MACRO_ITERATOR_FOR_EACH_CONTAINER(owner, node,                  \
                                          implement, container,         \
                                          container_type,               \
                                          comtainer_member_name)        \
        int offset = 0;                                                 \
        MACRO_ITERATOR_CONTAINER_OFFSET(offset, container_type,         \
                                        comtainer_member_name);         \
    for (node  = MACRO_ITERATOR_ ## implement ## _peek_head(container), \
         owner = (node != NULL ? node - offset : NULL);                 \
         node != NULL;                                                  \
         node  = MACRO_ITERATOR_ ## implement ## _peek_next(node),      \
         owner = node - offset)

//安全的迭代全部节点的所有者
#define MACRO_ITERATOR_FOR_EACH_CONTAINER_SAFE(owner, node,              \
                                               owners, nodes,            \
                                               implement, container,     \
                                               container_type,           \
                                               comtainer_member_name)    \
        int offset = 0;                                                  \
        MACRO_ITERATOR_CONTAINER_OFFSET(offset, container_type,          \
                                        comtainer_member_name);          \
    for (node   = MACRO_ITERATOR_ ## implement ## _peek_head(container), \
         owner  = (node != NULL ? node - offset : NULL),                 \
         nodes  = MACRO_ITERATOR_ ## implement ## _peek_next(node),      \
         owners = (nodes != NULL ? nodes - offset : NULL);               \
         node  != NULL ; node = nodes, owner = owners,                   \
         nodes  = MACRO_ITERATOR_ ## implement ## _peek_next(node),      \
         owners = node - offset)

///声明,学习参考///

//宏迭代器为空判断
#define MACRO_ITERATOR_IS_EMPTY(implement)                                 \
    static inline bool MACRO_ITERATOR_ ## implement ## _is_empty(          \
                       MACRO_ITERATOR_ ## implement ## _t *list)           \
    {                                                                      \
        return (MACRO_ITERATOR_ ## implement ## _peek_head(list) == NULL); \
    }

//不检查获取下一个节点
#define MACRO_ITERATOR_PEEK_NEXT_NO_CHECK(implement, nodename)         \
    static inline MACRO_ITERATOR_ ## nodename ## _t *                  \
                  MACRO_ITERATOR_ ## implement ## _peek_next_no_check( \
                  MACRO_ITERATOR_ ## nodename ## _t *node)             \
    {                                                                  \
        return z_ ## nodename ## _next_peek(node);                     \
    }

//获取下一个节点
#define MACRO_ITERATOR_PEEK_NEXT(implement, nodename)                   \
    static inline MACRO_ITERATOR_ ## nodename ## _t *                   \
                  MACRO_ITERATOR_ ## implement ## _peek_next(           \
                  MACRO_ITERATOR_ ## nodename ## _t *node)              \
    {                                                                   \
        return node != NULL ?                                           \
            MACRO_ITERATOR_ ## implement ## _peek_next_no_check(node) : \
            NULL;                                                       \
    }

//结点回环
#define MACRO_ITERATOR_PREPEND(implement, nodename)                     \
    static inline void MACRO_ITERATOR_ ## implement ## _prepend(        \
                       MACRO_ITERATOR_ ## implement ## _t *list,        \
                       MACRO_ITERATOR_ ## nodename ## _t *node)         \
    {                                                                   \
        z_ ## nodename ## _next_set(node,                               \
            MACRO_ITERATOR_ ## implement ## _peek_head(list));          \
        z_ ## implement ## _head_set(list, node);                       \
        if (MACRO_ITERATOR_ ## implement ## _peek_tail(list) == NULL) { \
            z_ ## implement ## _tail_set(list,                          \
                MACRO_ITERATOR_ ## implement ## _peek_head(list));      \
        }                                                               \
    }

//追加一个节点
#define MACRO_ITERATOR_APPEND(implement, nodename)                      \
    static inline void MACRO_ITERATOR_ ## implement ## _append(         \
                       MACRO_ITERATOR_ ## implement ## _t *list,        \
                       MACRO_ITERATOR_ ## nodename ## _t *node)         \
    {                                                                   \
        z_ ## nodename ## _next_set(node, NULL);                        \
        if (MACRO_ITERATOR_ ## implement ## _peek_tail(list) == NULL) { \
            z_ ## implement ## _tail_set(list, node);                   \
            z_ ## implement ## _head_set(list, node);                   \
        } else {                                                        \
            z_ ## nodename ## _next_set(                                \
                MACRO_ITERATOR_ ## implement ## _peek_tail(list),       \
                node);                                                  \
            z_ ## implement ## _tail_set(list, node);                   \
        }                                                               \
    }

//追加一堆节点
#define MACRO_ITERATOR_APPEND_LIST(implement, nodename)               \
    static inline void  MACRO_ITERATOR_ ## implement ## _append_list( \
                        MACRO_ITERATOR_ ## implement ## _t *list,     \
                        void *head, void *tail)                       \
{                                                                     \
    if (MACRO_ITERATOR_ ## implement ## _peek_tail(list) == NULL) {   \
        z_ ## implement ## _head_set(list,                            \
            (MACRO_ITERATOR_ ## nodename ## _t *)head);               \
    } else {                                                          \
        z_ ## nodename ## _next_set(                                  \
            MACRO_ITERATOR_ ## implement ## _peek_tail(list),         \
            (MACRO_ITERATOR_ ## nodename ## _t *)head);               \
    }                                                                 \
    z_ ## implement ## _tail_set(list,                                \
                     (MACRO_ITERATOR_ ## nodename ## _t *)tail);      \
}

//追加一个链表
#define MACRO_ITERATOR_MERGE_LIST(implement, nodename)                     \
    static inline void                                                     \
        MACRO_ITERATOR_ ## implement ## _merge_ ## implement (             \
            MACRO_ITERATOR_ ## implement ## _t *list,                      \
            MACRO_ITERATOR_ ## implement ## _t *list_to_append)            \
    {                                                                      \
        MACRO_ITERATOR_ ## nodename ## _t *head, *tail;                    \
        head = MACRO_ITERATOR_ ## implement ## _peek_head(list_to_append); \
        tail = MACRO_ITERATOR_ ## implement ## _peek_tail(list_to_append); \
        MACRO_ITERATOR_ ## implement ## _append_list(list, head, tail);    \
        MACRO_ITERATOR_ ## implement ## _init(list_to_append);             \
    }

//指定位置插入一个节点
#define MACRO_ITERATOR_INSERT(implement, nodename)                \
    static inline void MACRO_ITERATOR_ ## implement ## _insert(   \
                       MACRO_ITERATOR_ ## implement ## _t *list,  \
                       MACRO_ITERATOR_ ## nodename ## _t *prev,   \
                       MACRO_ITERATOR_ ## nodename ## _t *node)   \
    {                                                             \
        if (prev == NULL) {                                       \
            MACRO_ITERATOR_ ## implement ## _prepend(list, node); \
        } else if (z_ ## nodename ## _next_peek(prev) == NULL) {  \
            MACRO_ITERATOR_ ## implement ## _append(list, node);  \
        } else {                                                  \
            z_ ## nodename ## _next_set(node,                     \
                z_ ## nodename ## _next_peek(prev));              \
            z_ ## nodename ## _next_set(prev, node);              \
        }                            \
    }

//迭代器非空获取第一个节点并移除它
#define MACRO_ITERATOR_GET_NOT_EMPTY(implement, nodename)               \
    static inline MACRO_ITERATOR_ ## nodename ## _t *                   \
        MACRO_ITERATOR_ ## implement ## _get_not_empty(                 \
        MACRO_ITERATOR_ ## implement ## _t *list)                       \
    {                                                                   \
        MACRO_ITERATOR_ ## nodename ## _t *node =                       \
            MACRO_ITERATOR_ ## implement ## _peek_head(list);           \
        z_ ## implement ## _head_set(list,                              \
            z_ ## nodename ## _next_peek(node));                        \
        if (MACRO_ITERATOR_ ## implement ## _peek_tail(list) == node) { \
            z_ ## implement ## _tail_set(list,                          \
                MACRO_ITERATOR_ ## implement ## _peek_head(list));      \
        }                                                               \
        return node;                                                    \
    }

//获取链表的一个节点,如果非空不获取
#define MACRO_ITERATOR_GET(implement, nodename)                         \
    static inline MACRO_ITERATOR_ ## nodename ## _t *                   \
                  MACRO_ITERATOR_ ## implement ## _get(                 \
                  MACRO_ITERATOR_ ## implement ## _t *list)             \
    {                                                                   \
        return MACRO_ITERATOR_ ## implement ## _is_empty(list) ? NULL : \
            MACRO_ITERATOR_ ## implement ## _get_not_empty(list);       \
    }

//移除指定的一个节点
#define MACRO_ITERATOR_REMOVE(implement, nodename)                          \
    static inline void MACRO_ITERATOR_ ## implement ## _remove(             \
                       MACRO_ITERATOR_ ## implement ## _t *list,            \
                       MACRO_ITERATOR_ ## nodename ## _t *prev_node,        \
                       MACRO_ITERATOR_ ## nodename ## _t *node)             \
    {                                                                       \
        if (prev_node == NULL) {                                            \
            z_ ## implement ## _head_set(list,                              \
                z_ ## nodename ## _next_peek(node));                        \
            if (MACRO_ITERATOR_ ## implement ## _peek_tail(list) == node) { \
                z_ ## implement ## _tail_set(list,                          \
                    MACRO_ITERATOR_ ## implement ## _peek_head(list));      \
            }                                                               \
        } else {                                                            \
            z_ ## nodename ## _next_set(prev_node,                          \
                z_ ## nodename ## _next_peek(node));                        \
            if (MACRO_ITERATOR_ ## implement ## _peek_tail(list) == node) { \
                z_ ## implement ## _tail_set(list, prev_node);              \
            }                                                               \
        }                                                                   \
        z_ ## nodename ## _next_set(node, NULL);                            \
    }

//寻找指定的节点被移除
#define MACRO_ITERATOR_FIND_AND_REMOVE(implement, nodename)                \
    static inline bool MACRO_ITERATOR_ ## implement ## _find_and_remove(   \
                       MACRO_ITERATOR_ ## implement ## _t *list,           \
                       MACRO_ITERATOR_ ## nodename ## _t *node)            \
    {                                                                      \
        MACRO_ITERATOR_ ## nodename ## _t *prev = NULL;                    \
        MACRO_ITERATOR_ ## nodename ## _t *test;                           \
        MACRO_ITERATOR_FOR_EACH_NODE(implement, list, test) {              \
            if (test == node) {                                            \
                MACRO_ITERATOR_ ## implement ## _remove(list, prev, node); \
                return true;                                               \
            }                                                              \
            prev = test;                                                   \
        }                                                                  \
        return false;                                                      \
    }

#endif
