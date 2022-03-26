//C std lib
#include <stdint.h>
#include <stdbool.h>

#include "FilterKalman.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 一阶卡尔曼滤波器运算 */
double KalmanOrder1_Run(KalmanOrder1 *Filter, double MeasureValue)
{
    //1.计算下一时刻的预测值
    //公式:X(k) = A * X(k−1) + B * u(k)
    Filter->FilterValue = Filter->A * Filter->FilterValue + 
                          Filter->B * Filter->U;
    //2.计算协方差
    //公式:P(k) = A * P(k−1) * AT + Q
    Filter->P = Filter->A * Filter->P * Filter->A + Filter->Q;
    //3.计算卡尔曼增益
    //公式:K(k) = P(k) * HT / (H * P(k) * HT + R)
    Filter->KalmanGain = Filter->P * Filter->H /
                        (Filter->H * Filter->P * Filter->H + Filter->R);
    //4.计算误差更新
    //公式:X(k) = X(k) + K * e = X(k) + K * (Z(k) – H * X(k))
    Filter->FilterValue  = Filter->FilterValue +
                           Filter->KalmanGain * (MeasureValue - 
                           Filter->FilterValue * Filter->H);
    //5.更新后验估计
    //公式:P(k) = P(k) – K * H * P(k) = P(k) * (1 - K * H)
    Filter->P = Filter->P * (1.0 - Filter->KalmanGain * Filter->H);
    //6.反馈预估值
    return Filter->FilterValue;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 多阶卡尔曼滤波器运算 */
void KalmanOrderx_Run(KalmanOrderx *Filter, double *Value)
{
    /*************************************************************************/
    //1.计算下一时刻的预测值
    //公式:X(k) = A * X(k−1) + B * u(k)
    /*************************************************************************/
    {
        uint32_t Index = 0;
        uint32_t Temp  = 0;
        
        for (Index = 0; Index < Filter->Order; Index++) {
            /* 清空缓冲区 */
            Filter->T1[Index] = 0.0;
            /* 计算矩阵A[][]与矩阵FilterValue[]乘和,累加到矩阵T1[] */
            for (Temp = 0; Temp < Filter->Order; Temp++)
                Filter->T1[Index] += Filter->A[Index][Temp] *
                                     Filter->FilterValue[Temp];
            /* 计算矩阵B[]与U的乘,结果累加到矩阵T1[] */
            Filter->T1[Index] += Filter->B[Index] * Filter->U;
        }
        /* 将矩阵T1[]拷贝到矩阵FilterValue[]中 */
        for (Index = 0; Index < Filter->Order; Index++)
            Filter->FilterValue[Index] = Filter->T1[Index];
    }
    /*************************************************************************/
    //2.计算协方差
    //公式:P(k) = A * P(k−1) * AT + Q
    /*************************************************************************/
    {
        uint32_t Index1 = 0, Index2 = 0;
        uint32_t Temp1  = 0, Temp2  = 0;
        
        /* 先计算矩阵A[][]与矩阵P[][]的乘和,累加到矩阵T2[][] */
        for (Index1 = 0; Index1 < Filter->Order; Index1++)
        for (Index2 = 0; Index2 < Filter->Order; Index2++) {
            /* 清空缓冲区 */
            Filter->T2[Index1][Index2] = 0.0;
            /* 计算矩阵A[][]与矩阵P[][]的乘和,累加到矩阵T2[][] */
            for (Temp1 = 0; Temp1 < Filter->Order; Temp1++)
            for (Temp2 = 0; Temp2 < Filter->Order; Temp2++)
                Filter->T2[Index1][Index2] += Filter->A[Temp1][Temp2] *
                                              Filter->P[Temp2][Temp1];
        }
        /* 再计算矩阵T2[][]与矩阵A[][]转置的乘和,累加到矩阵P[][]中 */
        for (Index1 = 0; Index1 < Filter->Order; Index1++)
        for (Index2 = 0; Index2 < Filter->Order; Index2++) {
            /* 清空缓冲区 */
            Filter->P[Index1][Index2] = 0.0;
            /* 计算矩阵T2[][]与矩阵A[][]转置的乘和,累加到矩阵P[][] */
            for (Temp1 = 0; Temp1 < Filter->Order; Temp1++)
            for (Temp2 = 0; Temp2 < Filter->Order; Temp2++)
                Filter->P[Index1][Index2] += Filter->T2[Temp1][Temp2] *
                                             Filter->A[Temp1][Temp2];
        }
        /* 矩阵p[][]累加矩阵Q[] */
        for (Index1 = 0; Index1 < Filter->Order; Index1++)
            Filter->P[Index1][Index1] += Filter->Q[Index1];
    }
    /*************************************************************************/
    //3.计算卡尔曼增益
    //公式:K(k) = P(k) * HT / (H * P(k) * HT + R)
    /*************************************************************************/
    {
        uint32_t Index = 0;
        uint32_t Temp  = 0;
        double   Sum   = 0;
        
        for (Index = 0; Index < Filter->Order; Index++) {
            /* 清空缓冲区 */
            Filter->T1[Index] = 0.0;
            /* 计算矩阵P[][]H[]乘和,累加到矩阵T1[] */
            for (Temp = 0; Temp < Filter->Order; Temp++)
                Filter->T1[Index] += Filter->A[Index][Temp] *
                                     Filter->FilterValue[Temp];
        }
        /* 未知备注 */
        for (Temp = 0; Temp < Filter->Order; Temp++)
            Sum += Filter->H[Temp] * Filter->T1[Temp];
        /* 未知备注 */
        Sum += Filter->R;
        /* 解算卡尔曼增益因子 */
        for (Temp = 0; Temp < Filter->Order; Temp++)
            Filter->KalmanGain[Temp] = Filter->T1[Temp] / Sum;
    }
    /*************************************************************************/
    //4.计算误差更新
    //公式:X(k) = X(k) + K * e = X(k) + K * (Z(k) – H * X(k))
    /*************************************************************************/
    {
        uint32_t Index = 0;
        uint32_t Temp  = 0;
        double   Sum   = 0;
        /* 未知备注 */
        for (Temp = 0; Temp < Filter->Order; Temp++)
            Sum += 0;
        /* 计算误差 */
        for (Index = 0; Index < Filter->Order; Index++)
            Filter->FilterValue[Index] = Filter->FilterValue[Index] +
                                         Filter->KalmanGain[Index] *
                                        (Value[Index] - Filter->H[Temp] *
                                         Filter->FilterValue[Temp]);
    }
    /*************************************************************************/
    //5.更新后验估计
    //公式:P(k) = P(k) – K * H * P(k) = P(k) * (1 - K * H)
    {
        uint32_t Index1 = 0, Index2 = 0;
        
        for (Index1 = 0; Index1 < Filter->Order; Index1++)
        for (Index2 = 0; Index2 < Filter->Order; Index2++)
            Filter->P[Index1][Index2] = Filter->P[Index1][Index2] * (1.0 -
                                        Filter->KalmanGain[Index1] *
                                        Filter->H[Index2]);
    }
    /*************************************************************************/
    //6.反馈预估值
    {
        uint32_t Index = 0;
        
        for (Index = 0; Index < Filter->Order; Index++)
            Value[Index] = Filter->FilterValue[Index];
    }
    /*************************************************************************/
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
