#ifndef MATRIX_H
#define MATRIX_H
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 因为只有头文件,所以仅当需要使用的源才可引用 */
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#define Matrix_ToArray(I, J, Col) ((I) * (Col) + (J))
//#define Matrix_ToArray(J, I, Row) ((J) * (Row) + (I))
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵内部使用的精度 */
#define Matrix_Precision    (1E-6)
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵交换运算(Matrix[M][N]):Matrix0 <==> Matrix1 */
static inline void Matrix_Swap(double *Matrix0, double *Matrix1, uint32_t M, uint32_t N)
{
    #if 1
    for (uint32_t T = 0; T < M * N; T++) {
         double Temp = Matrix0[T];
         Matrix0[T]  = Matrix1[T];
         Matrix1[T]  = Temp;
    }
    #else
    for (uint32_t I = 0; I < M; I++)
    for (uint32_t J = 0; J < N; J++) {
         uint32_t Index = Matrix_ToArray(I, J, N);
         double Temp    = Matrix0[Index];
         Matrix0[Index] = Matrix1[Index];
         Matrix1[Index] = Temp;
    }
    #endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵拷贝运算(Matrix[M][N]):Matrix0 = Matrix1 */
static inline void Matrix_Copy(double *Matrix0, double *Matrix1, uint32_t M, uint32_t N)
{
    #if 1
    for (uint32_t T = 0; T < M * N; Matrix0[T] = Matrix1[T], T++);
    #else
    for (uint32_t I = 0; I < M; I++)
    for (uint32_t J = 0; J < N; J++) {
         uint32_t Index = Matrix_ToArray(I, J, N);
         Matrix0[Index] = Matrix1[Index];
    }
    #endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵和运算(Matrix[M][N]):Matrix += Data */
static inline void Matrix_SumData(double *Matrix, uint32_t M, uint32_t N, double Data)
{
    #if 1
    for (uint32_t T = 0; T < M * N; Matrix[T] += Data, T++);
    #else
    for (uint32_t I = 0; I < M; I++)
    for (uint32_t J = 0; J < N; J++)
         Matrix[Matrix_ToArray(I, J, N)] += Data;
    #endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵星运算(Matrix[M][N]):Matrix *= Data */
static inline void Matrix_StarData(double *Matrix, uint32_t M, uint32_t N, double Data)
{
    #if 1
    for (uint32_t T = 0; T < M * N; Matrix[T] *= Data, T++);
    #else
    for (uint32_t I = 0; I < M; I++)
    for (uint32_t J = 0; J < N; J++)
         Matrix[Matrix_ToArray(I, J, N)] *= Data;
    #endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵和运算(Matrix0[M][N],Matrix1[M][N],Matrix2[M][N]):Matrix0 = Matrix1 + Matrix2 */
static inline void Matrix_Sum(double *Matrix0, double *Matrix1, double *Matrix2,
                              uint32_t M, uint32_t N)
{
    #if 1
    for (uint32_t T = 0; T < M * N; Matrix0[T] = Matrix1[T] + Matrix2[T], T++);
    #else
    /* 公式:Matrix0[I][J] = Matrix1[I][J] (+,-) Matrix2[I][J] */
    for (uint32_t I = 0; I < M; I++)
    for (uint32_t J = 0; J < N; J++) {
         uint32_t Index = Matrix_ToArray(I, J, N);
         Matrix0[Index] = Matrix1[Index] + Matrix2[Index];
    }
    #endif
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵星运算(Matrix0[M][N],Matrix1[M][K],Matrix2[K][N]):Matrix0 = Matrix1 * Matrix2 */
static inline void Matrix_Star(double *Matrix0, double *Matrix1, double *Matrix2,
                               uint32_t M, uint32_t K, uint32_t N)
{
    /* 公式:Matrix0[I][J] = SUM{T=(0,K-1)} Matrix1[I][T] * Matrix2[T][J] */
    for (uint32_t I = 0; I < M; I++)
    for (uint32_t J = 0; J < N; J++) {
         uint32_t Index1 = Matrix_ToArray(I, J, N);
         Matrix0[Index1] = 0.0;
    for (uint32_t T = 0; T < K; T++) {
         uint32_t Index2 = Matrix_ToArray(I, T, K);
         uint32_t Index3 = Matrix_ToArray(T, J, N);
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
    Matrix_Copy(Matrix0, Matrix1, N, N);
    for (uint32_t I = 0; I < Exp; I++) {
         Matrix_Copy(Matrix2, Matrix0, N, N);
         Matrix_Star(Matrix0, Matrix2, Matrix1, N, N, N);
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵转置运算(Matrix0[M][N],Matrix1[N][M]):Matrix0 = Matrix1**T */
static inline void Matrix_Transpose(double *Matrix0, double *Matrix1, uint32_t M, uint32_t N)
{
    /* 公式:Matrix0[I][J] = Matrix1[J][I] */
    for (uint32_t I = 0; I < M; I++)
    for (uint32_t J = 0; J < N; J++) {
         uint32_t Index1 = Matrix_ToArray(I, J, N);
         uint32_t Index2 = Matrix_ToArray(J, I, N);
         Matrix0[Index1] = Matrix1[Index2];
    }
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵(方阵)逆运算(Matrix0[N][N],Matrix1[N][N],Matrix2[N][2*N]):Matrix0 = Matrix1**(-1) */
static inline bool Matrix_Inverse(double *Matrix0, double *Matrix1, double *Matrix2, uint32_t N)
{
    if (N == 0)
        return false;
    /* 1.一阶矩阵:直接求逆 */
    if (N == 1) {
        Matrix0[Matrix_ToArray(0, 0, N)] = 1 / Matrix1[Matrix_ToArray(0, 0, N)];
        return true;
    }
    /* 2.二阶矩阵:伴随矩阵法 */
    if (N == 2) {
        double Temp = Matrix1[Matrix_ToArray(0, 0, N)] * Matrix1[Matrix_ToArray(1, 1, N)] -
                      Matrix1[Matrix_ToArray(0, 1, N)] * Matrix1[Matrix_ToArray(1, 0, N)];
        Matrix0[Matrix_ToArray(0, 0, N)] = +Matrix1[Matrix_ToArray(1, 1, N)] / Temp;
        Matrix0[Matrix_ToArray(1, 1, N)] = +Matrix1[Matrix_ToArray(0, 0, N)] / Temp;
        Matrix0[Matrix_ToArray(0, 1, N)] = -Matrix1[Matrix_ToArray(0, 1, N)] / Temp;
        Matrix0[Matrix_ToArray(1, 0, N)] = -Matrix1[Matrix_ToArray(1, 0, N)] / Temp;
        return true;
    }
    /* 3.高阶矩阵:高斯消元法(LU分解法)(不使用) */
    /* 3.高阶矩阵:全选主元高斯-约当消去法(高效) */
    if (N >= 3) {
        /* 1.生成增广矩阵:Matrix2 = {Matrix1, E} */
        for (uint32_t I = 0; I < N; I++)
        for (uint32_t J = 0; J < N; J++)
            Matrix2[Matrix_ToArray(I, J, N * 2)] =
            Matrix1[Matrix_ToArray(I, J, N)];
        for (uint32_t I = 0; I < N; I++)
        for (uint32_t J = 0; J < N; J++)
            Matrix2[Matrix_ToArray(I, J + N, N * 2)] = (I == J) ? 1 : 0;
        /* 2.全选主元:(共计N次全选主元) */
        for (uint32_t I = 0; I < N; I++) {
            /* 1.列选主元,检查精度:  Matrix_Precision */
            double   MMax = 0.0;
            uint32_t MI   = 0;
            /* 要选取绝对值最大的值,即使是负数简易变换也能归一化 */
            for (uint32_t J = I; J < N; J++) {
                double Temp = Matrix2[Matrix_ToArray(J, I, N * 2)];
                       Temp = Temp < 0 ? -Temp : Temp;
                if (MMax < Temp) {
                    MMax = Temp;
                    MI = J;
                }
            }
            /* 如果连主元都有为0的可能,那么矩阵是奇异矩阵 */
            if (MMax <= Matrix_Precision * 2)
                return false;
            /* 2.经过行变换移动到主对角线上 */
            if (MI != I) {
                /* 交换主元所在行与目标行 */
                for (uint32_t J = 0; J < N * 2; J++) {
                    double Temp1 = Matrix2[Matrix_ToArray(MI, J, N * 2)];
                    double Temp2 = Matrix2[Matrix_ToArray(I,  J, N * 2)];
                    Matrix2[Matrix_ToArray(MI, J, N * 2)] = Temp2;
                    Matrix2[Matrix_ToArray(I,  J, N * 2)] = Temp1;
                }
            }
            MMax = Matrix2[Matrix_ToArray(I, I, N * 2)];
            /* 3.主元行归一化 */
            for (uint32_t J = 0; J < N * 2; J++)
                Matrix2[Matrix_ToArray(I, J, N * 2)] /= MMax;
            /* 4.主元列归零化 */
            for (uint32_t J = 0; J < N; J++)
                if (I != J) {
                    double Temp = Matrix2[Matrix_ToArray(J, I, N * 2)];
                    for (uint32_t K = 0; K < N * 2; K++)
                        Matrix2[Matrix_ToArray(J, K, N * 2)] -=
                        Matrix2[Matrix_ToArray(I, K, N * 2)] * Temp;
                }
        }
        /* 3.拷贝逆矩阵 */
        for (uint32_t I = 0; I < N; I++)
        for (uint32_t J = 0; J < N; J++)
            Matrix0[Matrix_ToArray(I, J, N)] = 
            Matrix2[Matrix_ToArray(I, J + N, N * 2)];
        return true;
    }
    return false;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 行列式矩阵(方阵)值运算(Matrix0[N][N], Matrix1[N][N]): |Matrix0| */
static inline double Matrix_Value(double *Matrix0, double *Matrix1, uint32_t N)
{
    /* 备注:逆序数法涉及到全排列问题较复杂,暂未研究,插眼存档 */
    /* 矩阵行列式:高斯消元法(生成上三角矩阵) */
    /* 1.将其拷贝到缓冲矩阵中 */
    for (uint32_t I = 0; I < N; I++)
    for (uint32_t J = 0; J < N; J++) {
         uint32_t Index = Matrix_ToArray(I, J, N);
         Matrix1[Index] = Matrix0[Index];
    }
    /* 2.列清除,只清除当前行下面的列 */
    for (uint32_t I = 0; I < N; I++) {
        double Temp = Matrix1[Matrix_ToArray(I, I, N)];
        /* 只要遇到主元为0,终止并退出 */
        if (Matrix_Precision > (Temp < 0 ? -Temp : Temp))
            return 0.0;
        for (uint32_t J = I + 1; J < N; J++) {
                double Temp1 = Matrix1[Matrix_ToArray(J, I, N)];
                for (uint32_t K = 0; K < N; K++)
                    Matrix1[Matrix_ToArray(J, K, N)] -=
                    Matrix1[Matrix_ToArray(I, K, N)] / Temp * Temp1;
        }
    }
    /* 3.计算结果 */
    double Result = 1.0;
    for (uint32_t I = 0; I < N; I++)
        Result *= Matrix1[Matrix_ToArray(I, I, N)];
    return Result;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵Cholesky主分解流程(Matrix[N][N], Vector[N]) */
static inline bool Matrix_CholeskyDCM(double *Matrix, double *Vector, uint32_t N)
{
    /* cholesky分解矩阵反演代码,改编自
       http://jean-pierre.moreau.pagesperso-orange.fr/Cplus/choles_cpp.txt */
    
    for (uint32_t I = 0; I < N; I++)
    for (uint32_t J = I; J < N; J++) {
         double Sum = Matrix[Matrix_ToArray(I, J, N)];
         for (uint32_t K = 0; K < I; K++) {
              uint32_t Index1 = Matrix_ToArray(I, K, N);
              uint32_t Index2 = Matrix_ToArray(J, K, N);
              Sum -= Matrix[Index1] * Matrix[Index2];
         }
         if (I == J) {
             if (Sum <= Matrix_Precision)
                 return false;
             Vector[I] = sqrt(Sum);
         } else {
            Matrix[Matrix_ToArray(J, I, N)] = Sum / Vector[I];
         }
    }
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵Cholesky Decompose流程(Matrix[N][N], Matrix1[N][N]) */
static inline bool Matrix_CholeskyDC(double *Matrix0, double *Matrix1, double *Vector, uint32_t N)
{
    for (uint32_t I = 0; I < N; I++)
    for (uint32_t J = 0; J < N; J++) {
         uint32_t Index = Matrix_ToArray(I, J, N);
         Matrix0[Index] = Matrix1[Index];
    }
    if (!Matrix_CholeskyDCM(Matrix0, Vector, N))
         return false;
    for (uint32_t I = 0; I < N; I++) {
         uint32_t Index = Matrix_ToArray(I, I, N);
         Matrix0[Index] = Vector[I];
         for (uint32_t J = I + 1; J < N; J++) {
              uint32_t Index = Matrix_ToArray(I, J, N);
              Matrix0[Index] = 0;
        }
    }
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵Cholesky分解反演流程(Matrix0[N][N], Matrix1[N][N]): Matrix0 = Matrix1**(-1) */
static inline bool Matrix_CholeskyDCI(double *Matrix0, double *Matrix1, double *Vector, uint32_t N)
{
    for (uint32_t I = 0; I < N; I++)
    for (uint32_t J = 0; J < N; J++) {
         uint32_t Index = Matrix_ToArray(I, J, N);
         Matrix0[Index] = Matrix1[Index];
    }
    if (!Matrix_CholeskyDCM(Matrix0, Vector, N))
         return false;
    for (uint32_t I = 0; I < N; I++) {
         uint32_t Index = Matrix_ToArray(I, I, N);
         Matrix0[Index] = 1.0 / Vector[I];
         for (uint32_t J = I + 1; J < N; J++) {
              double Sum = 0.0;
              for (uint32_t K = I; K < J; K++) {
                   uint32_t Index1 = Matrix_ToArray(J, K, N);
                   uint32_t Index2 = Matrix_ToArray(K, I, N);
                   Sum -= Matrix0[Index1] * Matrix0[Index2];
              }
              Matrix0[Matrix_ToArray(J, I, N)] = Sum / Vector[J];
        }
    }
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵Cholesky分解反演流程(Matrix0[N][N], Matrix1[N][N]): Matrix0 = Matrix1**(-1) */
static inline bool Matrix_CholeskyInverse(double *Matrix0, double *Matrix1, double *Vector, uint32_t N)
{
    if (!Matrix_CholeskyDCI(Matrix0, Matrix1, Vector, N))
         return false;
    for (uint32_t I = 0; I < N; I++)
    for (uint32_t J = I + 1; J < N; J++)
         Matrix0[Matrix_ToArray(I, J, N)] = 0.0;
    for (uint32_t I = 0; I < N; I++) {
         uint32_t Index0  = Matrix_ToArray(I, I, N);
         Matrix0[Index0] *= Matrix0[Index0];
         for (uint32_t K = I + 1; K < N; K++) {
              uint32_t Index1  = Matrix_ToArray(K, I, N);
              Matrix0[Index0] += Matrix0[Index1] * Matrix0[Index1];
         }
         for (uint32_t J = I + 1; J < N; J++)
         for (uint32_t K = J; K < N; K++) {
              uint32_t Index0 = Matrix_ToArray(I, J, N);
              uint32_t Index1 = Matrix_ToArray(K, I, N);
              uint32_t Index2 = Matrix_ToArray(K, J, N);
              Matrix0[Index0] += Matrix0[Index1] * Matrix0[Index2];
         }
    }
    for (uint32_t I = 0; I < N; I++)
    for (uint32_t J = 0; J < I; J++) {
         uint32_t Index0 = Matrix_ToArray(I, J, N);
         uint32_t Index1 = Matrix_ToArray(J, I, N);
         Matrix0[Index0] = Matrix0[Index1];
    }
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
/* 矩阵Cholesky分解计算矩阵的行列式(Matrix0[N][N], Matrix1[N][N]) */
static inline bool Matrix_CholeskyDet(double *Matrix0, double *Matrix1, double *Vector, uint32_t N, double *Result)
{
    if (!Matrix_CholeskyDC(Matrix0, Matrix1, Vector, N))
         return false;
    
    double Number = 1.0;
    
    for (uint32_t I = 0; I < N; I++)
         Number *= Matrix0[Matrix_ToArray(I, I, N)];
    
     Number = Number * Number;
    *Result = Number;
    return true;
}
/*************************************************************************************************/
/*************************************************************************************************/
/*************************************************************************************************/
#endif
