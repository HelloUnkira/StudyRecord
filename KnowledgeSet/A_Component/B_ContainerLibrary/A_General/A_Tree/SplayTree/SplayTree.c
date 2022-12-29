/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/* 使用堆栈维护伸展树可以去除Parent指针 */
/* 将空间占用进一步其紧缩到双向链表式的,这里不使用 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 伸展树容器(节点) */
typedef struct SplayTree_Node {
    struct SplayTree_Node *Parent;      /* 节点父亲 */
    struct SplayTree_Node *LeftChild;   /* 节点左孩子 */
    struct SplayTree_Node *RightChild;  /* 节点右孩子 */
} ST_Node;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 伸展树集合(树根) */
typedef struct SplayTree_Tree {
    ST_Node  *Root;
    uint8_t (*KeyCompare)(ST_Node *ThisNode, ST_Node *ThatNode);
    uint8_t (*KeyConfirm)(ST_Node *ThisNode, ST_Node *ThatNode);
} ST_Tree;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* ThisNode严格小于ThatNode返回非零值 */
typedef uint8_t (*ST_KeyCompare)(ST_Node *ThisNode, ST_Node *ThatNode);
/* ThisNode与ThatNode的关键字一致返回0 */
typedef uint8_t (*ST_KeyConfirm)(ST_Node *ThisNode, ST_Node *ThatNode);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef enum {LEFT  = 0, RIGHT = 1} ST_Side;        /* 节点关系 */
typedef enum {ERROR = 2}            ST_Error;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 伸展树集合抽象方法 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void ST_TreeSetRoot(ST_Tree *Tree, ST_Node *Root)
{
    Tree->Root = Root;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void ST_TreeSetCompare(ST_Tree *Tree, ST_KeyCompare Compare)
{
    Tree->KeyCompare = Compare;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void ST_TreeSetComfirm(ST_Tree *Tree, ST_KeyConfirm Comfirm)
{
    Tree->KeyConfirm = Comfirm;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline ST_Node * ST_TreeGetRoot(ST_Tree *Tree)
{
    return Tree->Root;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint8_t ST_TreeCompare(ST_Tree *Tree, ST_Node *Node1, ST_Node *Node2)
{
    return Tree->KeyCompare(Node1, Node2);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint8_t ST_TreeComfirm(ST_Tree *Tree, ST_Node *Node1, ST_Node *Node2)
{
    return Tree->KeyConfirm(Node1, Node2);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 伸展树容器抽象方法(设置) */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void ST_NodeSetParent(ST_Node *Node, ST_Node *Parent)
{
    Node->Parent = Parent;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void ST_NodeSetChild(ST_Node *Node, ST_Node *Child, ST_Side Side)
{
    if (Side == LEFT)
        Node->LeftChild  = Child;
    if (Side == RIGHT)
        Node->RightChild = Child;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 伸展树容器抽象方法(获取) */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline ST_Node * ST_NodeGetParent(ST_Node *Node)
{
    return Node->Parent;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline ST_Node * ST_NodeGetChild(ST_Node *Node, ST_Side Side)
{
    if (Side == LEFT)
        return Node->LeftChild;
    if (Side == RIGHT)
        return Node->RightChild;
    return NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline ST_Side ST_NodeGetSide(ST_Node *Node, ST_Node *Parent)
{
    if (Node == Parent->LeftChild)
        return LEFT;
    if (Node == Parent->RightChild)
        return RIGHT;
    return ERROR;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline ST_Side ST_NodeGetOtherSide(ST_Side Side)
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
#define RETURN_EMPTY(Target) if (Target) return;
#define RETURN_NULL(Target)  if (Target) return NULL;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 伸展树核心动作:交换(左右旋转) */
static void ST_RotateNode(ST_Tree *Tree, ST_Node *Child, ST_Node *Parent)
{
    ERROR_PRINT(Tree    == NULL, "ST_RotateNode: Tree");
    ERROR_PRINT(Child   == NULL, "ST_RotateNode: Child");
    ERROR_PRINT(Parent  == NULL, "ST_RotateNode: Parent");
    /* 1.获得父子节点的对位关系,父亲与对顶位孙子的对位关系 */
    ST_Side Side       = ST_NodeGetSide(Child, Parent);
    ST_Side OtherSide  = ST_NodeGetOtherSide(Side);
    /* 2.获得祖父节点,获得父亲对顶位的孙子节点 */
    ST_Node *Ancestor  = ST_NodeGetParent(Parent);
    ST_Node *Son       = ST_NodeGetChild(Child, OtherSide);
    /* 3.原父亲获得新孩子,孙子获得新父亲 */
    ST_NodeSetChild(Parent, Son, Side);
    if (Son != NULL)
        ST_NodeSetParent(Son, Parent);
    /* 4.交换父子节点位置 */
    ST_NodeSetChild(Child, Parent, OtherSide);
    ST_NodeSetParent(Child, Ancestor);
    ST_NodeSetParent(Parent, Child);
    /* 5.更新祖父节点 */
    if (Ancestor == NULL)
        ST_TreeSetRoot(Tree, Child);
    if (Ancestor != NULL) {
        Side = ST_NodeGetSide(Parent, Ancestor);
        ST_NodeSetChild(Ancestor, Child, Side);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 伸展函数 */
static void ST_SplayNode(ST_Tree *Tree, ST_Node *Node)
{
    RETURN_EMPTY(Node == NULL);
    /* 不断地将其旋转到根即可,这满足伸展树的六种语义 */
    /* (Zig,Zag,ZigZig,ZagZag,ZigZag,ZagZig) */
    while (ST_NodeGetParent(Node) != NULL)
        ST_RotateNode(Tree, Node, ST_NodeGetParent(Node));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 搜索函数(大小元) */
static ST_Node * ST_Search_MinOrMax(ST_Node *Node, ST_Side Side)
{
    ERROR_PRINT(Node == NULL,  "ST_Search_MinOrMax: Node");
    ERROR_PRINT(Side == ERROR, "ST_Search_MinOrMax: Side");
    
    while (Node != NULL && ST_NodeGetChild(Node, Side) != NULL)
        Node = ST_NodeGetChild(Node, Side);
    
    return Node;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 搜索函数(前驱和后继) */
ST_Node * ST_Search_PrevOrNext(ST_Node *Node, ST_Side Side)
{
    ERROR_PRINT(Node == NULL,  "ST_Search_PrevOrNext: Node");
    ERROR_PRINT(Side == ERROR, "ST_Search_PrevOrNext: Side");
    
    ST_Node *Child      = ST_NodeGetChild(Node, Side);
    ST_Node *Parent     = ST_NodeGetParent(Node);
    ST_Side  OtherSide  = ST_NodeGetOtherSide(Side);
    /* 先找孩子最大项 */
    if (Child != NULL)
        return ST_Search_MinOrMax(Child, OtherSide);
    /* 节点的孩子不存在,找父亲 */
    while (Parent != NULL) {
        /* 节点的孩子不存在,父亲与对应的孩子在同一边 */
        if (ST_NodeGetSide(Node, Parent) == OtherSide)
            return Parent;
        /* 父亲与对应孩子不在同一边,找递归项到第一个相遇的同边 */
        Node    = Parent;
        Parent  = ST_NodeGetParent(Parent);
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
static ST_Node * ST_SearchNode_Only(ST_Tree *Tree, ST_Node *Target)
{
    ERROR_PRINT(Tree == NULL,             "ST_SearchNode_Only: Tree");
    ERROR_PRINT(Tree->KeyCompare == NULL, "ST_SearchNode_Only: KeyCompare");
    ERROR_PRINT(Tree->KeyConfirm == NULL, "ST_SearchNode_Only: KeyConfirm");
    ERROR_PRINT(Target == NULL,           "ST_SearchNode_Only: Target");
    
    ST_Node *Node = ST_TreeGetRoot(Tree);
    while (Node != NULL) {
        /* 关键字匹配成功,返回 */
        if (ST_TreeComfirm(Tree, Target, Node) == 0)
            return Node;
        /* 该结点小于此本结点,到左子树去 */
        if (ST_TreeCompare(Tree, Target, Node) != 0) {
            Node = ST_NodeGetChild(Node, LEFT);
            continue;
        }
        /* 该结点大于此本结点,到右子树去 */
        if (ST_TreeCompare(Tree, Target, Node) == 0) {
            Node = ST_NodeGetChild(Node, RIGHT);
            continue;
        }
    }
    return NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 单次插入函数 */
static void ST_InsertNode_Only(ST_Tree *Tree, ST_Node *Node)
{
    ERROR_PRINT(Tree == NULL,             "ST_InsertNode_Only: Tree");
    ERROR_PRINT(Node == NULL,             "ST_InsertNode_Only: Node");
    ERROR_PRINT(Tree->KeyCompare == NULL, "ST_InsertNode_Only: KeyCompare");
    
    /* 1.初始化要插入的节点 */
    ST_NodeSetParent(Node, NULL);
    ST_NodeSetChild(Node,  NULL, LEFT);
    ST_NodeSetChild(Node,  NULL, RIGHT);
    
    /* 2.如果是插入到根节点 */
    if (ST_TreeGetRoot(Tree) == NULL) {
        ST_TreeSetRoot(Tree, Node);
        return;
    }
    
    ST_Node *InsertNode = ST_TreeGetRoot(Tree);
    ST_Node *InsertLeft  = NULL;
    ST_Node *InsertRight = NULL;
    /* 3.从根节点开始寻找插入的起始位置并插入 */
    while (InsertNode != NULL) {
        /* 左子树为空且该结点小于此本结点,表示结点可以插到其左子树 */
        if (ST_TreeCompare(Tree, Node, InsertNode) != 0) {
            /* 获得其左孩子 */
            InsertLeft = ST_NodeGetChild(InsertNode, LEFT);
            /* 左孩子不为空继续迭代,左孩子为空插入 */
            if (InsertLeft != NULL) {
                InsertNode  = InsertLeft;
                continue; 
            }
            ST_NodeSetChild(InsertNode, Node, LEFT);
            ST_NodeSetParent(Node, InsertNode);
            return;
        }
        /* 右子树为空且该节点不小于此本节点,表示结点可以插到其右子树 */
        if (ST_TreeCompare(Tree, Node, InsertNode) == 0) {
            /* 获得其右孩子 */
            InsertRight = ST_NodeGetChild(InsertNode, RIGHT);
            /* 右孩子不为空继续迭代,右孩子为空插入 */
            if (InsertRight != NULL) {
                InsertNode   = InsertRight;
                continue;
            }
            ST_NodeSetChild(InsertNode, Node, RIGHT);
            ST_NodeSetParent(Node, InsertNode);
            return;
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 单次删除函数 */
static void ST_RemoveNode_Only(ST_Tree *Tree, ST_Node *Node)
{
    ERROR_PRINT(Tree == NULL, "ST_RemoveNode_Only: Tree");
    ERROR_PRINT(Node == NULL, "ST_RemoveNode_Only: Node");
    /* 找不到要删除的结点，就return */
    RETURN_EMPTY(Node == NULL);
    /* 1.获得该节点的左右孩子 */
    ST_Node *LeftChild  = ST_NodeGetChild(Node, LEFT);
    ST_Node *RightChild = ST_NodeGetChild(Node, RIGHT);
    /* 情况1:该结点是叶子结点,删除该结点 */
    /* 情况2:该结点仅有一个孩子,删除该节点 */
    /* 情况3:该结点有俩个孩子,找一个(前驱或后继)节点替换它 */
    if (LeftChild != NULL && RightChild != NULL) {
        /* 1.获取其前驱和后继(这里使用最大前驱,也可以使用最小后继,反一下即可) */
        ST_Node *Prev = ST_Search_MinOrMax(LeftChild, RIGHT);
        ST_Node *Next = ST_Search_MinOrMax(RightChild, LEFT);
        /* 2.更新需要交换的俩个父亲的孩子信息 */
        ST_Node *PrevParent = ST_NodeGetParent(Prev);
        ST_Node *NodeParent = ST_NodeGetParent(Node);
        /* 这里有一些特殊情况需要检查,原则上,这是通过在节点之间交换子指针 */
        /* 并将指向它们的节点从它们的父节点重新定位来实现的,但是: */
        /* (1)上面的节点可能是树的根,没有父节点 */
        if (NodeParent == NULL)
            ST_TreeSetRoot(Tree, Prev);
        if (NodeParent != NULL)
            ST_NodeSetChild(NodeParent, Prev, ST_NodeGetSide(Node, NodeParent));
        /* (2)下面的节点可能是上面节点的直接子节点 */
        if (PrevParent == Node) {
            /* 交换俩个节点的左孩子 */
            ST_Node *NodeLeftChild = Prev;
            ST_Node *PrevLeftChild = ST_NodeGetChild(Prev, LEFT);
            ST_NodeSetChild(Node, PrevLeftChild, LEFT);
            ST_NodeSetChild(Prev, Node, LEFT);
            /* 左孩子父亲更新 */
            if (PrevLeftChild != NULL)
                ST_NodeSetParent(PrevLeftChild, Node);
            /* 交换俩个节点的父亲 */
            ST_NodeSetParent(Prev, NodeParent);
            ST_NodeSetParent(Node, Prev);
        }
        if (PrevParent != Node) {
            /* 更新低父亲指向高孩子 */
            ST_NodeSetChild(PrevParent, Node, ST_NodeGetSide(Prev, PrevParent));
            /* 交换俩个节点的左孩子 */
            ST_Node *NodeLeftChild = ST_NodeGetChild(Node, LEFT);
            ST_Node *PrevLeftChild = ST_NodeGetChild(Prev, LEFT);
            ST_NodeSetChild(Node, PrevLeftChild, LEFT);
            ST_NodeSetChild(Prev, NodeLeftChild, LEFT);
            /* 左孩子父亲更新 */
            if (NodeLeftChild != NULL)
                ST_NodeSetParent(NodeLeftChild, Prev);
            if (PrevLeftChild != NULL)
                ST_NodeSetParent(PrevLeftChild, Node);
            /* 交换俩个节点的父亲 */
            ST_NodeSetParent(Prev, NodeParent);
            ST_NodeSetParent(Node, PrevParent);
        }
        /* 交换俩个节点的右孩子 */
        ST_Node *NodeRightChild = ST_NodeGetChild(Node, RIGHT);
        ST_Node *PrevRightChild = NULL;
        ST_NodeSetChild(Prev, NodeRightChild, RIGHT);
        ST_NodeSetChild(Node, PrevRightChild, RIGHT);
        /* 右孩子父亲更新 */
        if (NodeRightChild != NULL)
            ST_NodeSetParent(NodeRightChild, Prev);
        /* 3.此时被删除元素变成其后继结点 */
        LeftChild  = ST_NodeGetChild(Node, LEFT);
        RightChild = ST_NodeGetChild(Node, RIGHT);
    }
    /* 2.经过转化,变成获得可能存在的一个孩子 */
    ST_Node *TargetChild = (LeftChild == NULL ? RightChild : LeftChild);
    /* 3.解除目标target在原集合的关联 */
    ST_Node *Parent = ST_NodeGetParent(Node);
    /* 4.如果是要删除根节点 */
    if (Parent == NULL) {
        ST_TreeSetRoot(Tree, TargetChild);
        return;
    }
    /* 5.父亲获得新孩子 */
    ST_Side Side = ST_NodeGetSide(Node, Parent);
    ST_NodeSetChild(Parent, TargetChild, Side);
    /* 6.为孩子设置新父亲 */
    if (TargetChild != NULL)
        ST_NodeSetParent(TargetChild, Parent);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 对外接口封装 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void ST_InsertNode(ST_Tree *Tree, ST_Node *Node)
{
    RETURN_EMPTY(Tree == NULL);
    RETURN_EMPTY(Node == NULL);
    
    ST_InsertNode_Only(Tree, Node);
    ST_SplayNode(Tree, Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void ST_RemoveNode(ST_Tree *Tree, ST_Node *Node)
{
    RETURN_EMPTY(Tree == NULL);
    RETURN_EMPTY(Node == NULL);
    
    ST_RemoveNode_Only(Tree, Node);
    /* 注意:移除时是要伸展其父亲而不是自己 */
    Node = ST_NodeGetParent(Node);
    ST_SplayNode(Tree, Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
ST_Node * ST_SearchNode(ST_Tree *Tree, ST_Node *Node)
{
    RETURN_NULL(Tree == NULL);
    RETURN_NULL(Node == NULL);
    
    ST_Node *Target = ST_SearchNode_Only(Tree, Node);
    ST_SplayNode(Tree, Target);
    return Target;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
ST_Node * ST_SearchPrev(ST_Tree *Tree, ST_Node *Node)
{
    RETURN_NULL(Node == NULL);
    ST_Node * Target = ST_Search_PrevOrNext(Node, LEFT);
    ST_SplayNode(Tree, Target);
    return Target;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
ST_Node * ST_SearchNext(ST_Tree *Tree, ST_Node *Node)
{
    RETURN_NULL(Node == NULL);
    ST_Node * Target = ST_Search_PrevOrNext(Node, RIGHT);
    ST_SplayNode(Tree, Target);
    return Target;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
ST_Node * ST_SearchMin(ST_Tree *Tree)
{
    RETURN_NULL(Tree == NULL);
    ST_Node *Root = ST_TreeGetRoot(Tree);
    ST_Node * Target = ST_Search_MinOrMax(Root, LEFT);
    ST_SplayNode(Tree, Target);
    return Target;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
ST_Node * ST_SearchMax(ST_Tree *Tree)
{
    RETURN_NULL(Tree == NULL);
    ST_Node *Root = ST_TreeGetRoot(Tree);
    ST_Node * Target = ST_Search_MinOrMax(Root, RIGHT);
    ST_SplayNode(Tree, Target);
    return Target;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void ST_JoinTree(ST_Tree *Tree, ST_Tree *Tree1, ST_Tree *Tree2)
{
    RETURN_NULL(Tree == NULL);
    RETURN_NULL(Node == NULL);
    RETURN_NULL(Tree1 == NULL);
    RETURN_NULL(Tree2 == NULL);
    
    ST_Node *NewRoot = ST_SearchMax(Tree1);
    ST_SplayNode(Tree1, NewRoot);
    ST_NodeSetChild(NewRoot, ST_TreeGetRoot(Tree2), RIGHT);
    ST_TreeSetRoot(Tree,  NewRoot);
    ST_TreeSetRoot(Tree1, NULL);
    ST_TreeSetRoot(Tree2, NULL);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void ST_SplitTree(ST_Tree *Tree, ST_Node *Node, ST_Tree *Tree1, ST_Tree *Tree2)
{
    RETURN_NULL(Tree == NULL);
    RETURN_NULL(Node == NULL);
    RETURN_NULL(Tree1 == NULL);
    RETURN_NULL(Tree2 == NULL);
    
    ST_Node *NewRoot1 = ST_SearchNode_Only(Tree, Node);
    ST_SplayNode(Tree, NewRoot1);
    ST_Node *NewRoot2 = ST_NodeGetChild(NewRoot1, RIGHT);
    ST_NodeSetChild(NewRoot1, NULL, RIGHT);
    ST_TreeSetRoot(Tree, NULL);
    ST_TreeSetRoot(Tree1, NewRoot1);
    ST_TreeSetRoot(Tree2, NewRoot2);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 上层接口,辅助构建动作 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void ST_ResetTree(ST_Tree *Tree)
{
    RETURN_EMPTY(Tree == NULL);
    ST_TreeSetRoot(Tree, NULL);
    ST_TreeSetCompare(Tree, NULL);
    ST_TreeSetComfirm(Tree, NULL);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void ST_ResetNode(ST_Node *Node)
{
    ST_NodeSetParent(Node, NULL);
    ST_NodeSetChild(Node, NULL, LEFT);
    ST_NodeSetChild(Node, NULL, RIGHT);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void ST_SetTree(ST_Tree *Tree, ST_KeyCompare Compare, ST_KeyConfirm Confirm)
{
    RETURN_EMPTY(Tree == NULL);
    ST_TreeSetRoot(Tree, NULL);
    ST_TreeSetCompare(Tree, Compare);
    ST_TreeSetComfirm(Tree, Confirm);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void ST_SetRoot(ST_Tree *Tree, ST_Node *Node)
{
    RETURN_EMPTY(Tree == NULL);
    RETURN_EMPTY(Node == NULL);
    ST_TreeSetRoot(Tree, Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void ST_GetRoot(ST_Tree *Tree, ST_Node **Node)
{
    *Node = ST_TreeGetRoot(Tree);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef void (*ST_Print)(ST_Node *Node);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 窥探:数据的层序遍历并打印 */
void ST_Sequence_Traversal(ST_Tree *Tree, ST_Print Printf, ST_Node **Queue, int32_t Length)
{
    /* 层序遍历(这里使用循环队列): */
    
    /* 获取插入的起始位置 */
    int32_t QueueHead = 0;
    int32_t QueueTail = 0;
    int32_t ElementNumber = 0;
    Queue[QueueTail++] = ST_TreeGetRoot(Tree);
    ElementNumber++;
    do {
        /* 节点出队列 */
        ST_Node *Node = Queue[QueueHead++];
        ElementNumber--;
        RETURN_EMPTY(Node == NULL);
        /* 节点打印 */
        Printf(Node);
        /* 这是一个循环队列 */
        if (QueueHead >= Length) QueueHead = 0;
        if (QueueTail >= Length) QueueTail = 0; 
        
        /* 子节点入队列 */
        ST_Node *LeftChild  = ST_NodeGetChild(Node, LEFT);
        ST_Node *RightChild = ST_NodeGetChild(Node, RIGHT);
 
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
        ERROR_PRINT(ElementNumber >= Length, "ST_Sequence_Traversal: Overflow");
    } while (ElementNumber > 0);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
