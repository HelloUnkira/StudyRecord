#ifndef SYNCHRONIZATION_PRIMITIVE
#define SYNCHRONIZATION_PRIMITIVE

//同步原语总结
//通用简化的同步原语
//一般可以退化为对队列的阻塞

typedef struct synchronization_primitive
{
	//同步原语核心是需要一个队列
	macro_iterator * queue;
	//根据不同的使用情况可以使同步原语设计多种机制
	//无需机制(条件变量)
	//二值机制(互斥锁,二值信号量等)
	//多值机制
	int value;
} s_p;

//同步的前提是启用临界区
//当一个线程需要访问对应实例保护的临界区时
//它需要在临界区内查看是否存在它的机会

//获取同步
void* take_synchronization_primitive(s_p * instance)
{
	//进入临界区
	critical_entry();
	
	//访问同步机制
	if (!satisfy_synchronization_condition()) {
		//如果不达到同步条件
		//当前线程必须阻塞到当前点位
		//它不可能会主动的继续下去
		//而只会被其他线程唤醒
		add_itself_to_synchronization_queue();
		
		//此处启用调度,则线程在该调用的中间被阻塞
		//CPU现场在该函数之中
		//此时的上下文切换与线程切换,该线程相当于"停在此处"
		yield();
	}
	
	//那么不管如何,是被其他线程唤醒还是直接满足同步条件
	//现在需要根据同步机制去迫使其他线程"不可能再达到同步条件"
	clear_synchronization_condition();
	//如此一来,目前只有自己本身单独占用该资源并使用
	//其他妄图访问被该同步原语保护的资源时,被该机制被迫阻塞
	
	//退出临界区
	critical_exit();
}

//释放同步
void* give_synchronization_primitive(s_p * instance)
{
	//进入临界区
	critical_entry();
	
	//访问同步机制
	if (satisfy_synchronization_condition()) {
		//如果达到同步条件
		//此时我已经使用完资源了,可以让别的朋友去使用了
		set_synchronization_condition();
		//将一个最适合的线程从同步原语队列中移除阻塞
		remove_other_from_synchronization_queue();
		//现在我可以让它去执行了,我释放的它,它获得了执行权
		
		//此处启用调度,则线程在该调用的中间被阻塞
		//CPU现场在该函数之中
		//此时的上下文切换与线程切换,该线程相当于"停在此处"		
		yield();
	}
	
	//那么不管如何,程序执行到这里之后我已经释放完资源了
	//当然如果根本没有获得同步,就没有资格获取同步
	
	//退出临界区
	critical_exit();
}

//相对较为特殊一点的是CPU内部的同步原语
//称之为 每CPU变量
//通常 每CPU变量同步原语 适用于为俩个线程资源间交互所用
//A线程在CPUa下运行,需要向B线程联动,交换资源
//此时 处理器间中断产生 内核会让CPUb下的B线程执行
//它们会在内核的缓存中交换资源,此时AB线程同步执行
//或者B如果不满足执行条件,内核会让CPUa下的A线程阻塞睡眠
//等到未来在CPUb下的B线程达到执行条件时
//B线程需要和A线程联动,交换资源时,由CPUb产生 处理器间中断
//内核会继续检查AB的可执行权限判定是否继续阻塞或者让CPUa执行A线程

#endif
