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

//静态映射
#define NXP_WDOGREG(x) ((x) + IO_ADDRESS(PHY_BASEADDR_WDT))
#define NXP_WTCON      NXP_WDOGREG(0x00)
#define NXP_WTDAT      NXP_WDOGREG(0x04)
#define NXP_WTCNT      NXP_WDOGREG(0x08)
#define NXP_WTCLRINT   NXP_WDOGREG(0x0c)

//4、看门狗初始化	
static struct clk	*watchdog_clock;

//5、申请自旋锁
static  spinlock_t  watchdog_lock;

static long x6818_watchdog_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	unsigned long flags;
	switch(cmd){
	case X6818_WATCHDOG_START: {
		//打开看门狗 //第一次分频 第二次分频 打开中断 打开复位
		unsigned int temp;			
		spin_lock_irqsave(&wdt_lock, flags);			
		temp = (255 << 8) + (3 << 3) + (1 << 2) + (1 << 0);
		temp |= (1 << 5);
		writel(temp, NXP_WTCON);
		spin_unlock_irqrestore(&wdt_lock, flags);	
		break;
	}
	case X6818_WATCHDOG_STOP:
		//关闭看门狗
		spin_lock_irqsave(&wdt_lock, flags);		
		writel(0, NXP_WTCON);
		spin_unlock_irqrestore(&wdt_lock, flags);	
		break;		
	case X6818_WATCHDOG_ALIVE:
		//看门狗喂狗
		spin_lock_irqsave(&wdt_lock, flags);
		writel(0xEE66, NXP_WTCNT);
		spin_unlock_irqrestore(&wdt_lock, flags);	
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
			
	//4、看门狗初始化		
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
	writel(0, NXP_WTCON);
	writel(0xEE66, NXP_WTDAT);
	writel(0xEE66, NXP_WTCNT);
	writel(0, NXP_WTCLRINT);
			
	//5、初始化自旋锁
	spin_lock_init(&watchdog_lock);	
			
	printk(KERN_WARNING "device watchdog inited\n");
	return 0;

clk_get_fail:	
	misc_deregister(&x6818_watchdog_misc);
misc_register_fail:	
	return ret;
}

static void __exit watchdog_exit(void)
{
	//返还时钟并关闭时钟		
	clk_disable(watchdog_clock);
	clk_put(watchdog_clock);	
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