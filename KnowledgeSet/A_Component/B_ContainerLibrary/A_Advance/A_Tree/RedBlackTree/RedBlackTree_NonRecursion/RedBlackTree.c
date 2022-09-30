#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if 0
#include <stdio.h>
#define ERROR_PRINT(Condition, String)  if (Condition) printf("%s\n",(String));
#else
#define ERROR_PRINT(Condition, String)
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 将实现提交隐藏在本地(向上层不透明) */
/* 使用堆栈维护红黑树可以去除Parent指针,然后将颜色值挂载到其他俩个孩子之一 */
/* 将空间占用进一步其紧缩到双向链表式的,这里不使用 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 红黑树容器(节点) */
typedef struct RedBlackTree_Node {
    struct RedBlackTree_Node *ParentAndColor;   /* 节点父亲(以及颜色) */
    struct RedBlackTree_Node *LeftChild;        /* 节点左孩子 */
    struct RedBlackTree_Node *RightChild;       /* 节点右孩子 */
} RBT_Node;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 红黑树集合(树根) */
typedef struct RedBlackTree_Tree {
    RBT_Node *Root;
    uint8_t (*KeyCompare)(RBT_Node *ThisNode, RBT_Node *ThatNode);
    uint8_t (*KeyConfirm)(RBT_Node *ThisNode, RBT_Node *ThatNode);
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
typedef enum {RBT_RED   = 0, RBT_BLACK = 1} RBT_Color;      /* 节点颜色 */
typedef enum {RBT_LEFT  = 0, RBT_RIGHT = 1} RBT_Side;       /* 节点关系 */
typedef enum {RBT_ERROR = 2}                RBT_Error;
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
static inline void RBT_TreeSetComfirm(RBT_Tree *Tree, RBT_KeyConfirm Comfirm)
{
    Tree->KeyConfirm = Comfirm;
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
static inline uint8_t RBT_TreeComfirm(RBT_Tree *Tree, RBT_Node *Node1, RBT_Node *Node2)
{
    return Tree->KeyConfirm(Node1, Node2);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 红黑树容器抽象方法(设置) */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void RBT_NodeSetColor(RBT_Node *Node, RBT_Color Color)
{
    uintptr_t Result = 0;
    
    Result |= (uintptr_t)(Node->ParentAndColor);
    Result &= (uintptr_t)(~1);
    Result |= (uintptr_t)(Color & 1);
    
    Node->ParentAndColor = (RBT_Node *)(Result);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void RBT_NodeSetParent(RBT_Node *Node, RBT_Node *Parent)
{
    uintptr_t Result = 0;
    
    Result |= (uintptr_t)(Node->ParentAndColor);
    Result &= (uintptr_t)(1);
    Result |= (uintptr_t)(Parent);
    
    Node->ParentAndColor = (RBT_Node *)(Result);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void RBT_NodeSetChild(RBT_Node *Node, RBT_Node *Child, RBT_Side Side)
{
    if (Side == RBT_LEFT)
        Node->LeftChild  = Child;
    if (Side == RBT_RIGHT)
        Node->RightChild = Child;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 红黑树容器抽象方法(获取) */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline RBT_Color RBT_NodeGetColor(RBT_Node *Node)
{
    uintptr_t Result = 0;
    
    Result |= (uintptr_t)(Node->ParentAndColor);
    Result &= (uintptr_t)(1);
    
    return (RBT_Color)Result;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline RBT_Node * RBT_NodeGetParent(RBT_Node *Node)
{
    uintptr_t Result = 0;
    
    Result |= (uintptr_t)(Node->ParentAndColor);
    Result &= (uintptr_t)(~1);
    
    return (RBT_Node *)Result;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline RBT_Node * RBT_NodeGetChild(RBT_Node *Node, RBT_Side Side)
{
    if (Side == RBT_LEFT)
        return Node->LeftChild;
    if (Side == RBT_RIGHT)
        return Node->RightChild;
    return NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline RBT_Side RBT_NodeGetSide(RBT_Node *Node, RBT_Node *Parent)
{
    if (Node == Parent->LeftChild)
        return RBT_LEFT;
    if (Node == Parent->RightChild)
        return RBT_RIGHT;
    return RBT_ERROR;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline RBT_Side RBT_NodeGetOtherSide(RBT_Side Side)
{
    if (Side == RBT_LEFT)
        return RBT_RIGHT;
    if (Side == RBT_RIGHT)
        return RBT_LEFT;
    return RBT_ERROR;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 辅助定制宏,美化代码,剔除边缘逻辑 */
#define RETURN_EMPTY(Target) if (Target) return;
#define RETURN_NULL(Target)  if (Target) return NULL;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 红黑树核心动作:交换(左右旋转) */
static void RBT_RotateNode(RBT_Tree *Tree, RBT_Node *Child, RBT_Node *Parent)
{
    ERROR_PRINT(Tree    == NULL, "RBT_RotateNode: Tree");
    ERROR_PRINT(Child   == NULL, "RBT_RotateNode: Child");
    ERROR_PRINT(Parent  == NULL, "RBT_RotateNode: Parent");
    /* 1.获得父子节点的对位关系,父亲与对顶位孙子的对位关系 */
    RBT_Side Side       = RBT_NodeGetSide(Child, Parent);
    RBT_Side OtherSide  = RBT_NodeGetOtherSide(Side);
    /* 2.获得祖父节点,获得父亲对顶位的孙子节点 */
    RBT_Node *Ancestor  = RBT_NodeGetParent(Parent);
    RBT_Node *Son       = RBT_NodeGetChild(Child, OtherSide);
    /* 3.原父亲获得新孩子,孙子获得新父亲 */
    RBT_NodeSetChild(Parent, Son, Side);
    if (Son != NULL)
        RBT_NodeSetParent(Son, Parent);
    /* 4.交换父子节点位置 */
    RBT_NodeSetChild(Child, Parent, OtherSide);
    RBT_NodeSetParent(Child, Ancestor);
    RBT_NodeSetParent(Parent, Child);
    /* 5.更新祖父节点 */
    if (Ancestor == NULL)
        RBT_TreeSetRoot(Tree, Child);
    if (Ancestor != NULL) {
        Side = RBT_NodeGetSide(Parent, Ancestor);
        RBT_NodeSetChild(Ancestor, Child, Side);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 搜索函数(大小元) */
static RBT_Node * RBT_Search_MinOrMax(RBT_Node *Node, RBT_Side Side)
{
    ERROR_PRINT(Node == NULL,  "RBT_Search_MinOrMax: Node");
    ERROR_PRINT(Side == RBT_ERROR, "RBT_Search_MinOrMax: Side");
    
    while (Node != NULL && RBT_NodeGetChild(Node, Side) != NULL)
        Node = RBT_NodeGetChild(Node, Side);
    
    return Node;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 搜索函数(前驱和后继) */
RBT_Node * RBT_Search_PrevOrNext(RBT_Node *Node, RBT_Side Side)
{
    ERROR_PRINT(Node == NULL,  "RBT_Search_PrevOrNext: Node");
    ERROR_PRINT(Side == RBT_ERROR, "RBT_Search_PrevOrNext: Side");
    
    RBT_Node *Child      = RBT_NodeGetChild(Node, Side);
    RBT_Node *Parent     = RBT_NodeGetParent(Node);
    RBT_Side  OtherSide  = RBT_NodeGetOtherSide(Side);
    /* 先找孩子最大项 */
    if (Child != NULL)
        return RBT_Search_MinOrMax(Child, OtherSide);
    /* 节点的孩子不存在,找父亲 */
    while (Parent != NULL) {
        /* 节点的孩子不存在,父亲与对应的孩子在同一边 */
        if (RBT_NodeGetSide(Node, Parent) == OtherSide)
            return Parent;
        /* 父亲与对应孩子不在同一边,找递归项到第一个相遇的同边 */
        Node    = Parent;
        Parent  = RBT_NodeGetParent(Parent);
    }
    /* 退出可能: */
    /* 父亲一开始就不存在(根节点) */
    /* 祖父节点不存在(迭代到根节点,端点边界值没有对应的下一项) */
    return NULL;
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
    ERROR_PRINT(Target == NULL,           "RBT_SearchNode_Only: Target");
    
    RBT_Node *Node = RBT_TreeGetRoot(Tree);
    while (Node != NULL) {
        /* 关键字匹配成功,返回 */
        if (RBT_TreeComfirm(Tree, Target, Node) == 0)
            return Node;
        /* 该结点小于此本结点,到左子树去 */
        if (RBT_TreeCompare(Tree, Target, Node) != 0) {
            Node = RBT_NodeGetChild(Node, RBT_LEFT);
            continue;
        }
        /* 该结点大于此本结点,到右子树去 */
        if (RBT_TreeCompare(Tree, Target, Node) == 0) {
            Node = RBT_NodeGetChild(Node, RBT_RIGHT);
            continue;
        }
    }
    return NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 单次插入函数 */
static void RBT_InsertNode_Only(RBT_Tree *Tree, RBT_Node *Node)
{
    ERROR_PRINT(Tree == NULL,             "RBT_InsertNode_Only: Tree");
    ERROR_PRINT(Node == NULL,             "RBT_InsertNode_Only: Node");
    ERROR_PRINT(Tree->KeyCompare == NULL, "RBT_InsertNode_Only: KeyCompare");
    
    /* 1.初始化要插入的节点(插入节点都是红色的) */
    RBT_NodeSetColor(Node,  RBT_RED);
    RBT_NodeSetParent(Node, NULL);
    RBT_NodeSetChild(Node,  NULL, RBT_LEFT);
    RBT_NodeSetChild(Node,  NULL, RBT_RIGHT);
    
    /* 2.如果是插入到根节点 */
    if (RBT_TreeGetRoot(Tree) == NULL) {
        RBT_TreeSetRoot(Tree, Node);
        return;
    }
    
    RBT_Node *InsertNode = RBT_TreeGetRoot(Tree);
    RBT_Node *InsertLeft  = NULL;
    RBT_Node *InsertRight = NULL;
    /* 3.从根节点开始寻找插入的起始位置并插入 */
    while (InsertNode != NULL) {
        /* 左子树为空且该结点小于此本结点,表示结点可以插到其左子树 */
        if (RBT_TreeCompare(Tree, Node, InsertNode) != 0) {
            /* 获得其左孩子 */
            InsertLeft = RBT_NodeGetChild(InsertNode, RBT_LEFT);
            /* 左孩子不为空继续迭代,左孩子为空插入 */
            if (InsertLeft != NULL) {
                InsertNode  = InsertLeft;
                continue; 
            }
            RBT_NodeSetChild(InsertNode, Node, RBT_LEFT);
            RBT_NodeSetParent(Node, InsertNode);
            return;
        }
        /* 右子树为空且该节点不小于此本节点,表示结点可以插到其右子树 */
        if (RBT_TreeCompare(Tree, Node, InsertNode) == 0) {
            /* 获得其右孩子 */
            InsertRight = RBT_NodeGetChild(InsertNode, RBT_RIGHT);
            /* 右孩子不为空继续迭代,右孩子为空插入 */
            if (InsertRight != NULL) {
                InsertNode   = InsertRight;
                continue;
            }
            RBT_NodeSetChild(InsertNode, Node, RBT_RIGHT);
            RBT_NodeSetParent(Node, InsertNode);
            return;
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 插入调整函数 */
static void RBT_InsertNode_Adjust(RBT_Tree *Tree, RBT_Node *Node)
{
    ERROR_PRINT(Tree == NULL, "RBT_InsertNode_Adjust: Tree");
    ERROR_PRINT(Node == NULL, "RBT_InsertNode_Adjust: Node");
    
    do {
        RBT_Node *Parent = RBT_NodeGetParent(Node);
        /* 情况1:插入节点是根节点,该结点染色为黑色 */
        if (Parent == NULL) {
            RBT_NodeSetColor(Node, RBT_BLACK);
            return;
        }
        /* 情况2:插入节点父节点是黑色,什么都不做 */
        RETURN_EMPTY(RBT_NodeGetColor(Parent) == RBT_BLACK);
        /* 情况3:插入节点的父节点是红色(无需判断,一定是红节点) */
        /* 1.获得祖父节点(红色父节点一定有祖父节点) */
        RBT_Node *Ancestor  = RBT_NodeGetParent(Parent);
        RBT_Side  Side      = RBT_NodeGetSide(Parent, Ancestor);
        RBT_Side  OtherSide = RBT_NodeGetOtherSide(Side);
        /* 2.获得叔叔节点 */
        RBT_Node *Uncle = RBT_NodeGetChild(Ancestor, OtherSide);
        /* 情况3.1:同时出现双红色节点,将冲突向上调一层 */
        if (Uncle != NULL && RBT_NodeGetColor(Uncle) == RBT_RED) {
            /* 1.将父节点和叔叔节点设为黑色 */
            RBT_NodeSetColor(Parent, RBT_BLACK);
            RBT_NodeSetColor(Uncle,  RBT_BLACK);
            /* 2.将祖父节点设为红色 */
            RBT_NodeSetColor(Ancestor, RBT_RED);
            /* 3.将祖父节点设为当前节点调整 */
            Node = Ancestor;
            continue;
        }
        
        /* 情况3.2:不管叔叔节点是否存在或者为黑色,此时调整只能在内部进行 */
        RBT_Side ChildSide = RBT_NodeGetSide(Node, Parent);
        /* 情况3.2.1:如果是之字型分支,先通过旋转调整到俩个对位 */
        if ((Side == RBT_LEFT  && ChildSide == RBT_RIGHT) ||
            (Side == RBT_RIGHT && ChildSide == RBT_LEFT)) {
            /* 1.将父节点作为新的当前节点 */
            /* 2.对父节点进行旋转 */
            RBT_RotateNode(Tree, Node, Parent);
            /* 更新索引为下述情况使用 */
            ChildSide = RBT_NodeGetOtherSide(ChildSide);
            /*  */
            RBT_Node *Temp1 = Node;
            RBT_Node *Temp2 = Parent;
            Node    = Temp2;
            Parent  = Temp1;
        }

        /* 情况3.2.2:如果是对位分支,直接内部旋转 */
        if ((Side == RBT_LEFT  && ChildSide == RBT_LEFT) ||
            (Side == RBT_RIGHT && ChildSide == RBT_RIGHT)) {
            /* 1.将父节点设为黑色 */
            RBT_NodeSetColor(Parent, RBT_BLACK); 
            /* 2.将祖父节点设为红色 */
            RBT_NodeSetColor(Ancestor, RBT_RED); 
            /* 3.对祖父节点进行旋转 */
            RBT_RotateNode(Tree, Parent, Ancestor);
            /* 经过旋转调整了父节点和祖父节点的关系 */
            if (RBT_NodeGetParent(Parent) == NULL)
                RBT_TreeSetRoot(Tree, Parent);
        }
        return;
    } while (1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 单次删除函数 */
static void RBT_RemoveNode_Only(RBT_Tree *Tree, RBT_Node *Node, RBT_Side *Side)
{
    ERROR_PRINT(Tree == NULL,   "RBT_RemoveNode_Only: Tree");
    ERROR_PRINT(Node == NULL,   "RBT_RemoveNode_Only: Node");
    /* 找不到要删除的结点，就return */
    RETURN_EMPTY(Node == NULL);
    /* 1.获得该节点的左右孩子 */
    RBT_Node *LeftChild  = RBT_NodeGetChild(Node, RBT_LEFT);
    RBT_Node *RightChild = RBT_NodeGetChild(Node, RBT_RIGHT);
    /* 情况1:该结点是叶子结点,删除该结点 */
    /* 情况2:该结点仅有一个孩子,删除该节点 */
    /* 情况3:该结点有俩个孩子,找一个(前驱或后继)节点替换它 */
    if (LeftChild != NULL && RightChild != NULL) {
        /* 1.获取其前驱和后继(这里使用最大前驱,也可以使用最小后继,反一下即可) */
        RBT_Node *Prev = RBT_Search_MinOrMax(LeftChild, RBT_RIGHT);
        RBT_Node *Next = RBT_Search_MinOrMax(RightChild, RBT_LEFT);
        /* 2.更新需要交换的俩个父亲的孩子信息 */
        RBT_Node *PrevParent = RBT_NodeGetParent(Prev);
        RBT_Node *NodeParent = RBT_NodeGetParent(Node);
        /* 这里有一些特殊情况需要检查,原则上,这是通过在节点之间交换子指针 */
        /* 并将指向它们的节点从它们的父节点重新定位来实现的,但是: */
        /* (1)上面的节点可能是树的根,没有父节点 */
        if (NodeParent == NULL)
            RBT_TreeSetRoot(Tree, Prev);
        if (NodeParent != NULL)
            RBT_NodeSetChild(NodeParent, Prev, RBT_NodeGetSide(Node, NodeParent));
        /* (2)下面的节点可能是上面节点的直接子节点 */
        if (PrevParent == Node) {
            /* 交换俩个节点的左孩子 */
            RBT_Node *NodeLeftChild = Prev;
            RBT_Node *PrevLeftChild = RBT_NodeGetChild(Prev, RBT_LEFT);
            RBT_NodeSetChild(Node, PrevLeftChild, RBT_LEFT);
            RBT_NodeSetChild(Prev, Node, RBT_LEFT);
            /* 左孩子父亲更新 */
            if (PrevLeftChild != NULL)
                RBT_NodeSetParent(PrevLeftChild, Node);
            /* 交换俩个节点的父亲 */
            RBT_NodeSetParent(Prev, NodeParent);
            RBT_NodeSetParent(Node, Prev);
        }
        if (PrevParent != Node) {
            /* 更新低父亲指向高孩子 */
            RBT_NodeSetChild(PrevParent, Node, RBT_NodeGetSide(Prev, PrevParent));
            /* 交换俩个节点的左孩子 */
            RBT_Node *NodeLeftChild = RBT_NodeGetChild(Node, RBT_LEFT);
            RBT_Node *PrevLeftChild = RBT_NodeGetChild(Prev, RBT_LEFT);
            RBT_NodeSetChild(Node, PrevLeftChild, RBT_LEFT);
            RBT_NodeSetChild(Prev, NodeLeftChild, RBT_LEFT);
            /* 左孩子父亲更新 */
            if (NodeLeftChild != NULL)
                RBT_NodeSetParent(NodeLeftChild, Prev);
            if (PrevLeftChild != NULL)
                RBT_NodeSetParent(PrevLeftChild, Node);
            /* 交换俩个节点的父亲 */
            RBT_NodeSetParent(Prev, NodeParent);
            RBT_NodeSetParent(Node, PrevParent);
        }
        /* 交换俩个节点的右孩子 */
        RBT_Node *NodeRightChild = RBT_NodeGetChild(Node, RBT_RIGHT);
        RBT_Node *PrevRightChild = NULL;
        RBT_NodeSetChild(Prev, NodeRightChild, RBT_RIGHT);
        RBT_NodeSetChild(Node, PrevRightChild, RBT_RIGHT);
        /* 右孩子父亲更新 */
        if (NodeRightChild != NULL)
            RBT_NodeSetParent(NodeRightChild, Prev);
        /* 交换俩个节点颜色 */
        RBT_Color NodeColor = RBT_NodeGetColor(Node);
        RBT_Color PrevColor = RBT_NodeGetColor(Prev);
        RBT_NodeSetColor(Node, PrevColor);
        RBT_NodeSetColor(Prev, NodeColor);
        /* 3.此时被删除元素变成其后继结点 */
        LeftChild  = RBT_NodeGetChild(Node, RBT_LEFT);
        RightChild = RBT_NodeGetChild(Node, RBT_RIGHT);
    }
    /* 2.经过转化,变成获得可能存在的一个孩子 */
    RBT_Node *TargetChild = (LeftChild == NULL ? RightChild : LeftChild);
    /* 3.解除目标target在原集合的关联 */
    RBT_Node *Parent = RBT_NodeGetParent(Node);
    /* 4.如果是要删除根节点 */
    if (Parent == NULL) {
        RBT_TreeSetRoot(Tree, TargetChild);
        return;
    }
    /* 5.父亲获得新孩子 */
    *Side = RBT_NodeGetSide(Node, Parent);
    RBT_NodeSetChild(Parent, TargetChild, *Side);
    /* 6.为孩子设置新父亲 */
    if (TargetChild != NULL)
        RBT_NodeSetParent(TargetChild, Parent);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 删除调整函数 */
static void RBT_RemoveNode_Adjust(RBT_Tree *Tree, RBT_Node *Node, RBT_Side Side)
{
    ERROR_PRINT(Tree == NULL,  "RBT_RemoveNode_Adjust: Tree");
    ERROR_PRINT(Node == NULL,  "RBT_RemoveNode_Adjust: Node");
    
    /* 情况1:删除空节点,无需调整 */
    RETURN_EMPTY(Node == NULL);
    /* 1.获得删除的节点的父亲和左右孩子 */
    RBT_Color Color      = RBT_NodeGetColor(Node);
    RBT_Node *LeftChild  = RBT_NodeGetChild(Node, RBT_LEFT);
    RBT_Node *RightChild = RBT_NodeGetChild(Node, RBT_RIGHT);
    RBT_Node *OnlyOne    = (LeftChild == NULL ? RightChild : LeftChild);
    /* 情况2:删除节点为红色,无需调整 */
    RETURN_EMPTY(Color == RBT_RED);
    /* 情况3:自己为黑色但是唯一孩子为红色 */
    if (OnlyOne != NULL && RBT_NodeGetColor(OnlyOne) == RBT_RED) {
        RBT_NodeSetColor(OnlyOne, RBT_BLACK);
        return;
    }
    /* 情况4:自己是黑色,删除导致缺失了一个黑色 */
    do {
        RBT_Node *Parent      = RBT_NodeGetParent(Node);
        RBT_Side  SiblingSide = RBT_NodeGetOtherSide(Side);
        /* 情况4.1:删除的节点为根节点(或者调整到根节点),什么都不做 */
        RETURN_EMPTY(Parent == NULL);
        /* 否则,删除黑色节点导致的不平衡兄弟一定存在 */
        RBT_Node *Sibling = RBT_NodeGetChild(Parent, SiblingSide);
        /* 情况4.2:兄弟节点为红色 */
        if (RBT_NodeGetColor(Sibling) == RBT_RED) {
            /* 1.父节点染成红色,兄弟节点染成黑色 */
            RBT_NodeSetColor(Sibling, RBT_BLACK);
            RBT_NodeSetColor(Parent,  RBT_RED);
            /* 2.对父节点进行旋转 */
            RBT_RotateNode(Tree, Sibling, Parent);
            /* 更新父子兄弟关系 */
            Sibling = RBT_NodeGetChild(Parent, SiblingSide);
        }
        
        RBT_Node *SiblingLeftChild  = RBT_NodeGetChild(Sibling, RBT_LEFT);
        RBT_Node *SiblingRightChild = RBT_NodeGetChild(Sibling, RBT_RIGHT);
        
        /* 情况4.3:孩子都是黑色 */
        if ((SiblingLeftChild  == NULL || RBT_NodeGetColor(SiblingLeftChild)  == RBT_BLACK) &&
            (SiblingRightChild == NULL || RBT_NodeGetColor(SiblingRightChild) == RBT_BLACK)) {
            if (RBT_NodeGetColor(Parent) == RBT_BLACK) {
                /* 情况4.3.1:父亲是黑色 */
                /* 1.染色兄弟 */
                RBT_NodeSetColor(Sibling, RBT_RED);
                /* 2.调整到上一层 */
                Node = Parent;
                Parent = RBT_NodeGetParent(Node);
                RETURN_EMPTY(Parent == NULL);
                Side = RBT_NodeGetSide(Node, Parent);
                continue;
            } else {
                /* 情况4.3.2:父亲是红色 */
                /* 1.交换父亲兄弟的颜色 */
                RBT_NodeSetColor(Parent,  RBT_BLACK);
                RBT_NodeSetColor(Sibling, RBT_RED);
                return;
            }
        }
        
        RBT_Node *RedChild = NULL;
        /* 情况4.4:如果是之字型分支,先通过旋转调整到俩个对位 */
        /* 情况4.4.1:孩子节点是父左节点,兄弟左儿子是红色,右儿子是黑色 */
        /* 情况4.4.2:孩子节点是父右节点,兄弟右儿子是红色,左儿子是黑色 */
        if (Side == RBT_LEFT &&
           (SiblingRightChild == NULL || RBT_NodeGetColor(SiblingRightChild) == RBT_BLACK))
            RedChild = SiblingLeftChild;
        
        if (Side == RBT_RIGHT &&
           (SiblingLeftChild == NULL ||  RBT_NodeGetColor(SiblingLeftChild) == RBT_BLACK))
            RedChild = SiblingRightChild;
        
        if (RedChild != NULL) {
            /* 1.染红兄弟节点,染黑红色孩子 */
            RBT_NodeSetColor(RedChild, RBT_BLACK);
            RBT_NodeSetColor(Sibling,  RBT_RED);
            /* 2.对兄弟节点进行旋转 */
            RBT_RotateNode(Tree, RedChild, Sibling);
            /* 兄弟节点以及对应的左右孩子都更新了! */
            Sibling = RedChild;
            SiblingLeftChild  = RBT_NodeGetChild(Sibling, RBT_LEFT);
            SiblingRightChild = RBT_NodeGetChild(Sibling, RBT_RIGHT);
        }
            
        RedChild = NULL;
        /* 情况4.5:如果是对位分支,直接内部旋转 */
        /* 情况4.5.1:孩子节点是父左节点,兄弟的右孩子为红色 */
        /* 情况4.5.2:孩子节点是父右节点,兄弟的左孩子为红色 */
        if (Side == RBT_LEFT &&
           (SiblingRightChild != NULL && RBT_NodeGetColor(SiblingRightChild) == RBT_RED))
            RedChild = SiblingRightChild;
        
        if (Side == RBT_RIGHT &&
           (SiblingLeftChild != NULL && RBT_NodeGetColor(SiblingLeftChild) == RBT_RED))
            RedChild = SiblingLeftChild;
        
        if (RedChild != NULL) {
            /* 1.兄弟获得父节点的颜色 */
            RBT_NodeSetColor(Sibling, RBT_NodeGetColor(Parent));
            /* 2.父节点获得兄弟节点的颜色 */
            RBT_NodeSetColor(Parent, RBT_BLACK);
            /* 3.染黑红子节点 */
            RBT_NodeSetColor(RedChild, RBT_BLACK);
            /* 4.对父节点进行旋转 */
            RBT_RotateNode(Tree, Sibling, Parent);
        }
        
        return;
    } while (1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 对外接口封装 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_InsertNode(RBT_Tree *Tree, RBT_Node *Node)
{
    RETURN_EMPTY(Tree == NULL);
    RETURN_EMPTY(Node == NULL);
    
    RBT_InsertNode_Only(Tree, Node);
    RBT_InsertNode_Adjust(Tree, Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_RemoveNode(RBT_Tree *Tree, RBT_Node *Node)
{
    RETURN_EMPTY(Tree == NULL);
    RETURN_EMPTY(Node == NULL);
    
    RBT_Side Side = RBT_ERROR;
    RBT_RemoveNode_Only(Tree, Node,  &Side);  
    RBT_RemoveNode_Adjust(Tree, Node, Side);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
RBT_Node * RBT_SearchNode(RBT_Tree *Tree, RBT_Node *Node)
{
    RETURN_NULL(Tree == NULL);
    RETURN_NULL(Node == NULL);
    
    return RBT_SearchNode_Only(Tree, Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
RBT_Node * RBT_SearchPrev(RBT_Node *Node)
{
    RETURN_NULL(Node == NULL);
    return RBT_Search_PrevOrNext(Node, RBT_LEFT);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
RBT_Node * RBT_SearchNext(RBT_Node *Node)
{
    RETURN_NULL(Node == NULL);
    return RBT_Search_PrevOrNext(Node, RBT_RIGHT);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
RBT_Node * RBT_SearchMin(RBT_Tree *Tree)
{
    RETURN_NULL(Tree == NULL);
    RBT_Node *Root = RBT_TreeGetRoot(Tree);
    return RBT_Search_MinOrMax(Root, RBT_LEFT);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
RBT_Node * RBT_SearchMax(RBT_Tree *Tree)
{
    RETURN_NULL(Tree == NULL);
    RBT_Node *Root = RBT_TreeGetRoot(Tree);
    return RBT_Search_MinOrMax(Root, RBT_RIGHT);
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
    RBT_TreeSetComfirm(Tree, NULL);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_ResetNode(RBT_Node *Node)
{
    RBT_NodeSetParent(Node, NULL);
    RBT_NodeSetChild(Node, NULL, RBT_LEFT);
    RBT_NodeSetChild(Node, NULL, RBT_RIGHT);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_SetTree(RBT_Tree *Tree, RBT_KeyCompare Compare, RBT_KeyConfirm Confirm)
{
    RETURN_EMPTY(Tree == NULL);
    RBT_TreeSetRoot(Tree, NULL);
    RBT_TreeSetCompare(Tree, Compare);
    RBT_TreeSetComfirm(Tree, Confirm);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_SetRoot(RBT_Tree *Tree, RBT_Node *Node)
{
    RETURN_EMPTY(Tree == NULL);
    RETURN_EMPTY(Node == NULL);
    RBT_TreeSetRoot(Tree, Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void RBT_GetRoot(RBT_Tree *Tree, RBT_Node **Node)
{
    *Node = RBT_TreeGetRoot(Tree);
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
        if (QueueHead >= Length) QueueHead = 0;
        if (QueueTail >= Length) QueueTail = 0; 
        
        /* 子节点入队列 */
        RBT_Node *LeftChild  = RBT_NodeGetChild(Node, RBT_LEFT);
        RBT_Node *RightChild = RBT_NodeGetChild(Node, RBT_RIGHT);
 
        /* 左孩子入队列 */
        if (LeftChild != NULL) {
            Queue[QueueTail++] = LeftChild;
            ElementNumber++;
        }
        
        /* 右孩子入队列 */
        if (RightChild != NULL) {
            Queue[QueueTail++] = RightChild;
            ElementNumber++;
        }
        
        /* 队列太小,警告 */
        ERROR_PRINT(ElementNumber >= Length, "RBT_Sequence_Traversal: Overflow");
    } while (ElementNumber > 0);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 验证一棵树是否为红黑树(内部检查) */
uint8_t RBT_InternalCheck(RBT_Tree *Tree, RBT_Node **Stack, int32_t *Flags, int32_t Length)
{
    /* 验证:1根节点不是黑色 */
    if (RBT_TreeGetRoot(Tree) == NULL)
        return 1;
    if (RBT_NodeGetColor(RBT_TreeGetRoot(Tree)) == RBT_RED)
        return 0;
    
    /* 初始化flags的值 */
    for (int32_t Index = 0; Index < Length; Flags[Index++] = RBT_LEFT);
    /* 深度优先搜索,使用flags同步维护栈进动情况 */
    RBT_Node *Node       = NULL;
    RBT_Node *LeftChild  = NULL;
    RBT_Node *RightChild = NULL;
    /* 从根节点开始从左进动 */
    /* 入栈节点只有当左右孩子都使用完毕后才退栈 */
    int32_t MaxLength = 0;//一条从根到叶子的最大度
    int32_t CurrentBlackLength = 0;
    int32_t StackTop = 0;//栈顶位置
    
    /* 1.根节点入栈 */
    Stack[StackTop++] = RBT_TreeGetRoot(Tree);
    CurrentBlackLength++;
    Flags[StackTop - 1] = RBT_LEFT;
    
    do {
        /* 2.获得栈顶元素的左右孩子 */
        Node       = Stack[StackTop - 1];
        LeftChild  = RBT_NodeGetChild(Node, RBT_LEFT);
        RightChild = RBT_NodeGetChild(Node, RBT_RIGHT);
        
        /* 验证:2红色节点不相连 */
        if (LeftChild != NULL)
            if (RBT_NodeGetColor(Node) == RBT_RED && RBT_NodeGetColor(LeftChild) == RBT_RED)
                    return 0;
                    
        if (RightChild != NULL)
            if (RBT_NodeGetColor(Node) == RBT_RED && RBT_NodeGetColor(RightChild) == RBT_RED)
                    return 0;
        
        /* 3.左孩子未曾进过栈 */
        if (Flags[StackTop - 1] == RBT_LEFT) {
            /* 3.1.标记左孩子进过栈,下一次该右孩子进了 */
            Flags[StackTop - 1] = RBT_RIGHT;
            /* 左孩子存在,可以进栈 */
            if(LeftChild != NULL) {
                Stack[StackTop++] = LeftChild;
                /* 如果左孩子是黑色的,计算其度 */
                if (RBT_NodeGetColor(LeftChild) == RBT_BLACK)
                    CurrentBlackLength++;
                Flags[StackTop - 1] = RBT_LEFT;//左孩子的左孩子未曾进栈
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
        if (Flags[StackTop - 1] == RBT_RIGHT) {
            /* 3.2标记右孩子进过栈,下一次该退栈了 */
            Flags[StackTop - 1] = RBT_ERROR;
            /* 右孩子存在,可以进栈 */
            if(RightChild != NULL) {
                Stack[StackTop++] = RightChild;
                /* 如果右孩子是黑色的,计算其度 */
                if (RBT_NodeGetColor(RightChild) == RBT_BLACK)
                    CurrentBlackLength++;
                Flags[StackTop - 1] = RBT_LEFT;//右孩子的左孩子未曾进栈
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
        if (Flags[StackTop - 1] == RBT_ERROR) {
            if (RBT_NodeGetColor(Node) == RBT_BLACK)
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
