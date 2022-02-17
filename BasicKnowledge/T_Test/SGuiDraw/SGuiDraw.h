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

/* 缓冲区单元类型(这决定了你内存的占用情况): */
#define SGUIPIXEL       uint32_t
/* 当前平台字节对齐类型(它应该满足:sizeof(void *)): */
#define SGUITYPEALIGN   uint64_t
/* 地址对齐配置:sizeof(SGUITYPEALIGN) / sizeof(SGUIPIXEL) */
/* 修改缓冲区单元值它应该被同步变化,无需地址对齐时默认1 */
/* 预编译执行时理论先于sizeof,所以此处只能静态设置(暂未知解决方法) */
#define SGUIALIGN      2
/* 宽高布局:0;高宽布局:1;(通常不会用到后者) */
#define SGUILAYOUT     0

/*****************************************************************************/

/* 绘制空间 */
typedef struct SimpleGuiBuffer {
    SGUIPIXEL *buffer; /* 缓冲区(像素点) */
    SGUIPIXEL  color;  /* 像素点颜色值 */
    uint32_t   width;  /* 缓冲区宽度 */
    uint32_t   height; /* 缓冲区高度 */
} SGuiBuffer;

/*****************************************************************************/

/* 行列坐标:(x,y)转缓冲区偏移量 */
/* 程序扫描转换时均采用行列扫描 */

/* 缓冲区布局为:宽*高时(常规:像素点一行一行存) */
#define SGUIOFF1(instance, x, y) ((x) + (y) * (instance.width))
/* 缓冲区布局为:高*宽时(常规:像素点一列一列存) */
#define SGUIOFF2(instance, x, y) ((y) + (x) * (instance.height))

#if (SGUILAYOUT == 0)
#define SGUIOFF(instance, x, y) SGUIOFF1(instance, x, y)
#endif
#if (SGUILAYOUT == 1)
#define SGUIOFF(instance, x, y) SGUIOFF2(instance, x, y)
#endif

#ifndef SGUIOFF
#error "SGUIOFF:unsupported layout way"
#endif

/*****************************************************************************/

/* 地址未对齐点绘制: */
#define SGUIDRAWDOT(instance, value, x, y) \
    (instance.buffer)[SGUIOFF(instance, x, y)] = (value)

/* 地址对齐点绘制: */
#if (SGUIALIGN != 1)
/*
 * 此处采用的是妥协的方法,这种策略存在一定的局限性
 * 事实上只是勉强够用而已,无法预料未来会怎么样
 */
#define SGUICOLORALIGN1(value)      (value)
#define SGUICOLORALIGN2(value)                 \
    (SGUITYPEALIGN)(((SGUITYPEALIGN)(SGUICOLORALIGN1(value))) | \
                   (((SGUITYPEALIGN)(SGUICOLORALIGN1(value))) << (sizeof(SGUIPIXEL) * 1)))
#define SGUICOLORALIGN4(value)                 \
    (SGUITYPEALIGN)(((SGUITYPEALIGN)(SGUICOLORALIGN2(value))) | \
                   (((SGUITYPEALIGN)(SGUICOLORALIGN2(value))) << (sizeof(SGUIPIXEL) * 2)))
#define SGUICOLORALIGN8(value)                 \
    (SGUITYPEALIGN)(((SGUITYPEALIGN)(SGUICOLORALIGN4(value))) | \
                   (((SGUITYPEALIGN)(SGUICOLORALIGN4(value))) << (sizeof(SGUIPIXEL) * 4)))

#if (SGUIALIGN == 2)
#define SGUICOLORALIGN(value) SGUICOLORALIGN2(value)
#endif
#if (SGUIALIGN == 4)
#define SGUICOLORALIGN(value) SGUICOLORALIGN4(value)
#endif
#if (SGUIALIGN == 8)
#define SGUICOLORALIGN(value) SGUICOLORALIGN8(value)
#endif

#ifndef SGUICOLORALIGN
#error "SGUICOLORALIGN:unsupported alignment way"
#endif

#define SGUIDRAWDOTALIGNCHECK(instance, x, y) \
    (((uint8_t)(SGUITYPEALIGN)(&((instance.buffer)[SGUIOFF(instance, x, y)]))) % sizeof(SGUITYPEALIGN))

#define SGUIDRAWDOTALIGN(instance, value, x, y) \
    *((SGUITYPEALIGN *)(&((instance.buffer)[SGUIOFF(instance, x, y)]))) = SGUICOLORALIGN(value)

#endif

/*****************************************************************************/

/* 逻辑化简 */
#define SGUIDIS(data1, data2) (SGUIMAX(data1, data2) - SGUIMIN(data1, data2))

/*****************************************************************************/

/* 绘制序列 */
typedef struct SimpleGuiSequence {
    int32_t *x_point;
    int32_t *y_point;
    uint32_t  length;
} SGuiSequence;

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

void SGuiDrawLine(SGuiLine line);
void SGuiDrawVerticalLine(SGuiLine line);   /* 绘制:垂直线(内部使用,无检查) */
void SGuiDrawHorizontalLine(SGuiLine line); /* 绘制:水平线(内部使用,无检查) */

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

void SGuiDrawRectangle(SGuiRectangle rectangle);

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

void SGuiDrawCircle(SGuiCircle circle);

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

void SGuiDrawEllipse(SGuiEllipse ellipse);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#endif
