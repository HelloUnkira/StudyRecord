#include <stdio.h>
#include <stdlib.h>

#include "Console.h"

/* 图元刷取回调 */
uint32_t InstanceGetType(void)
{
    return rand() % TetrisGraph_GetNumber();
}

/* 颜色设置回调 */
void InstanceSetRecord(uint32_t Type, TetrisBlock *Block)
{
    uint8_t Color = 0;
    switch (Type) {
    case 0:  Color = 13; break;
    case 1:  Color = 12; break;
    case 2:
    case 3:  Color = 10; break;
    case 4:
    case 5:  Color = 14; break;
    case 6:  Color = 11; break;
    default: Color = 7;  break;
    }
    Block->Record = Color;
}

int main(int argc, char *argv[])
{
    TetrisBlock Matrix[TetrisLines][TetrisElements] = {0};
    
    Tetris Instance = {0};
    Instance.Elements  = TetrisElements;
    Instance.Lines     = TetrisLines;
    Instance.Matrix    = (TetrisBlock *)Matrix;
    Instance.EraseLine = 0;
    Instance.GetType   = InstanceGetType;
    Instance.SetRecord = InstanceSetRecord;
    
    ConsoleExecuteTetris(&Instance, 15);
    return 0;
}
