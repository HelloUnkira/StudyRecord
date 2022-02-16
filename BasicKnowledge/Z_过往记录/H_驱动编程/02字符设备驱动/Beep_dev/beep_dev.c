#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/uaccess.h>

#include <linux/fs.h>
#include <linux/errno.h>

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

static int x6818_beep_open(struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "beep driver is opened\n");
	return 0;
}

static int x6818_beep_release(struct inode *inode, struct file *filp)
{
	printk(KERN_WARNING "beep driver is closed\n");
	return 0;
}

//应用程序write()-->系统调用-->驱动程序gec6818_beep_write()
//const char __user *user_buf ---->应用程序写下来的数据
//size_t size --->应用程序写下来的数据大小
//定义一个数据格式：user_buf[1]--->beep的灯号：7/8/9/10
//                  user_buf[0]--->beep的状态：1--灯亮， 0--灯灭

static ssize_t x6818_beep_read_1(struct file *filp, char __user *user_buf, size_t size, loff_t *off)
{
	//将驱动程序的数据发送给应用程序，这个数据代表beep的状态
	char temp;
	int ret;
	if(size != 1)
		return -EINVAL;
	//通过读取寄存器的状态，得到每个beep的状态
	//D10 -- c14
	temp = (*(unsigned int *)gpiocout_va >> 14) & 0x01;	
	ret = copy_to_user(user_buf, &temp, size);
	if(ret != 0)
		return -EFAULT;
	return size;
}

static ssize_t x6818_beep_write_1(struct file *filp, const char __user *user_buf, size_t size, loff_t *off)
{
	char temp;
	int ret;
	if (size != 1)
		return -EINVAL;
	ret = copy_from_user(&temp, user_buf, size);
	if (ret != 0)
		return -EFAULT;
		
	if (temp == 0)
		*(unsigned int *)gpiocout_va |= (1<<14);
	else if (temp == 1)
		*(unsigned int *)gpiocout_va &= ~(1<<14);
	else
		return -EINVAL;
		
	return size;
}


static ssize_t x6818_beep_read_2(struct file *filp, char __user *user_buf, size_t size, loff_t *off)
{
	//将驱动程序的数据发送给应用程序，这个数据代表beep的状态
	char temp;
	int ret;
	if(size != 1)
		return -EINVAL;
	//通过读取寄存器的状态，得到每个beep的状态-->kbuf[4]	
	temp = (char)(~readl(gpiocout_va) >> 14) & 0x01;
	ret = copy_to_user(user_buf, &temp, size);
	if(ret != 0)
		return -EFAULT;
	return size;
}

static ssize_t x6818_beep_write_2(struct file *filp, const char __user *user_buf, size_t size, loff_t *off)
{
	char temp;
	int ret;
	if (size != 1)
		return -EINVAL;
	ret = copy_from_user(&temp, user_buf, size);
	if (ret != 0)
		return -EFAULT;
	
	if (temp == 0)
		writel(readl(gpiocout_va)|(1<<14), gpiocout_va);
	else if (temp == 1)
		writel(readl(gpiocout_va)&~(1<<14), gpiocout_va);
	else
		return -EINVAL;

	return size;
}



static ssize_t x6818_beep_read(struct file *filp, char __user *user_buf, size_t size, loff_t *off)
{
	return x6818_beep_read_1(filp, user_buf, size, off);
	//return x6818_beep_read_2(filp, user_buf, size, off);
}

static ssize_t x6818_beep_write(struct file *filp, const char __user *user_buf, size_t size, loff_t *off)
{
	return x6818_beep_write_1(filp, user_buf, size, off);
	//return x6818_beep_write_2(filp, user_buf, size, off);
}

//2.定义一个字符操作集
static const struct file_operations x6818_beep_dev_fops = {
	.owner = THIS_MODULE,
	.open = x6818_beep_open,
	.read = x6818_beep_read,
	.write = x6818_beep_write,
	.release = x6818_beep_release,
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