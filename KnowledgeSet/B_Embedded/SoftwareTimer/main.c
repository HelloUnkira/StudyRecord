#include <stdio.h>
#include <windows.h>
#include "SoftwareTimer.h"

void Timer1Callback(void *Parameter)
{
	static uint32_t Count1 = 0;
    if (++Count1 % 100 == 0)
        printf("Timer1Callback: %d\t%d\n", Count1, *((uint32_t *)Parameter));
}

void Timer2Callback(void *Parameter)
{
    static uint32_t Count2 = 0;
    if (++Count2 % 100 == 0)
        printf("Timer2Callback: %d\t%d\n", Count2, *((uint32_t *)Parameter));
}

void Timer3Callback(void *Parameter)
{
    static uint32_t Count3 = 0;
    if (++Count3 % 100 == 0)
        printf("Timer3Callback: %d\t%d\n", Count3, *((uint32_t *)Parameter));
}

void STimerEventThrowCallback(STimer *Timer)
{
     STimer_Execute(Timer);
}

int main(int argc, char *argv[]) {
    
    STimerQueue Queue;
    
    STimerQueue_Configure(&Queue, STimerEventThrowCallback);
    
    uint32_t Data1 = 0;
    uint32_t Data2 = 0;
    uint32_t Data3 = 0;
    
    STimer Timer1 = STimer_CONFIGURE(Timer1Callback, &Data1, 1, true);
    STimer Timer2 = STimer_CONFIGURE(Timer2Callback, &Data2, 2, true);
    STimer Timer3 = STimer_CONFIGURE(Timer3Callback, &Data3, 3, true);
    
    STimer_Start(&Queue, &Timer1);
    STimer_Start(&Queue, &Timer2);
    STimer_Start(&Queue, &Timer3);
    
    while (1) {
        Sleep(10);
        
        STimer *Timer = NULL;
        
        STimer_Reduce(&Queue);
        
        Data1++;
        Data2++;
        Data3++;
    }
}
