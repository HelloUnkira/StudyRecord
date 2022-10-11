#ifndef SGUI_CLIP_REGION_H
#define SGUI_CLIP_REGION_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 本意将俩部分分立文件,这里使用宏放在一起 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_CLIP_REGION1
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 俩个剪切域交集 */
SGui_Area SGui_ClipRectAnd(SGui_Area Clip1, SGui_Area Clip2);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 俩个剪切域并集 */
SGui_Area SGui_ClipRectOr(SGui_Area Clip1, SGui_Area Clip2);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 俩个剪切域联合(要求:剪切域互相平行或垂直) */
SGui_Area SGui_ClipRectUnion(SGui_Area Clip1, SGui_Area Clip2);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 俩个剪切域求差(要求:必须存在交集) */
uint32_t SGui_ClipRectSub(SGui_Area *Result, SGui_Area Master, SGui_Area Slave);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 剪切域存在检查 */
bool SGui_ClipRectIsVaild(SGui_Area Clip);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 剪切域包含关系检查1:(域包含) */
bool SGui_ClipRectCheck1(SGui_Area Child, SGui_Area Parent);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 剪切域包含关系检查2:(点包含) */
bool SGui_ClipRectCheck2(SGui_Dot Dot, SGui_Area Clip);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_CLIP_REGION2
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 获取一个剪切域集合 */
uint32_t SGui_ClipRectsTake(void);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 释放一个剪切域集合 */
void SGui_ClipRectsGive(uint32_t Clips);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 为剪切域集合设置主域 */
void SGui_ClipRectsMasterSet(uint32_t Clips, SGui_Area Clip);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 为剪切域集合获取主域 */
SGui_Area SGui_ClipRectsMasterGet(uint32_t Clips);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 剪切域集合添加剪切域 */
void SGui_ClipRectsSlaveAdd(uint32_t Clips, SGui_Area Clip);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 剪切域集合移除剪切域 */
void SGui_ClipRectsSlaveRemove(uint32_t Clips, SGui_Area Clip);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 获取剪切域集合长度 */
void SGui_ClipRectsSlaveLengthGet(uint32_t Clips, uint32_t *Length);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 获取剪切域集合 */
void SGui_ClipRectsSlaveSetGet(uint32_t Clips, SGui_Area *Slave);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_INTERNAL_TEST
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_CLIP_REGION1
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ClipRegion1_Test(void);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_CLIP_REGION2
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_ClipRegion2_Test(void);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif