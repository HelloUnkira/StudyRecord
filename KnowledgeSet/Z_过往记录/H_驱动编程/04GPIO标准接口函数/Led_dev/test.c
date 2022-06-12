#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <sys/ioctl.h>
//定义灯亮灯灭的命令，模拟定义
#define X6818_LED_ON	_IOW('L', 1, unsigned long)
#define X6818_LED_OFF	_IOW('L', 2, unsigned long)
#define X6818_LED_STATE _IOR('L', 3, unsigned char*)



int main(int argc, char* argv[])
{
	int fd, ret;
	char led_read[4] = {0};
	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("open fail\n");
		return -1;
	}
	
	int i = 0;	
	while (1) {
		for (i = 7; i <= 10; i++) {	
			ret = ioctl(fd, X6818_LED_STATE, led_read);
			if (ret < 0)
				perror("ioctl fail\n");
			else
				printf("%d %d %d %d \n", led_read[0], led_read[1], led_read[2], led_read[3]);
			
			ret = ioctl(fd, X6818_LED_ON, i);
			if (ret < 0)
				perror("ioctl fail\n");
			
			usleep(300 * 1000);
			
			ret = ioctl(fd, X6818_LED_STATE, led_read);
			if (ret < 0)
				perror("ioctl fail\n");
			else
				printf("%d %d %d %d \n", led_read[0], led_read[1], led_read[2], led_read[3]);			
			
			ret = ioctl(fd, X6818_LED_OFF, i);
			if (ret < 0) 
				perror("ioctl fail\n");
			
			usleep(300 * 1000);			
		}
	}
	
	close(fd);
	return 0;
}