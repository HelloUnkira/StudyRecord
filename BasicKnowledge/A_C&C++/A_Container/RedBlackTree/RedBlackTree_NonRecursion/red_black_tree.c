#define  SPECIAL_REDBLACKTREE_C
#include "red_black_tree.h"

#define SPECIAL_REDBLACKTREE_ERROR
#ifdef  SPECIAL_REDBLACKTREE_ERROR

///////////////////////////////////////////////////////////////////////////////
//错误检查宏(非完善)///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifndef STATIC_INLINE
#define STATIC_INLINE static
#endif

#ifndef VOID_STAR_CHANGE
#define VOID_STAR_CHANGE unsigned int
#endif

#ifdef  REDBLACKTREE_ACCIDENTAL_ERROR
#define SPECIAL_REDBLACKTREE_ACCIDENTAL_ERROR
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef ERROR_PRINT
#define ERROR_PRINT(target, str) 
#endif

#endif

#define SPECIAL_REDBLACKTREE_CORE
#ifdef  SPECIAL_REDBLACKTREE_CORE
///////////////////////////////////////////////////////////////////////////////
//红黑树核心(红黑树容器)///////////////////////////////////////////////////////
//将实现提交隐藏在本地(向上层不透明)///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
struct red_black_tree_container {
	struct red_black_tree_container *this_parent;   //节点父亲
    struct red_black_tree_container *left_child;  //节点左孩子
	struct red_black_tree_container *right_child; //节点右孩子
    void  *data_and_color;                     //要维护的上层数据集索引(含节点)
#ifdef SPECIAL_REDBLACKTREE_ACCIDENTAL_ERROR
    unsigned char color;                       //如果程序错误,释放它
#endif
};

struct red_black_tree_set {
	struct red_black_tree_container *rb_tree_root;
    unsigned char (*key_compare)(void *this_data, void *that_data);
    unsigned char (*key_comfirm)(void *that_data, void *target_data);
};

//this_data严格小于that_data返回非零值
//this_data与target_data的关键字一致返回0
typedef unsigned char (*rb_key_compare)(void *this_data, void *that_data);
typedef unsigned char (*rb_key_comfirm)(void *that_data, void *target_data);
typedef struct red_black_tree_container rb_node;     //红黑树节点
typedef struct red_black_tree_set       rb_tree;     //红黑树根
typedef enum {RED   = 0, BLACK = 1} rb_color;        //节点颜色
typedef enum {LEFT  = 0, RIGHT = 1} rb_side;         //节点关系
typedef enum {ERROR = 2}            rb_error;
///////////////////////////////////////////////////////////////////////////////
//内部保护,外部不可访问,备注://////////////////////////////////////////////////
//空间优化:指针地址对齐时低位是未使用的,可用于记录信息/////////////////////////
//寻找一个实现: 整形类型 <==> void*类型////////////////////////////////////////
//的强制转化(并保证不会被截断)即可优化/////////////////////////////////////////
//如果产生截断ERROR_PRINT会产生side错误////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#endif

#define SPECIAL_REDBLACKTREE_BASIC_FUNCTION
#ifdef  SPECIAL_REDBLACKTREE_BASIC_FUNCTION

#define SPECIAL_REDBLACKTREE_BASIC_FUNCTION_TREE
#ifdef  SPECIAL_REDBLACKTREE_BASIC_FUNCTION_TREE

STATIC_INLINE rb_node * rb_tree_get_root(rb_tree *tree)
{
    return tree->rb_tree_root;
}

STATIC_INLINE void rb_tree_set_root(rb_tree *tree, rb_node *root)
{
    tree->rb_tree_root = root;
}

STATIC_INLINE void rb_tree_set_key_compare(rb_tree *tree,
                                           rb_key_compare compare)
{
    tree->key_compare = compare;
}
   
STATIC_INLINE void rb_tree_set_key_comfirm(rb_tree *tree,
                                           rb_key_comfirm comfirm)
{
    tree->key_comfirm = comfirm;
}

STATIC_INLINE unsigned char rb_tree_use_key_compare(rb_tree *tree,
                                                    void *data1,
                                                    void *data2)
{
    return tree->key_compare(data1, data2);
}

STATIC_INLINE unsigned char rb_tree_use_key_comfirm(rb_tree *tree,
                                                    void *data1,
                                                    void *data2)
{
    return tree->key_comfirm(data1, data2);
}

#endif

#define SPECIAL_REDBLACKTREE_BASIC_FUNCTION_NODE_GET
#ifdef  SPECIAL_REDBLACKTREE_BASIC_FUNCTION_NODE_GET

STATIC_INLINE unsigned char rb_node_get_color(rb_node *node)
{
#ifdef SPECIAL_REDBLACKTREE_ACCIDENTAL_ERROR
    return node->color;
#endif
    return (unsigned char)
           (((VOID_STAR_CHANGE)(node->data_and_color)) & 
            ((VOID_STAR_CHANGE)1));
}

