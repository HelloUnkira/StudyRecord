//点灯程序

//以下定义查数据手则
#define GPIOEOUT 	(*(volatile unsigned int *)0xC001E000)
#define GPIOEOUTENB (*(volatile unsigned int *)0xC001E004)
#define GPIOEALTFN0 (*(volatile unsigned int *)0xC001E020)
#define GPIOEALTFN1 (*(volatile unsigned int *)0xC001E024)

void delay(int val);

void _start(void) //C程序入口，源文件开始编写，裸机
{
	GPIOEALTFN0 &= ~(3<<26);
	GPIOEOUTENB |= (1<<13);
	
	while (1) {
		GPIOEOUT &= ~(1<<13);
		delay(0x40000000);
		
		GPIOEOUT |= (1<<13);
		delay(0x40000000);
	}
	
}

void delay(int val)
{
	volatile int i = val;
	while((i--) != 0);
}

