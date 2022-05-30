#include "SGui_Kernel.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 实现目标:
 * 设计简要的动画服务
 * 并当产生动画事件时上报给控件
 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 资源容器集合扩张缩减因子 */
/* 数量越大,额外维护空间越小 */
/* 数量越小,内存需求弹性越高 */
#define SGUI_ANIMATION_FACTOR   5
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SimpleGui_AnimationUnit {
    SGui_Handle Handle;     /* 控件句柄记录 */
    uint32_t Period;        /* 周期 */
    uint32_t Remainder;     /* 周期下的计数余值 */
    uint32_t Label:8;       /* 句柄标号 */
    uint32_t Valid:1;       /* 该单元是否有效 */
    uint32_t Execute:1;     /* 该单元是否执行 */
    uint32_t Repeat:1;      /* 该单元是否重复 */
} SGui_AnimationUnit;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SimpleGui_Animation {
    SGui_AnimationUnit *Unit;
    uint32_t Length;
    uint32_t Number;
    uint64_t RecordCount;
    void   (*EventDispatchCallback)(SGui_Handle Handle, uint8_t Label);
    uint8_t  Valid:1;       /* 是否启动 */
} SGui_Animation;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static SGui_Animation Animation = {0};
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_AnimationMSHandler(void)
{
    /* 1.动画是否启动 */
    if (Animation.Valid == false)
        return;
    /* 2.动画是否有检查项 */
    if (Animation.Number == 0 || Animation.Unit == NULL)
        return;
    /* 3.动画自然计数值递增(64位毫秒级,不担心溢出) */
    Animation.RecordCount++;
    /* 4.迭代动画单元检查其有效性 */
    for (uint32_t Index = 0; Index < Animation.Length; Index++) {
        /* 跳过无效动画 */
        if (Animation.Unit[Index].Valid == false)
            continue;
        /* 跳过未工作的动画 */
        if (Animation.Unit[Index].Execute == false)
            continue;
        /* 跳过未达到周期的动画 */
        if (Animation.RecordCount % Animation.Unit[Index].Period !=
                                    Animation.Unit[Index].Remainder)
            continue;
        /* 是否为单周期动画,以此更新工作情况 */
        if (Animation.Unit[Index].Repeat == false)
            Animation.Unit[Index].Execute = false;
        /* 投送动画周期更新到达事件 */
        if (Animation.EventDispatchCallback != NULL)
            Animation.EventDispatchCallback(Animation.Unit[Index].Handle,
                                            Animation.Unit[Index].Label);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
void SGui_AnimationRegister(void (*EventCallback)(SGui_Handle Handle, uint8_t Label))
{
    Animation.EventDispatchCallback = EventCallback;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 创建一个动画 */
void SGui_AnimationCreate(SGui_Handle Handle, uint8_t Label)
{
    /* 遍历检查是否存在冲突项 */
    for (uint32_t Index = 0; Index < Animation.Length; Index++) {
        /* 跳过合法的动画单元 */
        if (Animation.Unit[Index].Valid == true)
            continue;
        /* 如果遇到冲突项,退出 */
        if (Animation.Unit[Index].Handle == Handle &&
            Animation.Unit[Index].Label  == Label)
            return;
    }
    /* 动画单元不足检查,扩充它 */
    if (Animation.Number == Animation.Length) {
        uint32_t Number = Animation.Number + SGUI_ANIMATION_FACTOR;
        uint32_t Length = Animation.Length + SGUI_ANIMATION_FACTOR;
        SGui_AnimationUnit *Unit = SGUI_ALLOC(sizeof(SGui_AnimationUnit) * Length);
        /* 拷贝原生的资源管理单元集合 */
        for (uint32_t Index = 0; Index < Animation.Length; Index++)
            Unit[Index] = Animation.Unit[Index];
        /* 初始化新生成的资源管理单元集合 */
        for (uint32_t Index = Animation.Length; Index < Length; Index++)
            Unit[Index].Valid = false;
        /* 释放原生的资源管理单元集合 */
        SGUI_FREE(Animation.Unit);
        /* 设置新生成的资源管理单元集合 */
        Animation.Number = Number;
        Animation.Length = Length;
        Animation.Unit   = Unit;
    }
    /* 遍历找到一个空闲单元将其加入 */
    for (uint32_t Index = 0; Index < Animation.Length; Index++) {
        /* 跳过合法的动画单元 */
        if (Animation.Unit[Index].Valid == true)
            continue;
        Animation.Unit[Index].Handle  = Handle;
        Animation.Unit[Index].Label   = Label;
        Animation.Unit[Index].Valid   = true;
        Animation.Unit[Index].Execute = false;
        Animation.Number--;
        return;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 销毁一个动画 */
void SGui_AnimationDestroy(SGui_Handle Handle, uint8_t Label)
{
    /* 寻找到目标动画单元 */
    for (uint32_t Index = 0; Index < Animation.Length; Index++) {
        /* 跳过合法的动画单元 */
        if (Animation.Unit[Index].Valid == true)
            continue;
        /* 跳过非目标动画单元 */
        if (Animation.Unit[Index].Handle != Handle ||
            Animation.Unit[Index].Label  != Label)
            continue;
        /* 取消目标动画单元 */
        Animation.Unit[Index].Valid = false;
        Animation.Number++;
        /* 如果空闲动画单元空闲过多,压缩它 */
        if (Animation.Number > SGUI_ANIMATION_FACTOR) {
            uint32_t Count  = 0;
            uint32_t Number = Animation.Number - SGUI_ANIMATION_FACTOR;
            uint32_t Length = Animation.Length - SGUI_ANIMATION_FACTOR;
            SGui_AnimationUnit *Unit = SGUI_ALLOC(sizeof(SGui_AnimationUnit) * Length);
            /* 拷贝原生的资源管理单元集合 */
            for (uint32_t Index = 0; Index < Animation.Length; Index++)
                if (Animation.Unit[Index].Valid == true)
                    Unit[Count++] = Animation.Unit[Index];
            /* 释放原生的资源管理单元集合 */
            SGUI_FREE(Animation.Unit);
            /* 设置新生成的资源管理单元集合 */
            Animation.Number = Number;
            Animation.Length = Length;
            Animation.Unit   = Unit;
        }
        return;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 启动一个动画 */
void SGui_AnimationStart(SGui_Handle Handle, uint8_t Label, uint32_t Period, bool Repeat)
{
    /* 寻找到目标动画单元 */
    for (uint32_t Index = 0; Index < Animation.Length; Index++) {
        /* 跳过非法的动画单元 */
        if (Animation.Unit[Index].Valid == true)
            continue;
        /* 跳过非目标动画单元 */
        if (Animation.Unit[Index].Handle != Handle ||
            Animation.Unit[Index].Label  != Label)
            continue;
        /* 计算周期余子 */
        uint64_t Remainder = Animation.RecordCount % Period;
        /* 设置目标动画单元 */
        Animation.Unit[Index].Period    = Period;
        Animation.Unit[Index].Remainder = Remainder;
        Animation.Unit[Index].Execute   = true;
        Animation.Unit[Index].Repeat    = Repeat;
        return;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 停止一个动画 */
void SGui_AnimationStop(SGui_Handle Handle, uint8_t Label)
{
    /* 寻找到目标动画单元 */
    for (uint32_t Index = 0; Index < Animation.Length; Index++) {
        /* 跳过非法的动画单元 */
        if (Animation.Unit[Index].Valid == true)
            continue;
        /* 跳过非目标动画单元 */
        if (Animation.Unit[Index].Handle != Handle ||
            Animation.Unit[Index].Label  != Label)
            continue;
        /* 设置目标动画单元 */
        Animation.Unit[Index].Execute = false;
        return;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 中止一个动画 */
void SGui_AnimationAbort(SGui_Handle Handle, uint8_t Label)
{
    /* 寻找到目标动画单元 */
    for (uint32_t Index = 0; Index < Animation.Length; Index++) {
        /* 跳过非法的动画单元 */
        if (Animation.Unit[Index].Valid == true)
            continue;
        /* 跳过非目标动画单元 */
        if (Animation.Unit[Index].Handle != Handle ||
            Animation.Unit[Index].Label  != Label)
            continue;
        /* 设置目标动画单元 */
        Animation.Unit[Index].Execute = false;
        return;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 恢复一个动画 */
void SGui_AnimationResume(SGui_Handle Handle, uint8_t Label)
{
    /* 寻找到目标动画单元 */
    for (uint32_t Index = 0; Index < Animation.Length; Index++) {
        /* 跳过非法的动画单元 */
        if (Animation.Unit[Index].Valid == true)
            continue;
        /* 跳过非目标动画单元 */
        if (Animation.Unit[Index].Handle != Handle ||
            Animation.Unit[Index].Label  != Label)
            continue;
        /* 设置目标动画单元 */
        Animation.Unit[Index].Execute = true;
        return;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