STATIC_INLINE void * rb_node_get_data(rb_node *node)
{
#ifdef SPECIAL_REDBLACKTREE_ACCIDENTAL_ERROR
    return node->data_and_color;
#endif
    return (void *)
           (((VOID_STAR_CHANGE)(node->data_and_color)) & 
           (~(VOID_STAR_CHANGE)1));
}

STATIC_INLINE rb_node * rb_node_get_parent(rb_node *node)
{
    return node->this_parent;
}

STATIC_INLINE unsigned char rb_node_get_side(rb_node *node, rb_node *parent)
{
    if (node == parent->left_child)
        return LEFT;
    if (node == parent->right_child)
        return RIGHT;
    return ERROR;
}

STATIC_INLINE unsigned char rb_node_get_other_side(unsigned char side)
{
    if (side == LEFT)
        return RIGHT;
    if (side == RIGHT)
        return LEFT;
    return ERROR;
}

STATIC_INLINE rb_node * rb_node_get_child(rb_node *node, unsigned char side)
{
    if (side == LEFT)
        return node->left_child;
    if (side == RIGHT)
        return node->right_child;
    return NULL;
}
   
#endif

#define SPECIAL_REDBLACKTREE_BASIC_FUNCTION_NODE_SET
#ifdef  SPECIAL_REDBLACKTREE_BASIC_FUNCTION_NODE_SET

STATIC_INLINE void rb_node_set_data(rb_node *node, void *data)
{
#ifdef SPECIAL_REDBLACKTREE_ACCIDENTAL_ERROR
    node->data_and_color = data;
    return;
#endif
    node->data_and_color = (void *)
        ((((VOID_STAR_CHANGE)(node->data_and_color)) & 
         (~(VOID_STAR_CHANGE)1)) | 
          ((VOID_STAR_CHANGE)data & 
         (~(VOID_STAR_CHANGE)1)));
}

STATIC_INLINE void rb_node_set_black(rb_node *node)
{
#ifdef SPECIAL_REDBLACKTREE_ACCIDENTAL_ERROR
    node->color = BLACK;
    return;
#endif    
    node->data_and_color = (void *)
        ((((VOID_STAR_CHANGE)(node->data_and_color)) & 
         (~(VOID_STAR_CHANGE)1)) | (VOID_STAR_CHANGE)BLACK);
}
    
STATIC_INLINE void rb_node_set_red(rb_node *node)
{
#ifdef SPECIAL_REDBLACKTREE_ACCIDENTAL_ERROR
    node->color = RED;
    return;
#endif  
    node->data_and_color = (void *)
        ((((VOID_STAR_CHANGE)(node->data_and_color)) & 
         (~(VOID_STAR_CHANGE)1)) | (VOID_STAR_CHANGE)RED);
}

STATIC_INLINE void rb_node_set_color(rb_node *node, unsigned char color)
{
#ifdef SPECIAL_REDBLACKTREE_ACCIDENTAL_ERROR
    node->color = color;
    return;
#endif  
    node->data_and_color = (void *)
        ((((VOID_STAR_CHANGE)(node->data_and_color)) & 
         (~(VOID_STAR_CHANGE)1)) | 
          ((VOID_STAR_CHANGE)color & 
           (VOID_STAR_CHANGE)1));
}
    
STATIC_INLINE void rb_node_set_parent(rb_node *node, rb_node *parent)
{
    node->this_parent = parent;
}

STATIC_INLINE void rb_node_set_child(rb_node *node, 
                                     rb_node *child, 
                                     unsigned char side)
{
    if (side == LEFT)
        node->left_child  = child;
    if (side == RIGHT)
        node->right_child = child;
}

#endif

#endif

#define SPECIAL_REDBLACKTREE_USE_MACRO
#ifdef  SPECIAL_REDBLACKTREE_USE_MACRO

///////////////////////////////////////////////////////////////////////////////
//配置定制宏:树////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define RB_TREE_GET_ROOT(tree) \
        rb_tree_get_root(tree)

#define RB_TREE_SET_ROOT(tree, root) \
        rb_tree_set_root(tree, root)

#define RB_TREE_SET_KEY_COMPARE(tree, compare) \
        rb_tree_set_key_compare(tree, compare)
    
#define RB_TREE_SET_KEY_COMFIRM(tree, comfirm) \
        rb_tree_set_key_comfirm(tree, comfirm)

#define RB_TREE_USE_KEY_COMPARE(tree, data1, data2) \
        rb_tree_use_key_compare(tree, data1, data2)

#define RB_TREE_USE_KEY_COMFIRM(tree, data1, data2) \
        rb_tree_use_key_comfirm(tree, data1, data2)

///////////////////////////////////////////////////////////////////////////////
//配置定制宏:节点//////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define RB_NODE_GET_COLOR(node) \
        rb_node_get_color(node)

#define RB_NODE_GET_DATA(node) \
        rb_node_get_data(node)

#define RB_NODE_GET_PARENT(node) \
        rb_node_get_parent(node)

#define RB_NODE_GET_SIDE(node, parent) \
        rb_node_get_side(node, parent)
        
#define RB_NODE_GET_OTHER_SIDE(side) \
        rb_node_get_other_side(side)

