/* 例: */

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "WorkStation.h"

#define ZoneAlignment   8
#define ZoneAlign       __attribute__ ((aligned (ZoneAlignment)))

void TestWork1(void)
{
    printf("work1 exec\r\n");
}

void TestWork2(void *data)
{
    printf("work2 exec: data addr:%p data:%s\r\n", data, data);
}

void TestWork3(void *data, uint32_t size)
{
    printf("work3 exec: data addr:%p data:%s\r\n", data, data);
}

void TestQueueStatic(void)
{
    /* 强制生成个平台字节对齐的空间 */
    uint64_t Zone1[1024 * 1] ZoneAlign = {0};
    /* 生成静态工作队列实体空间(它不一定要强制对齐) */
    uint8_t SQueue1[WORKSTATION_SQUEUE_SIZE] = {0};
    
    /* 当静态空间首地址为平台字节对齐且长度满足1 << n时,效率达到最大 */
    printf("static workqueue size:%u\r\n",
            WS_SQueue_Ready(SQueue1, Zone1, 1024));
    
    /* 测试时所用到的传递数据 */
    uint8_t Temp1[] = "Hello Kitty";
    uint8_t Temp2[] = "Hello DoRaEMon";
    /* 为了保证其传输高效性,导致接口不太好用,需要额外计算字节对齐占用 */
    uint32_t Temp1Length = WS_Worker_SizesCaculate(strlen(Temp1));
    uint32_t Temp2Length = WS_Worker_SizesCaculate(strlen(Temp2));
    
    printf("static workqueue load:%6u\r\n",
            WS_SQueue_WorkReady(SQueue1, WorkStation_Work1, TestWork1, NULL,  0));
    printf("static workqueue load:%6u\r\n",
            WS_SQueue_WorkReady(SQueue1, WorkStation_Work2, TestWork2, Temp1, Temp1Length));
    printf("static workqueue load:%6u\r\n",
            WS_SQueue_WorkReady(SQueue1, WorkStation_Work3, TestWork3, Temp2, Temp2Length));

    void *Work = NULL;
    printf("static workqueue load:%6u, work addr:%p\r\n",
            WS_SQueue_WorkRun(SQueue1, &Work), Work);
    printf("static workqueue load:%6u, work addr:%p\r\n",
            WS_SQueue_WorkRun(SQueue1, &Work), Work);
    printf("static workqueue load:%6u, work addr:%p\r\n",
            WS_SQueue_WorkRun(SQueue1, &Work), Work);
    
}

void TestQueueDynamic(void)
{
    /* 生成静态工作队列实体空间(它不一定要强制对齐) */
    uint8_t DQueue1[WORKSTATION_DQUEUE_SIZE] = {0};
    
    WS_DQueue_Ready(DQueue1);
    
    /* 测试时所用到的传递数据 */
    uint8_t Temp1[] = "Hello Kitty";
    uint8_t Temp2[] = "Hello DoRaEMon";
    /* 为了保证其传输高效性,导致接口不太好用,需要额外计算字节对齐占用 */
    uint32_t Temp1Length = WS_Worker_SizesCaculate(strlen(Temp1));
    uint32_t Temp2Length = WS_Worker_SizesCaculate(strlen(Temp2));
    
    printf("dynamic workqueue load:%6u\r\n",
            WS_DQueue_WorkReady(DQueue1, WorkStation_Work1, TestWork1, NULL,  0));
    printf("dynamic workqueue load:%6u\r\n",
            WS_DQueue_WorkReady(DQueue1, WorkStation_Work2, TestWork2, Temp1, Temp1Length));
    printf("dynamic workqueue load:%6u\r\n",
            WS_DQueue_WorkReady(DQueue1, WorkStation_Work3, TestWork3, Temp2, Temp2Length));

    void *Work = NULL;
    printf("dynamic workqueue load:%6u, work addr:%p\r\n",
            WS_DQueue_WorkRun(DQueue1, &Work), Work);
    printf("dynamic workqueue load:%6u, work addr:%p\r\n",
            WS_DQueue_WorkRun(DQueue1, &Work), Work);
    printf("dynamic workqueue load:%6u, work addr:%p\r\n",
            WS_DQueue_WorkRun(DQueue1, &Work), Work);
    
}

int main(int argc, char *argv[]) {
    
    /* 测试当前平台的实际空间占用和预估空间占用情况: */
    /* 测试之后,将WORKQUEUQ_xxx_SIZE替换为对应的WQ_xQUEUE_BytesGet()返回值 */
    printf("basic work zone:%u\r\n",
            WS_Worker_BytesGet());
    printf("static workqueue current:%u, evaluate:%u\r\n",
            WS_SQueue_BytesGet(), WORKSTATION_SQUEUE_SIZE);
    printf("dynamic workqueue current:%u, evaluate:%u\r\n",
            WS_DQueue_BytesGet(), WORKSTATION_DQUEUE_SIZE);
    /* 获得测试函数地址 */
    printf("TestWork1:addr:%p\r\n", TestWork1);
    printf("TestWork2:addr:%p\r\n", TestWork2);
    printf("TestWork3:addr:%p\r\n", TestWork3);
    
    printf("------------------------------------------------------------\r\n");
    TestQueueStatic();
    printf("------------------------------------------------------------\r\n");
    TestQueueDynamic();
    printf("------------------------------------------------------------\r\n");
}







