#ifndef SGUICORE_H
#define SGUICORE_H
/* 标准库依赖 */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
/* 平台依赖 */
#include "SGuiPort.h"

/* 实现目标:
 * 设计基础控件的接口及其绘制挂载
 */

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 内部定制优化策略 */
#include "SGuiCommon.h"
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 句柄资源运算 */

/* 错误的句柄 */
#define SGUIINVALIDHANDLE ((uint32_t)(~0))
/* 获取一个空闲句柄 */
uint32_t SGuiHandleTake(void);
/* 释放一个句柄 */
void SGuiHandleGive(uint32_t handle);
/* 句柄绑定或更新资源 */
void SGuiHandleSet(uint32_t handle, void *source);
/* 句柄获取资源 */
void * SGuiHandleGet(uint32_t handle);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

/* 基本图元描述:点*1 */
typedef struct SimpleGuiDot1 {
    int32_t x;   //横坐标
    int32_t y;   //纵坐标
} SGuiDot1;

/* 基本图元描述:点*2 */
typedef struct SimpleGuiDot2 {
    SGuiDot1 dot1;  //左上角坐标
    SGuiDot1 dot2;  //右下角坐标
} SGuiDot2;

/* 点操作 */
/* 非法坐标点值(这里随意指定一个最大最小值) */
#define SGUIINVALIDX ((int32_t)(0x7FFFFFFF))
#define SGUIINVALIDY ((int32_t)(0x80000000))

/* 变量重置时可能需要用到 */
#define SGUIDOT1RESET(variable)  \
do {                             \
    (variable).x = SGUIINVALIDX; \
    (variable).y = SGUIINVALIDY; \
} while (0)

#define SGUIDOT2RESET(variable)       \
do {                                  \
    (variable).dot1.x = SGUIINVALIDX; \
    (variable).dot1.y = SGUIINVALIDY; \
    (variable).dot2.x = SGUIINVALIDX; \
    (variable).dot2.y = SGUIINVALIDY; \
} while (0)

#define SGUIDOT2SET(variable,x1,y1,x2,y2) \
do {                                      \
    (variable).dot1.x = (x1);             \
    (variable).dot1.y = (y1);             \
    (variable).dot2.x = (x2);             \
    (variable).dot2.y = (y2);             \
} while (0)

/* 变量定义时可能需要用到 */
#define SGUIDOT1DEFINE(label) \
    SGuiDot1 (label) = {0};   \
    SGUIDOT1RESET(label);

#define SGUIDOT2DEFINE(label) \
    SGuiDot2 (label) = {0};   \
    SGUIDOT2RESET(label);

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 剪切域基本运算: */

/* 俩个剪切域交集 */
SGuiDot2 SGuiClipRectAnd(SGuiDot2 rect1, SGuiDot2 rect2);
/* 俩个剪切域并集 */
SGuiDot2 SGuiClipRectOr(SGuiDot2 rect1, SGuiDot2 rect2);
/* 俩个剪切域联合 */
SGuiDot2 SGuiClipRectUnion(SGuiDot2 rect1, SGuiDot2 rect2);
/* 俩个剪切域求差(要求:必须存在交集) */
uint32_t SGuiClipRectSub(SGuiDot2 *result, SGuiDot2 master, SGuiDot2 slave);
/* 剪切域存在检查 */
bool SGuiClipRectIsVaild(SGuiDot2 rect);
/* 剪切域包含关系检查1:(域包含) */
bool SGuiClipRectCheck1(SGuiDot2 child, SGuiDot2 parent);
/* 剪切域包含关系检查2:(点包含) */
bool SGuiClipRectCheck2(SGuiDot1 dot, SGuiDot2 rect);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 剪切域集合运算(容器使用): */

/* 获取一个剪切域集合 */
uint32_t SGuiClipRectsTake(void);
/* 释放一个剪切域集合 */
void SGuiClipRectsGive(uint32_t rects);
/* 为剪切域集合设置主域 */
void SGuiClipRectsMasterSet(uint32_t rects, SGuiDot2 rect);
/* 为剪切域集合获取主域 */
SGuiDot2 SGuiClipRectsMasterGet(uint32_t rects);
/* 剪切域集合添加剪切域 */
void SGuiClipRectsSlaveAdd(uint32_t rects, SGuiDot2 rect);
/* 剪切域集合移除剪切域 */
void SGuiClipRectsSlaveRemove(uint32_t rects, SGuiDot2 rect);
/* 获取剪切域集合(动态生成的副本,使用完毕要SGUIFREE掉) */
void SGuiClipRectsSlaveGet(uint32_t rects, SGuiDot2 **slave, uint32_t *length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 控件容器属性: */

/* 获得一个容器 */
uint32_t SGuiContainerTake(void);
/* 释放一个容器 */
void SGuiContainerGive(uint32_t container);
/* 容器添加一个子控件 */
void SGuiContainerAdd(uint32_t container, uint32_t handle);
/* 容器移除一个子控件 */
void SGuiContainerRemove(uint32_t container, uint32_t handle);
/* 获取容器集合(动态生成的副本,使用完毕要SGUIFREE掉) */
void SGUIContainerGet(uint32_t container, uint32_t **source, uint32_t *number);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 消息集合: */

/* 第0号消息为:默认假消息 */
#define SGUIMSGFAKERTYPE    0
/* 消息所用的参数集合,该集合可以指定类型和大小 */
#define SGUIMSGPARAMTYPE    uint32_t
#define SGUIMSGPARAMLEN     4

//消息
typedef struct SimpleGuiMessage {
    //消息类型
    uint32_t type;
    //消息携带的参数集合
    SGUIMSGPARAMTYPE parameter[SGUIMSGPARAMLEN];
} SGuiMessage;

/* 生成一个消息集合 */
uint32_t SGuiMessageSetTake(uint32_t number);
/* 释放一个消息集合 */
uint32_t SGuiMessageSetGive(uint32_t set);
/* 消息集合添加一个消息(先向) */
uint32_t SGuiMessageSetAdd(uint32_t set, SGuiMessage message);
/* 消息集合移除一个消息(先向, 先定向) */
uint32_t SGuiMessageSetRemove(uint32_t set, SGuiMessage *message);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

#endif
