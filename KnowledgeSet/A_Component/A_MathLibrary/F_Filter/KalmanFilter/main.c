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
    
    uint32_t I = 0;
    const uint32_t TestKalmanOrder1Length = 20;
    
    while (I < TestKalmanOrder1Length) {
        //生成输入并输出
        printf("\r\n--------------------------------\r\n");
        Data = TestKalmanOrderSwin();
        printf("%5f\t", Data);
        //生成输出并输出
        Data = KalmanOrder1_Run(&Filter, Data);
        printf("%5f\t", Data);
        
        I++;
    }
        printf("\r\n--------------------------------\r\n");
}

/* 测试不通过,忽略了一个问题,矩阵不一定有逆 */
void TestKalmanOrderx(void)
{
    KalmanOrderx Filter;
    uint32_t Order = KalmanOrderx_Init(&Filter);
    
    double *Data0 = malloc(Order);
    double *Data1 = malloc(Order);
    
    uint32_t I = 0;
    const uint32_t TestKalmanOrderxLength = 20;
    
    while (I < TestKalmanOrderxLength) {
        //生成输入
        for (uint32_t J = 0; J < Order; J++)
            Data0[J] = Data1[J] = TestKalmanOrderSwin();
        //生成输入并输出
        bool Result = KalmanOrderx_Run(&Filter, Data1);
        if (!Result)
             continue;
        printf("\r\n--------------------------------\r\n");
        for (uint32_t J = 0; J < Order; J++)
            printf("%5f\t", Data0[J]);
        printf("\t\t\t");
        for (uint32_t J = 0; J < Order; J++)
            printf("%5f\t", Data1[J]);
        
        I++;
    }
        printf("\r\n--------------------------------\r\n");
}

int main(int argc, char *argv[]) {
    
    TestKalmanOrder1();
    TestKalmanOrderx();
}
