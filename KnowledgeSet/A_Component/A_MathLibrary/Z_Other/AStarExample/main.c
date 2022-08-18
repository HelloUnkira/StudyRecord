
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
    
/* 备注:有意思的问题,如果当前点的多个临接点能同时碰撞出相同的H */
/*      搜索效率低下,能捕获出很多看起来荒诞却不违反原理的路径 */
/*      个人理解该问题是对预估距离的过分可信而忽略实际情况导致的 */
/*      这个方式需要拥有一个更高可信度的预估值H */
/*      下面这个及其简单的地图就充分碰撞出该问题 */
/*      但如果地图变得复杂这个问题出现碰撞的概率反而会变低 */

typedef enum AStarWay {
    AS_Way_None,
    AS_Way_SU,      // Straight Up
    AS_Way_SD,      // Straight Down
    AS_Way_SL,      // Straight Left
    AS_Way_SR,      // Straight Right
    AS_Way_LU,      // Left  Up
    AS_Way_LD,      // Left  Down
    AS_Way_RU,      // Right Up
    AS_Way_RD,      // Right Down
    AS_Way_Over,
} AS_Way;

/* 地图上点描述 */
typedef struct AStarPoint {
    uint32_t X;
    uint32_t Y;
    /* A Star 启发式节点数据 */
    uint32_t F;
    uint32_t G;
    uint32_t H;
    /* 用于回溯递归时的有限状态位 */
    uint32_t S;
} AS_Point;

uint32_t AStar_H(AS_Point *T1, AS_Point *T2)
{
    #define Max(A, B) ((A) > (B) ? (A) : (B))
    #define Min(A, B) ((A) < (B) ? (A) : (B))
    
    uint32_t X = Max(T1->X, T2->X) - Min(T1->X, T2->X);
    uint32_t Y = Max(T1->Y, T2->Y) - Min(T1->Y, T2->Y);
    
    /* 很显然这里是直接使用了麦哈顿距离 */
    return (X + Y) * 10;
}

uint32_t AStar_G(bool Straight)
{
    if (Straight == true)  return 10;
    if (Straight == false) return 14;
    return 0;
}

#define AStar_ROWS  10
#define AStar_COLS  10

/* 地图 */
uint32_t AStarMap[AStar_ROWS][AStar_COLS] = {
    { +0, +0, +0, +0, -1, +0, +0, +0, +0, +0, },
    { +0, +0, +0, +0, -1, +0, +0, +0, +0, +0, },
    { +0, +0, +0, +0, -1, +0, +0, +0, +0, +0, },
    { +0, +0, +0, +0, -1, +0, +0, +0, +0, +0, },
    { +0, +0, +0, +0, -1, +0, +0, +0, +0, +0, },
    { +0, +0, +0, +0, -1, +0, +0, +0, +0, +0, },
    { +0, +0, +0, +0, -1, +0, +0, +0, +0, +0, },
    { +0, +0, +0, +0, -1, +0, +0, +0, +0, +0, },
    { +0, +0, +0, +0, +0, +0, +0, +0, +0, +0, },
    { +0, +0, +0, +0, -1, +0, +0, +0, +0, +0, },
};

bool ASP_IsValid(AS_Point *Point)
{
    if (0 <= Point->X && Point->X < AStar_ROWS &&
        0 <= Point->Y && Point->Y < AStar_ROWS &&
        AStarMap[Point->X][Point->Y] != -1 &&   // -1代表障碍物
        AStarMap[Point->X][Point->Y] !=  1)     // -2代表已走过
        return true;
    return false;
}


