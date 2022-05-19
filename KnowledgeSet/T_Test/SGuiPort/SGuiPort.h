#ifndef SGUIPORT_H
#define SGUIPORT_H
/* 标准库依赖 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* 实现目标:
 * 设计基础控件的平台接口
 */

/* 跨平台支持:1启用,0禁用 */
#define SGUI_USE_LINUX      1
#define SGUI_USE_XXXX       0

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 内存分配监控策略 */
#define SGUIMEMORYCHECK             1   /* 启用内存分配监控策略   */
#define SGUIMEMORYCHECKFILELEN      20  /* file长度,超出时逆向截取 */
#define SGUIMEMORYCHECKLEN          100 /* 最多监控多少条分配信息 */
#define SGUIMEMORYCHECKAUTO         0   /* 每超过多少条分配信息启用一次显示 */
//记录的启停控制(默认不开启)
void SGuiMemoryCheckStart(void);
void SGuiMemoryCheckEnd(void);
//记录的重置
void SGuiMemoryCheckReset(void);
//内存分配情况检查
void SGuiMemoryCheck(void);
//申请内存和释放内存
#if SGUIMEMORYCHECK == 1
#define SGUIMALLOC(size)  SGuiMalloc(size,  __FILE__, __LINE__)
#define SGUIFREE(pointer) SGuiFree(pointer, __FILE__, __LINE__)
void * SGuiMalloc(uint32_t size, int8_t *file, uint32_t line);
void SGuiFree(void *pointer, int8_t *file, uint32_t line);
#endif
#if SGUIMEMORYCHECK == 0
#define SGUIMALLOC(size)  SGuiMalloc(size)
#define SGUIFREE(pointer) SGuiFree(pointer)
void * SGuiMalloc(uint32_t size);
void SGuiFree(void *pointer);
#endif
//这是专门为内存监控器准备的同步原语,其他模块不得使用,请使用下面的接口
void SGuiMemoryCheckOptSync(bool StartOrEnd);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 同步原语 */
/* 更改同步原语或修订里面的内容需要同步更新对应源 */
typedef struct SimpleGuiSynchronizationPrimitive {
    /*按union中的顺序对应: 1 ~ n */
#define SGUIMUTEX       1
#define SGUISEMAPHONE   2
    /* 原语类型,宏定义如上 */
    uint32_t type;
    /* 原语支持集合 */
    union {
        /* 第一个类型为默认保留类型 */
        void *primitive;
        /* 支持的同步原语 */
        void *mutex;
        void *semaphone;
        /* ... */
    };
    /* 同步原语静态管理资源库 */
    void *resource;
} SGuiSyncPrimit;

//创建指定类型的同步原语
SGuiSyncPrimit * SGuiSyncPrimitCreate(uint32_t type);
//销毁指定类型的同步原语
void SGuiSyncPrimitDestory(SGuiSyncPrimit *primit);
//获取同步原语保护的资源使用权限
void * SGuiSyncPrimitTakeResource(SGuiSyncPrimit *primit);
//归还同步原语保护的资源使用权限
void SGuiSyncPrimitGiveResource(SGuiSyncPrimit *primit);
//设置同步原语所需保护的资源(可选项)
void SGuiSyncPrimitSetResource(SGuiSyncPrimit *primit, void *resource);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//定时器服务
#if SGUI_USE_LINUX

typedef void (*SGuiTimerMSHandler)(void);
void SGuiTimerCfg(SGuiTimerMSHandler handler);
void SGuiTimerInit(void);

#else

#endif
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//Lcd绘制
#if SGUI_USE_LINUX
void SGuiLcdInit(void);
void SGuiLcdDeinit(void);
uint32_t * SGuiLcdInstance(void);
uint32_t SGuiLcdWidth(void);
uint32_t SGuiLcdHeight(void);
#else

#endif

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#endif