#define RB_NODE_GET_CHILD(node, side) \
        rb_node_get_child(node, side)

#define RB_NODE_SET_DATA(node, data) \
        rb_node_set_data(node, data)

#define RB_NODE_SET_BLACK(node) \
        rb_node_set_black(node)
    
#define RB_NODE_SET_RED(node) \
        rb_node_set_red(node)
    
#define RB_NODE_SET_COLOR(node, color) \
        rb_node_set_color(node, color)
    
#define RB_NODE_SET_PARENT(node, parent) \
        rb_node_set_parent(node, parent)

#define RB_NODE_SET_CHILD(node, child, side) \
        rb_node_set_child(node, child, side)
        
///////////////////////////////////////////////////////////////////////////////
//配置定制宏:辅助,美观/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define RETURN_EMPTY(target) if (target) return;
#define RETURN_ZERO(target)  if (target) return 0;
#define RETURN_NULL(target)  if (target) return NULL;
#define FAKE_ACTION() (1 == 0)

#endif

#define SPECIAL_REDBLACKTREE_CORE_ROTATE
#ifdef  SPECIAL_REDBLACKTREE_CORE_ROTATE
///////////////////////////////////////////////////////////////////////////////
//交换函数(左右旋转)///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void rotate_two_node(rb_tree *tree, rb_node *child, rb_node *parent)
{
    ERROR_PRINT(tree == NULL, "rotate: tree");
    ERROR_PRINT(child == NULL, "rotate: child");
    ERROR_PRINT(parent == NULL, "rotate: parent");
    RETURN_EMPTY(tree == NULL || child == NULL || parent == NULL);
    //1.获得父子节点的对位关系,父亲与对顶位孙子的对位关系
    unsigned char side = RB_NODE_GET_SIDE(child, parent);
    unsigned char other_side = RB_NODE_GET_OTHER_SIDE(side);
    ERROR_PRINT(side == ERROR, "rotate: side");
    RETURN_EMPTY(side == ERROR);
    //2.获得祖父节点,获得父亲对顶位的孙子节点
    rb_node *ancestor = RB_NODE_GET_PARENT(parent);
    rb_node *son = RB_NODE_GET_CHILD(child, other_side);

    //3.原父亲获得新孩子,孙子获得新父亲
    RB_NODE_SET_CHILD(parent, son, side);
    (son != NULL) ? RB_NODE_SET_PARENT(son, parent) : FAKE_ACTION();
    //4.交换父子节点位置
    RB_NODE_SET_CHILD(child, parent, other_side);
    RB_NODE_SET_PARENT(child, ancestor);
    RB_NODE_SET_PARENT(parent, child);
    //5.更新祖父节点
    if (ancestor != NULL) {
        side = RB_NODE_GET_SIDE(parent, ancestor);
        RB_NODE_SET_CHILD(ancestor, child, side);
        ERROR_PRINT(side == ERROR, "rotate: ancestor");
        RETURN_EMPTY(side == ERROR);
        return;
    }
    RB_TREE_SET_ROOT(tree, child);
}

#endif

#define SPECIAL_REDBLACKTREE_CORE_SEARCH
#ifdef  SPECIAL_REDBLACKTREE_CORE_SEARCH
///////////////////////////////////////////////////////////////////////////////
//搜索函数(大小元)/////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static rb_node * search_tree_min_or_max(rb_node *node, unsigned char side)
{
    ERROR_PRINT(side == ERROR, "min_or_max: side");
    RETURN_NULL(side == ERROR);
    
    while (node != NULL && RB_NODE_GET_CHILD(node, side) != NULL)
        node = RB_NODE_GET_CHILD(node, side);
    
    //很显然该测试没有意义
    ERROR_PRINT(RB_NODE_GET_CHILD(node, LEFT) != NULL &&
                RB_NODE_GET_CHILD(node, RIGHT) != NULL, "min_or_max: child");
    return node;
}

