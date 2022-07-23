#include <stdio.h>
#include "FastFourierTransform.h"

int main(int argc, char *argv[]) {
    
    int length = 16;
    int array1[16] = {0};
    int array2[16] = {0};
    int k = 0,i = 0;
    double real_list[16] = {
         2.2,  4.5,  6.7,  8.5,
        10.2, 12.3, 14.5, 16.2,
        19.3, 21.2, 25.2, 29.4,
        36.4, 39.2, 45.2, 50.1};
    double imag_list[16] = {0};
    
    printf("\n:-----------------------------------------------------------\n");
    printf("\nREAL:\n");
    for (k = 0; k < length; k += 4) {
        for (i = k; i < k + 4; i++)
            printf("%lf\t",real_list[i]);
        printf("\n");
    }
    printf("\nIMAGINARY:\n");
    for (k = 0; k < length; k += 4) {
        for (i = k; i < k + 4; i++)
            printf("%lf\t",imag_list[i]);    
        printf("\n");
    }

    //result:
    //real_list = {
    //     341.1,   9.7, -23.3, -24.7,
    //     -23.5, -26.8, -26.9, -26.7,
    //     -21.7, -26.7, -26.9, -26.8,
    //     -23.5, -24.7, -23.3,   9.7}; 
    //imag_list = {
    //      0.0, -124.5, -66.1, -37.0,
    //    -27.0,  -19.9, -10.5, -2.5,
    //      0.0,    2.5,  10.5, 19.9,
    //     27.0,   37.0,  66.1, 124.5};
        

    //1.启用快速傅里叶变换
    FFT(real_list, imag_list, length, array1);
    printf("\nFFT:--------------------------------------------------------\n");
    for (k = 0; k < length; k++)
        printf("%d ",array1[k]);
    printf("\nREAL:\n");
    for (k = 0; k < length; k += 4) {
        for (i = k; i < k + 4; i++)
            printf("%lf\t",real_list[i]);
        printf("\n");
    }
    printf("\nIMAGINARY:\n");
    for (k = 0; k < length; k += 4) {
        for (i = k; i < k + 4; i++)
            printf("%lf\t",imag_list[i]);
        printf("\n");
    }

    //2.启用快速傅里叶逆变换
    IFFT(real_list, imag_list, length, array2);
    printf("\nIFFT:-------------------------------------------------------\n");
    for (k = 0; k < length; k++)
        printf("%d ",array2[k]);
    printf("\nREAL:\n");
    for (k = 0; k < length; k += 4) {
        for (i = k; i < k + 4; i++)
            printf("%lf\t",real_list[i]);
        printf("\n");
    }
    printf("\nIMAGINARY:\n");
    for (k = 0; k < length; k += 4) {
        for (i = k; i < k + 4; i++)
            printf("%lf\t",imag_list[i]);    
        printf("\n");
    }
}
