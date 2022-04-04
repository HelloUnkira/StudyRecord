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

/* 将实现提交隐藏在本地(向上层不透明) */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 红黑树核心(红黑树容器) */
struct RedBlackTreeContainer {
    struct RedBlackTreeContainer *ThisParent; //节点父亲
    struct RedBlackTreeContainer *LeftChild;  //节点左孩子
    struct RedBlackTreeContainer *RightChild; //节点右孩子
    void  *DataAndColor;                      //要维护的上层数据集索引(含节点)
};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 红黑树核心(红黑树集合) */
struct RedBlackTreeSet {
    struct RedBlackTreeContainer *Root;
    uint8_t (*KeyCompare)(void *ThisData, void *ThatData);
    uint8_t (*KeyComfirm)(void *ThatData, void *TargetData);
};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 核心转义 */
typedef struct RedBlackTreeContainer RB_Node;     //红黑树节点
typedef struct RedBlackTreeSet       RB_Tree;     //红黑树根
//ThisData严格小于ThatData返回非零值
typedef uint8_t (*RBT_KeyCompare)(void *ThisData, void *ThatData);
//ThisData与TargetData的关键字一致返回0
typedef uint8_t (*RBT_KeyComfirm)(void *ThatData, void *TargetData);

typedef enum {RED   = 0, BLACK = 1} RB_Color;        //节点颜色
typedef enum {LEFT  = 0, RIGHT = 1} RB_Side;         //节点关系
typedef enum {ERROR = 2}            RB_Error;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 红黑树集合抽象方法 */

STATIC_INLINE RB_Node * RBTreeGetRoot(RB_Tree *Tree)
{
    return Tree->Root;
}

STATIC_INLINE void RBTreeSetRoot(RB_Tree *Tree, RB_Node *Root)
{
    Tree->Root = Root;
}

STATIC_INLINE void RBTreeSetCompare(RB_Tree *Tree, RBT_KeyCompare Compare)
{
    Tree->KeyCompare = Compare;
}
   
STATIC_INLINE void RBTreeSetComfirm(RB_Tree *Tree, RBT_KeyComfirm Comfirm)
{
    Tree->KeyComfirm = Comfirm;
}

STATIC_INLINE uint8_t RBTreeCompare(RB_Tree *Tree, void *Data1, void *Data2)
{
    return Tree->KeyCompare(Data1, Data2);
}

STATIC_INLINE uint8_t RBTreeComfirm(RB_Tree *Tree, void *Data1, void *Data2)
{
    return Tree->KeyComfirm(Data1, Data2);
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

STATIC_INLINE RB_Node * RBNodeGetParent(RB_Node *Node)
{
    return Node->ThisParent;
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

STATIC_INLINE void RBNodeSetParent(RB_Node *Node, RB_Node *Parent)
{
    Node->ThisParent = Parent;
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
static void RBT_RotateNode(RB_Tree *Tree, RB_Node *Child, RB_Node *Parent)
{
    ERROR_PRINT(Tree    == NULL, "RBT_RotateNode: Tree");
    ERROR_PRINT(Child   == NULL, "RBT_RotateNode: Child");
    ERROR_PRINT(Parent  == NULL, "RBT_RotateNode: Parent");
    //1.获得父子节点的对位关系,父亲与对顶位孙子的对位关系
    uint8_t Side         = RBNodeGetSide(Child, Parent);
    uint8_t OtherSide    = RBNodeGetOtherSide(Side);
    ERROR_PRINT(Side == ERROR, "RBT_RotateNode: Side");
    //2.获得祖父节点,获得父亲对顶位的孙子节点
    RB_Node *Ancestor    = RBNodeGetParent(Parent);
    RB_Node *Son         = RBNodeGetChild(Child, OtherSide);
    //3.原父亲获得新孩子,孙子获得新父亲
    RBNodeSetChild(Parent, Son, Side);
    if (Son != NULL)
        RBNodeSetParent(Son, Parent);
    //4.交换父子节点位置
    RBNodeSetChild(Child, Parent, OtherSide);
    RBNodeSetParent(Child, Ancestor);
    RBNodeSetParent(Parent, Child);
    //5.更新祖父节点
    if (Ancestor == NULL)
        RBTreeSetRoot(Tree, Child);
    if (Ancestor != NULL) {
        Side = RBNodeGetSide(Parent, Ancestor);
        RBNodeSetChild(Ancestor, Child, Side);
        ERROR_PRINT(Side == ERROR, "RBT_RotateNode: Ancestor");
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 搜索函数(大小元) */
static RB_Node * RBT_Search_MinOrMax(RB_Node *Node, uint8_t Side)
{
    ERROR_PRINT(Side == ERROR, "RBT_Search_MinOrMax: Side");
    
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
/* 搜索函数(前驱和后继) */
RB_Node * RBT_Search_PrevOrNext(RB_Node *Node, uint8_t Side)
{
    ERROR_PRINT(Node == NULL,  "RBT_Search_PrevOrNext: Node");
    ERROR_PRINT(Side == ERROR, "RBT_Search_PrevOrNext: Side");
    
    RB_Node *Child      = RBNodeGetChild(Node, Side);
    RB_Node *Parent     = RBNodeGetParent(Node);
    uint8_t  OtherSide  = RBNodeGetOtherSide(Side);
    //先找孩子最大项
    if (Child != NULL)
        return RBT_Search_MinOrMax(Child, OtherSide);
    //节点的孩子不存在,找父亲
    while (Parent != NULL) {
        //节点的孩子不存在,父亲与对应的孩子在同一边
        if (RBNodeGetSide(Node, Parent) == OtherSide)
            return Parent;
        //父亲与对应孩子不在同一边,找递归项到第一个相遇的同边
        Node    = Parent;
        Parent  = RBNodeGetParent(Parent);
    }
    //退出可能:
    //父亲一开始就不存在(根节点)
    //祖父节点不存在(迭代到根节点,端点边界值没有对应的下一项)
    return NULL;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 查找函数 */
static RB_Node * RBT_SearchNode_Only(RB_Tree *Tree, void *TargetData)
{
    ERROR_PRINT(Tree == NULL,             "RBT_SearchNode_Only: Tree");
    ERROR_PRINT(Tree->KeyCompare == NULL, "RBT_SearchNode_Only: KeyCompare");
    ERROR_PRINT(Tree->KeyComfirm == NULL, "RBT_SearchNode_Only: KeyComfirm");
    
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
/* 单次插入函数 */
static void RBT_InsertNode_Only(RB_Tree *Tree, RB_Node *Node)
{
    ERROR_PRINT(Tree == NULL,             "RBT_InsertNode_Only: Tree");
    ERROR_PRINT(Node == NULL,             "RBT_InsertNode_Only: Node");
    ERROR_PRINT(Tree->KeyCompare == NULL, "RBT_InsertNode_Only: KeyCompare");
    
    //1.初始化要插入的节点(插入节点都是红色的)
    RBNodeSetRed(Node);
    RBNodeSetParent(Node, NULL);
    RBNodeSetChild(Node, NULL, LEFT);
    RBNodeSetChild(Node, NULL, RIGHT);
    
    //2.如果是插入到根节点
    if (RBTreeGetRoot(Tree) == NULL) {
        RBTreeSetRoot(Tree, Node);
        return;
    }
    
    RB_Node *InsertIndex = RBTreeGetRoot(Tree);
    RB_Node *InsertLeft  = NULL;
    RB_Node *InsertRight = NULL;
    //3.从根节点开始寻找插入的起始位置并插入
    while (InsertIndex != NULL) {
        //左子树为空且该结点小于此本结点,表示结点可以插到其左子树
        if (RBTreeCompare(Tree, 
            RBNodeGetData(Node), 
            RBNodeGetData(InsertIndex)) != 0) {
            //获得其左孩子
            InsertLeft = RBNodeGetChild(InsertIndex, LEFT);
            //左孩子不为空继续迭代,左孩子为空插入
            if (InsertLeft != NULL) {
                InsertIndex = InsertLeft;
                continue; 
            }
            RBNodeSetChild(InsertIndex, Node, LEFT);
            RBNodeSetParent(Node, InsertIndex);
            return;
        }
        //右子树为空且该节点不小于此本节点,表示结点可以插到其右子树
        if (RBTreeCompare(Tree, 
            RBNodeGetData(Node),
            RBNodeGetData(InsertIndex)) == 0) {
            //获得其右孩子
            InsertRight = RBNodeGetChild(InsertIndex, RIGHT);
            //右孩子不为空继续迭代,右孩子为空插入
            if (InsertRight != NULL) {
                InsertIndex = InsertRight;
                continue;
            }
            RBNodeSetChild(InsertIndex, Node, RIGHT);
            RBNodeSetParent(Node, InsertIndex);
            return;
        }
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 插入调整函数 */
static void RBT_InsertNode_Adjust(RB_Tree *Tree, RB_Node *Node)
{
    ERROR_PRINT(Tree == NULL, "RBT_InsertNode_Adjust: Tree");
    ERROR_PRINT(Node == NULL, "RBT_InsertNode_Adjust: Node");
    
    do {
        RB_Node *Parent = RBNodeGetParent(Node);
        //情况1:插入节点是根节点,该结点染色为黑色
        if (Parent == NULL) {
            RBNodeSetBlack(Node);
            return;
        }
        //情况2:插入节点父节点是黑色,什么都不做
        RETURN_EMPTY(RBNodeGetColor(Parent) == BLACK);
        //情况3:插入节点的父节点是红色(无需判断,一定是红节点)
        //1.获得祖父节点(红色父节点一定有祖父节点)
        RB_Node *Ancestor = RBNodeGetParent(Parent);
        ERROR_PRINT(Ancestor == NULL, "RBT_InsertNode_Adjust: Ancestor");
        uint8_t Side = RBNodeGetSide(Parent, Ancestor);
        uint8_t OtherSide = RBNodeGetOtherSide(Side);
        ERROR_PRINT(Side == ERROR, "RBT_InsertNode_Adjust: Side");
        //2.获得叔叔节点
        RB_Node *Uncle = RBNodeGetChild(Ancestor, OtherSide);
        //情况3.1:同时出现双红色节点,将冲突向上调一层
        if (Uncle != NULL && RBNodeGetColor(Uncle) == RED) {
            //1.将父节点和叔叔节点设为黑色
            RBNodeSetBlack(Parent);
            RBNodeSetBlack(Uncle);
            //2.将祖父节点设为红色
            RBNodeSetRed(Ancestor);
            //3.将祖父节点设为当前节点调整 
            Node = Ancestor;
            continue;
        }
        
        //情况3.2:不管叔叔节点是否存在或者为黑色,此时调整只能在内部进行
        uint8_t ChildSide = RBNodeGetSide(Node, Parent);
        ERROR_PRINT(ChildSide == ERROR, "RBT_InsertNode_Adjust: ChildSide");
        //情况3.2.1:如果是之字型分支,先通过旋转调整到俩个对位
        if ((Side == LEFT && ChildSide == RIGHT) ||
            (Side == RIGHT && ChildSide == LEFT)) {
            //1.将父节点作为新的当前节点
            //2.对父节点进行旋转
            RBT_RotateNode(Tree, Node, Parent);
            //更新索引为下述情况使用
            ChildSide = RBNodeGetOtherSide(ChildSide);
            //
            Node    = (RB_Node *)(((VOID_STAR_TYPE)Node) ^
                                  ((VOID_STAR_TYPE)Parent));
            Parent  = (RB_Node *)(((VOID_STAR_TYPE)Node) ^
                                  ((VOID_STAR_TYPE)Parent));
            Node    = (RB_Node *)(((VOID_STAR_TYPE)Node) ^
                                  ((VOID_STAR_TYPE)Parent));
        }

        //情况3.2.2:如果是对位分支,直接内部旋转
        if ((Side == LEFT  && ChildSide == LEFT) ||
            (Side == RIGHT && ChildSide == RIGHT)) {
            //1.将父节点设为黑色
            RBNodeSetBlack(Parent); 
            //2.将祖父节点设为红色
            RBNodeSetRed(Ancestor); 
            //3.对祖父节点进行旋转
            RBT_RotateNode(Tree, Parent, Ancestor);
            //经过旋转调整了父节点和祖父节点的关系
            if (RBNodeGetParent(Parent) == NULL)
                RBTreeSetRoot(Tree, Parent);
        }
        return;
    } while (1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 单次删除函数 */
static void RBT_RemoveNode_Only(RB_Tree *Tree, RB_Node **Node, uint8_t *Side)
{
    ERROR_PRINT(Tree == NULL, "RBT_RemoveNode_Only: Tree");
    //找不到要删除的结点，就return
    RETURN_EMPTY(*Node == NULL);
    //1.获得该节点的左右孩子
    RB_Node *LeftChild  = RBNodeGetChild(*Node, LEFT);
    RB_Node *RightChild = RBNodeGetChild(*Node, RIGHT);
    //情况1:该结点是叶子结点,删除该结点
    //情况2:该结点仅有一个孩子,删除该节点
    //情况3:该结点有俩个孩子,找一个(前驱或后继)节点替换它
    if (LeftChild != NULL && RightChild != NULL) {
        //1.获取其前驱和后继(实际只用到一个)
        RB_Node *Prev = RBT_Search_MinOrMax(LeftChild, RIGHT);
        RB_Node *Next = RBT_Search_MinOrMax(RightChild, LEFT);
        //2.交换该数与(前驱)后继结点中的数据,不交换颜色
        void *NodeDataCurrent = RBNodeGetData(*Node);
        void *NodeDataNext    = RBNodeGetData(Next);
        RBNodeSetData(*Node, NodeDataNext);
        RBNodeSetData(Next,  NodeDataCurrent);
        //3.此时被删除元素变成其后继结点
        LeftChild   = RBNodeGetChild(Next, LEFT);
        RightChild  = RBNodeGetChild(Next, RIGHT);
        *Node = Next;
    }
    //2.经过转化,变成获得可能存在的一个孩子
    RB_Node *TargetChild = (LeftChild == NULL ? RightChild : LeftChild);
    //3.解除目标target在原集合的关联
    RB_Node *Parent = RBNodeGetParent(*Node);
    *Side = RBNodeGetSide(*Node, Parent);
    ERROR_PRINT(*Side == ERROR, "RBT_RemoveNode_Only: Side");
    //4.如果是要删除根节点
    if (Parent == NULL) {
        RBTreeSetRoot(Tree, TargetChild);
        return;
    }
    //5.父亲获得新孩子
    RBNodeSetChild(Parent, TargetChild, *Side);
    //6.为孩子设置新父亲
    if (TargetChild != NULL)
        RBNodeSetParent(TargetChild, Parent);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 删除调整函数 */
static void RBT_RemoveNode_Adjust(RB_Tree *Tree, RB_Node *Node, uint8_t Side)
{
    ERROR_PRINT(Tree == NULL, "RBT_RemoveNode_Adjust: Tree");
    //情况1:删除空节点,无需调整
    RETURN_EMPTY(Node == NULL);
    //1.获得删除的节点的父亲和左右孩子
    uint8_t Color  = RBNodeGetColor(Node);
    RB_Node *LeftChild  = RBNodeGetChild(Node, LEFT);
    RB_Node *RightChild = RBNodeGetChild(Node, RIGHT);
    RB_Node *OnlyOne    = (LeftChild == NULL ? RightChild : LeftChild);
    //情况2:删除节点为红色,无需调整
    RETURN_EMPTY(Color == RED);
    //情况3:自己为黑色但是唯一孩子为红色
    if (OnlyOne != NULL && RBNodeGetColor(OnlyOne) == RED) {
            RBNodeSetBlack(OnlyOne);
            return;
    }
    //情况4:自己是黑色,删除导致缺失了一个黑色
    do {
        RB_Node *Parent = RBNodeGetParent(Node);
        uint8_t SiblingSide = RBNodeGetOtherSide(Side);
        //情况4.1:删除的节点为根节点(或者调整到根节点),什么都不做
        RETURN_EMPTY(Parent == NULL);
        RB_Node *Sibling = RBNodeGetChild(Parent, SiblingSide);
        //注意:删除黑色节点导致的不平衡兄弟一定存在
        ERROR_PRINT(Sibling == NULL, "RBT_RemoveNode_Adjust: Sibling");
        //情况4.2:兄弟节点为红色
        if (RBNodeGetColor(Sibling) == RED) {
            //1.父节点染成红色,兄弟节点染成黑色
            RBNodeSetBlack(Sibling);
            RBNodeSetRed(Parent);
            //2.对父节点进行旋转
            RBT_RotateNode(Tree, Sibling, Parent);
            //更新父子兄弟关系
            Sibling = RBNodeGetChild(Parent, SiblingSide);
        }
        
        RB_Node *SiblingLeftChild  = RBNodeGetChild(Sibling, LEFT);
        RB_Node *SiblingRightChild = RBNodeGetChild(Sibling, RIGHT);
        
        //情况4.3:孩子都是黑色
        if ((SiblingLeftChild  == NULL ||
            RBNodeGetColor(SiblingLeftChild) == BLACK) &&
            (SiblingRightChild == NULL ||
            RBNodeGetColor(SiblingRightChild) == BLACK)) {
            if (RBNodeGetColor(Parent) == BLACK) {
                //情况4.3.1:父亲是黑色
                //1.染色兄弟
                RBNodeSetRed(Sibling);
                //2.调整到上一层
                Node = Parent;
                Parent = RBNodeGetParent(Node);
                RETURN_EMPTY(Parent == NULL);
                Side = RBNodeGetSide(Node, Parent);
                ERROR_PRINT(Side == ERROR, "RBT_RemoveNode_Adjust: Side");
                continue;
            } else {
                //情况4.3.2:父亲是红色
                //1.交换父亲兄弟的颜色
                RBNodeSetBlack(Parent);
                RBNodeSetRed(Sibling);
                return;
            }
        }
        
        RB_Node *RedChild = NULL;
        //情况4.4:如果是之字型分支,先通过旋转调整到俩个对位
        //情况4.4.1:孩子节点是父左节点,兄弟左儿子是红色,右儿子是黑色
        //情况4.4.2:孩子节点是父右节点,兄弟右儿子是红色,左儿子是黑色
        if (Side == LEFT &&  (SiblingRightChild == NULL || 
            RBNodeGetColor(SiblingRightChild) == BLACK))
                RedChild = SiblingLeftChild;
        
        if (Side == RIGHT && (SiblingLeftChild == NULL || 
            RBNodeGetColor(SiblingLeftChild) == BLACK))
                RedChild = SiblingRightChild;
        
        if (RedChild != NULL) {
            //1.染红兄弟节点,染黑红色孩子
            RBNodeSetBlack(RedChild);
            RBNodeSetRed(Sibling);
            //2.对兄弟节点进行旋转
            RBT_RotateNode(Tree, RedChild, Sibling);
            //兄弟节点以及对应的左右孩子都更新了!
            Sibling = RedChild;
            SiblingLeftChild  = RBNodeGetChild(Sibling, LEFT);
            SiblingRightChild = RBNodeGetChild(Sibling, RIGHT);
        }
            
        RedChild = NULL;
        //情况4.5:如果是对位分支,直接内部旋转
        //情况4.5.1:孩子节点是父左节点,兄弟的右孩子为红色
        //情况4.5.2:孩子节点是父右节点,兄弟的左孩子为红色
        if (Side == LEFT &&  (SiblingRightChild != NULL && 
            RBNodeGetColor(SiblingRightChild) == RED))
                RedChild = SiblingRightChild;
        
        if (Side == RIGHT && (SiblingLeftChild != NULL && 
            RBNodeGetColor(SiblingLeftChild) == RED))
                RedChild = SiblingLeftChild;
        
        if (RedChild != NULL) {
            //1.兄弟获得父节点的颜色
            RBNodeSetColor(Sibling, RBNodeGetColor(Parent));
            //2.父节点获得兄弟节点的颜色
            RBNodeSetBlack(Parent);
            //3.染黑红子节点
            RBNodeSetBlack(RedChild);
            //4.对父节点进行旋转
            RBT_RotateNode(Tree, Sibling, Parent);
        }
        
        return;
    } while (1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 上层接口封装 */

void RBT_InsertNode(void *Tree, void *Node)
{
    RBT_InsertNode_Only((RB_Tree *)Tree, (RB_Node *)Node);
    RBT_InsertNode_Adjust((RB_Tree *)Tree, (RB_Node *)Node);
}

void RBT_RemoveNode(void *Tree, void *Node)
{
    uint8_t Side = ERROR;
    RBT_RemoveNode_Only((RB_Tree *)Tree, (RB_Node **)(&Node), &Side);  
    RBT_RemoveNode_Adjust((RB_Tree *)Tree, (RB_Node *)Node, Side);
}

void * RBT_SearchNode(void *Tree, void *Data)
{
    return (void *)RBT_SearchNode_Only((RB_Tree *)Tree, Data);
}

void * RBT_Search_Prev(void *Node)
{
    return (void *)RBT_Search_PrevOrNext((RB_Node *)Node, LEFT);
}

void * RBT_Search_Next(void *Node)
{
    return (void *)RBT_Search_PrevOrNext((RB_Node *)Node, RIGHT);
}

void * RBT_Search_Min(void *Tree)
{
    RB_Node *Root = RBTreeGetRoot((RB_Tree *)Tree);
    return (void *)RBT_Search_MinOrMax(Root, LEFT);
}

void * RBT_Search_Max(void *Tree)
{
    RB_Node *Root = RBTreeGetRoot((RB_Tree *)Tree);
    return (void *)RBT_Search_MinOrMax(Root, RIGHT);
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
/* 验证一棵树是否为红黑树(内部检查) */
uint8_t RBT_InternalCheck(void *Tree, void *Stack, int32_t *Flags, int32_t Length)
{
    ERROR_PRINT(Tree == NULL,  "RBT_InternalCheck: Tree");
    ERROR_PRINT(Stack == NULL, "RBT_InternalCheck: Tree");
    ERROR_PRINT(Flags == NULL, "RBT_InternalCheck: Tree");
    
    //验证:1根节点不是黑色
    if (RBTreeGetRoot((RB_Tree *)Tree) == NULL)
        return 1;
    if (RBNodeGetColor(RBTreeGetRoot((RB_Tree *)Tree)) == RED)
        return 0;
    
    //初始化flags的值
    for (int32_t Index = 0; Index < Length; Index++) Flags[Index] = LEFT;
    //深度优先搜索,使用flags同步维护栈进动情况
    RB_Node *LeftChild  = NULL;
    RB_Node *RightChild = NULL;
    RB_Node *Node = NULL;
    //从根节点开始从左进动
    //入栈节点只有当左右孩子都使用完毕后才退栈
    int32_t MaxLength = 0;//一条从根到叶子的最大度
    int32_t CurrentBlackLength = 0;
    int32_t StackTop = 0;//栈顶位置
    
    //1.根节点入栈
    ((RB_Node **)Stack)[StackTop++] = RBTreeGetRoot((RB_Tree *)Tree);
    CurrentBlackLength++;
    Flags[StackTop - 1] = LEFT;
    
    do {
        //2.获得栈顶元素的左右孩子
        Node = ((RB_Node **)Stack)[StackTop - 1];
        LeftChild  = RBNodeGetChild(Node, LEFT);
        RightChild = RBNodeGetChild(Node, RIGHT);
        
        //验证:2红色节点不相连
        if (LeftChild != NULL)
            if (RBNodeGetColor(Node) == RED &&
                RBNodeGetColor(LeftChild) == RED)
                    return 0;
                    
        if (RightChild != NULL)
            if (RBNodeGetColor(Node) == RED &&
                RBNodeGetColor(RightChild) == RED)
                    return 0;
        
        //3.左孩子未曾进过栈
        if (Flags[StackTop - 1] == LEFT) {
            //3.1.标记左孩子进过栈,下一次该右孩子进了
            Flags[StackTop - 1] = RIGHT;
            //左孩子存在,可以进栈
            if(LeftChild != NULL) {
                ((RB_Node **)Stack)[StackTop++] = LeftChild;
                //如果左孩子是黑色的,计算其度
                if (RBNodeGetColor(LeftChild) == BLACK)
                    CurrentBlackLength++;
                Flags[StackTop - 1] = LEFT;//左孩子的左孩子未曾进栈
                continue;
            } else {
                //当前节点不存在左孩子
                //说明一条从根到叶子的路径产生了
                if (MaxLength == 0)
                    MaxLength = CurrentBlackLength;
                //验证:路径黑色节点不一致
                if (MaxLength != CurrentBlackLength)
                    return 0;
            }
        }
        
        //4.右孩子未曾进过栈
        if (Flags[StackTop - 1] == RIGHT) {
            //3.2标记右孩子进过栈,下一次该退栈了
            Flags[StackTop - 1] = ERROR;
            //右孩子存在,可以进栈
            if(RightChild != NULL) {
                ((RB_Node **)Stack)[StackTop++] = RightChild;
                //如果右孩子是黑色的,计算其度
                if (RBNodeGetColor(RightChild) == BLACK)
                    CurrentBlackLength++;
                Flags[StackTop - 1] = LEFT;//右孩子的左孩子未曾进栈
                continue;
            } else {
                //当前节点不存在右孩子,
                //说明一条从根到叶子的路径产生了
                if (MaxLength == 0)
                    MaxLength = CurrentBlackLength;
                //验证:路径黑色节点不一致
                if (MaxLength != CurrentBlackLength)
                    return 0;
            }
        }
        
        //5.都进栈了,当前节点该退栈了
        if (Flags[StackTop - 1] == ERROR) {
            if (RBNodeGetColor(Node) == BLACK)
                    CurrentBlackLength--; 
            StackTop--;
        }
        
        //队列太小,警告
        ERROR_PRINT(StackTop >= Length, "RBT_InternalCheck: overflow");
        RETURN_EMPTY(StackTop >= Length);
    } while (StackTop > 0);
    
    //6.退栈完毕了,验证完成
    return 1;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
