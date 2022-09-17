#ifndef TETRIS_H
#define TETRIS_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define TetrisGraphScale    4
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 内部使用:图元类型: */
uint32_t TetrisGraph_GetNumber(void);
/* 内部使用:图元状态:旋转时使用 */
uint32_t TetrisGraph_GetWheel(uint32_t Type);
/* 内部使用:图元大小:阶数*阶数 */
uint32_t TetrisGraph_GetScale(uint32_t Type);
/* 内部使用:获得一个指定类型指定状态的图元地址 */
bool *   TetrisGraph_GetGraph(uint32_t Type, uint32_t Wheel);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//#pargma pack(1)
typedef struct TetrisBlockDescriptor {
    uint8_t Status:1;   /* 内部维护:方块是否存在(1存在,0不存在) */
    uint8_t Record:7;   /* 方块记录, 可以是颜色索引或者其他 */
} TetrisBlock;
//#pargma pack()
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct TetrisDescriptor {
    TetrisBlock *Matrix;    /* 方块矩阵(第四象限) */
    uint32_t Elements;      /* 方块矩阵行元素 */
    uint32_t Lines;         /* 方块矩阵行数量 */
    uint32_t EraseLine;     /* 数据记录 */
    uint32_t (*GetType)(void);                                  /* 下一图元类型 */
    void     (*SetRecord)(uint32_t Type, TetrisBlock *Block);   /* 当前图元类型设置其他参数 */
    /* 回调1:实现对不同类型图元按概率刷取 */
    /*       取值范围为:[0, TetrisGraph_GetNumber() - 1] */
    /* 回调2:设置图元除Status以外的其他参数 */
    /*       该单次的设置会同等的作用到该图元的所有方块上 */
    /*       参数为图元的类型与一个交互样板地址 */
    /* 内部使用的状态迭代,仅读取显示: */
    TetrisBlock Graph[TetrisGraphScale][TetrisGraphScale]; /* 当前图元实体 */
    uint32_t Type;      /* 当前图元类型 */
    uint32_t Wheel;     /* 当前图元轮数 */
    uint32_t Scale;     /* 当前图元阶数 */
    uint32_t PosX;      /* 图元左上坐标点X */
    uint32_t PosY;      /* 图元左上坐标点Y */
    /* 内部使用的状态迭代,仅读取显示: */
    TetrisBlock GraphNext[TetrisGraphScale][TetrisGraphScale]; /* 下一图元实体 */
    uint32_t TypeNext;  /* 下一图元类型 */
    uint32_t WheelNext; /* 当前图元轮数 */
    uint32_t ScaleNext; /* 当前图元阶数 */
    /* 内部使用的状态迭代 */
    uint32_t Step;
} Tetris;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 目标图元向左移动,无法移动则不移动 */
bool Tetris_MoveLeft(Tetris *Instance);
/* 目标图元向左移动,无法移动则不移动 */
bool Tetris_MoveRight(Tetris *Instance);
/* 目标图元向下移动,无法移动则不移动 */
bool Tetris_MoveStep(Tetris *Instance);
/* 目标图元向左旋转(逆时针),无法旋转则不旋转 */
bool Tetris_RotateLeft(Tetris *Instance);
/* 目标图元向右旋转(顺时针),无法旋转则不旋转 */
bool Tetris_RotateRight(Tetris *Instance);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 准备一个俄罗斯方块流程 */
bool Tetris_Ready(Tetris *Instance);
/* 定时事件循环(它需要从已经准备好的流程开始执行) */
bool Tetris_Iterate(Tetris *Instance);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
