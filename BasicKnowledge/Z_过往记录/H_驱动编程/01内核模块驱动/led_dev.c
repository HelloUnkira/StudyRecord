#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init led_init(void)
{
	printk(KERN_WARNING "driver led inited\n");
	return 0;
}

static void __exit led_exit(void)
{
	printk("<4>" "driver led exited\n");
}




//驱动程序出入口:insmod, rmmod
module_init(led_init);
module_exit(led_exit);

//module的描述。#modinfo led_drv.ko
MODULE_AUTHOR("hahaha");
MODULE_DESCRIPTION("LED driver for X6818");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");