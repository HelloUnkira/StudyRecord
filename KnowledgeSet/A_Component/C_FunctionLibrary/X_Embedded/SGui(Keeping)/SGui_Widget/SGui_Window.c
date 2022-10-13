#include "SGui_Kernel.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
SGui_Handle SGui_WidgetGetWindow(SGui_Handle Widget_H)
{
    SGui_Widget *Widget = NULL;
    /* 当前控件无效 */
    if ((Widget = SGui_HandleSourceGet(Widget_H)) == NULL)
        return SGUI_HANDLE_INVALID;
    
    SGui_Handle Parent_H = Widget->Parent;
    /* 迭代直到找不到寻找父控件,自己控件为祖先控件 */
    while (SGui_HandleSourceGet(Parent_H) != NULL) {
        Widget_H = (Widget = SGui_HandleSourceGet(Widget_H))->Parent;
        Parent_H = (Widget = SGui_HandleSourceGet(Parent_H))->Parent;
    }
    
    return Widget_H;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
SGui_Handle SGui_WindowCreate(uint32_t Number)
{
    SGui_Window *Window   = SGUI_ALLOC(sizeof(SGui_Window));
    SGui_Handle  Window_H = SGui_HandleTake();
    SGui_ContainerCreate(&Window->Container, Number);
    
    SGui_HandleSourceSet(Window_H, (void *)Window);

    SGui_AreaReset(&Window->Widget.Clip);
    Window->Widget.Parent = SGUI_HANDLE_INVALID;
    Window->Widget.Type   = SGui_WidgetType_Window;
    return Window_H;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_WindowDestroy(SGui_Handle Window_H)
{
    SGui_Window *Window = SGui_HandleSourceGet(Window_H);
    
    SGui_ContainerDestroy(&Window->Container);
    SGUI_FREE(Window);
    SGui_HandleGive(Window_H);
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
