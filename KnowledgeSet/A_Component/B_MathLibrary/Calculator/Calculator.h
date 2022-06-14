#ifndef CALCULATOR_H
#define CALCULATOR_H
//C std lib
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*实现目标:
 *一个可以解析字符串表达式
 *并完成计算结果的简要计算器
 */

/* 内部配置项 */
#ifdef  CALCULATOR_C
/* 该计算器依赖平台的动态内存策略 */
#include <stdlib.h>
#define CALCULATE_MALLOC(size)  malloc(size)
#define CALCULATE_FREE(pointer) free(pointer)
/* 伸缩因子:
 * 数字越大:空间额外开销增加,时间额外开销降低
 * 数字越小:空间额外开销降低,时间额外开销增加
 */
#define CALCULATE_LENGTH_FACTOR     10
/* 初始栈占用,情况如上类似 */
#define CALCULATE_LENGTH_BASE       100
/* 可裁剪项,紧化程序 */
#include <stdio.h>
#define ERROR_PRINT(target, str) \
    if (target) printf("%s\n",(str));

#endif

/* 解析时产生的错误信息输出重定向接口 */
typedef void (*ErrorPrint)(const char *ErrorString);
/* 表达式解析策略 */
bool CalculatorMathExpression(char *Expression, double *Result, ErrorPrint Print);

/*当前表达式可解析
 *留白默认:空格,回车,制表符
 *基本运算符支持:
 *(,) 双目运算符:+-/*,单目运算符+-,+目前默认是无效的
 *math.h函数库运算:
 *fmod(x,y),pow(x,y),fabs(x)
 *ceil(x),floor(x),sqrt(x)
 *cos(x),sin(x),tan(x)
 *cosh(x),sinh(x),tanh(x)
 *acos(x),asin(x),atan(x),atan2(x,y)
 *log(x),log10(x),exp(x)
 *部分转义:
 *pow(x,y)  ==> x**y
 *fmod(x,y) ==> x%y
 *exp(1)    ==> E,e
 *acos(-1)  ==> PAI,pai
 *部分自定义
 *阶乘:     ==> x!
 *2阶乘:    ==> x!!
 *组合      ==> c(k,n)
 *
 *当前优先级规则:()最高,前单目+-符号仅次,库函数运算居中,/*在之后,最低双目+-
 *例: log(2)*log10(2)+7-(5*3)/pai%cos(123)+atan2(0.001,234) = 9.949332
 *    暂不知算的对不对,未验证
 */

#endif
