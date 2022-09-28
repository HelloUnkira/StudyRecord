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
#include "Maze.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define MazeWidth      (30 * 3 + 2)
#define MazeHeight     (20 * 2 + 2)
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
        for (uint32_t X = 0; X < MazeWidth + 20; X++) {
            ConsoleJumpCursor(X, 0);
            printf("=");
            ConsoleJumpCursor(X, MazeHeight + 1);
            printf("=");
        }
        /* 画三条竖线(左中右边框) */
        for (uint32_t Y = 1; Y < MazeHeight + 1; Y++) {
            ConsoleJumpCursor(0, Y);
            printf("|");
            ConsoleJumpCursor(MazeWidth + 1, Y);
            printf("|");
            ConsoleJumpCursor(MazeWidth + 20 - 1, Y);
            printf("|");
        }
        /* 画右上角的固定文本信息 */
        ConsoleJumpCursor(MazeWidth + 2, 15 - 5);
        printf("Nothing Else");
        ConsoleJumpCursor(MazeWidth + 2, 15 - 3);
        printf("Waiting Addition");
        /* 画一条横线(右下隔断) */
        for (uint32_t X = MazeWidth + 2; X < MazeWidth + 20 - 1; X++) {
            ConsoleJumpCursor(X, 15);
            printf("=");
        }
        /* 画右下角的固定文本信息 */
        ConsoleJumpCursor(MazeWidth + 2, 15 + 1);
        printf("Up");
        ConsoleJumpCursor(MazeWidth + 2, 15 + 2);
        printf("Down");
        ConsoleJumpCursor(MazeWidth + 2, 15 + 3);
        printf("Left");
        ConsoleJumpCursor(MazeWidth + 2, 15 + 4);
        printf("Right");
        ConsoleJumpCursor(MazeWidth + 2, 15 + 5);
        printf("Reflush:Space");
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define MazeWidth       30
#define MazeHeight      20
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static uint32_t ConsoleGetColorByIndex(uint32_t Index)
{
    switch (Index) {
    case 1: return 14;
    case 2: return 13;
    case 3: return 12;
    case 4: return 10;
    default: return 7;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 绘制迷宫 */
static void ConsoleDrawMaze(MazeBlock Matrix[MazeHeight][MazeWidth])
{
    /* 墙体的有效范围是单元格中间的部分 */
    for (uint32_t Y = 0; Y < MazeHeight; Y++)
    for (uint32_t X = 0; X < MazeWidth; X++) {
        
        ConsoleJumpCursor(X * 3 + 2, Y * 2 + 2);
        uint32_t Color = ConsoleGetColorByIndex(Matrix[Y][X].Record);
        ConsoleSetColor(Color);
        if (Color == 7)
            printf("  ");
        else
            printf("[]");
        ConsoleSetColor(7);
        ConsoleJumpCursor(X * 3 + 4, Y * 2 + 2);
        if (Matrix[Y][X].V_Status)
            printf("|");
        else
            printf(" ");
        ConsoleJumpCursor(X * 3 + 2, Y * 2 + 3);
        if (Matrix[Y][X].H_Status)
            printf("--+");
        else
            printf("  +");
    }
    
    ConsoleSetColor(7);
    /* 绘制四个边界 */
    for (uint32_t Y = 0; Y < MazeHeight; Y++) {
        ConsoleJumpCursor(1, Y * 2 + 1);
        printf("+");
        ConsoleJumpCursor(1, Y * 2 + 2);
        printf("|");
        ConsoleJumpCursor((MazeWidth) * 3 + 1, Y * 2 + 1);
        printf("+");
        ConsoleJumpCursor((MazeWidth) * 3 + 1, Y * 2 + 2);
        printf("|");
    }
    for (uint32_t X = 0; X < MazeWidth; X++) {
        ConsoleJumpCursor(X * 3 + 1, 1);
        printf("+--");
        ConsoleJumpCursor(X * 3 + 1, (MazeHeight) * 2 + 1);
        printf("+--");
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void ConsoleExecuteMaze(Maze *Instance)
{
    srand((unsigned int)time(NULL));
    system("title Maze");                   //设置cmd窗口名字
    system("mode con lines=44 cols=112");   //设置cmd窗口大小
    /* 隐藏光标 */
    ConsoleHideCursor();
    /* 绘制游戏背景 */
    ConsoleDrawBackground();
    /* 开始迷宫生产流程 */
    while (true) {
        /* 设置起始点,结束点,过程点,当前点颜色编号 */
        Instance->RecordS.Record = 1;
        Instance->RecordE.Record = 2;
        Instance->RecordT.Record = 3;
        Instance->RecordC.Record = 4;
        /* 清除历史路径状态 */
        for (uint32_t Offset = 0; Offset < Instance->Lines * Instance->Elements; Offset++)
            Instance->Matrix[Offset].Record = 0;
        /* 准备一个迷宫 */
        if (!Maze_Ready(Instance)) {
            system("cls");
            ConsoleSetColor(7);
            ConsoleJumpCursor(0, 0);
            printf("Reflush Error");
            while (true);
        }
        /**/
        while (true) {
            /* 刷屏更新(Reflush) */
            ConsoleDrawMaze(Instance->Matrix);
            /* 按键事件监听 */
            while (kbhit() == 0);
            /* 产生了按键事件,获取按键码 */
            char Command = getch();
            /* 按下空格刷新 */
            if (Command == 32)
                break;
            /* 否则执行对应的动作 */
            bool Status = false;
            switch (Command) {
            case 72: Status = Maze_Execute(Instance, Maze_Up);      break;
            case 80: Status = Maze_Execute(Instance, Maze_Down);    break;
            case 75: Status = Maze_Execute(Instance, Maze_Left);    break;
            case 77: Status = Maze_Execute(Instance, Maze_Right);   break;
            default: Status = true; break;
            }
            if (Status == false)
                break;
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
