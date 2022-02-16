#define SPECIAL_REDBLACKTREE_C
#include "red_black_tree.h"

///////////////////////////////////////////////////////////////////////////////
//红黑树核心(红黑树容器)///////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef struct red_black_tree_node {//红黑树节点
	struct red_black_tree_node *rb_right;
	struct red_black_tree_node *rb_left;
    void *data_struct;
    //空间优化,上述指针地址对齐时低俩位是未使用的,可用于记录颜色信息
    //但是平台并不一定满足对应的void *强转,修改宏使void *正确的强转
    //是有必要的(也就是说不可以被截短)
    unsigned char rb_color;
} rb_node;

typedef struct red_black_tree {//红黑树根
	rb_node *rb_root;
    //this_data严格小于that_data返回非零值
    unsigned char (*key_compare)(void *this_data, void *that_data);
    //this_data与target_data的关键字一致返回0
    unsigned char (*key_comfirm)(void *that_data, void *target_data);
    //红黑树最大深度
    unsigned int max_depth;
    //红黑树中的节点
    unsigned int node_number;
    //红黑树需要使用到的堆栈
    rb_node **iterator_stack;
    //堆栈的元素最大容量(堆栈大小)
    unsigned int stack_length;
} rb_tree;

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
//辅助结构/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
typedef enum {RED = 0, BLACK = 1} rb_color;
typedef enum {LEFT = 0, RIGHT = 1} rb_side;

///////////////////////////////////////////////////////////////////////////////
//错误检查宏///////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifndef NULL
#define NULL 0
#endif

#define ERROR 2

#ifndef ERROR_PRINT
#define ERROR_PRINT(target, str) 
#endif

#ifndef ERROR_RETURN
#define ERROR_RETURN(target)
#endif

#ifndef ERROR_RETURN_0
#define ERROR_RETURN_0(target)
#endif

#ifndef ERROR_RETURN_NULL
#define ERROR_RETURN_NULL(target)
#endif

///////////////////////////////////////////////////////////////////////////////
//配置定制宏:树////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define RB_TREE_GET_ROOT(tree) ((tree)->rb_root)

#define RB_TREE_SET_ROOT(tree, root) ((tree)->rb_root = (root))

#define RB_TREE_GET_MAX_DEPTH(tree) ((tree)->max_depth)

#define RB_TREE_SET_MAX_DEPTH(tree, depth) ((tree)->max_depth = (depth))

#define RB_TREE_GET_NODE_NUMBER(tree) ((tree)->node_number)

#define RB_TREE_SET_NODE_NUMBER(tree, number) ((tree)->node_number = (number))

#define RB_TREE_GET_STACK(tree) ((tree)->iterator_stack)

#define RB_TREE_SET_STACK(tree, stack) ((tree)->iterator_stack = stack)

#define RB_TREE_GET_STACK_LENGTH(tree) ((tree)->stack_length)

#define RB_TREE_SET_STACK_LENGTH(tree, length) ((tree)->stack_length = length)

#define RB_TREE_SET_KEY_COMPARE(tree, compare) \
    (((tree)->key_compare) = (compare))
    
#define RB_TREE_SET_KEY_COMFIRM(tree, comfirm) \
    (((tree)->key_comfirm) = (comfirm))

#define RB_TREE_USE_KEY_COMPARE(tree, data1, data2) \
    (((tree)->key_compare)((data1), (data2)))

#define RB_TREE_USE_KEY_COMFIRM(tree, data1, data2) \
    (((tree)->key_comfirm)((data1), (data2)))

///////////////////////////////////////////////////////////////////////////////
//配置定制宏:节点//////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//Get宏

#define RB_NODE_GET_COLOR(rb) ((rb)->rb_color)

#define RB_NODE_GET_DATA(rb) ((rb)->data_struct)

#define RB_NODE_PARENT_GET_SIDE(rb, parent) \
    ((rb) == ((parent)->rb_left) ? LEFT :   \
    ((rb) == ((parent)->rb_right) ? RIGHT : ERROR))

#define RB_NODE_GET_CHILD(rb, side)    \
    ((side) == LEFT ?  (rb)->rb_left : \
    ((side) == RIGHT ? (rb)->rb_right : NULL))

