/* ʵ��Ŀ��:
 * ��Ƽ�Ҫ�ļ���������
 * ������ͳһ��Ը����ڵ�ƫ��������
 */

/* ����Ψһ������ */
#include "SGuiCore.h"

/* ���������򽻼� */
SGuiDot2 SGuiClipRectAnd(SGuiDot2 rect1, SGuiDot2 rect2)
{
    SGUIDOT2DEFINE(rect);
    
    rect.dot1.x = SGUIMAX(rect1.dot1.x, rect2.dot1.x);
    rect.dot1.y = SGUIMAX(rect1.dot1.y, rect2.dot1.y);
    rect.dot2.x = SGUIMIN(rect1.dot2.x, rect2.dot2.x);
    rect.dot2.y = SGUIMIN(rect1.dot2.y, rect2.dot2.y);

    return rect;
}

/* ���������򲢼� */
SGuiDot2 SGuiClipRectOr(SGuiDot2 rect1, SGuiDot2 rect2)
{
    SGUIDOT2DEFINE(rect);

    rect.dot1.x = SGUIMIN(rect1.dot1.x, rect2.dot1.x);
    rect.dot1.y = SGUIMIN(rect1.dot1.y, rect2.dot1.y);
    rect.dot2.x = SGUIMAX(rect1.dot2.x, rect2.dot2.x);
    rect.dot2.y = SGUIMAX(rect1.dot2.y, rect2.dot2.y);

    return rect;
}

/* �������������� */
SGuiDot2 SGuiClipRectUnion(SGuiDot2 rect1, SGuiDot2 rect2)
{
    SGUIDOT2DEFINE(rect);

    /* ���������Ҫ��:��������ƽ�л�ֱ */

    /* ������ֱ: */
    if (rect1.dot1.x == rect2.dot1.x && rect1.dot2.x == rect2.dot2.x) {
        /* ����1:���ཻ */
        if (rect1.dot2.y < rect2.dot1.y || rect2.dot2.y < rect1.dot1.y)
            return rect;
        /* ����2:�ཻ */
        rect.dot1.x = (rect1.dot1.x, rect2.dot1.x);
        rect.dot2.x = (rect1.dot2.x, rect2.dot2.x);
        rect.dot1.y = SGUIMIN(rect1.dot1.y, rect2.dot1.y);
        rect.dot2.y = SGUIMAX(rect1.dot2.y, rect2.dot2.y);
        return rect;
    }
    /* ������ƽ��: */
    if (rect1.dot1.y == rect2.dot1.y && rect1.dot2.y == rect2.dot2.y) {
        /* ����1:���ཻ */
        if (rect1.dot2.x < rect2.dot1.x || rect2.dot2.x < rect1.dot1.x)
            return rect;
        /* ����2:�ཻ */
        rect.dot1.y = (rect1.dot1.y, rect2.dot1.y);
        rect.dot2.y = (rect1.dot2.y, rect2.dot2.y);
        rect.dot1.x = SGUIMIN(rect1.dot1.x, rect2.dot1.x);
        rect.dot2.x = SGUIMAX(rect1.dot2.x, rect2.dot2.x);
        return rect;
    }
    /*  */
    return rect;
}

/* ������������� */
/* Ҫ��:������ڽ��� */
uint32_t SGuiClipRectSub(SGuiDot2 *result, SGuiDot2 master, SGuiDot2 slave)
{
    /* �������4�������� */
    /* �ü���������(�������) */
    uint32_t count = 0;

    /* �ü���������ϲ� */
    if (master.dot1.y < slave.dot1.y) {
        result[count].dot1.x = master.dot1.x;
        result[count].dot2.x = master.dot2.x;
        result[count].dot1.y = master.dot1.y;
        result[count].dot2.y = slave.dot1.y;
        
        master.dot1.y = slave.dot1.y;
        count++;
    }
    /* �ü���������²� */
    if (master.dot2.y > slave.dot2.y) {
        result[count].dot1.x = master.dot1.x;
        result[count].dot2.x = master.dot2.x;
        result[count].dot1.y = slave.dot2.y;
        result[count].dot2.y = master.dot2.y;

        master.dot2.y = slave.dot2.y;
        count++;
    }
    /* �ü���������� */
    if (master.dot1.x < slave.dot1.x) {
        result[count].dot1.y = master.dot1.y;
        result[count].dot2.y = master.dot2.y;
        result[count].dot1.x = master.dot1.x;
        result[count].dot2.x = slave.dot1.x;
        
        //master.dot1.x = slave.dot1.x;
        count++;
    }
    /* �ü���������Ҳ� */
    if (master.dot2.x > slave.dot2.x) {
        result[count].dot1.y = master.dot1.y;
        result[count].dot2.y = master.dot2.y;
        result[count].dot1.x = slave.dot2.x;
        result[count].dot2.x = master.dot2.x;

        //master.dot2.x = slave.dot2.x;
        count++;
    }

    return count;
}

/* ��������ڼ�� */
bool SGuiClipRectIsVaild(SGuiDot2 rect)
{
    bool result = true;
    /* �����벹������ */
    if (rect.dot1.x == SGUIINVALIDX && rect.dot1.y == SGUIINVALIDY &&
        rect.dot2.x == SGUIINVALIDX && rect.dot2.y == SGUIINVALIDY)
        result = false;
    
    if (rect.dot1.x >= rect.dot2.x  || rect.dot1.y >= rect.dot2.y)
        result = false;

    return result;
}

/* �����������ϵ���1:(�����) */
bool SGuiClipRectCheck1(SGuiDot2 child, SGuiDot2 parent)
{
    bool result = false;
    /* �����벹������ */
    if (child.dot1.x >= parent.dot1.x && child.dot2.x <= parent.dot2.x &&
        child.dot1.y >= parent.dot1.y && child.dot2.y <= parent.dot2.y)
        result = true;

    return result;
}

/* �����������ϵ���2:(�����) */
bool SGuiClipRectCheck2(SGuiDot1 dot, SGuiDot2 rect)
{
    bool result = false;
    /* �����벹������ */
    if (dot.x >= rect.dot1.x && dot.x <= rect.dot2.x &&
        dot.y >= rect.dot1.y && dot.y <= rect.dot2.y)
        result = true;

    return result;
}
