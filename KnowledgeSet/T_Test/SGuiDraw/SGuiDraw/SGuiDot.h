#ifndef SGUIDRAWDOT_H
#define SGUIDRAWDOT_H
/* 标准库依赖 */
#include <stdint.h>
#include <stdbool.h>
/*实现目标:
 *基础图形绘制之像素点绘制
 */
/*****************************************************************************/
/*****************************************************************************/
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
/*****************************************************************************/
/*****************************************************************************/
/* 绘制空间 */
typedef struct SimpleGuiBuffer {
    SGUIPIXEL *buffer; /* 缓冲区(像素点) */
    SGUIPIXEL  color;  /* 像素点颜色值 */
    uint32_t   width;  /* 缓冲区宽度 */
    uint32_t   height; /* 缓冲区高度 */
} SGuiBuffer;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 缓冲区布局不支持检查 */
#if ((SGUILAYOUT != 0) && \
     (SGUILAYOUT != 1))
#error "SGuiOffset(instance, x, y) : unsupported layout way"
#else
/* 行列坐标:(x,y)转缓冲区偏移量 */
/* 程序扫描转换时均采用行列扫描 */
static inline uint32_t SGuiOffset(SGuiBuffer *instance, uint32_t x, uint32_t y)
{
/* 缓冲区布局为:宽*高时(常规:像素点一行一行存) */
#if (SGUILAYOUT == 0)
    return ((x) + (y) * (instance->width));
#endif
/* 缓冲区布局为:高*宽时(常规:像素点一列一列存) */
#if (SGUILAYOUT == 1)
    return ((x) + (y) * (instance->height));
#endif
}
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 像素点绘制: */
static inline void SGuiDrawDot(SGuiBuffer *instance, uint32_t x, uint32_t y)
{
    (instance->buffer)[SGuiOffset(instance, x, y)] = (instance->color);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 感觉无用,暂不移除 */
#if 0

/* 像素点绘制(地址对齐批量绘制):
 * 此处采用的是妥协的方法,这种策略存在一定的局限性
 * 事实上只是勉强够用而已,无法预料未来会怎么样
 */
static inline SGUITYPEALIGN SGuiColorAlign1(SGUIPIXEL color)
{
    return color;
}

static inline SGUITYPEALIGN SGuiColorAlign2(SGUIPIXEL color)
{
    SGUITYPEALIGN result = 0;

    result |= SGuiColorAlign1(color);
    result |= SGuiColorAlign1(color) << (sizeof(SGUIPIXEL) * 1);

    return result;
}

static inline SGUITYPEALIGN SGuiColorAlign4(SGUIPIXEL color)
{
    SGUITYPEALIGN result = 0;

    result |= SGuiColorAlign2(color);
    result |= SGuiColorAlign2(color) << (sizeof(SGUIPIXEL) * 2);

    return result;
}

static inline SGUITYPEALIGN SGuiColorAlign8(SGUIPIXEL color)
{
    SGUITYPEALIGN result = 0;

    result |= SGuiColorAlign4(color);
    result |= SGuiColorAlign4(color) << (sizeof(SGUIPIXEL) * 4);

    return result;
}


#ifndef ((SGUIALIGN != 1) && \
         (SGUIALIGN != 2) && \
         (SGUIALIGN != 4) && \
         (SGUIALIGN != 8))
#error "SGuiColorAlign(SGUIPIXEL color) : unsupported alignment way"
#endif

static inline SGUITYPEALIGN SGuiColorAlign(SGUIPIXEL color)
{
#if (SGUIALIGN == 1)
    return SGuiColorAlign1(SGUIPIXEL color);
#endif
#if (SGUIALIGN == 2)
    return SGuiColorAlign2(SGUIPIXEL color);
#endif
#if (SGUIALIGN == 4)
    return SGuiColorAlign4(SGUIPIXEL color);
#endif
#if (SGUIALIGN == 8)
    return SGuiColorAlign8(SGUIPIXEL color);
#endif
}

/* 点绘制前地址对齐检查 */
static inline bool SGuiDrawDotAlignCheck(SGuiBuffer *instance, uint32_t x, uint32_t y)
{
    void *pointer = &((instance->buffer)[SGuiOffset(instance, x, y)]);

    uint8_t temp = 0;
    temp |= (uint8_t)pointer;
    temp &= sizeof(SGUITYPEALIGN);

    bool result = (temp == 0) ? true : false;

    return result;
}

/* 地址对齐点绘制(用于加速定点拷贝): */
static inline void SGuiDrawDotAlign(SGuiBuffer *instance, uint32_t x, uint32_t y)
{
    void *pointer = &((instance->buffer)[SGuiOffset(instance, x, y)]);

    SGUITYPEALIGN *address = pointer;

    *address = SGuiColorAlign(instance->color);
}

#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#endif
