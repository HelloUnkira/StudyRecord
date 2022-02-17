#ifndef SGUIDRAW_H
#define SGUIDRAW_H
/* ��׼������ */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
/* ƽ̨���� */
#include "SGuiCommon.h"
#include "SGuiPort.h"

/* 
 * ���Ŀ��:
 * �Ի��������л������ƶ���(�������ݴ����)
 * �ϲ�����������·�
 */

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

/* ���ƿռ� */
typedef struct SimpleGuiBuffer {
    SGUIPIXEL *buffer; /* ������(���ص�) */
    SGUIPIXEL  color;  /* ���ص���ɫֵ */
    uint32_t   width;  /* ��������� */
    uint32_t   height; /* �������߶� */
} SGuiBuffer;

/*****************************************************************************/

/* ��������:(x,y)ת������ƫ���� */
/* ����ɨ��ת��ʱ����������ɨ�� */

/* ����������Ϊ:��*��ʱ(����:���ص�һ��һ�д�) */
#define SGUIOFF1(instance, x, y) ((x) + (y) * (instance.width))
/* ����������Ϊ:��*��ʱ(����:���ص�һ��һ�д�) */
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

/* ��ַδ��������: */
#define SGUIDRAWDOT(instance, value, x, y) \
    (instance.buffer)[SGUIOFF(instance, x, y)] = (value)

/* ��ַ��������: */
#if (SGUIALIGN != 1)
/*
 * �˴����õ�����Э�ķ���,���ֲ��Դ���һ���ľ�����
 * ��ʵ��ֻ����ǿ���ö���,�޷�Ԥ��δ������ô��
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

/* �߼����� */
#define SGUIDIS(data1, data2) (SGUIMAX(data1, data2) - SGUIMIN(data1, data2))

/*****************************************************************************/

/* �������� */
typedef struct SimpleGuiSequence {
    int32_t *x_point;
    int32_t *y_point;
    uint32_t  length;
} SGuiSequence;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/* ������ */
typedef struct SimpleGuiLine {
    SGuiBuffer instance;
    int32_t x_start;
    int32_t y_start;
    int32_t x_end;
    int32_t y_end;
} SGuiLine;

void SGuiDrawLine(SGuiLine line);
void SGuiDrawVerticalLine(SGuiLine line);   /* ����:��ֱ��(�ڲ�ʹ��,�޼��) */
void SGuiDrawHorizontalLine(SGuiLine line); /* ����:ˮƽ��(�ڲ�ʹ��,�޼��) */

/*****************************************************************************/

/* ���ƾ��� */
/* ��ע:���ľ��β�֧��,��Ϊ�������ĸ��� */
typedef struct SimpleGuiRectangle {
    SGuiBuffer instance;
    int32_t x_left;
    int32_t y_left;
    int32_t width;
    int32_t height;
    int32_t type;    /* ��������(0:������;) */
} SGuiRectangle;

void SGuiDrawRectangle(SGuiRectangle rectangle);

/* ����Բ */
typedef struct SimpleGuiCircle {
    SGuiBuffer instance;
    int32_t x_center;
    int32_t y_center;
    int32_t radius;
    int32_t band;      /* �������(��չΪ����) */
    int32_t type;      /* Բ����(0:����;1:���;2:����; */
    int32_t x_current; /* �ڲ�ʹ��,����Ϊ0 */
    int32_t y_current; /* �ڲ�ʹ��,����Ϊ0 */
} SGuiCircle;

void SGuiDrawCircle(SGuiCircle circle);

/* ������Բ */
typedef struct SimpleGuiEllipse {
    SGuiBuffer instance;
    int32_t x_center;
    int32_t y_center;
    int32_t x_radius;
    int32_t y_radius;
    int32_t type;        /* ��Բ����(0:����;1:���;) */
    int32_t x_current;   /* �ڲ�ʹ��,����Ϊ0 */
    int32_t y_current;   /* �ڲ�ʹ��,����Ϊ0 */
} SGuiEllipse;

void SGuiDrawEllipse(SGuiEllipse ellipse);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#endif
