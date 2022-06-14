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
    
    /* 深度优先搜索 */ {
        printf("\nMap_SearchDepthFirst Start\n");
        bool Record[10] = {0};
        MapVertex *Stack[10] = {0};
        
        TestMapVertex TVertex;
        TVertex.Label = 'A' + rand() % 5;
        Map_VertexReset(&(TVertex.InVertex));
        MapVertex *Vertex = Map_GetVertex(&TestMap, &(TVertex.InVertex));
        
        printf("\nMap_SearchDepthFirst Check:%d",
                  Map_SearchDepthFirst(&TestMap, Vertex, Stack, Record, 10, Map_Visit1));
        printf("\nMap_SearchDepthFirst End\n");
    }
    
    /* 广度优先搜索 */ {
        printf("\nMap_SearchBreadthFirst Start\n");
        bool Record[10] = {0};
        MapVertex *Queue[10] = {0};
        
        TestMapVertex TVertex;
        TVertex.Label = 'A' + rand() % 5;
        Map_VertexReset(&(TVertex.InVertex));
        MapVertex *Vertex = Map_GetVertex(&TestMap, &(TVertex.InVertex));
        
        printf("\nMap_SearchBreadthFirst Check:%d",
                  Map_SearchBreadthFirst(&TestMap, Vertex, Queue, Record, 10, Map_Visit1));
        printf("\nMap_SearchBreadthFirst End\n");
    }
    
    /* 迪杰斯特拉最短路径搜索(其中ShortestPath类型为: MapVertex *X[][]) */ {
        printf("\nMap_SearchDijkstra Start\n");
        bool Record[10] = {0};
        uint32_t Weight[10] = {0};
        uint32_t WeightMax  = 200;
        uint32_t VertexCount = 5;
        MapVertex *ShortestPath[10 * 10] = {0};
        
        TestMapVertex TVertex;
        TVertex.Label = 'A' + rand() % 5;
        Map_VertexReset(&(TVertex.InVertex));
        MapVertex *Vertex = Map_GetVertex(&TestMap, &(TVertex.InVertex));
        
        Map_SearchDijkstra(&TestMap, Vertex, Weight, ShortestPath, Record, VertexCount, 
                            WeightMax, Map_EdgeGetWeight);
        
        printf("ShortestPath:\n");
        for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++) {
            Map_Check("Vertex :", Map_GetVertexFromIndex(&TestMap, Index1), NULL);
            printf("\t%u\n", Weight[Index1]);
            printf("Path:");
        for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++)
            if (ShortestPath[Index1 * VertexCount + Index2] != NULL)
                Map_Check("->", ShortestPath[Index1 * VertexCount + Index2], NULL);
            printf("\n");
        }
        printf("\nMap_SearchDijkstra End\n");
    }
    
    /* 贝尔曼弗德最短路径搜索(其中ShortestPath类型为: MapVertex *X[][]) */ {
        printf("\nMap_SearchBellmanFord Start\n");
        uint32_t Weight[10] = {0};
        uint32_t WeightMax  = 200;
        uint32_t VertexCount = 5;
        MapVertex *ShortestPath[10 * 10] = {0};
        
        TestMapVertex TVertex;
        TVertex.Label = 'A' + rand() % 5;
        Map_VertexReset(&(TVertex.InVertex));
        MapVertex *Vertex = Map_GetVertex(&TestMap, &(TVertex.InVertex));
        
        printf("\nMap_SearchBellmanFord Check:%d\n",
                  Map_SearchBellmanFord(&TestMap, Vertex, Weight, ShortestPath,
                                         VertexCount, WeightMax, Map_EdgeGetWeight));
        
        printf("ShortestPath:\n");
        for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++) {
            Map_Check("Vertex :", Map_GetVertexFromIndex(&TestMap, Index1), NULL);
            printf("\t%u\n", Weight[Index1]);
            printf("Path:");
        for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++)
            if (ShortestPath[Index1 * VertexCount + Index2] != NULL)
                Map_Check("->", ShortestPath[Index1 * VertexCount + Index2], NULL);
            printf("\n");
        }
        printf("\nMap_SearchBellmanFord End\n");
    }
    
    /* 弗洛伊德最短路径搜索(其中ShortestPath类型为: uint32_t X[][]) */ {
        printf("\nMap_SearchFloyd Start\n");
        
        uint32_t WeightMax  = 200;
        uint32_t VertexCount = 5;
        uint32_t Weight[10 * 10] = {0};;
        uint32_t ShortestPath[10 * 10] = {0};
        
        Map_SearchFloyd(&TestMap, Weight, ShortestPath, VertexCount, WeightMax, Map_EdgeGetWeight);

        printf("Weight:\n");
        for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++) {
        for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++)
            printf("%u\t", Weight[Index1 * VertexCount + Index2]);
            printf("\n");
        }
        
        printf("ShortestPath:\n");
        for (uint32_t Index1 = 0; Index1 < VertexCount; Index1++) {
        for (uint32_t Index2 = 0; Index2 < VertexCount; Index2++)
            printf("%u\t", ShortestPath[Index1 * VertexCount + Index2]);
            printf("\n");
        }
        
        /* 随机输出一条最短路径 */
        while (1) {
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
            
            uint32_t Index1 = Map_GetVertexIndex(&TestMap, Vertex1);
            uint32_t Index2 = Map_GetVertexIndex(&TestMap, Vertex2);
            uint32_t Index0 = ShortestPath[Index1 * VertexCount + Index2];
            if (Index0 == Index2)
                continue;
            
            Map_Check("Vertex1:", Vertex1, NULL);
            printf("->");
            Map_Check("Vertex2:", Vertex2, NULL);
            printf("\n");
            
            Map_Check("->", Vertex1, NULL);
            while (Index0 != Index2) {
                MapVertex *Vertex = Map_GetVertexFromIndex(&TestMap, Index0);
                Map_Check("->", Vertex, NULL);
                Index0 = ShortestPath[Index0 *VertexCount + Index2];
            }
            Map_Check("->", Vertex2, NULL);
            printf("\n");
            
            break;
        }
        
        printf("\nMap_SearchFloyd End\n");
    }
    
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
