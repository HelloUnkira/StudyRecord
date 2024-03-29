#ifndef MAP_H
#define MAP_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "DoubleList.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 载体为邻接多重链表的图 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define Map_GetOwner(Type, Member, MemberAddress)   \
    ((Type *)((uint8_t *)(MemberAddress) - ((uintptr_t) &((Type *)0)->Member)))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct MapEdgeStruct {
    DL_Node Parameter1;
    DL_Node Parameter2;
    void   *Parameter3;
    void   *Parameter4;
} MapEdge;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct MapVertexStruct {
    DL_Node Parameter1;
    DL_List Parameter2;
    DL_List Parameter3;
} MapVertex;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct MapBodyStruct {
    DL_List Parameter1;
    void   *Parameter2;
} MapBody;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef void (*Map_MessageCheck)(char *String, MapVertex *Vertex, MapEdge *Edge);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_MessageCheckAll(MapBody *Map, Map_MessageCheck Check);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_EdgeReset(MapEdge *Edge);
void Map_VertexReset(MapVertex *Vertex);
void Map_BodyReset(MapBody *Map);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_EdgeSet(MapEdge *Edge, MapVertex *Vertex1, MapVertex *Vertex2);
void Map_BodySet(MapBody *Map, bool (*Confirm)(MapVertex *Vertex1, MapVertex *Vertex2));
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 其他接口的参数都是图中真实存在的顶点和边,如果不知道可以用下面的俩个接口查询 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 通过"特性值匹配"的假顶点获取图中的真顶点 */
MapVertex * Map_GetVertex(MapBody *Map, MapVertex *Vertex);
/* 通过图中的俩个真顶点获得真边,如果存在 */
MapEdge * Map_GetEdge(MapVertex *Vertex1, MapVertex *Vertex2);
/* 获取图节点数量 */
uint32_t Map_GetVertexNumber(MapBody *Map);
/* 按照顶点加载顺序获得它的虚拟数组映射的下标(用于某些扩展运算) */
uint32_t Map_GetVertexIndex(MapBody *Map, MapVertex *Vertex);
MapVertex * Map_GetVertexFromIndex(MapBody *Map, uint32_t Index);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapVertex * Map_EdgeGetVertex1(MapEdge *Edge);
MapVertex * Map_EdgeGetVertex2(MapEdge *Edge);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 图的基本运算 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_EdgeAdd(MapEdge *Edge);
void Map_EdgeRemove(MapEdge *Edge);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void Map_VertexAdd(MapBody *Map, MapVertex *Vertex);
void Map_VertexRemove(MapBody *Map, MapVertex *Vertex);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 迭代语义接口 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapVertex * Map_GetVertexHead(MapBody *Map);
MapVertex * Map_GetVertexTail(MapBody *Map);
MapVertex * Map_GetVertexPrev(MapVertex *Vertex);
MapVertex * Map_GetVertexNext(MapVertex *Vertex);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
MapEdge * Map_GetEdgeFirst(MapVertex *Vertex);
MapEdge * Map_GetEdgeLast(MapVertex *Vertex);
MapEdge * Map_GetEdgePrev(MapEdge *Edge);
MapEdge * Map_GetEdgeNext(MapEdge *Edge);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 基本图论接口扩展 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 普利姆最小生成树 */
void Map_ToMinTreePrim(MapBody *Map, uint32_t *Matrix, uint32_t *MatrixPrim,
                       bool *NodeSet, uint32_t VertexCount, uint32_t WeightMax,
                       uint32_t (*EdgeWeight)(MapEdge *Edge),
                       uint32_t (*MakeRand)(void));
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 凯斯特拉最小生成树 */
void Map_ToMinTreeKruskal(MapBody *Map, uint32_t *Matrix, uint32_t *MatrixTemp,
                          uint32_t *MatrixKruskal, uint32_t *Set, uint32_t *SetFather,
                          uint32_t VertexCount, uint32_t WeightMax,
                          uint32_t (*EdgeWeight)(MapEdge *Edge));
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
