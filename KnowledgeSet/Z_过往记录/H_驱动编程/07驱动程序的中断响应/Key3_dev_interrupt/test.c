#include <stdio.h>#include <stdlib.h>#include <fcntl.h>#include <sys/ioctl.h>//定义读取Key的命令，模拟定义#define X6818_KEY_STATE _IOR('K', 1, unsigned char*)int main(int argc, char* argv[]){	int fd, ret;		fd = open(argv[1], O_RDWR);	if (fd < 0) {		perror("open fail\n");		return -1;	}		int i = 0;		while (1) {				char key_value[5] = {0};		int ret;		while (key_value[0] == 0)			ret = ioctl(fd, X6818_KEY_STATE, key_value);		if (ret < 0)			perror("ioctl fail\n");		else			printf("%d %d %d %d\n", key_value[1], key_value[2], \				key_value[3], key_value[4]);	}		close(fd);	return 0;}