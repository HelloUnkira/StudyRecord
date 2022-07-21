#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "BStarTree.h"
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
static inline void * BST_Allocate(uint32_t size)
{
#ifdef BST_ALLOC
    return BST_ALLOC(size);
#else
#error "B * Tree Needs To Redirect The Dynamic Allocate Function"
#endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void BST_Release(void *pointer)
{
#ifdef BST_FREE
    BST_FREE(pointer);
#else
#error "B * Tree Needs To Redirect The Dynamic Release Function"
#endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* B*树核心(B*树容器) */
typedef struct BST_Tree_Node {
    struct BST_Tree_Node *ThisParent;   /* 父亲 */
    struct BST_Tree_Node *LeftBrother;  /* 左兄弟 */
    struct BST_Tree_Node *RightBrother; /* 右兄弟 */
    void    **ChildOrDataSetAndState;   /* 数据元集或节点集 */
    uint32_t *ChildOrDataKeySet;        /* 关键字集 */
    uint32_t  SetNumber;                /* 当前数据量 */
} BST_Node;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* B*树核心(B*树集合) */
typedef struct BST_Tree_Tree {
    BST_Node *Root;
    uint32_t ArrayLength;   //节点和叶子数组限制
    uint32_t Criterion;     //节点分裂值
    uint32_t NodeNumber;
    uint32_t (*GetDataKey)(void *Data);
} BST_Tree;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//计算叶子节点实体的关键字
typedef uint32_t (*BST_GetDataKey)(void *Data);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef enum {BST_COMMON = 0, BST_LEAVES = 1} BST_State;
typedef enum {BST_ERROR = 2}                  BST_Error;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 地址等价整型,满足: sizeof(BST_VoidSTarType) >= sizeof(void *) */
#define BST_VoidSTarType    uint64_t
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* B*树集合抽象方法 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void BST_TreeSetRoot(BST_Tree *Tree, BST_Node *Root)
{
    Tree->Root = Root;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline BST_Node * BST_TreeGetRoot(BST_Tree *Tree)
{
    return Tree->Root;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void BST_TreeSetArrayLength(BST_Tree *Tree, uint32_t Length)
{
    Tree->ArrayLength = Length;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint32_t BST_TreeGetArrayLength(BST_Tree *Tree)
{
    return Tree->ArrayLength;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void BST_TreeSetDataKey(BST_Tree *Tree, BST_GetDataKey Key)
{
    Tree->GetDataKey = Key;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint32_t BST_TreeDataKey(BST_Tree *Tree, void *Data)
{
    return Tree->GetDataKey(Data);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void BST_TreeSetCriterion(BST_Tree *Tree, uint32_t Criterion)
{
    Tree->Criterion = Criterion;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint32_t BST_TreeGetCriterion(BST_Tree *Tree)
{
    return Tree->Criterion;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void BST_TreeSetNodeNumber(BST_Tree *Tree, uint32_t Number)
{
    Tree->NodeNumber = Number;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint32_t BST_TreeGetNodeNumber(BST_Tree *Tree)
{
    return Tree->NodeNumber;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* B*树容器抽象方法(设置) */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void BST_NodeSetParent(BST_Node *Node, BST_Node *Parent)
{
    Node->ThisParent = Parent;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void BST_NodeSetLeft(BST_Node *Node, BST_Node *Brother)
{
    Node->LeftBrother = Brother;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void BST_NodeSetRight(BST_Node *Node, BST_Node *Brother)
{
    Node->RightBrother = Brother;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void BST_NodeSetKeySet(BST_Node *Node, uint32_t *KeySet)
{
    Node->ChildOrDataKeySet = KeySet;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void BST_NodeSetSetNumber(BST_Node *Node, uint32_t Number)
{
    Node->SetNumber = Number;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void BST_NodeSetChildOrDataSet(BST_Node *Node, void **Set)
{
    BST_VoidSTarType Result = 0;
    
    Result |= (BST_VoidSTarType)(Node->ChildOrDataSetAndState);
    Result &= (BST_VoidSTarType)(1);
    Result |= (BST_VoidSTarType)(Set);

    Node->ChildOrDataSetAndState = (void *)Result;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void BST_NodeSetState(BST_Node *Node, uint32_t State)
{
    BST_VoidSTarType Result = 0;
    
    Result |= (BST_VoidSTarType)(Node->ChildOrDataSetAndState);
    Result &= (BST_VoidSTarType)(~1);
    
    if (State  == BST_LEAVES)
        Result |= (BST_VoidSTarType)BST_LEAVES;
    if (State  == BST_COMMON)
        Result |= (BST_VoidSTarType)BST_COMMON;

    Node->ChildOrDataSetAndState = (void *)Result;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* B*树容器抽象方法(获取) */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void * BST_NodeGetParent(BST_Node *Node)
{
    return Node->ThisParent;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void * BST_NodeGetLeft(BST_Node *Node)
{
    return Node->LeftBrother;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void * BST_NodeGetRight(BST_Node *Node)
{
    return Node->RightBrother;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint32_t * BST_NodeGetKeySet(BST_Node *Node)
{
    return Node->ChildOrDataKeySet;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint32_t BST_NodeGetSetNumber(BST_Node *Node)
{
    return Node->SetNumber;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void ** BST_NodeGetChildOrDataSet(BST_Node *Node)
{
    BST_VoidSTarType Result = 0;
    
    Result |= (BST_VoidSTarType)(Node->ChildOrDataSetAndState);
    Result &= (BST_VoidSTarType)(~1);
    
    return (void **)Result;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline uint32_t BST_NodeGetState(BST_Node *Node)
{
    BST_VoidSTarType Result = 0;
    
    Result |= (BST_VoidSTarType)(Node->ChildOrDataSetAndState);
    Result &= (BST_VoidSTarType)(1);
    
    if (Result == BST_LEAVES)
        return BST_LEAVES;
    if (Result == BST_COMMON)
        return BST_COMMON;

    return BST_ERROR;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 辅助定制宏,美化代码,剔除边缘逻辑 */
#define RETURN_EMPTY(target) if (target) return;
#define RETURN_ZERO(Target)  if (Target) return 0;
#define RETURN_NULL(Target)  if (Target) return NULL;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 创建一个动态节点 */
static BST_Node * BST_CreateNode(BST_Tree *Tree, uint32_t State)
{
    ERROR_PRINT(Tree == NULL, "BST_CreateNode: Tree");
    ERROR_PRINT(BST_TreeGetArrayLength(Tree) < 3, "BST_CreateNode: Tree Zone");
    //分配空间计算
    uint32_t Zone1 = sizeof(BST_Node);
    uint32_t Zone2 = BST_TreeGetArrayLength(Tree) * sizeof(uint32_t);
    uint32_t Zone3 = BST_TreeGetArrayLength(Tree) * sizeof(void *);
    //1.创建节点,创建关键字集,创建数据元集或节点集
    BST_Node *Node = BST_Allocate(Zone1);
    uint32_t *KeySet = BST_Allocate(Zone2);
    void     *NodeSet = BST_Allocate(Zone3);
    //2.检查分配空间
    if (Node == NULL || KeySet == NULL || NodeSet == NULL) {
        BST_Release(NodeSet);
        BST_Release(KeySet);
        BST_Release(Node);
        //利用外界的容错检查简化此处
        ERROR_PRINT(NULL == NULL, "BST_CreateNode: malloc Node");
        RETURN_NULL(NULL == NULL);
    }
    //3.初始化该节点,直接清零
    for (uint32_t Index = 0; Index < Zone1; Index++) ((uint8_t *)Node)[Index] = 0;
    for (uint32_t Index = 0; Index < Zone2; Index++) ((uint8_t *)KeySet)[Index] = 0;
    for (uint32_t Index = 0; Index < Zone3; Index++) ((uint8_t *)NodeSet)[Index] = 0;
    //4.关键字集绑定,数据元集或节点集绑定
    BST_NodeSetChildOrDataSet(Node, NodeSet);
    BST_NodeSetKeySet(Node, KeySet);
    BST_NodeSetState(Node, State);
    return Node;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 销毁一个动态节点 */
static void BST_DestroyNode(void *Node)
{
    BST_Release(BST_NodeGetChildOrDataSet(Node));
    BST_Release(BST_NodeGetKeySet(Node));
    BST_Release(Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 辅助功能开始 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 二分搜索 */
static void BST_BinarySearch(uint32_t KeyValue, uint32_t *KeySet, uint32_t Boundary,
                             int32_t *Left, int32_t *Right)
{
    ERROR_PRINT(Boundary >= 2,  "BST_BinarySearch: Boundary");
    ERROR_PRINT(KeySet == NULL, "BST_BinarySearch: KeySet");
    ERROR_PRINT(*Left > *Right, "BST_BinarySearch: Left>Right");
    RETURN_EMPTY(KeySet == NULL || *Left > *Right || Boundary >= 2);
    int32_t Center = 0;
    while (*Left <= *Right) {
        Center = *Left + (*Right - *Left) / 2;
        if (KeyValue > KeySet[Center])
            *Left  = Center + 1;
        if (KeyValue < KeySet[Center])
            *Right = Center - 1;
        //根据参数确定,左右边界 Boundary ==>0:  <xxx<=  1:  <=xxx<
        if (KeyValue == KeySet[Center]) {
            if (Boundary == 0)
                *Right = Center - 1;
            if (Boundary == 1)
                *Left  = Center + 1;
        }
    }
    //此时交换left与right的值(为人性化使用其数据)
    *Left  = *Left ^ *Right;
    *Right = *Left ^ *Right;
    *Left  = *Left ^ *Right;
    //不可检查左边界溢出(场景要交付给上层使用)
    //不可检查右边界溢出(场景要交付给上层使用)
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 迭代更新从指定节点到根节点的索引 */
static void BST_KeyValueUpdate(BST_Node *Node, uint32_t KeyValueNew, uint32_t KeyValueOld)
{
    RETURN_EMPTY(Node == NULL || KeyValueNew == KeyValueOld);
    //迭代向上更新对应数据元,因为它是去往最近最小的项
    Node = BST_NodeGetParent(Node);
    while (Node != NULL && KeyValueNew != KeyValueOld) {
        //遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
        uint32_t *KeySet    = BST_NodeGetKeySet(Node);
        uint32_t  SetNumber = BST_NodeGetSetNumber(Node);
        int32_t Left  = 0;
        int32_t Right = SetNumber - 1;
        int32_t Index = -1;
        //查找更新father中对应数据
        if (KeyValueNew > KeyValueOld) {
            BST_BinarySearch(KeyValueOld, KeySet, 1, &Left, &Right);
            Index = (Left < 0) ? 0 : Left;

        }
        if (KeyValueNew < KeyValueOld) {
            BST_BinarySearch(KeyValueOld, KeySet, 0, &Left, &Right);
            Index = (Right > SetNumber - 1) ? SetNumber - 1 : Right;
        }
        
        //注意:必须要保证更新正确的父亲的关键字索引
        ERROR_PRINT(KeySet[Index] != KeyValueOld, "BST_KeyValueUpdate");
        KeyValueOld = KeySet[Index];
        KeySet[Index] = KeyValueNew;
        //迭代到下一层,如果可以的话
        Node = BST_NodeGetParent(Node);
        RETURN_EMPTY(Index != 0);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 从指定节点迁移批量数据到另一节点 */
static void BST_MigrateData(BST_Tree *Tree, BST_Node *Node1, BST_Node *Node2,
                            uint32_t MigrateNumber, uint32_t MigrateState)
{
    RETURN_EMPTY(Tree == NULL || Node1 == NULL || Node2 == NULL);
    //指定将其进行数据迁移:data1 ==> data2
    //MigrateState == 0,将data1后migrate_number数据加入到data2的前面
    //MigrateState == 1,将data1前migrate_number数据加入到data2的后面
    //注意:跨节点迁移时其对父亲的索引要一并迁移
    int32_t Index = 0, Temp = 0;//迁移所用到的索引
    uint32_t  Length  = BST_TreeGetArrayLength(Tree);
    uint32_t *KeySet1 = BST_NodeGetKeySet(Node1);
    uint32_t *KeySet2 = BST_NodeGetKeySet(Node2);
    void **NodeSet1 = BST_NodeGetChildOrDataSet(Node1);
    void **NodeSet2 = BST_NodeGetChildOrDataSet(Node2);
    uint32_t  SetNumber1 = BST_NodeGetSetNumber(Node1);
    uint32_t  SetNumber2 = BST_NodeGetSetNumber(Node2);
    uint32_t  Node1State = BST_NodeGetState(Node1);
    uint32_t  Node2State = BST_NodeGetState(Node2);
    //要迁移migrate项数据(极限检测)
    ERROR_PRINT(MigrateState >= 2, "BST_MigrateData: MigrateState");
    ERROR_PRINT(Node1State != Node2State, "BST_MigrateData: NodeState");
    ERROR_PRINT(SetNumber1 - MigrateNumber < 0, "BST_MigrateData: 1");
    ERROR_PRINT(SetNumber2 + MigrateNumber > Length, "BST_MigrateData: 2");

    //根据实际情况继续不同的迁移动作
    if (MigrateState == 0) {
        //1.先漂移data2的数据
        if (SetNumber2 != 0)
            for (Index = SetNumber2 - 1; Index >= 0; Index--) {
                KeySet2[Index + MigrateNumber]  = KeySet2[Index];
                NodeSet2[Index + MigrateNumber] = NodeSet2[Index];
            }
        //2.data1的数据迁移到data2
        for (Index = 0; Index < MigrateNumber; Index++) {
            KeySet2[Index]  = KeySet1[Index + SetNumber1 - MigrateNumber];
            NodeSet2[Index] = NodeSet1[Index + SetNumber1 - MigrateNumber];
            Temp = Index;
            if (Node2State != BST_LEAVES && NodeSet2[Temp] != NULL)
                BST_NodeSetParent((BST_Node*)(NodeSet2[Temp]), Node2);
        }
    }
    if (MigrateState == 1) {
        //1.data1的数据迁移到data2
        for (Index = 0; Index < MigrateNumber; Index++) {
            KeySet2[Index + SetNumber2]  = KeySet1[Index];
            NodeSet2[Index + SetNumber2] = NodeSet1[Index];
            Temp = Index + SetNumber2;
            if (Node1State != BST_LEAVES && NodeSet2[Temp] != NULL)
                BST_NodeSetParent((BST_Node*)(NodeSet2[Temp]), Node2);
        }
        //2.后漂移data1的数据
        if (SetNumber1 - MigrateNumber != 0)
            for (Index = 0; Index < SetNumber1 - MigrateNumber; Index++) {
                KeySet1[Index] = KeySet1[Index + MigrateNumber];
                NodeSet1[Index] = NodeSet1[Index + MigrateNumber];
            }
    }
    //更新data1和data2的计数量
    BST_NodeSetSetNumber(Node1, SetNumber1 - MigrateNumber);
    BST_NodeSetSetNumber(Node2, SetNumber2 + MigrateNumber);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 迭代搜索到叶节点,根据特定关键字 */
static void BST_SearchNodeOnly(BST_Tree *Tree, BST_Node **Node, uint32_t KeyValue)
{
    RETURN_EMPTY(Tree == NULL || Node == NULL);
    BST_Node *Root = BST_TreeGetRoot(Tree);
    //从根节点一直走到叶子节点即可
    while (Root != NULL && BST_NodeGetState(Root) == BST_COMMON) {
        //根节点至少是有元素,此时需要向下搜索
        uint32_t *KeySet = BST_NodeGetKeySet(Root);
        void    **NodeSet = BST_NodeGetChildOrDataSet(Root);
        uint32_t  SetNumber = BST_NodeGetSetNumber(Root);
        //遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
        int32_t Left  = 0;
        int32_t Right = SetNumber - 1;
        //二分搜索,查找时有效项是left
        BST_BinarySearch(KeyValue, KeySet, 1, &Left, &Right);
        Left = (Left < 0) ? 0 : Left;
        //判断有效元,因为查找时可能出现溢出的情况
        Root = (BST_Node *)(NodeSet[Left]);
    }
    *Node = Root;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 在一个指定节点中插入一个项 */
static void BST_InsertDataOnly(BST_Node *Node, uint32_t State, uint32_t *Index,
                                               uint32_t KeyValue, void  *Item)
{
    RETURN_EMPTY(Node == NULL || Index == NULL || Item == NULL || State >= 2);
    uint32_t *KeySet = BST_NodeGetKeySet(Node);
    void    **NodeSet = BST_NodeGetChildOrDataSet(Node);
    uint32_t  SetNumber = BST_NodeGetSetNumber(Node);
    //1.遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
    int32_t Left = 0, Right = (int32_t)SetNumber - 1;
    BST_BinarySearch(KeyValue, KeySet, State, &Left, &Right);
    *Index = (Right < 0) ? 0 : Right;//插入时,有效项是right
    //2.需要将后续的部分迁移到下一项,为新入项腾出一个空间
    if (*Index < SetNumber) {
        for (int32_t Index1 = SetNumber - 1; Index1 >= (int32_t)(*Index); Index1--) {
            //当前项覆盖后一项,直到==index覆盖完毕
            KeySet[Index1 + 1]  = KeySet[Index1];
            NodeSet[Index1 + 1] = NodeSet[Index1];
        }
    }
    //3.将数据加入到指定位置即可
    NodeSet[*Index] = Item;
    KeySet[*Index]  = KeyValue;
    BST_NodeSetSetNumber(Node, SetNumber + 1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 从一个指定节点中移除一个项 */
static void BST_RemoveDataOnly(BST_Node *Node, uint32_t State, uint32_t *Index,
                                               uint32_t KeyValue, void **Item)
{
    RETURN_EMPTY(Node == NULL || Index == NULL || Item == NULL || State >= 2);
    uint32_t *KeySet = BST_NodeGetKeySet(Node);
    void    **NodeSet = BST_NodeGetChildOrDataSet(Node);
    uint32_t  SetNumber = BST_NodeGetSetNumber(Node);
    //如果是无效的删除的话,直接回退即可
    *Item == NULL;
    RETURN_EMPTY(SetNumber == 0);
    int32_t Left = 0, Right = (int32_t)SetNumber - 1, Index1 = 0;
    //1.遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
    BST_BinarySearch(KeyValue, KeySet, State, &Left, &Right);
    *Index = ((State == 1) ? ((Left < 0) ? 0 : Left) :
             ((State == 0) ?  (Left + 1) : 0));//删除时,有效项是left
    ERROR_PRINT(KeySet[*Index] != KeyValue, "BST_RemoveDataOnly: KeyValue");
    //2.记录其数据
    *Item = NodeSet[*Index];
    //3.需要将后续的部分迁移到前一项,为其覆盖一个空间
    if (SetNumber != 1 && *Index < SetNumber)
        for (Index1 = *Index; Index1 <= (int32_t)(SetNumber); Index1++) {
            //当前项覆盖后一项,直到==index覆盖完毕
            KeySet[Index1]  = KeySet[Index1 + 1];
            NodeSet[Index1] = NodeSet[Index1 + 1];
        }
    BST_NodeSetSetNumber(Node, SetNumber - 1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 从一个指定节点中搜索一个项 */
static void BST_SearchDataOnly(BST_Node *Node, uint32_t State, uint32_t *Index,
                                               uint32_t KeyValue, void **Item)
{
    RETURN_EMPTY(Node == NULL || Index == NULL || Item == NULL || State >= 2);
    uint32_t *KeySet = BST_NodeGetKeySet(Node);
    void    **NodeSet = BST_NodeGetChildOrDataSet(Node);
    uint32_t  SetNumber = BST_NodeGetSetNumber(Node);
    //保证搜索时,内部至少有一个项
    *Item = NULL;
    RETURN_EMPTY(SetNumber == 0);
    //遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
    int32_t Left = 0, Right = SetNumber - 1;
    BST_BinarySearch(KeyValue, KeySet, State, &Left, &Right);
    *Index = ((State == 1) ? ((Left < 0) ? 0 : Left) :
             ((State == 0) ?  (Left + 1) : 0));//搜索时,有效项是left
    ERROR_PRINT(KeySet[*Index] != KeyValue, "BST_SearchDataOnly: KeyValue");
    //记录其数据
    *Item = NodeSet[*Index];
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 辅助功能结束 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 单次插入函数 */
static void BST_InsertOnly(BST_Tree *Tree, BST_Node **Node, void *Data)
{
    ERROR_PRINT(Tree == NULL || Node == NULL || Data == NULL, "only_insert");
    //1.计算关键字
    uint32_t KeyValue = BST_TreeDataKey(Tree, Data);
    //2.如果是插入到空树中时或者插入到叶子节点时
    *Node = BST_TreeGetRoot(Tree);
    //3.确认是否是初次插入,不存在项
    if (*Node == NULL) {
        //为其生成一个空间,设置其为叶子节点
        BST_TreeSetRoot(Tree, BST_CreateNode(Tree, BST_LEAVES));
        *Node = BST_TreeGetRoot(Tree);
        RETURN_EMPTY(*Node == NULL);
    }
    //4.迭代搜索到叶子节点
    BST_SearchNodeOnly(Tree, Node, KeyValue);
    //5.数据元加入到叶子节点
    int32_t Index = -1;
    BST_InsertDataOnly(*Node, 1, &Index, KeyValue, Data);
    //6.插入的节点是叶子节点的首节点,修复它所产生的影响
    if (Index == 0) {
        uint32_t *KeySet = BST_NodeGetKeySet(*Node);
        BST_KeyValueUpdate(*Node, KeySet[0], KeySet[1]);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 插入调整函数 */
static void BST_InsertAdjust(BST_Tree *Tree, BST_Node *Node)
{
    RETURN_EMPTY(Tree == NULL || Node == NULL);
    uint32_t Index = 0;
    uint32_t State = 0;
    uint32_t *KeySet1 = NULL, KeyValue1 = 0;
    uint32_t *KeySet2 = NULL, KeyValue2 = 0;
    uint32_t *KeySet3 = NULL, KeyValue3 = 0;
    uint32_t Length = BST_TreeGetArrayLength(Tree);
    uint32_t Criterion = BST_TreeGetCriterion(Tree);
    uint32_t SetNumber = BST_NodeGetSetNumber(Node);
    BST_Node *Parent = NULL;
    BST_Node *LeftBrother = NULL;
    BST_Node *RightBrother = NULL;
    //插入时导致的节点超出
    //如果左右兄弟只要有一个存在盈余空间就无需分裂
    //如果左右兄弟都负重不堪,那么往左边分裂和往右边分裂(这是一个问题)
    //这里直接按正常习惯往右边分裂,但是到底需要有向左边分裂的必要性,是否
    //是叶节点分裂还是非叶子节点分裂(情况是一致的)
    while (SetNumber >= Criterion) {
        //先获取它与周围节点的关系
        Parent = BST_NodeGetParent(Node);
        LeftBrother = BST_NodeGetLeft(Node);
        RightBrother = BST_NodeGetRight(Node);
        State = BST_NodeGetState(Node);
        //情况1:如果右兄弟有盈余空间
        if (RightBrother != NULL &&
            BST_NodeGetSetNumber(RightBrother) < Criterion) {
            //1.获取右兄弟在父亲那的记录
            KeySet3 = BST_NodeGetKeySet(RightBrother);
            KeyValue3 = KeySet3[0];
            //2.将一部分数据迁移至右兄弟
            BST_MigrateData(Tree, Node, RightBrother, Length - Criterion, 0);
            //3.迭代更新从指定节点到根节点的索引
            Node = RightBrother;
            BST_KeyValueUpdate(Node, KeySet3[0], KeyValue3);
            return;
        }
        
        //情况2:如果左兄弟有盈余空间(它应该只做少部分数据承担者)
        //添加该种情况的话可以更大化的利用空间
        if (LeftBrother != NULL &&
            BST_NodeGetSetNumber(LeftBrother) < Criterion) {
            //1.获取自己在父亲那的记录
            KeySet1 = BST_NodeGetKeySet(Node);
            KeyValue1 = KeySet1[0];
            //2.将一部分数据迁移至左兄弟
            BST_MigrateData(Tree, Node, LeftBrother, Length - Criterion, 1);
            //3.迭代更新从指定节点到根节点的索引
            BST_KeyValueUpdate(Node, KeySet1[0], KeyValue1);
            return;
        }
        
        //情况3:如果没有兄弟节点时
        if (RightBrother == NULL) {
            //1.创建兄弟节点
            RightBrother = BST_CreateNode(Tree, State);
            ERROR_PRINT(RightBrother == NULL, "BST_InsertAdjust: RightBrother");
            //2.为自己设置兄弟,为兄弟设置兄弟
            BST_NodeSetRight(Node, RightBrother);
            BST_NodeSetLeft(RightBrother, Node);
            //3.将本节点部分数据拷贝到右兄弟中去
            BST_MigrateData(Tree, Node, RightBrother, Length - Criterion, 0);
            //4.如果没有父亲要生成父亲,如果有父亲要添加兄弟节点
            if (Parent == NULL) {
                //1.创建一个父亲
                Parent = BST_CreateNode(Tree, BST_COMMON);
                ERROR_PRINT(Parent == NULL, "BST_InsertAdjust: Parent");
                //2.为自己设置新父亲
                BST_NodeSetParent(Node, Parent);
                //3.新生成父亲时需要更新根节点
                BST_TreeSetRoot(Tree, Parent);
                //4.将自己加入到新父亲中
                KeySet1 = BST_NodeGetKeySet(Node);
                BST_InsertDataOnly(Parent, 1, &Index, KeySet1[0], (void *)Node);
            }
            //5.为兄弟设置父亲
            BST_NodeSetParent(RightBrother, Parent);
            //6.将兄弟插入到父亲中
            KeySet3 = BST_NodeGetKeySet(RightBrother);
            BST_InsertDataOnly(Parent, 1, &Index, KeySet3[0], (void *)RightBrother);
            //7.迭代到上层调整
            SetNumber = BST_NodeGetSetNumber(Parent);
            Node = Parent;
            continue;
        }
        
        //情况4:自己和左右兄弟都是负重不堪
        //1.创建一个新节点
        BST_Node *Temp = BST_CreateNode(Tree, State);
        //2.为其设置它的父亲
        BST_NodeSetParent(Temp, Parent);
        //3.先迁移自己的数据
        BST_MigrateData(Tree, Node, Temp, Length - Criterion, 0);
        //4.获取右兄弟在父亲那的记录
        KeySet3 = BST_NodeGetKeySet(RightBrother);
        KeyValue3 = KeySet3[0];
        //5.将右兄弟一部分数据迁移
        BST_MigrateData(Tree, RightBrother, Temp, Length - Criterion, 1);
        //6.迭代更新从指定节点到根节点的索引
        BST_KeyValueUpdate(RightBrother, KeySet3[0], KeyValue3);
        //7.更新兄弟关系
        BST_NodeSetLeft(RightBrother, Temp);
        BST_NodeSetLeft(Temp, Node);
        BST_NodeSetRight(Node, Temp);
        BST_NodeSetRight(Temp, RightBrother);
        //9.新兄弟加入到父亲中
        KeySet1 = BST_NodeGetKeySet(Node);
        KeySet2 = BST_NodeGetKeySet(Temp);
        //注意:默认是在其最后产生分裂
        //但新兄弟在父亲的位置应该只能出现在当前节点之后
        State = ((KeySet1[0] <  KeySet2[0]) ? 0 :
                ((KeySet1[0] >= KeySet2[0]) ? 1 : 0));
        BST_InsertDataOnly(Parent, State, &Index, KeySet2[0], (void *)Temp);
        //10.迭代到上层调整
        SetNumber = BST_NodeGetSetNumber(Parent);
        Node = Parent;
        continue;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 单次删除函数 */
static void BST_RemoveOnly(BST_Tree *Tree, BST_Node **Node, uint32_t KeyValue, void **Data)
{
    ERROR_PRINT(Tree == NULL || Data == NULL || Node == NULL, "BST_RemoveOnly");
    *Node = BST_TreeGetRoot(Tree);
    //确认是否是初次删除,不存在项
    RETURN_EMPTY(*Node == NULL);
    int32_t Index;
    //从根节点开始向下迭代
    BST_SearchNodeOnly(Tree, Node, KeyValue);
    //数据元从叶子节点中移除
    BST_RemoveDataOnly(*Node, 1, &Index, KeyValue, Data);
    //调zheng一下删除该数所造成的影响
    uint32_t *KeySet = BST_NodeGetKeySet(*Node);
    if (Index != 0)
        return;
    BST_KeyValueUpdate(*Node, KeySet[0], KeyValue);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 删除调整函数 */
static void BST_RemoveAdjust(BST_Tree *Tree, BST_Node *Node)
{
    RETURN_EMPTY(Tree == NULL || Node == NULL);
    //先获取它与周围节点的关系
    BST_Node *Parent = BST_NodeGetParent(Node);
    BST_Node *LeftBrother = BST_NodeGetLeft(Node);
    BST_Node *RightBrother = BST_NodeGetRight(Node);
    uint32_t    SetNumber = BST_NodeGetSetNumber(Node);
    uint32_t    State = BST_NodeGetState(Node);
    //情况1:节点被清空(正常情况下,节点被清空之前会做合并的动作)
    if (SetNumber == 0 && State == BST_LEAVES) {
        //添加必要的检查
        ERROR_PRINT(Parent       != NULL, "BST_RemoveAdjust: Parent");
        ERROR_PRINT(LeftBrother  != NULL, "BST_RemoveAdjust: LeftBrother");
        ERROR_PRINT(RightBrother != NULL, "BST_RemoveAdjust: RightBrother");
        //它仅会出现在一种情况,就是清空树时,直接删除掉该节点就好
        BST_TreeSetRoot(Tree, NULL);
        BST_DestroyNode(Node);
        return;
    }
    
    void    *Data = NULL;
    uint32_t Index = -1;
    uint32_t Length = BST_TreeGetArrayLength(Tree);
    uint32_t Criterion = BST_TreeGetCriterion(Tree);
    //情况2:删除导致的节点缺失,可能需要合并
    //合并仅向右方向合并,向左兄弟合并无法很好的迭代更新
    //而数据的提供优先向左兄弟索要,它更容易被溢出
    //是叶节点合并还是非叶子节点合并(情况是一致的)
    while (Node != NULL && SetNumber < Criterion) {
        ERROR_PRINT(SetNumber == 0, "BST_RemoveAdjust: SetNumber");
        RETURN_EMPTY(SetNumber == 0);
        uint32_t *KeySet1 = NULL, KeyValue1 = 0;
        uint32_t *KeySet2 = NULL, KeyValue2 = 0;
        uint32_t *KeySet3 = NULL, KeyValue3 = 0;
        //获取需要调整的节点的兄弟节点和父节点,以及孩子数量
        Parent = BST_NodeGetParent(Node);
        LeftBrother = BST_NodeGetLeft(Node);
        RightBrother = BST_NodeGetRight(Node);
        
        //情况2.1:没有左右兄弟(孤立的节点)
        if (LeftBrother == NULL && RightBrother == NULL) {
            //合并时产生的孤立节点,如果它的数据量只有一个
            if (SetNumber == 1 && State == BST_COMMON) {
                void **NodeSet = BST_NodeGetChildOrDataSet(Node);
                //删除本体,让其孩子上位
                BST_TreeSetRoot(Tree, (BST_Node *)(NodeSet[0]));
                BST_DestroyNode(Node);
                //新的根节点被设置为空
                BST_NodeSetParent(BST_TreeGetRoot(Tree), NULL);
            }
            return;
        }
        
        //情况2.2:左兄弟数据充足
        if (LeftBrother != NULL && SetNumber +
            BST_NodeGetSetNumber(LeftBrother) >= Length) {
            //1.向左兄弟索要一个数据并插入到自己中
            BST_MigrateData(Tree, LeftBrother, Node, 1, 0);
            //2.迭代更新从指定节点到根节点的索引
            KeySet1 = BST_NodeGetKeySet(Node);
            BST_KeyValueUpdate(Node, KeySet1[0], KeySet1[1]);
            return;
        }
        
        //情况2.3:右兄弟数据充足
        if (RightBrother != NULL && SetNumber +
            BST_NodeGetSetNumber(RightBrother) >= Length) {
            //迁移前获取节点信息
            KeySet3 = BST_NodeGetKeySet(RightBrother);
            KeyValue3 = KeySet3[0];
            //1.向右兄弟索要一个数据并插入到自己中
            BST_MigrateData(Tree, RightBrother, Node, 1, 1);
            //2.迭代更新从指定节点到根节点的索引
            Node = RightBrother;
            BST_KeyValueUpdate(Node, KeySet3[0], KeyValue3);
            return;
        }
        
        //情况2.4:自己和右兄弟都贫困
        if (RightBrother != NULL && SetNumber +
            BST_NodeGetSetNumber(RightBrother) < Length) {
            //迁移前获取节点信息
            KeySet1 = BST_NodeGetKeySet(Node);
            KeySet3 = BST_NodeGetKeySet(RightBrother);
            KeyValue1 = KeySet1[0];
            KeyValue3 = KeySet3[0];
            //多多少少有些尴尬,因为对于多key的处理仅关系首项与末项
            //这里在向上迭代调整时会,使得node可能成为第二项
            //这很显然是设计上存在的缺失,不能将俩种情况合并
            if (KeyValue1 != KeyValue3) {
                //1.将右兄弟的数据迁移到自己
                SetNumber = BST_NodeGetSetNumber(RightBrother);
                BST_MigrateData(Tree, RightBrother, Node, SetNumber, 1);
                //2.删除右兄弟
                Parent = BST_NodeGetParent(RightBrother);
                RightBrother = BST_NodeGetRight(RightBrother);
                BST_DestroyNode(BST_NodeGetRight(Node));
                //3.更新兄弟关系
                BST_NodeSetRight(Node, RightBrother);
                if (RightBrother != NULL)
                    BST_NodeSetLeft(RightBrother, Node);
            }
            if (KeyValue1 == KeyValue3) {
                //1.将自己的数据迁移到右兄弟
                BST_MigrateData(Tree, Node, RightBrother, SetNumber, 0);
                //2.删除自己
                BST_DestroyNode(Node);
                //3.更新兄弟关系
                BST_NodeSetLeft(RightBrother, LeftBrother);
                if (LeftBrother != NULL)
                    BST_NodeSetRight(LeftBrother, RightBrother);
            }
            //4.删除在父亲中的记录
            BST_RemoveDataOnly(Parent, 0, &Index, KeyValue3, &Data);
            //5.清除父亲节点因删除照成的影响
            if (Index == 0) {
                KeySet3 = BST_NodeGetKeySet(Parent);
                BST_KeyValueUpdate(Parent, KeySet3[0], KeyValue3);
            }
            //6.迭代到上层调整
            SetNumber = BST_NodeGetSetNumber(Parent);
            State = BST_NodeGetState(Parent);
            Node = Parent;
            continue;
        }
    
        //情况2.5:自己和左兄弟都贫困
        if (LeftBrother != NULL && SetNumber +
            BST_NodeGetSetNumber(LeftBrother) < Length) {
            //迁移前获取节点信息
            KeySet1 = BST_NodeGetKeySet(Node);
            KeyValue1 = KeySet1[0];
            //1.将自己的数据迁移到左兄弟
            BST_MigrateData(Tree, Node, LeftBrother, SetNumber, 1);
            //2.删除自己
            BST_DestroyNode(Node);
            //3.更新兄弟关系
            BST_NodeSetRight(LeftBrother, RightBrother);
            if (RightBrother != NULL)
                BST_NodeSetLeft(RightBrother, LeftBrother);
            //4.删除自己在父亲中的记录
            BST_RemoveDataOnly(Parent, 1, &Index, KeyValue1, &Data);
            //5.清除父亲节点因删除照成的影响
            if (Index == 0) {
                KeySet1 = BST_NodeGetKeySet(Parent);
                BST_KeyValueUpdate(Parent, KeySet1[0], KeyValue1);
            }
            //6.迭代到上层调整
            SetNumber = BST_NodeGetSetNumber(Parent);
            State = BST_NodeGetState(Parent);
            Node = Parent;
            continue;
        }
        
        //也许会存在导致其节点亏空的情况,意味着它没有兄弟为其解决苦难
        //但我们依然相信,我们希望的美好的事情会发生
        break;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 单次查找函数 */
static void BST_SearchOnly(BST_Tree *Tree, uint32_t KeyValue, void **Data)
{
    ERROR_PRINT(Tree == NULL || Data == NULL, "BST_SearchOnly");
    BST_Node *Node = BST_TreeGetRoot(Tree);
    //确认是否是初次删除,不存在项
    *Data = NULL;
    RETURN_EMPTY(Node == NULL);
    int32_t Index;
    //从根节点开始向下迭代
    BST_SearchNodeOnly(Tree, &Node, KeyValue);
    RETURN_EMPTY(Node == NULL);
    //找到了目标节点,确定目标节点是否存在目标
    BST_SearchDataOnly(Node, 0, &Index, KeyValue, Data);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 上层接口封装 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void BST_InsertNode(void *Tree, void *Data)
{
    BST_Node *Node;
    BST_InsertOnly((BST_Tree *)Tree, &Node, Data);
    BST_InsertAdjust((BST_Tree *)Tree, Node);
    uint32_t cuttent_number;
    cuttent_number = BST_TreeGetNodeNumber((BST_Tree *)Tree);
    BST_TreeSetNodeNumber((BST_Tree *)Tree, cuttent_number + 1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void BST_RemoveNode(void *Tree, void **Data, uint32_t KeyValue)
{
    BST_Node *Node;
    BST_RemoveOnly((BST_Tree *)Tree, &Node, KeyValue, Data);
    BST_RemoveAdjust((BST_Tree *)Tree, Node);
    RETURN_EMPTY(*Data != NULL);
    uint32_t cuttent_number;
    cuttent_number = BST_TreeGetNodeNumber((BST_Tree *)Tree);
    BST_TreeSetNodeNumber((BST_Tree *)Tree, cuttent_number - 1);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void BST_SearchNode(void *Tree, void **Data, uint32_t KeyValue)
{
    BST_SearchOnly((BST_Tree *)Tree, KeyValue, Data);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 额外的辅助函数,为外界提供数据的建立空间 */
uint32_t BST_GetBytes_Set(void)
{
    return sizeof(BST_Tree);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
uint32_t BST_GetBytes_Container(void)
{
    return sizeof(BST_Node);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 辅助动作 */
void BST_SetTree(void *Tree, uint32_t Length, data_key Key)
{
    //一旦创建便会固定,所以需要容错处理
    RETURN_EMPTY(Tree == NULL || Key == NULL || Length < 3);
    ERROR_PRINT(Length == 3, "BST_SetTree");
    ERROR_PRINT(Length / 3 * 2 + 1 >= Length, "BST_SetTree");
    BST_TreeSetArrayLength((BST_Tree *)Tree, Length);
    BST_TreeSetCriterion((BST_Tree *)Tree, Length / 3 * 2 + 1);
    BST_TreeSetDataKey((BST_Tree *)Tree, Key);
    BST_TreeSetRoot((BST_Tree *)Tree, NULL);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 窥探:数据的层序遍历并打印 */
uint32_t BST_Sequence_Traversal(void *Tree, BST_Visit Visit, uint8_t Destroy)
{
    ERROR_PRINT(Tree == NULL, "RST_Sequence_Traversal: Tree");
    ERROR_PRINT(Visit == NULL, "RST_Sequence_Traversal: Visit");
    //层序遍历(这里使用循环队列):
    //创建一个队列(这里使用投机取巧的方法,队列极限情况下存在
    //数量最大时顶多应为满B*树的全部叶子节点数大小+1
    uint32_t Length = BST_TreeGetNodeNumber((BST_Tree *)Tree);
    uint32_t Zone   = sizeof(BST_Node *) * Length;
    BST_Node **Queue  = BST_Allocate(Zone);
    RETURN_ZERO(Queue == NULL);
    //获取插入的起始位置
    int32_t QueueHead = 0;
    int32_t QueueTail = 0;
    int32_t ElementNumber = 0;
    int32_t AllNumber = 0;
    
    Queue[QueueTail++] = BST_TreeGetRoot((BST_Tree *)Tree);
    QueueTail %= Length;
    ElementNumber++;
    //这是一个循环队列
    do {
        //节点出队列
        BST_Node *Node = Queue[QueueHead++];
        QueueHead %= Length;
        ElementNumber--;
        RETURN_ZERO(Node == NULL);
        uint32_t *KeySet = BST_NodeGetKeySet(Node);
        void    **NodeSet = BST_NodeGetChildOrDataSet(Node);
        uint32_t SetNumber = BST_NodeGetSetNumber(Node);
        //节点打印
        if (BST_NodeGetState(Node) == BST_COMMON)
            Visit(SetNumber, KeySet, NULL);
        if (BST_NodeGetState(Node) == BST_LEAVES)
            Visit(SetNumber, KeySet, NodeSet);
        
        //子节点入队列
        if (BST_NodeGetState(Node) == BST_COMMON) {
            int32_t Index = 0;
            for (Index = 0; Index < SetNumber; Index++) {
                Queue[QueueTail++] = (BST_Node *)(NodeSet[Index]);
                QueueTail %= Length;
                ElementNumber++;
            }
        }
        
        //计算元节点个数
        if (BST_NodeGetState(Node) == BST_LEAVES)
            AllNumber += SetNumber;
        
        if (Destroy != 0)
            BST_DestroyNode(Node);

        //队列太小,警告
        ERROR_PRINT(ElementNumber >= (int32_t)Length,
                    "RST_Sequence_Traversal: overflow");
        RETURN_ZERO(ElementNumber >= (int32_t)Length);
    } while (ElementNumber > 0);

    return AllNumber;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
