#include <stdio.h>

#include <stdlib.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	int fd, ret;
	char led_read[4] = {0};
	char led_ctrl[2];
	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("open fail\n");
		return -1;
	}
	int i = 5;
	while (i-- > 0) {
		led_ctrl[1] = 7; led_ctrl[0] = 1;
		ret = read(fd, led_read, sizeof(led_read));
		if (ret == 4) {
			printf("%d %d %d %d\n",led_read[3],led_read[2],led_read[1],led_read[0]);
		}
		ret = write(fd, led_ctrl, sizeof(led_ctrl));
		if (ret != 2) {
			perror("write fail\n");
		}
		
		usleep(300 * 1000);
		
		led_ctrl[1] = 7; led_ctrl[0] = 0;
		ret = read(fd, led_read, sizeof(led_read));
		if (ret == 4) {
			printf("%d %d %d %d\n",led_read[3],led_read[2],led_read[1],led_read[0]);
		}
		ret = write(fd, led_ctrl, sizeof(led_ctrl));
		if (ret != 2) {
			perror("write fail\n");
		}		
		
		usleep(300 * 1000);
	}
	
	i = 5;
	while (i-- > 0) {
		led_ctrl[1] = 8; led_ctrl[0] = 1;
		ret = read(fd, led_read, sizeof(led_read));
		if (ret == 4) {
			printf("%d %d %d %d\n",led_read[3],led_read[2],led_read[1],led_read[0]);
		}
		ret = write(fd, led_ctrl, sizeof(led_ctrl));
		if (ret != 2) {
			perror("write fail\n");
		}
		
		usleep(300 * 1000);
		
		led_ctrl[1] = 8; led_ctrl[0] = 0;
		ret = read(fd, led_read, sizeof(led_read));
		if (ret == 4) {
			printf("%d %d %d %d\n",led_read[3],led_read[2],led_read[1],led_read[0]);
		}
		ret = write(fd, led_ctrl, sizeof(led_ctrl));
		if (ret != 2) {
			perror("write fail\n");
		}		
		
		usleep(300 * 1000);
	}
	
	i = 5;
	while (i-- > 0) {
		led_ctrl[1] = 9; led_ctrl[0] = 1;
		ret = read(fd, led_read, sizeof(led_read));
		if (ret == 4) {
			printf("%d %d %d %d\n",led_read[3],led_read[2],led_read[1],led_read[0]);
		}
		ret = write(fd, led_ctrl, sizeof(led_ctrl));
		if (ret != 2) {
			perror("write fail\n");
		}
		
		usleep(300 * 1000);
		
		led_ctrl[1] = 9; led_ctrl[0] = 0;
		ret = read(fd, led_read, sizeof(led_read));
		if (ret == 4) {
			printf("%d %d %d %d\n",led_read[3],led_read[2],led_read[1],led_read[0]);
		}
		ret = write(fd, led_ctrl, sizeof(led_ctrl));
		if (ret != 2) {
			perror("write fail\n");
		}		
		
		usleep(300 * 1000);
	}
	
	i = 5;
	while (i-- > 0) {
		led_ctrl[1] = 10; led_ctrl[0] = 1;
		ret = read(fd, led_read, sizeof(led_read));
		if (ret == 4) {
			printf("%d %d %d %d\n",led_read[3],led_read[2],led_read[1],led_read[0]);
		}
		ret = write(fd, led_ctrl, sizeof(led_ctrl));
		if (ret != 2) {
			perror("write fail\n");
		}
		
		usleep(300 * 1000);
		
		led_ctrl[1] = 10; led_ctrl[0] = 0;
		ret = read(fd, led_read, sizeof(led_read));
		if (ret == 4) {
			printf("%d %d %d %d\n",led_read[3],led_read[2],led_read[1],led_read[0]);
		}
		ret = write(fd, led_ctrl, sizeof(led_ctrl));
		if (ret != 2) {
			perror("write fail\n");
		}		
		
		usleep(300 * 1000);
	}
	
	close(fd);
	return 0;
}