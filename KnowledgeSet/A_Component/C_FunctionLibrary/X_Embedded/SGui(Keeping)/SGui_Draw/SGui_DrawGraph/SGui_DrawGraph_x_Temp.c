#include "SGui_DrawGraph_x_Temp.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 下面的内容还未适配进入该层,暂未知好的适配方式,仅做参考 */
/* 目前有个问题未想清楚,面向画布剪切域绘制不是完全绘制 */
// #define SGUI_DRAW_CONFIG
// #define SGUI_DRAW_LINE_CONFIG
// #define SGUI_DRAW_CIRCLE_CONFIG
// #define SGUI_DRAW_ELLIPSE_CONFIG
// #define SGUI_DRAW_TEST_CONFIG
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_DRAW_CONFIG
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define SGUIPIXEL   uint32_t
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 绘制空间 */
typedef struct SimpleGuiBuffer {
    SGUIPIXEL *buffer; /* 缓冲区(像素点) */
    SGUIPIXEL  color;  /* 像素点颜色值 */
    uint32_t   width;  /* 缓冲区宽度 */
    uint32_t   height; /* 缓冲区高度 */
} SGuiBuffer;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#ifdef SGUI_DRAW_LINE_CONFIG
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SimpleGuiLine {
    SGuiBuffer instance;
    int32_t x_start;
    int32_t y_start;
    int32_t x_end;
    int32_t y_end;
} SGuiLine;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 绘制:垂直线,水平线,对角线 */
static uint8_t SGuiDrawSimpleLine(SGuiLine *line)
{
    int32_t x = line->x_start;
    int32_t y = line->y_start;
    SGuiBuffer *buffer = &(line->instance);
    /* 确认是垂直线 */
    if (line->x_start == line->x_end) {
        int32_t start = 0, end = 0, i = 0;
        start = SGUIMIN(line->y_start, line->y_end);
        end   = SGUIMAX(line->y_start, line->y_end);
        for (i = start; i <= end; i++)
            SGuiDrawDot(buffer, x, i);
        return true;
    }
    /* 确认是水平线 */
    if (line->y_start == line->y_end) {
        int32_t start = 0, end = 0, i = 0;
        start = SGUIMIN(line->x_start, line->x_end);
        end   = SGUIMAX(line->x_start, line->x_end);
        for (i = start; i <= end; i++)
            SGuiDrawDot(buffer, i, y);
        return true;
    }
    /* 确认是对角线 */
    if (SGUIDIS(line->x_start, line->x_end) ==
        SGUIDIS(line->y_start, line->y_end)) {
        int32_t step = 0, i = 0;
        step = SGUIDIS(line->y_start, line->y_end);
        /* 第一象限 */
        if (line->x_start < line->x_end && line->y_start < line->y_end)
            for (i = 0; i <= step; i++)
                SGuiDrawDot(buffer, x++, y++);
        /* 第二象限 */
        if (line->x_start > line->x_end && line->y_start < line->y_end)
            for (i = 0; i <= step; i++)
                SGuiDrawDot(buffer, x--, y++);
        /* 第三象限 */
        if (line->x_start > line->x_end && line->y_start > line->y_end)
            for (i = 0; i <= step; i++)
                SGuiDrawDot(buffer, x--, y--);
        /* 第四象限 */
        if (line->x_start < line->x_end && line->y_start > line->y_end)
            for (i = 0; i <= step; i++)
                SGuiDrawDot(buffer, x++, y--);
        return true;
    }
    return false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 绘制:普通线 */
static void SGuiDrawOrdinaryLine(SGuiLine *line)
{
    uint8_t flag = (~0);
    SGuiBuffer *buffer = &(line->instance);
    /* 第一类,斜率大于0: */
    if ((line->x_end > line->x_start && line->y_end > line->y_start) ||
        (line->x_end < line->x_start && line->y_end < line->y_start)) {
        /* 斜率满足:(0,1) */
        if (SGUIDIS(line->x_end, line->x_start) >
            SGUIDIS(line->y_end, line->y_start))
            flag = 0;
        /* 斜率满足:(1,+oo) */
        if (SGUIDIS(line->x_end, line->x_start) <
            SGUIDIS(line->y_end, line->y_start))
            flag = 2;
    }

    /* 第二类,斜率小于0: */
    if ((line->x_end > line->x_start && line->y_end < line->y_start) ||
        (line->x_end < line->x_start && line->y_end < line->y_start)) {
        /* 斜率满足:(-1,0)*/
        if (SGUIDIS(line->x_end, line->x_start) >
            SGUIDIS(line->y_end, line->y_start))
            flag = 1;
        /* 斜率满足:(-oo,-1) */
        if (SGUIDIS(line->x_end, line->x_start) <
            SGUIDIS(line->y_end, line->y_start))
            flag = 3;
    }

    /* 作部分预处理,因为线的绘制是无向的 */
    /* 此处便保证绘制是正向递增的 */
    if (((flag == 0 || flag == 1) && (line->x_end < line->x_start)) ||
        ((flag == 2 || flag == 3) && (line->y_end < line->y_start))) {
        /* 交换坐标点 */
        line->x_start = line->x_start ^ line->x_end;
        line->x_end   = line->x_start ^ line->x_end;
        line->x_start = line->x_start ^ line->x_end;
        line->y_start = line->y_start ^ line->y_end;
        line->y_end   = line->y_start ^ line->y_end;
        line->y_start = line->y_start ^ line->y_end;
    }
    /* 中点线扫描转化 */
    int32_t dx = SGUIDIS(line->x_end, line->x_start); /* 微元x */
    int32_t dy = SGUIDIS(line->y_end, line->y_start); /* 微元y */
    /*  */
    int32_t  x = line->x_start;
    int32_t  y = line->y_start;
    /* 绘制: */
    if (flag == 0 || flag == 1) {
        int32_t df = 2 * (dy) - dx;  /* 导数:斜率 */
        int32_t ls = 2 * (dy);       /* 左直 */
        int32_t lu = 2 * (dy - dx);  /* 左上 */
        if (flag == 0)
            do {
                SGuiDrawDot(buffer, x, y);
                y  += ((df > 0)  ?  1 :  0);
                df += ((df <= 0) ? ls : lu);
            } while (++x <= line->x_end);
        if (flag == 1)
            do {
                SGuiDrawDot(buffer, x, y);
                y  -= ((df > 0)  ?  1 :  0);
                df += ((df <= 0) ? ls : lu);
            } while (++x <= line->x_end);
    }
    if (flag == 2 || flag == 3) {
        int32_t df = 2 * (dx) - dy;  /* 导数:斜率 */
        int32_t us = 2 * (dx);       /* 上直 */
        int32_t ul = 2 * (dx - dy);  /* 上左 */
        if (flag == 2)
            do {
                SGuiDrawDot(buffer, x, y);
                x  += ((df > 0)  ?  1 :  0);
                df += ((df <= 0) ? us : ul);
            } while (++y <= line->y_end);
        if (flag == 3)
            do {
                SGuiDrawDot(buffer, x, y);
                x  -= ((df > 0)  ?  1 :  0);
                df += ((df <= 0) ? us : ul);
            } while (++y <= line->y_end);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 绘制:普通线 */
void SGuiDrawLine(SGuiLine *line)
{
    /* 绘制:垂直线,水平线,对角线 */
    if (SGuiDrawSimpleLine(line) == false)
        SGuiDrawOrdinaryLine(line);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 绘制:垂直线(内部使用,无检查) */
void SGuiDrawVerticalLine(SGuiLine *line)
{
    SGuiDrawSimpleLine(line);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 绘制:水平线(内部使用,无检查) */
void SGuiDrawHorizontalLine(SGuiLine *line)
{
    SGuiDrawSimpleLine(line);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#ifdef SGUI_DRAW_CIRCLE_CONFIG
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 绘制圆 */
typedef struct SimpleGuiCircle {
    SGuiBuffer instance;
    int32_t x_center;
    int32_t y_center;
    int32_t radius;
    int32_t band;      /* 环带宽度(扩展为奇数) */
    int32_t type;      /* 圆类型(0:空心;1:填充;2:环带; */
    int32_t x_current; /* 内部使用,设置为0 */
    int32_t y_current; /* 内部使用,设置为0 */
} SGuiCircle;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 绘制:一个圆 */
void SGuiDrawCircle(SGuiCircle *circle)
{
    switch (circle->type) {
    case 0: SGuiDrawHollowCircle(circle);    return;
    case 1: SGuiDrawFillCircle(circle);      return;
    case 2: SGuiDrawBandCircle(circle);      return;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#ifdef SGUI_DRAW_ELLIPSE_CONFIG
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 绘制椭圆 */
typedef struct SimpleGuiEllipse {
    SGuiBuffer instance;
    int32_t x_center;
    int32_t y_center;
    int32_t x_radius;
    int32_t y_radius;
    int32_t type;        /* 椭圆类型(0:空心;1:填充;) */
    int32_t x_current;   /* 内部使用,设置为0 */
    int32_t y_current;   /* 内部使用,设置为0 */
} SGuiEllipse;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
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
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 绘制:一个椭圆 */
void SGuiDrawEllipse(SGuiEllipse *ellipse)
{
    if (ellipse->type == 0)
        SGuiDrawHollowEllipse(ellipse);
    if (ellipse->type == 1)
        SGuiDrawFillEllipse(ellipse);
    if (ellipse->type == 2);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#ifdef  SGUI_DRAW_TEST_CONFIG
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGuiDrawLineTest(SGuiBuffer instance)
{
    SGuiLine line = {0};
    line.instance = instance;
    line.instance.color = 0xFFFFFFFF;

    /* 基础绘制: */
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 200;
    line.y_end    = 100;
    SGuiDrawLine(&line);
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 100;
    line.y_end    = 200;
    SGuiDrawLine(&line);
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 200;
    line.y_end    = 200;
    SGuiDrawLine(&line);
    line.x_start  = 200;
    line.y_start  = 100;
    line.x_end    = 200;
    line.y_end    = 200;
    SGuiDrawLine(&line);
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 200;
    line.y_end    = 200;
    SGuiDrawLine(&line);
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 200;
    line.y_end    = 100;
    SGuiDrawLine(&line);
    /* 绘制: */
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 200;
    line.y_end    = 150;
    SGuiDrawLine(&line);
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 200;
    line.y_end    = 125;
    SGuiDrawLine(&line);
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 200;
    line.y_end    = 112;
    SGuiDrawLine(&line);
    //
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 150;
    line.y_end    = 200;
    SGuiDrawLine(&line);
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 125;
    line.y_end    = 200;
    SGuiDrawLine(&line);
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 112;
    line.y_end    = 200;
    SGuiDrawLine(&line);
    //
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 200;
    line.y_end    = 150;
    SGuiDrawLine(&line);
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 200;
    line.y_end    = 175;
    SGuiDrawLine(&line);
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 200;
    line.y_end    = 183;
    SGuiDrawLine(&line);
    //
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 150;
    line.y_end    = 100;
    SGuiDrawLine(&line);
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 125;
    line.y_end    = 100;
    SGuiDrawLine(&line);
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 112;
    line.y_end    = 100;
    SGuiDrawLine(&line);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGuiDrawCricleTest(SGuiBuffer instance)
{
    SGuiCircle circle = {0};
    circle.instance = instance;
    circle.instance.color = 0xFFFFFFFF;

    circle.x_center   = 300;
    circle.y_center   = 100;
    circle.radius     = 50;
    circle.type       = 1;
    SGuiDrawCircle(&circle);
    circle.x_center   = 300;
    circle.y_center   = 250;
    circle.radius     = 50;
    circle.band       = 5;
    circle.type       = 2;
    SGuiDrawCircle(&circle);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGuiDrawEllipseTest(SGuiBuffer instance)
{
    SGuiEllipse ellipse = {0};
    ellipse.instance = instance;
    ellipse.instance.color = 0xFFFFFFFF;

    ellipse.x_center = 400;
    ellipse.y_center = 100;
    ellipse.x_radius = 50;
    ellipse.y_radius = 25;
    ellipse.type     = 1;
    SGuiDrawEllipse(&ellipse);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGuiDrawRectangleTest(SGuiBuffer instance)
{
    SGuiRectangle rectangle = {0};
    rectangle.instance = instance;
    rectangle.instance.color = 0xFFFFFFFF;

    rectangle.x_left = 0;
    rectangle.y_left = 0;
    rectangle.width  = 50;
    rectangle.height = 100;
    rectangle.type   = 0;
    SGuiDrawRectangle(&rectangle);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGuiDrawTest(SGuiBuffer instance)
{
    SGuiDrawLineTest(instance);
    SGuiDrawCricleTest(instance);
    SGuiDrawEllipseTest(instance);
    SGuiDrawRectangleTest(instance);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
