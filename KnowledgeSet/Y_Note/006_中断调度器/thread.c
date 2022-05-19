/*
 *尝试使用中断实现抢占式多线程
 */

/* 包含芯片对应的头文件*/
/*#include<>*/

/* 线程结构体，实现具体线程所用*/
struct thread{
	/* 指向下一个优先级比本线程低的线程*/
	thread* next;
	/* 线程函数指针*/
	void (*real_thread)(void);
};

/* 说明，常规的thread编写与此编写的不同之处*/
/* 常规thread编写如下*/
void thread_normal(void)
{
	/* 线程初始化，各个步骤*/
	/* 这些数据一直存在且只会运行一次，所以可以用初始化函数代替*/
	while (1) {
		/* 线程开始执行*/
	
	
		/* 线程在此处挂起，下次运行时从此处继续*/
		/* 所以所有的数据都一直未被释放，可以使用全局变量代替*/
		/* 亦可以在thread结构体添加，形成定向的thread*/
	}
	
}


/* 线程所需的数据结构*/
/* 注TIMEx_MAX指的是，中断多少次后才执行*/
/* 这里则等价于线程睡眠多久后开始执行，睡眠时间等于*/
/* 单次中断触发时间 * TIMEx_MAX*/
#define TIME1_MAX 100
int time1 = 0;
void thread1_init()
{
}
void thread1(void)
{
	if (time1 <= TIME1_MAX)
		time1++;
	else {
	
	/* 用户执行的操作*/
	return;	
	}

}

#define TIME2_MAX 100
int time2 = 0;
void thread2_init()
{
}
void thread2(void)
{
	if (time2 <= TIME2_MAX)
		time2++;
	else {
	
	/* 用户执行的操作*/
	return;	
	}
	
}

/* 创建一个空的起始线程，多线程调度入口在此*/
struct thread thread_zero;
thread_zero.real_thread = NULL;
	
/* 创建线程1*/
struct thread thread_one;
thread_one.real_thread = thread1;
	
/* 创建线程2*/
struct thread thread_two;
thread_two.real_thread = thread2;
	
/* 线程连接*/
thread_zero.next = thread_one;
thread_one.next  = thread_two;
thread_two.next  = NULL;
	
/* 用户线程间的数据交互，与必须保存的数据进度，使用全局变量定义如下*/


int main()
{
	thread1_init();
	thread2_init();

	/* 初始化需要使用的时钟中断*/
	/* 推荐中断产生实际在几百微秒到几毫秒间*/
	/* 但最好保证它是各个线程的约数*/

	while (1);
}

/* 线程中断服务程序，根据实际格式编写*/
void thread_interrupt(void) 
{
	thread* thread_buf = thread_zero;
	while((thread_buf = thread_buf->next) != NULL)
		if(thread_buf->real_thread != NULL)
			thread_buf->real_thread();
}