int main(int argc, char *argv[]) {
    /* 最小路径记录 */
    uint32_t  MinLength = -1;
    AS_Point *MinPath = NULL;
    /* 起点与终点 */
    AS_Point Target[2] = { {.X = 1, .Y = 1, },
                           {.X = 6, .Y = 7, }, };
    Target[0].H = AStar_H(&Target[0], &Target[1]);
    Target[0].F = Target[0].H;
    AStarMap[Target[0].X][Target[0].Y] = 3;
    AStarMap[Target[1].X][Target[1].Y] = 3;
    /* 回溯递归栈 */
    uint32_t  StackTop = 0;
    uint32_t  StackFactor = 7;
    uint32_t  StackLength = 10;
    AS_Point *Stack = malloc(sizeof(AS_Point) * StackLength);
    AS_Point *Current = NULL;
    /* 起始节点入栈 */
    Stack[StackTop++] = Target[0];
    /* 邻居节点最大记录 */
    #define NeightborZone   8
    /* 栈式回溯递归 */
    while (StackTop != 0) {
        Current = &Stack[StackTop - 1];
        if (AStarMap[Current->X][Current->Y] == 0)
            AStarMap[Current->X][Current->Y] =  1;
        /* 在此处来输出该图以及当前进动点: */
        if (Current->X == Target[1].X && Current->Y == Target[1].Y) {
            printf("Catch One Path:\n");
            for (uint32_t Index = 0; Index < StackTop; Index++)
                printf("<%u,%u>", Stack[Index].X, Stack[Index].Y);
            printf("\n");
            
            if (MinLength > StackTop) {
                MinLength = StackTop;
                MinPath = realloc(MinPath, sizeof(AS_Point) * MinLength);
                for(uint32_t Index = 0; Index < StackTop; Index++)
                    MinPath[Index] = Stack[Index];
            }
        }
        if (0) {
            for (uint32_t I = 0; I < AStar_ROWS; I++) {
            for (uint32_t J = 0; J < AStar_COLS; J++)
                printf("%3d\t", (int32_t)AStarMap[I][J]);
                printf("\n");
            }
            //sleep(1);
        }
        /* 动态扩张栈,如果它不够的话 */
        if (StackLength <= StackTop)
            Stack = realloc(Stack, sizeof(AS_Point) *
                           (StackLength += StackFactor));
        /* 如果找到目标,检查该路径 */
        if (Current->X == Target[1].X && Current->Y == Target[1].Y) {
            if (AStarMap[Current->X][Current->Y] == 1)
                AStarMap[Current->X][Current->Y] = 0;
            StackTop--;
            continue;
        }
        /* 栈顶元素已经完成全部A Star运算 */
        if (Current->S >= NeightborZone || Current->F == -1) {
            if (AStarMap[Current->X][Current->Y] == 1)
                AStarMap[Current->X][Current->Y] = 0;
            StackTop--;
            continue;
        }
        /* 对栈顶节点进行A Star运算 */
        uint32_t NeightborNumber = 0;
        AS_Point Neightbor[NeightborZone] = {0};
        /* 初始化邻居的信息为自己的信息 */
        for (uint32_t Index = 0; Index < NeightborZone; Index++) {
            Neightbor[Index] = *Current;
            Neightbor[Index].S = 0;
        }
        /* 从当前点开始计算邻居的信息 */
        for (uint32_t Index = 0; Index < NeightborZone; Index++) {
            /* 因为枚举0值点用于起始状态使用,所以这里偏移一位 */
            switch (Index + 1) {
            case AS_Way_SU: Neightbor[Index].X--; break;
            case AS_Way_SD: Neightbor[Index].X++; break;
            case AS_Way_SL: Neightbor[Index].Y--; break;
            case AS_Way_SR: Neightbor[Index].Y++; break;
            case AS_Way_LU: Neightbor[Index].X--;
                            Neightbor[Index].Y--; break;
            case AS_Way_LD: Neightbor[Index].X++;
                            Neightbor[Index].Y--; break;
            case AS_Way_RU: Neightbor[Index].X--;
                            Neightbor[Index].Y++; break;
            case AS_Way_RD: Neightbor[Index].X++;
                            Neightbor[Index].Y++; break;
            }
            /* 节点有效? */
            if (ASP_IsValid(&Neightbor[Index])) {
                switch (Index + 1) {
                case AS_Way_SU:
                case AS_Way_SD:
                case AS_Way_SL:
                case AS_Way_SR:
                    Neightbor[Index].G += AStar_G(true);
                    break;
                case AS_Way_LU:
                case AS_Way_LD:
                case AS_Way_RU:
                case AS_Way_RD:
                    Neightbor[Index].G += AStar_G(false);
                    break;
                }
                /* 计算期望开销 */
                Neightbor[Index].H = AStar_H(&Target[1], &Neightbor[Index]);
                /* 计算启发值 */
                Neightbor[Index].F = 0;
                Neightbor[Index].F += Neightbor[Index].G;
                Neightbor[Index].F += Neightbor[Index].H;
            } else {
                Neightbor[Index].G = -1;
                Neightbor[Index].H = -1;
                Neightbor[Index].F = -1;
            }
        }
        /* 对邻居节点更新检查,按照F值排序 */
        for (uint32_t I = 0; I < NeightborZone; I++)
        for (uint32_t J = I; J < NeightborZone; J++)
            if (Neightbor[I].F > Neightbor[J].F) {
                AS_Point Temp = Neightbor[I];
                Neightbor[I] = Neightbor[J];
                Neightbor[J] = Temp;
            }
        /* 将当前邻居和之前的邻居比较,只有相同F才可被选择 */
        if (Current->S == 0 ||
            Neightbor[Current->S - 1].F == Neightbor[Current->S].F)
            /* 将下一邻居节点入栈 */
            Stack[StackTop++] = Neightbor[Current->S++];
        else
            Current->S = NeightborZone;
    }
    
    printf("\n\nCatch One Min Path:\n");
    for (uint32_t Index = 0; Index < MinLength; Index++)
        printf("<%u,%u>", MinPath[Index].X, MinPath[Index].Y);
    printf("\n");
    
    free(Stack);
    free(MinPath);
    
    return 0;
}
