#define  SPECIAL_BSTARTREE_C
#include "b_star_tree.h"

#define SPECIAL_BSTARTREE_ERROR
#ifdef  SPECIAL_BSTARTREE_ERROR

#ifndef BSTARTREE_MALLOC(size)
#error  "B * tree needs to redirect the dynamic allocation function"
#endif

#ifndef BSTARTREE_FREE(pointer)
#error  "B * tree needs to redirect the dynamic release function"
#endif

#define SPECIAL_BSTARTREE_MALLOC(size) \
                BSTARTREE_MALLOC(size)

#define SPECIAL_BSTARTREE_FREE(pointer) \
                BSTARTREE_FREE(pointer)

#ifndef STATIC_INLINE
#define STATIC_INLINE static
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef ERROR_PRINT
#define ERROR_PRINT(target, str) 
#endif

#endif

#define SPECIAL_BSTARTREE_CORE
#ifdef  SPECIAL_BSTARTREE_CORE
///////////////////////////////////////////////////////////////////////////////
//B*树核心(B*树容器)///////////////////////////////////////////////////////////
//将实现提交隐藏在本地(向上层不透明)///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct b_star_tree_container {
    struct b_star_tree_container *this_parent;   //父亲
    struct b_star_tree_container *left_brother;  //左兄弟
    struct b_star_tree_container *right_brother; //右兄弟
    unsigned int current_number;    //当前数据集数据量
    unsigned int *key_set;          //关键字集
    void **child_or_data_set;       //数据元集或节点集
};

struct b_star_tree_set {
    struct b_star_tree_container *bstar_tree_root;
    unsigned int array_length;  //节点和叶子数组限制
    unsigned int criterion;     //节点分裂值
    unsigned int (*get_data_key)(void *data);
    unsigned int node_number;
};

//计算叶子节点实体的关键字
typedef unsigned int (*bstar_get_data_key)(void *data);
typedef struct b_star_tree_container bstar_node;    //B*树节点
typedef struct b_star_tree_set       bstar_tree;    //B*树集合
enum {COMMON_NODE = 0, LEAVES_NODE = 1, ERROR = 2} bstar_state;
///////////////////////////////////////////////////////////////////////////////
//内部保护,外部不可访问,备注://////////////////////////////////////////////////
//空间优化:指针地址对齐时低位是未使用的,可用于记录信息/////////////////////////
//寻找一个实现: 整形类型 <==> void*类型////////////////////////////////////////
//的强制转化(并保证不会被截断)即可优化/////////////////////////////////////////
//此处是采用数据量元素的最高位去确定是否为叶子节点/////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#endif

#define SPECIAL_BSTARTREE_BASIC_FUNCTION
#ifdef  SPECIAL_BSTARTREE_BASIC_FUNCTION

#define SPECIAL_BSTARTREE_BASIC_FUNCTION_NODE_SET
#ifdef  SPECIAL_BSTARTREE_BASIC_FUNCTION_NODE_SET

STATIC_INLINE void bstar_node_set_parent(bstar_node *node, bstar_node *parent)
{
    node->this_parent = parent;
}

STATIC_INLINE void bstar_node_set_left_brother(bstar_node *node,
                                               bstar_node *brother)
{
    node->left_brother = brother;
}

STATIC_INLINE void bstar_node_set_right_brother(bstar_node *node,
                                                bstar_node *brother)
{
    node->right_brother = brother;
}

STATIC_INLINE void bstar_node_set_key_set(bstar_node *node,
                                          unsigned int *key_set)
{
    node->key_set = key_set;
}

STATIC_INLINE void bstar_node_set_child_or_data_set(bstar_node *node,
                                                    void **child_or_data_set)
{
    node->child_or_data_set = child_or_data_set;
}

STATIC_INLINE void bstar_node_set_current_number(bstar_node *node,
                                                 unsigned int value)
{
    node->current_number &= (1 << 31);
    node->current_number |= value;
}

STATIC_INLINE void bstar_node_set_state(bstar_node *node,
                                        unsigned int state)
{
    if (state == COMMON_NODE)
        node->current_number  |= (0 << 31);
    if (state == LEAVES_NODE)
        node->current_number  |= (1 << 31);
}

#endif

#define SPECIAL_BSTARTREE_BASIC_FUNCTION_NODE_GET
#ifdef  SPECIAL_BSTARTREE_BASIC_FUNCTION_NODE_GET

STATIC_INLINE void * bstar_node_get_parent(bstar_node *node)
{
    return node->this_parent;
}

STATIC_INLINE void * bstar_node_get_left_brother(bstar_node *node)
{
    return node->left_brother;
}

STATIC_INLINE void * bstar_node_get_right_brother(bstar_node *node)
{
    return node->right_brother;
}

STATIC_INLINE unsigned int * bstar_node_get_key_set(bstar_node *node)
{
    return node->key_set;
}

STATIC_INLINE void ** bstar_node_get_child_or_data_set(bstar_node *node)
{
    return node->child_or_data_set;
}

STATIC_INLINE unsigned int bstar_node_get_current_number(bstar_node *node)
{
    return (node->current_number) & (~(1 << 31));
}

STATIC_INLINE unsigned int bstar_node_get_state(bstar_node *node)
{
    if (((node->current_number) & (1 << 31)) == (1 << 31))
        return LEAVES_NODE;
    if (((node->current_number) & (1 << 31)) != (1 << 31))
        return COMMON_NODE;
    return ERROR;
}

#endif

#define SPECIAL_BSTARTREE_BASIC_FUNCTION_TREE
#ifdef  SPECIAL_BSTARTREE_BASIC_FUNCTION_TREE

STATIC_INLINE void bstar_tree_set_root(bstar_tree *tree, bstar_node *root)
{
    tree->bstar_tree_root = root;
}

STATIC_INLINE void bstar_tree_set_length(bstar_tree *tree, unsigned int length)
{
    tree->array_length = length;
}

STATIC_INLINE void bstar_tree_set_key(bstar_tree *tree, bstar_get_data_key key)
{
    tree->get_data_key = key;
}

STATIC_INLINE void bstar_tree_set_criterion(bstar_tree *tree,
                                            unsigned int criterion)
{
    tree->criterion = criterion;
}

