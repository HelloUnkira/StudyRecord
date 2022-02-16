#ifndef JS_ITERATOR_H
#define JS_ITERATOR_H

/*实现目标:
 *单链表宏迭代器
 *注意这只是一个简单的摘要
 *它并不是很完备的实现
 */

//所有满足以下模板的链表均可使用宏迭代器
typedef struct single_list_macro_iterator {
    /* ... */
    struct single_list_macro_iterator *next;
    /* ... */
} slist_mi;

typedef slist_mi   slist;   /* 链表实体 */
typedef slist_mi * slistp;  /* 链表引用 */
typedef slist_mi   snode;   /* 节点实体 */
typedef slist_mi * snodep;  /* 节点引用 */

/**
 * 查找并返回在链表中找到的节点
 * @param type          结构体类型(链表节点类型)
 * @param list          链表头节点指针
 * @param cmp_element   结构体成员名
 * @param cmp_to        目标值
 * @return              匹配列表项,或NULL
 */
#define JS_LIST_FIND(type, list, cmp_element, cmp_to) \
    ({                                                 \
        type *found = NULL;                            \
        type *cur = list;                              \
        while (cur) {                                  \
            if (cur->cmp_element == cmp_to) {          \
                found = cur;                           \
                break;                                 \
            }                                          \
            cur = cur->next;                           \
        }                                              \
        found;                                         \
    })

/**
 * 在每个列表项上运行比较函数,直到找到与目标值匹配的项
 * @param type       结构体类型(链表节点类型)
 * @param list       链表头节点指针
 * @param cmp_func   返回0比较成功,函数原型为
                     int (type *item, (它的类型) cmp_to)
 * @param cmp_to     比较值
 * @return           匹配列表项,或NULL
 */
#define JS_LIST_FIND_CMP(type, list, cmp_func, cmp_to) \
    ({                                                  \
        type *found = NULL;                             \
        type *cur = list;                               \
        while (cur) {                                   \
            if (!cmp_func(cur, cmp_to)) {               \
                found = cur;                            \
                break;                                  \
            }                                           \
            cur = cur->next;                            \
        }                                               \
        found;                                          \
    })

// 将一个新节点追加到列表的末尾
// 调用者应该将该项的下一个指针设置为NULL(做了空初始化动作)
// list_item_t *list =...;  //这是一个指定list_item_t类型的链表头
// list_item_t *item =...;  //这是一个指定list_item_t类型的节点
// JS_LIST_APPEND(list_item_t, list, item); //追加插入
#define JS_LIST_APPEND(type, list, p) \
    {                                  \
        type **pnext = &list;          \
        while (*pnext) {               \
            pnext = &(*pnext)->next;   \
        }                              \
        *pnext = p;                    \
    }

// 在列表的开头添加一个新节点
// list_item_t *list =...;  //这是一个指定list_item_t类型的链表头
// list_item_t *item =...;  //这是一个指定list_item_t类型的节点
// JS_LIST_PREPEND(list_item_t, list, item); //前向追加插入
#define JS_LIST_PREPEND(type, list, p) \
    {                                   \
        p->next = list;                 \
        list = p;                       \
    }

// 从列表中移除一个节点.如果节点被移除,返回1
// list_item_t *list =...;  //这是一个指定list_item_t类型的链表头
// list_item_t *item =...;  //这是一个指定list_item_t类型的链表中节点
// JS_LIST_REMOVE(list_item_t, list, item);
#define JS_LIST_REMOVE(type, list, p)   \
    ({                                   \
        u8_t removed = 0;                \
        type *cur = list;                \
        if (p == list) {                 \
            list = p->next;              \
            removed = 1;                 \
        } else {                         \
            while (cur->next) {          \
                if (cur->next == p) {    \
                    cur->next = p->next; \
                    removed = 1;         \
                    break;               \
                }                        \
                cur = cur->next;         \
            }                            \
        }                                \
        removed;                         \
    })

// 释放并迭代一个链表,为每个链表项调用一个回调,并在每次迭代中删除当前项
// 回调函数需要完成删除节点的实际动作
// JS_LIST_FREE(list_item_t, list, callback);
#define JS_LIST_FREE(type, list, callback) \
    {                                       \
        while (list) {                      \
            type *tmp = list->next;         \
            callback(list);                 \
            list = tmp;                     \
        }                                   \
    }

// 获取链表的长度
// JS_LIST_LENGTH(list_item_t, list);
#define ZJS_LIST_LENGTH(type, list) \
    ({                              \
        int ret = 0;                \
        type *i = list;             \
        while (i) {                 \
            ret++;                  \
            i = i->next;            \
        }                           \
        ret;                        \
    })

#endif
