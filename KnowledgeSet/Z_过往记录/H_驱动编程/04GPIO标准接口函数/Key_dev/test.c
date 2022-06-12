#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <sys/ioctl.h>
//定义读取Key的命令，模拟定义
#define X6818_KEY_STATE _IOR('K', 1, unsigned char*)

int main(int argc, char* argv[])
{
	int fd, ret;	
	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("open fail\n");
		return -1;
	}
	
	int i = 0;	
	while (1) {		
		char key_value[4] = {0};
		if (ioctl(fd, X6818_KEY_STATE, key_value) < 0)
			perror("ioctl fail\n");
		if (key_value[1] == 0) {
			usleep(10 * 1000);
			if (ioctl(fd, X6818_KEY_STATE, key_value) < 0)
				perror("ioctl fail\n");
			if (key_value[1] == 0) {
				if (i == 0) {
					i = 1;
					printf("0\n");
				}					
			} else 
				i = 0;
		} else 
			i = 0;
	}
	
	close(fd);
	return 0;
}