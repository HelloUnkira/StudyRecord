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
#include "Game2048.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define Game2048Scale  20
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
        for (uint32_t X = 0; X < Game2048Scale * 2 + 20; X++) {
            ConsoleJumpCursor(X, 0);
            printf("=");
            ConsoleJumpCursor(X, Game2048Scale + 1);
            printf("=");
        }
        /* 画三条竖线(左中右边框) */
        for (uint32_t Y = 1; Y < Game2048Scale + 1; Y++) {
            ConsoleJumpCursor(0, Y);
            printf("|");
            ConsoleJumpCursor(Game2048Scale * 2 + 1, Y);
            printf("|");
            ConsoleJumpCursor(Game2048Scale * 2 + 20 - 1, Y);
            printf("|");
        }
        /* 画右上角的固定文本信息 */
        ConsoleJumpCursor(Game2048Scale * 2 + 2, 1);
        printf("Game Running");
        /* 画三条横线(右中隔断) */
        for (uint32_t X = Game2048Scale * 2 + 2; X < Game2048Scale * 2 + 20 - 1; X++) {
            ConsoleJumpCursor(X, 15);
            printf("=");
        }
        /* 画右下角的固定文本信息 */
        ConsoleJumpCursor(Game2048Scale * 2 + 2, 15 + 1);
        printf("Up");
        ConsoleJumpCursor(Game2048Scale * 2 + 2, 15 + 2);
        printf("Down");
        ConsoleJumpCursor(Game2048Scale * 2 + 2, 15 + 3);
        printf("Left");
        ConsoleJumpCursor(Game2048Scale * 2 + 2, 15 + 4);
        printf("Right");
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 数字绘制 */
static void ConsoleDrawDigit(uint8_t *Matrix, uint32_t Scale)
{
    for (uint32_t Y = 0; Y < Scale; Y++)
    for (uint32_t X = 0; X < Scale; X++) {
        uint8_t Index = Matrix[Y * Scale + X];
        
        char String[5] = {0};
        for (uint32_t I = 0; I < 4; String[I++] = ' ');
        ConsoleJumpCursor(X * 4 + 3, Y * 4 + 2);
        printf(String);
        
        if (Index != 0)
            sprintf(String, "%u", 1 << Index);
        ConsoleJumpCursor(X * 4 + 3, Y * 4 + 2);
        printf(String);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static void ConsoleExecuteGame2048(Game2048 *Instance, uint8_t Difficult)
{
    srand((unsigned int)time(NULL));
    system("title 2048");                   //设置cmd窗口名字
    system("mode con lines=22 cols=60");    //设置cmd窗口大小
    /* 隐藏光标 */
    ConsoleHideCursor();
    /* 绘制游戏背景 */
    ConsoleDrawBackground();
    /* 准备一个2048流程 */
    if (Game2048_Ready(Instance, Difficult) == false) {
        system("cls");
        ConsoleSetColor(7);
        ConsoleJumpCursor(0, 0);
        printf("Game Over");
        while (true);
    }
    while (true) {
        /* 刷屏更新(Reflush) */
        ConsoleDrawDigit(Instance->Matrix, Instance->Scale);
        /* 按键事件监听 */
        while (kbhit() == 0);
        /* 下面的代码放在main中可以抓取键值 */
        // for (char ch = '\0'; (ch=getch())!=0x1B; printf("%d ", ch));
        /* 产生了按键事件,获取按键码 */
        char Command = getch();
        Game2048Way Way = Game2048_None;
        switch (Command) {
        case 72: Way = Game2048_Up; break;
        case 80: Way = Game2048_Down; break;
        case 75: Way = Game2048_Left; break;
        case 77: Way = Game2048_Right; break;
        }
        if (Way != Game2048_None) {
            if (!Game2048_Execute(Instance, Way)) {
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
