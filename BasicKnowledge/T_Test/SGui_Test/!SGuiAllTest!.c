/* 核心唯一依赖项 */
#include <stdio.h>
#include <stdlib.h>

#include "SGuiCore.h"
#include "SGuiDraw.h"

#define TEST_SGUI_PORT
#ifdef  TEST_SGUI_PORT

void sync_primit_test(void)
{
    SGuiSyncPrimit *mutex     = SGuiSyncPrimitCreate(SGUIMUTEX);
    SGuiSyncPrimit *semaphone = SGuiSyncPrimitCreate(SGUISEMAPHONE);
    
    volatile uint32_t variable1 = 12345678;
    volatile uint32_t variable2 = 87654321;
    //上述关键字导致此处需要强转(一般其他参数类型可默认转void *)
    SGuiSyncPrimitSetResource(mutex,     (void *)(&variable1));
    SGuiSyncPrimitSetResource(semaphone, (void *)(&variable2));

    uint32_t * pointer1 = SGuiSyncPrimitTakeResource(mutex);
    if (pointer1 != NULL)
        printf("\nget resource:%u from mutex", *pointer1);
    uint32_t * pointer2 = SGuiSyncPrimitTakeResource(semaphone);
    if (pointer2 != NULL)
        printf("\nget resource:%u from semaphone\n", *pointer2);

    SGuiSyncPrimitGiveResource(semaphone);
    SGuiSyncPrimitGiveResource(mutex);

    SGuiSyncPrimitDestory(mutex);
    SGuiSyncPrimitDestory(semaphone);
}

void timer_test(void)
{
    static uint64_t current_timer = 0;

    if (current_timer++ % 1000 == 0)
        printf("1s cb\n");
}

void SGuiPortTest(void)
{
    // SGuiMemoryCheckStart();
    // sync_primit_test();
    // SGuiMemoryCheckEnd();
    // SGuiMemoryCheck();

    SGuiTimerCfg(timer_test);
    SGuiTimerInit();

}

#endif

#define TEST_SGUI_CORE
#ifdef  TEST_SGUI_CORE

void handle_test(void)
{
#define HANDLE_ALL  10
    uint32_t  index = 0;
    uint32_t  array[HANDLE_ALL] = {0};
    uint32_t *pointer = NULL;

    for (index = 0; index < HANDLE_ALL; index++) {
        array[index] = SGuiHandleTake();
        SGuiHandleSet(array[index], &(array[index]));
    }

    /* 打乱 */
    for (index = 0; index < HANDLE_ALL; index++) {
        uint32_t temp_index = random() % HANDLE_ALL;
        uint32_t temp = array[index];
        array[index] = array[temp_index];
        array[temp_index] = temp;
    }

    printf("array:");
    for (index = 0; index < HANDLE_ALL; index++)
        printf("%d ", array[index]);
    printf("\n");

    /* 获取句柄资源 */
    for (index = 0; index < HANDLE_ALL; index++) {
        pointer = SGuiHandleGet(array[index]);
        printf("array[%d]:%d\n", array[index], *pointer);
    }

    /* 释放 */
    for (index = 0; index < HANDLE_ALL; index++)
        SGuiHandleGive(array[index]);

    /* 再次获取,验证 */
    for (index = 0; index < HANDLE_ALL; index++) {
        array[index] = SGuiHandleTake();
        SGuiHandleSet(array[index], &(array[index]));
    }

    for (index = 0; index < HANDLE_ALL; index++) {
        pointer = SGuiHandleGet(array[index]);
        printf("array[%d]:%d\n", array[index], *pointer);
    }

    /* 释放 */
    for (index = 0; index < HANDLE_ALL; index++)
        SGuiHandleGive(array[index]);
}

#define CLIPREGIONPRINT(clip)           \
    printf("left:%d\n",   (clip).dot1.x); \
    printf("up:%d\n",     (clip).dot1.y); \
    printf("right:%d\n",  (clip).dot2.x); \
    printf("bottom:%d\n", (clip).dot2.y); \
    printf("\n");

void clip_region_base_test(void)
{
    /* 剪切域基本运算 */
    SGUIDOT2DEFINE(clip0);
    SGUIDOT2DEFINE(clip1);
    SGUIDOT2DEFINE(clip2);
    SGUIDOT2DEFINE(clip3);

    SGUIDOT2SET(clip1, 100, 100, 300, 300);
    SGUIDOT2SET(clip2, 150, 150, 250, 250);
    SGUIDOT2SET(clip3, 150, 100, 350, 300);

    CLIPREGIONPRINT(clip1);
    CLIPREGIONPRINT(clip2);
    CLIPREGIONPRINT(clip3);

    /* 俩个剪切域交集 */
    printf("rect1 & rect2:\n");
    clip0 = SGuiClipRectAnd(clip1, clip2);
    CLIPREGIONPRINT(clip0);

    /* 俩个剪切域并集 */
    printf("rect1 | rect2:\n");
    clip0 = SGuiClipRectOr(clip1, clip2);
    CLIPREGIONPRINT(clip0);

    /* 俩个剪切域联合 */
    printf("rect1 U rect2:\n");
    clip0 = SGuiClipRectUnion(clip1, clip3);
    CLIPREGIONPRINT(clip0);

    /* 剪切域包含关系检查1:(域包含) */
    printf("rect1 have rect2:%d\n", SGuiClipRectCheck1(clip2, clip1));

    /* 俩个剪切域求差(要求:必须存在交集) */
    SGuiDot2 result[4] = {0};
    int index = 0;
    int count = SGuiClipRectSub(result, clip1, clip2);
    
    printf("rect1 - rect2:\n");
    for (int index = 0; index < count; index++) {
        CLIPREGIONPRINT(result[index]);
    }

}

void clip_region_set_test(void)
{
    SGuiDot2   *slave = NULL;
    uint32_t length = 0;
    uint32_t index  = 0;

    SGUIDOT2DEFINE(master);
    SGUIDOT2SET(master, 0, 0, 400, 400);
    SGUIDOT2DEFINE(slave1);
    SGUIDOT2SET(slave1, 150, 150, 250, 250);
    SGUIDOT2DEFINE(slave2);
    SGUIDOT2SET(slave2, 50, 50 , 100, 100);
    /* 1.获得一个剪切域集合 */
    uint32_t rects = SGuiClipRectsTake();
    /* 2.为剪切域集合设置主域 */
    SGuiClipRectsMasterSet(rects, master);
    /* 3.剪切域集合添加剪切域 */
    SGuiClipRectsSlaveAdd(rects, master);
    /* 4.剪切域集合移除剪切域 */
    SGuiClipRectsSlaveRemove(rects, slave1);
    /* 获取剪切域集合 */
    printf("current rects:\n");
    SGuiClipRectsSlaveGet(rects, &slave, &length);
    if (slave != NULL)
        for (index = 0; index < length; index++)
            if (SGuiClipRectIsVaild(slave[index]) == true) {
                printf("index:%d\n", index);
                CLIPREGIONPRINT(slave[index]);
            }
    /* 4.剪切域集合移除剪切域 */
    SGuiClipRectsSlaveRemove(rects, slave2);
    /* 获取剪切域集合 */
    printf("current rects:\n");
    SGuiClipRectsSlaveGet(rects, &slave, &length);
    if (slave != NULL)
        for (index = 0; index < length; index++)
            if (SGuiClipRectIsVaild(slave[index]) == true) {
                printf("index:%d\n", index);
                CLIPREGIONPRINT(slave[index]);
            }
    /* 3.剪切域集合添加剪切域 */
    SGuiClipRectsSlaveAdd(rects, slave1);
    /* 获取剪切域集合 */
    printf("current rects:\n");
    SGuiClipRectsSlaveGet(rects, &slave, &length);
    if (slave != NULL)
        for (index = 0; index < length; index++)
            if (SGuiClipRectIsVaild(slave[index]) == true) {
                printf("index:%d\n", index);
                CLIPREGIONPRINT(slave[index]);
            }
    /* 3.剪切域集合添加剪切域 */
    SGuiClipRectsSlaveAdd(rects, slave2);
    /* 获取剪切域集合 */
    printf("current rects:\n");
    SGuiClipRectsSlaveGet(rects, &slave, &length);
    if (slave != NULL)
        for (index = 0; index < length; index++)
            if (SGuiClipRectIsVaild(slave[index]) == true) {
                printf("index:%d\n", index);
                CLIPREGIONPRINT(slave[index]);
            }
    /* 释放一个剪切域集合 */
    SGuiClipRectsGive(rects);
}

