/* 模组唯一依赖项 */
#include "SGuiDraw.h"

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

/* 绘制:普通线 */
void SGuiDrawLine(SGuiLine *line)
{
    /* 绘制:垂直线,水平线,对角线 */
    if (SGuiDrawSimpleLine(line) == false)
        SGuiDrawOrdinaryLine(line);
}


/* 绘制:垂直线(内部使用,无检查) */
void SGuiDrawVerticalLine(SGuiLine *line)
{
    SGuiDrawSimpleLine(line);
}

/* 绘制:水平线(内部使用,无检查) */
void SGuiDrawHorizontalLine(SGuiLine *line)
{
    SGuiDrawSimpleLine(line);
}
