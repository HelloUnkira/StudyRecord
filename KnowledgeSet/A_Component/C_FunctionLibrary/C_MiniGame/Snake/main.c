#include <stdio.h>
#include <stdlib.h>

#include "Console.h"

#define SnakeWidth   20
#define SnakeHeight  30

bool Map[SnakeHeight][SnakeWidth] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,},
    {0,0,1,0,0,0,1,0,0,1,0,1,1,1,1,1,1,0,0,0,},
    {0,0,1,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,1,0,0,0,1,1,1,0,0,0,1,0,0,1,0,0,},
    {0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,},
    {0,1,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,1,0,0,},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,},
    {0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,},
    {0,1,0,1,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,0,},
    {0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,},
    {0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,0,1,0,0,},
    {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,1,0,1,0,0,},
    {0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,},
    {0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,0,0,1,0,},
    {0,0,0,1,0,1,0,0,0,0,1,0,0,0,0,0,0,0,1,0,},
    {0,0,0,1,0,1,1,0,0,1,1,0,0,0,0,0,1,1,1,0,},
    {0,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,},
    {0,1,0,0,0,1,1,1,0,1,0,0,0,0,1,1,0,0,0,0,},
    {0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,},
    {0,0,0,1,1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,},
    {0,0,1,1,0,0,0,1,0,0,0,1,0,0,0,1,1,1,0,0,},
    {0,0,1,0,0,1,0,1,1,1,0,0,0,0,0,0,0,1,0,0,},
    {0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,},
    {0,0,0,0,0,1,0,0,0,0,1,1,1,1,1,0,0,1,0,0,},
    {0,0,0,1,1,1,0,0,0,0,0,0,1,0,0,0,0,1,0,0,},
    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,},
};

void InstanceCreate(uint32_t Size, void **Pointer)
{
    *Pointer = malloc(Size);
}

void InstanceDestroy(void *Pointer)
{
    free(Pointer);
}

void InstanceSetRecord1(SnakeNode *OldHead, SnakeNode *Node)
{
    OldHead->Record = 0;    /* 旧节点设置为蛇身 */
    Node->Record = 1;       /* 新节点设置为蛇头 */
}

void InstanceSetRecord2(SnakeNode *NewTail, SnakeNode *Node)
{
    Node->Record = 0;       /* 旧节点设置为蛇身 */
    NewTail->Record = 2;    /* 新节点设置为蛇尾 */
}

void InstanceNextTouch(uint32_t *X, uint32_t *Y)
{
    *X = rand() % SnakeWidth;
    *Y = rand() % SnakeHeight;
}

int main(int argc, char *argv[])
{
    SnakeBlock Matrix[SnakeHeight][SnakeWidth] = {0};
    
    Snake Instance = {0};
    Instance.Matrix  = Matrix;
    Instance.Width   = SnakeWidth;
    Instance.Height  = SnakeHeight;
    Instance.Create  = InstanceCreate;
    Instance.Destroy = InstanceDestroy;
    Instance.SetRecord1 = InstanceSetRecord1;
    Instance.SetRecord2 = InstanceSetRecord2;
    Instance.NextTouch  = InstanceNextTouch;
    
    /* 使用Map生成Matrix */
    for (uint32_t Y = 0; Y < SnakeHeight; Y++)
    for (uint32_t X = 0; X < SnakeWidth;  X++)
        Matrix[Y][X].Status = Map[Y][X];
    
    ConsoleExecuteSnake(&Instance, 200);
    return 0;
}
