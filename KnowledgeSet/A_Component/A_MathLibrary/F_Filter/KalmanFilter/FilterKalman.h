#ifndef FILTER_KALMAN_H
#define FILTER_KALMAN_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include "Matrix.h"
/*实现目标:
 *卡尔曼滤波器
 *实现部分参考于CSDN
 *我不是代码的产生者
 *我只是代码的搬运工
 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 一阶卡尔曼滤波器 */
typedef struct FilterKalmanOrder1 {
    double KG;  //Kalamn增益
    double FV;  //上一时刻的滤波值
    double B;   //系统输入参数权值
    double U;   //系统输入参数
    double A;   //X(N)=A * X(N-1) + U(N),   U(N)~N(0,Q)
    double H;   //Z(N)=H * X(N)   + W(N),   W(N)~N(0,R)
    double P;   //估计误差协方差
    double Q;   //预测过程噪声偏差的方差
    double R;   //测量噪声偏差(系统搭建好以后,通过测量统计实验获得)
} KalmanOrder1;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 一阶卡尔曼滤波器初始化 */
static inline void KalmanOrder1_Init(KalmanOrder1 *Filter, double FV,
         double B, double U, double A, double H, double P, double Q, double R)
{
    Filter->FV = FV;    //测量的初始值,通常为0
    Filter->B = B;      //推荐参数:0
    Filter->U = U;      //推荐参数:0
    Filter->A = A;      //推荐参数:1
    Filter->H = H;      //推荐参数:1
    Filter->P = P;      //推荐参数:10
    Filter->Q = Q;      //预测(过程)噪声方差,影响收敛速率,可以根据实际需求给出
    Filter->R = R;      //测量(观测)噪声方差,可以通过实验手段获得
}
/* 一阶卡尔曼滤波器运算 */
double KalmanOrder1_Run(KalmanOrder1 *Filter, double MV);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 多阶卡尔曼滤波器 */
typedef struct FilterKalmanOrderx {
    uint32_t N;         //阶数
    double *KG; //N*N   //Kalamn增益
    double *FV; //N*1   //上一时刻的滤波值
    double *B;  //N*N   //系统输入参数权值
    double *U;  //N*1   //系统输入参数
    double *A;  //N*N   //X(N) = A(N,N) * X(N-1) + U(N),  U(N)~N(0,Q)
    double *AT; //N*N   //上一参数的转置
    double *H;  //N*N   //Z(N) = H(N,N) * X(N)   + W(N),  W(N)~N(0,R)
    double *HT; //N*N   //上一参数的转置
    double *P;  //N*N   //估计误差协方差
    double *Q;  //N*N   //预测过程噪声偏差的方差
    double *R;  //N*N   //测量噪声偏差(系统搭建好以后,通过测量统计实验获得)
    /* 额外所需的缓冲空间 */
    double *T1; //N*N
    double *T2; //N*N
    double *T3; //N*N
    double *T4; //N*N*2
    /*  */
} KalmanOrderx;
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 一阶卡尔曼滤波器初始化 */
static inline uint32_t KalmanOrderx_Init(KalmanOrderx *Filter)
{
    #ifdef  KalmanOrder
    #undef  KalmanOrder
    #endif
    #define KalmanOrder 3
    
    static double KG[KalmanOrder][KalmanOrder] = {0};
    static double FV[KalmanOrder] = {0};
    static double B[KalmanOrder][KalmanOrder] = {0};
    static double U[KalmanOrder] = {0};
    static double A[KalmanOrder][KalmanOrder] = {0};
    static double AT[KalmanOrder][KalmanOrder] = {0};
    static double H[KalmanOrder][KalmanOrder] = {0};
    static double HT[KalmanOrder][KalmanOrder] = {0};
    static double P[KalmanOrder][KalmanOrder] = {0};
    static double Q[KalmanOrder][KalmanOrder] = {0};
    static double R[KalmanOrder][KalmanOrder] = {0};
    static double T1[KalmanOrder][KalmanOrder] = {0};
    static double T2[KalmanOrder][KalmanOrder] = {0};
    static double T3[KalmanOrder][KalmanOrder] = {0};
    static double T4[KalmanOrder][KalmanOrder][2] = {0};
    
    for (uint32_t I = 0; I < KalmanOrder; I++)
    for (uint32_t J = 0; J < KalmanOrder; J++) {
        B[I][J] = 0.1;
        A[I][J] = 1.0;
        H[I][J] = 1.0;
        P[I][J] = 0.1;
        Q[I][J] = 0.05;
        R[I][J] = 0.1;
    }
    
    Matrix_Transpose(AT, A, KalmanOrder, KalmanOrder);
    Matrix_Transpose(HT, H, KalmanOrder, KalmanOrder);
    
    Filter->N    = KalmanOrder;
    Filter->KG   = KG;
    Filter->FV   = FV;
    Filter->B    = B;
    Filter->U    = U;
    Filter->A    = A;
    Filter->AT   = AT;
    Filter->H    = H;
    Filter->HT   = HT;
    Filter->P    = P;
    Filter->Q    = Q;
    Filter->R    = R;
    Filter->T1   = T1;
    Filter->T2   = T2;
    Filter->T3   = T3;
    Filter->T4   = T4;
    
    return KalmanOrder;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 多阶卡尔曼滤波器运算 */
bool KalmanOrderx_Run(KalmanOrderx *Filter, double *V);
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