STATIC_INLINE void BSTAR_TREE_SET_CURRENT_NUMBER(bstar_tree *tree,
                                             unsigned int number)
{
    tree->node_number = number;
}

STATIC_INLINE bstar_node * bstar_tree_get_root(bstar_tree *tree)
{
    return tree->bstar_tree_root;
}

STATIC_INLINE unsigned int bstar_tree_get_length(bstar_tree *tree)
{
    return tree->array_length;
}

STATIC_INLINE unsigned int bstar_tree_use_key(bstar_tree *tree, void *data)
{
    return tree->get_data_key(data);
}

STATIC_INLINE unsigned int bstar_tree_get_criterion(bstar_tree *tree)
{
    return tree->criterion;
}

STATIC_INLINE unsigned int BSTAR_TREE_GET_CURRENT_NUMBER(bstar_tree *tree)
{
    return tree->node_number;
}

#endif

#endif

#define SPECIAL_BSTARTREE_USE_MACRO
#ifdef  SPECIAL_BSTARTREE_USE_MACRO

///////////////////////////////////////////////////////////////////////////////
//配置定制宏:节点//////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define BSTAR_NODE_SET_PARENT(node, parent) \
        bstar_node_set_parent(node, parent)

#define BSTAR_NODE_GET_PARENT(node) \
        bstar_node_get_parent(node)

#define BSTAR_NODE_SET_LEFT_BROTHER(node, brother) \
        bstar_node_set_left_brother(node, brother)

#define BSTAR_NODE_GET_LEFT_BROTHER(node) \
        bstar_node_get_left_brother(node)

#define BSTAR_NODE_SET_RIGHT_BROTHER(node, brother) \
        bstar_node_set_right_brother(node, brother)

#define BSTAR_NODE_GET_RIGHT_BROTHER(node) \
        bstar_node_get_right_brother(node)

#define BSTAR_NODE_SET_KEY_SET(node, value) \
        bstar_node_set_key_set(node, value)

#define BSTAR_NODE_GET_KEY_SET(node) \
        bstar_node_get_key_set(node)
        
#define BSTAR_NODE_SET_CHILD_OR_DATA_SET(node, child_or_data_set) \
        bstar_node_set_child_or_data_set(node, child_or_data_set)

#define BSTAR_NODE_GET_CHILD_OR_DATA_SET(node) \
        bstar_node_get_child_or_data_set(node)

#define BSTAR_NODE_SET_CURRENT_NUMBER(node, value) \
        bstar_node_set_current_number(node, value)

#define BSTAR_NODE_GET_CURRENT_NUMBER(node) \
        bstar_node_get_current_number(node)

#define BSTAR_NODE_SET_STATE(node, state) \
        bstar_node_set_state(node, state)

#define BSTAR_NODE_GET_STATE(node) \
        bstar_node_get_state(node)
        
///////////////////////////////////////////////////////////////////////////////
//配置定制宏:树////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define BSTAR_TREE_SET_ROOT(tree, root) \
        bstar_tree_set_root(tree, root)

#define BSTAR_TREE_GET_ROOT(tree) \
        bstar_tree_get_root(tree)

#define BSTAR_TREE_SET_LENGTH(tree, length) \
        bstar_tree_set_length(tree, length)

#define BSTAR_TREE_GET_LENGTH(tree) \
        bstar_tree_get_length(tree)

#define BSTAR_TREE_SET_KEY(tree, key) \
        bstar_tree_set_key(tree, key)

#define BSTAR_TREE_USE_KEY(tree, data) \
        bstar_tree_use_key(tree, data)
        
#define BSTAR_TREE_SET_CRITERION(node, criterion) \
        bstar_tree_set_criterion(node, criterion)

#define BSTAR_TREE_GET_CRITERION(node) \
        bstar_tree_get_criterion(node)

#define BSTAR_TREE_SET_CURRENT_NUMBER(tree, number) \
        BSTAR_TREE_SET_CURRENT_NUMBER(tree, number)

#define BSTAR_TREE_GET_CURRENT_NUMBER(tree) \
        BSTAR_TREE_GET_CURRENT_NUMBER(tree) \
        
///////////////////////////////////////////////////////////////////////////////
//配置定制宏:辅助,美观/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define RETURN_EMPTY(target) if (target) return;
#define RETURN_ZERO(target)  if (target) return 0;
#define RETURN_NULL(target)  if (target) return NULL;
#define FAKE_ACTION() (1 == 0)

#endif

#define SPECIAL_BSTARTREE_CORE_CREATE_DELETE_NODE
#ifdef  SPECIAL_BSTARTREE_CORE_CREATE_DELETE_NODE

static bstar_node * create_node(bstar_tree *tree, unsigned int state)
{
    ERROR_PRINT(tree == NULL, "create_node: tree");
    ERROR_PRINT(BSTAR_TREE_GET_LENGTH(tree) < 3, "create_node: tree zone");
    RETURN_NULL(tree == NULL || BSTAR_TREE_GET_LENGTH(tree) < 3);
    //分配空间计算
    unsigned int i = 0;
    unsigned int zone1 = sizeof(bstar_node);
    unsigned int zone2 = sizeof(unsigned int);
    unsigned int zone3 = sizeof(void *);
    zone2 *= BSTAR_TREE_GET_LENGTH(tree);//它是一个数组
    zone3 *= BSTAR_TREE_GET_LENGTH(tree);//它是一个数组
    //1.创建节点,创建关键字集,创建数据元集或节点集
    bstar_node *node      = SPECIAL_BSTARTREE_MALLOC(zone1);
    unsigned int *key_set = SPECIAL_BSTARTREE_MALLOC(zone2);
    void *data_child      = SPECIAL_BSTARTREE_MALLOC(zone3);
    //2.检查分配空间
    if (node == NULL || key_set == NULL || data_child == NULL) {
        SPECIAL_BSTARTREE_FREE(data_child);
        SPECIAL_BSTARTREE_FREE(key_set);
        SPECIAL_BSTARTREE_FREE(node);
        //利用外界的容错检查简化此处
        ERROR_PRINT(NULL == NULL, "create_node: malloc node");
        RETURN_NULL(NULL == NULL);
    }
    //3.初始化该节点,直接清零
    for (i = 0; i < zone1; i++)
        ((unsigned char *)node)[i] = 0;
    for (i = 0; i < zone2; i++)
        ((unsigned char *)key_set)[i] = 0;
    for (i = 0; i < zone3; i++)
        ((unsigned char *)data_child)[i] = 0;
    //4.关键字集绑定,数据元集或节点集绑定
    BSTAR_NODE_SET_KEY_SET(node, key_set);
    BSTAR_NODE_SET_STATE(node, state);
    BSTAR_NODE_SET_CHILD_OR_DATA_SET(node, data_child);
    return node;
}

