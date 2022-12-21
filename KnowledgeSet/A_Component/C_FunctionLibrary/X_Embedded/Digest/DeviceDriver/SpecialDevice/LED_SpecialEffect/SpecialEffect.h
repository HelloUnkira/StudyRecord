#ifndef SPECIAL_EFFECT_H
#define SPECIAL_EFFECT_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
//C std lib
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* RGB泛用特效层:将部分通性提取出并归纳 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* RGB三原色抽象重定义 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SpecialEffectColorBody {
    uint8_t Green;  //三原色绿色:8bit位
    uint8_t Blue;   //三原色蓝色:8bit位
    uint8_t Red;    //三原色红色:8bit位
} SpecialEffectColor;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 呼吸特效(注:猛吸一大口,吸的很快,就变成了闪烁了) */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SpecialEffectBreathState {
    int32_t Start;      //呼吸起始位置,要求>=0
    int32_t End;        //呼吸结束位置,要求<=LENGTH - 1
    int32_t MinAlpha;   //百分比的最小值,要求>=0
    int32_t MaxAlpha;   //百分比的最大值,要求<=100
    int32_t Steps;      //一次递进或回退多少个值,要求>0,错误的取值会导致其不顺滑
    int32_t Way;        //+1:单递增,达到最大值立刻跳跃到最小值然后继续递增,往复
                        //-1:单递减,达到最小值立刻跳跃到最大值然后继续递减,往复
                        // 0:双向交替递进
    /* 初始化时配置的参数 */
    int32_t CurrentAlpha;   //起始值,要求:[Min, Max]
    int32_t CurrentWay;     //Way == 0时有效:
                            //+1:起始时递增
                            //-1:起始时递减
} SpecialEffectBreath;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 旋转特效: */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SpecialEffectRotateState {
    /* 旋转范围:[0, LENGTH - 1] */
    int32_t Start;      //旋转起始位置,要求>= 0
    int32_t End;        //旋转结束位置,要求<=LENGTH - 1
    int32_t Steps;      //单次旋转进动几个单元
    int32_t Way;        //+1:向右边旋转
                        //-1:向左边旋转
                        // 0:双向交替旋转
    int32_t Index;      //偏移值,要求:[0, LENGTH - 1]
    /* 初始化时配置的参数 */
    int32_t CurrentIndex;   //初始化时的起始值,要求:[0, LENGTH - 1]
    int32_t CurrentWay;     //Way == 0时有效:
                            //+1:起始时向右边旋转
                            //-1:起始时向左边旋转
} SpecialEffectRotate;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 渐变特效: */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
typedef struct SpecialEffectShadeState {
    /* 渐变范围:[0, LENGTH - 1] */
    int32_t Start;      //渐变起始位置,要求>= 0
    int32_t End;        //渐变结束位置,要求<=LENGTH - 1
    int32_t Count;      //渐变总次数
    /* 初始化时配置的参数 */
    int32_t CurrentCount;   //初始化时的起始值,要求0
} SpecialEffectShade;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