//Set宏

#define RB_NODE_SET_DATA(rb, data) (((rb)->data_struct) = (data))

#define RB_NODE_SET_BLACK(rb) (((rb)->rb_color) = BLACK)
    
#define RB_NODE_SET_RED(rb) (((rb)->rb_color) = RED)

#define RB_NODE_SET_COLOR(rb, color) (((rb)->rb_color) = (color))
    
#define RB_NODE_SET_CHILD(parent, child, side)         \
    ((side) == LEFT ?  ((parent)->rb_left = (child)) : \
    ((side) == RIGHT ? ((parent)->rb_right = (child)) : 1 == 0))

///////////////////////////////////////////////////////////////////////////////
//配置定制宏:其他//////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define RB_GET_OTHER_SIDE(side) (side == LEFT ? RIGHT : LEFT)

///////////////////////////////////////////////////////////////////////////////
//搜索函数(大小元)/////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static rb_node * search_tree_min_max(rb_node *node, int side)
{
    ERROR_PRINT(node == NULL, "s_n_p: node");
    ERROR_RETURN_NULL(node == NULL);
    if (node == NULL)
        return;
    rb_node* temp = RB_NODE_GET_CHILD(node, side);
    while (temp != NULL) {   
        node = temp;
        temp = RB_NODE_GET_CHILD(node, side);
    }
    return node;
}

///////////////////////////////////////////////////////////////////////////////
//交换函数(左右旋转)///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void rotate_two_node(rb_node **stack, int size)
{
    ERROR_PRINT(stack == NULL, "r_t_n: stack");
    ERROR_PRINT(size < 2, "r_t_n: size");
    ERROR_RETURN(stack == NULL || size < 2);//非空检查
    //到栈顶位置获得父子节点
    rb_node *parent = stack[size - 2];
    rb_node *child  = stack[size - 1];
    //获得父子节点的关系
    char side = RB_NODE_PARENT_GET_SIDE(child, parent);
    ERROR_PRINT(side == ERROR, "r_t_n: child:parent");
    ERROR_RETURN(side == ERROR);
    //获得子节点的俩个孩子
    rb_node *outer_child = RB_NODE_GET_CHILD(child, side);
    rb_node *inner_child  = RB_NODE_GET_CHILD(child, RB_GET_OTHER_SIDE(side));
    //存在祖父节点
    if (size >= 3) {
        //获取祖父节点索引
        rb_node *grandparent = stack[size - 3];
        //获得祖父与父亲的关系
        char parent_side = RB_NODE_PARENT_GET_SIDE(parent, grandparent);
        ERROR_PRINT(parent_side == ERROR, "r_t_n: child:parent");
        ERROR_RETURN(parent_side == ERROR);
        //现在孩子成为新的父节点
        RB_NODE_SET_CHILD(grandparent, child, parent_side);
    }
    
    //RB_NODE_SET_CHILD(child, outer_child, side);
    //更新父子关系
    RB_NODE_SET_CHILD(child, parent, RB_GET_OTHER_SIDE(side));
    //父亲获得对位孙子为新孩子
    RB_NODE_SET_CHILD(parent, inner_child, side);
    //更新堆栈俩个节点关系
    stack[size - 2] = child;
    stack[size - 1] = parent;
}

