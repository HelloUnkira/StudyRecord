#ifndef SGUI_COMMON_H
#define SGUI_COMMON_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#define SGUI_MIN(Data1, Data2) ((Data1) <= (Data2) ? (Data1) : (Data2))
#define SGUI_MAX(Data1, Data2) ((Data1) >= (Data2) ? (Data1) : (Data2))
#define SGUI_DIS(Data1, Data2) (SGUIMAX(Data1, Data2) - SGUIMIN(Data1, Data2))
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 通配辅助函数:拷贝函数 */
void SGui_Copy(void *Stream1, void *Stream2, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 通配辅助函数:交换函数 */
void SGui_Swap(void *Stream1, void *Stream2, uint32_t Length);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