static void delete_node(void *node)
{
    SPECIAL_BSTARTREE_FREE(BSTAR_NODE_GET_CHILD_OR_DATA_SET(node));
    SPECIAL_BSTARTREE_FREE(BSTAR_NODE_GET_KEY_SET(node));
    SPECIAL_BSTARTREE_FREE(node);
}

#endif

#define SPECIAL_BSTARTREE_CORE_AUXILIARY_FUNCTION
#ifdef  SPECIAL_BSTARTREE_CORE_AUXILIARY_FUNCTION
///////////////////////////////////////////////////////////////////////////////
//辅助功能1:二分搜索///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void bstar_tree_binary_search(unsigned int key_value,
                                     unsigned int *key_set,
                                     unsigned int boundary,
                                     int *left,
                                     int *right)
{
    ERROR_PRINT(boundary >= 2, "binary_search: boundary");
    ERROR_PRINT(key_set == NULL, "binary_search: key_set");
    ERROR_PRINT(*left > *right, "binary_search: left>right");
    RETURN_EMPTY(key_set == NULL || *left > *right || boundary >= 2);
    unsigned int center = 0;
    while (*left <= *right) {
        center = *left + (*right - *left) / 2;
        if (key_value > key_set[center])
            *left  = center + 1;
        if (key_value < key_set[center])
            *right = center - 1;
        //根据参数确定,左右边界 boundary ==>0:  <xxx<=  1:  <=xxx<
        if (key_value == key_set[center]) {
            if (boundary == 0)
                *right = center - 1;
            if (boundary == 1)
                *left  = center + 1;
        }
    }
    //此时交换left与right的值(为人性化使用其数据)
    *left  = *left ^ *right;
    *right = *left ^ *right;
    *left  = *left ^ *right;
    //不可检查左边界溢出(场景要交付给上层使用)
    //不可检查右边界溢出(场景要交付给上层使用)
}

///////////////////////////////////////////////////////////////////////////////
//辅助函数2:迭代更新从指定节点到根节点的索引///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void bstar_tree_update_key_value(bstar_node *node,
                                        unsigned int new_key_value,
                                        unsigned int old_key_value)
{
    RETURN_EMPTY(node == NULL || new_key_value == old_key_value);
    //迭代向上更新对应数据元,因为它是去往最近最小的项
    node = BSTAR_NODE_GET_PARENT(node);
    while (node != NULL && new_key_value != old_key_value) {
        //遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
        unsigned int *key_set        = BSTAR_NODE_GET_KEY_SET(node);
        unsigned int  current_number = BSTAR_NODE_GET_CURRENT_NUMBER(node);
        int left  = 0;
        int right = current_number - 1;
        int index = -1;
        //查找更新father中对应数据
        if (new_key_value > old_key_value) {
            bstar_tree_binary_search(old_key_value, key_set, 1, &left, &right);
            index = (left < 0) ? 0 : left;

        }
        if (new_key_value <  old_key_value) {
            bstar_tree_binary_search(old_key_value, key_set, 0, &left, &right);
            index = (right > current_number - 1) ? current_number - 1 : right;
        }
        
        //注意:必须要保证更新正确的父亲的关键字索引
        ERROR_PRINT(key_set[index] != old_key_value, "update_key_value");
        RETURN_EMPTY(key_set[index] != old_key_value);
        old_key_value = key_set[index];
        key_set[index] = new_key_value;
        //迭代到下一层,如果可以的话
        node = BSTAR_NODE_GET_PARENT(node);
        RETURN_EMPTY(index != 0);
    }
}

///////////////////////////////////////////////////////////////////////////////
//辅助函数3:从指定节点迁移批量数据到另一节点///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void bstar_tree_migrate_data(bstar_tree *tree,
                                    bstar_node *node1,
                                    bstar_node *node2,
                                    unsigned int migrate_number,
                                    unsigned int migrate_state)
{
    RETURN_EMPTY(tree == NULL || node1 == NULL || node2 == NULL);
    //指定将其进行数据迁移:data1 ==> data2
    //migrate_state == 0,将data1后migrate_number数据加入到data2的前面
    //migrate_state == 1,将data1前migrate_number数据加入到data2的后面
    //注意:跨节点迁移时其对父亲的索引要一并迁移
    int i = 0, temp = 0;//迁移所用到的索引
    void **data_set1 = BSTAR_NODE_GET_CHILD_OR_DATA_SET(node1);
    void **data_set2 = BSTAR_NODE_GET_CHILD_OR_DATA_SET(node2);
    unsigned int  length          = BSTAR_TREE_GET_LENGTH(tree);
    unsigned int *key_set1        = BSTAR_NODE_GET_KEY_SET(node1);
    unsigned int *key_set2        = BSTAR_NODE_GET_KEY_SET(node2);
    unsigned int  node1_state     = BSTAR_NODE_GET_STATE(node1);
    unsigned int  node2_state     = BSTAR_NODE_GET_STATE(node2);
    unsigned int  current_number1 = BSTAR_NODE_GET_CURRENT_NUMBER(node1);
    unsigned int  current_number2 = BSTAR_NODE_GET_CURRENT_NUMBER(node2);
    //要迁移migrate项数据(极限检测)
    ERROR_PRINT(migrate_state >= 2,"migrate_data: migrate_state");
    ERROR_PRINT(node1_state != node2_state, "migrate_data: node_state");
    ERROR_PRINT(current_number1 - migrate_number < 0, "migrate_data: 1");
    ERROR_PRINT(current_number2 + migrate_number > length, "migrate_data: 2");
    RETURN_EMPTY((migrate_state >= 2) ||
                 (node1_state != node2_state) ||
                 (current_number1 - migrate_number < 0) ||
                 (current_number2 + migrate_number > length));
    //根据实际情况继续不同的迁移动作
    if (migrate_state == 0) {
        //1.先漂移data2的数据
        if (current_number2 != 0)
            for (i = current_number2 - 1; i >= 0; i--) {
                key_set2[i + migrate_number] = key_set2[i];
                data_set2[i + migrate_number] = data_set2[i];
            }
        //2.data1的数据迁移到data2
        for (i = 0; i < migrate_number; i++) {
            key_set2[i]  = key_set1[i + current_number1 - migrate_number];
            data_set2[i] = data_set1[i + current_number1 - migrate_number];
            temp = i;
            if (node2_state != LEAVES_NODE && data_set2[temp] != NULL)
                BSTAR_NODE_SET_PARENT((bstar_node*)(data_set2[temp]), node2);
        }
    }
    if (migrate_state == 1) {
        //1.data1的数据迁移到data2
        for (i = 0; i < migrate_number; i++) {
            key_set2[i + current_number2]  = key_set1[i];
            data_set2[i + current_number2] = data_set1[i];
            temp = i + current_number2;
            if (node1_state != LEAVES_NODE && data_set2[temp] != NULL)
                BSTAR_NODE_SET_PARENT((bstar_node*)(data_set2[temp]), node2);
        }
        //2.后漂移data1的数据
        if (current_number1 - migrate_number != 0)
            for (i = 0; i < current_number1 - migrate_number; i++) {
                key_set1[i] = key_set1[i + migrate_number];
                data_set1[i] = data_set1[i + migrate_number];
            }
    }
    //更新data1和data2的计数量
    BSTAR_NODE_SET_CURRENT_NUMBER(node1, current_number1 - migrate_number);
    BSTAR_NODE_SET_CURRENT_NUMBER(node2, current_number2 + migrate_number);
}

