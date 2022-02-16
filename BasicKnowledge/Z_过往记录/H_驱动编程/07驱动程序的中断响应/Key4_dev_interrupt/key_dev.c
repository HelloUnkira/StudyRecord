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

#include <cfg_type.h>
#include <linux/interrupt.h>

#include <linux/ioctl.h>
//定义命令，模拟定义
#define X6818_KEY_STATE _IOR('K', 1, unsigned char*)

#include <linux/sched.h>
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
//8、中断使用到的参数
static int key_value[4] = {2, 3, 4, 6};
static int key_state[4] = {1, 1, 1, 1};
struct key_interrupt_info{
	int int_num;
	char int_name[20];
};

static struct key_interrupt_info all_key_interrupt_info[4] = {
	{
		.int_num = IRQ_GPIO_A_START + 28,
		.int_name = "Key2_GpioA28",
	},
	{
		.int_num = IRQ_GPIO_B_START + 30,
		.int_name = "Key3_GpioB30",
	},
	{
		.int_num = IRQ_GPIO_B_START + 31,
		.int_name = "Key4_GpioB31",
	},
	{
		.int_num = IRQ_GPIO_B_START + 9,
		.int_name = "Key6_GpioB9",
	},
};

//9、定义并初始化等待队列
static int key_interrupt_flag = 0;
static wait_queue_head_t x6818_key_waitqueue;

//中断服务例程
irqreturn_t key_irq_handler_t(int irq, void *dev)
{ 
	printk(KERN_WARNING "press\n");
	if (*((int *)dev) == key_value[0])
		key_state[0] = 0;
	else if (*((int *)dev) == key_value[1])
		key_state[1] = 0;	
	else if (*((int *)dev) == key_value[2])
		key_state[2] = 0;	
	else if (*((int *)dev) == key_value[3])
		key_state[3] = 0;		
	//唤醒等待队列的进程
	key_interrupt_flag = 1;	
	wake_up(&x6818_key_waitqueue);
	return IRQ_HANDLED;
}

static long x6818_key_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	switch (cmd) {			
	case X6818_KEY_STATE: {
		//sizeof不能如期求出数组长度
		int i, size = sizeof(((unsigned char*)args));
		if (size != 4)
			return -EINVAL;
		//访问等待队列
		wait_event_interruptible(x6818_key_waitqueue, key_interrupt_flag);	
		for (i = 0; i < 4; i++) {
			((unsigned char*)args)[i] = (unsigned char)key_state[i];
			key_state[i] = 1;	
		}			
		key_interrupt_flag = 0;
		return 1;
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

static int __init x6818_key_init(void) //出现冲突
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
		printk(KERN_WARNING "can not get a device int_num \n");
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
	
	//8、中断的注册
	for (i = 0; i < 4; i++)
		if ((ret = request_irq(all_key_interrupt_info[i].int_num, \
			key_irq_handler_t, IRQF_TRIGGER_FALLING, \
			all_key_interrupt_info[i].int_name, (void *)(&key_value[i]))) < 0) {
			printk(KERN_WARNING "key interrupt request fail\n");
			goto key_request_irq_fail;
		}
	
	//9、初始化等待队列
	init_waitqueue_head(&x6818_key_waitqueue);
	
	printk(KERN_WARNING "x6818 key driver init \n");	
	return 0;
	
key_request_irq_fail:
	while(i--)
		free_irq(all_key_interrupt_info[i].int_num, \
			(void *)(&key_value[i]));
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

static void __exit x6818_key_exit(void)
{
	int i = 3;
	for (i = 3; i >= 0; i--)
		free_irq(all_key_interrupt_info[i].int_num, \
			(void *)(&key_value[i]));	
	device_destroy(keys_class, key_num);
	class_destroy(keys_class);
	cdev_del(&x6818_key_cdev);	
	unregister_chrdev_region(key_num, 1);
	
	printk(KERN_WARNING  "x6818 key driver exit \n");		
}

//驱动程序出入口:insmod, rmmod
module_init(x6818_key_init);
module_exit(x6818_key_exit);

//module的描述。#modinfo key_drv.ko
MODULE_AUTHOR("hahaha");
MODULE_DESCRIPTION("key driver for X6818");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");