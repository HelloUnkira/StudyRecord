#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ioctl.h>

#define X6818_WATCHDOG_START		_IO('W',  1)
#define X6818_WATCHDOG_STOP			_IO('W',  2)
#define X6818_WATCHDOG_ALIVE		_IO('W',  3)

int main(int argc, char* argv[])
{
	int fd,ret,i=0;
	fd = open(argv[1], O_RDWR);
	if(fd < 0)
	{
		perror("open fail\n");
		return -1;		
	}

	ret = ioctl(fd, X6818_WATCHDOG_START);
	if(ret < 0)
		perror("ioctl\n");

	do
	{
		printf("%d second\n",++i);
		sleep(1);		
	} while (i < 8);
	
	ret = ioctl(fd, X6818_WATCHDOG_ALIVE);
	if(ret < 0)
		perror("ioctl\n");	
	
	i = 0;
	while(1)
	{
		printf("%d second\n",++i);
		sleep(1);		
	}
	
	close(fd);
	return 0;
}
