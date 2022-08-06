#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/ioctl.h>

#define X6818_ADC_SETCHANNEL	_IOW('A', 1, unsigned long)
#define X6818_ADC_GETDAT	    _IOR('A', 2, unsigned long)

int main(int argc, char* argv[])
{
	int fd,ret;
	unsigned int adc_data = 0;
	fd = open(argv[1], O_RDWR);
	if(fd < 0) {
		perror("open fail\n");
		return -1;		
	}

	ret = ioctl(fd, X6818_ADC_SETCHANNEL, 0);
	if(ret < 0)
		perror("ioctl\n");
		
	while(1)
	{
		ret = ioctl(fd, X6818_ADC_GETDAT, &adc_data);
		if(ret < 0)
			perror("ioctl\n");
	
		printf("adc_data = %lu\n", adc_data);
		usleep(200 * 1000);		
	}
	
	close(fd);
	return 0;
}
