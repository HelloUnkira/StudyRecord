//C std lib
#include <stdint.h>
#include <stdbool.h>

#include "Matrix.h"
#include "FilterKalman.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 一阶卡尔曼滤波器运算 */
double KalmanOrder1_Run(KalmanOrder1 *Filter, double MV)
{
    //1.计算下一时刻的预测值
    //公式:X(K) = A * X(K−1) + B * U(K)
    Filter->FV = Filter->A * Filter->FV + Filter->B * Filter->U;
    //2.计算协方差
    //公式:P(K) = A * P(K−1) * AT + Q
    Filter->P = Filter->A * Filter->P * Filter->A + Filter->Q;
    //3.计算卡尔曼增益
    //公式:K(K) = (P(K) * HT) / (H * P(K) * HT + R)
    Filter->KG = (Filter->P * Filter->H) /
                 (Filter->H * Filter->P * Filter->H + Filter->R);
    //4.计算误差更新
    //公式:X(K) = X(K) + K * e = X(K) + K * (Z(K) – H * X(K))
    Filter->FV = Filter->FV + Filter->KG * (MV - Filter->H * Filter->FV);
    //5.更新后验估计
    //公式:P(K) = P(K) – K * H * P(K)
    Filter->P = Filter->P - (Filter->KG * Filter->H * Filter->P);
    //6.反馈预估值
    return Filter->FV;
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/* 多阶卡尔曼滤波器运算 */
void KalmanOrderx_Run(KalmanOrderx *Filter, double *V)
{
    /* 载入数据 */
    uint32_t N = Filter->N;
    double *KG = Filter->KG;
    double *FV = Filter->FV;
    double *B  = Filter->B;
    double *U  = Filter->U;
    double *A  = Filter->A;
    double *AT = Filter->AT;
    double *H  = Filter->H;
    double *HT = Filter->HT;
    double *P  = Filter->P;
    double *Q  = Filter->Q;
    double *R  = Filter->R;
    double *T1 = Filter->T1;
    double *T2 = Filter->T2;
    double *T3 = Filter->T3;
    double *T4 = Filter->T4;
    //1.计算下一时刻的预测值
    //公式:X(N)(K) = A(N,N) * X(N)(K - 1) + B(N,N) * U(N)
    Matrix_Star(T1, A, FV, N, N, 1);
    Matrix_Star(T2, B, U,  N, N, 1);
    Matrix_Sum(FV, T1, T2, N, 1);
    //2.计算协方差
    //公式:P(N,N)(K) = A(N,N) * P(N,N)(K−1) * AT(N,N) + Q(N,N)
    Matrix_Star(T1, A, P, N, N, N);
    Matrix_Star(T2, T1, AT, N, N, N);
    Matrix_Sum(P, T2, Q, N, N);
    //3.计算卡尔曼增益
    //公式:K(N,N)(K) = (P(N,N)(K) * HT(N,N)) /
    //                 (H(N,N) * P(N,N)(K) * HT(N,N) + R(N,N))
    Matrix_Star(T1, P,  HT, N, N, N);
    Matrix_Star(T3, H,  P,  N, N, N);
    Matrix_Star(T2, T3, HT, N, N, N);
    Matrix_Sum(T3,  T2, R,  N, N);
    Matrix_Inverse(T2, T3, T4, N);
    Matrix_Star(KG, T1, T2, N, N, N);
    //4.计算误差更新
    //公式:X(N)(K) = X(N)(K) + K(N,N) * e =
    //               X(N)(K) + K(N,N) * (Z(N)(K) – H(N,N) * X(N)(K))
    Matrix_Star(T3, H, FV, N, N, 1);
    Matrix_StarData(T3, N, N, -1.0);
    Matrix_Sum(T2, V, T3, N, 1);
    Matrix_Star(T1, KG, T2, N, N, 1);
    Matrix_Sum(T2,  FV, T1, N, 1);
    Matrix_Copy(FV, T2, N, 1);
    //5.更新后验估计
    //公式:P(N,N)(K) = P(N,N)(K) – K(N,N) * H(N,N) * P(N,N)(K)
    Matrix_Star(T1, KG, H, N, N, N);
    Matrix_Star(T2, T1, P, N, N, N);
    Matrix_StarData(T2, N, N, -1.0);
    Matrix_Sum(T1, P, T2, N, N);
    Matrix_Copy(P, T1, N, N);
    //6.反馈预估值
    Matrix_Copy(V, FV, N, 1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
