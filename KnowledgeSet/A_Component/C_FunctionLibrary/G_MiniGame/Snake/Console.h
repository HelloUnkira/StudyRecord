#ifndef CONSOLE_H
#define CONSOLE_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "Snake.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define SnakeWidth      20
#define SnakeHeight     30
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//隐藏光标(API)
static void ConsoleHideCursor(void)
{
    CONSOLE_CURSOR_INFO Cursor;
    Cursor.dwSize = 1;
    Cursor.bVisible = FALSE;
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorInfo(Handle, &Cursor);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//颜色设置(API)
static void ConsoleSetColor(int Color)
{
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE); 
    SetConsoleTextAttribute(Handle, Color);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//光标跳转(API)
static void ConsoleJumpCursor(int X, int Y)
{
    COORD Coord;
    Coord.X = X;
    Coord.Y = Y;
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(Handle, Coord);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 背景色绘制 */
static void ConsoleDrawBackground(void)
{
    /* 写字笔设置为白色 */
    ConsoleSetColor(7);
    /* 不动元素 */ {
        /* 画俩条横线(上下边框) */
        for (uint32_t X = 0; X < SnakeWidth * 2 + 20; X++) {
            ConsoleJumpCursor(X, 0);
            printf("=");
            ConsoleJumpCursor(X, SnakeHeight + 1);
            printf("=");
        }
        /* 画三条竖线(左中右边框) */
        for (uint32_t Y = 1; Y < SnakeHeight + 1; Y++) {
            ConsoleJumpCursor(0, Y);
            printf("|");
            ConsoleJumpCursor(SnakeWidth * 2 + 1, Y);
            printf("|");
            ConsoleJumpCursor(SnakeWidth * 2 + 20 - 1, Y);
            printf("|");
        }
        /* 画一条横线(右下隔断) */
        for (uint32_t X = SnakeWidth * 2 + 2; X < SnakeWidth * 2 + 20 - 1; X++) {
            ConsoleJumpCursor(X, 15);
            printf("=");
        }
        /* 画右下角的固定文本信息 */
        ConsoleJumpCursor(SnakeWidth * 2 + 2, 15 + 1);
        printf("Up");
        ConsoleJumpCursor(SnakeWidth * 2 + 2, 15 + 2);
        printf("Down");
        ConsoleJumpCursor(SnakeWidth * 2 + 2, 15 + 3);
        printf("Left");
        ConsoleJumpCursor(SnakeWidth * 2 + 2, 15 + 4);
        printf("Right");
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 画背景墙 */
static void ConsoleDrawBlock(SnakeBlock Matrix[SnakeHeight][SnakeWidth])
{
    for (uint32_t Y = 0; Y < SnakeHeight; Y++)
    for (uint32_t X = 0; X < SnakeWidth;  X++) {
        
        ConsoleJumpCursor(X * 2 + 1, Y + 1);
        
        if (Matrix[Y][X].Status) {
            ConsoleSetColor(11);
            printf("[]");
        } else
            printf("  ");
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 画蛇 */
static void ConsoleDrawSnakeAndFood(Snake *Instance)
{
    Snake_Traverse_Backward(Instance, Node) {
        
        ConsoleJumpCursor(Node->PosX * 2 + 1, Node->PosY + 1);
        
        if (Node->Record == 0)
           ConsoleSetColor(5);
        else if (Node->Record == 1)
            ConsoleSetColor(9);
        else if (Node->Record == 2)
            ConsoleSetColor(15);
        else
            ConsoleSetColor(5);
        
        printf("()");
    }
    
    ConsoleJumpCursor(Instance->TouchPosX * 2 + 1, Instance->TouchPosY + 1);
    ConsoleSetColor(10);
    printf("<>");
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void ConsoleExecuteSnake(Snake *Instance, uint32_t Speed)
{
    srand((unsigned int)time(NULL));
    system("title Snake");                  //设置cmd窗口名字
    system("mode con lines=32 cols=60");    //设置cmd窗口大小
    /* 隐藏光标 */
    ConsoleHideCursor();
    /* 绘制游戏背景 */
    ConsoleDrawBackground();
    /* 准备一个Snake流程 */
    if (Snake_Ready(Instance) == false) {
        system("cls");
        ConsoleSetColor(7);
        ConsoleJumpCursor(0, 0);
        printf("Game Over");
        while (true);
    }
    
    bool PauseFlag = false;
    bool ReflushFlag = true;
    uint32_t Timestamp = 0;
    SnakeWay Way = Snake_None;
    /* 开始事件循环 */
    while (true) {
        if (ReflushFlag) {
            ReflushFlag = false;
            /* 刷屏更新(Reflush) */
            ConsoleDrawBlock((SnakeBlock **)Instance->Matrix);
            ConsoleDrawSnakeAndFood(Instance);
        }
        /* 重置时间戳 */
        Timestamp = (Timestamp == 0 || PauseFlag) ? Speed * 100 : Timestamp;
        /* 按键事件监听 */
        while ((PauseFlag || --Timestamp) && kbhit() == 0);
        /* 如果事件未完整的结束一个周期 */
        if (Timestamp != 0) {
            /* 产生了按键事件,获取按键码 */
            char Command = getch();
            /* 暂停情况下的按键需要额外过滤 */
            if (PauseFlag) {
                if (Command == 32)
                    PauseFlag = false;
                continue;
            }
            /* 下面的代码放在main中可以抓取键值 */
            // for (char ch = '\0'; (ch=getch())!=0x1B; printf("%d ", ch));
            bool CommandResult = false;
            /* 抓到对应键值执行对应动作 */
            switch (Command) {
            case 72: Way = Snake_Up; break;
            case 80: Way = Snake_Down; break;
            case 75: Way = Snake_Left; break;
            case 77: Way = Snake_Right; break;
            case 32: PauseFlag = true; break;
            }
        } else {
            ReflushFlag = true;
            if (Way == Snake_None)
                continue;
            if (Snake_Execute(Instance, Way))
                continue;
            else {
                system("cls");
                ConsoleSetColor(7);
                ConsoleJumpCursor(0, 0);
                printf("Game Over");
                while (true);
            }
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
