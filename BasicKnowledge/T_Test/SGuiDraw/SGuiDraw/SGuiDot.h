#ifndef SGUIDRAWDOT_H
#define SGUIDRAWDOT_H
/* ��׼������ */
#include <stdint.h>
#include <stdbool.h>
/*ʵ��Ŀ��:
 *����ͼ�λ���֮���ص����
 */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* ��������Ԫ����(����������ڴ��ռ�����): */
#define SGUIPIXEL       uint32_t
/* ��ǰƽ̨�ֽڶ�������(��Ӧ������:sizeof(void *)): */
#define SGUITYPEALIGN   uint64_t
/* ��ַ��������:sizeof(SGUITYPEALIGN) / sizeof(SGUIPIXEL) */
/* �޸Ļ�������Ԫֵ��Ӧ�ñ�ͬ���仯,�����ַ����ʱĬ��1 */
/* Ԥ����ִ��ʱ��������sizeof,���Դ˴�ֻ�ܾ�̬����(��δ֪�������) */
#define SGUIALIGN      2
/* ��߲���:0;�߿���:1;(ͨ�������õ�����) */
#define SGUILAYOUT     0
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* ���ƿռ� */
typedef struct SimpleGuiBuffer {
    SGUIPIXEL *buffer; /* ������(���ص�) */
    SGUIPIXEL  color;  /* ���ص���ɫֵ */
    uint32_t   width;  /* ��������� */
    uint32_t   height; /* �������߶� */
} SGuiBuffer;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* ���������ֲ�֧�ּ�� */
#if ((SGUILAYOUT != 0) && \
     (SGUILAYOUT != 1))
#error "SGuiOffset(instance, x, y) : unsupported layout way"
#else
/* ��������:(x,y)ת������ƫ���� */
/* ����ɨ��ת��ʱ����������ɨ�� */
static inline uint32_t SGuiOffset(SGuiBuffer *instance, uint32_t x, uint32_t y)
{
/* ����������Ϊ:��*��ʱ(����:���ص�һ��һ�д�) */
#if (SGUILAYOUT == 0)
    return ((x) + (y) * (instance->width));
#endif
/* ����������Ϊ:��*��ʱ(����:���ص�һ��һ�д�) */
#if (SGUILAYOUT == 1)
    return ((x) + (y) * (instance->height));
#endif
}
#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* ���ص����: */
static inline void SGuiDrawDot(SGuiBuffer *instance, uint32_t x, uint32_t y)
{
    (instance->buffer)[SGuiOffset(instance, x, y)] = (instance->color);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* �о�����,�ݲ��Ƴ� */
#if 0

/* ���ص����(��ַ������������):
 * �˴����õ�����Э�ķ���,���ֲ��Դ���һ���ľ�����
 * ��ʵ��ֻ����ǿ���ö���,�޷�Ԥ��δ������ô��
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

/* �����ǰ��ַ������ */
static inline bool SGuiDrawDotAlignCheck(SGuiBuffer *instance, uint32_t x, uint32_t y)
{
    void *pointer = &((instance->buffer)[SGuiOffset(instance, x, y)]);

    uint8_t temp = 0;
    temp |= (uint8_t)pointer;
    temp &= sizeof(SGUITYPEALIGN);

    bool result = (temp == 0) ? true : false;

    return result;
}

/* ��ַ��������(���ڼ��ٶ��㿽��): */
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
