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
#include <linux/delay.h>

#include <mach/platform.h>
#include <mach/devices.h>
#include <mach/soc.h>

#define X6818_ADC_SETCHANNEL	_IOW('A', 1, unsigned long)
#define X6818_ADC_GETDAT	    _IOR('A', 2, unsigned long)

//静态映射
#define NXP_ADCREG(x) 			((x) + IO_ADDRESS(PHY_BASEADDR_ADC))
#define NXP_ADCCON      		NXP_ADCREG(0x00)
#define NXP_ADCDAT      		NXP_ADCREG(0x04)
#define NXP_ADCPRESCON      	NXP_ADCREG(0x10)


static long x6818_adc_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	switch(cmd){
	case X6818_ADC_SETCHANNEL: {
		int temp = readl(NXP_ADCCON);
		if(args < 0 || args > 7)
			return -EINVAL;		
		temp &= ~(7 << 3);
		temp |= (args << 3);
		writel(temp, NXP_ADCCON);
		return 0;
	}
	case X6818_ADC_GETDAT: {
		unsigned int time_out = 0;
		int ret, temp = readl(NXP_ADCCON);	
		temp &= ~(1 << 2);
		temp |= (1 << 0);
		writel(temp, NXP_ADCCON);
		do {
			temp = readl(NXP_ADCCON) & (1 << 0);
			udelay(1);
			if (++time_out > 1000)
				return -EBUSY;
		} while (temp == 1);
		temp = readl(NXP_ADCDAT) & 0xFFF;
		ret = copy_to_user((void *)args, &temp, sizeof(temp));
		if (ret < 0)
			return -EFAULT;
		temp = readl(NXP_ADCCON);
		temp |= (1 << 2);
		writel(temp, NXP_ADCCON);		
		return 0;			
	}
	default:
		return -ENOIOCTLCMD;		
	}	
}

//1、定义一个字符操作集，并初始化
static const struct file_operations x6818_adc_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = x6818_adc_ioctl,
};

//2、定义一个混杂设备，并初始化
static  struct miscdevice x6818_adc_misc = {
	.minor = MISC_DYNAMIC_MINOR, //动态分配次设备号
	.name = "adc_drv",
	.fops = &x6818_adc_fops,
};

static int __init adc_init(void)
{
	int ret;
	//硬件复位
	nxp_soc_peri_reset_set(RESET_ID_ADC);	
	
	//3、注册一个混杂设备
	ret = misc_register(&x6818_adc_misc);
	if (ret < 0) {
		printk(KERN_WARNING "miscdevice request error\n");
		goto misc_register_fail;
	}
			
	//ADC初始化
	writel(0, NXP_ADCPRESCON);
	writel((199 << 0), NXP_ADCPRESCON);	
	writel(readl(NXP_ADCPRESCON) | (1 << 15), NXP_ADCPRESCON);
	
	writel((1 << 10) + (10 << 6), NXP_ADCCON);
				
	printk(KERN_WARNING "device adc inited\n");
	return 0;

misc_register_fail:	
	return ret;
}

static void __exit adc_exit(void)
{
	misc_deregister(&x6818_adc_misc);
	printk(KERN_WARNING "device adc exited\n");
}

//驱动程序出入口:insmod, rmmod
module_init(adc_init);
module_exit(adc_exit);

//module的描述。#modinfo adc_drv.ko
MODULE_AUTHOR("hahaha");
MODULE_DESCRIPTION("adc driver for X6818");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");