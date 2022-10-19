#include "SGui_Kernel.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 画布单次推送屏幕(用于定向绘制) */
void SGui_EngineCanvasOnce(void (*HandleCanvas)(SGui_Canvas *Canvas), SGui_Area *Clip)
{
    uint32_t    Width  = 0;
    uint32_t    Height = 0;
    uint32_t    Length = 0;
    SGui_Pixel *Pixel  = NULL;
    /* 从屏幕获得一块像素点缓冲区 */
    SGui_ScreenTakeMap(&Pixel, &Length, &Width, &Height);
    /* 设置画布 */
    SGui_Canvas Canvas = {.Pixel = Pixel, .Length = Length};
    SGui_CanvasCleanPixel(&Canvas);
    SGui_AreaReset(&Canvas.Offset);
    SGui_AreaReset(&Canvas.Clip);
    SGui_CanvasSetClip(&Canvas, Clip);
    /* 使用该画布 */
    HandleCanvas(&Canvas);
    /* 将画布推送到屏幕 */
    SGui_ScreenCanvas(&Canvas);
    /* 向屏幕归还一块像素点缓冲区 */
    SGui_ScreenGiveMap(Pixel);
    /* 画布帧同步 */
    SGui_ScreenCanvasSync();
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 画布循环迭代推屏(用于重新推屏) */
void SGui_EngineCanvasRoll(void (*HandleCanvas)(SGui_Canvas *Canvas))
{
    uint32_t    Width  = 0;
    uint32_t    Height = 0;
    uint32_t    Length = 0;
    SGui_Pixel *Pixel  = NULL;
    static bool RePos_Valid = true;
    static uint32_t RePos_X = 0;
    static uint32_t RePos_Y = 0;
    while (RePos_Valid == true) {
        /* 从屏幕获得一块像素点缓冲区 */
        SGui_ScreenTakeMap(&Pixel, &Length, &Width, &Height);
        /* 重定向剪切域 */
        SGui_Area CanvasRegion = {.Off_X = RePos_X, .Off_W = Width,
                                  .Off_Y = RePos_Y, .Off_H = Height,};
        SGui_AreaOffToPos(&CanvasRegion);
        /* 设置画布 */
        SGui_Canvas Canvas = {.Pixel = Pixel, .Length = Length};
        SGui_CanvasCleanPixel(&Canvas);
        SGui_AreaReset(&Canvas.Offset);
        SGui_AreaReset(&Canvas.Clip);
        SGui_CanvasSetClip(&Canvas, &CanvasRegion);
        /* 使用该画布 */
        HandleCanvas(&Canvas);
        /* 将画布推送到屏幕 */
        SGui_ScreenCanvas(&Canvas);
        /* 向屏幕归还一块像素点缓冲区 */
        SGui_ScreenGiveMap(Pixel);
        /* 画布帧同步 */
        SGui_ScreenCanvasSync();
        /* 预先滑动到下一块画布 */
        if ((RePos_X += Width) < SGui_ScreenWidth())
            continue;
        RePos_X = 0;
        if ((RePos_Y += Height) < SGui_ScreenHeight())
            continue;
        RePos_Y = 0;
        RePos_Valid = false;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
