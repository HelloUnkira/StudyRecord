/* 模组唯一依赖项 */
#include "SGuiDraw.h"

/* 绘制:圆上中心对称的8个点 */
static void SGuiDrawCircleDot8(SGuiCircle *circle)
{
    /* 绘制点与中心点关系: */
    int32_t x = 0;
    int32_t y = 0;
    SGuiBuffer *buffer = &(circle->instance);
    /* 相对0坐标 */
    /* (x,y) */
    x = circle->x_center + circle->x_current;
    y = circle->y_center + circle->y_current;
    SGuiDrawDot(buffer, x, y);
    /* (y,x) */
    x = circle->x_center + circle->y_current;
    y = circle->y_center + circle->x_current;
    SGuiDrawDot(buffer, x, y);
    /* (y,-x) */
    x = circle->x_center + circle->y_current;
    y = circle->y_center - circle->x_current;
    SGuiDrawDot(buffer, x, y);
    /* (x,-y) */
    x = circle->x_center + circle->x_current;
    y = circle->y_center - circle->y_current;
    SGuiDrawDot(buffer, x, y);
    /* (-x,-y) */
    x = circle->x_center - circle->x_current;
    y = circle->y_center - circle->y_current;
    SGuiDrawDot(buffer, x, y);
    /* (-y,-x) */
    x = circle->x_center - circle->y_current;
    y = circle->y_center - circle->x_current;
    SGuiDrawDot(buffer, x, y);
    /* (-y,x) */
    x = circle->x_center - circle->y_current;
    y = circle->y_center + circle->x_current;
    SGuiDrawDot(buffer, x, y);
    /* (-x,y) */
    x = circle->x_center - circle->x_current;
    y = circle->y_center + circle->y_current;
    SGuiDrawDot(buffer, x, y);
}

/* 绘制:圆上中心对称的4个点(边界) */
static void SGuiDrawCircleDot4(SGuiCircle *circle)
{
    /* 绘制点与中心点关系: */
    int32_t x = 0;
    int32_t y = 0;
    SGuiBuffer *buffer = &(circle->instance);
    /* 相对0坐标 */
    /* (x,y) */
    x = circle->x_center + circle->x_current;
    y = circle->y_center + circle->y_current;
    SGuiDrawDot(buffer, x, y);
    /* (y,-x) */
    x = circle->x_center + circle->y_current;
    y = circle->y_center - circle->x_current;
    SGuiDrawDot(buffer, x, y);
    /* (-x,-y) */
    x = circle->x_center - circle->x_current;
    y = circle->y_center - circle->y_current;
    SGuiDrawDot(buffer, x, y);
    /* (-y,x) */
    x = circle->x_center - circle->y_current;
    y = circle->y_center + circle->x_current;
    SGuiDrawDot(buffer, x, y);
}

/* 绘制:圆上中心对称的8条线 */
static void SGuiDrawCircleLine8_0(SGuiCircle *circle)
{
    /* 扩展方法:依赖基础方法 */
    /* 偷了点懒,直接调用函数,而不是复刻 */
    SGuiLine line = {0};
    line.instance = circle->instance;
    /* 绘制点与中心点关系: */
    /* 相对0坐标 */
    /* (x,y) */
    line.x_start  = circle->x_center + 1;
    line.y_start  = circle->y_center + circle->y_current;
    line.x_end    = circle->x_center + circle->x_current;
    line.y_end    = circle->y_center + circle->y_current;
    SGuiDrawHorizontalLine(&line);
    /* (y,x) */
    line.x_start  = circle->x_center + 1;
    line.y_start  = circle->y_center + circle->x_current;
    line.x_end    = circle->x_center + circle->y_current;
    line.y_end    = circle->y_center + circle->x_current;
    SGuiDrawHorizontalLine(&line);
    /* (y,-x) */
    line.x_start  = circle->x_center + 1;
    line.y_start  = circle->y_center - circle->x_current;
    line.x_end    = circle->x_center + circle->y_current;
    line.y_end    = circle->y_center - circle->x_current;
    SGuiDrawHorizontalLine(&line);
    /* (x,-y) */
    line.x_start  = circle->x_center + 1;
    line.y_start  = circle->y_center - circle->y_current;
    line.x_end    = circle->x_center + circle->x_current;
    line.y_end    = circle->y_center - circle->y_current;
    SGuiDrawHorizontalLine(&line);
    /* (-x,-y) */
    line.x_start  = circle->x_center - circle->x_current;
    line.y_start  = circle->y_center - circle->y_current;
    line.x_end    = circle->x_center - 1;
    line.y_end    = circle->y_center - circle->y_current;
    SGuiDrawHorizontalLine(&line);
    /* (-y,-x) */
    line.x_start  = circle->x_center - circle->y_current;
    line.y_start  = circle->y_center - circle->x_current;
    line.x_end    = circle->x_center - 1;
    line.y_end    = circle->y_center - circle->x_current;
    SGuiDrawHorizontalLine(&line);
    /* (-y,x) */
    line.x_start  = circle->x_center - circle->y_current;
    line.y_start  = circle->y_center + circle->x_current;
    line.x_end    = circle->x_center - 1;
    line.y_end    = circle->y_center + circle->x_current;
    SGuiDrawHorizontalLine(&line);
    /* (-x,y) */
    line.x_start  = circle->x_center - circle->x_current;
    line.y_start  = circle->y_center + circle->y_current;
    line.x_end    = circle->x_center - 1;
    line.y_end    = circle->y_center + circle->y_current;
    SGuiDrawHorizontalLine(&line);
}

