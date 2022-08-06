#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <dirent.h>

#include <errno.h>

#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>

#include "jpeglib.h"

void print_a_bmp_picture(const char* pathname)
{
	//获取屏幕文件
	int fp1 = open("/dev/fb0", O_RDWR);
	if (fp1 == -1) {
		perror("open fail \n");
		return;
	}
	//获取屏幕长宽和像素点等信息
	struct fb_var_screeninfo fbinfo;
	int ret = ioctl(fp1,FBIOGET_VSCREENINFO,&fbinfo);	
	if (ret == -1) {
		perror("ioctl fail \n");
		return;
	}
	//获取像素点各项参数
	int transp_offset = fbinfo.transp.offset;
	int red_offset = fbinfo.red.offset;
	int green_offset = fbinfo.green.offset;
	int blue_offset = fbinfo.blue.offset;
	int xres = fbinfo.xres;
	int yres = fbinfo.yres;
	//映射，严格按照屏幕大小对其映射即可
    int* plcd = (int*)mmap(NULL, xres * yres * 4, \
		PROT_READ | PROT_WRITE,  MAP_SHARED, fp1, 0);	
	if (plcd == MAP_FAILED) {
		perror("mmap fail \n");
		return; 
	}
	close(fp1);	
	//获取图片文件
	int fp2 = open(pathname, O_RDONLY);
	if (fp2 == -1) {
		perror("open fail \n");
		return;
	}	
	//需要额外求出位深，图片（水平）长度与宽度
	char help_bit[2] = {0};
	lseek(fp2, 28, SEEK_SET);
	read(fp2, help_bit, 2);
	int bit_length = (help_bit[1]<<8) | help_bit[0];	
	lseek(fp2, 18, SEEK_SET);	
	char help_length[4] = {0};		
	read(fp2, help_length, 4);
	int picture_length = (help_length[3]<<24) | (help_length[2] <<16) | \
		(help_length[1] <<8) | help_length[0];		
	read(fp2, help_length, 4);	
	int picture_width = (help_length[3]<<24) | (help_length[2] <<16) | \
		(help_length[1] <<8) | help_length[0];	
	picture_length &= 0x7fffffff;		
	picture_width &= 0x7fffffff;
	//关于非对称4字节长度，可以直接开始便选择采用非对称补偿
	//则补偿后的长度和宽度将代替原有长度与宽度	
	int width = picture_width;		
	int length = picture_length * (bit_length == 24 ? 3 : 4);
	int offset = (bit_length == 24 ? (4 - length % 4) : 0);
	if (length % 4 != 0)
		length += offset;
	printf("%d - %d - %d - %d - %d \n", picture_length, picture_width, \
		bit_length, length, width);
	//buf只是一个颜色数据的集装箱，只需要保证足够大即可
	char *buf = (char*)malloc(sizeof(char) * (length * width + 54));
	lseek(fp2, 0, SEEK_SET);		
	read(fp2, buf, length * width + 54);
	close(fp2);	
		
	//跳过前面部分54字节
	int i, j, bit_offset = (bit_length == 24 ? 3 : 4), x = 54;	
	//以实际图像长宽为准，只不超过屏幕实际大小范围即可
	for (j = width - 1; j > 0; j--) {
		for (i = 0; i < length; i += bit_offset) {
			//出现非对称时，关键在于此处的跳跃操作
			//即跳过出现的补偿部分
			if ((bit_length == 24) && (i + offset == length)) {
				x += offset;
				break;
			}			
			int color = 0;
			if (bit_length == 32) {
				color |= buf[x]<<transp_offset;
				x++;
			}						
			color |= (buf[x]<<blue_offset);
			color |= (buf[x+1]<<green_offset);
			color |= (buf[x+2]<<red_offset);
			x += 3;	
		
			if (i / bit_offset < xres && j < yres)	
				*(plcd + j * xres + i / bit_offset) = color;					
		}
	}
	munmap(plcd,  xres * yres * 4);
	free(buf);
}

void print_a_jpeg_picture(const char* pathname)
{
	//获取屏幕文件
	int fp1 = open("/dev/fb0", O_RDWR);
	if (fp1 == -1) {
		perror("open fail \n");
		return;
	}
	//获取屏幕长宽和像素点等信息
	struct fb_var_screeninfo fbinfo;
	int ret = ioctl(fp1,FBIOGET_VSCREENINFO,&fbinfo);	
	if (ret == -1) {
		perror("ioctl fail \n");
		return;
	}
	//获取像素点各项参数
	int red_offset = fbinfo.red.offset;
	int green_offset = fbinfo.green.offset;
	int blue_offset = fbinfo.blue.offset;
	int transp_offset = fbinfo.transp.offset;
	int xres = fbinfo.xres;
	int yres = fbinfo.yres;
	
	//关于非对称4字节长度，可以直接开始便选择采用非对称补偿
	if (xres % 4 != 0)
		xres += (4 - xres%4);
	
    int* plcd = (int*)mmap(NULL, xres * yres * 4, \
		PROT_READ | PROT_WRITE,  MAP_SHARED, fp1, 0);	
	if (plcd == MAP_FAILED) {
		perror("mmap fail \n");
		return;
	}
	
	//声明一个解压的对象
	struct jpeg_decompress_struct dinfo;
	//声明一个出错信息的对象	
	struct jpeg_error_mgr jerr;
	//初始化这个出错对象	
	dinfo.err = jpeg_std_error(&jerr);	
	//初始化dinfo这个解压对象	
	jpeg_create_decompress(&dinfo);
	
	FILE *fp = fopen(pathname, "r");
	if (fp == NULL) {
		perror("fopen error:");
		return;
	}
	 //指定要解压的图像文件
	jpeg_stdio_src(&dinfo, fp);	
	//调用jpeg_read_header()获取图像信息
	jpeg_read_header(&dinfo, TRUE);	
	//调用jpeg_start_decompress()启动解压过程
	jpeg_start_decompress(&dinfo);
		
	//读取一行或者多行扫描线上数据并处理，通常的代码是这样子的:
	unsigned char *buffer = (unsigned char *)malloc( dinfo.output_height *\
		dinfo.output_width * dinfo.output_components);
	
	//dinfo.output_scanline , 表示的意思是，已经扫描了多少行	
	while (dinfo.output_scanline < dinfo.output_height) {
		//解压对象//保存解压后数据的二级指针,//读取多少行数据来解压 
		int y = dinfo.output_scanline ;
		jpeg_read_scanlines(&dinfo, &buffer, 1);	
		//dinfo.output_scanline + 1
		int offset = 0, x, color;
		for (x = 0; x < dinfo.output_width; x++)
		{
			//A - R - G - B			
			int color = 0;
			if (dinfo.output_components != 3) {				
				color = buffer[offset]<<transp_offset;
				offset++;				
			}
			
			color |= (buffer[offset+1]<<red_offset);
			color |= (buffer[offset+2]<<green_offset);
			color |= (buffer[offset+3]<<blue_offset);
			offset +=3;			
				
			if (y < yres && x <xres)
				*(plcd +  y * xres + x) = color;
			
		}
	}
	
	//调用jpeg_finish_decompress()完成解压过程	
	jpeg_finish_decompress(&dinfo);
	//调用jpeg_destroy_decompress释放jpeg解压对象dinfo	
	jpeg_destroy_decompress(&dinfo);

	close(fp1);
	munmap(plcd,  xres * yres * 4);	
	free(buffer);
}

//匹配str1的尾部中与str2相同的，扩展名以.结束
static int help_compare_extension(const char * str1, const char * str2)
{
	const char * temp1 = str1;
	const char * temp2 = str2;	
	while (*temp1 != '\0')
		temp1++;
	temp1--;
	while (*temp2 != '\0')
		temp2++;
	temp2--;
	
	while (temp1 >= str1 && temp2 >= str2 && \
		*temp2 != '.' && *temp1 != '.')
		if (*temp1 != *temp2)
			return 0;
		else {
			temp1--;
			temp2--;			
		}
		if (*temp1 == *temp2)
			return 1;
		return 0;
}

//辅助保存路径，所以只实现需要的部分
struct list{
	char* data;
	struct list* prev, * next;
};

struct list* insert_node(struct list* list, const char* data)
{
	char *buf_str = (char *)malloc(sizeof(char) * 1024);
	strcpy(buf_str, data);
	struct list* buf_list = (struct list*)malloc(sizeof(struct list));
	buf_list->data = buf_str;
	buf_list->prev = NULL;
	buf_list->next = NULL;
	
	//头插法
	if (list != NULL) {
		struct list* help = list;
		help->prev = buf_list;
		buf_list->next = help;		
	}
	return buf_list;	
}

void delete_list(struct list* list)
{
	struct list* help = list;
	while (help != NULL) {
		list = list->next;		
		free(help->data);
		free(help);
		help = list;
	}
}

void print_list(struct list* list)
{
	while (list != NULL) {
		printf("%s\n", list->data);
		if (help_compare_extension(list->data, ".jpg"))
			print_a_jpeg_picture(list->data);
		if (help_compare_extension(list->data, ".bmp"))
			print_a_bmp_picture(list->data);	
		sleep(1);
		list = list->next;
	}		
}

//该算法严重依赖进程工作路径,所以相对到其他地方不可行
void change_pathname(char * pathsource, char * pathzone, int num)
{
	//如果传入的第一个字节为/默认是绝对路径，无需修改
	if (*pathsource == '/')
		return;
	//传入的是相对路径，先获取进程当前路径，存在pathzone中
	char * pathname = getcwd(pathzone, num);
	//将剩余部分链接到pathzone后面，准备处理
	strcat(pathzone,pathsource);
	//对路径默认有一个很大的数组
	printf("%s \n", pathzone);
	char*help = pathzone;
	
	//其中，关于点号.只需要直接将其后面的部分与之连接即可
	//关于点点号..需要移除中间目录
	//经典情况如下：./（直接覆盖）.（直接覆盖）./.（直接覆盖）
	// .. 有关的部分一般情况是   /xxx/..   /xxx.. 
	//（先覆盖..前一个字节防止/干扰，然后从前一/开始一直覆盖到..为止）
	//
	while (*help !='\0') {
		//出现第一个.
		if (*help == '.') {
			*help = ' ';				
			if (*(help + 1) == '/')	
				*(help + 1) = ' ';
			else if (*(help + 1) == '.') {
				//出现连续的..需要对前面可能至多俩个/内的内容进行覆盖
				if (help - 1 >= pathzone)
					*(help - 1) = ' ';
				char * p = help;
				while (p >= pathzone && *p != '/')
					p--;
				while (p <= help + 1) {
					*p = ' ';
					p++;
				}		
			}			
		}
		help++;
	}
	
	printf("%s \n", pathzone);
	//处理空格键，将字符串中空格全部清除掉
	char* p = pathzone;
	while (*p != '\0') {
		if (*p == ' ') {
			char* q = p;
			while (*q != '\0' && *q == ' ')
				q++;
			if (*q == '\0') {
				*p = '\0';				
				break;
			}
			char *q1 = p;
			while (*q != '\0' && *q != ' ') {
				*q1 = *q;				
				q++;
				q1++;
			}
		}
		p++;
	}
	//再做一次检查，清除尾部的空格键和/
	while(p >= pathzone && (*p == '\0' || *p == ' ' || *p == '/')) {
		*p = '\0';
		p--;
	}
	//如果出现极端情况，清除了根目录，补充它
	if (*pathzone != '/') {
		*pathzone = '/';		
		//当根目录都被删除的时候，整个路径完全出现问题
		//则保留根目录即可，或者直接清除整个路径亦可
		*(pathzone + 1) = '\0';
	}

	
	printf("%s \n", pathzone);	
}

//flag为0，统计文件数量，为1，统计文件大小总和
//flag为2，统计特定文件数量，已及相对路径（若传入参数是相对路径）
//或者绝对路径（若传入参数是绝对路径）
//而相对路径本身与当前进程的工作路径有关，所以可以使用getwd提前做一步处理
//使得传入时的参数为绝对路径，则保存的也会是绝对路径,最好选择外层再封装一次
unsigned int statistics_file_infomation(const char *pathname, int flag, struct list** mylist)
{
	struct stat st;
	int ret = stat(pathname,&st);
	//失败了
	if (ret == -1) {
		perror("stat fail\n");
		errno = 0;
		return 0;				
	} if (!S_ISDIR(st.st_mode)) {
		//是否为目录,不是目录默认是普通文件	
		if (flag == 0)
			return 1;				
		if (flag == 1)
			return st.st_size;
		//修改此处str2，增添判断语句等皆可
		if (flag == 2) {
			if (help_compare_extension(pathname, ".jpg")  || \
				help_compare_extension(pathname, ".bmp")) {
				*mylist = insert_node(*mylist, pathname);				
				return 1;				
			}
			return 0;
		}
	}
	
	unsigned int all_number = 0;	
	DIR * buf = opendir(pathname);
	//打开目录失败
	if (buf == NULL && errno != 0) {
		perror("dir open fail\n");
		errno = 0;
		return 0;		
	} else {
		//遍历整个该目录，对每个目录项进行分析
		while (1) {
			struct dirent * help = readdir(buf);
			//该项失败，无法使用，跳到下一个
			if (help == NULL && errno != 0) {
				perror("entry fail\n");
				errno = 0;				
			} else if (help == NULL && errno == 0)
			//目录项被寻找完成				
				break;
			else {
				//排除.和..
				if (!(0 == strcmp(help->d_name, ".") || \
					0 == strcmp(help->d_name, ".."))) {
					//构建下一级路径
					char cmd[1024] = {0};
					strcat(cmd, pathname);
					//如果最后一个字节是/即出现根目录时
					char *p = cmd;
					while(*p != '\0')
						p++;
					p--;
					if (*p != '/')
						strcat(cmd, "/");
					strcat(cmd, help->d_name);
					all_number += statistics_file_infomation(cmd, flag, mylist);													
				}								
			}						
		}		
	}
	closedir(buf);
	return all_number;
}

int main(int argc, char* argv[])
{
	struct list** list = NULL;
	struct list* list_buf = NULL;
	list = &list_buf;
	
	char path[1024] = {0};
	
	//为表示简单，封装相对路径向绝对路径的转化		
	//change_pathname(argv[1], path, 1024);
	//相对路径是相对于进程所在路径的
	//readlink( "/proc/self/exe", path, 1024); 
	//相对路径是相对于当前工作路径的
	realpath(argv[1],path);
	
	printf("%d\n", statistics_file_infomation(path, 2, list));
	print_list(*list);
	delete_list(*list);
	
	return 0;
}
