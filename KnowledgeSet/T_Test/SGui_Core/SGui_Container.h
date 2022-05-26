#ifndef SGUI_CONTAINER_H
#define SGUI_CONTAINER_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 获得一个容器 */
uint32_t SGui_ContainerTake(void);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 释放一个容器 */
void SGui_ContainerGive(uint32_t Container);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 容器添加一个子控件 */
void SGui_ContainerAdd(uint32_t Container, uint32_t Handle);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 容器移除一个子控件 */
void SGui_ContainerRemove(uint32_t Container, uint32_t Handle);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 获取容器集合长度 */
void SGui_ContainerLengthGet(uint32_t Container, uint32_t *Number);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 获取容器集合 */
void SGui_ContainerSetGet(uint32_t Container, uint32_t *Source);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif