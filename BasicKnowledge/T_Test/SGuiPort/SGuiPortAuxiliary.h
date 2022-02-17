#ifndef SGUIPORTAUXILIARY_H
#define SGUIPORTAUXILIARY_H

/* ͬ��ԭ��������ɻ� */
#define SGUISYNCPRIMITSTSPECIAL
#ifdef  SGUISYNCPRIMITSTSPECIAL

/* ����ͬ��ԭ��(true:���ñ���,false:���ñ���) */
static void SGuiSyncPrimitOptSync(bool StartOrEnd)
{
    /* ʹ��һ����̬��Դ */
    static uint8_t local_variable   = 0xA5;
    static SGuiSyncPrimit *local_mutex = NULL;
    /* ͬ��ԭ��ĳ�ʼ��,����,��ֹ,��ͳһ�ӵ��˴� */
    static bool SGuiSyncPrimitOptSyncInit   = false;
    static bool SGuiSyncPrimitOptSyncDeInit = false;
    /* ͬ��ԭ��ĳ�ʼ�� */
    if (SGuiSyncPrimitOptSyncInit == false) {
        SGuiSyncPrimitOptSyncInit  = true;
        /* ��ʼ�� */
        local_mutex = SGuiSyncPrimitCreate(SGUIMUTEX);
        /* ����һ������Դ��ֹ����NULL */
        SGuiSyncPrimitSetResource(local_mutex, (&local_variable));
    }
    /* ͬ��ԭ��ķ���ʼ��(ʲôʱ�������ʽ����ʹ�þ����Դ����?) */
    if (SGuiSyncPrimitOptSyncDeInit == true) {
        SGuiSyncPrimitDestory(local_mutex);
    }
    /* ��ȡ����Դ */
    if (StartOrEnd == true) {
        uint8_t * pointer = SGuiSyncPrimitTakeResource(local_mutex);
        /* ���Լ�Ҫ�������ȥȷ��ͬ��ԭ���Ƿ���Ч */
    }
    /* �ͷż���Դ */
    if (StartOrEnd == false) {
        SGuiSyncPrimitGiveResource(local_mutex);
    }
}

#endif

#endif
