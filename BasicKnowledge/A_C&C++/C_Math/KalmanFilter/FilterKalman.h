#ifndef FILTERKALMAN_H
#define FILTERKALMAN_H
//C std lib
#include <stdint.h>
#include <stdbool.h>

/*实现目标:
 *卡尔曼滤波器
 *实现部分参考于CSDN
 *我不是代码的产生者
 *我只是代码的搬运工
 */

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 一阶卡尔曼滤波器 */
typedef struct FilterKalmanOrder1 {
    double KalmanGain;  //Kalamn增益
    double FilterValue; //上一时刻的滤波值
    double B;           //系统输入参数权值
    double U;           //系统输入参数
    double A;           //x(n)=A*x(n-1)+u(n),   u(n)~N(0,Q)
    double H;           //z(n)=H*x(n)+w(n),     w(n)~N(0,R)
    double P;           //估计误差协方差
    double Q;           //预测过程噪声偏差的方差
    double R;           //测量噪声偏差(系统搭建好以后,通过测量统计实验获得)
} KalmanOrder1;
/* 一阶卡尔曼滤波器初始化 */
static inline void KalmanOrder1_Init(KalmanOrder1 *Filter, double FilterValue,
         double B, double U, double A, double H, double P, double Q, double R)
{
    Filter->FilterValue = FilterValue;  //测量的初始值,通常为0
    Filter->B = B;  //推荐参数:0
    Filter->U = U;  //推荐参数:0
    Filter->A = A;  //推荐参数:1
    Filter->H = H;  //推荐参数:1
    Filter->P = P;  //推荐参数:10
    Filter->Q = Q;  //预测(过程)噪声方差,影响收敛速率,可以根据实际需求给出
    Filter->R = R;  //测量(观测)噪声方差,可以通过实验手段获得
}
/* 一阶卡尔曼滤波器运算 */
double KalmanOrder1_Run(KalmanOrder1 *Filter, double MeasureValue);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 多阶卡尔曼滤波器 */
typedef struct FilterKalmanOrderx {
    uint32_t Order;       //阶数
    double  *KalmanGain;  //Kalamn增益
    double  *FilterValue; //上一时刻的滤波值
    double  *B;           //系统输入参数权值
    double   U;           //系统输入参数
    double **A;           //x(n)=A*x(n-1)+u(n),   u(n)~N(0,Q)
    double  *H;           //z(n)=H*x(n)+w(n),     w(n)~N(0,R)
    double **P;           //估计误差协方差
    double  *Q;           //预测过程噪声偏差的方差
    double   R;           //测量噪声偏差(系统搭建好以后,通过测量统计实验获得)
    /* 额外所需的缓冲空间 */
    double  *T1;
    double **T2;
    /*  */
} KalmanOrderx;
/* 多阶卡尔曼滤波器运算 */
/* 备注:内部逆矩阵计算目前存在争议,未测试 */
void KalmanOrderx_Run(KalmanOrderx *Filter, double *Value);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif
