#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/platform_device.h>

//1.定义一个plaform device
void led_release(struct device *dev)
{
	printk(KERN_WARNING "release led device\n");
}

//4.定义一个属于device的resource---描述硬件的信息
static struct resource x6818_led_resource[] = {
	[0] = {
		.start	= 0xC001C000, //SFR的开始地址
		.end	= 0xC001C000 + 0x1000 -1,	//SFR的结束地址	
		.flags	= IORESOURCE_MEM //resource的类型
	},
	[1] = {
		.start	= 0xC001E000, //SFR的开始地址
		.end	= 0xC001E000 + 0x1000 -1,	//SFR的结束地址	
		.flags	= IORESOURCE_MEM //resource的类型
	},
};

static struct platform_device x6818_led_device = {
	.name			= "x6818_led", //device的名字，必须与driver的名字一致
	.id				= 0,
	.num_resources	= ARRAY_SIZE(x6818_led_resource),
	.resource		= x6818_led_resource,
	.dev			= {
	.release		= led_release,
	}
};


//入口函数--->安装驱动
static int __init x6818_led_dev_init(void)
{
	int ret;
	//2.将platform device安装到platform bus	
	ret = platform_device_register(&x6818_led_device);	
	printk(KERN_WARNING "x6818 led device init \n");	
	return ret;
}

//出口函数--->卸载驱动
static void __exit x6818_led_dev_exit(void)
{
	//3.卸载一个platform device	
	platform_device_unregister(&x6818_led_device);	
	printk(KERN_WARNING "x6818 led device exit \n");
}

module_init(x6818_led_dev_init);
module_exit(x6818_led_dev_exit);

MODULE_AUTHOR("hahaha");
MODULE_DESCRIPTION("LED driver for X6818");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");
