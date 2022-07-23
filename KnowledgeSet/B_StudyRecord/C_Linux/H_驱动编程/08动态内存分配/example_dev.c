#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/slab.h>
#include <linux/vmalloc.h>

static int* get_kmalloc = NULL;
static int* get_vmalloc = NULL;

static int __init example_init(void)
{
	//内存分配
	get_kmalloc = (int*)kmalloc(1024 * 4, GFP_KERNEL);
	get_vmalloc = (int*)vmalloc(1024 * 1024 * 4);
	
	printk(KERN_WARNING "device example inited\n");
	return 0;
}

static void __exit example_exit(void)
{
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