void container_test(void)
{
#define SOURCE_ALL  10
    uint32_t      index = 0;
    uint32_t      array[SOURCE_ALL]  = {0,1,2,3,4,5,6,7,8,9};

    /* 获得一个容器 */
    uint32_t container = SGuiContainerTake();
    /* 这里的测试时,句柄直接用的数组下标代替 */
    uint32_t *source = NULL;
    uint32_t  number = 0;

    for (index = 0; index < SOURCE_ALL; index++) {
        SGuiContainerAdd(container, index);
    }

    /* 打乱 */
    for (index = 0; index < SOURCE_ALL; index++) {
        uint32_t temp_index = random() % SOURCE_ALL;
        uint32_t temp = array[index];
        array[index] = array[temp_index];
        array[temp_index] = temp;
    }

    printf("array:");
    for (index = 0; index < SOURCE_ALL; index++)
        printf("%d ", array[index]);
    printf("\n");

    /* 获取资源 */
    SGUIContainerGet(container, &source, &number);

    for (index = 0; index < SOURCE_ALL; index++)
        printf("array[%d]:%d\n", source[index], array[source[index]]);

    printf("number:%d\n", number);
    for (index = 0; index < SOURCE_ALL; index++)
        printf("source[%d]:%d\n", index, source[index]);

    for (index = 0; index < SOURCE_ALL; index++)
        SGuiContainerRemove(container, source[index]);
    SGUIFREE(source);

    /* 获取资源 */
    SGUIContainerGet(container, &source, &number);
    printf("number:%d\n", number);
    for (index = 0; index < SOURCE_ALL; index++)
        printf("source[%d]:%d\n", index, source[index]);
    SGUIFREE(source);
    /* 释放 */
    SGuiContainerGive(container);
}

void SGuiCoreTest(void)
{
    // SGuiMemoryCheckStart();
    // handle_test();
    // SGuiMemoryCheckEnd();
    // SGuiMemoryCheck();
    // SGuiMemoryCheckReset();

    // clip_region_base_test();

    // SGuiMemoryCheckStart();
    // clip_region_set_test();
    // clip_region_set_test();
    // SGuiMemoryCheckEnd();
    // SGuiMemoryCheck();

    // SGuiMemoryCheckStart();
    // container_test();
    // SGuiMemoryCheckEnd();
    // SGuiMemoryCheck();
}

#endif

#define TEST_SGUI_DRAW
#ifdef  TEST_SGUI_DRAW

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
    SGuiDrawLine(line);
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 100;
    line.y_end    = 200;
    SGuiDrawLine(line);
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 200;
    line.y_end    = 200;
    SGuiDrawLine(line);
    line.x_start  = 200;
    line.y_start  = 100;
    line.x_end    = 200;
    line.y_end    = 200;
    SGuiDrawLine(line);
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 200;
    line.y_end    = 200;
    SGuiDrawLine(line);
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 200;
    line.y_end    = 100;
    SGuiDrawLine(line);
    /* 绘制: */
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 200;
    line.y_end    = 150;
    SGuiDrawLine(line);
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 200;
    line.y_end    = 125;
    SGuiDrawLine(line);
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 200;
    line.y_end    = 112;
    SGuiDrawLine(line);
    //
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 150;
    line.y_end    = 200;
    SGuiDrawLine(line);
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 125;
    line.y_end    = 200;
    SGuiDrawLine(line);
    line.x_start  = 100;
    line.y_start  = 100;
    line.x_end    = 112;
    line.y_end    = 200;
    SGuiDrawLine(line);
    //
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 200;
    line.y_end    = 150;
    SGuiDrawLine(line);
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 200;
    line.y_end    = 175;
    SGuiDrawLine(line);
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 200;
    line.y_end    = 183;
    SGuiDrawLine(line);
    //
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 150;
    line.y_end    = 100;
    SGuiDrawLine(line);
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 125;
    line.y_end    = 100;
    SGuiDrawLine(line);
    line.x_start  = 100;
    line.y_start  = 200;
    line.x_end    = 112;
    line.y_end    = 100;
    SGuiDrawLine(line);
}

void SGuiDrawCricleTest(SGuiBuffer instance)
{
    SGuiCircle circle = {0};
    circle.instance = instance;
    circle.instance.color = 0xFFFFFFFF;

    circle.x_center   = 300;
    circle.y_center   = 100;
    circle.radius     = 50;
    circle.type       = 1;
    SGuiDrawCircle(circle);
    circle.x_center   = 300;
    circle.y_center   = 250;
    circle.radius     = 50;
    circle.band       = 5;
    circle.type       = 2;
    SGuiDrawCircle(circle);
}

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
    SGuiDrawEllipse(ellipse);
}

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
    SGuiDrawRectangle(rectangle);
}

void SGuiDrawTest(SGuiBuffer instance)
{
    SGuiDrawLineTest(instance);
    SGuiDrawCricleTest(instance);
    SGuiDrawEllipseTest(instance);
    SGuiDrawRectangleTest(instance);
}

#endif

int main(int argc, char *argv[])
{
    SGuiPortTest();

    // SGuiCoreTest();

    SGuiLcdInit();

    SGuiBuffer instance = {0};

    instance.buffer = SGuiLcdInstance();
    instance.width  = SGuiLcdWidth();
    instance.height = SGuiLcdHeight();

    SGuiDrawTest(instance);

    while (true);

    // SGuiLcdDeinit();

    return 0;
}
