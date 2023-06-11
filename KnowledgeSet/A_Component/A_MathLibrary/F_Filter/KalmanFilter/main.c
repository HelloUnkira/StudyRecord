/* 例: */

#include <stdio.h>
#include <windows.h>
#include "FilterKalman.h"

static inline double TestKalmanOrderSwin(void)
{
    #define TestKalmanOrderBase     100
    return (double)(TestKalmanOrderBase + rand() % 10);
}

void TestKalmanOrder1(void)
{
    KalmanOrder1 Filter;
    KalmanOrder1_Init(&Filter, 0, 0.1, 0, 1, 1, 0.1, 0.05, 0.1);
    
    double Data = 0;
    
    #define TestKalmanOrder1Length  20
    for (uint32_t I = 0; I < TestKalmanOrder1Length; I++) {
    //生成输入并输出
    printf("\r\n--------------------------------\r\n");
    Data = TestKalmanOrderSwin();
    printf("%5f\t", Data);
    //生成输出并输出
    Data = KalmanOrder1_Run(&Filter, Data);
    printf("%5f\t", Data);
    }
    printf("\r\n--------------------------------\r\n");
}

/* 测试不通过,忽略了一个问题,矩阵不一定有逆 */
void TestKalmanOrderx(void)
{
    KalmanOrderx Filter;
    uint32_t Order = KalmanOrderx_Init(&Filter);
    
    double *Data = malloc(Order);
    
    #define TestKalmanOrderxLength  20
    for (uint32_t I = 0; I < TestKalmanOrderxLength; I++) {
         printf("\r\n--------------------------------\r\n");
        //生成输入并输出
        for (uint32_t J = 0; J < Order; J++) {
            Data[J] = TestKalmanOrderSwin();
            printf("%5f\t", Data[J]);
        }
        bool Result = KalmanOrderx_Run(&Filter, Data);
        if (!Result)
             continue;
        printf("\t\t\t");
        for (uint32_t J = 0; J < Order; J++)
            printf("%5f\t", Data[J]);
    }
        printf("\r\n--------------------------------\r\n");
}

int main(int argc, char *argv[]) {
    
    // TestKalmanOrder1();
    TestKalmanOrderx();
}
