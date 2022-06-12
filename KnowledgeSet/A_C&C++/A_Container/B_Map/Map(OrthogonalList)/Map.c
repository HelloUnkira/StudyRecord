//C std lib
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "List.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct MapVertexStruct {
    DL_Node Friend;     /* 伙伴顶点项 */
    DL_List DegreeIn;   /* 入度边集合 */
    DL_List DegreeOut;  /* 出度边集合 */
} MapVertex;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct MapEdgeStruct {
    DL_Node    DegreeIn;   /* 入度边项 */
    DL_Node    DegreeOut;  /* 出度边项 */
    MapVertex *VertexIn;   /* 弧的发起者 */
    MapVertex *VertexOut;  /* 弧的接受者 */
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
        Check("\n\tDegreeOut: ", NULL, NULL);
        /* 对称语义,二选其一 */
        // DL_List_Traverse_Forward(&(Vertex->DegreeOut), Current1)
        DL_List_Traverse_Backward(&(Vertex->DegreeOut), Current1) {
            MapEdge *Edge = List_GetOwner(MapEdge, DegreeOut, Current1);
            Check("\n\t\tEdge: ", NULL, Edge);
            Check("\n\t\t\tVertexIn:  ", Edge->VertexIn,  NULL);
            Check("\n\t\t\tVertexOut: ", Edge->VertexOut, NULL);
        }
        /* 遍历检查节点入度 */
        Check("\n\tDegreeIn: ", NULL, NULL);
        /* 对称语义,二选其一 */
        // DL_List_Traverse_Forward(&(Vertex->DegreeIn), Current1)
        DL_List_Traverse_Backward(&(Vertex->DegreeIn), Current1) {
            MapEdge *Edge = List_GetOwner(MapEdge, DegreeIn, Current1);
            Check("\n\t\tEdge: ", NULL, Edge);
            Check("\n\t\t\tVertexIn:  ", Edge->VertexIn,  NULL);
            Check("\n\t\t\tVertexOut: ", Edge->VertexOut, NULL);
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_EdgeReset(MapEdge *Edge)
{
    Edge->VertexIn  = NULL;
    Edge->VertexOut = NULL;
    DL_Node_Reset(&(Edge->DegreeIn));
    DL_Node_Reset(&(Edge->DegreeOut));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_VertexReset(MapVertex *Vertex)
{
    DL_Node_Reset(&(Vertex->Friend));
    DL_List_Reset(&(Vertex->DegreeIn));
    DL_List_Reset(&(Vertex->DegreeOut));
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
void Map_EdgeSet(MapEdge *Edge, MapVertex *Start, MapVertex *End)
{
    Edge->VertexIn  = Start;
    Edge->VertexOut = End;
    DL_Node_Reset(&(Edge->DegreeIn));
    DL_Node_Reset(&(Edge->DegreeOut));
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
MapVertex * Map_EdgeGetStart(MapEdge *Edge)
{
    return Edge->VertexIn;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapVertex * Map_EdgeGetEnd(MapEdge *Edge)
{
    return Edge->VertexOut;
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
        MapVertex *Target = List_GetOwner(MapVertex, Friend, Current);
        if (Vertex == Target)
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
        if (Map->Confirm(Target, Vertex))
            return Target;
    }
    return NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 通过俩个图中的真顶点获得真边,如果存在 */
MapEdge * Map_GetEdge(MapVertex *Start, MapVertex *End)
{
    /* 对称语义,二选其一 */
    DL_Node *Current = NULL;
    /* 这是从Start的出度寻找的方式 */
    DL_List_Traverse_Forward(&(Start->DegreeOut), Current) {
        MapEdge *Target = List_GetOwner(MapEdge, DegreeOut, Current);
        if (Target->VertexOut == End)
            return Target;
    }
    return NULL;
    /* 这是从End的入度寻找的方式 */
    DL_List_Traverse_Forward(&(End->DegreeIn), Current) {
        MapEdge *Target = List_GetOwner(MapEdge, DegreeOut, Current);
        if (Target->VertexIn == Start)
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
    if (Edge->VertexIn == Edge->VertexOut)
        return;
    /* 向弧添加(发起者的出度:尾部追加;接收者的入度:首项插入) */
    DL_List_InsertAppend(&(Edge->VertexIn->DegreeOut), NULL, &(Edge->DegreeOut));
    DL_List_InsertPrepend(&(Edge->VertexOut->DegreeIn), NULL, &(Edge->DegreeIn));
    /* 向弧添加(发起者的出度:首项插入;接收者的入度:尾部追加) */
    //DL_List_InsertPrepend(&(Edge->VertexIn->DegreeOut), NULL, &(Edge->DegreeOut));
    //DL_List_InsertAppend(&(Edge->VertexOut->DegreeIn), NULL, &(Edge->DegreeIn));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_EdgeRemove(MapEdge *Edge)
{
    /* 向弧的发起者移除出度 */
    /* 向弧的接受者移除入度 */
    DL_List_Remove(&(Edge->VertexIn->DegreeOut), &(Edge->DegreeOut));
    DL_List_Remove(&(Edge->VertexOut->DegreeIn), &(Edge->DegreeIn));
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
    return List_GetOwner(MapEdge, DegreeOut, DL_List_GetHead(&(Vertex->DegreeOut)));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapEdge * Map_GetEdgeLast(MapVertex *Vertex)
{
    return List_GetOwner(MapEdge, DegreeOut, DL_List_GetTail(&(Vertex->DegreeOut)));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapEdge * Map_GetEdgePrev(MapEdge *Edge)
{
    return List_GetOwner(MapEdge, DegreeOut, DL_Node_GetPrev(&(Edge->DegreeOut)));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapEdge * Map_GetEdgeNext(MapEdge *Edge)
{
    return List_GetOwner(MapEdge, DegreeOut, DL_Node_GetNext(&(Edge->DegreeOut)));
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 深度优先搜索 */
bool Map_SearchDepthFirst(MapBody *Map, MapVertex *Vertex, MapVertex **Stack, bool *Record,
                          uint32_t VertexCount, void (*Visit)(MapVertex *Vertex))
{
    uint32_t IndexStack = 0;
    /* 首节点入栈 */
    Stack[IndexStack++] = Vertex;
    /* 栈溢出检查 */
    if (IndexStack >= VertexCount)
        return false;
    /* 深度优先搜索: */
    while (IndexStack != 0) {
        /* 栈顶节点出栈 */
        MapVertex *Target = Stack[--IndexStack];
        /* 检查该节点是否被访问 */
        uint32_t Index = Map_GetVertexIndex(Map, Target);
        /* 如果该节点没有被访问,访问它,并添加访问记录 */
        if (Record[Index] == false) {
            Record[Index] = true;
            Visit(Target);
        }
        /* 遍历该节点所有边并为其添加,同时过滤掉已经访问过的点 */
        /* 对称语义,二选其一 */
        // DL_List_Traverse_Forward(&(Target->DegreeOut), Current)
        DL_List_Traverse_Backward(&(Target->DegreeOut), Current) {
            MapEdge *Edge = List_GetOwner(MapEdge, DegreeOut, Current);
            MapVertex *Temp = Map_EdgeGetEnd(Edge);
            /* 如果该节点没有被访问,将其入栈 */
            if (Record[Map_GetVertexIndex(Map, Temp)] == false) {
                Stack[IndexStack++] = Temp;
                /* 栈溢出检查 */
                if (IndexStack >= VertexCount)
                    return false;
            }
        }
    }
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 广度优先搜索 */
bool Map_SearchBreadthFirst(MapBody *Map, MapVertex *Vertex, MapVertex **Queue, bool *Record,
                            uint32_t VertexCount, void (*Visit)(MapVertex *Vertex))
{
    uint32_t IndexQueueHead = 0;
    uint32_t IndexQueueTail = 0;
    /* 首节点队列 */
    Queue[IndexQueueTail++ % VertexCount] = Vertex;
    /* 广度优先搜索: */
    while (IndexQueueHead < IndexQueueTail) {
        /* 首向节点出队列 */
        MapVertex *Target = Queue[IndexQueueHead++ % VertexCount];
        /* 检查该节点是否被访问 */
        uint32_t Index = Map_GetVertexIndex(Map, Target);
        /* 如果该节点没有被访问,访问它,并添加访问记录 */
        if (Record[Index] == false) {
            Record[Index] = true;
            Visit(Target);
        }
        /* 遍历该节点所有边并为其添加,同时过滤掉已经访问过的点 */
        /* 对称语义,二选其一 */
        // DL_List_Traverse_Forward(&(Target->DegreeOut), Current)
        DL_List_Traverse_Backward(&(Target->DegreeOut), Current) {
            MapEdge *Edge = List_GetOwner(MapEdge, DegreeOut, Current);
            MapVertex *Temp = Map_EdgeGetEnd(Edge);
            /* 如果该节点没有被访问,将其入队列 */
            if (Record[Map_GetVertexIndex(Map, Temp)] == false) {
                Queue[IndexQueueTail++ % VertexCount] = Temp;
                /* 队列小检查 */
                if (IndexQueueTail - IndexQueueHead > VertexCount)
                    return false;
            }
        }
    }
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 迪杰斯特拉最短路径搜索(其中ShortestPath类型为: MapVertex *X[][]) */
void Map_SearchDijkstra(MapBody *Map, MapVertex *Vertex, uint32_t *Weight,
                        MapVertex **ShortestPath, bool *Record, uint32_t VertexCount,
                        uint32_t WeightMax, uint32_t (*EdgeWeight)(MapEdge *Edge))
{
    /* 初始化最短路径数值,设置为最大值 */
    for (uint32_t Index = 0; Index < VertexCount; Weight[Index++] = WeightMax);
    /* 初始化最短路径情况 */
    for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++)
    for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++)
        if (Index2 != 0)
            ShortestPath[Index1 * VertexCount + Index2] = NULL;
        else
            ShortestPath[Index1 * VertexCount + Index2] = Vertex;
    /* 获得自己的虚拟坐标映射 */
    uint32_t Index = Map_GetVertexIndex(Map, Vertex);
    /* 配置到自己的最短路径为0 */
    Weight[Index] = 0;
    /* 保存到一次确定的最小值及其它的最短路径 */
    uint32_t MinWeight = WeightMax;
    uint32_t MinIndex  = Index;
    /* 迪杰斯特拉最短路径搜索: */
    while (1) {
        /* 如果找到一个需要被确定但未被确定的节点,标记它并准备确定它 */
        Index = MinIndex;
        /* 未确定检查 */
        if (Record[Index] == false)
            Record[Index] = true;
        else {
            /* 从记录中找一个未使用的下标 */
            uint32_t UnusedIndex = 0;
            for (UnusedIndex = 0; UnusedIndex < VertexCount; UnusedIndex++)
                if (Record[UnusedIndex] == false)
                    break;
            /* 如果所有的下标记录都被确定,退出搜索 */
            if (UnusedIndex >= VertexCount)
                break;
            /* 如果寻找到了第一个未使用下标,开始记录它 */
            Index = UnusedIndex;
            MinIndex = UnusedIndex;
            Record[Index] = true;
        }
        Vertex = Map_GetVertexFromIndex(Map, Index);
        /* 开始确定该节点 */
        /* 对称语义,二选其一 */
        // DL_List_Traverse_Forward(&(Vertex->DegreeOut), Current)
        DL_List_Traverse_Backward(&(Vertex->DegreeOut), Current) {
            MapEdge *Edge = List_GetOwner(MapEdge, DegreeOut, Current);
            MapVertex *Temp = Map_EdgeGetEnd(Edge);
            /* 获得该边的权值 */
            uint32_t CurrentWeight = EdgeWeight(Edge);
            /* 获得目标的虚拟坐标映射 */
            uint32_t CurrentIndex = Map_GetVertexIndex(Map, Temp);
            /* 更新最小值 */
            if (CurrentWeight < MinWeight) {
                MinIndex = CurrentIndex;
                MinWeight = CurrentWeight;
            }
            /* 更新最短路径数组值,更新最短路径 */
            if (Weight[Index] + CurrentWeight < Weight[CurrentIndex]) {
                /* 更新最短路径数组值 */
                Weight[CurrentIndex] = Weight[Index] + CurrentWeight;
                /* 将Vertex -> Index修改为Vertex -> CurrentIndex -> Index */
                uint32_t Nonius = 0;
                /* 无需担心路径溢出,除法成环否则不会重复 */
                while (ShortestPath[Index * VertexCount + (++Nonius)] != NULL)
                    ShortestPath[CurrentIndex * VertexCount + Nonius] =
                    ShortestPath[Index * VertexCount + Nonius];
                /* 新的路径加入 */
                ShortestPath[CurrentIndex * VertexCount + Nonius++] = Temp;
                /* 追加补充 */
                if (Nonius < VertexCount)
                    ShortestPath[CurrentIndex * VertexCount + Nonius] = NULL;
            }
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 贝尔曼弗德最短路径搜索(其中ShortestPath类型为: MapVertex *X[][]) */
bool Map_SearchBellmanFord(MapBody *Map, MapVertex *Vertex, uint32_t *Weight,
                           MapVertex **ShortestPath, uint32_t VertexCount,
                           uint32_t WeightMax, uint32_t (*EdgeWeight)(MapEdge *Edge))
{
    /* 初始化最短路径数值,设置为最大值 */
    for (uint32_t Index = 0; Index < VertexCount; Weight[Index++] = WeightMax);
    /* 初始化最短路径情况 */
    for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++)
    for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++)
        if (Index2 != 0)
            ShortestPath[Index1 * VertexCount + Index2] = NULL;
        else
            ShortestPath[Index1 * VertexCount + Index2] = Vertex;
    /* 获得自己的虚拟坐标映射,配置到自己的最短路径为0 */
    Weight[Map_GetVertexIndex(Map, Vertex)] = 0;
    /* 贝尔曼弗德最短路径搜索: */
    for (uint32_t Index = 0; Index < VertexCount; Index++) {
        bool UploadOver = true;
        /* 对每一条存在的边进行搜索 */
        for (uint32_t IndexFrom = 0; IndexFrom < VertexCount; IndexFrom++) {
            /* 获得目标节点 */
            MapVertex * Vertex = Map_GetVertexFromIndex(Map, IndexFrom);
            /* 检查目标节点是否存在边 */
            if (Map_GetEdgeFirst(Vertex) == NULL)
                continue;
            /* 从已知节点出发 */
            if (Weight[IndexFrom] == WeightMax)
                continue;
            /* 从IndexFrom到IndexTo,遍历检查 */
            /* 对称语义,二选其一 */
            // DL_List_Traverse_Forward(&(Vertex->DegreeOut), Current)
            DL_List_Traverse_Backward(&(Vertex->DegreeOut), Current) {
                MapEdge *Edge = List_GetOwner(MapEdge, DegreeOut, Current);
                MapVertex *Temp = Map_EdgeGetEnd(Edge);
                /* 获得该边的权值 */
                uint32_t CurrentWeight = EdgeWeight(Edge);
                /* 确定从顶点VertexFrom到VertexTo */
                uint32_t IndexTo = Map_GetVertexIndex(Map, Temp);
                /* 更新最短路径数组值,更新最短路径 */
                if (Weight[IndexFrom] + CurrentWeight < Weight[IndexTo]) {
                    /* 更新最短路径数组值 */
                    Weight[IndexTo] = Weight[IndexFrom] + CurrentWeight;
                    /* 标记当前段未更新完毕 */
                    UploadOver = false;
                    /* 将Vertex -> IndexTo修改为Vertex -> IndexFrom -> IndexTo */
                    uint32_t Nonius = 0;
                    /* 无需担心路径溢出,除法成环否则不会重复 */
                    while (ShortestPath[IndexFrom * VertexCount + (++Nonius)] != NULL)
                        ShortestPath[IndexTo * VertexCount + Nonius] =
                        ShortestPath[IndexFrom * VertexCount + Nonius];
                    /* 新的路径加入 */
                    ShortestPath[IndexTo * VertexCount + Nonius++] = Temp;
                    /* 追加补充 */
                    if (Nonius < VertexCount)
                        ShortestPath[IndexTo * VertexCount + Nonius] = NULL;
                }
            }
            if (UploadOver == true)
                break;
        }
    }
    /* 是否成环检查 */
    for (uint32_t IndexFrom = 0; IndexFrom < VertexCount; IndexFrom++) {
        /* 获得目标节点 */
        MapVertex * Vertex = Map_GetVertexFromIndex(Map, IndexFrom);
        /* 检查目标节点是否存在边 */
        if (Map_GetEdgeFirst(Vertex) == NULL)
            continue;
        /* 从IndexFrom到IndexTo,遍历检查 */
        /* 对称语义,二选其一 */
        // DL_List_Traverse_Forward(&(Vertex->DegreeOut), Current)
        DL_List_Traverse_Backward(&(Vertex->DegreeOut), Current) {
            MapEdge *Edge = List_GetOwner(MapEdge, DegreeOut, Current);
            MapVertex *Temp = Map_EdgeGetEnd(Edge);
            /* 获得该边的权值 */
            uint32_t CurrentWeight = EdgeWeight(Edge);
            /* 确定从顶点VertexFrom到VertexTo */
            uint32_t IndexTo = Map_GetVertexIndex(Map, Temp);
            /* 更新最短路径数组值,更新最短路径 */
            if (Weight[IndexFrom] + CurrentWeight < Weight[IndexTo])
                return false;
        }
    }
    
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 弗洛伊德最短路径搜索(其中Weight, ShortestPath类型为: uint32_t X[][]) */
void Map_SearchFloyd(MapBody *Map, uint32_t *Weight, uint32_t *ShortestPath,
                     uint32_t VertexCount, uint32_t WeightMax,
                     uint32_t (*EdgeWeight)(MapEdge *Edge))
{
    /* 构建Floyd权值邻接矩阵 */
    for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++)
    for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++) {
        /* 记录最短路径情况 */
        ShortestPath[Index1 * VertexCount + Index2] = Index2;
        /* 过滤到自己的点 */
        if (Index1 == Index2) {
            Weight[Index1 * VertexCount + Index2] = 0;
            continue;
        }
        /* 先获得顶点 */
        MapVertex *Vertex1 = Map_GetVertexFromIndex(Map, Index1);
        MapVertex *Vertex2 = Map_GetVertexFromIndex(Map, Index2);
        /* 再获得顶点关联边 */
        MapEdge *Edge = Map_GetEdge(Vertex1, Vertex2);
        /* 如果边不存在, 跳过它 */
        if (Edge == NULL)
            Weight[Index1 * VertexCount + Index2] = WeightMax;
        /* 如果边存在, 获取其权值写入 */
        if (Edge != NULL)
            Weight[Index1 * VertexCount + Index2] = EdgeWeight(Edge);
    }
    /* 弗洛伊德最短路径搜索: */
    for (uint32_t Index0 = 0; Index0 < VertexCount; Index0++)
    for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++) {
        if (Index0 == Index1)
            continue;
    for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++) {
        if (Index0 == Index2 || Index1 == Index2)
            continue;
        /* 获得权值 */
        uint32_t Weight10 = Weight[Index1 * VertexCount + Index0];
        uint32_t Weight02 = Weight[Index0 * VertexCount + Index2];
        /* 如果俩条边之和小于第三条边 */
        if (Weight[Index1 * VertexCount + Index2] > Weight10 + Weight02) {
            Weight[Index1 * VertexCount + Index2] = Weight10 + Weight02;
            ShortestPath[Index1 * VertexCount + Index2] = 
            ShortestPath[Index1 * VertexCount + Index0];
        }
    }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 拓扑排序 */
void Map_SortTopological(MapBody *Map, uint32_t *Sort, uint32_t VertexCount)
{
    /* 拓扑排序涉及到节点移除,不额外编写 */
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
