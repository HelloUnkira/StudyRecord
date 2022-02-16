#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "sort.h"

#define max_int 0x7fffffff
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

//仅判断是否有序以及数据个数
int test_file_data_have_fault(FILE*fp)
{
	//遍历fp，保证每一个前小于后
	int x = 0 , y = 0, data = 0, all_file_number = 0;
	fseek(fp, 0, SEEK_SET);
	while (!feof(fp)) {
		if (fscanf(fp, "%d ", &data) > 0) {
			x = y;
			y = data;
			if (x > y)
				printf("sort fail about: %d - %d\n", x, y);
			all_file_number++;
		}
	}
	return all_file_number;
}

//整理文件至每行10个
int tidy_file(FILE* fp)
{
	char cmd[1024] = {0};
	int i = 0, data, ret, count = 1;	
	FILE* fpbuf = fopen("help.txt", "w+");
	int all_data = test_file_data_have_fault(fp);
	fseek(fp, 0, SEEK_SET);	
	while (!feof(fp)) {
		for (i = 0; i < 10; i++) 
			if (!feof(fp)) {
				if (fscanf(fp, "%d ", &data) > 0) 
					fprintf(fpbuf, "%d ", data);				
			} else
				break;
		fprintf(fpbuf, "\n");
	}
	
	fseek(fp, 0, SEEK_SET);
	fseek(fpbuf, 0, SEEK_SET);
	while((count = fread(cmd, sizeof(char), 1024, fpbuf)) > 0) 
		fwrite(cmd, sizeof(char), count, fp);
	
	ret = fclose(fpbuf);
	if (ret == -1) {
		perror("file close fail\n");
		return 0;				
	}				
	remove("help.txt");
	return all_data;		
}

//只单纯使用快排的思想分治显然不满足，甚至于数据出现不稳定的情况
//所以采用彻底的分治，将大文件分成多个小文件，其中对多个小文件内部进行
//快速排序

//对一个小文件排序,此处为内排序
static void little_file_quick_sort(FILE* fp)
{
	//计算文件大小
	int start = 0, end = 0;
	fseek(fp, 0, SEEK_SET);
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	int* buffer = (int*)malloc(sizeof(int) * size);
	if (buffer == NULL)
		printf("malloc fail\n");
	fseek(fp, 0, SEEK_SET);
	while(!feof(fp)) 
		fscanf(fp, "%d ", &buffer[end++]);
	
	//选取一种内存快速排序
	//quick_sort(buffer, start, end - 1);		
	//insertion_sort(buffer, start, end - 1);		
	//bubble_sort(buffer, start, end - 1);
	//counting_sort(buffer, start, end - 1);
	//heap_sort(buffer, end);
	//radix_sort(buffer, start, end - 1, 9);
	//selection_sort(buffer, start, end - 1);
	//shell_sort(buffer, start, end - 1);
	single_bucket_sort(buffer, start, end - 1);
	
	fseek(fp, 0, SEEK_SET);
	while (start < end) 
		fprintf(fp, "%d ", buffer[start++]);
	free(buffer);
}

//俩路归并排序，设置好标签，即可保证遍历过去不错过数据
static void two_file_merge_sort(FILE* fp1, FILE* fp2)
{
	int buf1 = 0, buf2 = 0, data = 0;
	bool buf1_used = true, buf2_used = true;
	FILE* fp_help = fopen("needhelp.txt", "w+");
	fseek(fp_help, 0, SEEK_SET);
	fseek(fp1, 0, SEEK_SET);
	fseek(fp2, 0, SEEK_SET);	
	while ((!feof(fp1) || buf1_used == false) || \
			(!feof(fp2) || buf2_used == false)) {
		if (!feof(fp1) && buf1_used == true) {
			fscanf(fp1, "%d ", &buf1);
			buf1_used = false;
		}
		if (!feof(fp2) && buf2_used == true) {
			fscanf(fp2, "%d ", &buf2);
			buf2_used = false;
		}
		if (buf1_used == false && buf2_used == false) {
			if (buf1 <= buf2) {
				fprintf(fp_help, "%d ", buf1);
				buf1_used = true;
			} else {
				fprintf(fp_help, "%d ", buf2);
				buf2_used = true;
			}			
		} else if (buf1_used == false) {
			fprintf(fp_help, "%d ", buf1);
			buf1_used = true;			
		} else if (buf2_used == false) {
			fprintf(fp_help, "%d ", buf2);
			buf2_used = true;			
		} else
			break;
	}
	
	while (!feof(fp1)) 
		if (fscanf(fp1, "%d ", &buf1) > 0)
			fprintf(fp_help, "%d ", buf1);
	
	while (!feof(fp2)) 
		if (fscanf(fp2, "%d ", &buf2) > 0)
			fprintf(fp_help, "%d ", buf2);
	

	fseek(fp_help, 0, SEEK_SET);
	fseek(fp1, 0, SEEK_SET);	
	while (!feof(fp_help)) {
		fscanf(fp_help, "%d ", &data);
		fprintf(fp1, "%d ", data);
	}
	int ret = fclose(fp_help);
	if (ret == -1) 
		perror("file close fail\n");	
	remove("needhelp.txt");
}

