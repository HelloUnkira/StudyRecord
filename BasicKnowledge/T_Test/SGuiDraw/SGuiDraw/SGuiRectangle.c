/* 模组唯一依赖项 */
#include "SGuiDraw.h"

/* 绘制:填充矩形 */
static void SGuiDrawSimpleFillRectangle(SGuiRectangle rectangle)
{
    int32_t x = 0;
    int32_t y = 0;

    for (y = rectangle.y_left; y < rectangle.height; y++)
    for (x = rectangle.x_left; x < rectangle.width;  x++)
        SGUIDRAWDOT(rectangle.instance, rectangle.instance.color, x, y);
}

/* 绘制:填充矩形(地址对齐优化) */
/* 目前,还没写好,问题:SGUICOLORALIGN无效 */
static void SGuiDrawSimpleFillRectangle1(SGuiRectangle rectangle)
{
    int32_t x = 0;
    int32_t y = 0;

    for (y = rectangle.y_left; y < rectangle.height; y++)
    for (x = rectangle.x_left; x < rectangle.width;  x++) {
#if (SGUIALIGN == 1)
        SGUIDRAWDOT(rectangle.instance, rectangle.instance.color, x, y);
#endif
#if (SGUIALIGN != 1)
        /* 超出绘制域时(使用对齐绘制) */
        if (x + SGUIALIGN >= rectangle.width) {
            SGUIDRAWDOT(rectangle.instance, rectangle.instance.color, x, y);
            continue;
        }
        /* 地址不对齐时 */
        if (SGUIDRAWDOTALIGNCHECK(rectangle.instance, x, y) != 0) {
            SGUIDRAWDOT(rectangle.instance, rectangle.instance.color, x, y);
            continue;
        }
        /* 地址对齐时(此处的代码无意义,仅仅做对称式书写) */
        if (SGUIDRAWDOTALIGNCHECK(rectangle.instance, x, y) == 0);

            SGUIDRAWDOTALIGN(rectangle.instance, rectangle.instance.color, x, y);
            x += SGUIALIGN - 1;
#endif
    }
}

/* 绘制:矩形 */
void SGuiDrawRectangle(SGuiRectangle rectangle)
{
    if (rectangle.type == 0)
        SGuiDrawSimpleFillRectangle(rectangle);
        //SGuiDrawSimpleFillRectangle1(rectangle);
    if (rectangle.type == 1);
}
