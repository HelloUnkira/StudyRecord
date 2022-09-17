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
#include "Tetris.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define TetrisElements  20
#define TetrisLines     30
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
        for (uint32_t X = 0; X < TetrisElements * 2 + 20; X++) {
            ConsoleJumpCursor(X, 0);
            printf("=");
            ConsoleJumpCursor(X, TetrisLines + 1);
            printf("=");
        }
        /* 画三条竖线(左中右边框) */
        for (uint32_t Y = 1; Y < TetrisLines + 1; Y++) {
            ConsoleJumpCursor(0, Y);
            printf("|");
            ConsoleJumpCursor(TetrisElements * 2 + 1, Y);
            printf("|");
            ConsoleJumpCursor(TetrisElements * 2 + 20 - 1, Y);
            printf("|");
        }
        /* 画右上部固定文本信息 */
        ConsoleJumpCursor(TetrisElements * 2 + 2, 1);
        printf("Next Block:");
        /* 画一条横线(右上隔断) */
        for (uint32_t X = TetrisElements * 2 + 2; X < TetrisElements * 2 + 20 - 1; X++) {
            ConsoleJumpCursor(X, 2);
            printf("=");
        }
        /* 画右中部固定文本信息 */
        ConsoleJumpCursor(TetrisElements * 2 + 2, 10);
        printf("Count:");
        ConsoleJumpCursor(TetrisElements * 2 + 2, 12);
        printf("Speed:");
        ConsoleJumpCursor(TetrisElements * 2 + 2, 14);
        printf("Erase Line:");
        /* 画三条横线(右中隔断) */
        for (uint32_t X = TetrisElements * 2 + 2; X < TetrisElements * 2 + 20 - 1; X++) {
            ConsoleJumpCursor(X,  9);
            printf("=");
            ConsoleJumpCursor(X, 11);
            printf("=");
            ConsoleJumpCursor(X, 13);
            printf("=");
            ConsoleJumpCursor(X, 15);
            printf("=");
        }
        /* 画右下角的固定文本信息 */
        ConsoleJumpCursor(TetrisElements * 2 + 2, 15 + 1);
        printf("MoveStep:  S");
        ConsoleJumpCursor(TetrisElements * 2 + 2, 15 + 2);
        printf("MoveLeft:  A & <-");
        ConsoleJumpCursor(TetrisElements * 2 + 2, 15 + 3);
        printf("MoveRight: D & ->");
        ConsoleJumpCursor(TetrisElements * 2 + 2, 15 + 4);
        printf("RotateLeft:  Q");
        ConsoleJumpCursor(TetrisElements * 2 + 2, 15 + 5);
        printf("RotateRight: E");
        ConsoleJumpCursor(TetrisElements * 2 + 2, 15 + 6);
        printf("SpeedHigh: Down");
        ConsoleJumpCursor(TetrisElements * 2 + 2, 15 + 7);
        printf("SpeedLow:  Up");
        
        ConsoleJumpCursor(TetrisElements * 2 + 2, 15 + 15);
        printf("Game Running");
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void ConsoleDrawCountAndSpeed(uint32_t Count, uint32_t Speed)
{
    ConsoleSetColor(7);
    ConsoleJumpCursor(TetrisElements * 2 + 2, 10);
    ConsoleJumpCursor(TetrisElements * 2 + 2 + strlen("Count:"), 10);
    printf("    ");
    ConsoleJumpCursor(TetrisElements * 2 + 2 + strlen("Count:"), 10);
    printf("%u", Count);
    ConsoleJumpCursor(TetrisElements * 2 + 2 + strlen("Speed:"), 12);
    printf("    ");
    ConsoleJumpCursor(TetrisElements * 2 + 2 + strlen("Speed:"), 12);
    printf("%u", Speed);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void ConsoleDrawEraseLine(uint32_t EraseLine)
{
    ConsoleSetColor(7);
    ConsoleJumpCursor(TetrisElements * 2 + 2 + strlen("Erase Line:"), 14);
    printf("    ");
    ConsoleJumpCursor(TetrisElements * 2 + 2 + strlen("Erase Line:"), 14);
    printf("%u", EraseLine);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void ConsoleDrawTetris(TetrisBlock Matrix[TetrisLines][TetrisElements])
{
    for (uint32_t Y = 0; Y < TetrisLines; Y++)
    for (uint32_t X = 0; X < TetrisElements; X++) {
        
        ConsoleJumpCursor(X * 2 + 1, Y + 1);
        
        if (Matrix[Y][X].Status) {
            ConsoleSetColor(Matrix[Y][X].Record);
            printf("[]");
        } else
            printf("  ");
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void ConsoleDrawTetrisGraph(TetrisBlock Graph[TetrisGraphScale][TetrisGraphScale],
                                   uint32_t Scale, uint32_t PosX, uint32_t PosY)
{
    for (uint32_t Y = 0; Y < Scale; Y++)
    for (uint32_t X = 0; X < Scale; X++) {
        
        ConsoleJumpCursor((PosX + X) * 2 + 1, (PosY + Y) + 1);
        
        if (Graph[Y][X].Status) {
            ConsoleSetColor(Graph[Y][X].Record);
            printf("<>");
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void ConsoleDrawTetrisGraphNext(TetrisBlock Graph[TetrisGraphScale][TetrisGraphScale],
                                       uint32_t Scale)
{
    for (uint32_t Y = 0; Y < TetrisGraphScale; Y++)
    for (uint32_t X = 0; X < TetrisGraphScale; X++) {
        ConsoleJumpCursor(TetrisElements * 2 + 3 + X * 2 + 1, 4 + Y);
        printf("  ");
    }
    
    for (uint32_t Y = 0; Y < Scale; Y++)
    for (uint32_t X = 0; X < Scale; X++) {
        ConsoleJumpCursor(TetrisElements * 2 + 3 + X * 2 + 1, 4 + Y);
        if (Graph[Y][X].Status) {
            ConsoleSetColor(Graph[Y][X].Record);
            printf("<>");
        }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void ConsoleExecuteTetris(Tetris *Instance, uint32_t Speed)
{
    srand((unsigned int)time(NULL));
    system("title Tetris");                 //设置cmd窗口名字
    system("mode con lines=32 cols=60");    //设置cmd窗口大小
    /* 隐藏光标 */
    ConsoleHideCursor();
    /* 绘制游戏背景 */
    ConsoleDrawBackground();
    /* 准备一个Tetris流程 */
    if (Tetris_Ready(Instance) == false) {
        system("cls");
        ConsoleSetColor(7);
        ConsoleJumpCursor(0, 0);
        printf("Game Over");
        while (true);
    }
    bool PauseFlag = false;
    bool ReflushFlag = true;
    uint32_t Timestamp = 0;
    /* 开始事件循环 */
    while (true) {
        if (ReflushFlag) {
            ReflushFlag = false;
            /* 刷屏更新(Reflush) */
            ConsoleDrawTetris((TetrisBlock **)Instance->Matrix);
            ConsoleDrawTetrisGraph(Instance->Graph, Instance->Scale, Instance->PosX, Instance->PosY);
            ConsoleDrawTetrisGraphNext(Instance->GraphNext, Instance->ScaleNext);
            ConsoleDrawEraseLine(Instance->EraseLine);
        }
        /* 重置时间戳 */
        Timestamp = (Timestamp == 0 || PauseFlag) ? Speed * 100 : Timestamp;
        /* 按键事件监听 */
        while ((PauseFlag || --Timestamp) && kbhit() == 0) {
            ConsoleDrawCountAndSpeed(Timestamp, Speed);
        }
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
            case  72: if (Speed < 100) Speed++;
                      ReflushFlag = true;
                      break;
            case  80: if (Speed > 1)   Speed--;
                      ReflushFlag = true;
                      break;
            case 115: CommandResult = Tetris_MoveStep(Instance);
                      if (CommandResult == false) Timestamp = 1;
                      ReflushFlag = true; break;
            case  75:
            case  97: CommandResult = Tetris_MoveLeft(Instance);
                      ReflushFlag = true; break;
            case  77:
            case 100: CommandResult = Tetris_MoveRight(Instance);
                      ReflushFlag = true;
                      break;
            case 113: CommandResult = Tetris_RotateLeft(Instance);
                      ReflushFlag = true;
                      break;
            case 101: CommandResult = Tetris_RotateRight(Instance);
                      ReflushFlag = true;
                      break;
            case  32: PauseFlag = true; break;
            }
        } else {
            ReflushFlag = true;
            if (Tetris_Iterate(Instance))
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