//败者树
static void adjust_node(int* leave, int* node, int n, int i)
{
	int parent=(i + n - 1) / 2;      /* 求出父节点的下标 */
	while (parent > 0) {
		if (leave[i] > leave[node[parent]]) {
			int temp = node[parent];
			node[parent]=i;
			/* i指向的是优胜者 */
			i= temp;
		}
		parent = parent / 2;
	}
	node[0] = i;
}

static void init_min_data(int* leave, int* node, int n)
{
	//初始化node，利用leave
	//其中leave的范围是n，node的范围是n - 1
	int i = 0;
	 for(i = n; i > 0; i--)
		adjust_node(leave, node, n, i);
}

//多路归并排序,此处采用俩路归并排序,采用多路归并排序机制一样
void multicannel_merge_file_sort_1(int n, ...)
{
	//读取需要归并的文件，数量为n，
	//将其指针描述符存入指针数组之中
	int i, data, all_used = 0;
	va_list ap;
	va_start(ap, n);
	FILE* fp_help = fopen("needhelp.txt", "w+");	
	FILE** fp_all = (FILE**)malloc(sizeof(FILE*) * (n + 1));
	int* leave = (int*)malloc(sizeof(int) * (n + 1));
	int* node = (int*)malloc(sizeof(int) * n);
	for(fp_all[0] = NULL, leave[0] = 0, i = 1; \
		i < n + 1; i++) {
        fp_all[i] = va_arg(ap, FILE*);
		fseek(fp_all[i], 0, SEEK_SET);
		if (fscanf(fp_all[i], "%d ", &data) > 0) 
			leave[i] = data;					
		 else 
			leave[i] = max_int;
		node[i - 1] = 0;
	}
	va_end(ap);
	//创建大小为对应数量的树集
	init_min_data(leave, node, n);
	//用每一个，然后就是求值，存值即可了
	fseek(fp_help, 0, SEEK_SET);
	while (leave[node[0]] != max_int) {
		//不断地，从最小树集中取出最小值，取完后更新最小值，调整即可
		//注意到fp_all与leave是相对应的，
		int index = node[0];
		//取出最小值，放入缓存文件
		fprintf(fp_help, "%d ", leave[index]);
		//更新最小值
		if (!feof(fp_all[index]) && \
			fscanf(fp_all[index], "%d ", &data) > 0)	
			leave[index] = data;
		else 
			leave[index] = max_int;		
		//调整该结点
		adjust_node(leave, node, n, index);
	}
	//数据重写至fp_all[1]
	fseek(fp_all[1], 0, SEEK_SET);
	fseek(fp_help, 0, SEEK_SET);
	if (!feof(fp_help))
		if (fscanf(fp_help, "%d ", &data) > 0)
			fprintf(fp_all[1], "%d ", data);
		
	printf("%d\n", tidy_file(fp_help));
	int ret = fclose(fp_help);
	if (ret == -1) 
		perror("file close fail\n");	
	remove("needhelp.txt");	
	
	free(fp_all);
	free(leave);
	free(node);
}

