#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/ioctl.h>
#include <linux/miscdevice.h>
#include <linux/gpio.h>
#include <cfg_type.h>
#include <linux/ioport.h>
#include <linux/clk.h>

#include <mach/platform.h>
#include <mach/devices.h>
#include <mach/soc.h>

#define X6818_WATCHDOG_START		_IO('W',  1)
#define X6818_WATCHDOG_STOP			_IO('W',  2)
#define X6818_WATCHDOG_ALIVE		_IO('W',  3)

//4、申请物理内存区，申请SFR的地址区。
//SFR --- Special Function Register： GPIOEOUT
static struct resource *  watchdog_res;
//5、内存的动态映射，得到物理地址对应的虚拟地址
static void __iomem *watchdog_base_va;
static void __iomem *watchdog_con_va; //0x00
static void __iomem *watchdog_dat_va; //0x04
static void __iomem *watchdog_cnt_va; //0x08
static void __iomem *watchdog_clrint_va;//0x0c
//7、看门狗初始化	
static struct clk	*watchdog_clock;

static long x6818_watchdog_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	switch(cmd){
	case X6818_WATCHDOG_START:
		//打开看门狗						//第一次分频 第二次分频 打开中断 打开复位
		*(unsigned int *)watchdog_con_va = (255 << 8) + (3 << 3) + (1 << 2) + (1 << 0);
		*(unsigned int *)watchdog_con_va |= 1 << 5;
		break;	
	case X6818_WATCHDOG_STOP:
		//关闭看门狗	
		*(unsigned int *)watchdog_con_va = 0;	
		break;		
	case X6818_WATCHDOG_ALIVE:
		//看门狗喂狗
		*(unsigned int *)watchdog_cnt_va = 0xEE66;
		break;			
	default:
		return -ENOIOCTLCMD;		
	}
	return 0;	
}

//1、定义一个字符操作集，并初始化
static const struct file_operations x6818_watchdog_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = x6818_watchdog_ioctl,
};

//2、定义一个混杂设备，并初始化
static  struct miscdevice x6818_watchdog_misc = {
	.minor = MISC_DYNAMIC_MINOR, //动态分配次设备号
	.name = "watchdog_drv",
	.fops = &x6818_watchdog_fops,
};


static int __init watchdog_init(void)
{
	int ret;
	static unsigned long watchdog_freq;
	//3、注册一个混杂设备
	ret = misc_register(&x6818_watchdog_misc);
	if (ret < 0) {
		printk(KERN_WARNING "miscdevice request error\n");
		goto misc_register_fail;
	}

	//4、申请物理内存区
	watchdog_res = request_mem_region(0xC0019000, 0x1000,"watchdog_MEM");
	if(watchdog_res == NULL){
		printk(KERN_WARNING "request memory error\n");
		ret = -EBUSY;
		goto request_mem_region_fail;
	}
	
	//5、IO内存动态映射，得到物理地址对应的虚拟地址
	watchdog_base_va = ioremap(0xC0019000, 0x1000);
	if(watchdog_base_va == NULL){
		printk(KERN_WARNING "ioremap error\n");
		ret = -EBUSY;
		goto ioremap_fail;
	}
	
	//6、初始化地址
	watchdog_con_va = watchdog_base_va + 0x00; //0x00
	watchdog_dat_va = watchdog_base_va + 0x04; //0x04
	watchdog_cnt_va = watchdog_base_va + 0x08; //0x08
	watchdog_clrint_va = watchdog_base_va + 0x0C;//0x0c
			
	//7、看门狗初始化		
	watchdog_clock = clk_get(NULL, "pclk");		
	if (IS_ERR(watchdog_clock)) {
		printk(KERN_WARNING "failed to find watchdog clock source\n");
		ret = PTR_ERR(watchdog_clock);
		goto clk_get_fail;
	}		
	clk_enable(watchdog_clock);	
	//看一下当前时钟
	watchdog_freq = clk_get_rate(watchdog_clock);
	printk(KERN_WARNING "watchdog_freq = %luHz\n", watchdog_freq);

	//硬件复位
	nxp_soc_peri_reset_set(RESET_ID_WDT);
	nxp_soc_peri_reset_set(RESET_ID_WDT_POR);
	
	//看门狗初始化
	*(unsigned int *)watchdog_con_va = 0;
	*(unsigned int *)watchdog_dat_va = 0xEE66;//61030
	*(unsigned int *)watchdog_cnt_va = 0xEE66;//61030
	*(unsigned int *)watchdog_clrint_va = 0;	
			
	printk(KERN_WARNING "device watchdog inited\n");
	return 0;

clk_get_fail:	
	iounmap(watchdog_base_va);	
ioremap_fail:	
	release_mem_region(0xC0019000, 0x1000);
request_mem_region_fail:
	misc_deregister(&x6818_watchdog_misc);
misc_register_fail:	
	return ret;
}

static void __exit watchdog_exit(void)
{
	//返还时钟并关闭时钟		
	clk_disable(watchdog_clock);
	clk_put(watchdog_clock);	
	iounmap(watchdog_base_va);
	release_mem_region(0xC0019000, 0x1000);
	misc_deregister(&x6818_watchdog_misc);
	printk(KERN_WARNING "device watchdog exited\n");
}

//驱动程序出入口:insmod, rmmod
module_init(watchdog_init);
module_exit(watchdog_exit);

//module的描述。#modinfo watchdog_drv.ko
MODULE_AUTHOR("hahaha");
MODULE_DESCRIPTION("watchdog driver for X6818");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");