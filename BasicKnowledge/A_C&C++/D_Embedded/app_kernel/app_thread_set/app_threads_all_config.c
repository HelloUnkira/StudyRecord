/*
 *实现目的:
 *    静态配置的信号量与互斥锁,因为要通配各种系统的明细
 */

#include "app_threads_all_config.h"

//所有的线程都可以被一个通化容器承载
//通常没有比数组更好的选择了,因为嵌入式平台的线程都是定制好的
static thread_body_t thread_owner;
static thread_body_t thread_set[APP_THREAD_MAX_LENGTH];
static unsigned int current_thread = 0;

static void all_threads_init(void);
static void all_threads_start(void);

void init_all_thread()
{
    //1.初始化管理者线程及其他线程
    all_threads_init();

    //2.初始化信号量和互斥锁
    void synchronization_primitives_init(void);
         synchronization_primitives_init();

    //3.启动各个线程(至此为止我们真的准备好启动它们了吗?)
    //all_threads_start();
}

///////////////////////////////////////////////////////////////////////////////
//线程比较特殊,通常它应该是特例情况,所以只能使用是定制,而不像其他的组件
//也就是说我们还是无法为各个操作系统提供对于线程的合理创建方式
#define STACK_CONTROLLER_SIZE (256)
static K_THREAD_STACK_DEFINE(thread_controller_stack, STACK_CONTROLLER_SIZE);

#ifdef THREAD_INTERRUPT
#define STACK_INTERRUPT_SIZE  (512)
static K_THREAD_STACK_DEFINE(thread_interrupt_stack, STACK_INTERRUPT_SIZE);
unsigned int thread_interrupt_id;
#endif

//静态创建各个线程及其线程编号
#ifdef THREAD0
#define STACK0_SIZE  (5120)//5120
static K_THREAD_STACK_DEFINE(thread_stack0, STACK0_SIZE);
unsigned int thread0_id;
#endif

#ifdef THREAD1
#define STACK1_SIZE  (5120)//5120
static K_THREAD_STACK_DEFINE(thread_stack1, STACK1_SIZE);
unsigned int thread1_id;
#endif

//需要用于注册的线程集合:优先级由0最大开始
void app_thread_controller_main(void);  //控制者线程(主线程),优先级:2
void app_thread_interrupt_main(void);   //中断处理线程,优先级:1

static void all_threads_init(void)
{
    //先创建控制者线程
    thread_owner.stack      = thread_controller_stack;
    thread_owner.stack_size = STACK_CONTROLLER_SIZE;
    thread_owner.prio       = 2;
    thread_owner.entry      = (k_thread_entry_t)app_thread_controller_main;
    
#ifdef THREAD_INTERRUPT
    thread_set[current_thread].stack      = thread_interrupt_stack;
    thread_set[current_thread].stack_size = STACK_INTERRUPT_SIZE;
    thread_set[current_thread].prio       = 1;
    thread_set[current_thread].entry      = (k_thread_entry_t)app_thread_interrupt_main;
    thread_interrupt_id = current_thread++;
#endif

#ifdef THREAD0
    thread_set[current_thread].stack      = thread_stack0;
    thread_set[current_thread].stack_size = STACK0_SIZE;
    thread_set[current_thread].prio       = 3;
    thread_set[current_thread].entry      = NULL;
    thread0_id = current_thread++;
#endif

#ifdef THREAD1
    thread_set[current_thread].stack      = thread_stack1;
    thread_set[current_thread].stack_size = STACK1_SIZE;
    thread_set[current_thread].prio       = 3;
    thread_set[current_thread].entry      = NULL;
    thread1_id = current_thread++;
#endif
    
    void set_user_when_controller_init(unsigned int number);
         set_user_when_controller_init(current_thread);
}

static void all_threads_start(void)
{
    thread_create(&thread_owner);
#ifdef THREAD0
    thread_create(&(thread_set[thread0_id]));
#endif
#ifdef THREAD1
    thread_create(&(thread_set[thread1_id]));
#endif
}




