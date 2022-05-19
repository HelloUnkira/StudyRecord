/* 模组唯一依赖项 */
#include "SGuiDraw.h"

/* 绘制:椭圆上中心对称的4个点 */
static void SGuiDrawEllipseDot1(SGuiEllipse *ellipse)
{
    /* 绘制点与中心点关系: */
    int32_t x = 0;
    int32_t y = 0;
    SGuiBuffer *buffer = &(ellipse->instance);
    /* 相对0坐标 */
    /* (x,y) */
    x = ellipse->x_center + ellipse->x_current;
    y = ellipse->y_center + ellipse->y_current;
    SGuiDrawDot(buffer, x, y);
    /* (x,-y) */
    x = ellipse->x_center + ellipse->x_current;
    y = ellipse->y_center - ellipse->y_current;
    SGuiDrawDot(buffer, x, y);
    /* (-x,y) */
    x = ellipse->x_center - ellipse->x_current;
    y = ellipse->y_center + ellipse->y_current;
    SGuiDrawDot(buffer, x, y);
    /* (-x,-y) */
    x = ellipse->x_center - ellipse->x_current;
    y = ellipse->y_center - ellipse->y_current;
    SGuiDrawDot(buffer, x, y);
}

/* 绘制:椭圆上中心对称的4个点(边界) */
static void SGuiDrawEllipseDot2(SGuiEllipse *ellipse)
{
    /* 绘制点与中心点关系: */
    int32_t x = 0;
    int32_t y = 0;
    SGuiBuffer *buffer = &(ellipse->instance);
    /* 相对0坐标 */
    x = ellipse->x_center + ellipse->x_radius;
    y = ellipse->y_center + 0;
    SGuiDrawDot(buffer, x, y);
    x = ellipse->x_center - ellipse->x_radius;
    y = ellipse->y_center + 0;
    SGuiDrawDot(buffer, x, y);
    x = ellipse->x_center + 0;
    y = ellipse->y_center + ellipse->y_radius;
    SGuiDrawDot(buffer, x, y);
    x = ellipse->x_center + 0;
    y = ellipse->y_center - ellipse->y_radius;
    SGuiDrawDot(buffer, x, y);
}

/* 绘制:一个空心椭圆(进行简要缩放移除浮点运算) */
static void SGuiDrawHollowEllipse(SGuiEllipse *ellipse)
{
    int32_t a = ellipse->x_radius;
    int32_t b = ellipse->y_radius;
    /* 绘制边界 */
    SGuiDrawEllipseDot2(ellipse);
    /* 以梯度为-1分割为俩个区域 */
    ellipse->x_current = 0;
    ellipse->y_current = b;
    /* 检测梯度,是否第一区域 */
    int32_t d1 = b * b * 4 + a * a * (1 - b * 4);
    while (1) {
        int32_t temp1 = a * a * (2 * ellipse->y_current - 1);
        int32_t temp2 = b * b * (2 * ellipse->x_current + 2);
        /* temp1 > temp2 */
        if (temp1 <= temp2)
            break;

        if (d1 >= 0)
            d1 += a * a * (2 - 2 * (ellipse->y_current--)) * 4;
            d1 += b * b * (3 + 2 * (ellipse->x_current++)) * 4;

        SGuiDrawEllipseDot1(ellipse);
    }
    /* 检测梯度,是否第二区域 */
    int32_t d2 = 0 - a * a * b * b * 4;/* 太长了... */
    d2 += b * b * (ellipse->x_current * 2 + 1) * (ellipse->x_current * 2 + 1);
    d2 += a * a * (ellipse->y_current * 2 - 2) * (ellipse->y_current * 2 - 2);
    while (1) {
        /* ellipse->y_current > 0 */
        if (ellipse->y_current <= 0)
            break;

        if (d2 < 0)
            d2 += b * b * (2 + 2 * (ellipse->x_current++)) * 4;
            d2 += a * a * (3 - 2 * (ellipse->y_current--)) * 4;

        SGuiDrawEllipseDot1(ellipse);
    }
}

/* 绘制:椭圆上中心对称的4条线 */
static void SGuiDrawEllipseLine1(SGuiEllipse *ellipse)
{
    /* 扩展方法:依赖基础方法 */
    /* 偷了点懒,直接调用函数,而不是复刻 */
    SGuiLine line = {0};
    line.instance = ellipse->instance;
    /* 绘制点与中心点关系: */
    /* 相对0坐标 */
    /* (x,y) */
    line.x_start  = ellipse->x_center + 1;
    line.y_start  = ellipse->y_center + ellipse->y_current;
    line.x_end    = ellipse->x_center + ellipse->x_current;
    line.y_end    = ellipse->y_center + ellipse->y_current;
    SGuiDrawHorizontalLine(&line);
    /* (x,-y) */
    line.x_start  = ellipse->x_center + 1;
    line.y_start  = ellipse->y_center - ellipse->y_current;
    line.x_end    = ellipse->x_center + ellipse->x_current;
    line.y_end    = ellipse->y_center - ellipse->y_current;
    SGuiDrawHorizontalLine(&line);
    /* (-x,y) */
    line.x_start  = ellipse->x_center - ellipse->x_current;
    line.y_start  = ellipse->y_center + ellipse->y_current;
    line.x_end    = ellipse->x_center - 1;
    line.y_end    = ellipse->y_center + ellipse->y_current;
    SGuiDrawHorizontalLine(&line);
    /* (-x,-y) */
    line.x_start  = ellipse->x_center - ellipse->x_current;
    line.y_start  = ellipse->y_center - ellipse->y_current;
    line.x_end    = ellipse->x_center - 1;
    line.y_end    = ellipse->y_center - ellipse->y_current;
    SGuiDrawHorizontalLine(&line);
}

/* 绘制:椭圆上中心对称的4条线(边界) */
static void SGuiDrawEllipseLine2(SGuiEllipse *ellipse)
{
    /* 原点的绘制补写此处 */
    int32_t x = ellipse->x_center;
    int32_t y = ellipse->y_center;
    SGuiBuffer *buffer = &(ellipse->instance);
    SGuiDrawDot(buffer, x, y);
    /* 扩展方法:依赖基础方法 */
    /* 偷了点懒,直接调用函数,而不是复刻 */
    SGuiLine line = {0};
    line.instance = ellipse->instance;
    /* 绘制点与中心点关系: */
    /* 相对0坐标 */
    line.x_start  = ellipse->x_center + 1;
    line.y_start  = ellipse->y_center + 0;
    line.x_end    = ellipse->x_center + ellipse->x_radius;
    line.y_end    = ellipse->y_center + 0;
    SGuiDrawHorizontalLine(&line);
    line.x_start  = ellipse->x_center - ellipse->x_radius;
    line.y_start  = ellipse->y_center + 0;
    line.x_end    = ellipse->x_center - 1;
    line.y_end    = ellipse->y_center + 0;
    SGuiDrawHorizontalLine(&line);
    line.x_start  = ellipse->x_center + 0;
    line.y_start  = ellipse->y_center + 1;
    line.x_end    = ellipse->x_center + 0;
    line.y_end    = ellipse->y_center + ellipse->y_radius;
    SGuiDrawVerticalLine(&line);
    line.x_start  = ellipse->x_center + 0;
    line.y_start  = ellipse->y_center - ellipse->y_radius;
    line.x_end    = ellipse->x_center + 0;
    line.y_end    = ellipse->y_center - 1;
    SGuiDrawVerticalLine(&line);
}

/* 绘制:一个填充椭圆(进行简要缩放移除浮点运算) */
static void SGuiDrawFillEllipse(SGuiEllipse *ellipse)
{
    int32_t a = ellipse->x_radius;
    int32_t b = ellipse->y_radius;
    /* 绘制边界 */
    SGuiDrawEllipseLine2(ellipse);
    /* 以梯度为-1分割为俩个区域 */
    ellipse->x_current = 0;
    ellipse->y_current = b;
    /* 检测梯度,是否第一区域 */
    int32_t d1 = b * b * 4 + a * a * (1 - b * 4);
    while (1) {
        int32_t temp1 = a * a * (2 * ellipse->y_current - 1);
        int32_t temp2 = b * b * (2 * ellipse->x_current + 2);
        /* temp1 > temp2 */
        if (temp1 <= temp2)
            break;

        bool temp = (d1 >= 0) ? true : false;
        
        if (d1 >= 0)
            d1 += a * a * (2 - 2 * (ellipse->y_current--)) * 4;
            d1 += b * b * (3 + 2 * (ellipse->x_current++)) * 4;

        if (temp == false)
            SGuiDrawEllipseDot1(ellipse);
        if (temp == true)
            SGuiDrawEllipseLine1(ellipse);
    }
    /* 检测梯度,是否第二区域 */
    int32_t d2 = 0 - a * a * b * b * 4;/* 太长了... */
    d2 += b * b * (ellipse->x_current * 2 + 1) * (ellipse->x_current * 2 + 1);
    d2 += a * a * (ellipse->y_current * 2 - 2) * (ellipse->y_current * 2 - 2);
    while (1) {
        /* ellipse->y_current > 0 */
        if (ellipse->y_current <= 0)
            break;

        uint8_t temp = (d1 >= 0) ? 1 : 0;

        if (d2 < 0)
            d2 += b * b * (2 + 2 * (ellipse->x_current++)) * 4;
            d2 += a * a * (3 - 2 * (ellipse->y_current--)) * 4;

        if (temp == 0)
            SGuiDrawEllipseDot1(ellipse);
        if (temp == 1)
            SGuiDrawEllipseLine1(ellipse);
    }
}

/* 绘制:一个椭圆 */
void SGuiDrawEllipse(SGuiEllipse *ellipse)
{
    if (ellipse->type == 0)
        SGuiDrawHollowEllipse(ellipse);
    if (ellipse->type == 1)
        SGuiDrawFillEllipse(ellipse);
    if (ellipse->type == 2);
}