///////////////////////////////////////////////////////////////////////////////
//辅助函数4:迭代搜索到叶节点,根据特定关键字////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void bstar_tree_search_node(bstar_tree *tree,
                                   bstar_node **node,
                                   unsigned int key_value)
{
    RETURN_EMPTY(tree == NULL || node == NULL);
    bstar_node *root = BSTAR_TREE_GET_ROOT(tree);
    //从根节点一直走到叶子节点即可
    while (root != NULL && BSTAR_NODE_GET_STATE(root) == COMMON_NODE) {
        //根节点至少是有元素,此时需要向下搜索
        void **child_set = BSTAR_NODE_GET_CHILD_OR_DATA_SET(root);
        unsigned int *key_set        = BSTAR_NODE_GET_KEY_SET(root);
        unsigned int  current_number = BSTAR_NODE_GET_CURRENT_NUMBER(root);
        //遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
        int left  = 0;
        int right = current_number - 1;
        //二分搜索,查找时有效项是left
        bstar_tree_binary_search(key_value, key_set, 1, &left, &right);
        left = (left < 0) ? 0 : left;
        //判断有效元,因为查找时可能出现溢出的情况
        root = (bstar_node *)(child_set[left]);
    }
    *node = root;
}

///////////////////////////////////////////////////////////////////////////////
//辅助函数5:在一个指定节点中插入一个项/////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void bstar_tree_insert_data(bstar_node *node,
                                   unsigned int state,
                                   unsigned int *index,
                                   unsigned int key_value,
                                   void *item)
{
    RETURN_EMPTY(node == NULL || index == NULL || item == NULL || state >= 2);
    void **next_set = BSTAR_NODE_GET_CHILD_OR_DATA_SET(node);
    unsigned int *key_set        = BSTAR_NODE_GET_KEY_SET(node);
    unsigned int  current_number = BSTAR_NODE_GET_CURRENT_NUMBER(node);
    //1.遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
    int left = 0, right = (int)current_number - 1, i = 0;
    bstar_tree_binary_search(key_value, key_set, state, &left, &right);
    *index = (right < 0) ? 0 : right;//插入时,有效项是right
    //2.需要将后续的部分迁移到下一项,为新入项腾出一个空间
    if (*index < current_number) {
        for (i = current_number - 1; i >= (int)(*index); i--) {
            //当前项覆盖后一项,直到==index覆盖完毕
            key_set[i + 1]  = key_set[i];
            next_set[i + 1] = next_set[i];
        }
    }
    //3.将数据加入到指定位置即可
    next_set[*index] = item;
    key_set[*index]  = key_value;
    BSTAR_NODE_SET_CURRENT_NUMBER(node, current_number + 1);
}

///////////////////////////////////////////////////////////////////////////////
//辅助函数6:从一个指定节点中移除一个项/////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void bstar_tree_remove_data(bstar_node *node,
                                   unsigned int state,
                                   unsigned int *index,
                                   unsigned int key_value,
                                   void **item)
{
    RETURN_EMPTY(node == NULL || index == NULL || item == NULL || state >= 2);
    void **next_set = BSTAR_NODE_GET_CHILD_OR_DATA_SET(node);
    unsigned int *key_set        = BSTAR_NODE_GET_KEY_SET(node);
    unsigned int  current_number = BSTAR_NODE_GET_CURRENT_NUMBER(node);
    //如果是无效的删除的话,直接回退即可
    *item == NULL;
    RETURN_EMPTY(current_number == 0);
    int left = 0, right = (int)current_number - 1, i = 0;
    //1.遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
    bstar_tree_binary_search(key_value, key_set, state, &left, &right);
    *index = ((state == 1) ? ((left < 0) ? 0 : left) :
             ((state == 0) ? (left + 1) : 0));//删除时,有效项是left
    ERROR_PRINT(key_set[*index] != key_value, "key_value");
    RETURN_EMPTY(key_set[*index] != key_value);
    //2.记录其数据
    *item = next_set[*index];
    //3.需要将后续的部分迁移到前一项,为其覆盖一个空间
    if (current_number != 1 && *index < current_number)
        for (i = *index; i <= (int)(current_number); i++) {
            //当前项覆盖后一项,直到==index覆盖完毕
            key_set[i] = key_set[i + 1];
            next_set[i] = next_set[i + 1];
        }
    BSTAR_NODE_SET_CURRENT_NUMBER(node, current_number - 1);
}

