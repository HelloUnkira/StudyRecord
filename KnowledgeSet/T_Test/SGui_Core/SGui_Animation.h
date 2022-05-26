#ifndef SGUI_ANIMATHON_H
#define SGUI_ANIMATHON_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_AnimationMSHandler(void);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 创建一个动画 */
void SGui_AnimationCreate(uint32_t Handle, uint8_t Label);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 销毁一个动画 */
void SGui_AnimationDestroy(uint32_t Handle, uint8_t Label);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 启动一个动画 */
void SGui_AnimationStart(uint32_t Handle, uint8_t Label, uint32_t Period, bool Repeat);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 停止一个动画 */
void SGui_AnimationStop(uint32_t Handle, uint8_t Label);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 中止一个动画 */
void SGui_AnimationAbort(uint32_t Handle, uint8_t Label);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 恢复一个动画 */
void SGui_AnimationResume(uint32_t Handle, uint8_t Label);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif