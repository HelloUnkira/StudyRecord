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

#define TestKalmanOrder1Length  20

    uint32_t Index = 0;
    double Input[TestKalmanOrder1Length]  = {0};
    double Output[TestKalmanOrder1Length] = {0};
    
    //生成输入并输出
    printf("\r\n--------------------------------\r\n");
    printf("Input:\r\n");
    for (Index = 0; Index < TestKalmanOrder1Length; Index++) {
        Input[Index] = TestKalmanOrderSwin();
        printf("%5f\t", Input[Index]);
    }
    //生成输出并输出
    printf("\r\n--------------------------------\r\n");
    printf("Output:\r\n");
    for (Index = 0; Index < TestKalmanOrder1Length; Index++) {
        Output[Index] = KalmanOrder1_Run(&Filter, Input[Index]);
        printf("%5f\t", Output[Index]);
    }
    printf("\r\n--------------------------------\r\n");
}



int main(int argc, char *argv[]) {
    
    TestKalmanOrder1();
    
    
    
}