///////////////////////////////////////////////////////////////////////////////
//辅助函数7:从一个指定节点中搜索一个项/////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void bstar_tree_search_data(bstar_node *node,
                                   unsigned int state,
                                   unsigned int *index,
                                   unsigned int key_value,
                                   void **item)
{
    RETURN_EMPTY(node == NULL || index == NULL || item == NULL || state >= 2);
    void **next_set = BSTAR_NODE_GET_CHILD_OR_DATA_SET(node);
    unsigned int *key_set        = BSTAR_NODE_GET_KEY_SET(node);
    unsigned int  current_number = BSTAR_NODE_GET_CURRENT_NUMBER(node);
    //保证搜索时,内部至少有一个项
    *item = NULL;
    RETURN_EMPTY(current_number == 0);
    //遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
    int left = 0, right = current_number - 1;
    bstar_tree_binary_search(key_value, key_set, state, &left, &right);
    *index = ((state == 1) ? ((left < 0) ? 0 : left) :
             ((state == 0) ? (left + 1) : 0));//搜索时,有效项是left
    ERROR_PRINT(key_set[*index] != key_value, "key_value1");
    RETURN_EMPTY(key_set[*index] != key_value);
    //记录其数据
    *item = next_set[*index];
}

#endif

#define SPECIAL_BSTARTREE_CORE_INSERT
#ifdef  SPECIAL_BSTARTREE_CORE_INSERT
///////////////////////////////////////////////////////////////////////////////
//单次插入函数/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void only_insert_node(bstar_tree *tree, bstar_node **node, void *data)
{
    ERROR_PRINT(tree == NULL || node == NULL || data == NULL, "only_insert");
    RETURN_EMPTY(tree == NULL || node == NULL || data == NULL);
    //1.计算关键字
    unsigned int key_value = BSTAR_TREE_USE_KEY(tree, data);
    //2.如果是插入到空树中时或者插入到叶子节点时
    *node = BSTAR_TREE_GET_ROOT(tree);
    //3.确认是否是初次插入,不存在项
    if (*node == NULL) {
        //为其生成一个空间,设置其为叶子节点
        BSTAR_TREE_SET_ROOT(tree, create_node(tree, LEAVES_NODE));
        *node =  BSTAR_TREE_GET_ROOT(tree);
        RETURN_EMPTY(*node == NULL);
    }
    //4.迭代搜索到叶子节点
    bstar_tree_search_node(tree, node, key_value);
    //5.数据元加入到叶子节点
    int index = -1;
    bstar_tree_insert_data(*node, 1, &index, key_value, data);
    //6.插入的节点是叶子节点的首节点,修复它所产生的影响
    if (index == 0) {
        unsigned int *key_set = BSTAR_NODE_GET_KEY_SET(*node);
        bstar_tree_update_key_value(*node, key_set[0], key_set[1]);
    }
}

///////////////////////////////////////////////////////////////////////////////
//插入调整函数/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void adjust_insert_node(bstar_tree *tree, bstar_node *node)
{
    RETURN_EMPTY(tree == NULL || node == NULL);
    unsigned int index = 0;
    unsigned int state = 0;
    unsigned int *key_set1 = NULL, key_value1 = 0;
    unsigned int *key_set2 = NULL, key_value2 = 0;
    unsigned int *key_set3 = NULL, key_value3 = 0;
    unsigned int length         = BSTAR_TREE_GET_LENGTH(tree);
    unsigned int criterion      = BSTAR_TREE_GET_CRITERION(tree);
    unsigned int current_number = BSTAR_NODE_GET_CURRENT_NUMBER(node);
    bstar_node *parent        = NULL;
    bstar_node *left_brother  = NULL;
    bstar_node *right_brother = NULL;
    //插入时导致的节点超出
    //如果左右兄弟只要有一个存在盈余空间就无需分裂
    //如果左右兄弟都负重不堪,那么往左边分裂和往右边分裂(这是一个问题)
    //这里直接按正常习惯往右边分裂,但是到底需要有向左边分裂的必要性,是否
    //是叶节点分裂还是非叶子节点分裂(情况是一致的)
    while (current_number >= criterion) {
        //先获取它与周围节点的关系
        parent        = BSTAR_NODE_GET_PARENT(node);
        left_brother  = BSTAR_NODE_GET_LEFT_BROTHER(node);
        right_brother = BSTAR_NODE_GET_RIGHT_BROTHER(node);
        state         = BSTAR_NODE_GET_STATE(node);
        //情况1:如果右兄弟有盈余空间
        if (right_brother != NULL &&
            BSTAR_NODE_GET_CURRENT_NUMBER(right_brother) < criterion) {
            //1.获取右兄弟在父亲那的记录
            key_set3   = BSTAR_NODE_GET_KEY_SET(right_brother);
            key_value3 = key_set3[0];
            //2.将一部分数据迁移至右兄弟
            bstar_tree_migrate_data(tree, node, right_brother, 
                                    length - criterion, 0);
            //3.迭代更新从指定节点到根节点的索引
            node = right_brother;
            bstar_tree_update_key_value(node, key_set3[0], key_value3);
            return;
        }
        
        //情况2:如果左兄弟有盈余空间(它应该只做少部分数据承担者)
        //添加该种情况的话可以更大化的利用空间
        if (left_brother != NULL &&
            BSTAR_NODE_GET_CURRENT_NUMBER(left_brother) < criterion) {
            //1.获取自己在父亲那的记录
            key_set1   = BSTAR_NODE_GET_KEY_SET(node);
            key_value1 = key_set1[0];
            //2.将一部分数据迁移至左兄弟
            bstar_tree_migrate_data(tree, node, left_brother, 
                                    length - criterion, 1);
            //3.迭代更新从指定节点到根节点的索引
            bstar_tree_update_key_value(node, key_set1[0], key_value1);
            return;
        }
        
        //情况3:如果没有兄弟节点时
        if (right_brother == NULL) {
            //1.创建兄弟节点
            right_brother = create_node(tree, state);
            ERROR_PRINT(right_brother == NULL, "adjust_insert: right_brother");
            RETURN_EMPTY(right_brother == NULL);
            //2.为自己设置兄弟,为兄弟设置兄弟
            BSTAR_NODE_SET_RIGHT_BROTHER(node, right_brother);
            BSTAR_NODE_SET_LEFT_BROTHER(right_brother, node);
            //3.将本节点部分数据拷贝到右兄弟中去
            bstar_tree_migrate_data(tree, node, right_brother,
                                    length - criterion, 0);
            //4.如果没有父亲要生成父亲,如果有父亲要添加兄弟节点
            if (parent == NULL) {
                //1.创建一个父亲
                parent = create_node(tree, COMMON_NODE);
                ERROR_PRINT(parent == NULL, "adjust_insert: parent");
                RETURN_EMPTY(parent == NULL);
                //2.为自己设置新父亲
                BSTAR_NODE_SET_PARENT(node, parent);
                //3.新生成父亲时需要更新根节点
                BSTAR_TREE_SET_ROOT(tree, parent);
                //4.将自己加入到新父亲中
                key_set1 = BSTAR_NODE_GET_KEY_SET(node);
                bstar_tree_insert_data(parent, 1, &index, key_set1[0],
                                      (void *)node);
            }
            //5.为兄弟设置父亲
            BSTAR_NODE_SET_PARENT(right_brother, parent);
            //6.将兄弟插入到父亲中
            key_set3 = BSTAR_NODE_GET_KEY_SET(right_brother);
            bstar_tree_insert_data(parent, 1, &index, key_set3[0],
                                  (void *)right_brother);
            //7.迭代到上层调整
            current_number = BSTAR_NODE_GET_CURRENT_NUMBER(parent);
            node = parent;
            continue;
        }
        
        //情况4:自己和左右兄弟都是负重不堪
        //1.创建一个新节点
        bstar_node *temp = create_node(tree, state);
        //2.为其设置它的父亲
        BSTAR_NODE_SET_PARENT(temp, parent);
        //3.先迁移自己的数据
        bstar_tree_migrate_data(tree, node, temp, length - criterion, 0);
        //4.获取右兄弟在父亲那的记录
        key_set3 = BSTAR_NODE_GET_KEY_SET(right_brother);
        key_value3 = key_set3[0];
        //5.将右兄弟一部分数据迁移
        bstar_tree_migrate_data(tree, right_brother, temp, 
                                length - criterion, 1);
        //6.迭代更新从指定节点到根节点的索引
        bstar_tree_update_key_value(right_brother, key_set3[0], key_value3);
        //7.更新兄弟关系
        BSTAR_NODE_SET_LEFT_BROTHER(right_brother, temp);
        BSTAR_NODE_SET_LEFT_BROTHER(temp, node);
        BSTAR_NODE_SET_RIGHT_BROTHER(node, temp);
        BSTAR_NODE_SET_RIGHT_BROTHER(temp, right_brother);
        //9.新兄弟加入到父亲中
        key_set1 = BSTAR_NODE_GET_KEY_SET(node);
        key_set2 = BSTAR_NODE_GET_KEY_SET(temp);
        //注意:默认是在其最后产生分裂
        //但新兄弟在父亲的位置应该只能出现在当前节点之后
        state = ((key_set1[0] < key_set2[0]) ? 0 :
                ((key_set1[0] >= key_set2[0]) ? 1 : 0));
        bstar_tree_insert_data(parent, state, &index, key_set2[0],
                              (void *)temp);
        //10.迭代到上层调整
        current_number = BSTAR_NODE_GET_CURRENT_NUMBER(parent);
        node = parent;
        continue;
    }
}

