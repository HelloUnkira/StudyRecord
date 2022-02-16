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
//定义灯亮灯灭的命令，模拟定义
#define X6818_LED_ON	_IOW('L', 1, unsigned long)
#define X6818_LED_OFF	_IOW('L', 2, unsigned long)
#define X6818_LED_STATE _IOR('L', 3, unsigned char*)

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
//8、定义GPIO口集
struct led_gpio_info{
	unsigned gpio_num;
	char gpio_name[20];	
};

static const struct led_gpio_info  led_info[4]={
	{//D7  -- GPIOE13
		.gpio_num  = PAD_GPIO_E + 13,
		.gpio_name = "GPIOE13_D7",
	},
	{//D8  -- GPIOC17
		.gpio_num  = PAD_GPIO_C + 17,
		.gpio_name = "GPIOC17_D8",
	},
	{//D9  -- GPIOC8
		.gpio_num  = PAD_GPIO_C + 8,
		.gpio_name = "GPIOC8_D9",
	},
	{//D10 -- GPIOC7
		.gpio_num  = PAD_GPIO_C + 7,
		.gpio_name = "GPIOC7_D10",
	},
};

static long x6818_led_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	switch (cmd) {
	case X6818_LED_ON:
		if (args >= 7 && args <= 10) {		
			gpio_set_value(led_info[args - 7].gpio_num, 1);
			return 0;
		} else
			return -EINVAL;
	case X6818_LED_OFF:
		if (args >= 7 && args <= 10) {		
			gpio_set_value(led_info[args - 7].gpio_num, 0);
			return 0;
		} else
			return -EINVAL;		
	case X6818_LED_STATE: {
		unsigned char kbuf[4];
		int ret, i, size = sizeof((unsigned char*)args);	
		if (size != 4)
			return -EINVAL;
		for (i = 0; i < 4; i++)
			kbuf[i] = gpio_get_value(led_info[i].gpio_num);	
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
static const struct file_operations x6818_led_dev_fops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = x6818_led_ioctl,
};

static int __init led_init(void)
{
	int ret, i;
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
	
	//申请GPIO*4
	for (i = 0; i <4; i++)
		if(gpio_request(led_info[i].gpio_num, led_info[i].gpio_name) < 0) {
			printk(KERN_WARNING "cannot request gpio:%s\n", led_info[i].gpio_name);
			goto gpio_request_fail;
		} else
			gpio_direction_output(led_info[i].gpio_num, 1);
		
			
	printk(KERN_WARNING "x6818 led driver init \n");	
	return 0;
	
gpio_request_fail:
	while (i--)
		gpio_free(led_info[i].gpio_num);
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
	int i = 0;
	for (i = 0; i <4; i++)
		gpio_free(led_info[i].gpio_num);
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