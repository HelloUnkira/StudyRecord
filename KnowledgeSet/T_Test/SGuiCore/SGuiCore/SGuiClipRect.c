/* 实现目标:
 * 设计简要的剪切域运算
 * 剪切域统一相对父窗口的偏移做计算
 */

/* 核心唯一依赖项 */
#include "SGuiCore.h"

/* 俩个剪切域交集 */
SGuiDot2 SGuiClipRectAnd(SGuiDot2 rect1, SGuiDot2 rect2)
{
    SGUIDOT2DEFINE(rect);
    
    rect.dot1.x = SGUIMAX(rect1.dot1.x, rect2.dot1.x);
    rect.dot1.y = SGUIMAX(rect1.dot1.y, rect2.dot1.y);
    rect.dot2.x = SGUIMIN(rect1.dot2.x, rect2.dot2.x);
    rect.dot2.y = SGUIMIN(rect1.dot2.y, rect2.dot2.y);

    return rect;
}

/* 俩个剪切域并集 */
SGuiDot2 SGuiClipRectOr(SGuiDot2 rect1, SGuiDot2 rect2)
{
    SGUIDOT2DEFINE(rect);

    rect.dot1.x = SGUIMIN(rect1.dot1.x, rect2.dot1.x);
    rect.dot1.y = SGUIMIN(rect1.dot1.y, rect2.dot1.y);
    rect.dot2.x = SGUIMAX(rect1.dot2.x, rect2.dot2.x);
    rect.dot2.y = SGUIMAX(rect1.dot2.y, rect2.dot2.y);

    return rect;
}

/* 俩个剪切域联合 */
SGuiDot2 SGuiClipRectUnion(SGuiDot2 rect1, SGuiDot2 rect2)
{
    SGUIDOT2DEFINE(rect);

    /* 剪切域求和要求:剪切域互相平行或垂直 */

    /* 剪切域垂直: */
    if (rect1.dot1.x == rect2.dot1.x && rect1.dot2.x == rect2.dot2.x) {
        /* 场景1:非相交 */
        if (rect1.dot2.y < rect2.dot1.y || rect2.dot2.y < rect1.dot1.y)
            return rect;
        /* 场景2:相交 */
        rect.dot1.x = (rect1.dot1.x, rect2.dot1.x);
        rect.dot2.x = (rect1.dot2.x, rect2.dot2.x);
        rect.dot1.y = SGUIMIN(rect1.dot1.y, rect2.dot1.y);
        rect.dot2.y = SGUIMAX(rect1.dot2.y, rect2.dot2.y);
        return rect;
    }
    /* 剪切域平行: */
    if (rect1.dot1.y == rect2.dot1.y && rect1.dot2.y == rect2.dot2.y) {
        /* 场景1:非相交 */
        if (rect1.dot2.x < rect2.dot1.x || rect2.dot2.x < rect1.dot1.x)
            return rect;
        /* 场景2:相交 */
        rect.dot1.y = (rect1.dot1.y, rect2.dot1.y);
        rect.dot2.y = (rect1.dot2.y, rect2.dot2.y);
        rect.dot1.x = SGUIMIN(rect1.dot1.x, rect2.dot1.x);
        rect.dot2.x = SGUIMAX(rect1.dot2.x, rect2.dot2.x);
        return rect;
    }
    /*  */
    return rect;
}

/* 俩个剪切域求差 */
/* 要求:必须存在交集 */
uint32_t SGuiClipRectSub(SGuiDot2 *result, SGuiDot2 master, SGuiDot2 slave)
{
    /* 最高生成4个剪切域 */
    /* 裁剪主剪切域(四种情况) */
    uint32_t count = 0;

    /* 裁剪主域多余上部 */
    if (master.dot1.y < slave.dot1.y) {
        result[count].dot1.x = master.dot1.x;
        result[count].dot2.x = master.dot2.x;
        result[count].dot1.y = master.dot1.y;
        result[count].dot2.y = slave.dot1.y;
        
        master.dot1.y = slave.dot1.y;
        count++;
    }
    /* 裁剪主域多余下部 */
    if (master.dot2.y > slave.dot2.y) {
        result[count].dot1.x = master.dot1.x;
        result[count].dot2.x = master.dot2.x;
        result[count].dot1.y = slave.dot2.y;
        result[count].dot2.y = master.dot2.y;

        master.dot2.y = slave.dot2.y;
        count++;
    }
    /* 裁剪主域多余左部 */
    if (master.dot1.x < slave.dot1.x) {
        result[count].dot1.y = master.dot1.y;
        result[count].dot2.y = master.dot2.y;
        result[count].dot1.x = master.dot1.x;
        result[count].dot2.x = slave.dot1.x;
        
        //master.dot1.x = slave.dot1.x;
        count++;
    }
    /* 裁剪主域多余右部 */
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

/* 剪切域存在检查 */
bool SGuiClipRectIsVaild(SGuiDot2 rect)
{
    bool result = true;
    /* 本集与补集运算 */
    if (rect.dot1.x == SGUIINVALIDX && rect.dot1.y == SGUIINVALIDY &&
        rect.dot2.x == SGUIINVALIDX && rect.dot2.y == SGUIINVALIDY)
        result = false;
    
    if (rect.dot1.x >= rect.dot2.x  || rect.dot1.y >= rect.dot2.y)
        result = false;

    return result;
}

/* 剪切域包含关系检查1:(域包含) */
bool SGuiClipRectCheck1(SGuiDot2 child, SGuiDot2 parent)
{
    bool result = false;
    /* 本集与补集运算 */
    if (child.dot1.x >= parent.dot1.x && child.dot2.x <= parent.dot2.x &&
        child.dot1.y >= parent.dot1.y && child.dot2.y <= parent.dot2.y)
        result = true;

    return result;
}

/* 剪切域包含关系检查2:(点包含) */
bool SGuiClipRectCheck2(SGuiDot1 dot, SGuiDot2 rect)
{
    bool result = false;
    /* 本集与补集运算 */
    if (dot.x >= rect.dot1.x && dot.x <= rect.dot2.x &&
        dot.y >= rect.dot1.y && dot.y <= rect.dot2.y)
        result = true;

    return result;
}
