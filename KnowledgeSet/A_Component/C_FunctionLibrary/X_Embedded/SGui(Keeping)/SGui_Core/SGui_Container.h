#ifndef SGUI_CONTAINER_H
#define SGUI_CONTAINER_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 容器描述 */
typedef struct SimpleGui_Container {
    uint32_t     Length;
    SGui_Handle *Handles;
} SGui_Container;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 获得一个容器 */
static inline void SGui_ContainerCreate(SGui_Container *Container, uint32_t Length)
{
    Container->Length  = Length;
    Container->Handles = SGUI_ALLOC(sizeof(SGui_Handle) * Length);
    
    for (uint32_t Index = 0; Index < Container->Length; Index++)
         Container->Handles[Index] = SGUI_HANDLE_INVALID;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 释放一个容器 */
static inline void SGui_ContainerDestroy(SGui_Container *Container)
{
    SGUI_FREE(Container->Handles);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 容器添加一个子控件 */
static inline bool SGui_ContainerAdd(SGui_Container *Container, SGui_Handle Handle)
{
    for (uint32_t Index = 0; Index < Container->Length; Index++)
        if (Container->Handles[Index] == SGUI_HANDLE_INVALID) {
            Container->Handles[Index]  = Handle;
            return true;
        }
    return false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 容器移除一个子控件 */
static inline bool SGui_ContainerRemove(SGui_Container *Container, SGui_Handle Handle)
{
    for (uint32_t Index = 0; Index < Container->Length; Index++)
        if (Container->Handles[Index] == Handle) {
            Container->Handles[Index]  = SGUI_HANDLE_INVALID;
            return true;
        }
    return false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 容器作为一个静态属性使用,此处只提供几个基本的简化动作 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif