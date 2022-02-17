#ifndef SGUIPORTAUXILIARY_H
#define SGUIPORTAUXILIARY_H

/* 同步原语操作集成化 */
#define SGUISYNCPRIMITSTSPECIAL
#ifdef  SGUISYNCPRIMITSTSPECIAL

/* 操作同步原语(true:启用保护,false:禁用保护) */
static void SGuiSyncPrimitOptSync(bool StartOrEnd)
{
    /* 使用一个静态资源 */
    static uint8_t local_variable   = 0xA5;
    static SGuiSyncPrimit *local_mutex = NULL;
    /* 同步原语的初始化,启用,终止,都统一加到此处 */
    static bool SGuiSyncPrimitOptSyncInit   = false;
    static bool SGuiSyncPrimitOptSyncDeInit = false;
    /* 同步原语的初始化 */
    if (SGuiSyncPrimitOptSyncInit == false) {
        SGuiSyncPrimitOptSyncInit  = true;
        /* 初始化 */
        local_mutex = SGuiSyncPrimitCreate(SGUIMUTEX);
        /* 设置一个假资源防止返回NULL */
        SGuiSyncPrimitSetResource(local_mutex, (&local_variable));
    }
    /* 同步原语的反初始化(什么时候才算正式放弃使用句柄资源了呢?) */
    if (SGuiSyncPrimitOptSyncDeInit == true) {
        SGuiSyncPrimitDestory(local_mutex);
    }
    /* 获取假资源 */
    if (StartOrEnd == true) {
        uint8_t * pointer = SGuiSyncPrimitTakeResource(local_mutex);
        /* 可以简要添加内容去确认同步原语是否有效 */
    }
    /* 释放假资源 */
    if (StartOrEnd == false) {
        SGuiSyncPrimitGiveResource(local_mutex);
    }
}

#endif

#endif
