#include "SpecialEffect.h"
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 通用轮转函数 */
void GeneralTurnRound(int32_t Way,  int32_t Steps, int32_t *Current,
                      int32_t Left, int32_t Right, int32_t *CurrentWay)
{
    /* 单向轮转时 */
    if (Way != 0) {
        *Current += Way * Steps;
        /* 单向递增轮转时: */
        if (Way == +1 && *Current > Right)
            *Current -= Right - Left + 1;
        /* 单向递减轮转时: */
        if (Way == -1 && *Current < Left)
            *Current += Right - Left + 1;
    }
    /* 双向轮转时 */
    if (Way == 0) {
        *Current += *CurrentWay * Steps;
        /* 轮转到溢出时,更换轮转方向 */
        if (*Current >= Right || *Current <= Left)
            *CurrentWay = 0 - *CurrentWay;
        /* 因为steps的不恰当选择会让其超出[min, max]的范围,容错 */
        *Current =  *Current > Right ? Right : *Current < Left ? Left : *Current;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 呼吸特效执行 */
void SpecialEffectExecuteBreath(SpecialEffectBreath *BreathState,
                                SpecialEffectColor  *Default,
                                SpecialEffectColor  *Current)
{
    /* 呼吸特效状态轮转 */
    GeneralTurnRound(BreathState->Way,      BreathState->Steps,    &(BreathState->CurrentAlpha),
                     BreathState->MinAlpha, BreathState->MaxAlpha, &(BreathState->CurrentWay));
    /* 计算Alpha */
    float Breath = ((float)(BreathState->CurrentAlpha)) / ((float)100);
    /* 调整比例 */
    for (int32_t Index = BreathState->Start; Index <= BreathState->End; Index++) {
        Current[Index].Green = Breath * (float)Default[Index].Green;
        Current[Index].Blue  = Breath * (float)Default[Index].Blue;
        Current[Index].Red   = Breath * (float)Default[Index].Red;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 旋转特效执行 */
void SpecialEffectExecuteRotate(SpecialEffectRotate *RotateState,
                                SpecialEffectColor  *Default,
                                SpecialEffectColor  *Current)
{
    /* 旋转特效状态轮转 */
    GeneralTurnRound(BreathState->Way,   BreathState->Steps, &(BreathState->CurrentIndex),
                     BreathState->Start, BreathState->End,   &(BreathState->CurrentWay));
    /* 计算Index */
    int32_t Rotate = RotateState->CurrentIndex + RotateState->Index;
    /* 调整比例 */
    for (int32_t Index0 = RotateState->Start; Index0 <= RotateState->End; Index0++) {
        /* 绝对剩余域: */
        int32_t Index1 = (Index0 + Rotate) % (RotateState->End - RotateState->Start + 1);
        /* 外部更新 */
        Current[Index0].Green = Default[Index1].Green;
        Current[Index0].Blue  = Default[Index1].Blue;
        Current[Index0].Red   = Default[Index1].Red;
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 渐变特效执行 */
void SpecialEffectExecuteShade(SpecialEffectShade *ShadeState,
                               SpecialEffectColor *Start,
                               SpecialEffectColor *End,
                               SpecialEffectColor *Current)
{
    int32_t GradientGreen = 0;
    int32_t GradientBlue  = 0;
    int32_t GradientRed   = 0;
    /* 公式:Gradient = A + (B-A) * N / Step */
    for (int32_t Index = ShadeState->Start, Index <= ShadeState->End, Index++) {
        /* 解算Green增量 */
        GradientGreen = ((int32_t)(Start[index].Green)) +
                        ((int32_t)(End[index].Green) - 
                         (int32_t)(Start[index].Green)) *
                         (int32_t)(ShadeState->CurrentCount) /
                         (int32_t)(ShadeState->Count);
        /* 解算Blue增量 */
        GradientBlue  = ((int32_t)(Start[index].Blue)) +
                        ((int32_t)(End[index].Blue) - 
                         (int32_t)(Start[index].Blue)) *
                         (int32_t)(ShadeState->CurrentCount) /
                         (int32_t)(ShadeState->Count);
        /* 解算Red增量 */
        GradientRed   = ((int32_t)(Start[index].Red)) +
                        ((int32_t)(End[index].Red) - 
                         (int32_t)(Start[index].Red)) *
                         (int32_t)(ShadeState->CurrentCount) /
                         (int32_t)(ShadeState->Count);
        /* 更新Color */
        Current[Index].Green = GradientGreen;
        Current[Index].Blue  = GradientBlue;
        Current[Index].Red   = GradientRed;
    }
    ShadeState->CurrentCount++;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
