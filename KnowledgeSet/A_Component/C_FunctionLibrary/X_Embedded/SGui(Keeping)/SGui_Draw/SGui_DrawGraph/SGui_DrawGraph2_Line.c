#include "SGui_Draw.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static bool SGui_DrawLineSimple(SGui_Canvas *Canvas, SGui_Dot *S, SGui_Dot *E, SGui_Pixel Pixel)
{
    SGui_Area Dot2 = {0};
    SGui_AreaFromDot2(&Dot2, S, E);
    SGui_Area Offset = Dot2;
    if (SGui_CanvasPosToOff(Canvas, &Offset) == false)
        return false;
    
    /* 垂直线 */
    if (Offset.Off_W == 1) {
        for (SGui_Coord Y = Offset.Off_Y; Y < Offset.Off_Y + Offset.Off_H; Y++)
            SGui_CanvasPixel(Canvas, Offset.Off_X, Y, Pixel);
        return true;
    }
    /* 水平线 */
    if (Offset.Off_H == 1) {
        for (SGui_Coord X = Offset.Off_X; X < Offset.Off_X + Offset.Off_W; X++)
            SGui_CanvasPixel(Canvas, X, Offset.Off_Y, Pixel);
        return true;
    }
    
    /* 对角线 */
    if (Offset.Off_H == Offset.Off_W) {
        SGui_Coord Steps = (Offset.Off_H, Offset.Off_W);
        /* 斜下对角线 */
        if ((S->X <= E->X && S->Y <= E->Y) ||
            (S->X >= E->X && S->Y >= E->Y)) {
            for (SGui_Coord Step = 0; Step < Steps; Step++) {
                SGui_Coord X = Offset.Off_X + Step;
                SGui_Coord Y = Offset.Off_Y + Step;
                SGui_CanvasPixel(Canvas, X, Y, Pixel);
            }
            return true;
        }
        /* 斜上对角线 */
        if ((S->X <= E->X && S->Y >= E->Y) ||
            (S->X >= E->X && S->Y <= E->Y)) {
            for (SGui_Coord Step = 0; Step < Steps; Step++) {
                SGui_Coord X = Offset.Off_X + Step;
                SGui_Coord Y = Offset.Off_Y - Step + Steps - 1;
                SGui_CanvasPixel(Canvas, X, Y, Pixel);
            }
            return true;
        }
    }
    return false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static bool SGui_DrawLineOrdinary(SGui_Canvas *Canvas, SGui_Dot *S, SGui_Dot *E, SGui_Pixel Pixel)
{
    SGui_Area Dot2 = {0};
    SGui_AreaFromDot2(&Dot2, S, E);
    SGui_Area Offset = Dot2;
    if (SGui_CanvasPosToOff(Canvas, &Offset) == false)
        return false;
    
    /* 注意于Y的正方向是向下 */
    uint8_t Flag = ~0;
    /* 归一化:1.斜率大于0 */
    if ((E->X > S->X && E->Y > S->Y) ||
        (E->X < S->X && E->Y < S->Y)) {
        /* 斜率:(0,1) */
        if (Offset.Off_W > Offset.Off_H)
            Flag = 0;
        /* 斜率:(1,+oo) */
        if (Offset.Off_W < Offset.Off_H)
            Flag = 2;
    }
    /* 归一化:2.斜率小于0 */
    if ((E->X > S->X && E->Y < S->Y) ||
        (E->X < S->X && E->Y > S->Y)) {
        /* 斜率:(0,1) */
        if (Offset.Off_W > Offset.Off_H)
            Flag = 1;
        /* 斜率:(1,+oo) */
        if (Offset.Off_W < Offset.Off_H)
            Flag = 3;
    }
    
    /* 作部分预处理,因为线的绘制是无向的 */
    /* 此处便保证绘制是正向递增的 */
    SGui_Dot D_S = *S;
    SGui_Dot D_E = *E;
    if (((Flag == 0 || Flag == 1) && S->X > E->X) ||
        ((Flag == 2 || Flag == 3) && S->Y > E->Y)) {
        SGui_Dot T = {0};
         T = *E;
        *E = *S;
        *S = T;
    }
    /* 中点线扫描转化 */
    SGui_Coord Dx = SGui_Dis(S->X, E->X);
    SGui_Coord Dy = SGui_Dis(S->Y, E->Y);
    /*  */
    SGui_Coord X = Offset.Off_X;
    SGui_Coord Y = Offset.Off_Y;
    /* 绘制 */
    if (Flag == 0 || Flag == 1) {
        SGui_Coord Df = 2 * (Dy) - Dx;  /* 导数:斜率 */
        SGui_Coord Ls = 2 * (Dy);       /* 左直 */
        SGui_Coord Lu = 2 * (Dy - Dx);  /* 左上 */
        if (Flag == 0)
            do {
                SGui_CanvasPixel(Canvas, X, Y, Pixel);
                Y  += ((Df >  0) ?  1 :  0);
                Df += ((Df <= 0) ? Ls : Lu);
            } while (++X <= Offset.Off_X + Offset.Off_W);
        if (Flag == 0)
            do {
                SGui_CanvasPixel(Canvas, X, Y, Pixel);
                Y  -= ((Df >  0) ?  1 :  0);
                Df += ((Df <= 0) ? Ls : Lu);
            } while (++X <= Offset.Off_X + Offset.Off_W);
    }
    if (Flag == 2 || Flag == 3) {
        SGui_Coord Df = 2 * (Dx) - Dy;  /* 导数:斜率 */
        SGui_Coord Us = 2 * (Dx);       /* 上直 */
        SGui_Coord Ul = 2 * (Dx - Dy);  /* 上左 */
        if (Flag == 0)
            do {
                SGui_CanvasPixel(Canvas, X, Y, Pixel);
                X  += ((Df >  0) ?  1 :  0);
                Df += ((Df <= 0) ? Us : Ul);
            } while (++Y < Offset.Off_Y + Offset.Off_H);
        if (Flag == 0)
            do {
                SGui_CanvasPixel(Canvas, X, Y, Pixel);
                X  -= ((Df >  0) ?  1 :  0);
                Df += ((Df <= 0) ? Us : Ul);
            } while (++Y < Offset.Off_Y + Offset.Off_H);
    }
    *S = D_S;
    *E = D_E;
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_DrawLine(SGui_Canvas *Canvas, SGui_Dot *S, SGui_Dot *E, SGui_Pixel Pixel)
{
    if (SGui_ClipRegionCheck3(&Canvas->Clip, S, E) == false)
        return;
    if (SGui_DrawLineSimple(Canvas, S, E, Pixel) == true)
        return;
    if (SGui_DrawLineOrdinary(Canvas, S, E, Pixel) == true)
        return;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
