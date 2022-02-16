#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/uaccess.h>

#include <linux/fs.h>
#include <linux/errno.h>

#include <linux/ioctl.h>
//定义灯亮灯灭的命令，模拟定义
#define X6818_LED_ON	_IOW('L', 1, unsigned long)
#define X6818_LED_OFF	_IOW('L', 2, unsigned long)

//1.定义一个字符设备
static struct cdev x6818_led_cdev;
//3、给字符设备申请一个设备号
static unsigned int led_major = 100; //主设备号
static unsigned int led_minor = 0; //次设备号
static dev_t  led_num; //设备号
//6、创建class
static struct class * leds_class;
//7、创建device，其中device是属于class的
static struct device *leds_device;
//8、申请物理内存区，申请SFR的地址区。
//SFR --- Special Function Register： GPIOEOUT
static struct resource *  leds_res_c;
static struct resource *  leds_res_e;
//9、内存的动态映射，得到物理地址对应的虚拟地址
static void __iomem *gpioc_base_va;
static void __iomem *gpiocout_va;//0x00
static void __iomem *gpiocoutenb_va;//0x04
static void __iomem *gpiocaltfn0_va;//0x20
static void __iomem *gpiocaltfn1_va;//0x24
static void __iomem *gpiocpad_va;//0x18

static void __iomem *gpioe_base_va;
static void __iomem *gpioeout_va;//0x00
static void __iomem *gpioeoutenb_va;//0x04
static void __iomem *gpioealtfn0_va;//0x20
static void __iomem *gpioealtfn1_va;//0x24
static void __iomem *gpioepad_va;//0x18

static long x6818_led_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	if (args < 7 || args > 10)
		return -EINVAL;
	switch (cmd) {
	case X6818_LED_ON:
		switch (args) {
		case 7:		
			writel(readl(gpioeout_va) & ~(1 << 13), gpioeout_va);
			break;
		case 8:
			writel(readl(gpiocout_va) & ~(1 << 17), gpiocout_va);
			break;		
		case 9:		
			writel(readl(gpiocout_va) & ~(1 << 8), gpiocout_va);		
			break;	
		case 10:
			writel(readl(gpiocout_va) & ~(1 << 7), gpiocout_va);
			break;
		}
		break;
	case X6818_LED_OFF:
		switch (args) {
		case 7:	
			writel(readl(gpioeout_va) | (1 << 13), gpioeout_va);
			break;
		case 8:		
			writel(readl(gpiocout_va) | (1 << 17), gpiocout_va);		
			break;		
		case 9:		
			writel(readl(gpiocout_va) | (1 << 8), gpiocout_va);		
			break;	
		case 10:		
			writel(readl(gpiocout_va) | (1 << 7), gpiocout_va);
			break;
		}
		break;
	default:	
		return -ENOIOCTLCMD;
	}
	return args;
}

//2.定义一个字符操作集
static const struct file_operations x6818_led_dev_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = x6818_led_ioctl,
};

static void __init led_virtual_address_init_1(void)
{
	//10.1 GPIOC7,8,17 --->function1,作为普通的GPIO
	*(unsigned int *)gpiocaltfn0_va &=~((3 << 14) | (3 << 16));
	*(unsigned int *)gpiocaltfn0_va |= ((1 << 14) | (1 << 16));	
	*(unsigned int *)gpiocaltfn1_va &=~(3 << 2);
	*(unsigned int *)gpiocaltfn1_va |= (1 << 2);
	//10.2 GPIOC7,8,17 --->设置为输出
	*(unsigned int *)gpiocoutenb_va |= ((1 << 7) | (1 << 8) | (1 << 17));
	//10.3 GPIOC7,8,17 --->设置为输出高电平，D8~D11 off
	*(unsigned int *)gpiocout_va |= ((1 << 7) | (1 << 8) | (1 << 17));	
	
	//10.1 GPIOE13 --->function1,作为普通的GPIO
	*(unsigned int *)gpioealtfn0_va &=~(3 << 26);	
	*(unsigned int *)gpioealtfn0_va |= (1 << 26);
	//*(unsigned int *)gpioealtfn1_va ;	
	//*(unsigned int *)gpioealtfn1_va ;
	//10.2 GPIOE13 --->设置为输出
	*(unsigned int *)gpioeoutenb_va |= (1 << 13);
	//10.3 GPIOE13 --->设置为输出高电平，GPIOE13 off
	*(unsigned int *)gpioeout_va |= (1 << 13);
}

static void __init led_virtual_address_init_2(void)
{
	int temp;
	//10.1 GPIOC7,8,17 --->function1,作为普通的GPIO
	temp = readl(gpiocaltfn0_va);
	temp &=~((3 << 14) | (3 << 16));
	temp |= ((1 << 14) | (1 << 16));
	writel(temp, gpiocaltfn0_va);
	
	temp = readl(gpiocaltfn1_va);
	temp &=~(3 << 2);
	temp |= (1 << 2);
	writel(temp, gpiocaltfn1_va);
	
	//10.2 GPIOC7,8,17 --->设置为输出
	temp = readl(gpiocoutenb_va);
	temp |= ((1 << 7) | (1 << 8) | (1 << 17));
	writel(temp, gpiocoutenb_va);
	
	//10.3 GPIOC7,8,17 --->设置为输出高电平，D8~D11 off
	temp = readl(gpiocout_va);
	temp |= ((1 << 7) | (1 << 8) | (1 << 17));
	writel(temp, gpiocout_va);
	
	
	//10.1 GPIOE13 --->function1,作为普通的GPIO
	temp = readl(gpioealtfn0_va);
	temp &=~(3 << 26);
	temp |= (1 << 26);
	writel(temp, gpioealtfn0_va);
	
	//temp = readl(gpioealtfn1_va);
	//writel(temp,gpioealtfn1_va);
	
	//10.2 GPIOE13 --->设置为输出
	temp = readl(gpioeoutenb_va);
	temp |= (1 << 13);
	writel(temp, gpioeoutenb_va);
	
	//10.3 GPIOE13 --->设置为输出高电平，GPIOE13 off
	temp = readl(gpioeout_va);
	temp |= (1 << 13);
	writel(temp, gpioeout_va);
	
}

