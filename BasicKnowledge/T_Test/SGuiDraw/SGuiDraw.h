#ifndef SGUIDRAW_H
#define SGUIDRAW_H
/* 标准库依赖 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
/* 平台依赖 */
#include "SGuiCommon.h"
#include "SGuiPort.h"

/* 
 * 设计目标:
 * 对缓冲区进行基本绘制动作(不进行容错纠察)
 * 上层服务项在最下方
 */

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 绘制点 */
#include "SGuiDot.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 绘制线 */
typedef struct SimpleGuiLine {
    SGuiBuffer instance;
    int32_t x_start;
    int32_t y_start;
    int32_t x_end;
    int32_t y_end;
} SGuiLine;

void SGuiDrawLine(SGuiLine *line);
/* 绘制:垂直线(内部使用,无检查) */
void SGuiDrawVerticalLine(SGuiLine *line);
/* 绘制:水平线(内部使用,无检查) */
void SGuiDrawHorizontalLine(SGuiLine *line);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 绘制矩形 */
/* 备注:空心矩形不支持,因为它等于四根线 */
typedef struct SimpleGuiRectangle {
    SGuiBuffer instance;
    int32_t x_left;
    int32_t y_left;
    int32_t width;
    int32_t height;
    int32_t type;    /* 矩形类型(0:填充矩形;) */
} SGuiRectangle;

void SGuiDrawRectangle(SGuiRectangle *rectangle);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
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

void SGuiDrawCircle(SGuiCircle *circle);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
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

void SGuiDrawEllipse(SGuiEllipse *ellipse);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#endif
