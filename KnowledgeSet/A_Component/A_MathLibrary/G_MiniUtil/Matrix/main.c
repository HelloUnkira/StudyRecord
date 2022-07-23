
#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"

int main(int argc, char *argv[]) {
    
    uint32_t N = 5;
    double Matrix0[5][5] = {0};
    double Matrix1[5][5] = {0};
    double Matrix2[5][10] = {0};
    double Matrix3[5][5] = {0};
    
    for (uint32_t I = 0; I < N; I++)
    for (uint32_t J = 0; J < N; J++)
        Matrix1[I][J] = rand() % 100;
    printf("\n------------------------------------------------------------\n");
    bool Result = Matrix_Inverse(Matrix0, Matrix1, Matrix2, N);
    printf("Matrix_Inverse:%d", Result);
    printf("\n------------------------------------------------------------\n");
    printf("Matrix1:\n");
    for (uint32_t I = 0; I < N; I++) {
    for (uint32_t J = 0; J < N; J++)
        printf("%lf\t", Matrix1[I][J]);
        printf("\n");
    }
    printf("\n------------------------------------------------------------\n");
    printf("Matrix2:\n");
    for (uint32_t I = 0; I < N; I++) {
    for (uint32_t J = 0; J < N; J++)
        printf("%lf\t", Matrix2[I][J]);
        printf("\n");
    }
    printf("\n------------------------------------------------------------\n");
    printf("Matrix2:\n");
    for (uint32_t I = 0; I < N; I++) {
    for (uint32_t J = N; J < N * 2; J++)
        printf("%lf\t", Matrix2[I][J]);
        printf("\n");
    }
    printf("\n------------------------------------------------------------\n");
    printf("Matrix0:\n");
    for (uint32_t I = 0; I < N; I++) {
    for (uint32_t J = 0; J < N; J++)
        printf("%lf\t", Matrix0[I][J]);
        printf("\n");
    }
    printf("\n------------------------------------------------------------\n");
    Matrix_Star(Matrix3, Matrix1, Matrix0, N, N, N);
    printf("Matrix_Star:");
    printf("\n------------------------------------------------------------\n");
    printf("Matrix3:\n");
    for (uint32_t I = 0; I < N; I++) {
    for (uint32_t J = 0; J < N; J++)
        printf("%lf\t", Matrix3[I][J]);
        printf("\n");
    }
    printf("\n------------------------------------------------------------\n");
    
    printf("\n------------------------------------------------------------\n");
    double Result1 = Matrix_Value(Matrix1, Matrix3, N);
    printf("Matrix_Value:%lf", Result1);
    printf("\n------------------------------------------------------------\n");
    printf("Matrix3:\n");
    for (uint32_t I = 0; I < N; I++) {
    for (uint32_t J = 0; J < N; J++)
        printf("%lf\t", Matrix3[I][J]);
        printf("\n");
    }
    printf("\n------------------------------------------------------------\n");
    
    
    return 0;
}
