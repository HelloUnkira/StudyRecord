#ifndef SGUI_PIXEL_H
#define SGUI_PIXEL_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 实现目标:
 * 定义简洁通配的基本图元
 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 基本图元描述:颜色 */
#define SimpleGui_Color uint8_t
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 基本图元描述:颜色 */
typedef SimpleGui_Color SGui_Color;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 基本图元描述:像素点 */
typedef struct SimpleGui_Pixel {
    uint32_t Red:8;
    uint32_t Green:8;
    uint32_t Blue:8;
} SGui_Pixel;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline SGui_Pixel SGui_PixelAlphaBlend(SGui_Pixel Pixel, uint8_t Alpha)
{
    Pixel.Red   = (uint32_t)(Pixel.Red)   * Alpha / 255;
    Pixel.Green = (uint32_t)(Pixel.Green) * Alpha / 255;
    Pixel.Blue  = (uint32_t)(Pixel.Blue)  * Alpha / 255;
    
    return Pixel;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
