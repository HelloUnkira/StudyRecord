//C std lib
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define   BSTARTREE_C
#include "BStarTree.h"

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

#define BSTARTREE_MALLOC(size)  INTERNAL_MALLOC(size)
#define BSTARTREE_FREE(pointer) INTERNAL_FREE(pointer)

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* B*树核心(B*树容器) */
struct BStarTreeContainer {
    struct BStarTreeContainer *ThisParent;   //父亲
    struct BStarTreeContainer *LeftBrother;  //左兄弟
    struct BStarTreeContainer *RightBrother; //右兄弟
    void    **ChildOrDataSet;   //数据元集或节点集
    uint32_t *KeySet;           //关键字集
    uint32_t  CurrentNumber;    //当前数据集数据量
};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* B*树核心(B*树集合) */
struct BStarTreeSet {
    struct BStarTreeContainer *Root;
    uint32_t ArrayLength;   //节点和叶子数组限制
    uint32_t Criterion;     //节点分裂值
    uint32_t NodeNumber;
    uint32_t (*GetDataKey)(void *Data);
};
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 核心转义 */
typedef struct BStarTreeContainer BStar_Node;   //B*树节点
typedef struct BStarTreeSet       BStar_Tree;   //B*树集合
//计算叶子节点实体的关键字
typedef uint32_t (*BStar_GetDataKey)(void *Data);
//
typedef enum {NODE_COMMON = 0, NODE_LEAVES = 1} BStar_State;
typedef enum {ERROR = 2}                        BStar_Error;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* B*树集合抽象方法 */

STATIC_INLINE void BStarTreeSetRoot(BStar_Tree *Tree, BStar_Node *Root)
{
    Tree->Root = Root;
}

STATIC_INLINE BStar_Node * BStarTreeGetRoot(BStar_Tree *Tree)
{
    return Tree->Root;
}

STATIC_INLINE void BStarTreeSetArrayLength(BStar_Tree *Tree, uint32_t Length)
{
    Tree->ArrayLength = Length;
}

STATIC_INLINE uint32_t BStarTreeGetArrayLength(BStar_Tree *Tree)
{
    return Tree->ArrayLength;
}

STATIC_INLINE void BStarTreeSetDataKey(BStar_Tree *Tree, BStar_GetDataKey Key)
{
    Tree->GetDataKey = Key;
}

STATIC_INLINE uint32_t BStarTreeDataKey(BStar_Tree *Tree, void *Data)
{
    return Tree->GetDataKey(Data);
}

STATIC_INLINE void BStarTreeSetCriterion(BStar_Tree *Tree, uint32_t Criterion)
{
    Tree->Criterion = Criterion;
}

STATIC_INLINE uint32_t BStarTreeGetCriterion(BStar_Tree *Tree)
{
    return Tree->Criterion;
}

STATIC_INLINE void BStarTreeSetNodeNumber(BStar_Tree *Tree, uint32_t Number)
{
    Tree->NodeNumber = Number;
}

STATIC_INLINE uint32_t BStarTreeGetNodeNumber(BStar_Tree *Tree)
{
    return Tree->NodeNumber;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* B*树容器抽象方法(设置) */

STATIC_INLINE void BStarNodeSetParent(BStar_Node *Node, BStar_Node *Parent)
{
    Node->ThisParent = Parent;
}

STATIC_INLINE void BStarNodeSetLeft(BStar_Node *Node, BStar_Node *Brother)
{
    Node->LeftBrother = Brother;
}

STATIC_INLINE void BStarNodeSetRight(BStar_Node *Node, BStar_Node *Brother)
{
    Node->RightBrother = Brother;
}

STATIC_INLINE void BStarNodeSetKeySet(BStar_Node *Node, uint32_t *KeySet)
{
    Node->KeySet = KeySet;
}

STATIC_INLINE void BStarNodeSetCurrentNumber(BStar_Node *Node, uint32_t Number)
{
    Node->CurrentNumber = Number;
}

STATIC_INLINE void BStarNodeSetChildOrDataSet(BStar_Node *Node, void **Set)
{
    VOID_STAR_TYPE Result = 0;
    
    Result |= (VOID_STAR_TYPE)(Node->ChildOrDataSet);
    Result &= (VOID_STAR_TYPE)(1);
    Result |= (VOID_STAR_TYPE)(Set);

    Node->ChildOrDataSet = (void *)Result;
}

STATIC_INLINE void BStarNodeSetState(BStar_Node *Node, uint32_t State)
{
    VOID_STAR_TYPE Result = 0;
    
    Result |= (VOID_STAR_TYPE)(Node->ChildOrDataSet);
    Result &= (VOID_STAR_TYPE)(~1);
    
    if (State  == NODE_LEAVES)
        Result |= (VOID_STAR_TYPE)NODE_LEAVES;
    if (State  == NODE_COMMON)
        Result |= (VOID_STAR_TYPE)NODE_COMMON;

    Node->ChildOrDataSet = (void *)Result;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* B*树容器抽象方法(获取) */

STATIC_INLINE void * BStarNodeGetParent(BStar_Node *Node)
{
    return Node->ThisParent;
}

STATIC_INLINE void * BStarNodeGetLeft(BStar_Node *Node)
{
    return Node->LeftBrother;
}

STATIC_INLINE void * BStarNodeGetRight(BStar_Node *Node)
{
    return Node->RightBrother;
}

STATIC_INLINE uint32_t * BStarNodeGetKeySet(BStar_Node *Node)
{
    return Node->KeySet;
}

STATIC_INLINE uint32_t BStarNodeGetCurrentNumber(BStar_Node *Node)
{
    return Node->CurrentNumber;
}

STATIC_INLINE void ** BStarNodeGetChildOrDataSet(BStar_Node *Node)
{
    VOID_STAR_TYPE Result = 0;
    
    Result |= (VOID_STAR_TYPE)(Node->ChildOrDataSet);
    Result &= (VOID_STAR_TYPE)(~1);
    
    return (void **)Result;
}

STATIC_INLINE uint32_t BStarNodeGetState(BStar_Node *Node)
{
    VOID_STAR_TYPE Result = 0;
    
    Result |= (VOID_STAR_TYPE)(Node->ChildOrDataSet);
    Result &= (VOID_STAR_TYPE)(1);
    
    if (Result == NODE_LEAVES)
        return NODE_LEAVES;
    if (Result == NODE_COMMON)
        return NODE_COMMON;

    return ERROR;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 辅助定制宏,美化代码,剔除边缘逻辑 */
#define RETURN_EMPTY(target) if (target) return;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 创建一个动态节点 */
static BStar_Node * BStar_CreateNode(BStar_Tree *Tree, uint32_t State)
{
    ERROR_PRINT(Tree == NULL, "BStar_CreateNode: Tree");
    ERROR_PRINT(BStarTreeGetArrayLength(Tree) < 3, "BStar_CreateNode: Tree Zone");
    //分配空间计算
    uint32_t Index = 0;
    uint32_t Zone1 = sizeof(BStar_Node);
    uint32_t Zone2 = sizeof(uint32_t);
    uint32_t Zone3 = sizeof(void *);
    Zone2 *= BStarTreeGetArrayLength(Tree);//它是一个数组
    Zone3 *= BStarTreeGetArrayLength(Tree);//它是一个数组
    //1.创建节点,创建关键字集,创建数据元集或节点集
    BStar_Node *Node = BSTARTREE_MALLOC(Zone1);
    uint32_t *KeySet = BSTARTREE_MALLOC(Zone2);
    void  *DataChild = BSTARTREE_MALLOC(Zone3);
    //2.检查分配空间
    if (Node == NULL || KeySet == NULL || DataChild == NULL) {
        BSTARTREE_FREE(DataChild);
        BSTARTREE_FREE(KeySet);
        BSTARTREE_FREE(Node);
        //利用外界的容错检查简化此处
        ERROR_PRINT(NULL == NULL, "BStar_CreateNode: malloc Node");
    }
    //3.初始化该节点,直接清零
    for (Index = 0; Index < Zone1; Index++)
        ((uint8_t *)Node)[Index] = 0;
    for (Index = 0; Index < Zone2; Index++)
        ((uint8_t *)KeySet)[Index] = 0;
    for (Index = 0; Index < Zone3; Index++)
        ((uint8_t *)DataChild)[Index] = 0;
    //4.关键字集绑定,数据元集或节点集绑定
    BStarNodeSetKeySet(Node, KeySet);
    BStarNodeSetState(Node, State);
    BStarNodeSetChildOrDataSet(Node, DataChild);
    return Node;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 销毁一个动态节点 */
static void BStar_DestroyNode(void *Node)
{
    BSTARTREE_FREE(BStarNodeGetChildOrDataSet(Node));
    BSTARTREE_FREE(BStarNodeGetKeySet(Node));
    BSTARTREE_FREE(Node);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 辅助功能开始 */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 二分搜索 */
static void BStar_BinarySearch(uint32_t KeyValue, uint32_t *KeySet,
                               uint32_t Boundary, int32_t *Left, int32_t *Right)
{
    ERROR_PRINT(Boundary >= 2,  "BStar_BinarySearch: Boundary");
    ERROR_PRINT(KeySet == NULL, "BStar_BinarySearch: KeySet");
    ERROR_PRINT(*Left > *Right, "BStar_BinarySearch: Left>Right");
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
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 迭代更新从指定节点到根节点的索引 */
static void BStar_KeyValueUpdate(BStar_Node *Node, uint32_t KeyValueNew,
                                                   uint32_t KeyValueOld)
{
    RETURN_EMPTY(Node == NULL || KeyValueNew == KeyValueOld);
    //迭代向上更新对应数据元,因为它是去往最近最小的项
    Node = BStarNodeGetParent(Node);
    while (Node != NULL && KeyValueNew != KeyValueOld) {
        //遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
        uint32_t *KeySet        = BStarNodeGetKeySet(Node);
        uint32_t  CurrentNumber = BStarNodeGetCurrentNumber(Node);
        int32_t Left  = 0;
        int32_t Right = CurrentNumber - 1;
        int32_t Index = -1;
        //查找更新father中对应数据
        if (KeyValueNew > KeyValueOld) {
            BStar_BinarySearch(KeyValueOld, KeySet, 1, &Left, &Right);
            Index = (Left < 0) ? 0 : Left;

        }
        if (KeyValueNew < KeyValueOld) {
            BStar_BinarySearch(KeyValueOld, KeySet, 0, &Left, &Right);
            Index = (Right > CurrentNumber - 1) ? CurrentNumber - 1 : Right;
        }
        
        //注意:必须要保证更新正确的父亲的关键字索引
        ERROR_PRINT(KeySet[Index] != KeyValueOld, "BStar_KeyValueUpdate");
        KeyValueOld = KeySet[Index];
        KeySet[Index] = KeyValueNew;
        //迭代到下一层,如果可以的话
        Node = BStarNodeGetParent(Node);
        RETURN_EMPTY(Index != 0);
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 从指定节点迁移批量数据到另一节点 */
static void BStar_MigrateData(BStar_Tree *Tree, BStar_Node *Node1, BStar_Node *Node2,
                              uint32_t MigrateNumber, uint32_t MigrateState)
{
    RETURN_EMPTY(Tree == NULL || Node1 == NULL || Node2 == NULL);
    //指定将其进行数据迁移:data1 ==> data2
    //MigrateState == 0,将data1后migrate_number数据加入到data2的前面
    //MigrateState == 1,将data1前migrate_number数据加入到data2的后面
    //注意:跨节点迁移时其对父亲的索引要一并迁移
    int32_t Index = 0, Temp = 0;//迁移所用到的索引
    void **DataSet1 = BStarNodeGetChildOrDataSet(Node1);
    void **DataSet2 = BStarNodeGetChildOrDataSet(Node2);
    uint32_t  Length         = BStarTreeGetArrayLength(Tree);
    uint32_t *KeySet1        = BStarNodeGetKeySet(Node1);
    uint32_t *KeySet2        = BStarNodeGetKeySet(Node2);
    uint32_t  Node1State     = BStarNodeGetState(Node1);
    uint32_t  Node2State     = BStarNodeGetState(Node2);
    uint32_t  CurrentNumber1 = BStarNodeGetCurrentNumber(Node1);
    uint32_t  CurrentNumber2 = BStarNodeGetCurrentNumber(Node2);
    //要迁移migrate项数据(极限检测)
    ERROR_PRINT(MigrateState >= 2, "BStar_MigrateData: MigrateState");
    ERROR_PRINT(Node1State != Node2State, "BStar_MigrateData: NodeState");
    ERROR_PRINT(CurrentNumber1 - MigrateNumber < 0, "BStar_MigrateData: 1");
    ERROR_PRINT(CurrentNumber2 + MigrateNumber > Length, "BStar_MigrateData: 2");

    //根据实际情况继续不同的迁移动作
    if (MigrateState == 0) {
        //1.先漂移data2的数据
        if (CurrentNumber2 != 0)
            for (Index = CurrentNumber2 - 1; Index >= 0; Index--) {
                KeySet2[Index + MigrateNumber]  = KeySet2[Index];
                DataSet2[Index + MigrateNumber] = DataSet2[Index];
            }
        //2.data1的数据迁移到data2
        for (Index = 0; Index < MigrateNumber; Index++) {
            KeySet2[Index]  = KeySet1[Index + CurrentNumber1 - MigrateNumber];
            DataSet2[Index] = DataSet1[Index + CurrentNumber1 - MigrateNumber];
            Temp = Index;
            if (Node2State != NODE_LEAVES && DataSet2[Temp] != NULL)
                BStarNodeSetParent((BStar_Node*)(DataSet2[Temp]), Node2);
        }
    }
    if (MigrateState == 1) {
        //1.data1的数据迁移到data2
        for (Index = 0; Index < MigrateNumber; Index++) {
            KeySet2[Index + CurrentNumber2]  = KeySet1[Index];
            DataSet2[Index + CurrentNumber2] = DataSet1[Index];
            Temp = Index + CurrentNumber2;
            if (Node1State != NODE_LEAVES && DataSet2[Temp] != NULL)
                BStarNodeSetParent((BStar_Node*)(DataSet2[Temp]), Node2);
        }
        //2.后漂移data1的数据
        if (CurrentNumber1 - MigrateNumber != 0)
            for (Index = 0; Index < CurrentNumber1 - MigrateNumber; Index++) {
                KeySet1[Index] = KeySet1[Index + MigrateNumber];
                DataSet1[Index] = DataSet1[Index + MigrateNumber];
            }
    }
    //更新data1和data2的计数量
    BStarNodeSetCurrentNumber(Node1, CurrentNumber1 - MigrateNumber);
    BStarNodeSetCurrentNumber(Node2, CurrentNumber2 + MigrateNumber);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 迭代搜索到叶节点,根据特定关键字 */
static void BStar_SearchNode(BStar_Tree *Tree, BStar_Node **Node, uint32_t KeyValue)
{
    RETURN_EMPTY(Tree == NULL || Node == NULL);
    BStar_Node *Root = BStarTreeGetRoot(Tree);
    //从根节点一直走到叶子节点即可
    while (Root != NULL && BStarNodeGetState(Root) == NODE_COMMON) {
        //根节点至少是有元素,此时需要向下搜索
        uint32_t *KeySet        = BStarNodeGetKeySet(Root);
        void    **ChildSet      = BStarNodeGetChildOrDataSet(Root);
        uint32_t  CurrentNumber = BStarNodeGetCurrentNumber(Root);
        //遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
        int32_t Left  = 0;
        int32_t Right = CurrentNumber - 1;
        //二分搜索,查找时有效项是left
        BStar_BinarySearch(KeyValue, KeySet, 1, &Left, &Right);
        Left = (Left < 0) ? 0 : Left;
        //判断有效元,因为查找时可能出现溢出的情况
        Root = (BStar_Node *)(ChildSet[Left]);
    }
    *Node = Root;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 在一个指定节点中插入一个项 */
static void BStar_InsertData(BStar_Node *Node, uint32_t State, uint32_t *Index,
                                               uint32_t KeyValue, void  *Item)
{
    RETURN_EMPTY(Node == NULL || Index == NULL || Item == NULL || State >= 2);
    uint32_t *KeySet        = BStarNodeGetKeySet(Node);
    void    **NextSet       = BStarNodeGetChildOrDataSet(Node);
    uint32_t  CurrentNumber = BStarNodeGetCurrentNumber(Node);
    //1.遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
    int32_t Left = 0, Right = (int32_t)CurrentNumber - 1, Index1 = 0;
    BStar_BinarySearch(KeyValue, KeySet, State, &Left, &Right);
    *Index = (Right < 0) ? 0 : Right;//插入时,有效项是right
    //2.需要将后续的部分迁移到下一项,为新入项腾出一个空间
    if (*Index < CurrentNumber) {
        for (Index1 = CurrentNumber - 1; Index1 >= (int32_t)(*Index); Index1--) {
            //当前项覆盖后一项,直到==index覆盖完毕
            KeySet[Index1 + 1]  = KeySet[Index1];
            NextSet[Index1 + 1] = NextSet[Index1];
        }
    }
    //3.将数据加入到指定位置即可
    NextSet[*Index] = Item;
    KeySet[*Index]  = KeyValue;
    BStarNodeSetCurrentNumber(Node, CurrentNumber + 1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 从一个指定节点中移除一个项 */
static void BStar_RemoveData(BStar_Node *Node, uint32_t State, uint32_t *Index,
                                               uint32_t KeyValue, void **Item)
{
    RETURN_EMPTY(Node == NULL || Index == NULL || Item == NULL || State >= 2);
    uint32_t *KeySet        = BStarNodeGetKeySet(Node);
    void    **NextSet       = BStarNodeGetChildOrDataSet(Node);
    uint32_t  CurrentNumber = BStarNodeGetCurrentNumber(Node);
    //如果是无效的删除的话,直接回退即可
    *Item == NULL;
    RETURN_EMPTY(CurrentNumber == 0);
    int32_t Left = 0, Right = (int32_t)CurrentNumber - 1, Index1 = 0;
    //1.遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
    BStar_BinarySearch(KeyValue, KeySet, State, &Left, &Right);
    *Index = ((State == 1) ? ((Left < 0) ? 0 : Left) :
             ((State == 0) ? (Left + 1) : 0));//删除时,有效项是left
    ERROR_PRINT(KeySet[*Index] != KeyValue, "BStar_RemoveData: KeyValue");
    //2.记录其数据
    *Item = NextSet[*Index];
    //3.需要将后续的部分迁移到前一项,为其覆盖一个空间
    if (CurrentNumber != 1 && *Index < CurrentNumber)
        for (Index1 = *Index; Index1 <= (int32_t)(CurrentNumber); Index1++) {
            //当前项覆盖后一项,直到==index覆盖完毕
            KeySet[Index1]  = KeySet[Index1 + 1];
            NextSet[Index1] = NextSet[Index1 + 1];
        }
    BStarNodeSetCurrentNumber(Node, CurrentNumber - 1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 从一个指定节点中搜索一个项 */
static void BStar_SearchData(BStar_Node *Node, uint32_t State, uint32_t *Index,
                                               uint32_t KeyValue, void **Item)
{
    RETURN_EMPTY(Node == NULL || Index == NULL || Item == NULL || State >= 2);
    uint32_t *KeySet        = BStarNodeGetKeySet(Node);
    void    **NextSet       = BStarNodeGetChildOrDataSet(Node);
    uint32_t  CurrentNumber = BStarNodeGetCurrentNumber(Node);
    //保证搜索时,内部至少有一个项
    *Item = NULL;
    RETURN_EMPTY(CurrentNumber == 0);
    //遍历当前数据关键字集,定位到恰好遇到一项比它大或退出
    int32_t Left = 0, Right = CurrentNumber - 1;
    BStar_BinarySearch(KeyValue, KeySet, State, &Left, &Right);
    *Index = ((State == 1) ? ((Left < 0) ? 0 : Left) :
             ((State == 0) ? (Left + 1) : 0));//搜索时,有效项是left
    ERROR_PRINT(KeySet[*Index] != KeyValue, "BStar_SearchData: KeyValue");
    //记录其数据
    *Item = NextSet[*Index];
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 辅助功能结束 */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 单次插入函数 */
static void BStar_InsertOnly(BStar_Tree *Tree, BStar_Node **Node, void *Data)
{
    ERROR_PRINT(Tree == NULL || Node == NULL || Data == NULL, "only_insert");
    //1.计算关键字
    uint32_t KeyValue = BStarTreeDataKey(Tree, Data);
    //2.如果是插入到空树中时或者插入到叶子节点时
    *Node = BStarTreeGetRoot(Tree);
    //3.确认是否是初次插入,不存在项
    if (*Node == NULL) {
        //为其生成一个空间,设置其为叶子节点
        BStarTreeSetRoot(Tree, BStar_CreateNode(Tree, NODE_LEAVES));
        *Node =  BStarTreeGetRoot(Tree);
        RETURN_EMPTY(*Node == NULL);
    }
    //4.迭代搜索到叶子节点
    BStar_SearchNode(Tree, Node, KeyValue);
    //5.数据元加入到叶子节点
    int32_t Index = -1;
    BStar_InsertData(*Node, 1, &Index, KeyValue, Data);
    //6.插入的节点是叶子节点的首节点,修复它所产生的影响
    if (Index == 0) {
        uint32_t *KeySet = BStarNodeGetKeySet(*Node);
        BStar_KeyValueUpdate(*Node, KeySet[0], KeySet[1]);
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 插入调整函数 */
static void BStar_InsertAdjust(BStar_Tree *Tree, BStar_Node *Node)
{
    RETURN_EMPTY(Tree == NULL || Node == NULL);
    uint32_t Index = 0;
    uint32_t State = 0;
    uint32_t *KeySet1 = NULL, KeyValue1 = 0;
    uint32_t *KeySet2 = NULL, KeyValue2 = 0;
    uint32_t *KeySet3 = NULL, KeyValue3 = 0;
    uint32_t Length         = BStarTreeGetArrayLength(Tree);
    uint32_t Criterion      = BStarTreeGetCriterion(Tree);
    uint32_t CurrentNumber  = BStarNodeGetCurrentNumber(Node);
    BStar_Node *Parent       = NULL;
    BStar_Node *LeftBrother  = NULL;
    BStar_Node *RightBrother = NULL;
    //插入时导致的节点超出
    //如果左右兄弟只要有一个存在盈余空间就无需分裂
    //如果左右兄弟都负重不堪,那么往左边分裂和往右边分裂(这是一个问题)
    //这里直接按正常习惯往右边分裂,但是到底需要有向左边分裂的必要性,是否
    //是叶节点分裂还是非叶子节点分裂(情况是一致的)
    while (CurrentNumber >= Criterion) {
        //先获取它与周围节点的关系
        Parent       = BStarNodeGetParent(Node);
        LeftBrother  = BStarNodeGetLeft(Node);
        RightBrother = BStarNodeGetRight(Node);
        State        = BStarNodeGetState(Node);
        //情况1:如果右兄弟有盈余空间
        if (RightBrother != NULL &&
            BStarNodeGetCurrentNumber(RightBrother) < Criterion) {
            //1.获取右兄弟在父亲那的记录
            KeySet3   = BStarNodeGetKeySet(RightBrother);
            KeyValue3 = KeySet3[0];
            //2.将一部分数据迁移至右兄弟
            BStar_MigrateData(Tree, Node, RightBrother, Length - Criterion, 0);
            //3.迭代更新从指定节点到根节点的索引
            Node = RightBrother;
            BStar_KeyValueUpdate(Node, KeySet3[0], KeyValue3);
            return;
        }
        
        //情况2:如果左兄弟有盈余空间(它应该只做少部分数据承担者)
        //添加该种情况的话可以更大化的利用空间
        if (LeftBrother != NULL &&
            BStarNodeGetCurrentNumber(LeftBrother) < Criterion) {
            //1.获取自己在父亲那的记录
            KeySet1   = BStarNodeGetKeySet(Node);
            KeyValue1 = KeySet1[0];
            //2.将一部分数据迁移至左兄弟
            BStar_MigrateData(Tree, Node, LeftBrother, Length - Criterion, 1);
            //3.迭代更新从指定节点到根节点的索引
            BStar_KeyValueUpdate(Node, KeySet1[0], KeyValue1);
            return;
        }
        
        //情况3:如果没有兄弟节点时
        if (RightBrother == NULL) {
            //1.创建兄弟节点
            RightBrother = BStar_CreateNode(Tree, State);
            ERROR_PRINT(RightBrother == NULL, "BStar_InsertAdjust: RightBrother");
            //2.为自己设置兄弟,为兄弟设置兄弟
            BStarNodeSetRight(Node, RightBrother);
            BStarNodeSetLeft(RightBrother, Node);
            //3.将本节点部分数据拷贝到右兄弟中去
            BStar_MigrateData(Tree, Node, RightBrother, Length - Criterion, 0);
            //4.如果没有父亲要生成父亲,如果有父亲要添加兄弟节点
            if (Parent == NULL) {
                //1.创建一个父亲
                Parent = BStar_CreateNode(Tree, NODE_COMMON);
                ERROR_PRINT(Parent == NULL, "BStar_InsertAdjust: Parent");
                //2.为自己设置新父亲
                BStarNodeSetParent(Node, Parent);
                //3.新生成父亲时需要更新根节点
                BStarTreeSetRoot(Tree, Parent);
                //4.将自己加入到新父亲中
                KeySet1 = BStarNodeGetKeySet(Node);
                BStar_InsertData(Parent, 1, &Index, KeySet1[0], (void *)Node);
            }
            //5.为兄弟设置父亲
            BStarNodeSetParent(RightBrother, Parent);
            //6.将兄弟插入到父亲中
            KeySet3 = BStarNodeGetKeySet(RightBrother);
            BStar_InsertData(Parent, 1, &Index, KeySet3[0], (void *)RightBrother);
            //7.迭代到上层调整
            CurrentNumber = BStarNodeGetCurrentNumber(Parent);
            Node = Parent;
            continue;
        }
        
        //情况4:自己和左右兄弟都是负重不堪
        //1.创建一个新节点
        BStar_Node *Temp = BStar_CreateNode(Tree, State);
        //2.为其设置它的父亲
        BStarNodeSetParent(Temp, Parent);
        //3.先迁移自己的数据
        BStar_MigrateData(Tree, Node, Temp, Length - Criterion, 0);
        //4.获取右兄弟在父亲那的记录
        KeySet3 = BStarNodeGetKeySet(RightBrother);
        KeyValue3 = KeySet3[0];
        //5.将右兄弟一部分数据迁移
        BStar_MigrateData(Tree, RightBrother, Temp, Length - Criterion, 1);
        //6.迭代更新从指定节点到根节点的索引
        BStar_KeyValueUpdate(RightBrother, KeySet3[0], KeyValue3);
        //7.更新兄弟关系
        BStarNodeSetLeft(RightBrother, Temp);
        BStarNodeSetLeft(Temp, Node);
        BStarNodeSetRight(Node, Temp);
        BStarNodeSetRight(Temp, RightBrother);
        //9.新兄弟加入到父亲中
        KeySet1 = BStarNodeGetKeySet(Node);
        KeySet2 = BStarNodeGetKeySet(Temp);
        //注意:默认是在其最后产生分裂
        //但新兄弟在父亲的位置应该只能出现在当前节点之后
        State = ((KeySet1[0] <  KeySet2[0]) ? 0 :
                ((KeySet1[0] >= KeySet2[0]) ? 1 : 0));
        BStar_InsertData(Parent, State, &Index, KeySet2[0], (void *)Temp);
        //10.迭代到上层调整
        CurrentNumber = BStarNodeGetCurrentNumber(Parent);
        Node = Parent;
        continue;
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 单次删除函数 */
static void BStar_RemoveOnly(BStar_Tree *Tree, BStar_Node **Node,
                             uint32_t KeyValue, void **Data)
{
    ERROR_PRINT(Tree == NULL || Data == NULL || Node == NULL, "BStar_RemoveOnly");
    *Node = BStarTreeGetRoot(Tree);
    //确认是否是初次删除,不存在项
    RETURN_EMPTY(*Node == NULL);
    int32_t Index;
    //从根节点开始向下迭代
    BStar_SearchNode(Tree, Node, KeyValue);
    //数据元从叶子节点中移除
    BStar_RemoveData(*Node, 1, &Index, KeyValue, Data);
    //调zheng一下删除该数所造成的影响
    uint32_t *KeySet = BStarNodeGetKeySet(*Node);
    if (Index != 0)
        return;
    BStar_KeyValueUpdate(*Node, KeySet[0], KeyValue);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 删除调整函数 */
static void BStar_RemoveAdjust(BStar_Tree *Tree, BStar_Node *Node)
{
    RETURN_EMPTY(Tree == NULL || Node == NULL);
    //先获取它与周围节点的关系
    BStar_Node *Parent          = BStarNodeGetParent(Node);
    BStar_Node *LeftBrother     = BStarNodeGetLeft(Node);
    BStar_Node *RightBrother    = BStarNodeGetRight(Node);
    uint32_t    CurrentNumber   = BStarNodeGetCurrentNumber(Node);
    uint32_t    State           = BStarNodeGetState(Node);
    //情况1:节点被清空(正常情况下,节点被清空之前会做合并的动作)
    if (CurrentNumber == 0 && State == NODE_LEAVES) {
        //添加必要的检查
        ERROR_PRINT(Parent       != NULL, "BStar_RemoveAdjust: Parent");
        ERROR_PRINT(LeftBrother  != NULL, "BStar_RemoveAdjust: LeftBrother");
        ERROR_PRINT(RightBrother != NULL, "BStar_RemoveAdjust: RightBrother");
        //它仅会出现在一种情况,就是清空树时,直接删除掉该节点就好
        BStarTreeSetRoot(Tree, NULL);
        BStar_DestroyNode(Node);
        return;
    }
    
    void    *Data      = NULL;
    uint32_t Index     = -1;
    uint32_t Length    = BStarTreeGetArrayLength(Tree);
    uint32_t Criterion = BStarTreeGetCriterion(Tree);
    //情况2:删除导致的节点缺失,可能需要合并
    //合并仅向右方向合并,向左兄弟合并无法很好的迭代更新
    //而数据的提供优先向左兄弟索要,它更容易被溢出
    //是叶节点合并还是非叶子节点合并(情况是一致的)
    while (Node != NULL && CurrentNumber < Criterion) {
        ERROR_PRINT(CurrentNumber == 0, "BStar_RemoveAdjust: CurrentNumber");
        RETURN_EMPTY(CurrentNumber == 0);
        uint32_t *KeySet1 = NULL, KeyValue1 = 0;
        uint32_t *KeySet2 = NULL, KeyValue2 = 0;
        uint32_t *KeySet3 = NULL, KeyValue3 = 0;
        //获取需要调整的节点的兄弟节点和父节点,以及孩子数量
        Parent        = BStarNodeGetParent(Node);
        LeftBrother   = BStarNodeGetLeft(Node);
        RightBrother  = BStarNodeGetRight(Node);
        
        //情况2.1:没有左右兄弟(孤立的节点)
        if (LeftBrother == NULL && RightBrother == NULL) {
            //合并时产生的孤立节点,如果它的数据量只有一个
            if (CurrentNumber == 1 && State == NODE_COMMON) {
                void **DataSet = BStarNodeGetChildOrDataSet(Node);
                //删除本体,让其孩子上位
                BStarTreeSetRoot(Tree, (BStar_Node *)(DataSet[0]));
                BStar_DestroyNode(Node);
                //新的根节点被设置为空
                BStarNodeSetParent(BStarTreeGetRoot(Tree), NULL);
            }
            return;
        }
        
        //情况2.2:左兄弟数据充足
        if (LeftBrother != NULL && CurrentNumber +
            BStarNodeGetCurrentNumber(LeftBrother) >= Length) {
            //1.向左兄弟索要一个数据并插入到自己中
            BStar_MigrateData(Tree, LeftBrother, Node, 1, 0);
            //2.迭代更新从指定节点到根节点的索引
            KeySet1 = BStarNodeGetKeySet(Node);
            BStar_KeyValueUpdate(Node, KeySet1[0], KeySet1[1]);
            return;
        }
        
        //情况2.3:右兄弟数据充足
        if (RightBrother != NULL && CurrentNumber +
            BStarNodeGetCurrentNumber(RightBrother) >= Length) {
            //迁移前获取节点信息
            KeySet3 = BStarNodeGetKeySet(RightBrother);
            KeyValue3 = KeySet3[0];
            //1.向右兄弟索要一个数据并插入到自己中
            BStar_MigrateData(Tree, RightBrother, Node, 1, 1);
            //2.迭代更新从指定节点到根节点的索引
            Node = RightBrother;
            BStar_KeyValueUpdate(Node, KeySet3[0], KeyValue3);
            return;
        }
        
        //情况2.4:自己和右兄弟都贫困
        if (RightBrother != NULL && CurrentNumber +
            BStarNodeGetCurrentNumber(RightBrother) < Length) {
            //迁移前获取节点信息
            KeySet1 = BStarNodeGetKeySet(Node);
            KeySet3 = BStarNodeGetKeySet(RightBrother);
            KeyValue1 = KeySet1[0];
            KeyValue3 = KeySet3[0];
            //多多少少有些尴尬,因为对于多key的处理仅关系首项与末项
            //这里在向上迭代调整时会,使得node可能成为第二项
            //这很显然是设计上存在的缺失,不能将俩种情况合并
            if (KeyValue1 != KeyValue3) {
                //1.将右兄弟的数据迁移到自己
                CurrentNumber = BStarNodeGetCurrentNumber(RightBrother);
                BStar_MigrateData(Tree, RightBrother, Node, CurrentNumber, 1);
                //2.删除右兄弟
                Parent        = BStarNodeGetParent(RightBrother);
                RightBrother  = BStarNodeGetRight(RightBrother);
                BStar_DestroyNode(BStarNodeGetRight(Node));
                //3.更新兄弟关系
                BStarNodeSetRight(Node, RightBrother);
                if (RightBrother != NULL)
                    BStarNodeSetLeft(RightBrother, Node);
            }
            if (KeyValue1 == KeyValue3) {
                //1.将自己的数据迁移到右兄弟
                BStar_MigrateData(Tree, Node, RightBrother, CurrentNumber, 0);
                //2.删除自己
                BStar_DestroyNode(Node);
                //3.更新兄弟关系
                BStarNodeSetLeft(RightBrother, LeftBrother);
                if (LeftBrother != NULL)
                    BStarNodeSetRight(LeftBrother, RightBrother);
            }
            //4.删除在父亲中的记录
            BStar_RemoveData(Parent, 0, &Index, KeyValue3, &Data);
            //5.清除父亲节点因删除照成的影响
            if (Index == 0) {
                KeySet3 = BStarNodeGetKeySet(Parent);
                BStar_KeyValueUpdate(Parent, KeySet3[0], KeyValue3);
            }
            //6.迭代到上层调整
            CurrentNumber = BStarNodeGetCurrentNumber(Parent);
            State         = BStarNodeGetState(Parent);
            Node          = Parent;
            continue;
        }
    
        //情况2.5:自己和左兄弟都贫困
        if (LeftBrother != NULL && CurrentNumber +
            BStarNodeGetCurrentNumber(LeftBrother) < Length) {
            //迁移前获取节点信息
            KeySet1 = BStarNodeGetKeySet(Node);
            KeyValue1 = KeySet1[0];
            //1.将自己的数据迁移到左兄弟
            BStar_MigrateData(Tree, Node, LeftBrother, CurrentNumber, 1);
            //2.删除自己
            BStar_DestroyNode(Node);
            //3.更新兄弟关系
            BStarNodeSetRight(LeftBrother, RightBrother);
            if (RightBrother != NULL)
                BStarNodeSetLeft(RightBrother, LeftBrother);
            //4.删除自己在父亲中的记录
            BStar_RemoveData(Parent, 1, &Index, KeyValue1, &Data);
            //5.清除父亲节点因删除照成的影响
            if (Index == 0) {
                KeySet1 = BStarNodeGetKeySet(Parent);
                BStar_KeyValueUpdate(Parent, KeySet1[0], KeyValue1);
            }
            //6.迭代到上层调整
            CurrentNumber = BStarNodeGetCurrentNumber(Parent);
            State          = BStarNodeGetState(Parent);
            Node           = Parent;
            continue;
        }
        
        //也许会存在导致其节点亏空的情况,意味着它没有兄弟为其解决苦难
        //但我们依然相信,我们希望的美好的事情会发生
        break;
    }
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 单次查找函数 */
static void BStar_SearchOnly(BStar_Tree *Tree, uint32_t KeyValue, void **Data)
{
    ERROR_PRINT(Tree == NULL || Data == NULL, "BStar_SearchOnly");
    BStar_Node *BST_SearchNode = BStarTreeGetRoot(Tree);
    //确认是否是初次删除,不存在项
    *Data = NULL;
    RETURN_EMPTY(BST_SearchNode == NULL);
    int32_t Index;
    //从根节点开始向下迭代
    BStar_SearchNode(Tree, &BST_SearchNode, KeyValue);
    RETURN_EMPTY(BST_SearchNode == NULL);
    //找到了目标节点,确定目标节点是否存在目标
    BStar_SearchData(BST_SearchNode, 0, &Index, KeyValue, Data);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 上层接口封装 */

void BST_InsertNode(void *Tree, void *Data)
{
    BStar_Node *Node;
    BStar_InsertOnly((BStar_Tree *)Tree, &Node, Data);
    BStar_InsertAdjust((BStar_Tree *)Tree, Node);
    uint32_t cuttent_number;
    cuttent_number = BStarTreeGetNodeNumber((BStar_Tree *)Tree);
    BStarTreeSetNodeNumber((BStar_Tree *)Tree, cuttent_number + 1);
}

void BST_RemoveNode(void *Tree, void **Data, uint32_t KeyValue)
{
    BStar_Node *Node;
    BStar_RemoveOnly((BStar_Tree *)Tree, &Node, KeyValue, Data);
    BStar_RemoveAdjust((BStar_Tree *)Tree, Node);
    RETURN_EMPTY(*Data != NULL);
    uint32_t cuttent_number;
    cuttent_number = BStarTreeGetNodeNumber((BStar_Tree *)Tree);
    BStarTreeSetNodeNumber((BStar_Tree *)Tree, cuttent_number - 1);
}

void BST_SearchNode(void *Tree, void **Data, uint32_t KeyValue)
{
    BStar_SearchOnly((BStar_Tree *)Tree, KeyValue, Data);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 额外的辅助函数,为外界提供数据的建立空间 */

uint32_t BST_GetBytes_Set(void)
{
    return sizeof(BStar_Tree);
}

uint32_t BST_GetBytes_Container(void)
{
    return sizeof(BStar_Node);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 辅助动作 */

void BST_SetTree(void *Tree, uint32_t Length, data_key Key)
{
    //一旦创建便会固定,所以需要容错处理
    RETURN_EMPTY(Tree == NULL || Key == NULL || Length < 3);
    ERROR_PRINT(Length == 3, "BST_SetTree");
    ERROR_PRINT(Length / 3 * 2 + 1 >= Length, "BST_SetTree");
    BStarTreeSetArrayLength((BStar_Tree *)Tree, Length);
    BStarTreeSetCriterion((BStar_Tree *)Tree, Length / 3 * 2 + 1);
    BStarTreeSetDataKey((BStar_Tree *)Tree, Key);
    BStarTreeSetRoot((BStar_Tree *)Tree, NULL);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 窥探:数据的层序遍历并打印 */
uint32_t RST_Sequence_Traversal(void *Tree, Print Printf, uint8_t Destroy)
{
    ERROR_PRINT(Tree == NULL,   "RST_Sequence_Traversal: Tree");
    ERROR_PRINT(Printf == NULL, "RST_Sequence_Traversal: Printf");
    //层序遍历(这里使用循环队列):
    //创建一个队列(这里使用投机取巧的方法,队列极限情况下存在
    //数量最大时顶多应为满B*树的全部叶子节点数大小+1
    uint32_t Length = BStarTreeGetNodeNumber((BStar_Tree *)Tree);
    uint32_t Zone   = sizeof(BStar_Node *) * Length;
    BStar_Node **Queue  = BSTARTREE_MALLOC(Zone);
    RETURN_EMPTY(Queue == NULL);
    //获取插入的起始位置
    int32_t QueueHead = 0;
    int32_t QueueTail = 0;
    int32_t ElementNumber = 0;
    int32_t AllNumber = 0;
    
    Queue[QueueTail++] = BStarTreeGetRoot((BStar_Tree *)Tree);
    QueueTail %= Length;
    ElementNumber++;
    //这是一个循环队列
    do {
        //节点出队列
        BStar_Node *Node = Queue[QueueHead++];
        QueueHead %= Length;
        ElementNumber--;
        RETURN_EMPTY(Node == NULL);
        uint32_t *KeySet       = BStarNodeGetKeySet(Node);
        void    **DataSet      = BStarNodeGetChildOrDataSet(Node);
        uint32_t CurrentNumber = BStarNodeGetCurrentNumber(Node);
        //节点打印
        if (BStarNodeGetState(Node) == NODE_COMMON)
            Printf(CurrentNumber, KeySet, NULL);
        if (BStarNodeGetState(Node) == NODE_LEAVES)
            Printf(CurrentNumber, KeySet, DataSet);
        
        //子节点入队列
        if (BStarNodeGetState(Node) == NODE_COMMON) {
            int32_t Index = 0;
            for (Index = 0; Index < CurrentNumber; Index++) {
                Queue[QueueTail++] = (BStar_Node *)(DataSet[Index]);
                QueueTail %= Length;
                ElementNumber++;
            }
        }
        
        //计算元节点个数
        if (BStarNodeGetState(Node) == NODE_LEAVES)
            AllNumber += CurrentNumber;
        
        if (Destroy != 0)
            BStar_DestroyNode(Node);

        //队列太小,警告
        ERROR_PRINT(ElementNumber >= (int32_t)Length,
                    "RST_Sequence_Traversal: overflow");
        RETURN_EMPTY(ElementNumber >= (int32_t)Length);
    } while (ElementNumber > 0);

    return AllNumber;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