/* 绘制:圆上中心对称的4条线 */
static void SGuiDrawCircleLine8_1(SGuiCircle *circle)
{
    /* 扩展方法:依赖基础方法 */
    /* 偷了点懒,直接调用函数,而不是复刻 */
    SGuiLine line = {0};
    line.instance = circle->instance;
    /* 绘制点与中心点关系: */
    /* 相对0坐标 */
    /* (y,x) */
    line.x_start  = circle->x_center + 1;
    line.y_start  = circle->y_center + circle->x_current;
    line.x_end    = circle->x_center + circle->y_current - 1;
    line.y_end    = circle->y_center + circle->x_current;
    SGuiDrawHorizontalLine(&line);
    /* (y,-x) */
    line.x_start  = circle->x_center + 1;
    line.y_start  = circle->y_center - circle->x_current;
    line.x_end    = circle->x_center + circle->y_current - 1;
    line.y_end    = circle->y_center - circle->x_current;
    SGuiDrawHorizontalLine(&line);
    /* (-y,-x) */
    line.x_start  = circle->x_center - circle->y_current + 1;
    line.y_start  = circle->y_center - circle->x_current;
    line.x_end    = circle->x_center - 1;
    line.y_end    = circle->y_center - circle->x_current;
    SGuiDrawHorizontalLine(&line);
    /* (-y,x) */
    line.x_start  = circle->x_center - circle->y_current + 1;
    line.y_start  = circle->y_center + circle->x_current;
    line.x_end    = circle->x_center - 1;
    line.y_end    = circle->y_center + circle->x_current;
    SGuiDrawHorizontalLine(&line);
}

/* 绘制:圆上中心对称的4条线(坐标轴,及原点) */
static void SGuiDrawCircleLine4_0(SGuiCircle *circle)
{
    /* 原点的绘制补写此处 */
    int32_t x = circle->x_center;
    int32_t y = circle->y_center;
    SGuiBuffer *buffer = &(circle->instance);
    SGuiDrawDot(buffer, x, y);
    /* 扩展方法:依赖基础方法 */
    /* 偷了点懒,直接调用函数,而不是复刻 */
    SGuiLine line = {0};
    line.instance = circle->instance;
    /* 坐标线y: */
    line.y_start  = circle->y_center;
    line.y_end    = circle->y_center;
    /* 坐标线x: */
    line.x_start  = circle->x_center - circle->radius;
    line.x_end    = circle->x_center - 1;
    SGuiDrawHorizontalLine(&line);
    line.x_start  = circle->x_center + 1;
    line.x_end    = circle->x_center + circle->radius;
    SGuiDrawHorizontalLine(&line);
    /* 坐标线x: */
    line.x_start  = circle->x_center;
    line.x_end    = circle->x_center;
    /* 坐标线y: */
    line.y_start  = circle->y_center - circle->radius;
    line.y_end    = circle->y_center - 1;
    SGuiDrawVerticalLine(&line);
    line.y_start  = circle->y_center + 1;
    line.y_end    = circle->y_center + circle->radius;
    SGuiDrawVerticalLine(&line);
}

