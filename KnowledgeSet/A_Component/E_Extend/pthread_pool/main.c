#include <unistd.h>
#include <stdio.h>
#include "pthread_pool.h"

void test_thread1(void *arg)
{
    for (int i = 0; i < 10; i++) {
        printf("test_thread1 is execute: arg: %d\n", *((int *)arg));
        sleep(1);
    }
}

void test_thread2(void *arg)
{
    for (int i = 0; i < 10; i++) {
        printf("test_thread2 is execute: arg: %p\n", arg);
        sleep(1);
    }
}

void test_thread3(void *arg)
{
    for (int i = 0; i < 5; i++) {
        printf("test_thread3 is execute: arg: %d\n", *((int *)arg));
        sleep(1);
    }
}

void test_thread4(void *arg)
{
    for (int i = 0; i < 5; i++) {
        printf("test_thread4 is execute: arg: %p\n", arg);
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    int args1 = 1;
    int args2 = 2;
    
    printf("\n\ntest thread pool1 start\n\n");
    void *instance1 = pthread_pool_create(2);
    pthread_pool_commit(instance1, test_thread1, &args1);
    pthread_pool_commit(instance1, test_thread2, &args1);
    pthread_pool_commit(instance1, test_thread3, &args2);
    pthread_pool_commit(instance1, test_thread4, &args2);
    pthread_pool_destroy(instance1);
    printf("\n\ntest thread pool1 end\n\n");
    
    printf("\n\ntest thread pool2 start\n\n");
    void *instance2 = pthread_pool_create(2);
    pthread_pool_commit(instance2, test_thread3, &args2);
    pthread_pool_commit(instance2, test_thread4, &args2);
    pthread_pool_commit(instance2, test_thread1, &args1);
    pthread_pool_commit(instance2, test_thread2, &args1);
    pthread_pool_destroy(instance2);
    printf("\n\ntest thread pool2 end\n\n");

    printf("\n\ntest thread pool3 start\n\n");
    void *instance3 = pthread_pool_create(4);
    pthread_pool_commit(instance3, test_thread1, &args1);
    pthread_pool_commit(instance3, test_thread2, &args1);
    pthread_pool_commit(instance3, test_thread3, &args2);
    pthread_pool_commit(instance3, test_thread4, &args2);
    pthread_pool_destroy(instance3);
    printf("\n\ntest thread pool3 end\n\n");
}