#endif

#define SPECIAL_BSTARTREE_CORE_REMOVE
#ifdef  SPECIAL_BSTARTREE_CORE_REMOVE
///////////////////////////////////////////////////////////////////////////////
//单次删除函数/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void only_remove_node(bstar_tree *tree,
                             bstar_node **node,
                             unsigned int key_value,
                             void **data)
{
    ERROR_PRINT(tree == NULL || data == NULL || node == NULL, "only_remove");
    RETURN_EMPTY(tree == NULL || data == NULL || node == NULL);
    *node = BSTAR_TREE_GET_ROOT(tree);
    //确认是否是初次删除,不存在项
    RETURN_EMPTY(*node == NULL);
    int index;
    //从根节点开始向下迭代
    bstar_tree_search_node(tree, node, key_value);
    //数据元从叶子节点中移除
    bstar_tree_remove_data(*node, 1, &index, key_value, data);
    //调zheng一下删除该数所造成的影响
    unsigned int *key_set = BSTAR_NODE_GET_KEY_SET(*node);
    if (index == 0)
        bstar_tree_update_key_value(*node, key_set[0], key_value);
}

///////////////////////////////////////////////////////////////////////////////
//删除调整函数/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void adjust_remove_node(bstar_tree *tree, bstar_node *node)
{
    RETURN_EMPTY(tree == NULL || node == NULL);
    //先获取它与周围节点的关系
    bstar_node *parent        = BSTAR_NODE_GET_PARENT(node);
    bstar_node *left_brother  = BSTAR_NODE_GET_LEFT_BROTHER(node);
    bstar_node *right_brother = BSTAR_NODE_GET_RIGHT_BROTHER(node);
    //情况1:节点被清空(正常情况下,节点被清空之前会做合并的动作)
    unsigned int current_number = BSTAR_NODE_GET_CURRENT_NUMBER(node);
    unsigned int state = BSTAR_NODE_GET_STATE(node);
    if (current_number == 0 && state == LEAVES_NODE) {
        //添加必要的检查
        ERROR_PRINT(parent        != NULL, "adjust_remove: parent");
        ERROR_PRINT(left_brother  != NULL, "adjust_remove: left_brother");
        ERROR_PRINT(right_brother != NULL, "adjust_remove: right_brother");
        //它仅会出现在一种情况,就是清空树时,直接删除掉该节点就好
        BSTAR_TREE_SET_ROOT(tree, NULL);
        delete_node(node);
        return;
    }
    
    unsigned int length    = BSTAR_TREE_GET_LENGTH(tree);
    unsigned int criterion = BSTAR_TREE_GET_CRITERION(tree);
    unsigned int index;
    void *data;
    //情况2:删除导致的节点缺失,可能需要合并
    //合并仅向右方向合并,向左兄弟合并无法很好的迭代更新
    //而数据的提供优先向左兄弟索要,它更容易被溢出
    //是叶节点合并还是非叶子节点合并(情况是一致的)
    while (node != NULL && current_number < criterion) {
        ERROR_PRINT(current_number == 0, "adjust_remove: current_number");
        RETURN_EMPTY(current_number == 0);
        unsigned int *key_set1 = NULL, key_value1 = 0;
        unsigned int *key_set2 = NULL, key_value2 = 0;
        unsigned int *key_set3 = NULL, key_value3 = 0;
        //获取需要调整的节点的兄弟节点和父节点,以及孩子数量
        parent         = BSTAR_NODE_GET_PARENT(node);
        left_brother   = BSTAR_NODE_GET_LEFT_BROTHER(node);
        right_brother  = BSTAR_NODE_GET_RIGHT_BROTHER(node);
        
        //情况2.1:没有左右兄弟(孤立的节点)
        if (left_brother == NULL && right_brother == NULL) {
            //合并时产生的孤立节点,如果它的数据量只有一个
            if (current_number == 1 && state == COMMON_NODE) {
                void **data_set = BSTAR_NODE_GET_CHILD_OR_DATA_SET(node);
                //删除本体,让其孩子上位
                BSTAR_TREE_SET_ROOT(tree, (bstar_node *)(data_set[0]));
                delete_node(node);
                //新的根节点被设置为空
                BSTAR_NODE_SET_PARENT(BSTAR_TREE_GET_ROOT(tree), NULL);
            }
            return;
        }
        
        //情况2.2:左兄弟数据充足
        if (left_brother != NULL && current_number +
            BSTAR_NODE_GET_CURRENT_NUMBER(left_brother) >= length) {
            //1.向左兄弟索要一个数据并插入到自己中
            bstar_tree_migrate_data(tree, left_brother, node, 1, 0);
            //2.迭代更新从指定节点到根节点的索引
            key_set1 = BSTAR_NODE_GET_KEY_SET(node);
            bstar_tree_update_key_value(node, key_set1[0], key_set1[1]);
            return;
        }
        
        //情况2.3:右兄弟数据充足
        if (right_brother != NULL && current_number +
            BSTAR_NODE_GET_CURRENT_NUMBER(right_brother) >= length) {
            //迁移前获取节点信息
            key_set3 = BSTAR_NODE_GET_KEY_SET(right_brother);
            key_value3 = key_set3[0];
            //1.向右兄弟索要一个数据并插入到自己中
            bstar_tree_migrate_data(tree, right_brother, node, 1, 1);
            //2.迭代更新从指定节点到根节点的索引
            node = right_brother;
            bstar_tree_update_key_value(node, key_set3[0], key_value3);
            return;
        }
        
        //情况2.4:自己和右兄弟都贫困
        if (right_brother != NULL && current_number +
            BSTAR_NODE_GET_CURRENT_NUMBER(right_brother) < length) {
            //迁移前获取节点信息
            key_set1 = BSTAR_NODE_GET_KEY_SET(node);
            key_set3 = BSTAR_NODE_GET_KEY_SET(right_brother);
            key_value1 = key_set1[0];
            key_value3 = key_set3[0];
            //多多少少有些尴尬,因为对于多key的处理仅关系首项与末项
            //这里在向上迭代调整时会,使得node可能成为第二项
            //这很显然是设计上存在的缺失,不能将俩种情况合并
            if (key_value1 != key_value3) {
                //1.将右兄弟的数据迁移到自己
                current_number = BSTAR_NODE_GET_CURRENT_NUMBER(right_brother);
                bstar_tree_migrate_data(tree, right_brother, node,
                                        current_number, 1);
                //2.删除右兄弟
                parent        = BSTAR_NODE_GET_PARENT(right_brother);
                right_brother = BSTAR_NODE_GET_RIGHT_BROTHER(right_brother);
                delete_node(BSTAR_NODE_GET_RIGHT_BROTHER(node));
                //3.更新兄弟关系
                BSTAR_NODE_SET_RIGHT_BROTHER(node, right_brother);
                if (right_brother != NULL)
                    BSTAR_NODE_SET_LEFT_BROTHER(right_brother, node);
            }
            if (key_value1 == key_value3) {
                //1.将自己的数据迁移到右兄弟
                bstar_tree_migrate_data(tree, node, right_brother,
                                        current_number, 0);
                //2.删除自己
                delete_node(node);
                //3.更新兄弟关系
                BSTAR_NODE_SET_LEFT_BROTHER(right_brother, left_brother);
                if (left_brother != NULL)
                    BSTAR_NODE_SET_RIGHT_BROTHER(left_brother, right_brother);
            }
            //4.删除在父亲中的记录
            bstar_tree_remove_data(parent, 0, &index, key_value3, &data);
            //5.清除父亲节点因删除照成的影响
            if (index == 0) {
                key_set3 = BSTAR_NODE_GET_KEY_SET(parent);
                bstar_tree_update_key_value(parent, key_set3[0], key_value3);
            }
            //6.迭代到上层调整
            current_number = BSTAR_NODE_GET_CURRENT_NUMBER(parent);
            state          = BSTAR_NODE_GET_STATE(parent);
            node           = parent;
            continue;
        }
    
        //情况2.5:自己和左兄弟都贫困
        if (left_brother != NULL && current_number +
            BSTAR_NODE_GET_CURRENT_NUMBER(left_brother) < length) {
            //迁移前获取节点信息
            key_set1 = BSTAR_NODE_GET_KEY_SET(node);
            key_value1 = key_set1[0];
            //1.将自己的数据迁移到左兄弟
            bstar_tree_migrate_data(tree, node, left_brother,
                                    current_number, 1);
            //2.删除自己
            delete_node(node);
            //3.更新兄弟关系
            BSTAR_NODE_SET_RIGHT_BROTHER(left_brother, right_brother);
            if (right_brother != NULL)
                BSTAR_NODE_SET_LEFT_BROTHER(right_brother, left_brother);
            //4.删除自己在父亲中的记录
            bstar_tree_remove_data(parent, 1, &index, key_value1, &data);
            //5.清除父亲节点因删除照成的影响
            if (index == 0) {
                key_set1 = BSTAR_NODE_GET_KEY_SET(parent);
                bstar_tree_update_key_value(parent, key_set1[0], key_value1);
            }
            //6.迭代到上层调整
            current_number = BSTAR_NODE_GET_CURRENT_NUMBER(parent);
            state          = BSTAR_NODE_GET_STATE(parent);
            node           = parent;
            continue;
        }
        
        //也许会存在导致其节点亏空的情况,意味着它没有兄弟为其解决苦难
        //但我们依然相信,我们希望的美好的事情会发生
        break;
    }
}

