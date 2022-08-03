/*
 * RTOS全局配置
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

//头文件

//启用抢占
#define 	configUSE_PREEMPTION	1

//任务优化选项
#define 	configUSE_PORT_OPTIMISED_TASK_SELECTION 1	

//CPU时钟
#define 	configCPU_CLOCK_HZ		48000000UL

//时钟滴答步速
#define 	configTICK_RATE_HZ		1000

//最大优先级
#define 	configMAX_PRIORITIES	32
//最小堆栈尺寸
#define 	configMINIMAL_STACK_SIZE (256)

//堆大小
#define 	configTOTAL_HEAP_SIZE	(100 * 1024)
//最大任务名字
#define 	configMAX_TASK_NAME_LEN 32
//不使用16位滴答
#define 	configUSE_16_BIT_TICKS	0
//空闲调度
#define 	configIDLE_SHOULD_YIELD 0

//配置互斥锁
#define 	configUSE_MUTEXES		1
//配置嵌套互斥锁
#define 	configUSE_RECURSIVE_MUTEXES 1
//配置计数信号量
#define 	configUSE_COUNTING_SEMAPHORES 1
//不使用兼容API
#define 	configUSE_ALTERNATIVE_API 0
//队列注册大小
#define 	configQUEUE_REGISTRY_SIZE 8
//使用队列集合
#define 	configUSE_QUEUE_SETS	1
//不使用时间片轮转
#define 	configUSE_TIME_SLICING	0
//不使用NewLib
#define 	configUSE_NEWLIB_REENTRANT 0
//不使用兼容
#define 	configENABLE_BACKWARD_COMPATIBILITY 0

//使用IDLE钩子
#define 	configUSE_IDLE_HOOK 	1
//不使用滴答钩子
#define 	configUSE_TICK_HOOK 	0
//堆栈溢出检查
#define 	configCHECK_FOR_STACK_OVERFLOW 2
//使用动态分配失败回调钩子
#define 	configUSE_MALLOC_FAILED_HOOK 1

//使用运行集
#if	USE_FREERTOS_RUN_STATS
#define 	configGENERATE_RUN_TIME_STATS 1
#else
#define 	configGENERATE_RUN_TIME_STATS 0
#endif

//FreeRTOS提供的trace,需要手动提供实现
#define 	configUSE_TRACE_FACILITY 1
//格式化功能使用
#define 	configUSE_STATS_FORMATTING_FUNCTIONS 1

//使用软件定时器
#define 	configUSE_TIMERS		1
//定时器优先级
#define 	configTIMER_TASK_PRIORITY configTIMER_TASK_PRI
//定时器队列长度
#define 	configTIMER_QUEUE_LENGTH 30
//定时器任务堆栈深度
#define 	configTIMER_TASK_STACK_DEPTH 1024

//内核中断优先级
#define 	configKERNEL_INTERRUPT_PRIORITY (0x7 << 5)
//最大系统服务中断优先级
#define 	configMAX_SYSCALL_INTERRUPT_PRIORITY (0x4 << 5)
//配置内核中断优先级和系统服务中断优先级
#define NVIC_configKERNEL_INTERRUPT_PRIORITY        (0x7)
#define NVIC_configMAX_SYSCALL_INTERRUPT_PRIORITY   (0x4)

//系统断言检查
#define 	configASSERT(x) sys_assert_check(x,(uint8_t *)__FILE__,(uint16_t)__LINE__)

//不使用应用定义的特权函数
#define 	configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0

//确认任务的功能裁剪
#define 	INCLUDE_vTaskPrioritySet 0
#define 	INCLUDE_uxTaskPriorityGet 0
#define 	INCLUDE_vTaskDelete 	0
#define 	INCLUDE_vTaskSuspend	1
#define 	INCLUDE_xResumeFromISR	0
#define 	INCLUDE_vTaskDelayUntil 1

#define 	INCLUDE_vTaskDelay		1
#define 	INCLUDE_xTaskGetSchedulerState 1
#define 	INCLUDE_xTaskGetCurrentTaskHandle 0
#define 	INCLUDE_uxTaskGetStackHighWaterMark 0
#define 	INCLUDE_xTaskGetIdleTaskHandle 1
#define 	INCLUDE_xTimerGetTimerDaemonTaskHandle 0
#define 	INCLUDE_pcTaskGetTaskName 0
#define 	INCLUDE_eTaskGetState	0
#define 	INCLUDE_xEventGroupSetBitFromISR 1
#define 	INCLUDE_xTimerPendFunctionCall 1

//三个内核使用到的中断钩子
#define vPortSVCHandler                         SVC_Handler
#define xPortPendSVHandler                      PendSV_Handler
#define xPortSysTickHandler                     SysTick_Handler

//重写默认滴答配置
#define configOVERRIDE_DEFAULT_TICK_CONFIGURATION 1 
#define configUSE_TICKLESS_IDLE                   2

#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) 
	extern uint32_t am_freertos_sleep(uint32_t);
	extern void am_freertos_wakeup(uint32_t);

#define 	configPRE_SLEEP_PROCESSING(time)							\
			do {														\
			(time) = am_freertos_sleep(time);							\
			} while (0);

#define 	configPOST_SLEEP_PROCESSING(time) am_freertos_wakeup(time)
#endif

//平台的一些配置
#ifndef AM_PART_APOLLO
#define 	AM_FREERTOS_USE_STIMER_FOR_TICK
#endif

#ifdef AM_FREERTOS_USE_STIMER_FOR_TICK
#define configSTIMER_CLOCK_HZ                     32768
#define configSTIMER_CLOCK                        AM_HAL_STIMER_XTAL_32KHZ
#else //使用C定时器
#define configCTIMER_NUM                          3
#define configCTIMER_CLOCK_HZ                     32768
#define configCTIMER_CLOCK                        AM_HAL_CTIMER_XT_32_768KHZ
#endif

#ifdef __cplusplus
}

#endif

#endif

