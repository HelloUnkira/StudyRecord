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
#define X6818_BEEP_ON	_IO('B', 1)
#define X6818_BEEP_OFF	_IO('B', 2)

//1.定义一个字符设备
static struct cdev x6818_beep_cdev;
//3、给字符设备申请一个设备号
static unsigned int beep_major = 100; //主设备号
static unsigned int beep_minor = 0; //次设备号
static dev_t  beep_num; //设备号
//6、创建class
static struct class * beep_class;
//7、创建device，其中device是属于class的
static struct device *beep_device;
//8、申请物理内存区，申请SFR的地址区。
//SFR --- Special Function Register： GPIOEOUT
static struct resource *  beep_res_c;
//9、内存的动态映射，得到物理地址对应的虚拟地址
static void __iomem *gpioc_base_va;
static void __iomem *gpiocout_va;//0x00
static void __iomem *gpiocoutenb_va;//0x04
static void __iomem *gpiocaltfn0_va;//0x20
static void __iomem *gpiocaltfn1_va;//0x24
static void __iomem *gpiocpad_va;//0x18

static long x6818_beep_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	switch (cmd) {
	case X6818_BEEP_ON:
		*(unsigned int *)gpiocout_va &= ~(1<<14);
		return 1;
	case X6818_BEEP_OFF:
		*(unsigned int *)gpiocout_va |= (1<<14);
		return 0;
	default:	
		return -ENOIOCTLCMD;
	}
}

//2.定义一个字符操作集
static const struct file_operations x6818_beep_dev_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = x6818_beep_ioctl,
};

static void __init beep_virtual_address_init_1(void)
{
	//10.1 GPIOC14 --->function1,作为普通的GPIO
	*(unsigned int *)gpiocaltfn1_va &= ~(2<<28);	
	//10.2 GPIOC7,8,17 --->设置为输出
	*(unsigned int *)gpiocoutenb_va |=  (1<<14);
	//10.3 GPIOC7,8,17 --->设置为输出高电平，D8~D11 off
	*(unsigned int *)gpiocout_va 	&= ~(1<<14);	
}

static void __init beep_virtual_address_init_2(void)
{
	int temp;
	//10.1 GPIOC14 --->function1,作为普通的GPIO
	temp = readl(gpiocaltfn1_va);
	temp &=~(2 << 28);
	writel(temp, gpiocaltfn1_va);
		
	//10.2 GPIOC14 --->设置为输出
	temp = readl(gpiocoutenb_va);
	temp |= (1 << 14);
	writel(temp, gpiocoutenb_va);
	
	//10.3 GPIOC14 --->设置为输出高电平，D8~D11 off
	temp = readl(gpiocout_va);
	temp &= ~(1 << 14);
	writel(temp, gpiocout_va);	
}

static int __init beep_init(void)
{
	int ret;
	//3、给字符设备申请一个设备号
	if (beep_major == 0)
		ret = alloc_chrdev_region(&beep_num, beep_minor, 1, "beep_device");
	else {
		beep_num = MKDEV(beep_major, beep_minor);
		ret = register_chrdev_region(beep_num, 1, "beep_device");
	}
	
	if (ret < 0) {
		printk(KERN_WARNING "can not get a device number \n");
		goto chrdev_region_fail;
	}
	
	//4、初始化字符设备
	cdev_init(&x6818_beep_cdev, &x6818_beep_dev_fops);
	
	//5、将字符设备加入内核
	ret = cdev_add(&x6818_beep_cdev, beep_num, 1);
	if (ret < 0) {
		printk(KERN_WARNING "can not add cdev \n");
		goto cdev_add_fail;
	}
	
	//6、创建class
	beep_class = class_create(THIS_MODULE, "x6818_beeps");
	if(beep_class == NULL) {
		printk(KERN_WARNING "class create error\n");
		goto class_create_fail;
	}
	
	//7、创建device，其中device是属于class的
	beep_device = device_create(beep_class, NULL, beep_num, NULL, "beep_dev");
	if (beep_device == NULL) {
		printk(KERN_WARNING "device create error\n");
		goto device_create_fail;
	}
	
	//8、申请物理内存区，申请SFR的地址区。
	//SFR --- Special Function Register： GPIOEOUT
	beep_res_c = request_mem_region(0xC001C000, 0x1000,"GPIOC_MEM");
	if (beep_res_c == NULL) {
		printk(KERN_WARNING "request memory error\n");
		goto request_mem_region_fail;
	}
		
	//9、内存的动态映射，得到物理地址对应的虚拟地址
	gpioc_base_va = ioremap(0xC001C000, 0x1000);
	if (gpioc_base_va == NULL) {
		printk(KERN_WARNING "request memory error\n");
		goto ioremap_fail;		
	}	
			
	//得到每个寄存器的虚拟地址
	gpiocout_va = gpioc_base_va + 0x00;
	gpiocoutenb_va = gpioc_base_va + 0x04;
	gpiocaltfn0_va = gpioc_base_va + 0x20;
	gpiocaltfn1_va = gpioc_base_va + 0x24;
	gpiocpad_va = gpioc_base_va + 0x18;		

	printk(KERN_WARNING "gpiocout_va = %p,gpiocpad_va=%p\n",gpiocout_va, gpiocpad_va);
	
	//10、访问虚拟地址
	//beep_virtual_address_init_1();
	beep_virtual_address_init_2();
	
		
	printk(KERN_WARNING "x6818 beep driver init \n");	
	return 0;
	
ioremap_fail:
	release_mem_region(0xC001C000, 0x1000);	
request_mem_region_fail:
	device_destroy(beep_class, beep_num);
device_create_fail:
	class_destroy(beep_class);
class_create_fail:
	cdev_del(&x6818_beep_cdev);	
	unregister_chrdev_region(beep_num, 1);	
	return -EBUSY;

cdev_add_fail:
	unregister_chrdev_region(beep_num, 1);
chrdev_region_fail:
	return ret;		
}

static void __exit beep_exit(void)
{

	iounmap(gpioc_base_va);	
	release_mem_region(0xC001C000, 0x1000);	
	device_destroy(beep_class, beep_num);
	class_destroy(beep_class);
	cdev_del(&x6818_beep_cdev);	
	unregister_chrdev_region(beep_num, 1);
	
	printk(KERN_WARNING  "x6818 beep driver exit \n");		
}

//驱动程序出入口:insmod, rmmod
module_init(beep_init);
module_exit(beep_exit);

//module的描述。#modinfo beep_drv.ko
MODULE_AUTHOR("hahaha");
MODULE_DESCRIPTION("beep driver for X6818");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");