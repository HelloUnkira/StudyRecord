#ifndef MATH_LIBRARY_H
#define MATH_LIBRARY_H

///////////////////////////////////////////////////////////////////////////////
//简单项://////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//x!
unsigned int calculate_factorial(unsigned int x);
//x!!
unsigned int calculate_double_factorial(unsigned int x);
//c(k,n) (k为上标,n为下标)
unsigned int calculate_combination(unsigned int k, unsigned int n);
//x**n
double calculate_x_n(double x, unsigned int n);

///////////////////////////////////////////////////////////////////////////////
//扩展项:计算定积分////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//f_x 被积函数 left 积分下限 right 积分上限 precision 精度 flag 积分模式1,2////
///////////////////////////////////////////////////////////////////////////////
double calculate_definite_integration(double (*f_x)(double x),
                                      double left,
                                      double right,
                                      unsigned int precision,
                                      unsigned int flag);

///////////////////////////////////////////////////////////////////////////////
//扩展项:方程求解近似值////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//计算多项式
///////////////////////////////////////////////////////////////////////////////
//exponent 最高次幂 level 导数阶次(为0不求导) parameter 参数(a0->an) x 指定点//
///////////////////////////////////////////////////////////////////////////////
double calculate_multinomial(unsigned int exponent,
                             unsigned int level,
                             double *parameter,
                             double  x);
//牛顿切线
///////////////////////////////////////////////////////////////////////////////
//f_0_x 0阶导函数 f_1_x 1阶导函数 start 起始点 precision 精度//////////////////
///////////////////////////////////////////////////////////////////////////////
double calculate_newton_tangent(double (*f_0_x)(double x0),
                                double (*f_1_x)(double x1),
                                double start,
                                double precision);

///////////////////////////////////////////////////////////////////////////////
//扩展项:泰勒公式展开:(精度precision)//////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//e**x
double calculate_e_x(double x, unsigned int precision);
//ln(x)
double calculate_ln(double x, double precision);
//sin(x)
double calculate_sin(double x, double precision);
//cos(x)
double calculate_cos(double x, double precision);
//arctan(x) --- x(-1.0,1.0)
double calculate_arctan(double x, double precision);

#endif

/*
*例:
#include <stdio.h>
#include <stdlib.h>

#define MATH_LIBRARY_H_SIMPLE_FUNCTION_TEST
#define MATH_LIBRARY_H_DEFINITE_INTEGRATION
#define MATH_LIBRARY_H_NETTON_TANGENT
#define MATH_LIBRARY_H_FFT_AND_IFFT_TEST
#include "math_library.h"

int main(int argc, char* argv[]) {
    test_simple_function();
    test_definite_integration();
    test_newton_tangent();
    test_fft_and_ifft();
    return 0;
}
 */
 
//仅测试所用,工程中只可低层级定义一次,不放入.c是不希望它被污染
//包括下述头文件在内的部分都可被去除,待稳定后
#include <stdio.h>
#ifdef MATH_LIBRARY_H_SIMPLE_FUNCTION_TEST

static void test_simple_function(void)
{
    unsigned int  i = 0, j = 0;
/*
    for (i = 0; i < 10; i++)//测试x!
        printf("\n%d!  = %d",i,calculate_factorial(i));
    for (i = 0; i < 10; i++)//测试x!!
        printf("\n%d!! = %d",i,calculate_double_factorial(i));
    for (i = 5; i < 10; i++)//测试C(k,n)
    for (j = i + 1; j < 10; j++)
        printf("\nc(%d,%d) = %d", i, j, calculate_combination(i, j));
    for (i = 7; i < 10; i++)//测试x**y
    for (j = i + 1; j < 10; j++)
        printf("\n%d**%d = %lf", i, j, calculate_x_n(i, j));
    for (i = 0; i < 10; i++)//测试e**x
        //注意:x越大收敛越慢,越有误
        printf("\ne**%d = %lf", i, calculate_e_x((double)i, 10));
    for (i = 0; i < 15; i++) {//测试ln(x)
        double t = (double)i * 1.1;
        printf("\nln(%lf)=%lf",t, calculate_ln(t, 0.0000001));
    }
    for (i = 0; i < 10; i++) {//测试sin,cos,arctan
        double t = (double)i * 1.1;
        //精度越高,收敛越慢,越容易死锁,甚至溢出!
        printf("\nsin(%lf) = %lf"
               "\ncos(%lf) = %lf"
               "\narctan(%lf) = %lf", 
               t, calculate_sin(t, 0.00001),
               t, calculate_cos(t, 0.00001),
               t /10, calculate_arctan(t / 10, 0.00001));
               //受限的arctan:t=[-1,1];
    }

*/
}

#endif

#ifdef MATH_LIBRARY_H_DEFINITE_INTEGRATION

static double f_x_t(double x)
{
    return (1 / (1 + x * x));
}

static void test_definite_integration(void)
{
    printf("\n%lf",calculate_definite_integration(f_x_t, 0.0, 1.0, 10, 1));
    printf("\n%lf",calculate_definite_integration(f_x_t, 0.0, 1.0, 10, 2));
}   

#endif

#ifdef MATH_LIBRARY_H_NETTON_TANGENT

//方程-7 - 4x - 2x^2 + x^3 = 0    //它的近似值从4.0开始取
double temp_1[4] = {-7.0,-4.0,-2.0,1.0};

static double calculate_multinomial_0_value(double x)
{   //求多项式的指定点x的0阶导数的值
    return calculate_multinomial(3, 0, temp_1, x);
}

static double calculate_multinomial_1_value(double x)
{   //求多项式的指定点x的1阶导数的值
    return calculate_multinomial(3, 1, temp_1, x);
}

static void test_newton_tangent(void)
{
    //方程: -7 - 4x - 2x^2 + x^3 = 0
    printf("\n%lf",calculate_multinomial(3, 0, temp_1, 2));
    printf("\n%lf",calculate_multinomial(3, 1, temp_1, 2));
    printf("\n%lf",calculate_multinomial(3, 2, temp_1, 2));
    
    printf("\n%lf",calculate_newton_tangent(
                   calculate_multinomial_0_value,
                   calculate_multinomial_1_value,
                   4.0, 0.0001)); //起点为4,预估出来的
}

#endif
