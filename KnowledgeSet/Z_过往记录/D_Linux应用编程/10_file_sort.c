#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "sort.h"

#define max_int 0x7fffffff
//ƥ��str1��β������str2��ͬ�ģ���չ����.����
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

//���ж��Ƿ������Լ����ݸ���
int test_file_data_have_fault(FILE*fp)
{
	//����fp����֤ÿһ��ǰС�ں�
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

//�����ļ���ÿ��10��
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

//ֻ����ʹ�ÿ��ŵ�˼�������Ȼ�����㣬���������ݳ��ֲ��ȶ������
//���Բ��ó��׵ķ��Σ������ļ��ֳɶ��С�ļ������жԶ��С�ļ��ڲ�����
//��������

//��һ��С�ļ�����,�˴�Ϊ������
static void little_file_quick_sort(FILE* fp)
{
	//�����ļ���С
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
	
	//ѡȡһ���ڴ��������
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

//��·�鲢�������úñ�ǩ�����ɱ�֤������ȥ���������
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

//������
static void adjust_node(int* leave, int* node, int n, int i)
{
	int parent=(i + n - 1) / 2;      /* ������ڵ���±� */
	while (parent > 0) {
		if (leave[i] > leave[node[parent]]) {
			int temp = node[parent];
			node[parent]=i;
			/* iָ�������ʤ�� */
			i= temp;
		}
		parent = parent / 2;
	}
	node[0] = i;
}

static void init_min_data(int* leave, int* node, int n)
{
	//��ʼ��node������leave
	//����leave�ķ�Χ��n��node�ķ�Χ��n - 1
	int i = 0;
	 for(i = n; i > 0; i--)
		adjust_node(leave, node, n, i);
}

//��·�鲢����,�˴�������·�鲢����,���ö�·�鲢�������һ��
void multicannel_merge_file_sort_1(int n, ...)
{
	//��ȡ��Ҫ�鲢���ļ�������Ϊn��
	//����ָ������������ָ������֮��
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
	//������СΪ��Ӧ����������
	init_min_data(leave, node, n);
	//��ÿһ����Ȼ�������ֵ����ֵ������
	fseek(fp_help, 0, SEEK_SET);
	while (leave[node[0]] != max_int) {
		//���ϵأ�����С������ȡ����Сֵ��ȡ��������Сֵ����������
		//ע�⵽fp_all��leave�����Ӧ�ģ�
		int index = node[0];
		//ȡ����Сֵ�����뻺���ļ�
		fprintf(fp_help, "%d ", leave[index]);
		//������Сֵ
		if (!feof(fp_all[index]) && \
			fscanf(fp_all[index], "%d ", &data) > 0)	
			leave[index] = data;
		else 
			leave[index] = max_int;		
		//�����ý��
		adjust_node(leave, node, n, index);
	}
	//������д��fp_all[1]
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

//����һ��������һ�£�ֻ�ǲ�����ͬ����
void multicannel_merge_file_sort_2(FILE** fp_all, int left, int right)
{
	//��ȡ��Ҫ�鲢���ļ�������Ϊn = right - left + 1��
	//����ָ������������ָ������֮��
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
	//������СΪ��Ӧ����������
	init_min_data(leave, node, n);
	//��ÿһ����Ȼ�������ֵ����ֵ������
	fseek(fp_help, 0, SEEK_SET);
	while (leave[node[0]] != max_int) {
		//���ϵأ�����С������ȡ����Сֵ��ȡ��������Сֵ����������
		//ע�⵽fp_all��leave�����Ӧ�ģ�
		int index = node[0];
		//ȡ����Сֵ�����뻺���ļ�
		fprintf(fp_help, "%d ", leave[index]);
		//������Сֵ
		if (!feof(fp_all[left + index - 1]) && \
			fscanf(fp_all[left + index - 1], "%d ", &data) > 0)	
			leave[index] = data;
		else 
			leave[index] = max_int;		
		//�����ý��
		adjust_node(leave, node, n, index);
	}
	//������д��fp_all[left]
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
//�����ļ��ָ��С�ļ�������С�ļ�����help_xxx.txt��������ʽ
//Ȼ���С�ļ����������Դ��ļ�ʵ����·�鲢����
//���ֻʣ��һ���ļ�
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
	//Ԥ��һ��С�ļ��Ļ������1Mbit

	while (!feof(fp)) {
		//��ȡС�ļ�����
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

		//��С�ļ��ź���
		little_file_quick_sort(fp_buf);
		int ret = fclose(fp_buf);
		if (ret == -1)
			perror("file close fail\n");
	}
	
	//׼��һЩ��Ҫ������
	FILE**all = (FILE**)malloc(sizeof(FILE*) * number);
	bool* used_file = (bool*)malloc(sizeof(bool) * number);
	for (i = 0; i < number; i++) {
		sprintf(file_name, "help_%d.txt", i);
		FILE* fp_buf = fopen(file_name,"r+");
		all[i] = fp_buf;
		used_file[i] = 0;	
	}
	//�������е�С�ļ����Ѿ��ź����ˣ���ЩС�ļ�һ����
	//number���������ֱ��Ϊhelp_0.txt --- help_(number-1).txt
	all_file_number = number;
	
	while (all_file_number > 1) {
		//��˳��ȡ����С�ļ����鲢����
		for (i = 0; i < number; i++) {	
			if (used_file[i] != 0)
				continue;
			else {
				//��i + 1��ʼȡһ��δʹ���ļ�
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
	//���Ľ�������all[0]��
	fseek(all[0], 0, SEEK_SET);
	while (!feof(all[0]))
		if (fscanf(all[0], "%d ", &data) > 0) 
			fprintf(fp, "%d ", data);	

			
	//ɾ��ȫ���������ļ�
	for (i = 0; i < number; i++) {	
		fclose(all[i]);
		sprintf(file_name, "help_%d.txt", i);
		remove(file_name);
	}
	//�ͷŶ���ռ�
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



//���Զ�·�鲢���������Ч��
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



