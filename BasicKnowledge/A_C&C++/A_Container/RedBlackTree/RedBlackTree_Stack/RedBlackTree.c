//C std lib
#include <stdint.h>
#include <stdbool.h>

#define   REDBLACKTREE_C
#include "RedBlackTree.h"

/*静态内联:
 *因为部分动作引用过多,静态内联可能造成额外空间开销
 *所以可以使用该宏去除内联特性
 *但接口必须保持静态特性,因为它不可以向上层暴露
 *评估中......
 */
#define STATIC_INLINE static inline

/* 错误检查宏 */
#ifndef ERROR_PRINT
#define ERROR_PRINT(target, str) 
#endif

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 红黑树核心(红黑树容器) */
struct RedBlackTreeContainer {
	struct RedBlackTreeContainer *RightChild;   //节点左孩子
	struct RedBlackTreeContainer *LeftChild;    //节点右孩子
    void  *DataAndColor;                        //要维护的上层数据集索引(含节点)
};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 红黑树核心(红黑树集合) */
struct RedBlackTreeSet {
	struct RedBlackTreeContainer  *RB_Root;
    uint8_t (*KeyCompare)(void *ThisData, void *ThatData);
    uint8_t (*KeyConfirm)(void *ThatData, void *TargetData);
    //红黑树维护与追踪时需要使用到的堆栈
    struct RedBlackTreeContainer **StackIterator;
    uint32_t StackLength;   //堆栈的元素最大容量(堆栈大小)
    uint32_t MaxDepth;      //红黑树最大深度
    uint32_t NodeNumber;    //红黑树中的节点数量

};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 核心转义 */
typedef struct RedBlackTreeContainer    RB_Node;
typedef struct RedBlackTreeSet          RB_Tree;
//ThisData严格小于ThatData返回非零值
typedef uint8_t (*RBT_KeyCompare)(void *ThisData, void *ThatData);
//ThisData与TargetData的关键字一致返回0
typedef uint8_t (*RBT_KeyComfirm)(void *ThatData, void *TargetData);

typedef enum {RED = 0,  BLACK = 1}  RB_Color;
typedef enum {LEFT = 0, RIGHT = 1}  RB_Side;
typedef enum {ERROR = 2}            RB_Error;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 红黑树集合抽象方法 */
STATIC_INLINE RB_Node * RBTreeGetRoot(RB_Tree *Tree)
{
    return Tree->RB_Root;
}

STATIC_INLINE void RBTreeSetRoot(RB_Tree *Tree, RB_Node *Root)
{
    Tree->RB_Root = Root;
}

STATIC_INLINE void RBTreeSetCompare(RB_Tree *Tree, RBT_KeyCompare Compare)
{
    Tree->KeyCompare = Compare;
}
   
STATIC_INLINE void RBTreeSetComfirm(RB_Tree *Tree, RBT_KeyComfirm Comfirm)
{
    Tree->KeyConfirm = Comfirm;
}

STATIC_INLINE uint8_t RBTreeCompare(RB_Tree *Tree, void *Data1, void *Data2)
{
    return Tree->KeyCompare(Data1, Data2);
}

STATIC_INLINE uint8_t RBTreeComfirm(RB_Tree *Tree, void *Data1, void *Data2)
{
    return Tree->KeyConfirm(Data1, Data2);
}

STATIC_INLINE RB_Node ** RBTreeGetStack(RB_Tree *Tree)
{
    return Tree->StackIterator;
}

STATIC_INLINE void RBTreeSetStack(RB_Tree *Tree, RB_Node **Stack)
{
    Tree->StackIterator = Stack;
}

STATIC_INLINE uint32_t RBTreeGetStackLength(RB_Tree *Tree)
{
    return Tree->StackLength;
}

STATIC_INLINE void RBTreeSetStackLength(RB_Tree *Tree, uint32_t Length)
{
    Tree->StackLength = Length;
}

STATIC_INLINE uint32_t RBTreeGetMaxDepth(RB_Tree *Tree)
{
    return Tree->MaxDepth;
}

STATIC_INLINE void RBTreeSetMaxDepth(RB_Tree *Tree, uint32_t Depth)
{
    Tree->MaxDepth = Depth;
}

STATIC_INLINE uint32_t RBTreeGetNodeNumber(RB_Tree *Tree)
{
    return Tree->NodeNumber;
}

STATIC_INLINE void RBTreeSetNodeNumber(RB_Tree *Tree, uint32_t Number)
{
    Tree->NodeNumber = Number;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 红黑树容器抽象方法(获取) */

STATIC_INLINE void * RBNodeGetData(RB_Node *Node)
{
    VOID_STAR_TYPE Result = 0;
    
    Result |= (VOID_STAR_TYPE)(Node->DataAndColor);
    Result &= (VOID_STAR_TYPE)(~1);

    return (void *)Result;
}

STATIC_INLINE uint8_t RBNodeGetColor(RB_Node *Node)
{
    VOID_STAR_TYPE Result = 0;
    
    Result |= (VOID_STAR_TYPE)(Node->DataAndColor);
    Result &= (VOID_STAR_TYPE)(1);
    
    return (uint8_t)Result;
}

STATIC_INLINE RB_Node * RBNodeGetChild(RB_Node *Node, uint8_t Side)
{
    if (Side == LEFT)
        return Node->LeftChild;
    if (Side == RIGHT)
        return Node->RightChild;
    return NULL;
}

STATIC_INLINE uint8_t RBNodeGetSide(RB_Node *Node, RB_Node *Parent)
{
    if (Node == Parent->LeftChild)
        return LEFT;
    if (Node == Parent->RightChild)
        return RIGHT;
    return ERROR;
}

STATIC_INLINE uint8_t RBNodeGetOtherSide(uint8_t Side)
{
    if (Side == LEFT)
        return RIGHT;
    if (Side == RIGHT)
        return LEFT;
    return ERROR;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 红黑树容器抽象方法(设置) */

STATIC_INLINE void RBNodeSetData(RB_Node *Node, void *Data)
{
    VOID_STAR_TYPE Result1 = 0;
    VOID_STAR_TYPE Result2 = 0;
    
    Result1 |= (VOID_STAR_TYPE)(Node->DataAndColor);
    Result1 &= (VOID_STAR_TYPE)(1);
    Result2 |= (VOID_STAR_TYPE)(Data);
    Result2 &= (VOID_STAR_TYPE)(~1);

    Node->DataAndColor = (void *)(Result1 | Result2);
}

STATIC_INLINE void RBNodeSetColor(RB_Node *Node, uint8_t Color)
{
    VOID_STAR_TYPE Result = 0;
    
    Result |= (VOID_STAR_TYPE)(Node->DataAndColor);
    Result &= (VOID_STAR_TYPE)(~1);
    Result |= (VOID_STAR_TYPE)(Color & 1);
    
    Node->DataAndColor = (void *)(Result);
}

STATIC_INLINE void RBNodeSetChild(RB_Node *Node, RB_Node *Child, uint8_t Side)
{
    if (Side == LEFT)
        Node->LeftChild  = Child;
    if (Side == RIGHT)
        Node->RightChild = Child;
}

STATIC_INLINE void RBNodeSetBlack(RB_Node *Node)
{
    VOID_STAR_TYPE Result = 0;
    
    Result |= (VOID_STAR_TYPE)(Node->DataAndColor);
    Result &= (VOID_STAR_TYPE)(~1);
    Result |= (VOID_STAR_TYPE)(BLACK);
    
    Node->DataAndColor = (void *)(Result);
}
    
STATIC_INLINE void RBNodeSetRed(RB_Node *Node)
{
    VOID_STAR_TYPE Result = 0;
    
    Result |= (VOID_STAR_TYPE)(Node->DataAndColor);
    Result &= (VOID_STAR_TYPE)(~1);
    Result |= (VOID_STAR_TYPE)(RED);
    
    Node->DataAndColor = (void *)(Result);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 辅助定制宏,美化代码,剔除边缘逻辑 */
#define RETURN_EMPTY(target) if (target) return;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 红黑树核心动作:交换(左右旋转) */
static void RBT_RotateNode(RB_Node **Stack, int32_t Size)
{
    ERROR_PRINT(Stack == NULL,  "RBT_RotateNode: Stack");
    ERROR_PRINT(Size < 2,       "RBT_RotateNode: Size");
    //到栈顶位置获得父子节点
    RB_Node *Parent = Stack[Size - 2];
    RB_Node *Child  = Stack[Size - 1];
    //获得父子节点的关系
    uint8_t Side        = RBNodeGetSide(Child, Parent);
    uint8_t OtherSide   = RBNodeGetOtherSide(Side);
    ERROR_PRINT(Side == ERROR,  "RBT_RotateNode: Child:Side");
    //获得子节点的俩个孩子
    RB_Node *OuterChild = RBNodeGetChild(Child, Side);
    RB_Node *InnerChild = RBNodeGetChild(Child, OtherSide);
    //存在祖父节点
    if (Size >= 3) {
        //获取祖父节点索引
        RB_Node *GrandParent = Stack[Size - 3];
        //获得祖父与父亲的关系
        uint8_t ParentSide = RBNodeGetSide(Parent, GrandParent);
        ERROR_PRINT(ParentSide == ERROR, "RBT_RotateNode: Parent:Side");
        //现在孩子成为新的父节点
        RBNodeSetChild(GrandParent, Child, ParentSide);
    }
    
    //RBNodeSetChild(Child, OuterChild, Side);
    //更新父子关系
    RBNodeSetChild(Child, Parent, OtherSide);
    //父亲获得对位孙子为新孩子
    RBNodeSetChild(Parent, InnerChild, Side);
    //更新堆栈俩个节点关系
    Stack[Size - 2] = Child;
    Stack[Size - 1] = Parent;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 搜索函数(大小元) */
static RB_Node * RBT_Search_MinOrMax(RB_Node *Node, uint32_t Side)
{
    ERROR_PRINT(Node == NULL, "RBT_Search_MinOrMax: Node");
    
    while (Node != NULL && RBNodeGetChild(Node, Side) != NULL)
        Node = RBNodeGetChild(Node, Side);
    
    //该测试没有逻辑意义
    ERROR_PRINT(RBNodeGetChild(Node, LEFT)  != NULL &&
                RBNodeGetChild(Node, RIGHT) != NULL,
                "RBT_Search_MinOrMax: Child");
    
    return Node;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 插入和删除的准备函数(节点入栈) */
static void RBT_Stack_Prepare(RB_Tree *Tree, RB_Node *Node, RB_Node **Stack,
                              int32_t *Size, int32_t Remove)
{
    ERROR_PRINT(Tree == NULL, "RBT_Stack_Prepare: Tree");
    //根节点入栈
    Stack[(*Size = 1) - 1] = RBTreeGetRoot(Tree);
    //自此以后从该节点向下依次渐入
    do {
        //删除动作时额外需要去停留在此处,插入动作时无需
        //因为我们关心的是按顺序排序而不是冲突修改
        if (Remove && 
            RBTreeComfirm(Tree, 
            RBNodeGetData(Node), 
            RBNodeGetData(Stack[*Size - 1])) == 0)
            return;
        //红黑树具有二叉查找树的性质,可以以此规则获得迭代记录
        int32_t Side = ERROR;
        //如果目标小于该节点
        if (RBTreeCompare(Tree, 
            RBNodeGetData(Node),
            RBNodeGetData(Stack[*Size - 1])) != 0)
            Side = LEFT;
        //如果目标大于等于该节点(这里是对称书写,实际应该省略)
        if (RBTreeCompare(Tree, 
            RBNodeGetData(Node),
            RBNodeGetData(Stack[*Size - 1])) == 0)
            Side = RIGHT;
        //再检查一次(实际使用时被裁空)
        ERROR_PRINT(Side == ERROR, "RBT_Stack_Prepare: Side");
        //获取其孩子
        RB_Node *Child = RBNodeGetChild(Stack[*Size - 1], Side);
        //检查孩子存在性决定是否继续向下搜索
        if (Child == NULL)
            break;
        if (Child != NULL)
            Stack[(*Size)++] = Child;
        //当没有找到参数相同的节点break退出时
        //此时栈顶保留了最后一次查找记录
        //它停留的位置其孩子刚好为空(适合插入)
    } while (1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 插入调整函数 */
static void RBT_InsertNode_Adjust(RB_Node **Stack, int32_t Size)
{
    ERROR_PRINT(Stack == NULL, "RBT_InsertNode_Adjust: Stack");
    
    while (Size > 1) {
        RB_Node *Node   = Stack[Size - 1];
        RB_Node *Parent = Stack[Size - 2];
    
        //节点的左右孩子不是红色的
        ERROR_PRINT(RBNodeGetChild(Node, LEFT) != NULL &&
                    RBNodeGetChild(Node, LEFT) == RED,
                    "RBT_InsertNode_Adjust: LeftChild red");
        ERROR_PRINT(RBNodeGetChild(Node, RIGHT) != NULL &&
                    RBNodeGetChild(Node, RIGHT) == RED,
                    "RBT_InsertNode_Adjust: RightChild red");
        
        //父节点是黑节点无需调整
        RETURN_EMPTY(RBNodeGetColor(Parent) == BLACK);
        
        //红节点一定有父亲
        ERROR_PRINT(Size < 2, "RBT_InsertNode_Adjust: lost Parent");
        
        //获取祖父节点
        RB_Node *GrandParent = Stack[Size - 3];
        //确认父亲与祖父的对位关系
        uint8_t Side         = RBNodeGetSide(Parent, GrandParent);
        uint8_t OtherSide    = RBNodeGetOtherSide(Side);
        ERROR_PRINT(Side == ERROR, "RBT_InsertNode_Adjust: Side");
        
        //获得父亲的兄弟节点
        RB_Node *Uncle = RBNodeGetChild(GrandParent, OtherSide);
        
        //情况1:叔叔节点是红色(父亲也是红色节点,黑色节点下沉)
        if (Uncle != NULL && RBNodeGetColor(Uncle) == RED) {
            //1.染红祖父节点
            RBNodeSetRed(GrandParent);
            //2.染黑叔叔节点与父亲节点
            RBNodeSetBlack(Parent);
            RBNodeSetBlack(Uncle);
            //3.红色节点冲突回退到祖父节点
            Size -= 2;
            continue; 
        }
        
        //情况2:叔叔节点不存在或为黑色(本地旋转修正该树)
        uint8_t ParentSide = RBNodeGetSide(Node, Parent);
        ERROR_PRINT(ParentSide == ERROR, "RBT_InsertNode_Adjust: ParentSide");
        //情况2.1:本节点与祖父节点是对位关系,需要先旋转至顺位关系
        if (ParentSide != Side) {
            RBT_RotateNode(Stack, Size);
            Node = Stack[Size - 1];
        }
        //情况2.2:本节点与祖父节点是顺位关系
        //1.旋转父节点与祖父节点
        RBT_RotateNode(Stack, Size - 1);
        //2.原祖父节点染成红色
        RBNodeSetBlack(Stack[Size - 3]);
        //3.原父节点染成黑色
        RBNodeSetRed(Stack[Size - 2]);
        
        //如果是到这种情况,需要注意一下流程:
        //本节点(插入时的颜色为红色),父节点为红色,兄弟节点为黑色
        //旋转过后,本节点晋升为祖父节点(染成黑色),原父节点和祖父节点成为新的
        //父节点和兄弟节点(染成红色),原兄弟节点下沉,原黑色不变
        return;
    }
    
    //我们退出循环是因为已经到了根节点,但根节点一定是黑色的
    RBNodeSetBlack(Stack[0]);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 删除调整函数 */
static void RBT_RemoveNode_Adjust(RB_Node **Stack, int32_t Size, RB_Node *Null)
{
    //一直调整到根节点或调整完毕
    while (Size > 1) {
        RB_Node *Node   = Stack[Size - 1];
        RB_Node *Parent = Stack[Size - 2];
        //获得孩子与父亲的关系
        uint8_t Side        = RBNodeGetSide(Node, Parent);
        uint8_t OtherSide   = RBNodeGetOtherSide(Side);
        ERROR_PRINT(Side == ERROR, "RBT_RemoveNode_Adjust: Side");
        
        //获得兄弟节点
        RB_Node *Sibling    = RBNodeGetChild(Parent, OtherSide);
        //删除一个黑色节点导致的不平衡,兄弟节点一定存在
        ERROR_PRINT(Sibling == NULL, "RBT_RemoveNode_Adjust: Sibling");
        
        //情况1:如果兄弟节点是红色的
        if (RBNodeGetColor(Sibling) == RED) {
            //要注意的一点:如果兄弟节点为红色
            //那么父节点和它的所有子节点必为黑色
            //1.兄弟节点覆盖当前节点
            Stack[Size - 1] = Sibling;
            //2.兄弟节点与父节点旋转
            RBT_RotateNode(Stack, Size);
            //3.原父节点下沉为兄弟节点,染成红色
            RBNodeSetRed(Parent);
            //4.原兄弟节点晋升为父节点,染成黑色
            RBNodeSetBlack(Sibling);
            //对兄弟节点旋转意味着自己下沉一级,将自己重新加入到集合中
            //现在它是原父节点(现在兄弟节点)的子节点
            //5.当前节点重新加入枝干
            Stack[Size++] = Node;
            //6.重新定位父节点
            Parent = Stack[Size - 2];
            //7.重新定位兄弟节点
            Sibling = RBNodeGetChild(Parent, OtherSide);
        }
        
        //注意,旋转到下一级的时候
        //它的新兄弟一定是存在的,因为如果不存在
        //那么树在旋转之前本身就不是平衡的
        ERROR_PRINT(Sibling == NULL, "RBT_RemoveNode_Adjust: Sibling");
        //获得兄弟节点的孩子们
        RB_Node *LeftChild  = RBNodeGetChild(Sibling, LEFT);
        RB_Node *RightChild = RBNodeGetChild(Sibling, RIGHT);
        
        //情况2:兄弟孩子都是黑色的
        if ((LeftChild  == NULL || RBNodeGetColor(LeftChild) == BLACK) &&
            (RightChild == NULL || RBNodeGetColor(RightChild) == BLACK)) {
            
            //需要删除节点,断开关联
            if (Node == Null)
                RBNodeSetChild(Parent, NULL, Side);
            
            //兄弟只有黑孩子的情况有简单的解决办法
            //1.将新兄弟节点染成红色
            RBNodeSetRed(Sibling);
            
            if (RBNodeGetColor(Parent) == BLACK) {
                //2.父结点缺少一个黑色,向上迭代
                Size--;
                continue;
            } else {
                //重新上色使整个树OK
                //(父节点是红色直接染成黑色即可完成)
                RBNodeSetBlack(Parent);
                return;
            }
        }
        
        ERROR_PRINT((LeftChild  == NULL || RBNodeGetColor(LeftChild) == BLACK) &&
                    (RightChild == NULL || RBNodeGetColor(RightChild) == BLACK), 
                     "RBT_RemoveNode_Adjust: Child");
        //我们知道兄弟姐妹至少有一个红色的孩子
        //固定它,使远/外的位置(即在N的对面)肯定是红色的
        //如果远处位置是红色的(即在N的对面)不管里面是红是黑都不重要
        RB_Node *OuterChild = RBNodeGetChild(Sibling, OtherSide);
        //情况3:兄弟远处位置不存在红色节点
        if ((OuterChild == NULL || RBNodeGetColor(OuterChild) == BLACK)) {
            //1.获得内部节点
            RB_Node *InnerChild = RBNodeGetChild(Sibling, Side);
            //2.兄弟节点写入到栈顶
            Stack[Size - 1] = Sibling;
            //3.内部的红节点入栈
            Stack[Size++] = InnerChild;
            //4.旋转内部节点与兄弟节点
            RBT_RotateNode(Stack, Size);
            //5.原兄弟节点下沉为远端节点,染成红色
            RBNodeSetRed(Sibling);
            //6.原内部节点晋升为兄弟节点,染成黑色
            RBNodeSetBlack(InnerChild);
            //7.更新兄弟节点
            Sibling = Stack[Size - 2];
            //8.更新远端节点
            OuterChild = RBNodeGetChild(Sibling, OtherSide);
            //9.栈顶恢复成当前节点
            Stack[Size - 2] = Node;
            //10.回退栈顶
            Size--;
        }
        
        //最后,兄弟结点必须在远/外插槽中有一个红色的子结点
        //我们可以旋转兄弟和我们的父元素并重新着色以生成一个有效的树
        ERROR_PRINT((OuterChild == NULL || RBNodeGetColor(OuterChild) == BLACK),
                     "RBT_RemoveNode_Adjust: OuterChild");
        //情况4:兄弟远处位置存在红色节点
        //1.交换父亲与兄弟的颜色
        RBNodeSetColor(Sibling, RBNodeGetColor(Parent));
        RBNodeSetBlack(Parent);
        //2.远端红孩子染成黑色补色
        RBNodeSetBlack(OuterChild);
        //3.栈顶更新为兄弟节点
        Stack[Size - 1] = Sibling;
        //4.旋转兄弟节点与父节点
        RBT_RotateNode(Stack, Size);
        
        //需要删除节点,断开关联
        if (Node == Null)
            RBNodeSetChild(Parent, NULL, Side);
        
        return;
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 插入函数 */
static void RBT_InsertNode_All(RB_Tree *Tree, RB_Node *Node)
{
    ERROR_PRINT(Tree == NULL,             "RBT_InsertNode_All: Tree");
    ERROR_PRINT(Node == NULL,             "RBT_InsertNode_All: Node");
    ERROR_PRINT(Tree->KeyCompare == NULL, "RBT_InsertNode_All: KeyCompare");

    //辅助初始化节点(可能需要)
    RBNodeSetChild(Node, NULL, LEFT);
    RBNodeSetChild(Node, NULL, RIGHT);

    //第一次插入
    if (RBTreeGetRoot(Tree) == NULL) {
        RBTreeSetRoot(Tree, Node);
        RBTreeSetMaxDepth(Tree, 1);
        RBNodeSetBlack(Node);
        return;
    } 

    //1.获得其对应的栈
    RB_Node **Stack = RBTreeGetStack(Tree);
    //2.获取插入的枝干
    int32_t Size = -1;
    RBT_Stack_Prepare(Tree, Node, Stack, &Size, 0);
    //3.获得父节点
    RB_Node *Parent = Stack[Size - 1];
    //4.获得父子节点的关联
    uint8_t Side = ERROR;
    
    if (RBTreeCompare(Tree, 
        RBNodeGetData(Node), 
        RBNodeGetData(Parent)) != 0)
        Side = LEFT;
    if (RBTreeCompare(Tree, 
        RBNodeGetData(Node), 
        RBNodeGetData(Parent)) == 0)
        Side = RIGHT;
    //5.建立亲子关系
    RBNodeSetChild(Parent, Node, Side);
    //6.染红插入的节点
    RBNodeSetRed(Node);
    //7.当前节点入栈
    Stack[Size++] = Node;
    //8.修复该树
    RBT_InsertNode_Adjust(Stack, Size);
    //更新树深
    if (Size > RBTreeGetMaxDepth(Tree))
        RBTreeSetMaxDepth(Tree, Size);
    //可能调整到根节点
    RBTreeSetRoot(Tree, Stack[0]);
    //树的根是黑色的
    ERROR_PRINT(RBNodeGetColor(Stack[0]) != BLACK, "RBT_InsertNode_All: Root");
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 删除函数 */
static void RBT_RemoveNode_All(RB_Tree *Tree, RB_Node *Node)
{
    ERROR_PRINT(Tree == NULL, "RBT_RemoveNode_All: Tree");
    
    //1.获得其对应的栈
    RB_Node **Stack = RBTreeGetStack(Tree);
    //2.获取删除的枝干
    int32_t Size = -1;
    RBT_Stack_Prepare(Tree, Node, Stack, &Size, 1);
    //3.没找到要删除节点
    if (RBTreeComfirm(Tree,
        RBNodeGetData(Node),
        RBNodeGetData(Stack[Size - 1])) != 0)
        return;
    
    //实际上非侵入式的数据结构直接交换其子节点数据索引即可完成
    //这里是继承Zephyr的改写,它原是侵入式数据结构,所以保留
    
    //我们只能删除一个有0或1个子节点的节点
    //如果我们有两个,那么选择边0的“最大”子节点(最小的1也可以)
    //并与它交换我们在树中的位置
    //4.获得目标节点
    if (RBNodeGetChild(Node, LEFT)  != NULL &&
        RBNodeGetChild(Node, RIGHT) != NULL) {
        int32_t Size0 = Size;
        //1.获得左孩子
        RB_Node *Child = RBNodeGetChild(Node, LEFT);
        //2.保存当前节点父亲
        RB_Node *HighParent = Size > 1 ? Stack[Size - 2] : NULL;
        //3.左孩子入栈
        Stack[Size++] = Child;
        //4.迭代寻找其右孩子
        while (RBNodeGetChild(Child, RIGHT) != NULL) {
            Child = RBNodeGetChild(Child, RIGHT);
            Stack[Size++] = Child;
        }
        //5.获得要交换的孩子的父亲
        RB_Node *LowParent = Stack[Size - 2];

        //这里有一些特殊情况需要检查
        //原则上,这是通过在节点之间交换子指针
        //并将指向它们的节点从它们的父节点重新定位来实现的,但是:
        //(1)上面的节点可能是树的根,没有父节点
        //(2)下面的节点可能是上面节点的直接子节点
        //记得交换两个节点的颜色位
        //且没有父指针,所以跟踪这个结构的堆栈也需要交换!
        
        if (HighParent == NULL)
            RBTreeSetRoot(Tree, Child);
        if (HighParent != NULL) {
            uint8_t Side = RBNodeGetSide(Node, HighParent);
            ERROR_PRINT(Side == ERROR, "RBT_RemoveNode_All: Side");
            //为高父亲设置低孩子
            RBNodeSetChild(HighParent, Child, Side);
        }
        
        //直属亲子关系
        if (LowParent == Node) {
            //交换亲子关系(交换它们的左孩子)
            RBNodeSetChild(Node, RBNodeGetChild(Child, LEFT), LEFT);
            RBNodeSetChild(Child, Node, LEFT);
        }
        //跨度亲子关系
        if (LowParent != Node) {
            uint8_t LowSide = RBNodeGetSide(Child, LowParent);
            ERROR_PRINT(LowSide == ERROR, "RBT_RemoveNode_All: LowSide");
            //更新低父亲指向高孩子
            RBNodeSetChild(LowParent, Node, LowSide);
            //交换俩个节点的左孩子(可能存在)
            RB_Node *Temp = RBNodeGetChild(Node, LEFT);
            RBNodeSetChild(Node, RBNodeGetChild(Child, LEFT), LEFT);
            RBNodeSetChild(Child, Temp, LEFT);
        }
    
        //交换俩个节点的右孩子
        RBNodeSetChild(Child, RBNodeGetChild(Node, RIGHT), RIGHT);
        //原child的右孩子一定不存在
        RBNodeSetChild(Node, NULL, RIGHT);
        //更新堆栈关系
        RB_Node *Temp       = Stack[Size0 - 1];
        Stack[Size0 - 1]    = Stack[Size - 1];
        Stack[Size - 1]     = Temp;
    
        //交换俩个节点颜色
        uint8_t Color = RBNodeGetColor(Node);
        RBNodeSetColor(Node, RBNodeGetColor(Child));
        RBNodeSetColor(Child, Color);
    }
    
    ERROR_PRINT(RBNodeGetChild(Node, LEFT)  != NULL &&
                RBNodeGetChild(Node, RIGHT) != NULL,
                "RBT_RemoveNode_All: Child");

    //5.获得唯一的孩子,或者没有
    RB_Node *Child = (RBNodeGetChild(Node, LEFT) == NULL ?
                      RBNodeGetChild(Node, RIGHT) :
                      RBNodeGetChild(Node, LEFT));

    //6.如果是删除根节点
    if (Size < 2) {
        RBTreeSetRoot(Tree, Child);
        if (Child != NULL)
            RBNodeSetBlack(Child);
        if (Child == NULL)
            RBTreeSetMaxDepth(Tree, 0);
        return;
    }
    
    //7.获得要删除节点的父亲
    RB_Node *Parent = Stack[Size - 2];

    //8.调整修复树
    //删除的节点没有孩子
    if (Child == NULL) {
        //红色的无子节点可以直接删除
        if (RBNodeGetColor(Node) == RED) {
            uint8_t Side = RBNodeGetSide(Node, Parent);
            ERROR_PRINT(Side == ERROR, "RBT_RemoveNode_All: Side");
            RBNodeSetChild(Parent, NULL, Side);
        }
        //特殊情况:如果要删除的节点是没有子节点的
        //那么我们在做缺少的黑色旋转时将它留在原地
        //当它们隔离它时将用适当的NULL替换它       
        //调整去除无子的黑色节点(此时节点未删除)
        if (RBNodeGetColor(Node) == BLACK)
            RBT_RemoveNode_Adjust(Stack, Size, Node);
    }
    //删除的节点有一个孩子
    if (Child != NULL) {
        uint8_t Side = RBNodeGetSide(Node, Parent);
        ERROR_PRINT(Side == ERROR, "RBT_RemoveNode_All: Side");
        RBNodeSetChild(Parent, Child, Side);//删除节点
        //如果该孩子可以直接染色修复
        if (RBNodeGetColor(Node)  == RED ||
            RBNodeGetColor(Child) == RED)
            RBNodeSetBlack(Child);
        else {
            //俩个节点都为黑色,但已经删除了node节点,让孩子节点顶替上去
            Stack[Size - 1] = Child;
            //节点已经删除,直接修复
            RBT_RemoveNode_Adjust(Stack, Size, NULL);
        }
    }

    //我们可能已经旋转到根了
    RBTreeSetRoot(Tree, Stack[0]);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 查找函数 */
static RB_Node * RBT_SearchNode_Only(RB_Tree *Tree, void *TargetData)
{
    ERROR_PRINT(Tree == NULL,             "RBT_SearchNode_Only: Tree");
    ERROR_PRINT(Tree->KeyCompare == NULL, "RBT_SearchNode_Only: KeyCompare");
    ERROR_PRINT(Tree->KeyConfirm == NULL, "RBT_SearchNode_Only: KeyConfirm");
    
    RB_Node *Node = RBTreeGetRoot(Tree);
    while (Node != NULL) {
        //关键字匹配成功,返回
        if (RBTreeComfirm(Tree, TargetData, 
            RBNodeGetData(Node)) == 0)
            return Node;
        
        //该结点小于此本结点,到左子树去
        if (RBTreeCompare(Tree, TargetData,
            RBNodeGetData(Node)) != 0) {
            Node = RBNodeGetChild(Node, LEFT);
            continue;
        }
        
        //该结点大于此本结点,到右子树去
        if (RBTreeCompare(Tree, TargetData,
            RBNodeGetData(Node)) == 0) {
            Node = RBNodeGetChild(Node, RIGHT);
            continue;
        }
    }
    
    return NULL;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 上层接口封装 */

void RBT_InsertNode(void *Tree, void *Node)
{
    RBT_InsertNode_All((RB_Tree *)Tree, (RB_Node *)Node);
}

void RBT_RemoveNode(void *Tree, void *Data)
{
    RB_Node *Node = RBT_SearchNode_Only((RB_Tree *)Tree, Data);
    if (Node == NULL)
        return;
    
    RBT_RemoveNode_All((RB_Tree *)Tree, Node);
}

void * RBT_SearchData(void *Tree, void *TargetData)
{
    RB_Node *Temp = RBT_SearchNode_Only((RB_Tree *)Tree, TargetData);

    return Temp == NULL ? NULL : RBNodeGetData(Temp);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 辅助动作 */

void RBT_SetTree(void *Tree, Compare k_compare, Comfirm k_confirm)
{
    RBTreeSetRoot((RB_Tree *)Tree, NULL);
    RBTreeSetCompare((RB_Tree *)Tree, k_compare);
    RBTreeSetComfirm((RB_Tree *)Tree, k_confirm);
}

void RBT_SetRoot(void *Tree, void *Node)
{
    RBTreeSetRoot((RB_Tree *)Tree, (RB_Node *)Node);
}

void RBT_GetRoot(void *Tree, void **Node)
{
    *Node = (void *)RBTreeGetRoot((RB_Tree *)Tree);
}

void RBT_SetNodeData(void *Node, void *Data)
{
    RBNodeSetData((RB_Node *)Node, Data);
}

void RBT_GetNodeData(void *Node, void **Data)
{
    *Data = RBNodeGetData((RB_Node *)Node);
}

void RBT_SetTreeStack(void *Tree, void *Stack, uint32_t Length)
{
    RBTreeSetStack((RB_Tree *)Tree, (RB_Node **)Stack);
    RBTreeSetStackLength((RB_Tree *)Tree, Length);
}

void RBT_GetTreeStack(void *Tree, void **Stack, uint32_t *Length)
{
    *(RB_Node **)Stack = RBTreeGetStack((RB_Tree *)Tree);
    *Length            = RBTreeGetStackLength((RB_Tree *)Tree);
}

uint32_t RBT_GetMaxDepth(void *Tree)
{
    return RBTreeGetMaxDepth((RB_Tree *)Tree);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 额外的辅助函数,为外界提供数据的建立空间 */

uint32_t RBT_GetBytes_Container(void)
{
    return sizeof(RB_Node);
}

uint32_t RBT_GetBytes_Set(void)
{
    return sizeof(RB_Tree);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 窥探:数据的层序遍历并打印 */
void RBT_Sequence_Traversal(void *Tree, Print Printf, void *Queue, int32_t Length)
{
    ERROR_PRINT(Tree == NULL,   "RBT_Sequence_Traversal: Tree");
    ERROR_PRINT(Queue == NULL,  "RBT_Sequence_Traversal: Tree");
    ERROR_PRINT(Printf == NULL, "RBT_Sequence_Traversal: print");
    //层序遍历(这里使用循环队列):
    
    //获取插入的起始位置
    int32_t QueueHead = 0;
    int32_t QueueTail = 0;
    int32_t ElementNumber = 0;
    ((RB_Node **)Queue)[QueueTail++] = RBTreeGetRoot((RB_Tree *)Tree);
    ElementNumber++;
    do {
        //节点出队列
        RB_Node *Node = ((RB_Node **)Queue)[QueueHead++];
        ElementNumber--;
        RETURN_EMPTY(Node == NULL);
        //节点打印
        Printf(Node, RBNodeGetData(Node), RBNodeGetColor(Node));
        //这是一个循环队列
        if (QueueHead >= Length)
            QueueHead = 0;
        if (QueueTail >= Length)
            QueueTail = 0; 
        
        //子节点入队列
        RB_Node *LeftChild  = RBNodeGetChild(Node, LEFT);
        RB_Node *RightChild = RBNodeGetChild(Node, RIGHT);
 
        //左孩子入队列
        if (LeftChild != NULL) {
            ((RB_Node **)Queue)[QueueTail++] = LeftChild;
            ElementNumber++;
        }
        
        //右孩子入队列
        if (RightChild != NULL) {
            ((RB_Node **)Queue)[QueueTail++] = RightChild;
            ElementNumber++;
        }
        
        //队列太小,警告
        ERROR_PRINT(ElementNumber >= Length, "RBT_Sequence_Traversal: overflow");
    } while (ElementNumber > 0);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