///////////////////////////////////////////////////////////////////////////////
//搜索函数(前驱和后继)/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
rb_node * search_node_prev_or_next(rb_node *node, unsigned char side)
{
    ERROR_PRINT(node == NULL, "prev_or_next: node");
    RETURN_NULL(node == NULL);
    ERROR_PRINT(side == ERROR, "prev_or_next: side");
    RETURN_NULL(side == ERROR);
    
    unsigned char other_side = RB_NODE_GET_OTHER_SIDE(side); 
    rb_node *child  = RB_NODE_GET_CHILD(node, side);
    rb_node *parent = RB_NODE_GET_PARENT(node);
    //先找孩子最大项
    if (child != NULL)
        return search_tree_min_or_max(child, other_side);
    //节点的孩子不存在,找父亲
    while (parent != NULL) {
        //节点的孩子不存在,父亲与对应的孩子在同一边
        if (RB_NODE_GET_SIDE(node, parent) == other_side)
            return parent;
        //父亲与对应孩子不在同一边,找递归项到第一个相遇的同边
        node = parent;
        parent = RB_NODE_GET_PARENT(parent);
    }
    //退出可能:
    //父亲一开始就不存在(根节点)
    //祖父节点不存在(迭代到根节点,端点边界值没有对应的下一项)
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//查找函数/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static rb_node * only_search_node(rb_tree *tree, void *target_data)
{
    ERROR_PRINT(tree == NULL, "only_search: tree");
    RETURN_NULL(tree == NULL);
    ERROR_PRINT(tree->key_compare == NULL, "only_search: key_compare");
    ERROR_PRINT(tree->key_comfirm == NULL, "only_search: key_comfirm");
    RETURN_NULL(tree->key_compare == NULL);
    RETURN_NULL(tree->key_comfirm == NULL);
    
    rb_node *search_index = RB_TREE_GET_ROOT(tree);
    while (search_index != NULL) {
        //关键字匹配成功,返回
        if (RB_TREE_USE_KEY_COMFIRM(tree, target_data, 
            RB_NODE_GET_DATA(search_index)) == 0)
            return search_index;
        
        //该结点小于此本结点,到左子树去
        if (RB_TREE_USE_KEY_COMPARE(tree, target_data,
            RB_NODE_GET_DATA(search_index)) != 0) {
            search_index = RB_NODE_GET_CHILD(search_index, LEFT);
            continue;
        }
        
        //该结点大于此本结点,到右子树去
        if (RB_TREE_USE_KEY_COMPARE(tree, target_data,
            RB_NODE_GET_DATA(search_index)) == 0) {
            search_index = RB_NODE_GET_CHILD(search_index, RIGHT);
            continue;
        }
    }
    return NULL;
}

#endif

#define SPECIAL_REDBLACKTREE_CORE_INSERT
#ifdef  SPECIAL_REDBLACKTREE_CORE_INSERT
///////////////////////////////////////////////////////////////////////////////
//单次插入函数/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void only_insert_node(rb_tree *tree, rb_node *node)
{
    ERROR_PRINT(tree == NULL, "only_insert: tree");
    ERROR_PRINT(node == NULL, "only_insert: node");
    ERROR_PRINT(tree->key_compare == NULL, "only_insert: key_compare");
    RETURN_EMPTY(node == NULL || tree == NULL || tree->key_compare == NULL);
    
    //1.初始化要插入的节点(插入节点都是红色的)
    RB_NODE_SET_RED(node);
    RB_NODE_SET_PARENT(node, NULL);
    RB_NODE_SET_CHILD(node, NULL, LEFT);
    RB_NODE_SET_CHILD(node, NULL, RIGHT);
    
    //2.如果是插入到根节点
    if (RB_TREE_GET_ROOT(tree) == NULL) {
        RB_TREE_SET_ROOT(tree, node);
        return;
    }

    rb_node *insert_index = RB_TREE_GET_ROOT(tree);
    rb_node *insert_left  = NULL;
    rb_node *insert_right = NULL;
    //3.从根节点开始寻找插入的起始位置并插入
    while (insert_index != NULL) {
        //左子树为空且该结点小于此本结点,表示结点可以插到其左子树
        if (RB_TREE_USE_KEY_COMPARE(tree, 
            RB_NODE_GET_DATA(node), 
            RB_NODE_GET_DATA(insert_index)) != 0) {
            //获得其左孩子
            insert_left = RB_NODE_GET_CHILD(insert_index, LEFT);
            //左孩子不为空继续迭代,左孩子为空插入
            if (insert_left != NULL) {
                insert_index = insert_left;
                continue; 
            }
            RB_NODE_SET_CHILD(insert_index, node, LEFT);
            RB_NODE_SET_PARENT(node, insert_index);
            return;
        }
        //右子树为空且该节点不小于此本节点,表示结点可以插到其右子树
        if (RB_TREE_USE_KEY_COMPARE(tree, 
            RB_NODE_GET_DATA(node),
            RB_NODE_GET_DATA(insert_index)) == 0) {
            //获得其右孩子
            insert_right = RB_NODE_GET_CHILD(insert_index, RIGHT);
            //右孩子不为空继续迭代,右孩子为空插入
            if (insert_right != NULL) {
                insert_index = insert_right;
                continue;
            }
            RB_NODE_SET_CHILD(insert_index, node, RIGHT);
            RB_NODE_SET_PARENT(node, insert_index);
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
//插入调整函数/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void adjust_insert_node(rb_tree *tree, rb_node *node)
{
    ERROR_PRINT(tree == NULL, "adjust_insert: tree");
    ERROR_PRINT(node == NULL, "adjust_insert: node");
    RETURN_EMPTY(tree == NULL || node == NULL);
    while (1) {
        rb_node *parent = RB_NODE_GET_PARENT(node);
        //情况1:插入节点是根节点,该结点染色为黑色
        if (parent == NULL) {
            RB_NODE_SET_BLACK(node);
            return;
        }
        //情况2:插入节点父节点是黑色,什么都不做
        RETURN_EMPTY(RB_NODE_GET_COLOR(parent) == BLACK);
        //情况3:插入节点的父节点是红色(无需判断,一定是红节点)
        //1.获得祖父节点(红色父节点一定有祖父节点)
        rb_node *ancestor = RB_NODE_GET_PARENT(parent);
        ERROR_PRINT(ancestor == NULL, "adjust_insert: ancestor");
        RETURN_EMPTY(ancestor == NULL);
        unsigned char side = RB_NODE_GET_SIDE(parent, ancestor);
        unsigned char other_side = RB_NODE_GET_OTHER_SIDE(side);
        ERROR_PRINT(side == ERROR, "adjust_insert: side");
        RETURN_EMPTY(side == ERROR);
        //2.获得叔叔节点
        rb_node *uncle = RB_NODE_GET_CHILD(ancestor, other_side);
        //情况3.1:同时出现双红色节点,将冲突向上调一层
        if (uncle != NULL && RB_NODE_GET_COLOR(uncle) == RED) {
            //1.将父节点和叔叔节点设为黑色
            RB_NODE_SET_BLACK(parent);
            RB_NODE_SET_BLACK(uncle);
            //2.将祖父节点设为红色
            RB_NODE_SET_RED(ancestor);
            //3.将祖父节点设为当前节点调整 
            node = ancestor;
            continue;
        }
        
        //情况3.2:不管叔叔节点是否存在或者为黑色,此时调整只能在内部进行
        unsigned char child_side = RB_NODE_GET_SIDE(node, parent);
        ERROR_PRINT(child_side == ERROR, "adjust_insert: child_side");
        RETURN_EMPTY(child_side == ERROR);
        //情况3.2.1:如果是之字型分支,先通过旋转调整到俩个对位
        if ((side == LEFT && child_side == RIGHT) ||
            (side == RIGHT && child_side == LEFT)) {
            //1.将父节点作为新的当前节点
            //2.对父节点进行旋转
            rotate_two_node(tree, node, parent);
            //更新索引为下述情况使用
            child_side = RB_NODE_GET_OTHER_SIDE(child_side);
            rb_node *help = NULL;
            help   = node;
            node   = parent;
            parent = help;
        }

        //情况3.2.2:如果是对位分支,直接内部旋转
        if ((side == LEFT  && child_side == LEFT) ||
            (side == RIGHT && child_side == RIGHT)) {
            //1.将父节点设为黑色
            RB_NODE_SET_BLACK(parent); 
            //2.将祖父节点设为红色
            RB_NODE_SET_RED(ancestor); 
            //3.对祖父节点进行旋转
            rotate_two_node(tree, parent, ancestor);
            //经过旋转调整了父节点和祖父节点的关系
            if (RB_NODE_GET_PARENT(parent) == NULL)
                RB_TREE_SET_ROOT(tree, parent);
        }
        return;
    }
}

#endif

#define SPECIAL_REDBLACKTREE_CORE_REMOVE
#ifdef  SPECIAL_REDBLACKTREE_CORE_REMOVE
///////////////////////////////////////////////////////////////////////////////
//单次删除函数/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void only_remove_node(rb_tree *tree, rb_node **node, char* side)
{
    ERROR_PRINT(tree == NULL, "only_remove: tree");
    RETURN_EMPTY(tree == NULL);
    //找不到要删除的结点，就return
	RETURN_EMPTY(*node == NULL);
    //1.获得该节点的左右孩子
    rb_node *left_child  = RB_NODE_GET_CHILD(*node, LEFT);
    rb_node *right_child = RB_NODE_GET_CHILD(*node, RIGHT);
	//情况1:该结点是叶子结点,删除该结点
    //情况2:该结点仅有一个孩子,删除该节点
    //情况3:该结点有俩个孩子,找一个(前驱或后继)节点替换它
	if (left_child != NULL && right_child != NULL) {
        //1.获取其前驱和后继(实际只用到一个)
        rb_node *prev = search_tree_min_or_max(left_child, RIGHT);
        rb_node *next = search_tree_min_or_max(right_child, LEFT);
        //2.交换该数与(前驱)后继结点中的数据,不交换颜色
        void *node_data_struct = RB_NODE_GET_DATA(*node);
        void *next_data_struct = RB_NODE_GET_DATA(next);
        RB_NODE_SET_DATA(*node, next_data_struct);
        RB_NODE_SET_DATA(next, node_data_struct);
        //3.此时被删除元素变成其后继结点
        left_child   = RB_NODE_GET_CHILD(next, LEFT);
        right_child  = RB_NODE_GET_CHILD(next, RIGHT);
        *node = next;
	}
    //2.经过转化,变成获得可能存在的一个孩子
    rb_node *target_child = (left_child == NULL ? right_child : left_child);
    //3.解除目标target在原集合的关联
    rb_node *parent = RB_NODE_GET_PARENT(*node);
    *side = RB_NODE_GET_SIDE(*node, parent);
    ERROR_PRINT(*side == ERROR, "only_remove: side");
    RETURN_EMPTY(*side == ERROR);
    //4.如果是要删除根节点
    if (parent == NULL) {
        RB_TREE_SET_ROOT(tree, target_child);
        return;
    }
    //5.父亲获得新孩子
    RB_NODE_SET_CHILD(parent, target_child, *side);
    //6.为孩子设置新父亲
    if (target_child != NULL)
        RB_NODE_SET_PARENT(target_child, parent);
}

///////////////////////////////////////////////////////////////////////////////
//删除调整函数/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void adjust_remove_node(rb_tree *tree, rb_node *node, char side)
{
    ERROR_PRINT(tree == NULL, "adjust_remove: tree");
    RETURN_EMPTY(tree == NULL);
    //情况1:删除空节点,无需调整
    RETURN_EMPTY(node == NULL);
    //1.获得删除的节点的父亲和左右孩子
    unsigned char color  = RB_NODE_GET_COLOR(node);
    rb_node *left_child  = RB_NODE_GET_CHILD(node, LEFT);
    rb_node *right_child = RB_NODE_GET_CHILD(node, RIGHT);
    rb_node *only_one    = (left_child == NULL ? right_child : left_child);
    //情况2:删除节点为红色,无需调整
    RETURN_EMPTY(color == RED);
    //情况3:自己为黑色但是唯一孩子为红色
    if (only_one != NULL && RB_NODE_GET_COLOR(only_one) == RED) {
            RB_NODE_SET_BLACK(only_one);
            return;
    }
    //情况4:自己是黑色,删除导致缺失了一个黑色
    while (1) {
        rb_node *parent = RB_NODE_GET_PARENT(node);
        unsigned char sibling_side = RB_NODE_GET_OTHER_SIDE(side);
        //情况4.1:删除的节点为根节点(或者调整到根节点),什么都不做
        RETURN_EMPTY(parent == NULL);
        rb_node *sibling = RB_NODE_GET_CHILD(parent, sibling_side);
        //注意:删除黑色节点导致的不平衡兄弟一定存在
        ERROR_PRINT(sibling == NULL, "adjust_remove: sibling");
        RETURN_EMPTY(sibling == NULL);
        //情况4.2:兄弟节点为红色
        if (RB_NODE_GET_COLOR(sibling) == RED) {
            //1.父节点染成红色,兄弟节点染成黑色
            RB_NODE_SET_BLACK(sibling);
            RB_NODE_SET_RED(parent);
            //2.对父节点进行旋转
            rotate_two_node(tree, sibling, parent);
            //更新父子兄弟关系
            sibling = RB_NODE_GET_CHILD(parent, sibling_side);
        }
        
        rb_node *sibling_left_child  = RB_NODE_GET_CHILD(sibling, LEFT);
        rb_node *sibling_right_child = RB_NODE_GET_CHILD(sibling, RIGHT);
        
        //情况4.3:孩子都是黑色
        if ((sibling_left_child  == NULL ||
            RB_NODE_GET_COLOR(sibling_left_child) == BLACK) &&
            (sibling_right_child == NULL ||
            RB_NODE_GET_COLOR(sibling_right_child) == BLACK)) {
            if (RB_NODE_GET_COLOR(parent) == BLACK) {
                //情况4.3.1:父亲是黑色
                //1.染色兄弟
                RB_NODE_SET_RED(sibling);
                //2.调整到上一层
                node = parent;
                parent = RB_NODE_GET_PARENT(node);
                RETURN_EMPTY(parent == NULL);
                side = RB_NODE_GET_SIDE(node, parent);
                ERROR_PRINT(side == ERROR, "adjust_remove: side");
                RETURN_EMPTY(side == ERROR);
                continue;
            } else {
                //情况4.3.2:父亲是红色
                //1.交换父亲兄弟的颜色
                RB_NODE_SET_BLACK(parent);
                RB_NODE_SET_RED(sibling);
                return;
            }
        }
        
        rb_node *red_child = NULL;
        //情况4.4:如果是之字型分支,先通过旋转调整到俩个对位
        //情况4.4.1:孩子节点是父左节点,兄弟左儿子是红色,右儿子是黑色
        //情况4.4.2:孩子节点是父右节点,兄弟右儿子是红色,左儿子是黑色
        if (side == LEFT &&  (sibling_right_child == NULL || 
            RB_NODE_GET_COLOR(sibling_right_child) == BLACK))
                red_child = sibling_left_child;
        
        if (side == RIGHT && (sibling_left_child == NULL || 
            RB_NODE_GET_COLOR(sibling_left_child) == BLACK))
                red_child = sibling_right_child;
        
        if (red_child != NULL) {
            //1.染红兄弟节点,染黑红色孩子
            RB_NODE_SET_BLACK(red_child);
            RB_NODE_SET_RED(sibling);
            //2.对兄弟节点进行旋转
            rotate_two_node(tree, red_child, sibling);
            //兄弟节点以及对应的左右孩子都更新了!
            sibling = red_child;
            sibling_left_child  = RB_NODE_GET_CHILD(sibling, LEFT);
            sibling_right_child = RB_NODE_GET_CHILD(sibling, RIGHT);
        }
            
        red_child = NULL;
        //情况4.5:如果是对位分支,直接内部旋转
        //情况4.5.1:孩子节点是父左节点,兄弟的右孩子为红色
        //情况4.5.2:孩子节点是父右节点,兄弟的左孩子为红色
        if (side == LEFT &&  (sibling_right_child != NULL && 
            RB_NODE_GET_COLOR(sibling_right_child) == RED))
                red_child = sibling_right_child;
        
        if (side == RIGHT && (sibling_left_child != NULL && 
            RB_NODE_GET_COLOR(sibling_left_child) == RED))
                red_child = sibling_left_child;
        
        if (red_child != NULL) {
            //1.兄弟获得父节点的颜色
            RB_NODE_SET_COLOR(sibling, RB_NODE_GET_COLOR(parent));
            //2.父节点获得兄弟节点的颜色
            RB_NODE_SET_BLACK(parent);
            //3.染黑红子节点
            RB_NODE_SET_BLACK(red_child);
            //4.对父节点进行旋转
            rotate_two_node(tree, sibling, parent);
        }
        
        return;
    }
}

#endif

#define SPECIAL_REDBLACKTREE_FUNCTION
#ifdef  SPECIAL_REDBLACKTREE_FUNCTION
///////////////////////////////////////////////////////////////////////////////
//核心动作/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void insert_node(void *tree, void *node)
{
    only_insert_node((rb_tree *)tree, (rb_node *)node);
    adjust_insert_node((rb_tree *)tree, (rb_node *)node);
}

void remove_node(void *tree, void *node)
{
    unsigned char side = ERROR;
    only_remove_node((rb_tree *)tree, (rb_node **)(&node), &side);  
    adjust_remove_node((rb_tree *)tree, (rb_node *)node, side);
}

void * search_node(void *tree, void *data)
{
    return (void *)only_search_node((rb_tree *)tree, data);
}

void * search_node_prev(void *node)
{
    return (void *)search_node_prev_or_next((rb_node *)node, LEFT);
}

void * search_node_next(void *node)
{
    return (void *)search_node_prev_or_next((rb_node *)node, RIGHT);
}

void * search_min_node(void *tree)
{
    rb_node *root = RB_TREE_GET_ROOT((rb_tree *)tree);
    return (void *)search_tree_min_or_max(root, LEFT);
}

void * search_max_node(void *tree)
{
    rb_node *root = RB_TREE_GET_ROOT((rb_tree *)tree);
    return (void *)search_tree_min_or_max(root, RIGHT);
}

///////////////////////////////////////////////////////////////////////////////
//数据的层序遍历并打印/////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void sequence_traversal(void *tree, print k_print, void *queue, int length)
{
    ERROR_PRINT(tree == NULL, "internal_check: tree");
    RETURN_EMPTY(tree == NULL);
    ERROR_PRINT(queue == NULL, "internal_check: tree");
    RETURN_EMPTY(queue == NULL);
    ERROR_PRINT(k_print == NULL, "internal_check: print");
    RETURN_EMPTY(k_print == NULL);
    //层序遍历(这里使用循环队列):
    
    //获取插入的起始位置
    int queue_head = 0;
    int queue_tail = 0;
    int element_number = 0;
    ((rb_node **)queue)[queue_tail++] = RB_TREE_GET_ROOT((rb_tree *)tree);
    element_number++;
    do {
        //节点出队列
        rb_node *node = ((rb_node **)queue)[queue_head++];
        element_number--;
        RETURN_EMPTY(node == NULL);
        //节点打印
        k_print(node, RB_NODE_GET_DATA(node), RB_NODE_GET_COLOR(node));
        //这是一个循环队列
        if (queue_head >= length)
            queue_head = 0;
        if (queue_tail >= length)
            queue_tail = 0; 
        
        //子节点入队列
        rb_node *left_child  = RB_NODE_GET_CHILD(node, LEFT);
        rb_node *right_child = RB_NODE_GET_CHILD(node, RIGHT);
 
        //左孩子入队列
        if (left_child != NULL) {
            ((rb_node **)queue)[queue_tail++] = left_child;
            element_number++;
        }
        
        //右孩子入队列
        if (right_child != NULL) {
            ((rb_node **)queue)[queue_tail++] = right_child;
            element_number++;
        }
        
        //队列太小,警告
        ERROR_PRINT(element_number >= length, "internal_check: overflow");
        RETURN_EMPTY(element_number >= length);
    } while (element_number > 0);
}

#endif

#define SPECIAL_REDBLACKTREE_OTHER
#ifdef  SPECIAL_REDBLACKTREE_OTHER
///////////////////////////////////////////////////////////////////////////////
//额外的辅助函数,为外界提供数据的建立空间//////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
unsigned int get_rb_container_bytes(void)
{
    return sizeof(rb_node);
}

unsigned int get_rb_set_bytes(void)
{
    return sizeof(rb_tree);
}

///////////////////////////////////////////////////////////////////////////////
//辅助动作/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void set_rb_tree(void *tree, compare k_compare, comfirm k_confirm)
{
    RB_TREE_SET_ROOT((rb_tree *)tree, NULL);
    RB_TREE_SET_KEY_COMPARE((rb_tree *)tree, k_compare);
    RB_TREE_SET_KEY_COMFIRM((rb_tree *)tree, k_confirm);
}

void set_rb_root(void *tree, void *node)
{
	RB_TREE_SET_ROOT((rb_tree *)tree, (rb_node *)node);
}

void get_rb_root(void *tree, void **node)
{
	*node = (void *)RB_TREE_GET_ROOT((rb_tree *)tree);
}

void set_rb_node(void *node, void *data)
{
    RB_NODE_SET_DATA((rb_node *)node, data);
}

void get_rb_data(void *node, void **data)
{
    *data = RB_NODE_GET_DATA((rb_node *)node);
}

#endif

#define SPECIAL_REDBLACKTREE_CHECK
#ifdef  SPECIAL_REDBLACKTREE_CHECK
///////////////////////////////////////////////////////////////////////////////
//验证一棵树是否为红黑树(内部检查)/////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
unsigned char check_rb_tree(void *tree, void *stack, int *flags, int length)
{
    ERROR_PRINT(tree == NULL, "internal_check: tree");
    RETURN_ZERO(tree == NULL);
    ERROR_PRINT(stack == NULL, "internal_check: tree");
    RETURN_ZERO(stack == NULL);
    ERROR_PRINT(flags == NULL, "internal_check: tree");
    RETURN_ZERO(flags == NULL);
    
    //验证:1根节点不是黑色
    if (RB_TREE_GET_ROOT((rb_tree *)tree) == NULL)
        return 1;
    if (RB_NODE_GET_COLOR(RB_TREE_GET_ROOT((rb_tree *)tree)) == RED)
        return 0;
    
    //初始化flags的值
    int i = 0;
    for (i = 0; i < length; i++) flags[i] = LEFT;
    //深度优先搜索,使用flags同步维护栈进动情况
    rb_node *left_child  = NULL;
    rb_node *right_child = NULL;
    rb_node *node = NULL;
    //从根节点开始从左进动
    //入栈节点只有当左右孩子都使用完毕后才退栈
    int max_black_node = 0;//一条从根到叶子的最大度
    int current_black_node = 0;
    int top = 0;//栈顶位置
    
    //1.根节点入栈
    ((rb_node **)stack)[top++] = RB_TREE_GET_ROOT((rb_tree *)tree);
    current_black_node++;
    flags[top - 1] = LEFT;
    
    do {
        //2.获得栈顶元素的左右孩子
        node = ((rb_node **)stack)[top - 1];
        left_child  = RB_NODE_GET_CHILD(node, LEFT);
        right_child = RB_NODE_GET_CHILD(node, RIGHT);
        
        //验证:2红色节点不相连
        if (left_child != NULL)
            if (RB_NODE_GET_COLOR(node) == RED &&
                RB_NODE_GET_COLOR(left_child) == RED)
                    return 0;
                    
        if (right_child != NULL)
            if (RB_NODE_GET_COLOR(node) == RED &&
                RB_NODE_GET_COLOR(right_child) == RED)
                    return 0;
        
        //3.左孩子未曾进过栈
        if (flags[top - 1] == LEFT) {
            //3.1.标记左孩子进过栈,下一次该右孩子进了
            flags[top - 1] = RIGHT;
            //左孩子存在,可以进栈
            if(left_child != NULL) {
                ((rb_node **)stack)[top++] = left_child;
                //如果左孩子是黑色的,计算其度
                if (RB_NODE_GET_COLOR(left_child) == BLACK)
                    current_black_node++;
                flags[top - 1] = LEFT;//左孩子的左孩子未曾进栈
                continue;
            } else {
                //当前节点不存在左孩子
                //说明一条从根到叶子的路径产生了
                if (max_black_node == 0)
                    max_black_node = current_black_node;
                //验证:路径黑色节点不一致
                if (max_black_node != current_black_node)
                    return 0;
            }
        }
        
        //4.右孩子未曾进过栈
        if (flags[top - 1] == RIGHT) {
            //3.2标记右孩子进过栈,下一次该退栈了
            flags[top - 1] = ERROR;
            //右孩子存在,可以进栈
            if(right_child != NULL) {
                ((rb_node **)stack)[top++] = right_child;
                //如果右孩子是黑色的,计算其度
                if (RB_NODE_GET_COLOR(right_child) == BLACK)
                    current_black_node++;
                flags[top - 1] = LEFT;//右孩子的左孩子未曾进栈
                continue;
            } else {
                //当前节点不存在右孩子,
                //说明一条从根到叶子的路径产生了
                if (max_black_node == 0)
                    max_black_node = current_black_node;
                //验证:路径黑色节点不一致
                if (max_black_node != current_black_node)
                    return 0;
            }
        }
        
        //5.都进栈了,当前节点该退栈了
        if (flags[top - 1] == ERROR) {
            if (RB_NODE_GET_COLOR(node) == BLACK)
                    current_black_node--; 
            top--;
        }
        
        //队列太小,警告
        ERROR_PRINT(top >= length, "internal_check: overflow");
        RETURN_EMPTY(top >= length);
    } while (top > 0);
    
    //6.退栈完毕了,验证完成
    return 1;
}

#endif

