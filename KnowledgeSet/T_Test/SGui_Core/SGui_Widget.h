#ifndef SGUI_WIDGET_H
#define SGUI_WIDGET_H
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*实现目标:
 *这是所有派生类(如窗口,按钮等等)的共生基类
 *它需要精简的集合出所有派生类的全部通性
 *并为其他派生类提供一个明确的抽象功能集合
 */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 控件基类 */
typedef struct SGuiWidgetBody {
    /* 当前控件的父控件,祖先控件无父控件 */
    SGui_Handle Parent;
    /* 相对父控件的剪切域(如果父容器存在) */
    /* 相对画布的相对剪切域(如果父容器不存在) */
    SGui_Area Clip;
    /* 事件派发回调(派发未知事件到控件) */
    SGui_EventCallback Callback;
    /* 控件背景:控件绘制前的绘制动作 */
    SGui_Color Background;
    /* 派生控件类型 */
    uint32_t WidgetType;
} SGuiWidget;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 为了简化,该部分不做实际额外抽象,为派生控件提供更多的自定义规则 */
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
