#include <stdio.h>
#include <stdlib.h>
#include "Map.h"

typedef struct TestMapEdgeStruct {
    /* 边带一个权值即可 */
    uint32_t Weight;
    
    MapEdge InEdge;
} TestMapEdge;

typedef struct TestMapVertexStruct {
    /* 节点带一个标号即可 */
    char Label;
    
    MapVertex InVertex;
} TestMapVertex;

static MapBody TestMap = {0};

bool ConfirmVertex(MapVertex *Vertex1, MapVertex *Vertex2)
{
    if ((Map_GetOwner(TestMapVertex, InVertex, Vertex1))->Label ==
        (Map_GetOwner(TestMapVertex, InVertex, Vertex2))->Label)
        return true;
    return false;
}

uint32_t Map_EdgeGetWeight(MapEdge *Edge)
{
    return (Map_GetOwner(TestMapEdge, InEdge, Edge))->Weight;
}

uint32_t Map_MakeRand(void)
{
    return rand();
}

void Map_Check(char *String, MapVertex *Vertex, MapEdge *Edge)
{
    printf(String);
    
    if (Vertex != NULL)
        printf("%c", (Map_GetOwner(TestMapVertex, InVertex, Vertex))->Label);
    if (Edge != NULL)
        printf("%d", (Map_GetOwner(TestMapEdge, InEdge, Edge))->Weight);
}

void Map_Visit1(MapVertex *Vertex)
{
    if (Vertex != NULL)
        printf("%c \n", (Map_GetOwner(TestMapVertex, InVertex, Vertex))->Label);
}

int main(int argc, char *argv[])
{
    Map_BodyReset(&TestMap);
    Map_BodySet(&TestMap, ConfirmVertex);
    
    /* 五个顶点 */
    for (uint32_t I = 0; I < 5; I++) {
        TestMapVertex *TVertex = malloc(sizeof(TestMapVertex));
        TVertex->Label = 'A' + I;
        Map_VertexReset(&(TVertex->InVertex));
        Map_VertexAdd(&TestMap, &(TVertex->InVertex));
    }
    
    printf("Map Vertex Number:%d\n", Map_GetVertexNumber(&TestMap));
    
    /* 生成满环图 */
    for (uint32_t I = 0; I < 5; I++) {
        
        TestMapVertex TVertex1;
        TVertex1.Label = 'A' + I;
        Map_VertexReset(&(TVertex1.InVertex));
        
        for (uint32_t J = 0; J < 5; J) {
            
            TestMapVertex TVertex2;
            TVertex2.Label = 'A' + J;
            Map_VertexReset(&(TVertex2.InVertex));
            
            if (TVertex1.Label == TVertex2.Label) {
                J++;
                continue;
            }
            
            MapVertex *Vertex1 = Map_GetVertex(&TestMap, &(TVertex1.InVertex));
            MapVertex *Vertex2 = Map_GetVertex(&TestMap, &(TVertex2.InVertex));
            if (Vertex1 == NULL || Vertex2 == NULL) {
                printf("What Happend");
                continue;
            }
            
            MapEdge *Edge = Map_GetEdge(Vertex1, Vertex2);
            if (Edge != NULL)
                continue;
            
            TestMapEdge *TEdge = malloc(sizeof(TestMapEdge));
            TEdge->Weight = rand() % 100;
            Map_EdgeSet(&(TEdge->InEdge), Vertex1, Vertex2);
            Map_EdgeAdd(&(TEdge->InEdge));
            
            J++;
        }
    }
    
    /* 随机删除一半边 */
    for (uint32_t I = 0; I < 10; I) {
        
        TestMapVertex TVertex1;
        TVertex1.Label = 'A' + rand() % 5;
        Map_VertexReset(&(TVertex1.InVertex));
        TestMapVertex TVertex2;
        TVertex2.Label = 'A' + rand() % 5;
        Map_VertexReset(&(TVertex2.InVertex));
        if (TVertex1.Label == TVertex2.Label)
            continue;
        
        MapVertex *Vertex1 = Map_GetVertex(&TestMap, &(TVertex1.InVertex));
        MapVertex *Vertex2 = Map_GetVertex(&TestMap, &(TVertex2.InVertex));
        if (Vertex1 == NULL || Vertex2 == NULL) {
            printf("What Happend");
            continue;
        }
        
        MapEdge *Edge = Map_GetEdge(Vertex1, Vertex2);
        if (Edge == NULL)
            continue;
        
        Map_EdgeRemove(Edge);
        TestMapEdge *TEdge = Map_GetOwner(TestMapEdge, InEdge, Edge);
        free(TEdge);
        
        I++;
    }
    
    Map_MessageCheckAll(&TestMap, Map_Check);
    
    /* 普利姆最小生成树 */ {
        printf("\nMap_ToMinTreePrim Start\n");
        
        uint32_t WeightMax = 200;
        uint32_t VertexCount = 5;
        uint32_t Matrix[10 * 10] = {0};
        uint32_t MatrixPrim[10 * 10] = {0};
        bool NodeSet[10] = {0};
        
        Map_ToMinTreePrim(&TestMap, Matrix, MatrixPrim, NodeSet, VertexCount,
                           WeightMax, Map_EdgeGetWeight, Map_MakeRand);
        
        printf("Matrix:\n");
        for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++) {
        for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++)
            printf("%u\t", Matrix[Index1 * VertexCount + Index2]);
            printf("\n");
        }
        
        printf("MatrixPrim:\n");
        for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++) {
        for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++)
            printf("%u\t", MatrixPrim[Index1 * VertexCount + Index2]);
            printf("\n");
        }
        
        printf("\nMap_ToMinTreePrim End\n");
    }
    
    /* 凯斯特拉最小生成树 */ {
        printf("\nMap_ToMinTreeKruskal Start\n");
        
        uint32_t WeightMax = 200;
        uint32_t VertexCount = 5;
        uint32_t Matrix[10 * 10] = {0};
        uint32_t MatrixTemp[10 * 10] = {0};
        uint32_t MatrixKruskal[10 * 10] = {0};
        uint32_t Set[10] = {0};
        uint32_t SetFather[10] = {0};
        
        
        Map_ToMinTreeKruskal(&TestMap, Matrix, MatrixTemp, MatrixKruskal,
                              Set, SetFather, VertexCount, WeightMax,
                              Map_EdgeGetWeight);
        
        printf("Matrix:\n");
        for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++) {
        for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++)
            printf("%u\t", Matrix[Index1 * VertexCount + Index2]);
            printf("\n");
        }
        
        printf("MatrixKruskal:\n");
        for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++) {
        for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++)
            printf("%u\t", MatrixKruskal[Index1 * VertexCount + Index2]);
            printf("\n");
        }
        
        printf("\nMap_ToMinTreeKruskal End\n");
    }
    
    return 0;
}
