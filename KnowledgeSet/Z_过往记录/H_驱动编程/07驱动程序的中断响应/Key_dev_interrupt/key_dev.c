#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <cfg_type.h>

#include <linux/interrupt.h>

irqreturn_t key2_irq_handler_t(int irq, void *dev)
{
	printk(KERN_WARNING "key2 is pressing, irq = %d\n", irq);
	return IRQ_HANDLED;
}

irqreturn_t key3_irq_handler_t(int irq, void *dev)
{
	printk(KERN_WARNING "key3 is pressing, irq = %d\n", irq);
	return IRQ_HANDLED;
}

irqreturn_t key4_irq_handler_t(int irq, void *dev)
{
	printk(KERN_WARNING "key4 is pressing, irq = %d\n", irq);
	return IRQ_HANDLED;
}

irqreturn_t key6_irq_handler_t(int irq, void *dev)
{
	printk(KERN_WARNING "key6 is pressing, irq = %d\n", irq);
	return IRQ_HANDLED;
}

static int __init key_init(void)
{
	int ret;
	ret = request_irq(IRQ_GPIO_A_START + 28, key2_irq_handler_t, \
		IRQF_TRIGGER_FALLING, "Key2_GpioA28", NULL);
	if (ret < 0) {
		printk(KERN_WARNING "Key2 Interrupt request fail");
		goto key2_request_irq_fail;
	}	
	
	ret = request_irq(IRQ_GPIO_B_START + 30, key3_irq_handler_t, \
		IRQF_TRIGGER_FALLING, "Key3_GpioB30", NULL);	
	if (ret < 0) {
		printk(KERN_WARNING "Key3 Interrupt request fail");
		goto key3_request_irq_fail;
	}
	
	ret = request_irq(IRQ_GPIO_B_START + 31, key4_irq_handler_t, \
		IRQF_TRIGGER_FALLING, "Key4_GpioB31", NULL);
	if (ret < 0) {
		printk(KERN_WARNING "Key4 Interrupt request fail");
		goto key4_request_irq_fail;
	}
	
	ret = request_irq(IRQ_GPIO_B_START + 9, key6_irq_handler_t, \
		IRQF_TRIGGER_FALLING, "Key6_GpioB9", NULL);		
	if (ret < 0) {
		printk(KERN_WARNING "Key6 Interrupt request fail");
		goto key6_request_irq_fail;
	}


	
	printk(KERN_WARNING "device key inited\n");
	return 0;
	
	
key6_request_irq_fail:
	free_irq(IRQ_GPIO_B_START + 31, NULL);
key4_request_irq_fail:
	free_irq(IRQ_GPIO_B_START + 30, NULL);
key3_request_irq_fail:	
	free_irq(IRQ_GPIO_A_START + 28, NULL);
key2_request_irq_fail:
	return ret;
}

static void __exit key_exit(void)
{
	free_irq(IRQ_GPIO_B_START + 9, NULL);
	free_irq(IRQ_GPIO_B_START + 31, NULL);
	free_irq(IRQ_GPIO_B_START + 30, NULL);
	free_irq(IRQ_GPIO_A_START + 28, NULL);
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