#ifndef SGUI_MEMORY_ADAPTOR_H
#define SGUI_MEMORY_ADAPTOR_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if 0
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#elif SGUI_USE_LINUX
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void * SGui_PortAlloc(uint32_t Size)
{
    return malloc(Size);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_PortFree(void *Pointer)
{
    free(Pointer);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//继续添加其他平台
//#elif SGUI_USE_UNKONWN
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#else
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void * SGui_PortAlloc(uint32_t Size)
{
    return NULL;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_PortFree(void *Pointer)
{
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 上面是需要进行平台适配的部分 */
/* 下面是内部使用的通配的转接层 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#if SGUI_USE_MEMORY_CHECK
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void * SGui_Alloc(uint32_t Size, int8_t *File, uint32_t Line)
{
    void * Pointer = SGui_PortAlloc(Size);
    SGUI_LOGMESSAGE("SGui_Alloc: \n[%s][%u][%p][%u]\n", File, Line, Pointer, Size);
    return Pointer;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_Free(void *Pointer, int8_t *File, uint32_t Line)
{
    SGUI_LOGMESSAGE("SGui_Free: \n[%s][%u][%p]\n", File, Line, Pointer);
    SGui_PortFree(Pointer);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#else
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void * SGui_Alloc(uint32_t Size, int8_t *File, uint32_t Line)
{
    void * Pointer = SGui_PortAlloc(Size);
    return Pointer;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void SGui_Free(void *Pointer, int8_t *File, uint32_t Line)
{
    SGui_PortFree(Pointer);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define SGUI_ALLOC(Size)   SGui_Alloc(Size,   __FILE__, __LINE__)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define SGUI_FREE(Pointer) SGui_Free(Pointer, __FILE__, __LINE__)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