///////////////////////////////////////////////////////////////////////////////
//查找函数(节点入栈)///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void node_entry_stack(rb_tree *tree, rb_node *node,
                             rb_node **stack, int *size, int is_remove)
{
    ERROR_PRINT(tree == NULL, "n_e_s: tree");
    ERROR_RETURN(tree == NULL);
    *size = 1;
    //根节点入栈
    stack[*size - 1] = RB_TREE_GET_ROOT(tree);
    //自此以后从该节点向下依次渐入
    while (1) {
        //删除动作时额外需要去停留在此处,插入动作时无需
        //因为我们关心的是按顺序排序而不是冲突修改
        if (is_remove && 
            RB_TREE_USE_KEY_COMFIRM(tree, 
            RB_NODE_GET_DATA(node), 
            RB_NODE_GET_DATA(stack[*size - 1])) == 0)
            return;
        
        int side = ERROR;
        //如果目标小于该节点
        if (RB_TREE_USE_KEY_COMPARE(tree, 
            RB_NODE_GET_DATA(node),
            RB_NODE_GET_DATA(stack[*size - 1])))
            side = LEFT;
        //如果目标大于等于该节点(这里是对称书写,实际应该省略)
        if (!RB_TREE_USE_KEY_COMPARE(tree, 
             RB_NODE_GET_DATA(node),
             RB_NODE_GET_DATA(stack[*size - 1])))
            side = RIGHT;
        //再检查一次(实际使用时被裁空)
        ERROR_PRINT(side == ERROR, "n_e_s: side");
        ERROR_RETURN(side == ERROR);
        //获取其孩子
        rb_node *child = RB_NODE_GET_CHILD(stack[*size - 1], side);
        if (child != NULL) {
            stack[*size] = child;
            *size = *size + 1;
        } else
            break;
        //当没有找到参数相同的节点break退出时
        //此时栈顶保留了最后一次查找记录
        //它停留的位置其孩子刚好为空(适合插入)
    }
}

///////////////////////////////////////////////////////////////////////////////
//插入调整函数/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void adjust_insert_node(rb_node **stack, int size)
{
    ERROR_PRINT(stack == NULL, "a_i_n: stack");
    ERROR_RETURN(stack == NULL);
    while (size > 1) {
        rb_node *node = stack[size - 1];
        rb_node *parent = stack[size - 2];
    
        //节点的左右孩子不是红色的
        ERROR_PRINT(RB_NODE_GET_CHILD(node, LEFT) != NULL &&
                    RB_NODE_GET_CHILD(node, LEFT) == RED,
                    "a_i_n: left_child red");
        ERROR_PRINT(RB_NODE_GET_CHILD(node, RIGHT) != NULL &&
                    RB_NODE_GET_CHILD(node, RIGHT) == RED,
                    "a_i_n: right_child red");
        ERROR_RETURN(RB_NODE_GET_CHILD(node, LEFT) != NULL &&
                     RB_NODE_GET_CHILD(node, LEFT) == RED);
        ERROR_RETURN(RB_NODE_GET_CHILD(node, RIGHT) != NULL &&
                     RB_NODE_GET_CHILD(node, RIGHT) == RED);
        
        if (RB_NODE_GET_COLOR(parent) == BLACK)
            return;
        
        //红节点一定有父亲
        ERROR_PRINT(size < 2, "a_i_n: lost parent");
        ERROR_RETURN(size < 2);
        
        //获取祖父节点
        rb_node *grandparent = stack[size - 3];
        //确认父亲与祖父的对位关系
        char side = RB_NODE_PARENT_GET_SIDE(parent, grandparent);
        ERROR_PRINT(side == ERROR, "a_i_n: side");
        ERROR_RETURN(side == ERROR);
        //获得父亲的兄弟节点
        rb_node *aunt = RB_NODE_GET_CHILD(grandparent, 
                        RB_GET_OTHER_SIDE(side));
        
        //情况1:叔叔节点是红色(父亲也是红色节点,黑色节点下沉)
        if (aunt != NULL && RB_NODE_GET_COLOR(aunt) == RED) {
            //1.染红祖父节点
            RB_NODE_SET_RED(grandparent);
            //2.染黑叔叔节点与父亲节点
            RB_NODE_SET_BLACK(parent);
            RB_NODE_SET_BLACK(aunt);
            //3.红色节点冲突回退到祖父节点
            size -= 2;
            continue; 
        }
        
        //情况2:叔叔节点不存在或为黑色(本地旋转修正该树)
        char parent_side = RB_NODE_PARENT_GET_SIDE(node, parent);
        ERROR_PRINT(parent_side == ERROR, "a_i_n: parent_side");
        ERROR_RETURN(parent_side == ERROR);
        //清况2.1:本节点与祖父节点是对位关系,需要先旋转至顺位关系
        if (parent_side != side) {
            rotate_two_node(stack, size);
            node = stack[size - 1];
        }
        //清空2.2:本节点与祖父节点是顺位关系
        //1.旋转父节点与祖父节点
        rotate_two_node(stack, size - 1);
        //2.原祖父节点染成红色
        RB_NODE_SET_BLACK(stack[size - 3]);
        //3.原父节点染成黑色
        RB_NODE_SET_RED(stack[size - 2]);
        
        //如果是到这种情况,需要注意一下流程:
        //本节点(插入时的颜色为红色),父节点为红色,兄弟节点为黑色
        //旋转过后,本节点晋升为祖父节点(染成黑色),原父节点和祖父节点成为新的
        //父节点和兄弟节点(染成红色),原兄弟节点下沉,原黑色不变
        return;
    }
    
    //我们退出循环是因为已经到了根节点,但根节点一定是黑色的
    RB_NODE_SET_BLACK(stack[0]);
}

///////////////////////////////////////////////////////////////////////////////
//删除调整函数/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void adjust_remove_node(rb_node **stack, int size, rb_node *null_node)
{
    //一直调整到根节点或调整完毕
    while (size > 1) {
        rb_node *node = stack[size - 1];
        rb_node *parent = stack[size - 2];
        //孩子与父亲的关系
        char node_side = RB_NODE_PARENT_GET_SIDE(node, parent);
        //获得兄弟节点
        rb_node *sibling = RB_NODE_GET_CHILD(parent, 
                           RB_GET_OTHER_SIDE(node_side));
        
        //删除一个黑色节点导致的不平衡,兄弟节点一定存在
        ERROR_PRINT(sibling == NULL, "a_r_n: sibling");
        ERROR_RETURN(sibling == NULL);
        
        //ERROR_PRINT(RB_NODE_GET_COLOR(sibling) == RED, "a_r_n: sibling 2");
        
        //情况1:如果兄弟节点是红色的
        if (RB_NODE_GET_COLOR(sibling) == RED) {
            //要注意的一点:如果兄弟节点为红色
            //那么父节点和它的所有子节点必为黑色
            //1.兄弟节点覆盖当前节点
            stack[size - 1] = sibling;
            //2.兄弟节点与父节点旋转
            rotate_two_node(stack, size);
            //3.原父节点下沉为兄弟节点,染成红色
            RB_NODE_SET_RED(parent);
            //4.原兄弟节点晋升为父节点,染成黑色
            RB_NODE_SET_BLACK(sibling);
            //对兄弟节点旋转意味着自己下沉一级,将自己重新加入到集合中
            //现在它是原父节点(现在兄弟节点)的子节点
            //5.当前节点重新加入枝干
            stack[size++] = node;
            //6.重新定位父节点
            parent = stack[size - 2];
            //7.重新定位兄弟节点
            sibling = RB_NODE_GET_CHILD(parent, RB_GET_OTHER_SIDE(node_side));
        }
        
        //注意,旋转到下一级的时候
        //它的新兄弟一定是存在的,因为如果不存在
        //那么树在旋转之前本身就不是平衡的
        ERROR_PRINT(sibling == NULL, "a_r_n: sibling");
        ERROR_RETURN(sibling == NULL);
        //获得兄弟节点的孩子们
        rb_node *left_child  = RB_NODE_GET_CHILD(sibling, LEFT);
        rb_node *right_child = RB_NODE_GET_CHILD(sibling, RIGHT);
        
        //情况2:兄弟孩子都是黑色的
        if ((left_child == NULL  || RB_NODE_GET_COLOR(left_child) == BLACK) &&
            (right_child == NULL || RB_NODE_GET_COLOR(right_child) == BLACK)) {
            if (node == null_node) //需要删除节点,断开关联
                RB_NODE_SET_CHILD(parent, NULL, node_side);
            
            //兄弟只有黑孩子的情况有简单的解决办法
            //1.将新兄弟节点染成红色
            RB_NODE_SET_RED(sibling);
            
            if (RB_NODE_GET_COLOR(parent) == BLACK) {
                //2.父结点缺少一个黑色,向上迭代
                size--;
                continue;
            } else {
                //重新上色使整个树OK
                //(父节点是红色直接染成黑色即可完成)
                RB_NODE_SET_BLACK(parent);
                return;
            }
        }
        
        ERROR_PRINT((left_child == NULL  || 
                     RB_NODE_GET_COLOR(left_child) == BLACK) &&
                    (right_child == NULL || 
                     RB_NODE_GET_COLOR(right_child) == BLACK), 
                     "a_r_n: child");
        ERROR_RETURN((left_child == NULL || 
                     RB_NODE_GET_COLOR(left_child) == BLACK) &&
                    (right_child == NULL || 
                     RB_NODE_GET_COLOR(right_child) == BLACK));
        //我们知道兄弟姐妹至少有一个红色的孩子
        //固定它,使远/外的位置(即在N的对面)肯定是红色的
        //如果远处位置是红色的(即在N的对面)不管里面是红是黑都不重要
        rb_node *outer_child = RB_NODE_GET_CHILD(sibling, 
                               RB_GET_OTHER_SIDE(node_side));
        //情况3:兄弟远处位置不存在红色节点
        if ((outer_child == NULL || RB_NODE_GET_COLOR(outer_child) == BLACK)) {
            //1.获得内部节点
            rb_node *inner_child = RB_NODE_GET_CHILD(sibling, node_side);
            //2.兄弟节点写入到栈顶
            stack[size - 1] = sibling;
            //3.内部的红节点入栈
            stack[size++] = inner_child;
            //4.旋转内部节点与兄弟节点
            rotate_two_node(stack, size);
            //5.原兄弟节点下沉为远端节点,染成红色
            RB_NODE_SET_RED(sibling);
            //6.原内部节点晋升为兄弟节点,染成黑色
            RB_NODE_SET_BLACK(inner_child);
            //7.更新兄弟节点
            sibling = stack[size - 2];
            //8.更新远端节点
            outer_child = RB_NODE_GET_CHILD(sibling, 
                          RB_GET_OTHER_SIDE(node_side));
            //9.栈顶恢复成当前节点
            stack[size - 2] = node;
            //10.回退栈顶
            size--;
        }
        
        //最后,兄弟结点必须在远/外插槽中有一个红色的子结点
        //我们可以旋转兄弟和我们的父元素并重新着色以生成一个有效的树
        ERROR_PRINT((outer_child == NULL || 
                     RB_NODE_GET_COLOR(outer_child) == BLACK),
                     "a_r_n: outer_child");
        ERROR_RETURN((outer_child == NULL || 
                     RB_NODE_GET_COLOR(outer_child) == BLACK));
        //情况4:兄弟远处位置存在红色节点
        //1.交换父亲与兄弟的颜色
        RB_NODE_SET_COLOR(sibling, RB_NODE_GET_COLOR(parent));
        RB_NODE_SET_BLACK(parent);
        //2.远端红孩子染成黑色补色
        RB_NODE_SET_BLACK(outer_child);
        //3.栈顶更新为兄弟节点
        stack[size - 1] = sibling;
        //4.旋转兄弟节点与父节点
        rotate_two_node(stack, size);
        
        if (node == null_node) //需要删除节点,断开关联
            RB_NODE_SET_CHILD(parent, NULL, node_side);
        return;
    }
}

///////////////////////////////////////////////////////////////////////////////
//单次插入函数/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void single_insert_node(rb_tree *tree, rb_node *node)
{
    ERROR_PRINT(tree == NULL, "s_i_n: tree");
    ERROR_PRINT(node == NULL, "s_i_n: node");
    ERROR_PRINT(tree->key_compare == NULL, "s_i_n: key_compare");
    ERROR_RETURN(tree == NULL || node == NULL || tree->key_compare == NULL);

    //辅助初始化节点(可能需要)
    RB_NODE_SET_CHILD(node, NULL, LEFT);
    RB_NODE_SET_CHILD(node, NULL, RIGHT);

    //第一次插入
    if (RB_TREE_GET_ROOT(tree) == NULL) {
        RB_TREE_SET_ROOT(tree, node);
        RB_TREE_SET_MAX_DEPTH(tree, 1);
        RB_NODE_SET_BLACK(node);
        return;
    } 

    //1.获得其对应的栈
    rb_node **stack = RB_TREE_GET_STACK(tree);
    //2.获取插入的枝干
    int size = -1;
    node_entry_stack(tree, node, stack, &size, 0);
    //3.获得父节点
    rb_node *parent = stack[size - 1];
    //4.获得父子节点的关联
    char side = (RB_TREE_USE_KEY_COMPARE(tree, 
                 RB_NODE_GET_DATA(node), 
                 RB_NODE_GET_DATA(parent))) != 0 ? LEFT : RIGHT;
    //5.建立亲子关系
    RB_NODE_SET_CHILD(parent, node, side);
    //6.染红插入的节点
    RB_NODE_SET_RED(node);
    //7.当前节点入栈
    stack[size++] = node;
    //8.修复该树
    adjust_insert_node(stack, size);
    //更新树深
    if (size > RB_TREE_GET_MAX_DEPTH(tree))
        RB_TREE_SET_MAX_DEPTH(tree, size);
    //可能调整到根节点
    RB_TREE_SET_ROOT(tree, stack[0]);
    //树的根是黑色的
    ERROR_PRINT(RB_NODE_GET_COLOR(stack[0]) != BLACK, "s_i_n: root");
    ERROR_RETURN(RB_NODE_GET_COLOR(stack[0]) != BLACK);
}

///////////////////////////////////////////////////////////////////////////////
//单次删除函数/////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static void single_remove_node(rb_tree *tree, rb_node *node)
{
    ERROR_PRINT(tree == NULL, "s_d_n: tree");
    ERROR_RETURN(tree == NULL);
    
    //1.获得其对应的栈
    rb_node **stack = RB_TREE_GET_STACK(tree);
    //2.获取删除的枝干
    int size = -1;
    node_entry_stack(tree, node, stack, &size, 1);
    //3.没找到要删除节点
    if (RB_TREE_USE_KEY_COMFIRM(tree,
        RB_NODE_GET_DATA(node),
        RB_NODE_GET_DATA(stack[size - 1])) != 0)
        return;
        
    //我们只能删除一个有0或1个子节点的节点
    //如果我们有两个,那么选择边0的“最大”子节点(最小的1也可以)
    //并与它交换我们在树中的位置
    //4.获得目标节点
    if (RB_NODE_GET_CHILD(node, LEFT) != NULL && 
        RB_NODE_GET_CHILD(node, RIGHT) != NULL) {
        int size0 = size;
        //1.获得左孩子
        rb_node *child = RB_NODE_GET_CHILD(node, LEFT);
        //2.保存当前节点父亲
        rb_node *high_parent = size > 1 ? stack[size - 2] : NULL;
        //3.左孩子入栈
        stack[size++] = child;
        //4.迭代寻找其右孩子
        while (RB_NODE_GET_CHILD(child, RIGHT) != NULL) {
            child = RB_NODE_GET_CHILD(child, RIGHT);
            stack[size++] = child;
        }
        //5.获得要交换的孩子的父亲
        rb_node *low_parent = stack[size - 2];
    
        //实际上非侵入式的数据结构直接交换其子节点数据索引即可完成
        //这里是继承Zephyr的改写,它原是侵入式数据结构,所以保留
        //但有一些特殊情况需要检查
        //原则上,这是通过在节点之间交换子指针
        //并将指向它们的节点从它们的父节点重新定位来实现的,但是:
        //(1)上面的节点可能是树的根,没有父节点
        //(2)下面的节点可能是上面节点的直接子节点
        //还记得交换两个节点的颜色位
        //当然,我们没有父指针,所以跟踪这个结构的堆栈也需要交换!
        if (high_parent != NULL) {
            char side = RB_NODE_PARENT_GET_SIDE(node, high_parent);
            ERROR_PRINT(side == ERROR, "s_d_n: side");
            ERROR_RETURN(side == ERROR);
            //为高父亲设置低孩子
            RB_NODE_SET_CHILD(high_parent, child, side);
        } else
            RB_TREE_SET_ROOT(tree, child);
    
        //如果低父亲与node节点一致(node和child是亲子关系)
        if (low_parent == node) {
            //交换亲子关系(交换它们的左孩子)
            RB_NODE_SET_CHILD(node, RB_NODE_GET_CHILD(child, LEFT), LEFT);
            RB_NODE_SET_CHILD(child, node, LEFT);
        } else {//跨度亲子关系
            char low_side = RB_NODE_PARENT_GET_SIDE(child, low_parent);
            ERROR_PRINT(low_side == ERROR, "s_d_n: low_side");
            ERROR_RETURN(low_side == ERROR);
            //更新低父亲指向高孩子
            RB_NODE_SET_CHILD(low_parent, node, low_side);
            //交换俩个节点的左孩子(可能存在)
            rb_node *temp = RB_NODE_GET_CHILD(node, LEFT);
            RB_NODE_SET_CHILD(node, RB_NODE_GET_CHILD(child, LEFT), LEFT);
            RB_NODE_SET_CHILD(child, temp, LEFT);
        }
    
        //交换俩个节点的右孩子
        RB_NODE_SET_CHILD(child, RB_NODE_GET_CHILD(node, RIGHT), RIGHT);
        RB_NODE_SET_CHILD(node, NULL, RIGHT);//原child的右孩子一定不存在
        //更新堆栈关系
        rb_node *temp = stack[size0 - 1];
        stack[size0 - 1] = stack[size - 1];
        stack[size - 1] = temp;
    
        //交换俩个节点颜色
        char color = RB_NODE_GET_COLOR(node);
        RB_NODE_SET_COLOR(node, RB_NODE_GET_COLOR(child));
        RB_NODE_SET_COLOR(child, color);
    }
    
    ERROR_PRINT(RB_NODE_GET_CHILD(node, LEFT)  != NULL &&
                RB_NODE_GET_CHILD(node, RIGHT) != NULL, "s_d_n: child");
    ERROR_RETURN(RB_NODE_GET_CHILD(node, LEFT)  != NULL &&
                RB_NODE_GET_CHILD(node, RIGHT) != NULL);

    //5.获得唯一的孩子,或者没有
    rb_node *child = (RB_NODE_GET_CHILD(node, LEFT) == NULL ?
                      RB_NODE_GET_CHILD(node, RIGHT) :
                      RB_NODE_GET_CHILD(node, LEFT));

    //6.如果是删除根节点
    if (size < 2) {
        RB_TREE_SET_ROOT(tree, child);
        if (child != NULL)
            RB_NODE_SET_BLACK(child);
        else
            RB_TREE_SET_MAX_DEPTH(tree, 0);
        return;
    }
    
    //7.获得要删除节点的父亲
    rb_node *parent = stack[size - 2];



    //8.调整修复树
    if (child == NULL) {
        if (RB_NODE_GET_COLOR(node) == BLACK)
            //特殊情况:如果要删除的节点是没有子节点的
            //那么我们在做缺少的黑色旋转时将它留在原地
            //当它们隔离它时将用适当的NULL替换它       
            //调整去除无子的黑色节点(此时节点未删除)
            adjust_remove_node(stack, size, node);
        else {
            char side = RB_NODE_PARENT_GET_SIDE(node, parent);
            ERROR_PRINT(side == ERROR, "s_d_n: side");
            ERROR_RETURN(side == ERROR);
            //红色的无子节点可以直接删除
            RB_NODE_SET_CHILD(parent, NULL, side);
        }
    } else {
        //删除的节点有一个孩子
        char side = RB_NODE_PARENT_GET_SIDE(node, parent);
        ERROR_PRINT(side == ERROR, "s_d_n: side");
        ERROR_RETURN(side == ERROR);
        RB_NODE_SET_CHILD(parent, child, side);//删除节点
        //如果该孩子可以直接染色修复
        if (RB_NODE_GET_COLOR(node) == RED ||
            RB_NODE_GET_COLOR(child) == RED)
            RB_NODE_SET_BLACK(child);
        else {
            //俩个节点都为黑色,但已经删除了node节点,让孩子节点顶替上去
            stack[size - 1] = child;
            //节点已经删除,直接修复
            adjust_remove_node(stack, size, NULL);
        }
    }

    //我们可能已经旋转到根了
    RB_TREE_SET_ROOT(tree, stack[0]);
}