/* 绘制:圆上中心对称的4条线(边界) */
static void SGuiDrawCircleLine4_1(SGuiCircle *circle)
{
    /* 扩展方法:依赖基础方法 */
    /* 偷了点懒,直接调用函数,而不是复刻 */
    SGuiLine line = {0};
    line.instance = circle->instance;
    /* 绘制点与中心点关系: */
    /* 相对0坐标 */
    /* (x,y) */
    line.x_start  = circle->x_center + 1;
    line.y_start  = circle->y_center + circle->y_current;
    line.x_end    = circle->x_center + circle->x_current;
    line.y_end    = circle->y_center + circle->y_current;
    SGuiDrawHorizontalLine(&line);
    /* (y,-x) */
    line.x_start  = circle->x_center + 1;
    line.y_start  = circle->y_center - circle->x_current;
    line.x_end    = circle->x_center + circle->y_current;
    line.y_end    = circle->y_center - circle->x_current;
    SGuiDrawHorizontalLine(&line);
    /* (-x,-y) */
    line.x_start  = circle->x_center - circle->x_current;
    line.y_start  = circle->y_center - circle->y_current;
    line.x_end    = circle->x_center - 1;
    line.y_end    = circle->y_center - circle->y_current;
    SGuiDrawHorizontalLine(&line);
    /* (-y,x) */
    line.x_start  = circle->x_center - circle->y_current;
    line.y_start  = circle->y_center + circle->x_current;
    line.x_end    = circle->x_center - 1;
    line.y_end    = circle->y_center + circle->x_current;
    SGuiDrawHorizontalLine(&line);
}

/* 绘制:一个空心圆 */
static void SGuiDrawHollowCircle(SGuiCircle *circle)
{
    int32_t df = 1 - circle->radius;
    int32_t ls = 3;
    int32_t ld = 5 - circle->radius * 2;

    circle->x_current = 0;
    circle->y_current = circle->radius;
    SGuiDrawCircleDot4(circle);

    while (circle->y_current > circle->x_current) {
        circle->x_current++;
        circle->y_current -= ((df >= 0) ? 1 : 0);
        SGuiDrawCircleDot8(circle);
        if (df  < 0) {
            df += ls;
            ls += 2;
            ld += 2;
            continue;
        }
        if (df >= 0) {
            df += ld;
            ls += 2;
            ld += 4;
            continue;
        }
    }

    if (circle->y_current < circle->x_current)
        return;

    SGuiDrawCircleDot4(circle);
}

/* 绘制:一个填充圆 */
static void SGuiDrawFillCircle(SGuiCircle *circle)
{
    int32_t df = 1 - circle->radius;
    int32_t ls = 3;
    int32_t ld = 5 - circle->radius * 2;

    circle->x_current = 0;
    circle->y_current = circle->radius;
    SGuiDrawCircleLine4_0(circle);

    while (circle->y_current > circle->x_current) {
        circle->x_current++;
        circle->y_current -= ((df >= 0) ? 1 : 0);

        if (df  < 0) {
            SGuiDrawCircleDot8(circle);
            SGuiDrawCircleLine8_1(circle);
        }

        if (df >= 0)
            SGuiDrawCircleLine8_0(circle);

        if (df  < 0) {
            df += ls;
            ls += 2;
            ld += 2;
            continue;
        }
        if (df >= 0) {
            df += ld;
            ls += 2;
            ld += 4;
            continue;
        }
    }

    if (circle->y_current < circle->x_current)
        return;

    SGuiDrawCircleLine4_1(circle);
}

/* 绘制:一个环带圆 */
static void SGuiDrawBandCircle(SGuiCircle *circle)
{
    int32_t df = 1 - circle->radius;
    int32_t ls = 3;
    int32_t ld = 5 - circle->radius * 2;

    circle->x_current = 0;
    circle->y_current = circle->radius;

    int32_t temp = circle->y_current, i = 0;
    for (i = 0; i < circle->band; i++) {
        circle->y_current = temp - i;
        SGuiDrawCircleDot4(circle);
    }
    circle->y_current = temp;

    while (circle->y_current > circle->x_current) {
        circle->x_current++;
        circle->y_current -= ((df >= 0) ? 1 : 0);

        int32_t temp = circle->y_current, i = 0;
        for (i = 0; i < circle->band; i++) {
            circle->y_current = temp - i;
            SGuiDrawCircleDot8(circle);
        }
        circle->y_current = temp;


        if (df  < 0) {
            df += ls;
            ls += 2;
            ld += 2;
            continue;
        }
        if (df >= 0) {
            df += ld;
            ls += 2;
            ld += 4;
            continue;
        }
    }

    if (circle->y_current < circle->x_current)
        return;

    SGuiDrawCircleDot4(circle);

}

/* 绘制:一个圆 */
void SGuiDrawCircle(SGuiCircle *circle)
{
    switch (circle->type) {
    case 0: SGuiDrawHollowCircle(circle);    return;
    case 1: SGuiDrawFillCircle(circle);      return;
    case 2: SGuiDrawBandCircle(circle);      return;
    }
}
