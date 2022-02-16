#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <cfg_type.h>

#include <linux/interrupt.h>

irqreturn_t key_irq_handler_t(int irq, void *dev)
{
	if (*((int *)dev) == 2)
		printk(KERN_WARNING "key2 is pressing, irq = %d\n", irq);
	else if (*((int *)dev) == 3)
		printk(KERN_WARNING "key3 is pressing, irq = %d\n", irq);	
	else if (*((int *)dev) == 4)
		printk(KERN_WARNING "key4 is pressing, irq = %d\n", irq);	
	else if (*((int *)dev) == 6)
		printk(KERN_WARNING "key6 is pressing, irq = %d\n", irq);	
	return IRQ_HANDLED;
}

static int key_value[4] = {2, 3, 4, 6};

static int __init key_init(void)
{
	int ret;

	ret = request_irq(IRQ_GPIO_A_START + 28, key_irq_handler_t, \
		IRQF_TRIGGER_FALLING, "Key2_GpioA28", (void *)(&key_value[0]));
	if (ret < 0) {
		printk(KERN_WARNING "Key2 Interrupt request fail");
		goto key2_request_irq_fail;
	}	
	
	ret = request_irq(IRQ_GPIO_B_START + 30, key_irq_handler_t, \
		IRQF_TRIGGER_FALLING, "Key3_GpioB30", (void *)(&key_value[1]));	
	if (ret < 0) {
		printk(KERN_WARNING "Key3 Interrupt request fail");
		goto key3_request_irq_fail;
	}
	
	ret = request_irq(IRQ_GPIO_B_START + 31, key_irq_handler_t, \
		IRQF_TRIGGER_FALLING, "Key4_GpioB31", (void *)(&key_value[2]));
	if (ret < 0) {
		printk(KERN_WARNING "Key4 Interrupt request fail");
		goto key4_request_irq_fail;
	}
	
	ret = request_irq(IRQ_GPIO_B_START + 9, key_irq_handler_t, \
		IRQF_TRIGGER_FALLING, "Key6_GpioB9", (void *)(&key_value[3]));		
	if (ret < 0) {
		printk(KERN_WARNING "Key6 Interrupt request fail");
		goto key6_request_irq_fail;
	}


	
	printk(KERN_WARNING "device key inited\n");
	return 0;
	
	
key6_request_irq_fail:
	free_irq(IRQ_GPIO_B_START + 31, (void *)(&key_value[2]));
key4_request_irq_fail:
	free_irq(IRQ_GPIO_B_START + 30, (void *)(&key_value[1]));
key3_request_irq_fail:	
	free_irq(IRQ_GPIO_A_START + 28, (void *)(&key_value[0]));
key2_request_irq_fail:
	return ret;
}

static void __exit key_exit(void)
{
	free_irq(IRQ_GPIO_B_START + 9, (void *)(&key_value[3]));
	free_irq(IRQ_GPIO_B_START + 31, (void *)(&key_value[2]));
	free_irq(IRQ_GPIO_B_START + 30, (void *)(&key_value[1]));
	free_irq(IRQ_GPIO_A_START + 28, (void *)(&key_value[0]));
	printk(KERN_WARNING "device key exited\n");
}




//驱动程序出入口:insmod, rmmod
module_init(key_init);
module_exit(key_exit);

//module的描述。#modinfo key_drv.ko
MODULE_AUTHOR("hahaha");
MODULE_DESCRIPTION("key driver for X6818");
MODULE_LICENSE("GPL");
MODULE_VERSION("V1.0");