///////////////////////////////////////////////////////////////////////////////
//查找函数/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static rb_node * only_search_node(rb_tree *tree, void *target_data)
{
    ERROR_PRINT(tree == NULL, "n_s_i_t: tree");
    ERROR_RETURN_NULL(tree == NULL);
    ERROR_PRINT(tree->key_compare == NULL, "n_s_i_t: key_compare");
    ERROR_RETURN_NULL(tree->key_compare == NULL);
    ERROR_PRINT(tree->key_comfirm == NULL, "n_s_i_t: key_comfirm");
    ERROR_RETURN_NULL(tree->key_comfirm == NULL);
    
    rb_node *search_index = RB_TREE_GET_ROOT(tree);
    while (search_index != NULL) {
        if (RB_TREE_USE_KEY_COMFIRM(tree, target_data, 
            RB_NODE_GET_DATA(search_index)) == 0)
            return search_index;
        
        //该结点小于此本结点,到左子树去
        if (RB_TREE_USE_KEY_COMPARE(tree, target_data,
            RB_NODE_GET_DATA(search_index))) {
            if (RB_NODE_GET_CHILD(search_index, LEFT) != NULL) {
                search_index = RB_NODE_GET_CHILD(search_index, LEFT);
                continue;
            } else {
                return NULL;
            }
        }
        
        //该结点大于此本结点,到右子树去
        if (!RB_TREE_USE_KEY_COMPARE(tree, target_data,
            RB_NODE_GET_DATA(search_index))) {
            if (RB_NODE_GET_CHILD(search_index, RIGHT) != NULL) {
                search_index = RB_NODE_GET_CHILD(search_index, RIGHT);
                continue;
            } else {
                return NULL;
            }
        }
    }
    
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//核心动作/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void insert_node(void *tree, void *node)
{
    single_insert_node((rb_tree *)tree, (rb_node *)node);
}

void remove_node(void *tree, void *data)
{
    rb_node *node = only_search_node((rb_tree *)tree, data);
    if (node == NULL)
        return;
    
    single_remove_node((rb_tree *)tree, node);
}

void * search_data(void *tree, void *target_data)
{
    rb_node *temp = only_search_node((rb_tree *)tree, target_data);
    if (temp == NULL)
        return NULL;
    else
        return RB_NODE_GET_DATA(temp);
}

///////////////////////////////////////////////////////////////////////////////
//辅助动作/////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void initialize_tree(void *tree, compare k_compare, comfirm k_confirm)
{
    //if (sizeof(tree) != sizeof(rb_tree)) return;
    RB_TREE_SET_ROOT((rb_tree *)tree, NULL);
    RB_TREE_SET_KEY_COMPARE((rb_tree *)tree, k_compare);
    RB_TREE_SET_KEY_COMFIRM((rb_tree *)tree, k_confirm);
}

void initialize_node(void *node, void *data)
{
    //if (sizeof(node) != sizeof(rb_node)) return;
    RB_NODE_SET_DATA((rb_node *)node, data);
}

void config_stack(void *tree, void *stack, unsigned int length)
{
    RB_TREE_SET_STACK((rb_tree *)tree, (rb_node **)stack);
    RB_TREE_SET_STACK_LENGTH((rb_tree *)tree, length);
}

unsigned int get_tree_max_depth(void *tree)
{
    return RB_TREE_GET_MAX_DEPTH((rb_tree *)tree);
}

///////////////////////////////////////////////////////////////////////////////
//内部检查(数据遍历)///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void internal_check_for_level(void *tree, print k_print,
                              void *queue, int max_length)
{
    ERROR_PRINT(tree == NULL, "i_c_f_e: tree");
    ERROR_RETURN(tree == NULL);
    ERROR_PRINT(k_print == NULL, "i_c_f_e: print");
    ERROR_RETURN(k_print == NULL);
    //层序遍历(这里使用循环队列):
    
    //获取插入的起始位置
    int queue_head = 0;
    int queue_tail = 0;
    int element_number = 0;
    ((rb_node **)queue)[queue_tail++] = ((rb_tree *)tree)->rb_root;
    element_number++;
    do {
        //节点出队列
        rb_node *node = ((rb_node **)queue)[queue_head++];
        element_number--;
        ERROR_RETURN(node == NULL);
        //节点打印
        k_print(RB_NODE_GET_DATA(node), RB_NODE_GET_COLOR(node));
        //这是一个循环队列
        if (queue_head >= max_length)
            queue_head = 0;
        if (queue_tail >= max_length)
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
        ERROR_PRINT(element_number >= max_length, "i_c_f_e: overflow");
        ERROR_RETURN(element_number >= max_length);
    } while (element_number > 0);
}


、