static int __init led_init(void)
{
	int ret;
	//3、给字符设备申请一个设备号
	if (led_major == 0)
		ret = alloc_chrdev_region(&led_num, led_minor, 1, "led_device");
	else {
		led_num = MKDEV(led_major, led_minor);
		ret = register_chrdev_region(led_num, 1, "led_device");
	}
	
	if (ret < 0) {
		printk(KERN_WARNING "can not get a device number \n");
		goto chrdev_region_fail;
	}
	
	//4、初始化字符设备
	cdev_init(&x6818_led_cdev, &x6818_led_dev_fops);
	
	//5、将字符设备加入内核
	ret = cdev_add(&x6818_led_cdev, led_num, 1);
	if (ret < 0) {
		printk(KERN_WARNING "can not add cdev \n");
		goto cdev_add_fail;
	}
	
	//6、创建class
	leds_class = class_create(THIS_MODULE, "x6818_leds");
	if(leds_class == NULL) {
		printk(KERN_WARNING "class create error\n");
		goto class_create_fail;
	}
	
	//7、创建device，其中device是属于class的
	leds_device = device_create(leds_class, NULL, led_num, NULL, "led_dev");
	if (leds_device == NULL) {
		printk(KERN_WARNING "device create error\n");
		goto device_create_fail;
	}
	
	//8、申请物理内存区，申请SFR的地址区。
	//SFR --- Special Function Register： GPIOEOUT
	leds_res_c = request_mem_region(0xC001C000, 0x1000, "GPIOC_MEM");
	if (leds_res_c == NULL) {
		printk(KERN_WARNING "request memory error\n");
		goto request_mem_region_C_fail;
	}
	
	leds_res_e = request_mem_region(0xC001E000, 0x1000, "GPIOE_MEM");
	if (leds_res_e == NULL)	{ 
		printk(KERN_WARNING "request memory error\n");	
		goto request_mem_region_E_fail;		
	}	
		
	//9、内存的动态映射，得到物理地址对应的虚拟地址
	gpioc_base_va = ioremap(0xC001C000, 0x1000);
	if (gpioc_base_va == NULL) {
		printk(KERN_WARNING "request memory error\n");
		goto ioremap_C_fail;		
	}	
	
	gpioe_base_va = ioremap(0xC001E000, 0x1000);
	if (gpioe_base_va == NULL) {
		printk(KERN_WARNING "request memory error\n");
		goto ioremap_E_fail;			
	}	
	
	//得到每个寄存器的虚拟地址
	gpiocout_va = gpioc_base_va + 0x00;
	gpiocoutenb_va = gpioc_base_va + 0x04;
	gpiocaltfn0_va = gpioc_base_va + 0x20;
	gpiocaltfn1_va = gpioc_base_va + 0x24;
	gpiocpad_va = gpioc_base_va + 0x18;		
	//得到每个寄存器的虚拟地址
	gpioeout_va = gpioe_base_va + 0x00;
	gpioeoutenb_va = gpioe_base_va + 0x04;
	gpioealtfn0_va = gpioe_base_va + 0x20;
	gpioealtfn1_va = gpioe_base_va + 0x24;
	gpioepad_va = gpioe_base_va + 0x18;	
	
	printk(KERN_WARNING "gpiocout_va = %p,gpiocpad_va=%p\n", gpiocout_va, gpiocpad_va);
	printk(KERN_WARNING "gpioeout_va = %p,gpioepad_va=%p\n", gpioeout_va, gpioepad_va);	
	
	//10、访问虚拟地址
	//led_virtual_address_init_1();
	led_virtual_address_init_2();
	
		
	printk(KERN_WARNING "x6818 led driver init \n");	
	return 0;
	
ioremap_E_fail:
	iounmap(gpioc_base_va);	
ioremap_C_fail:
	release_mem_region(0xC001E000, 0x1000);	
request_mem_region_E_fail:
	release_mem_region(0xC001C000, 0x1000);	
request_mem_region_C_fail:
	device_destroy(leds_class, led_num);
device_create_fail:
	class_destroy(leds_class);
class_create_fail:
	cdev_del(&x6818_led_cdev);	
	unregister_chrdev_region(led_num, 1);	
	return -EBUSY;
	
cdev_add_fail:
	unregister_chrdev_region(led_num, 1);
chrdev_region_fail:
	return ret;		
}

static void __exit led_exit(void)
{
	iounmap(gpioe_base_va);	
	iounmap(gpioc_base_va);	
	release_mem_region(0xC001E000, 0x1000);	
	release_mem_region(0xC001C000, 0x1000);	
	device_destroy(leds_class, led_num);
	class_destroy(leds_class);
	cdev_del(&x6818_led_cdev);	
	unregister_chrdev_region(led_num, 1);
	
	printk(KERN_WARNING  "x6818 led driver exit \n");		
}

//驱动程序出入口:insmod, rmmod
module_init(led_init);
module_exit(led_exit);

//module的描述。#modinfo led_drv.ko
MODULE_AUTHOR("hahaha");
MODULE_DESCRIPTION("LED driver for X6818");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");