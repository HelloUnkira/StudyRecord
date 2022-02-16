#ifndef GENERAL_LIST
#define GENERAL_LIST

typedef struct macro_iterator {
    union {
        struct macro_iterator *head;//双向链表头
        struct macro_iterator *next;//双向链表下一节点
    };
    union {
        struct macro_iterator *tail;//双向链表尾
        struct macro_iterator *prev;//双向链表上一节点
    };
//在包含该头文件之前
//如果声明该宏,链表将退化为普通形式
//如果取消该宏,链表将进化为内嵌形式
#ifdef NEED_NODE_MEMBER
    void * member;
#endif
} mi_t;

typedef mi_t list;//对于list它的成员是head和tail
typedef mi_t node;//对于node它的成员是next和prev

//模板宏迭代器
#ifdef NEED_NODE_MEMBER
//普通版本的宏迭代器
#include MACRO_ITERATOR_TRAVERSE_LIST(list_name, list, node, member) \
    for (node   = list_name ## _peek_head(list),                     \
         member = list_name ## _peek_member(node);                   \
         node  != NULL;                                              \
         node   = list_name ## _peek_next(node),                     \
         member = list_name ## _peek_member(node))
//
#else
//内嵌版本的宏迭代器,此时需要解决一个逆索引问题
//该问题可利用编译器细节实现(已知一个成员地址求它的所有者),也可以利用上层解决
#include MACRO_ITERATOR_TRAVERSE_LIST(list_name, list, node, container,           \
                                      container_type, comtainer_member_name)      \
    container_type container_name;                                                \
    int offset = &container_name - &container_name ## . ## comtainer_member_name; \
    for (node      = list_name ## _peek_head(list),                               \
         container = node != NULL ? &node - offset : NULL;                        \
         node     != NULL;                                                        \
         node      = list_name ## _peek_next(node), container = &node - offset)
#endif

/*
 * 例:普通形式的宏迭代器
 
#define NEED_NODE_MEMBER
#include <001_macro_iterator.h>
//为迭代器实现以下属性

d_l_n * macro_list_peek_head(d_l_n * list)
{
	return list->head;
}

d_l_n * macro_list_peek_next(d_l_n * node)
{
	return node->next;
}  

void * macro_list_peek_member(d_l_n * node)
{
	return node->member;
}

//遍历方法
list macro_list;
node macro_node;
void* member;

MACRO_ITERATOR_TRAVERSE_LIST(macro_list, &macro_list, &macro_node, member) {
	//使用macro_member即可
    printf("%s", macro_member.member);
}
 */
 
/*
 * 例:内嵌版本的宏迭代器
#include <001_macro_iterator.h>
//为迭代器实现以下属性 
d_l_n * macro_list_peek_head(d_l_n * list)
{
	return list->head;
}

d_l_n * macro_list_peek_next(d_l_n * node)
{
	return node->next;
}

//如果存在一个所有者包含该结构,此时可如此
typedef struct _xxx {
    node current_node;
    void* other_member;
} xxx;

//遍历方法
list macro_list;
node macro_node;
xxx  macro_xxx;

MACRO_ITERATOR_TRAVERSE_LIST(macro_list, &macro_list, &macro_node, &macro_xxx
                             xxx, current_node) {
    //使用macro_xxx即可
    printf("%s", xxx.other_member);
}
 */

#endif
