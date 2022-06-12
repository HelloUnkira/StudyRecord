#include <stdio.h>

#include <stdlib.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#define X6818_BEEP_ON	_IO('B', 1)
#define X6818_BEEP_OFF	_IO('B', 2)

int main(int argc, char* argv[])
{
	int fd, ret;
	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("open fail\n");
		return -1;
	}
	while (1) {
		ret = ioctl(fd, X6818_BEEP_ON);
		if (ret < 0) {
			perror("ioctl fail\n");
		}
			
		usleep(300 * 1000);
		
		ret = ioctl(fd, X6818_BEEP_OFF);
		if (ret < 0)
			perror("ioctl fail\n");
		
		
		usleep(300 * 1000);
	}
	close(fd);
	return 0;
}