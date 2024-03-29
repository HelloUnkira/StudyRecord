#ifndef SNAKE_H
#define SNAKE_H
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
//#pargma pack(1)
typedef struct SnakeBlockDescriptor {
    uint8_t Status:1;   /* 地图该点是否为障碍物 */
    uint8_t Record:7;   /* 外部信息,可以是颜色索引 */
} SnakeBlock;
//#pargma pack()
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SnakeNodeDescriptor {
    DL_Node  Node;      /* 内部维护:蛇身链表节点 */
    uint32_t PosX;      /* 内部维护:蛇身节点坐标 */
    uint32_t PosY;      /* 内部维护:蛇身节点坐标 */
    uint8_t  Record;    /* 外部信息:可以是颜色索引 */
} SnakeNode;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef enum SnakeWayDescriptor {
    Snake_None = 0,
    Snake_Up,
    Snake_Down,
    Snake_Left,
    Snake_Right,
} SnakeWay;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SnakeDescriptor {
    SnakeBlock *Matrix; /* 地图信息(不携带其他内容简要扩充即可) */
    uint32_t Width;     /* 地图宽度 */
    uint32_t Height;    /* 地图高度 */
    DL_List  List;      /* 贪吃蛇蛇身链表 */
    void   (*Create)(uint32_t Size, void **Pointer);    /* 蛇身节点生成 */
    void   (*Destroy)(void *Pointer);                   /* 蛇身节点销毁 */
    void   (*SetRecord1)(SnakeNode *OldHead, SnakeNode *Node);  /* 蛇节点新增头时设置其他信息 */
    void   (*SetRecord2)(SnakeNode *NewTail, SnakeNode *Node);  /* 蛇节点销毁尾时设置其他信息 */
    void   (*NextTouch)(uint32_t *X, uint32_t *Y);  /* 生成下一个触碰点的坐标 */
    uint32_t TouchPosX;                             /* 蛇头触碰该点使其变长 */
    uint32_t TouchPosY;                             /* 要避免生成障碍物和蛇体 */
    SnakeWay CurrentWay;
} Snake;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
SnakeNode * Snake_GetHead(Snake *Instance);
SnakeNode * Snake_GetTail(Snake *Instance);
SnakeNode * SnakeNode_GetPrev(SnakeNode *Instance);
SnakeNode * SnakeNode_GetNext(SnakeNode *Instance);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 向后遍历宏 */
#define Snake_Traverse_Backward(Instance, Node) \
    for (SnakeNode *Node = Snake_GetHead(Instance); Node != NULL; Node = SnakeNode_GetNext(Node))
/* 向前遍历宏 */
#define Snake_Traverse_Forward(Instance, Node)  \
    for (SnakeNode *Node = Snake_GetTail(Instance); Node != NULL; Node = SnakeNode_GetPrev(Node))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
bool Snake_Ready(Snake *Instance);
bool Snake_Execute(Snake *Instance, SnakeWay Way);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
