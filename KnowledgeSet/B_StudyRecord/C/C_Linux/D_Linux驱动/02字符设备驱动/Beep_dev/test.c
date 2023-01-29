#include <stdio.h>

#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int fd, ret;
	char led_read;
	char led_ctrl;
	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("open fail\n");
		return -1;
	}
	
	while (1) {
		led_ctrl = 1;
		ret = read(fd, &led_read, sizeof(led_read));
		if (ret == 1) {
			printf("%d\n",led_read);
		}
		ret = write(fd, &led_ctrl, sizeof(led_ctrl));
		if (ret != 1) {
			perror("write fail\n");
		}
		
		usleep(300 * 1000);
		
		led_ctrl = 0;
		ret = read(fd, &led_read, sizeof(led_read));
		if (ret == 1) {
			printf("%d\n",led_read);
		}
		ret = write(fd, &led_ctrl, sizeof(led_ctrl));
		if (ret != 1) {
			perror("write fail\n");
		}		
		
		usleep(300 * 1000);
	}
	
	
	
	close(fd);
	return 0;
}