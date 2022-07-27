#include <stdio.h>
#include "coroutine.h"

void target1_exec(void *target, void *args)
{
    int *data = args;
    for (int i = 0; i < 5; i++) {
        printf("target1_exec:%d, data:%d\n", i, *data);
        coroutine_yieldtask(target);
    }
}

void target2_exec(void *target, void *args)
{
    int *data = args;
    for (int i = 0; i < 5; i++) {
        printf("target2_exec:%d, data:%d\n", i, *data);
        coroutine_yieldtask(target);
    }
}

int main(int argc, char *argv[])
{
    int data1 = 1;
    int data2 = 2;
    void *instance = coroutine_ready(16 * 1024, NULL, -1);
    void *target1 = coroutine_commit(instance, target1_exec, &data1);
    void *target2 = coroutine_commit(instance, target2_exec, &data2);

    while (1) {
        coroutine_schedule(instance, 0);
        coroutine_schedule(instance, 0);
        /* 超时类型的挂起可以使用软硬件定时器的方式解决 */
        coroutine_waketask(target1);
        coroutine_waketask(target2);
    }
}
