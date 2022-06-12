#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <cfg_type.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/uaccess.h>

#include <linux/ioctl.h>
//定义命令，模拟定义
#define X6818_KEY_STATE _IOR('K', 1, unsigned char*)

//1.定义一个字符设备
static struct cdev x6818_key_cdev;
//3、给字符设备申请一个设备号
static unsigned int key_major = 100; //主设备号
static unsigned int key_minor = 0; //次设备号
static dev_t  key_num; //设备号
//6、创建class
static struct class * keys_class;
//7、创建device，其中device是属于class的
static struct device *keys_device;
//8、定义GPIO口集
struct key_gpio_info{
	unsigned gpio_num;
	char gpio_name[20];	
};

static const struct key_gpio_info  key_info[4]={
	{//K2  -- GPIOA28
		.gpio_num  = PAD_GPIO_A + 28,
		.gpio_name = "GPIOA28_D7",
	},
	{//K3  -- GPIOB30
		.gpio_num  = PAD_GPIO_B + 30,
		.gpio_name = "GPIOB30_D8",
	},
	{//K4  -- GPIOB31
		.gpio_num  = PAD_GPIO_B + 31,
		.gpio_name = "GPIOB31_D9",
	},
	{//K6  -- GPIOB9
		.gpio_num  = PAD_GPIO_B + 9,
		.gpio_name = "GPIOB9_D10",
	},
};

static long x6818_key_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	switch (cmd) {			
	case X6818_KEY_STATE: {
		unsigned char kbuf[4];
		int ret, i, size = sizeof((unsigned char*)args);	
		if (size != 4)
			return -EINVAL;
		for (i = 0; i < 4; i++)
			kbuf[i] = gpio_get_value(key_info[i].gpio_num);	
		ret = copy_to_user((unsigned char*)args, kbuf, size);
		if(ret != 0)
			return -EFAULT;
		return 0;
	}		
	default:	
		return -ENOIOCTLCMD;
	}
}

//2.定义一个字符操作集
static const struct file_operations x6818_key_dev_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = x6818_key_ioctl,
};

static int __init key_init(void)
{
	int ret, i;
	//3、给字符设备申请一个设备号
	if (key_major == 0)
		ret = alloc_chrdev_region(&key_num, key_minor, 1, "key_device");
	else {
		key_num = MKDEV(key_major, key_minor);
		ret = register_chrdev_region(key_num, 1, "key_device");
	}
	
	if (ret < 0) {
		printk(KERN_WARNING "can not get a device number \n");
		goto chrdev_region_fail;
	}
	
	//4、初始化字符设备
	cdev_init(&x6818_key_cdev, &x6818_key_dev_fops);
	
	//5、将字符设备加入内核
	ret = cdev_add(&x6818_key_cdev, key_num, 1);
	if (ret < 0) {
		printk(KERN_WARNING "can not add cdev \n");
		goto cdev_add_fail;
	}
	
	//6、创建class
	keys_class = class_create(THIS_MODULE, "x6818_keys");
	if(keys_class == NULL) {
		printk(KERN_WARNING "class create error\n");
		goto class_create_fail;
	}
	
	//7、创建device，其中device是属于class的
	keys_device = device_create(keys_class, NULL, key_num, NULL, "key_dev");
	if (keys_device == NULL) {
		printk(KERN_WARNING "device create error\n");
		goto device_create_fail;
	}
	
	//申请GPIO*4
	for (i = 0; i <4; i++)
		if(gpio_request(key_info[i].gpio_num, key_info[i].gpio_name) < 0) {
			printk(KERN_WARNING "cannot request gpio:%s\n", key_info[i].gpio_name);
			goto gpio_request_fail;
		} else
			gpio_direction_input(key_info[i].gpio_num);
		
			
	printk(KERN_WARNING "x6818 key driver init \n");	
	return 0;
	
gpio_request_fail:
	while (i--)
		gpio_free(key_info[i].gpio_num);
	device_destroy(keys_class, key_num);
device_create_fail:
	class_destroy(keys_class);
class_create_fail:
	cdev_del(&x6818_key_cdev);	
	unregister_chrdev_region(key_num, 1);	
	return -EBUSY;
	
cdev_add_fail:
	unregister_chrdev_region(key_num, 1);
chrdev_region_fail:
	return ret;		
}

static void __exit key_exit(void)
{
	int i = 0;
	for (i = 0; i <4; i++)
		gpio_free(key_info[i].gpio_num);
	device_destroy(keys_class, key_num);
	class_destroy(keys_class);
	cdev_del(&x6818_key_cdev);	
	unregister_chrdev_region(key_num, 1);
	
	printk(KERN_WARNING  "x6818 key driver exit \n");		
}

//驱动程序出入口:insmod, rmmod
module_init(key_init);
module_exit(key_exit);

//module的描述。#modinfo key_drv.ko
MODULE_AUTHOR("hahaha");
MODULE_DESCRIPTION("key driver for X6818");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");