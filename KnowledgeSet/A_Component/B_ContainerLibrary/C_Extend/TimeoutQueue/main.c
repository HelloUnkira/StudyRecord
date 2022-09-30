#include <stdio.h>
#include <windows.h>
#include "TimeroutQueue.h"

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

void TQ_NodeEventThrowCallback(TQ_Node *Node)
{
     TQ_Node_Execute(Node);
}

int main(int argc, char *argv[]) {
    
    TQ_Queue Queue;
    
    TQ_Queue_Configure(&Queue, TQ_NodeEventThrowCallback);
    
    uint32_t Data1 = 0;
    uint32_t Data2 = 0;
    uint32_t Data3 = 0;
    
    TQ_Node Timer1 = TQ_Node_CONFIGURE(Timer1Callback, &Data1, 1, true);
    TQ_Node Timer2 = TQ_Node_CONFIGURE(Timer2Callback, &Data2, 2, true);
    TQ_Node Timer3 = TQ_Node_CONFIGURE(Timer3Callback, &Data3, 3, true);
    
    TQ_Node_Start(&Queue, &Timer1);
    TQ_Node_Start(&Queue, &Timer2);
    TQ_Node_Start(&Queue, &Timer3);
    
    while (1) {
        Sleep(10);
        
        TQ_Node *Node = NULL;
        
        TQ_Node_Reduce(&Queue);
        
        Data1++;
        Data2++;
        Data3++;
    }
}
