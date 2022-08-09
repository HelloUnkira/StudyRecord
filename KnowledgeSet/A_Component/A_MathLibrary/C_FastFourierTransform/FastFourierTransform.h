#ifndef FAST_FOURIER_TRANSFORM_H
#define FAST_FOURIER_TRANSFORM_H
//C std lib
#include <stdint.h>
#include <stdbool.h>
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
//Real 复数集实数部数组
//Imag 复数集虚数部数组
//Len  复数集元素个数
//Rev  长度len的辅助数组
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void FFT(double *Real, double *Imag, uint32_t Len, uint32_t *Rev);
void IFFT(double *Real, double *Imag, uint32_t Len, uint32_t *Rev);
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
#endif