/* 模组唯一依赖项 */
#include "SGuiDraw.h"

/* 绘制:填充矩形 */
static void SGuiDrawSimpleFillRectangle(SGuiRectangle *rectangle)
{
    int32_t x = 0;
    int32_t y = 0;
    SGuiBuffer *buffer = &(rectangle->instance);

    for (y = rectangle->y_left; y < rectangle->height; y++)
    for (x = rectangle->x_left; x < rectangle->width;  x++)
        SGuiDrawDot(buffer, x, y);
}

/* 绘制:矩形 */
void SGuiDrawRectangle(SGuiRectangle *rectangle)
{
    if (rectangle->type == 0)
        SGuiDrawSimpleFillRectangle(rectangle);
    if (rectangle->type == 1);
}
