#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

int get_abs_event()
{
	int fd = open("/dev/input/event0",O_RDONLY);
	if(fd == -1)
	{
		perror("open error");
		return;
	}
	
	struct input_event ev;
	int x[2] = {0};
	int y[2] = {0};	
	int count = 10;
	while (1) {
		int ret = read(fd, &ev, sizeof(ev));
		if(ret != sizeof(ev))
			continue;
		
		if ((ev.type == EV_ABS)  && (ev.code == ABS_X)) {
			x[1] = ev.value;			
			if (count == 10) {
				x[0] = x[1];
			}
		}
		if ((ev.type == EV_ABS)  && (ev.code == ABS_Y)) {
			y[1] = ev.value;				
			if (count == 10) {
				y[0] = y[1];
			}
		}
		if (count++ >10)
			count = 0;
	
		if(ev.type == EV_KEY && ev.code == BTN_TOUCH && ev.value == 0) {
			break;
		}
		
		usleep(1000);
	}
	
	close(fd);			
	//printf("%d - %d - %d - %d\n", x[0], x[1], y[0], y[1]);
	
	//向上，表示1
	if ((y[0] > y[1]) && abs(y[0] - y[1]) >= abs(x[0] - x[1]))
		return 1;
	//向下，表示2
	if ((y[0] < y[1]) && abs(y[0] - y[1]) >= abs(x[0] - x[1]))
		return 2;	
	//向左，表示3	
	if ((x[0] > x[1]) && abs(y[0] - y[1]) <= abs(x[0] - x[1]))
		return 3;
	//向右，表示4
	if ((x[0] < x[1]) && abs(y[0] - y[1]) <= abs(x[0] - x[1]))
		return 4;	
	
	return 0;
}



int main()
{
	
	//printf("%d \n", get_abs_event());
	return 0;
}