//和上一函数功能一致，只是参数不同而已
void multicannel_merge_file_sort_2(FILE** fp_all, int left, int right)
{
	//读取需要归并的文件，数量为n = right - left + 1，
	//将其指针描述符存入指针数组之中
	int i, data, n = right - left + 1;
	FILE* fp_help = fopen("needhelp.txt", "w+");	
	int* leave = (int*)malloc(sizeof(int) * (n + 1));
	int* node = (int*)malloc(sizeof(int) * n);
	for(leave[0] = 0, i = 1; i < n + 1; i++) {
		fseek(fp_all[left + i - 1], 0, SEEK_SET);
		if (fscanf(fp_all[left + i - 1], "%d ", &data) > 0) 
			leave[i] = data;					
		 else 
			leave[i] = max_int;
		node[i - 1] = 0;
	}
	//创建大小为对应数量的树集
	init_min_data(leave, node, n);
	//用每一个，然后就是求值，存值即可了
	fseek(fp_help, 0, SEEK_SET);
	while (leave[node[0]] != max_int) {
		//不断地，从最小树集中取出最小值，取完后更新最小值，调整即可
		//注意到fp_all与leave是相对应的，
		int index = node[0];
		//取出最小值，放入缓存文件
		fprintf(fp_help, "%d ", leave[index]);
		//更新最小值
		if (!feof(fp_all[left + index - 1]) && \
			fscanf(fp_all[left + index - 1], "%d ", &data) > 0)	
			leave[index] = data;
		else 
			leave[index] = max_int;		
		//调整该结点
		adjust_node(leave, node, n, index);
	}
	//数据重写至fp_all[left]
	fseek(fp_help, 0, SEEK_SET);
	fseek(fp_all[left], 0, SEEK_SET);	
	while (!feof(fp_help))
		if (fscanf(fp_help, "%d ", &data) > 0)
			fprintf(fp_all[left], "%d ", data);

	
	int ret = fclose(fp_help);
	if (ret == -1) 
		perror("file close fail\n");	
	remove("needhelp.txt");	
	
	free(leave);
	free(node);
}
//将大文件分割成小文件，其中小文件采用help_xxx.txt的命名格式
//然后对小文件内排序，最后对大文件实行俩路归并排序
//最后只剩下一个文件
void devide_and_conquer_file_sort(FILE* fp, int max_zone)
{
	char file_name[1024] = {0};		
	int size, number = 0, length = 0, data = 0;
	unsigned int max_length = max_zone != 0 ? \
		max_zone : 1024 * 1024 * 1024;
	int i = 0, all_file_number = 0;
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);	
	//预计一个小文件的话，大概1Mbit

	while (!feof(fp)) {
		//获取小文件名字
		sprintf(file_name, "help_%d.txt", number++);
		FILE* fp_buf = fopen(file_name,"w+");
		length =0;
		while (!feof(fp) && length < max_length) {
			int t = fscanf(fp, "%d ", &data);
			if (t > 0) {
				fprintf(fp_buf, "%d ", data);
				length += t;				
			}
		}

		//对小文件排好序
		little_file_quick_sort(fp_buf);
		int ret = fclose(fp_buf);
		if (ret == -1)
			perror("file close fail\n");
	}
	
	//准备一些需要的数据
	FILE**all = (FILE**)malloc(sizeof(FILE*) * number);
	bool* used_file = (bool*)malloc(sizeof(bool) * number);
	for (i = 0; i < number; i++) {
		sprintf(file_name, "help_%d.txt", i);
		FILE* fp_buf = fopen(file_name,"r+");
		all[i] = fp_buf;
		used_file[i] = 0;	
	}
	//现在所有的小文件都已经排好序了，这些小文件一共有
	//number个，其名字标号为help_0.txt --- help_(number-1).txt
	all_file_number = number;
	
	while (all_file_number > 1) {
		//按顺序取俩个小文件，归并排序
		for (i = 0; i < number; i++) {	
			if (used_file[i] != 0)
				continue;
			else {
				//从i + 1开始取一个未使用文件
				int j = i + 1;
				for (j = i + 1; j < number; j++) 
					if (used_file[j] == 0)
						break;
				if (j >= number)
					break;
				else {
					two_file_merge_sort(all[i], all[j]);
					used_file[j] = 1;
					all_file_number--;
				}
			}
		}
	}

	fseek(fp, 0, SEEK_SET);	
	//最后的结果存放在all[0]中
	fseek(all[0], 0, SEEK_SET);
	while (!feof(all[0]))
		if (fscanf(all[0], "%d ", &data) > 0) 
			fprintf(fp, "%d ", data);	

			
	//删除全部辅助的文件
	for (i = 0; i < number; i++) {	
		fclose(all[i]);
		sprintf(file_name, "help_%d.txt", i);
		remove(file_name);
	}
	//释放多余空间
	free(all);
	free(used_file);
}

/*
int main(int argc, char* argv[])
{
	int i = 0, ret = 0;
	FILE* fp = fopen(argv[1], "w");
	
	for (i =0; i < 100000; i++) 
		fprintf(fp, "%d ", rand()%1000);
	
	ret = fclose(fp);
	if (ret == -1) 
		perror("file close fail\n");
	fp = fopen(argv[1], "r+");		

	devide_and_conquer_file_sort(fp, 1024 * 4);	
	//printf("%d\n", test_file_data_have_fault(fp));	
	printf("%d\n", tidy_file(fp));	

	ret = fclose(fp);
	if (ret == -1) 
		perror("file close fail\n");
	
	return 0;
}
*/



//测试多路归并，可以提高效率
int main(int argc, char* argv[])
{
	int i = 0, j = 0, n = 10, ret = 0;
	FILE** fp = (FILE**)malloc(sizeof(FILE*) * n);
	char cmd[20] = {0};
	for (i = 0; i < n; i++) {
		sprintf(cmd, "help_test%d.txt", i);
		fp[i] = fopen(cmd, "w+");
		if (fp[i] == NULL) 
			perror("open fail\n");
		for (j = 0; j < 10; j++)
			fprintf(fp[i], "%d ", rand()%100);
		ret |= fclose(fp[i]);		
	}				
	if (ret == -1) 
		perror("file close fail\n");

	for (i = 0; i < n; i++) {
		sprintf(cmd, "help_test%d.txt", i);
		fp[i] = fopen(cmd, "r+");
		if (fp[i] == NULL) 
			perror("open fail\n");	
		little_file_quick_sort(fp[i]);
	}	
			
	//multicannel_merge_file_sort(10, fp[0], fp[1], \
		fp[2], fp[3], fp[4], fp[5], fp[6], fp[7], fp[8], fp[9]);
	multicannel_merge_file_sort_2(fp, 0, n - 1);
	//two_file_merge_sort(fp1, fp2);

	for (i = 0, ret = 0; i < n; i++) {
		printf("%d\n", tidy_file(fp[i]));
		ret |= fclose(fp[i]);
	}
	if (ret == -1) 
		perror("file close fail\n");
	
	return 0;
}