#endif

#define SPECIAL_BSTARTREE_CORE_SEARCH
#ifdef  SPECIAL_BSTARTREE_CORE_SEARCH
///////////////////////////////////////////////////////////////////////////////
//单次查找函数/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void only_search_node(bstar_tree *tree,
                             unsigned int key_value,
                             void **data)
{
    ERROR_PRINT(tree == NULL || data == NULL, "only_search");
    RETURN_EMPTY(tree == NULL || data == NULL);
    bstar_node *search_node = BSTAR_TREE_GET_ROOT(tree);
    //确认是否是初次删除,不存在项
    *data = NULL;
    RETURN_EMPTY(search_node == NULL);
    int index;
    //从根节点开始向下迭代
    bstar_tree_search_node(tree, &search_node, key_value);
    RETURN_EMPTY(search_node == NULL);
    //找到了目标节点,确定目标节点是否存在目标
    bstar_tree_search_data(search_node, 0, &index, key_value, data);
}

#endif

#define SPECIAL_BSTARTREE_FUNCTION
#ifdef  SPECIAL_BSTARTREE_FUNCTION

void insert_node(void *tree, void *data)
{
    bstar_node *node;
    only_insert_node((bstar_tree *)tree, &node, data);
    adjust_insert_node((bstar_tree *)tree, node);
    unsigned int cuttent_number;
    cuttent_number = BSTAR_TREE_GET_CURRENT_NUMBER((bstar_tree *)tree);
    BSTAR_TREE_SET_CURRENT_NUMBER((bstar_tree *)tree, cuttent_number + 1);
}

void remove_node(void *tree, void **data, unsigned int key_value)
{
    bstar_node *node;
    only_remove_node((bstar_tree *)tree, &node, key_value, data);
    adjust_remove_node((bstar_tree *)tree, node);
    RETURN_EMPTY(*data != NULL);
    unsigned int cuttent_number;
    cuttent_number = BSTAR_TREE_GET_CURRENT_NUMBER((bstar_tree *)tree);
    BSTAR_TREE_SET_CURRENT_NUMBER((bstar_tree *)tree, cuttent_number - 1);
}

void search_node(void *tree, void **data, unsigned int key_value)
{
    only_search_node((bstar_tree *)tree, key_value, data);
}

#endif

#define SPECIAL_BSTARTREE_OTHER
#ifdef  SPECIAL_BSTARTREE_OTHER
///////////////////////////////////////////////////////////////////////////////
//额外的辅助函数,为外界提供数据的建立空间//////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
unsigned int get_bstar_set_bytes(void)
{
    return sizeof(bstar_tree);
}

unsigned int get_bstar_container_bytes(void)
{
    return sizeof(bstar_node);
}

void set_bstar_tree(void *tree, unsigned int length, data_key key)
{
    //一旦创建便会固定,所以需要容错处理
    RETURN_EMPTY(tree == NULL || key == NULL || length < 3);
    ERROR_PRINT(length == 3, "set_bstar_tree");
    ERROR_PRINT(length / 3 * 2 + 1 >= length, "set_bstar_tree");
    BSTAR_TREE_SET_LENGTH((bstar_tree *)tree, length);
    BSTAR_TREE_SET_CRITERION((bstar_tree *)tree, length / 3 * 2 + 1);
    BSTAR_TREE_SET_KEY((bstar_tree *)tree, key);
    BSTAR_TREE_SET_ROOT((bstar_tree *)tree, NULL);
}

#endif

#define SPECIAL_BSTARTREE_CHECK
#ifdef  SPECIAL_BSTARTREE_CHECK
///////////////////////////////////////////////////////////////////////////////
//层序遍历检查/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
unsigned int sequence_traversal_check(void *tree, k_print print, char destory)
{
    ERROR_PRINT(tree == NULL, "internal_check: tree");
    RETURN_ZERO(tree == NULL);
    ERROR_PRINT(print == NULL, "internal_check: print");
    RETURN_ZERO(print == NULL);
    //层序遍历(这里使用循环队列):
    //创建一个队列(这里使用投机取巧的方法,队列极限情况下存在
    //数量最大时顶多应为满B*树的全部叶子节点数大小+1
    unsigned int length = BSTAR_TREE_GET_CURRENT_NUMBER((bstar_tree *)tree);
    unsigned int zone   = sizeof(bstar_node *) * length;
    bstar_node **queue  = SPECIAL_BSTARTREE_MALLOC(zone);
    RETURN_EMPTY(queue == NULL);
    //获取插入的起始位置
    int queue_head = 0;
    int queue_tail = 0;
    int element_number = 0;
    int all_number = 0;
    
    queue[queue_tail++] = BSTAR_TREE_GET_ROOT((bstar_tree *)tree);
    queue_tail %= length;
    element_number++;
    //这是一个循环队列
    do {
        //节点出队列
        bstar_node *node = queue[queue_head++];
        queue_head %= length;
        element_number--;
        RETURN_EMPTY(node == NULL);
        void **data_set = BSTAR_NODE_GET_CHILD_OR_DATA_SET(node);
        unsigned int *key_set       = BSTAR_NODE_GET_KEY_SET(node);
        unsigned int current_number = BSTAR_NODE_GET_CURRENT_NUMBER(node);
        //节点打印
        if (BSTAR_NODE_GET_STATE(node) == COMMON_NODE)
            print(current_number, key_set, NULL);
        if (BSTAR_NODE_GET_STATE(node) == LEAVES_NODE)
            print(current_number, key_set, data_set);
        
        //子节点入队列
        if (BSTAR_NODE_GET_STATE(node) == COMMON_NODE) {
            int i = 0;
            for (i = 0; i < current_number; i++) {
                queue[queue_tail++] = (bstar_node *)(data_set[i]);
                queue_tail %= length;
                element_number++;
            }
        }
        
        //计算元节点个数
        if (BSTAR_NODE_GET_STATE(node) == LEAVES_NODE)
            all_number += current_number;
        
        if (destory != 0)
            delete_node(node);

        //队列太小,警告
        ERROR_PRINT(element_number >= (int)length, "internal_check: overflow");
        RETURN_EMPTY(element_number >= (int)length);
    } while (element_number > 0);

    return all_number;
}

#endif
