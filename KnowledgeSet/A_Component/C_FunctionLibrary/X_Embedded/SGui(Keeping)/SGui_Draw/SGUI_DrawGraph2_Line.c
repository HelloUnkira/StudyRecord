#include "SGui_Draw.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static bool SGui_DrawSimpleLine(SGui_Canvas *Canvas, SGui_Dot *S, SGui_Dot *E, SGui_Pixel Pixel)
{
    /* 将点S到点E形成的区域 */
    SGui_Area Dot2 = {0};
    SGui_AreaFromDot2(&Dot2, S, E);
    SGui_Area Offset = {0};
    SGui_ClipRegionAnd(&Offset, &Canvas->Clip, &Dot2);
    /* 如果没有落在当前剪切域内,不进行绘制 */
    if (SGui_ClipRegionIsVaild(&Offset) == false)
        return false;
    SGui_AreaPosToOff(&Offset);
    /* 垂直线 */
    if (Offset.Off_W == 1) {
        for (SGui_Coord Y = 0; Y < Offset.Off_H; Y++)
            SGui_CanvasOffset(Canvas, Offset.Off_X, Y) = Pixel;
        return true;
    }
    /* 水平线 */
    if (Offset.Off_H == 1) {
        for (SGui_Coord X = 0; X < Offset.Off_W; X++)
            SGui_CanvasOffset(Canvas, X, Offset.Off_Y) = Pixel;
        return true;
    }
    /* 对角线 */
    if (Offset.Off_H == Offset.Off_W) {
        SGui_Coord Steps = (Offset.Off_H, Offset.Off_W);
        /* 斜下对角线 */
        if ((S->X <= E->X && S->Y <= E->Y) ||
            (S->X >= E->X && S->Y >= E->Y)) {
            for (SGui_Coord Step = 0; Step < Steps; Step++)
                SGui_CanvasOffset(Canvas, Step, Step) = Pixel;
            return true;
        }
        /* 斜上对角线 */
        if ((S->X <= E->X && S->Y >= E->Y) ||
            (S->X >= E->X && S->Y <= E->Y)) {
            for (SGui_Coord Step = 0; Step < Steps; Step++)
                SGui_CanvasOffset(Canvas, Step, Steps - 1 - Step) = Pixel;
            return true;
        }
    }
    return false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static bool SGui_DrawOrdinaryLine(SGui_Canvas *Canvas, SGui_Area *Dot2, SGui_Pixel Pixel)
{
    SGui_Area Offset = {0};
    SGui_ClipRegionAnd(&Offset, &Canvas->Clip, Dot2);
    /* 如果没有落在当前剪切域内,不进行绘制 */
    if (SGui_ClipRegionIsVaild(&Offset) == false)
        return false;
    SGui_AreaPosToOff(&Offset);
    
    
    
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/




