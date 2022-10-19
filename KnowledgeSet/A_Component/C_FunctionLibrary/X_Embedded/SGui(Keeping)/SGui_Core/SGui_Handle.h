#ifndef SGUI_HANDLE_H
#define SGUI_HANDLE_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 句柄描述 */
#define SimpleGui_Handle uint32_t
typedef SimpleGui_Handle SGui_Handle;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 错误的句柄(或者空句柄) */
#define SGUI_HANDLE_INVALID (~((SGui_Handle)0))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 获取一个空闲句柄 */
SGui_Handle SGui_HandleTake(void);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 释放一个句柄 */
void SGui_HandleGive(SGui_Handle Handle);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 句柄设置资源(设置,更新,清除) */
void SGui_HandleSourceSet(SGui_Handle Handle, void *Source);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 句柄获取资源 */
void * SGui_HandleSourceGet(SGui_Handle Handle);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 有一些句柄资源是编译时生成,所以为了静态句柄添加以下扩展 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 静态的句柄范围偏移量 */
#define SGUI_HANDLE_STATIC  (((SGui_Handle)0x80000000))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 静态句柄表绑定 */
void SGui_HandleStaticTableRegister(void **Resource, uint32_t Length);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_INTERNAL_TEST
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_Handle_Test(uint32_t (*Random)());
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
