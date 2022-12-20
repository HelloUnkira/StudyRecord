#ifndef MATRIX_H
#define MATRIX_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 因为只有头文件,所以仅当需要使用的源才可引用 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define MatrixToArray(I, J, Col) ((I) * (Col) + (J))
//#define MatrixToArray(J, I, Row) ((J) * (Row) + (I))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵内部使用的精度 */
#define DiscreteMathMatrix_Precision    (1E-6)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
static inline void Matrix_Copy(double *Matrix0, double *Matrix1, uint32_t M, uint32_t N)
{
    for (uint32_t I = 0; I < M; I++)
    for (uint32_t J = 0; J < N; J++) {
        uint32_t Index = MatrixToArray(I, J, N);
        Matrix0[Index] = Matrix1[Index];
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵和运算(Matrix[M][N]):Matrix += Data */
static inline void Matrix_SumData(double *Matrix, uint32_t M, uint32_t N, double Data)
{
    for (uint32_t I = 0; I < M; I++)
    for (uint32_t J = 0; J < N; J++)
        Matrix[MatrixToArray(I, J, N)] += Data;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵星运算(Matrix[M][N]):Matrix *= Data */
static inline void Matrix_StarData(double *Matrix, uint32_t M, uint32_t N, double Data)
{
    for (uint32_t I = 0; I < M; I++)
    for (uint32_t J = 0; J < N; J++)
        Matrix[MatrixToArray(I, J, N)] *= Data;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵和运算(Matrix0[M][N],Matrix1[M][N],Matrix2[M][N]):Matrix0 = Matrix1 + Matrix2 */
static inline void Matrix_Sum(double *Matrix0, double *Matrix1, double *Matrix2,
                              uint32_t M, uint32_t N)
{
    /* 公式:Matrix0[I][J] = Matrix1[I][J] (+,-) Matrix2[I][J] */
    for (uint32_t I = 0; I < M; I++)
    for (uint32_t J = 0; J < N; J++) {
        uint32_t Index = MatrixToArray(I, J, N);
        Matrix0[Index] = Matrix1[Index] + Matrix2[Index];
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵星运算(Matrix0[M][N],Matrix1[M][K],Matrix2[K][N]):Matrix0 = Matrix1 * Matrix2 */
static inline void Matrix_Star(double *Matrix0, double *Matrix1, double *Matrix2,
                               uint32_t M, uint32_t K, uint32_t N)
{
    /* 公式:Matrix0[I][J] = SUM['+'](T=(0,K-1)) Matrix1[I][T] * Matrix2[T][J] */
    for (uint32_t I = 0; I < M; I++)
    for (uint32_t J = 0; J < N; J++) {
         uint32_t Index1 = MatrixToArray(I, J, N);
         Matrix0[Index1] = 0.0;
    for (uint32_t T = 0; T < K; T++) {
        uint32_t Index2 = MatrixToArray(I, T, K);
        uint32_t Index3 = MatrixToArray(T, J, N);
        Matrix0[Index1] += Matrix1[Index2] * Matrix2[Index3];
    }
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵幂运算(Matrix0[N][N],Matrix1[N][N],Matrix2[N][N]):Matrix0 = Matrix1 ** Exp */
static inline void Matrix_Exp(double *Matrix0, double *Matrix1, double *Matrix2,
                              uint32_t N, uint32_t Exp)
{
    Matrix_Copy(Matrix2, Matrix1, N, N);
    for (uint32_t I = 0; I < Exp; I++) {
        Matrix_Star(Matrix0, Matrix2, Matrix1, N, N, N);
        Matrix_Copy(Matrix2, Matrix0, N, N);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵转置运算:Matrix0 = Matrix1**T */
/* 要求:(Matrix0[M][N],Matrix1[N][M]) */
static inline void Matrix_Transpose(double *Matrix0, double *Matrix1, uint32_t M, uint32_t N)
{
    /* 公式:Matrix0[I][J] = Matrix1[J][I] */
    for (uint32_t I = 0; I < M; I++)
    for (uint32_t J = 0; J < N; J++) {
        uint32_t Index1 = MatrixToArray(I, J, N);
        uint32_t Index2 = MatrixToArray(J, I, N);
        Matrix0[Index1] = Matrix1[Index2];
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵(方阵)逆运算(Matrix0[N][N],Matrix1[N][N],Matrix2[N][2*N]):Matrix0 = Matrix1**(-1) */
static bool Matrix_Inverse(double *Matrix0, double *Matrix1, double *Matrix2, uint32_t N)
{
    if (N == 0)
        return false;
    /* 1.一阶矩阵:直接求逆 */
    if (N == 1) {
        Matrix0[MatrixToArray(0, 0, N)] = 1 / Matrix1[MatrixToArray(0, 0, N)];
        return true;
    }
    /* 2.二阶矩阵:伴随矩阵法 */
    if (N == 2) {
        double Temp = Matrix1[MatrixToArray(0, 0, N)] * Matrix1[MatrixToArray(1, 1, N)] -
                      Matrix1[MatrixToArray(0, 1, N)] * Matrix1[MatrixToArray(1, 0, N)];
        Matrix0[MatrixToArray(0, 0, N)] = +Matrix1[MatrixToArray(1, 1, N)] / Temp;
        Matrix0[MatrixToArray(1, 1, N)] = +Matrix1[MatrixToArray(0, 0, N)] / Temp;
        Matrix0[MatrixToArray(0, 1, N)] = -Matrix1[MatrixToArray(0, 1, N)] / Temp;
        Matrix0[MatrixToArray(1, 0, N)] = -Matrix1[MatrixToArray(1, 0, N)] / Temp;
        return true;
    }
    /* 3.高阶矩阵:高斯消元法(LU分解法)(不使用) */
    /* 3.高阶矩阵:全选主元高斯-约当消去法(高效) */
    if (N >= 3) {
        /* 1.生成增广矩阵:Matrix2 = {Matrix1, E} */
        for (uint32_t I = 0; I < N; I++)
        for (uint32_t J = 0; J < N; J++)
            Matrix2[MatrixToArray(I, J, N * 2)] =
            Matrix1[MatrixToArray(I, J, N)];
        for (uint32_t I = 0; I < N; I++)
        for (uint32_t J = 0; J < N; J++)
            Matrix2[MatrixToArray(I, J + N, N * 2)] = (I == J) ? 1 : 0;
        /* 2.全选主元:(共计N次全选主元) */
        for (uint32_t I = 0; I < N; I++) {
            /* 1.列选主元,检查精度:  DiscreteMathMatrix_Precision */
            double   MMax = 0.0;
            uint32_t MI   = 0;
            /* 要选取绝对值最大的值,即使是负数简易变换也能归一化 */
            for (uint32_t J = I; J < N; J++) {
                double Temp = Matrix2[MatrixToArray(J, I, N * 2)];
                       Temp = Temp < 0 ? -Temp : Temp;
                if (MMax < Temp) {
                    MMax = Temp;
                    MI = J;
                }
            }
            /* 如果连主元都有为0的可能,那么矩阵是奇异矩阵 */
            if (MMax <= DiscreteMathMatrix_Precision * 2)
                return false;
            /* 2.经过行变换移动到主对角线上 */
            if (MI != I) {
                /* 交换主元所在行与目标行 */
                for (uint32_t J = 0; J < N * 2; J++) {
                    double Temp1 = Matrix2[MatrixToArray(MI, J, N * 2)];
                    double Temp2 = Matrix2[MatrixToArray(I,  J, N * 2)];
                    Matrix2[MatrixToArray(MI, J, N * 2)] = Temp2;
                    Matrix2[MatrixToArray(I,  J, N * 2)] = Temp1;
                }
            }
            MMax = Matrix2[MatrixToArray(I, I, N * 2)];
            /* 3.主元行归一化 */
            for (uint32_t J = 0; J < N * 2; J++)
                Matrix2[MatrixToArray(I, J, N * 2)] /= MMax;
            /* 4.主元列归零化 */
            for (uint32_t J = 0; J < N; J++)
                if (I != J) {
                    double Temp = Matrix2[MatrixToArray(J, I, N * 2)];
                    for (uint32_t K = 0; K < N * 2; K++)
                        Matrix2[MatrixToArray(J, K, N * 2)] -=
                        Matrix2[MatrixToArray(I, K, N * 2)] * Temp;
                }
        }
        /* 3.拷贝逆矩阵 */
        for (uint32_t I = 0; I < N; I++)
        for (uint32_t J = 0; J < N; J++)
            Matrix0[MatrixToArray(I, J, N)] = 
            Matrix2[MatrixToArray(I, J + N, N * 2)];
        return true;
    }
    return false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 行列式矩阵(方阵)值运算(Matrix[N][N], Help[N][N]): |Matrix0| */
static double Matrix_Value(double *Matrix0, double *Matrix1, uint32_t N)
{
    /* 备注:逆序数法涉及到全排列问题较复杂,暂未研究,插眼存档 */
    /* 矩阵行列式:高斯消元法(生成上三角矩阵) */
    /* 1.将其拷贝到缓冲矩阵中 */
    for (uint32_t I = 0; I < N; I++)
    for (uint32_t J = 0; J < N; J++) {
         uint32_t Index = MatrixToArray(I, J, N);
         Matrix1[Index] = Matrix0[Index];
    }
    /* 2.列清除,只清除当前行下面的列 */
    for (uint32_t I = 0; I < N; I++) {
        double Temp = Matrix1[MatrixToArray(I, I, N)];
        /* 只要遇到主元为0,终止并退出 */
        if (DiscreteMathMatrix_Precision > (Temp < 0 ? -Temp : Temp))
            return 0.0;
        for (uint32_t J = I + 1; J < N; J++) {
                double Temp1 = Matrix1[MatrixToArray(J, I, N)];
                for (uint32_t K = 0; K < N; K++)
                    Matrix1[MatrixToArray(J, K, N)] -=
                    Matrix1[MatrixToArray(I, K, N)] / Temp * Temp1;
        }
    }
    /* 3.计算结果 */
    double Result = 1.0;
    for (uint32_t I = 0; I < N; I++)
        Result *= Matrix1[MatrixToArray(I, I, N)];
    return Result;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
