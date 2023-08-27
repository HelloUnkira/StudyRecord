/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if 1
#include <stdio.h>
#define ERROR_PRINT(Condition, String)  if (Condition) printf("%s\n",(String));
#else
#define ERROR_PRINT(Condition, String)
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 红黑树核心(红黑树容器) */
typedef struct RedBlackTree_Node {
    struct RedBlackTree_Node *LeftChildAndColor;    /* 节点左孩子和颜色 */
    struct RedBlackTree_Node *RightChild;           /* 节点右孩子 */
} RBT_Node;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 红黑树核心(红黑树集合) */
typedef struct RedBlackTree_Tree {
    RBT_Node *Root;
    uint8_t (*KeyCompare)(RBT_Node *ThisNode, RBT_Node *ThatNode);
    uint8_t (*KeyConfirm)(RBT_Node *ThisNode, RBT_Node *ThatNode);
    /* 红黑树维护与追踪时需要使用到的堆栈 */
    RBT_Node **TraceStack;
    uint32_t   StackLength; /* 堆栈的元素最大容量(堆栈大小) */
    uint32_t   MaxDepth;    /* 红黑树最大深度 */
    uint32_t   NodeNumber;  /* 红黑树中的节点数量 */
} RBT_Tree;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* ThisNode严格小于ThatNode返回非零值 */
typedef uint8_t (*RBT_KeyCompare)(RBT_Node *ThisNode, RBT_Node *ThatNode);
/* ThisNode与ThatNode的关键字一致返回0 */
typedef uint8_t (*RBT_KeyConfirm)(RBT_Node *ThisNode, RBT_Node *ThatNode);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef enum {RED = 0,  BLACK = 1}  RBT_Color;      /* 节点颜色 */
typedef enum {LEFT = 0, RIGHT = 1}  RBT_Side;       /* 节点关系 */
typedef enum {ERROR = 2}            RBT_Error;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 红黑树集合抽象方法 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void RBT_TreeSetRoot(RBT_Tree *Tree, RBT_Node *Root)
{
    Tree->Root = Root;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void RBT_TreeSetCompare(RBT_Tree *Tree, RBT_KeyCompare Compare)
{
    Tree->KeyCompare = Compare;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void RBT_TreeSetConfirm(RBT_Tree *Tree, RBT_KeyConfirm Confirm)
{
    Tree->KeyConfirm = Confirm;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void RBT_TreeSetStack(RBT_Tree *Tree, RBT_Node **Stack)
{
    Tree->TraceStack = Stack;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void RBT_TreeSetStackLength(RBT_Tree *Tree, uint32_t Length)
{
    Tree->StackLength = Length;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void RBT_TreeSetNodeNumber(RBT_Tree *Tree, uint32_t Number)
{
    Tree->NodeNumber = Number;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void RBT_TreeSetMaxDepth(RBT_Tree *Tree, uint32_t Depth)
{
    Tree->MaxDepth = Depth;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline RBT_Node * RBT_TreeGetRoot(RBT_Tree *Tree)
{
    return Tree->Root;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint8_t RBT_TreeCompare(RBT_Tree *Tree, RBT_Node *Node1, RBT_Node *Node2)
{
    return Tree->KeyCompare(Node1, Node2);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint8_t RBT_TreeConfirm(RBT_Tree *Tree, RBT_Node *Node1, RBT_Node *Node2)
{
    return Tree->KeyConfirm(Node1, Node2);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline RBT_Node ** RBT_TreeGetStack(RBT_Tree *Tree)
{
    return Tree->TraceStack;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint32_t RBT_TreeGetStackLength(RBT_Tree *Tree)
{
    return Tree->StackLength;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint32_t RBT_TreeGetMaxDepth(RBT_Tree *Tree)
{
    return Tree->MaxDepth;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint32_t RBT_TreeGetNodeNumber(RBT_Tree *Tree)
{
    return Tree->NodeNumber;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 红黑树容器抽象方法 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void RBT_NodeSetColor(RBT_Node *Node, RBT_Color Color)
{
    uintptr_t Result = 0;
    
    Result |= (uintptr_t)(Node->LeftChildAndColor);
    Result &= (uintptr_t)(~1);
    Result |= (uintptr_t)(Color & 1);
    
    Node->LeftChildAndColor = (RBT_Node *)(Result);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void RBT_NodeSetChild(RBT_Node *Node, RBT_Node *Child, RBT_Side Side)
{
    if (Side == LEFT) {
        
        uintptr_t Result = 0;
        
        Result |= (uintptr_t)(Node->LeftChildAndColor);
        Result &= (uintptr_t)(1);
        Result |= (uintptr_t)(Child);
        
        Node->LeftChildAndColor = (RBT_Node *)Result;
    }
    if (Side == RIGHT)
        Node->RightChild = Child;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline RBT_Color RBT_NodeGetColor(RBT_Node *Node)
{
    uintptr_t Result = 0;
    
    Result |= (uintptr_t)(Node->LeftChildAndColor);
    Result &= (uintptr_t)(1);
    
    return (RBT_Color)Result;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline RBT_Node * RBT_NodeGetChild(RBT_Node *Node, RBT_Side Side)
{
    if (Side == LEFT) {
        uintptr_t Result = 0;
        
        Result |= (uintptr_t)(Node->LeftChildAndColor);
        Result &= (uintptr_t)(~1);
        
        return (RBT_Node *)Result;
    }
    if (Side == RIGHT)
        return Node->RightChild;
    return NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline RBT_Side RBT_NodeGetSide(RBT_Node *Node, RBT_Node *Parent)
{
    uintptr_t Result = 0;
    
    Result |= (uintptr_t)(Parent->LeftChildAndColor);
    Result &= (uintptr_t)(~1);
    
    RBT_Node *LeftChildAndColor = (RBT_Node *)Result;
    
    if (Node == LeftChildAndColor)
        return LEFT;
    if (Node == Parent->RightChild)
        return RIGHT;
    return ERROR;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline RBT_Side RBT_NodeGetOtherSide(RBT_Side Side)
{
    if (Side == LEFT)
        return RIGHT;
    if (Side == RIGHT)
        return LEFT;
    return ERROR;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 辅助定制宏,美化代码,剔除边缘逻辑 */
#define RETURN_EMPTY(target) if (target) return;
#define RETURN_NULL(Target)  if (Target) return NULL;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 红黑树核心动作:交换(左右旋转) */
static void RBT_RotateNode(RBT_Node **Stack, int32_t Size)
{
    ERROR_PRINT(Stack == NULL,  "RBT_RotateNode: Stack");
    ERROR_PRINT(Size < 2,       "RBT_RotateNode: Size");
    /* 到栈顶位置获得父子节点 */
    RBT_Node *Parent = Stack[Size - 2];
    RBT_Node *Child  = Stack[Size - 1];
    /* 获得父子节点的关系 */
    RBT_Side Side       = RBT_NodeGetSide(Child, Parent);
    RBT_Side OtherSide  = RBT_NodeGetOtherSide(Side);
    ERROR_PRINT(Side == ERROR,  "RBT_RotateNode: Child:Side");
    /* 获得子节点的俩个孩子 */
    RBT_Node *OuterChild = RBT_NodeGetChild(Child, Side);
    RBT_Node *InnerChild = RBT_NodeGetChild(Child, OtherSide);
    /* 存在祖父节点 */
    if (Size >= 3) {
        /* 获取祖父节点索引 */
        RBT_Node *GrandParent = Stack[Size - 3];
        /* 获得祖父与父亲的关系 */
        RBT_Side ParentSide = RBT_NodeGetSide(Parent, GrandParent);
        ERROR_PRINT(ParentSide == ERROR, "RBT_RotateNode: Parent:Side");
        /* 现在孩子成为新的父节点 */
        RBT_NodeSetChild(GrandParent, Child, ParentSide);
    }
    
    /* RBT_NodeSetChild(Child, OuterChild, Side); */
    /* 更新父子关系 */
    RBT_NodeSetChild(Child, Parent, OtherSide);
    /* 父亲获得对位孙子为新孩子 */
    RBT_NodeSetChild(Parent, InnerChild, Side);
    /* 更新堆栈俩个节点关系 */
    Stack[Size - 2] = Child;
    Stack[Size - 1] = Parent;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 搜索函数(大小元) */
static RBT_Node * RBT_Search_MinOrMax(RBT_Node *Node, uint32_t Side)
{
    ERROR_PRINT(Node == NULL, "RBT_Search_MinOrMax: Node");
    
    while (Node != NULL && RBT_NodeGetChild(Node, Side) != NULL)
        Node = RBT_NodeGetChild(Node, Side);
    
    /* 该测试没有逻辑意义 */
    ERROR_PRINT(RBT_NodeGetChild(Node, LEFT)  != NULL &&
                RBT_NodeGetChild(Node, RIGHT) != NULL,
                "RBT_Search_MinOrMax: Child");
    
    return Node;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 插入和删除的准备函数(节点入栈) */
static void RBT_Stack_Prepare(RBT_Tree *Tree, RBT_Node *Node, RBT_Node **Stack,
                              int32_t *Size, int32_t Remove)
{
    ERROR_PRINT(Tree == NULL, "RBT_Stack_Prepare: Tree");
    /* 根节点入栈 */
    Stack[(*Size = 1) - 1] = RBT_TreeGetRoot(Tree);
    /* 自此以后从该节点向下依次渐入 */
    do {
        /* 删除动作时额外需要去停留在此处,插入动作时无需 */
        /* 因为我们关心的是按顺序排序而不是冲突修改 */
        if (Remove && RBT_TreeConfirm(Tree, Node, Stack[*Size - 1]) == 0)
            return;
        /* 红黑树具有二叉查找树的性质,可以以此规则获得迭代记录 */
        int32_t Side = ERROR;
        /* 如果目标小于该节点 */
        if (RBT_TreeCompare(Tree, Node, Stack[*Size - 1]) != 0)
            Side = LEFT;
        /* 如果目标大于等于该节点(这里是对称书写,实际应该省略) */
        if (RBT_TreeCompare(Tree, Node, Stack[*Size - 1]) == 0)
            Side = RIGHT;
        /* 再检查一次(实际使用时被裁空) */
        ERROR_PRINT(Side == ERROR, "RBT_Stack_Prepare: Side");
        /* 获取其孩子 */
        RBT_Node *Child = RBT_NodeGetChild(Stack[*Size - 1], Side);
        /* 检查孩子存在性决定是否继续向下搜索 */
        if (Child == NULL)
            break;
        if (Child != NULL)
            Stack[(*Size)++] = Child;
        /* 当没有找到参数相同的节点break退出时 */
        /* 此时栈顶保留了最后一次查找记录 */
        /* 它停留的位置其孩子刚好为空(适合插入) */
    } while (1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 插入调整函数 */
static void RBT_InsertNode_Adjust(RBT_Node **Stack, int32_t Size)
{
    ERROR_PRINT(Stack == NULL, "RBT_InsertNode_Adjust: Stack");
    
    while (Size > 1) {
        RBT_Node *Node   = Stack[Size - 1];
        RBT_Node *Parent = Stack[Size - 2];
    
        /* 节点的左右孩子不是红色的 */
        ERROR_PRINT(RBT_NodeGetChild(Node, LEFT) != NULL &&
                    RBT_NodeGetChild(Node, LEFT) == RED,
                    "RBT_InsertNode_Adjust: LeftChildAndColor red");
        ERROR_PRINT(RBT_NodeGetChild(Node, RIGHT) != NULL &&
                    RBT_NodeGetChild(Node, RIGHT) == RED,
                    "RBT_InsertNode_Adjust: RightChild red");
        
        /* 父节点是黑节点无需调整 */
        RETURN_EMPTY(RBT_NodeGetColor(Parent) == BLACK);
        
        /* 红节点一定有父亲 */
        ERROR_PRINT(Size <= 2, "RBT_InsertNode_Adjust: lost Parent");
        
        /* 获取祖父节点 */
        RBT_Node *GrandParent = Stack[Size - 3];
        /* 确认父亲与祖父的对位关系 */
        RBT_Side Side       = RBT_NodeGetSide(Parent, GrandParent);
        RBT_Side OtherSide  = RBT_NodeGetOtherSide(Side);
        ERROR_PRINT(Side == ERROR, "RBT_InsertNode_Adjust: Side");
        
        /* 获得父亲的兄弟节点 */
        RBT_Node *Uncle = RBT_NodeGetChild(GrandParent, OtherSide);
        
        /* 情况1:叔叔节点是红色(父亲也是红色节点,黑色节点下沉) */
        if (Uncle != NULL && RBT_NodeGetColor(Uncle) == RED) {
            /* 1.染红祖父节点 */
            RBT_NodeSetColor(GrandParent, RED);
            /* 2.染黑叔叔节点与父亲节点 */
            RBT_NodeSetColor(Parent, BLACK);
            RBT_NodeSetColor(Uncle, BLACK);
            /* 3.红色节点冲突回退到祖父节点 */
            Size -= 2;
            continue; 
        }
        
        /* 情况2:叔叔节点不存在或为黑色(本地旋转修正该树) */
        RBT_Side ParentSide = RBT_NodeGetSide(Node, Parent);
        ERROR_PRINT(ParentSide == ERROR, "RBT_InsertNode_Adjust: ParentSide");
        /* 情况2.1:本节点与祖父节点是对位关系,需要先旋转至顺位关系 */
        if (ParentSide != Side) {
            RBT_RotateNode(Stack, Size);
            Node = Stack[Size - 1];
        }
        /* 情况2.2:本节点与祖父节点是顺位关系 */
        /* 1.旋转父节点与祖父节点 */
        RBT_RotateNode(Stack, Size - 1);
        /* 2.原祖父节点染成红色 */
        RBT_NodeSetColor(Stack[Size - 3], BLACK);
        /* 3.原父节点染成黑色 */
        RBT_NodeSetColor(Stack[Size - 2], RED);
        
        /* 如果是到这种情况,需要注意一下流程: */
        /* 本节点(插入时的颜色为红色),父节点为红色,兄弟节点为黑色 */
        /* 旋转过后,本节点晋升为祖父节点(染成黑色),原父节点和祖父节点成为新的 */
        /* 父节点和兄弟节点(染成红色),原兄弟节点下沉,原黑色不变 */
        return;
    }
    
    /* 我们退出循环是因为已经到了根节点,但根节点一定是黑色的 */
    RBT_NodeSetColor(Stack[0], BLACK);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 删除调整函数 */
static void RBT_RemoveNode_Adjust(RBT_Node **Stack, int32_t Size, RBT_Node *Null)
{
    /* 一直调整到根节点或调整完毕 */
    while (Size > 1) {
        RBT_Node *Node   = Stack[Size - 1];
        RBT_Node *Parent = Stack[Size - 2];
        /* 获得孩子与父亲的关系 */
        RBT_Side Side       = RBT_NodeGetSide(Node, Parent);
        RBT_Side OtherSide  = RBT_NodeGetOtherSide(Side);
        ERROR_PRINT(Side == ERROR, "RBT_RemoveNode_Adjust: Side");
        
        /* 获得兄弟节点 */
        RBT_Node *Sibling    = RBT_NodeGetChild(Parent, OtherSide);
        /* 删除一个黑色节点导致的不平衡,兄弟节点一定存在 */
        ERROR_PRINT(Sibling == NULL, "RBT_RemoveNode_Adjust: Sibling");
        
        /* 情况1:如果兄弟节点是红色的 */
        if (RBT_NodeGetColor(Sibling) == RED) {
            /* 要注意的一点:如果兄弟节点为红色 */
            /* 那么父节点和它的所有子节点必为黑色 */
            /* 1.兄弟节点覆盖当前节点 */
            Stack[Size - 1] = Sibling;
            /* 2.兄弟节点与父节点旋转 */
            RBT_RotateNode(Stack, Size);
            /* 3.原父节点下沉为兄弟节点,染成红色 */
            RBT_NodeSetColor(Parent, RED);
            /* 4.原兄弟节点晋升为父节点,染成黑色 */
            RBT_NodeSetColor(Sibling, BLACK);
            /* 对兄弟节点旋转意味着自己下沉一级,将自己重新加入到集合中 */
            /* 现在它是原父节点(现在兄弟节点)的子节点 */
            /* 5.当前节点重新加入枝干 */
            Stack[Size++] = Node;
            /* 6.重新定位父节点 */
            Parent = Stack[Size - 2];
            /* 7.重新定位兄弟节点 */
            Sibling = RBT_NodeGetChild(Parent, OtherSide);
        }
        
        /* 注意,旋转到下一级的时候 */
        /* 它的新兄弟一定是存在的,因为如果不存在 */
        /* 那么树在旋转之前本身就不是平衡的 */
        ERROR_PRINT(Sibling == NULL, "RBT_RemoveNode_Adjust: Sibling");
        /* 获得兄弟节点的孩子们 */
        RBT_Node *LeftChildAndColor  = RBT_NodeGetChild(Sibling, LEFT);
        RBT_Node *RightChild = RBT_NodeGetChild(Sibling, RIGHT);
        
        /* 情况2:兄弟孩子都是黑色的 */
        if ((LeftChildAndColor  == NULL || RBT_NodeGetColor(LeftChildAndColor) == BLACK) &&
            (RightChild == NULL || RBT_NodeGetColor(RightChild) == BLACK)) {
            
            /* 需要删除节点,断开关联 */
            if (Node == Null)
                RBT_NodeSetChild(Parent, NULL, Side);
            
            /* 兄弟只有黑孩子的情况有简单的解决办法 */
            /* 1.将新兄弟节点染成红色 */
            RBT_NodeSetColor(Sibling, RED);
            
            if (RBT_NodeGetColor(Parent) == BLACK) {
                /* 2.父结点缺少一个黑色,向上迭代 */
                Size--;
                continue;
            } else {
                /* 重新上色使整个树OK */
                /* (父节点是红色直接染成黑色即可完成) */
                RBT_NodeSetColor(Parent, BLACK);
                return;
            }
        }
        
        ERROR_PRINT((LeftChildAndColor  == NULL || RBT_NodeGetColor(LeftChildAndColor) == BLACK) &&
                    (RightChild == NULL || RBT_NodeGetColor(RightChild) == BLACK), 
                     "RBT_RemoveNode_Adjust: Child");
        /* 我们知道兄弟姐妹至少有一个红色的孩子 */
        /* 固定它,使远/外的位置(即在N的对面)肯定是红色的 */
        /* 如果远处位置是红色的(即在N的对面)不管里面是红是黑都不重要 */
        RBT_Node *OuterChild = RBT_NodeGetChild(Sibling, OtherSide);
        /* 情况3:兄弟远处位置不存在红色节点 */
        if ((OuterChild == NULL || RBT_NodeGetColor(OuterChild) == BLACK)) {
            /* 1.获得内部节点 */
            RBT_Node *InnerChild = RBT_NodeGetChild(Sibling, Side);
            /* 2.兄弟节点写入到栈顶 */
            Stack[Size - 1] = Sibling;
            /* 3.内部的红节点入栈 */
            Stack[Size++] = InnerChild;
            /* 4.旋转内部节点与兄弟节点 */
            RBT_RotateNode(Stack, Size);
            /* 5.原兄弟节点下沉为远端节点,染成红色 */
            RBT_NodeSetColor(Sibling, RED);
            /* 6.原内部节点晋升为兄弟节点,染成黑色 */
            RBT_NodeSetColor(InnerChild, BLACK);
            /* 7.更新兄弟节点 */
            Sibling = Stack[Size - 2];
            /* 8.更新远端节点 */
            OuterChild = RBT_NodeGetChild(Sibling, OtherSide);
            /* 9.栈顶恢复成当前节点 */
            Stack[Size - 2] = Node;
            /* 10.回退栈顶 */
            Size--;
        }
        
        /* 最后,兄弟结点必须在远/外插槽中有一个红色的子结点 */
        /* 我们可以旋转兄弟和我们的父元素并重新着色以生成一个有效的树 */
        ERROR_PRINT((OuterChild == NULL || RBT_NodeGetColor(OuterChild) == BLACK),
                     "RBT_RemoveNode_Adjust: OuterChild");
        /* 情况4:兄弟远处位置存在红色节点 */
        /* 1.交换父亲与兄弟的颜色 */
        RBT_NodeSetColor(Sibling, RBT_NodeGetColor(Parent));
        RBT_NodeSetColor(Parent, BLACK);
        /* 2.远端红孩子染成黑色补色 */
        RBT_NodeSetColor(OuterChild, BLACK);
        /* 3.栈顶更新为兄弟节点 */
        Stack[Size - 1] = Sibling;
        /* 4.旋转兄弟节点与父节点 */
        RBT_RotateNode(Stack, Size);
        
        /* 需要删除节点,断开关联 */
        if (Node == Null)
            RBT_NodeSetChild(Parent, NULL, Side);
        
        return;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 插入函数 */
static void RBT_InsertNode_All(RBT_Tree *Tree, RBT_Node *Node)
{
    ERROR_PRINT(Tree == NULL,             "RBT_InsertNode_All: Tree");
    ERROR_PRINT(Node == NULL,             "RBT_InsertNode_All: Node");
    ERROR_PRINT(Tree->KeyCompare == NULL, "RBT_InsertNode_All: KeyCompare");

    /* 辅助初始化节点(可能需要) */
    RBT_NodeSetChild(Node, NULL, LEFT);
    RBT_NodeSetChild(Node, NULL, RIGHT);

    /* 第一次插入 */
    if (RBT_TreeGetRoot(Tree) == NULL) {
        RBT_TreeSetRoot(Tree, Node);
        RBT_TreeSetMaxDepth(Tree, 1);
        RBT_NodeSetColor(Node, BLACK);
        return;
    } 

    /* 1.获得其对应的栈 */
    RBT_Node **Stack = RBT_TreeGetStack(Tree);
    /* 2.获取插入的枝干 */
    int32_t Size = -1;
    RBT_Stack_Prepare(Tree, Node, Stack, &Size, 0);
    /* 3.获得父节点 */
    RBT_Node *Parent = Stack[Size - 1];
    /* 4.获得父子节点的关联 */
    RBT_Side Side = ERROR;
    
    if (RBT_TreeCompare(Tree, Node, Parent) != 0)
        Side = LEFT;
    if (RBT_TreeCompare(Tree, Node, Parent) == 0)
        Side = RIGHT;
    /* 5.建立亲子关系 */
    RBT_NodeSetChild(Parent, Node, Side);
    /* 6.染红插入的节点 */
    RBT_NodeSetColor(Node, RED);
    /* 7.当前节点入栈 */
    Stack[Size++] = Node;
    /* 8.修复该树 */
    RBT_InsertNode_Adjust(Stack, Size);
    /* 更新树深 */
    if (Size > RBT_TreeGetMaxDepth(Tree))
        RBT_TreeSetMaxDepth(Tree, Size);
    /* 可能调整到根节点 */
    RBT_TreeSetRoot(Tree, Stack[0]);
    /* 树的根是黑色的 */
    ERROR_PRINT(RBT_NodeGetColor(Stack[0]) != BLACK, "RBT_InsertNode_All: Root");
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 删除函数 */
static void RBT_RemoveNode_All(RBT_Tree *Tree, RBT_Node *Node)
{
    ERROR_PRINT(Tree == NULL, "RBT_RemoveNode_All: Tree");
    
    /* 1.获得其对应的栈 */
    RBT_Node **Stack = RBT_TreeGetStack(Tree);
    /* 2.获取删除的枝干 */
    int32_t Size = -1;
    RBT_Stack_Prepare(Tree, Node, Stack, &Size, 1);
    /* 3.没找到要删除节点 */
    if (RBT_TreeConfirm(Tree, Node, Stack[Size - 1]) != 0)
        return;
    
    /* 实际上非侵入式的数据结构直接交换其子节点数据索引即可完成 */
    /* 这里是继承Zephyr的改写,它原是侵入式数据结构,所以保留 */
    
    /* 我们只能删除一个有0或1个子节点的节点 */
    /* 如果我们有两个,那么选择边0的“最大”子节点(最小的1也可以) */
    /* 并与它交换我们在树中的位置 */
    /* 4.获得目标节点 */
    if (RBT_NodeGetChild(Node, LEFT)  != NULL &&
        RBT_NodeGetChild(Node, RIGHT) != NULL) {
        int32_t Size0 = Size;
        /* 1.获得左孩子 */
        RBT_Node *Child = RBT_NodeGetChild(Node, LEFT);
        /* 2.保存当前节点父亲 */
        RBT_Node *HighParent = Size > 1 ? Stack[Size - 2] : NULL;
        /* 3.左孩子入栈 */
        Stack[Size++] = Child;
        /* 4.迭代寻找其右孩子 */
        while (RBT_NodeGetChild(Child, RIGHT) != NULL) {
            Child = RBT_NodeGetChild(Child, RIGHT);
            Stack[Size++] = Child;
        }
        /* 5.获得要交换的孩子的父亲 */
        RBT_Node *LowParent = Stack[Size - 2];

        /* 这里有一些特殊情况需要检查 */
        /* 原则上,这是通过在节点之间交换子指针 */
        /* 并将指向它们的节点从它们的父节点重新定位来实现的,但是: */
        /* (1)上面的节点可能是树的根,没有父节点 */
        /* (2)下面的节点可能是上面节点的直接子节点 */
        /* 记得交换两个节点的颜色位 */
        /* 且没有父指针,所以跟踪这个结构的堆栈也需要交换! */
        
        if (HighParent == NULL)
            RBT_TreeSetRoot(Tree, Child);
        if (HighParent != NULL) {
            RBT_Side Side = RBT_NodeGetSide(Node, HighParent);
            ERROR_PRINT(Side == ERROR, "RBT_RemoveNode_All: Side");
            /* 为高父亲设置低孩子 */
            RBT_NodeSetChild(HighParent, Child, Side);
        }
        
        /* 直属亲子关系 */
        if (LowParent == Node) {
            /* 交换亲子关系(交换它们的左孩子) */
            RBT_NodeSetChild(Node, RBT_NodeGetChild(Child, LEFT), LEFT);
            RBT_NodeSetChild(Child, Node, LEFT);
        }
        /* 跨度亲子关系 */
        if (LowParent != Node) {
            RBT_Side LowSide = RBT_NodeGetSide(Child, LowParent);
            ERROR_PRINT(LowSide == ERROR, "RBT_RemoveNode_All: LowSide");
            /* 更新低父亲指向高孩子 */
            RBT_NodeSetChild(LowParent, Node, LowSide);
            /* 交换俩个节点的左孩子(可能存在) */
            RBT_Node *Temp = RBT_NodeGetChild(Node, LEFT);
            RBT_NodeSetChild(Node, RBT_NodeGetChild(Child, LEFT), LEFT);
            RBT_NodeSetChild(Child, Temp, LEFT);
        }
    
        /* 交换俩个节点的右孩子 */
        RBT_NodeSetChild(Child, RBT_NodeGetChild(Node, RIGHT), RIGHT);
        /* 原child的右孩子一定不存在 */
        RBT_NodeSetChild(Node, NULL, RIGHT);
        /* 更新堆栈关系 */
        RBT_Node *Temp1 = Stack[Size - 1];
        RBT_Node *Temp2 = Stack[Size0 - 1];
        Stack[Size - 1] = Temp2;
        Stack[Size0 - 1] = Temp1;
        /* 交换俩个节点颜色 */
        RBT_Color Color = RBT_NodeGetColor(Node);
        RBT_NodeSetColor(Node, RBT_NodeGetColor(Child));
        RBT_NodeSetColor(Child, Color);
    }
    
    ERROR_PRINT(RBT_NodeGetChild(Node, LEFT)  != NULL &&
                RBT_NodeGetChild(Node, RIGHT) != NULL,
                "RBT_RemoveNode_All: Child");

    /* 5.获得唯一的孩子,或者没有 */
    RBT_Node *Child = (RBT_NodeGetChild(Node, LEFT) == NULL ?
                      RBT_NodeGetChild(Node, RIGHT) :
                      RBT_NodeGetChild(Node, LEFT));

    /* 6.如果是删除根节点 */
    if (Size < 2) {
        RBT_TreeSetRoot(Tree, Child);
        if (Child != NULL)
            RBT_NodeSetColor(Child, BLACK);
        if (Child == NULL)
            RBT_TreeSetMaxDepth(Tree, 0);
        return;
    }
    
    /* 7.获得要删除节点的父亲 */
    RBT_Node *Parent = Stack[Size - 2];

    /* 8.调整修复树 */
    /* 删除的节点没有孩子 */
    if (Child == NULL) {
        /* 红色的无子节点可以直接删除 */
        if (RBT_NodeGetColor(Node) == RED) {
            RBT_Side Side = RBT_NodeGetSide(Node, Parent);
            ERROR_PRINT(Side == ERROR, "RBT_RemoveNode_All: Side");
            RBT_NodeSetChild(Parent, NULL, Side);
        }
        /* 特殊情况:如果要删除的节点是没有子节点的 */
        /* 那么我们在做缺少的黑色旋转时将它留在原地 */
        /* 当它们隔离它时将用适当的NULL替换它 */
        /* 调整去除无子的黑色节点(此时节点未删除) */
        if (RBT_NodeGetColor(Node) == BLACK)
            RBT_RemoveNode_Adjust(Stack, Size, Node);
    }
    /* 删除的节点有一个孩子 */
    if (Child != NULL) {
        RBT_Side Side = RBT_NodeGetSide(Node, Parent);
        ERROR_PRINT(Side == ERROR, "RBT_RemoveNode_All: Side");
        RBT_NodeSetChild(Parent, Child, Side);/* 删除节点 */
        /* 如果该孩子可以直接染色修复 */
        if (RBT_NodeGetColor(Node)  == RED ||
            RBT_NodeGetColor(Child) == RED)
            RBT_NodeSetColor(Child, BLACK);
        else {
            /* 俩个节点都为黑色,但已经删除了node节点,让孩子节点顶替上去 */
            Stack[Size - 1] = Child;
            /* 节点已经删除,直接修复 */
            RBT_RemoveNode_Adjust(Stack, Size, NULL);
        }
    }

    /* 我们可能已经旋转到根了 */
    RBT_TreeSetRoot(Tree, Stack[0]);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 查找函数 */
static RBT_Node * RBT_SearchNode_Only(RBT_Tree *Tree, RBT_Node *Target)
{
    ERROR_PRINT(Tree == NULL,             "RBT_SearchNode_Only: Tree");
    ERROR_PRINT(Tree->KeyCompare == NULL, "RBT_SearchNode_Only: KeyCompare");
    ERROR_PRINT(Tree->KeyConfirm == NULL, "RBT_SearchNode_Only: KeyConfirm");
    
    RBT_Node *Node = RBT_TreeGetRoot(Tree);
    while (Node != NULL) {
        /* 关键字匹配成功,返回 */
        if (RBT_TreeConfirm(Tree, Target, Node) == 0)
            return Node;
        
        /* 该结点小于此本结点,到左子树去 */
        if (RBT_TreeCompare(Tree, Target, Node) != 0) {
            Node = RBT_NodeGetChild(Node, LEFT);
            continue;
        }
        
        /* 该结点大于此本结点,到右子树去 */
        if (RBT_TreeCompare(Tree, Target, Node) == 0) {
            Node = RBT_NodeGetChild(Node, RIGHT);
            continue;
        }
    }
    
    return NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 上层接口封装 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_InsertNode(RBT_Tree *Tree, RBT_Node *Node)
{    
    RETURN_EMPTY(Tree == NULL);
    RETURN_EMPTY(Node == NULL);
    
    RBT_InsertNode_All(Tree, Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_RemoveNode(RBT_Tree *Tree, RBT_Node *Node)
{
    RETURN_EMPTY(Tree == NULL);
    RETURN_EMPTY(Node == NULL);
    
    RBT_RemoveNode_All(Tree, Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
RBT_Node * RBT_SearchNode(RBT_Tree *Tree, RBT_Node *Node)
{
    return RBT_SearchNode_Only(Tree, Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 上层接口,辅助构建动作 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_ResetTree(RBT_Tree *Tree)
{
    RETURN_EMPTY(Tree == NULL);
    RBT_TreeSetRoot(Tree, NULL);
    RBT_TreeSetCompare(Tree, NULL);
    RBT_TreeSetConfirm(Tree, NULL);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_ResetNode(RBT_Node *Node)
{
    RBT_NodeSetChild(Node, NULL, LEFT);
    RBT_NodeSetChild(Node, NULL, RIGHT);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_SetTree(RBT_Tree *Tree, RBT_KeyCompare Compare, RBT_KeyConfirm Confirm)
{
    RBT_TreeSetRoot(Tree, NULL);
    RBT_TreeSetCompare(Tree, Compare);
    RBT_TreeSetConfirm(Tree, Confirm);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_SetRoot(RBT_Tree *Tree, RBT_Node *Node)
{
    RBT_TreeSetRoot(Tree, Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_GetRoot(RBT_Tree *Tree, RBT_Node **Node)
{
    RBT_TreeGetRoot(Tree);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_SetTreeStack(RBT_Tree *Tree, RBT_Node **Stack, uint32_t Length)
{
    RBT_TreeSetStack(Tree, Stack);
    RBT_TreeSetStackLength(Tree, Length);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_GetTreeStack(RBT_Tree *Tree, RBT_Node ***Stack, uint32_t *Length)
{
    *Stack  = RBT_TreeGetStack(Tree);
    *Length = RBT_TreeGetStackLength(Tree);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
uint32_t RBT_GetMaxDepth(RBT_Tree *Tree)
{
    return RBT_TreeGetMaxDepth(Tree);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef void (*RBT_Visit)(RBT_Node *Node, RBT_Color Color);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 窥探:数据的层序遍历并访问 */
void RBT_Sequence_Traversal(RBT_Tree *Tree, RBT_Visit Visit, RBT_Node **Queue, int32_t Length)
{
    /* 层序遍历(这里使用循环队列): */
    
    /* 获取插入的起始位置 */
    int32_t QueueHead = 0;
    int32_t QueueTail = 0;
    int32_t ElementNumber = 0;
    Queue[QueueTail++] = RBT_TreeGetRoot(Tree);
    ElementNumber++;
    do {
        /* 节点出队列 */
        RBT_Node *Node = Queue[QueueHead++];
        ElementNumber--;
        RETURN_EMPTY(Node == NULL);
        /* 节点访问 */
        Visit(Node, RBT_NodeGetColor(Node));
        /* 这是一个循环队列 */
        if (QueueHead >= Length)
            QueueHead = 0;
        if (QueueTail >= Length)
            QueueTail = 0; 
        
        /* 子节点入队列 */
        RBT_Node *LeftChildAndColor  = RBT_NodeGetChild(Node, LEFT);
        RBT_Node *RightChild = RBT_NodeGetChild(Node, RIGHT);
 
        /* 左孩子入队列 */
        if (LeftChildAndColor != NULL) {
            Queue[QueueTail++] = LeftChildAndColor;
            ElementNumber++;
        }
        
        /* 右孩子入队列 */
        if (RightChild != NULL) {
            Queue[QueueTail++] = RightChild;
            ElementNumber++;
        }
        
        /* 队列太小,警告 */
        ERROR_PRINT(ElementNumber >= Length, "RBT_Sequence_Traversal: overflow");
    } while (ElementNumber > 0);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 验证一棵树是否为红黑树(内部检查) */
uint8_t RBT_InternalCheck(RBT_Tree *Tree, RBT_Node **Stack, int32_t *Flags, int32_t Length)
{
    /* 验证:1根节点不是黑色 */
    if (RBT_TreeGetRoot(Tree) == NULL)
        return 1;
    if (RBT_NodeGetColor(RBT_TreeGetRoot(Tree)) == RED)
        return 0;
    
    /* 初始化flags的值 */
    for (int32_t Index = 0; Index < Length; Flags[Index++] = LEFT);
    /* 深度优先搜索,使用flags同步维护栈进动情况 */
    RBT_Node *Node       = NULL;
    RBT_Node *LeftChild  = NULL;
    RBT_Node *RightChild = NULL;
    /* 从根节点开始从左进动 */
    /* 入栈节点只有当左右孩子都使用完毕后才退栈 */
    int32_t MaxLength = 0;/* 一条从根到叶子的最大度 */
    int32_t CurrentBlackLength = 0;
    int32_t StackTop = 0;/* 栈顶位置 */
    
    /* 1.根节点入栈 */
    Stack[StackTop++] = RBT_TreeGetRoot(Tree);
    CurrentBlackLength++;
    Flags[StackTop - 1] = LEFT;
    
    do {
        /* 2.获得栈顶元素的左右孩子 */
        Node       = Stack[StackTop - 1];
        LeftChild  = RBT_NodeGetChild(Node, LEFT);
        RightChild = RBT_NodeGetChild(Node, RIGHT);
        
        /* 验证:2红色节点不相连 */
        if (LeftChild != NULL)
            if (RBT_NodeGetColor(Node) == RED && RBT_NodeGetColor(LeftChild) == RED)
                    return 0;
                    
        if (RightChild != NULL)
            if (RBT_NodeGetColor(Node) == RED && RBT_NodeGetColor(RightChild) == RED)
                    return 0;
        
        /* 3.左孩子未曾进过栈 */
        if (Flags[StackTop - 1] == LEFT) {
            /* 3.1.标记左孩子进过栈,下一次该右孩子进了 */
            Flags[StackTop - 1] = RIGHT;
            /* 左孩子存在,可以进栈 */
            if(LeftChild != NULL) {
                Stack[StackTop++] = LeftChild;
                /* 如果左孩子是黑色的,计算其度 */
                if (RBT_NodeGetColor(LeftChild) == BLACK)
                    CurrentBlackLength++;
                Flags[StackTop - 1] = LEFT;/* 左孩子的左孩子未曾进栈 */
                continue;
            } else {
                /* 当前节点不存在左孩子 */
                /* 说明一条从根到叶子的路径产生了 */
                if (MaxLength == 0)
                    MaxLength = CurrentBlackLength;
                /* 验证:路径黑色节点不一致 */
                if (MaxLength != CurrentBlackLength)
                    return 0;
            }
        }
        
        /* 4.右孩子未曾进过栈 */
        if (Flags[StackTop - 1] == RIGHT) {
            /* 3.2标记右孩子进过栈,下一次该退栈了 */
            Flags[StackTop - 1] = ERROR;
            /* 右孩子存在,可以进栈 */
            if(RightChild != NULL) {
                Stack[StackTop++] = RightChild;
                /* 如果右孩子是黑色的,计算其度 */
                if (RBT_NodeGetColor(RightChild) == BLACK)
                    CurrentBlackLength++;
                Flags[StackTop - 1] = LEFT;/* 右孩子的左孩子未曾进栈 */
                continue;
            } else {
                /* 当前节点不存在右孩子, 说明一条从根到叶子的路径产生了 */
                if (MaxLength == 0)
                    MaxLength = CurrentBlackLength;
                /* 验证:路径黑色节点不一致 */
                if (MaxLength != CurrentBlackLength)
                    return 0;
            }
        }
        
        /* 5.都进栈了,当前节点该退栈了 */
        if (Flags[StackTop - 1] == ERROR) {
            if (RBT_NodeGetColor(Node) == BLACK)
                    CurrentBlackLength--; 
            StackTop--;
        }
        
        /* 堆栈太小,警告 */
        ERROR_PRINT(StackTop >= Length, "RBT_InternalCheck: Overflow");
    } while (StackTop > 0);
    
    /* 6.退栈完毕了,验证完成 */
    return 1;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
