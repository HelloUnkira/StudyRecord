#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/slab.h>
#include <linux/vmalloc.h>

#include <linux/sched.h>
#include <linux/time.h>
//定义动态定时器
static struct timer_list x6818_timer;

static int* get_kmalloc = NULL;
static int* get_vmalloc = NULL;

//动态定时器超时处理
void  x6818_timer_fun(unsigned long data) //data=10
{
	int ret;
	printk(KERN_WARNING "jiffies=%lu\n",jiffies);
	printk(KERN_WARNING "data = %d\n",data);
	//修改超时时间并启动（这会导致死循环的）
	ret = mod_timer(&x6818_timer, jiffies + 100);
}



static int __init example_init(void)
{
	//内存分配,这里不做出错检查，简单点默认成功
	get_kmalloc = (int*)kmalloc(1024 * 4, GFP_KERNEL);
	get_vmalloc = (int*)vmalloc(1024 * 1024 * 4);
	
	//初始化动态定时器
	x6818_timer.function = x6818_timer_fun;
	x6818_timer.expires = jiffies + 100;
	x6818_timer.data = 10;	
	
	//初始化动态定时器
	init_timer(&x6818_timer);
	
	//动态定时器加入内核并启动
	add_timer(&x6818_timer);
			
	printk(KERN_WARNING "device example inited\n");
	return 0;
}

static void __exit example_exit(void)
{
	del_timer(&x6818_timer);

	kfree(get_kmalloc);
	vfree(get_vmalloc);

	printk(KERN_WARNING "device example exited\n");
}


//驱动程序出入口:insmod, rmmod
module_init(example_init);
module_exit(example_exit);

//module的描述。#modinfo example_drv.ko
MODULE_AUTHOR("hahaha");
MODULE_DESCRIPTION("example driver for X6818");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");