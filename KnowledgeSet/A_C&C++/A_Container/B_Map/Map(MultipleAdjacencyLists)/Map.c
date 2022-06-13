//C std lib
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "DoubleList.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct MapVertexStruct {
    DL_Node Friend;     /* 伙伴顶点项 */
    DL_List EdgeSet1;   /* 边集合1 */
    DL_List EdgeSet2;   /* 边集合2 */
} MapVertex;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct MapEdgeStruct {
    DL_Node    EdgeSet1;    /* 边集合项 */
    DL_Node    EdgeSet2;    /* 边集合项 */
    MapVertex *Vertex1;     /* 弧的一方 */
    MapVertex *Vertex2;     /* 弧的一方 */
} MapEdge;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct MapBodyStruct {
    DL_List List;       /* 顶点集合 */
    bool  (*Confirm)(MapVertex *Vertex1, MapVertex *Vertex2);
} MapBody;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef void (*Map_MessageCheck)(char *String, MapVertex *Vertex, MapEdge *Edge);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_MessageCheckAll(MapBody *Map, Map_MessageCheck Check)
{
    /* 遍历检查节点 */
    /* 对称语义,二选其一 */
    // DL_List_Traverse_Forward(&(Map->List), Current0)
    DL_List_Traverse_Backward(&(Map->List), Current0) {
        MapVertex *Vertex = List_GetOwner(MapVertex, Friend, Current0);
        Check("\nVertex: ", Vertex, NULL);
        /* 遍历检查节点出度 */
        Check("\n\tEdgeSet2: ", NULL, NULL);
        /* 对称语义,二选其一 */
        // DL_List_Traverse_Forward(&(Vertex->EdgeSet2), Current1)
        DL_List_Traverse_Backward(&(Vertex->EdgeSet2), Current1) {
            MapEdge *Edge = List_GetOwner(MapEdge, EdgeSet2, Current1);
            Check("\n\t\tEdge: ", NULL, Edge);
            Check("\n\t\t\tVertex1: ", Edge->Vertex1, NULL);
            Check("\n\t\t\tVertex2: ", Edge->Vertex2, NULL);
        }
        /* 遍历检查节点入度 */
        Check("\n\tEdgeSet1: ", NULL, NULL);
        /* 对称语义,二选其一 */
        // DL_List_Traverse_Forward(&(Vertex->EdgeSet1), Current1)
        DL_List_Traverse_Backward(&(Vertex->EdgeSet1), Current1) {
            MapEdge *Edge = List_GetOwner(MapEdge, EdgeSet1, Current1);
            Check("\n\t\tEdge: ", NULL, Edge);
            Check("\n\t\t\tVertex1: ", Edge->Vertex1, NULL);
            Check("\n\t\t\tVertex2: ", Edge->Vertex2, NULL);
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_EdgeReset(MapEdge *Edge)
{
    Edge->Vertex1  = NULL;
    Edge->Vertex2 = NULL;
    DL_Node_Reset(&(Edge->EdgeSet1));
    DL_Node_Reset(&(Edge->EdgeSet2));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_VertexReset(MapVertex *Vertex)
{
    DL_Node_Reset(&(Vertex->Friend));
    DL_List_Reset(&(Vertex->EdgeSet1));
    DL_List_Reset(&(Vertex->EdgeSet2));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_BodyReset(MapBody *Map)
{
    Map->Confirm = NULL;
    DL_List_Reset(&(Map->List));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_EdgeSet(MapEdge *Edge, MapVertex *Vertex1, MapVertex *Vertex2)
{
    Edge->Vertex1  = Vertex1;
    Edge->Vertex2 = Vertex2;
    DL_Node_Reset(&(Edge->EdgeSet1));
    DL_Node_Reset(&(Edge->EdgeSet2));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_BodySet(MapBody *Map, bool (*Confirm)(MapVertex *Vertex1, MapVertex *Vertex2))
{
    Map->Confirm = Confirm;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapVertex * Map_EdgeGetVertex1(MapEdge *Edge)
{
    return Edge->Vertex1;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapVertex * Map_EdgeGetVertex2(MapEdge *Edge)
{
    return Edge->Vertex2;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
uint32_t Map_GetVertexNumber(MapBody *Map)
{
    uint32_t Count = 0;
    /* 对称语义,二选其一 */
    // DL_List_Traverse_Forward(&(Map->List), Current)
    DL_List_Traverse_Backward(&(Map->List), Current)
        Count++;
    return Count;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
uint32_t Map_GetVertexIndex(MapBody *Map, MapVertex *Vertex)
{
    uint32_t Count = 0;
    /* 对称语义,二选其一 */
    // DL_List_Traverse_Forward(&(Map->List), Current)
    DL_List_Traverse_Backward(&(Map->List), Current) {
        if (&(Vertex->Friend) == Current)
            return Count;
        Count++;
    }
    return Count;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapVertex * Map_GetVertexFromIndex(MapBody *Map, uint32_t Index)
{
    uint32_t Count = 0;
    /* 对称语义,二选其一 */
    // DL_List_Traverse_Forward(&(Map->List), Current)
    DL_List_Traverse_Backward(&(Map->List), Current) {
        if (Count != Index) {
            Count++;
            continue;
        }
        return List_GetOwner(MapVertex, Friend, Current);
    }
    return NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 通过"特性值匹配"的假顶点获取图中的真顶点 */
MapVertex * Map_GetVertex(MapBody *Map, MapVertex *Vertex)
{
    /* 对称语义,二选其一 */
    //DL_List_Traverse_Forward(&(Map->List), Current)
    DL_List_Traverse_Backward(&(Map->List), Current) {
        MapVertex *Target = List_GetOwner(MapVertex, Friend, Current);
        if (Map->Confirm(Target, Vertex) == true)
            return Target;
    }
    return NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 通过俩个图中的真顶点获得真边,如果存在 */
MapEdge * Map_GetEdge(MapVertex *Vertex1, MapVertex *Vertex2)
{
    /* 对称语义,二选其一 */
    DL_Node *Current = NULL;
    /* 这是从Vertex1寻找的方式 */
    DL_List_Traverse_Forward(&(Vertex1->EdgeSet2), Current) {
        MapEdge *Target = List_GetOwner(MapEdge, EdgeSet2, Current);
        if (Target->Vertex2 == Vertex2)
            return Target;
    }
    /* 这是从Vertex2寻找的方式 */
    DL_List_Traverse_Forward(&(Vertex2->EdgeSet1), Current) {
        MapEdge *Target = List_GetOwner(MapEdge, EdgeSet2, Current);
        if (Target->Vertex1 == Vertex1)
            return Target;
    }
    return NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_EdgeAdd(MapEdge *Edge)
{
    /* 对称语义,二选其一 */
    if (Edge->Vertex1 == Edge->Vertex2)
        return;
    /* 向弧添加(Vertex1:尾部追加;Vertex2:首项插入) */
    DL_List_InsertAppend(&(Edge->Vertex1->EdgeSet2), NULL, &(Edge->EdgeSet2));
    DL_List_InsertPrepend(&(Edge->Vertex2->EdgeSet1), NULL, &(Edge->EdgeSet1));
    /* 向弧添加(Vertex1:首项插入;Vertex2:尾部追加) */
    //DL_List_InsertPrepend(&(Edge->Vertex1->EdgeSet2), NULL, &(Edge->EdgeSet2));
    //DL_List_InsertAppend(&(Edge->Vertex2->EdgeSet1), NULL, &(Edge->EdgeSet1));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_EdgeRemove(MapEdge *Edge)
{
    /* 向Vertex1移除EdgeSet2 */
    /* 向Vertex2移除EdgeSet1 */
    DL_List_Remove(&(Edge->Vertex1->EdgeSet2), &(Edge->EdgeSet2));
    DL_List_Remove(&(Edge->Vertex2->EdgeSet1), &(Edge->EdgeSet1));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_VertexAdd(MapBody *Map, MapVertex *Vertex)
{
    /* 对称语义,二选其一 */
    DL_List_InsertAppend(&(Map->List), NULL, &(Vertex->Friend));
    //DL_List_InsertPrepend(&(Map->List), NULL, &(Vertex->Friend));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_VertexRemove(MapBody *Map, MapVertex *Vertex)
{
    DL_List_Remove(&(Map->List), &(Vertex->Friend));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapVertex * Map_GetVertexHead(MapBody *Map)
{
    return List_GetOwner(MapVertex, Friend, DL_List_GetHead(&(Map->List)));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapVertex * Map_GetVertexTail(MapBody *Map)
{
    return List_GetOwner(MapVertex, Friend, DL_List_GetTail(&(Map->List)));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapVertex * Map_GetVertexPrev(MapVertex *Vertex)
{
    return List_GetOwner(MapVertex, Friend, DL_Node_GetPrev(&(Vertex->Friend)));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapVertex * Map_GetVertexNext(MapVertex *Vertex)
{
    return List_GetOwner(MapVertex, Friend, DL_Node_GetNext(&(Vertex->Friend)));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapEdge * Map_GetEdgeFirst(MapVertex *Vertex)
{
    return List_GetOwner(MapEdge, EdgeSet2, DL_List_GetHead(&(Vertex->EdgeSet2)));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapEdge * Map_GetEdgeLast(MapVertex *Vertex)
{
    return List_GetOwner(MapEdge, EdgeSet2, DL_List_GetTail(&(Vertex->EdgeSet2)));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapEdge * Map_GetEdgePrev(MapEdge *Edge)
{
    return List_GetOwner(MapEdge, EdgeSet2, DL_Node_GetPrev(&(Edge->EdgeSet2)));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapEdge * Map_GetEdgeNext(MapEdge *Edge)
{
    return List_GetOwner(MapEdge, EdgeSet2, DL_Node_GetNext(&(Edge->EdgeSet2)));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 普利姆最小生成树 */
void Map_ToMinTreePrim(MapBody *Map, uint32_t *Matrix, uint32_t *MatrixPrim,
                       bool *NodeSet, uint32_t VertexCount, uint32_t WeightMax,
                       uint32_t (*EdgeWeight)(MapEdge *Edge),
                       uint32_t (*MakeRand)(void))
{
    for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++)
    for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++)
        Matrix[Index1 * VertexCount + Index2] = WeightMax;
    /* 构建Prim权值邻接矩阵(这里采用俩边之间最小权值作为无向图的值) */
    for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++)
    for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++) {
        /* 过滤到自己的点 */
        if (Index1 == Index2)
            continue;
        /* 先获得顶点 */
        MapVertex *Vertex1 = Map_GetVertexFromIndex(Map, Index1);
        MapVertex *Vertex2 = Map_GetVertexFromIndex(Map, Index2);
        /* 再获得顶点关联边 */
        MapEdge *Edge = Map_GetEdge(Vertex1, Vertex2);
        /* 如果边不存在, 跳过它 */
        if (Edge == NULL)
            continue;
        /* 如果边存在, 获取其权值 */
        uint32_t CurrentWeight = EdgeWeight(Edge);
        /* 如果边存在, 写入其权值 */
        if (Matrix[Index1 * VertexCount + Index2] > CurrentWeight &&
            Matrix[Index2 * VertexCount + Index1] > CurrentWeight) {
            Matrix[Index1 * VertexCount + Index2] = CurrentWeight;
            Matrix[Index2 * VertexCount + Index1] = CurrentWeight;
        }
    }
    /* 初始化状态矩阵 */
    for (uint32_t Index = 0; Index < VertexCount; NodeSet[Index++] = false);
    /* 先随机选择一个顶点 */
    NodeSet[MakeRand() % VertexCount] = true;
    uint32_t CurrentCount = 1;
    /* 普利姆最小生成树: */
    while (CurrentCount < VertexCount) {
        uint32_t MinWeight = WeightMax;
        uint32_t IndexI = VertexCount;
        uint32_t IndexJ = VertexCount;
        /* 从集合中寻找一个节点,从集合外寻找一个节点 */
        for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++) {
            /* 从集合中寻找一个节点 */
            if (NodeSet[Index1] == false)
                continue;
        for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++) {
            /* 从集合外寻找一个节点 */
            if (NodeSet[Index2] == true)
                continue;
            /* 如果俩个节点不存在关系 */
            if (Matrix[Index1 * VertexCount + Index2] == WeightMax)
                continue;
            /* 检查这俩个节点是否可用 */
            if (MinWeight > Matrix[Index1 * VertexCount + Index2]) {
                MinWeight = Matrix[Index1 * VertexCount + Index2];
                IndexI = Index1;
                IndexJ = Index2;
            }
        }
        }
        /* 如果寻找到集合内与集合外的最小权值边,导入该关系 */
        if (IndexI != WeightMax && IndexJ != WeightMax) {
            MatrixPrim[IndexI * VertexCount + IndexJ] = MinWeight;
            MatrixPrim[IndexJ * VertexCount + IndexI] = MinWeight;
            NodeSet[IndexJ] = true;
            CurrentCount++;
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 凯斯特拉最小生成树 */
void Map_ToMinTreeKruskal(MapBody *Map, uint32_t *Matrix, uint32_t *MatrixTemp,
                          uint32_t *MatrixKruskal, uint32_t *Set, uint32_t *SetFather,
                          uint32_t VertexCount, uint32_t WeightMax,
                          uint32_t (*EdgeWeight)(MapEdge *Edge))
{
    for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++)
    for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++)
        Matrix[Index1 * VertexCount + Index2] = WeightMax;
    /* 构建Kruskal权值邻接矩阵(这里采用俩边之间最小权值作为无向图的值) */
    for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++)
    for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++) {
        /* 过滤到自己的点 */
        if (Index1 == Index2)
            continue;
        /* 先获得顶点 */
        MapVertex *Vertex1 = Map_GetVertexFromIndex(Map, Index1);
        MapVertex *Vertex2 = Map_GetVertexFromIndex(Map, Index2);
        /* 再获得顶点关联边 */
        MapEdge *Edge = Map_GetEdge(Vertex1, Vertex2);
        /* 如果边不存在, 跳过它 */
        if (Edge == NULL)
            continue;
        /* 如果边存在, 获取其权值 */
        uint32_t CurrentWeight = EdgeWeight(Edge);
        /* 如果边存在, 写入其权值 */
        if (Matrix[Index1 * VertexCount + Index2] > CurrentWeight &&
            Matrix[Index2 * VertexCount + Index1] > CurrentWeight) {
            Matrix[Index1 * VertexCount + Index2] = CurrentWeight;
            Matrix[Index2 * VertexCount + Index1] = CurrentWeight;
        }
    }
    /* 使用备份的邻接矩阵,因为它会被修改 */
    for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++)
    for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++)
        MatrixTemp[Index1 * VertexCount + Index2] = Matrix[Index1 * VertexCount + Index2];
    /* 初始化并查集 */
    for (uint32_t Index = 0; Index < VertexCount; Set[Index] = Index++);
    for (uint32_t Index = 0; Index < VertexCount; SetFather[Index] = Index++);
    /* 初始化链接数 */
    uint32_t CurrentLink = VertexCount;
    /* 凯斯特拉最小生成树 */
    while (CurrentLink > 1) {
        uint32_t MinWeight = WeightMax;
        uint32_t IndexI = VertexCount;
        uint32_t IndexJ = VertexCount;
        /* 先寻找一个权值最小的边 */
        for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++)
        for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++)
            if (MinWeight > MatrixTemp[Index1 * VertexCount + Index2]) {
                MinWeight = MatrixTemp[Index1 * VertexCount + Index2];
                IndexI = Index1;
                IndexJ = Index2;
            }
        /* 如果没找到最小权值边 */
        if (MinWeight == WeightMax)
            break;
        /* 找到一条最小边,检查集合组是否一致 */
        uint32_t IndexISelf   = Set[IndexI];
        uint32_t IndexJSelf   = Set[IndexJ];
        uint32_t IndexIFather = SetFather[IndexI];
        uint32_t IndexJFather = SetFather[IndexJ];
        /* 迭代检查IndexI,找到最终的祖先 */
        while (IndexISelf != IndexIFather) {
            IndexISelf = Set[IndexIFather];
            IndexIFather = SetFather[IndexIFather];
        }
        /* 迭代检查IndexJ,找到最终的祖先 */
        while (IndexJSelf != IndexJFather) {
            IndexJSelf = Set[IndexJFather];
            IndexJFather = SetFather[IndexJFather];
        }
        /* 如果是不同集合,可以对其建立联系 */
        if (IndexISelf != IndexJSelf) {
            /* 合并集合,让后一个集合都归为第一个集合 */
            uint32_t IndexJSelf   = Set[IndexJ];
            uint32_t IndexJFather = SetFather[IndexJ];
            /* 这里是按秩合并 */
            while (IndexISelf != IndexJFather) {
                IndexJSelf = SetFather[IndexJ];
                IndexJFather = SetFather[IndexJFather];
                /* 本节点指向第一个集合的祖先 */
                SetFather[IndexJSelf] = IndexISelf;
            }
            SetFather[IndexJSelf] = IndexISelf;
            /* 获得一条关联 */
            MatrixKruskal[IndexI * VertexCount + IndexJ] = MinWeight;
            MatrixKruskal[IndexJ * VertexCount + IndexI] = MinWeight;
            CurrentLink--;
        }
        /* 移除这条操作完毕的边 */
        MatrixTemp[IndexI * VertexCount + IndexJ] = WeightMax;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
