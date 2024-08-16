#ifndef __LEETCORD_TIMO_H_
#define __LEETCORD_TIMO_H_

#ifdef __cplusplus
	extern "C" {
#endif		

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct hash_node{
	int index;
	struct hash_node* next;
};


//正则表达式匹配，其中*表示它之前的数可以任意个数
bool regular_expression_matching(char* str1, char* str2)
{
	char *buf_str1 = str1,*buf_str2 = str2;
	int count1 = 0, count2 = 0, i = 0, j = 0;
	int nonius1 = 0, nonius2 = 0, result = 0;
	while (*buf_str1++ != '\0')
		count1++;
	while (*buf_str2++ != '\0')
		count2++;
	
	bool** dp = (bool**)malloc(sizeof(bool*) * (count1 + 1));
	for (i = 0; i < count1 + 1; i++)
		dp[i] = (bool*)malloc(sizeof(bool) * (count2 + 1));
	
	dp[0][0] = 1;
	for (i = 0; i < count1 + 1; i++)
		for (j = 0; j < count2 + 1; j++)
			if (i == 0 || j == 0)
				dp[i][j] = 1;
			else	
				dp[i][j] = 0;
		
	//转移方程
	//if str2[j] != '*' 
	//dp[i][j] = dp[i - 1][j - 1] if str1[i] == str2[j]
	//dp[i][j] = false if otherwise
	//if str2[j] == '*'
	//dp[i][j] = dp[i - 1][j] || dp[i][j - 2] if str1[i] == str2[j]
	//dp[i][j] =  dp[i][j - 2] if otherwise
	
	for (i = 1; i < count1 + 1; i++)
		for (j = 1; j < count2 + 1; j++) {
		//情况1：
			if (*(str2 + j - 1) != '*') {
				if (*(str1 + i - 1) == *(str2 + j - 1) || *(str1 + i - 1) == '.' || *(str2 + j - 1) == '.')
					dp[i][j] = dp[i - 1][j - 1];
				else
					dp[i][j] = 0;
		//情况2：
			} else {
				if (j  >= 2)
				if (*(str1 + i - 1) == *(str2 + j - 2))
					dp[i][j] = dp[i - 1][j] || dp[i][j - 2];
				else 
					dp[i][j] =  dp[i][j - 2];
			}	
	}	
	
	for (i = 0; i < count1 + 1; i++) {
		for (j = 0; j < count2 + 1; j++)
			printf("%d ", dp[i][j]);
		printf("\n");
	}	
	
	result = dp[count1][count2];
	
	for (i = 0; i < count1 + 1; i++)
		free(dp[i]);	
	free(dp);
	
	return result;
}

//寻找九键数字下对应字母集合
//即路径的查找打印，需要利用缓冲区存储节点数据
void help_telephone_number_to_str(int data, int level, char* list, int n)
{
	if (level == 0) {
		//产生一次全路径，逆序打印
		while (n >= 0) {
			printf("%c ", list[n]);
			n--;
		}
		printf("\n");
		return;
	} else {		
		//打印前面的位
		//打印最后一位
		int t = data % 10, i = 0;		
		switch (t) {
		case 2 :
		case 3 :
		case 4 :
		case 5 :
		case 6 :
			for (i = 0; i < 3; i++) {
				list[n - level] = 'a' + i + 3 * (t - 2);
				help_telephone_number_to_str(data / 10, level - 1, list, n);
			}
			break;
		case 7 :
			for (i = 0; i < 4; i++) {
				list[n - level] = 'a' + i + 3 * (t - 2);
				help_telephone_number_to_str(data / 10, level - 1, list, n);
			}			
			break;
		case 8 :
			for (i = 0; i < 3; i++) {
				list[n - level] = 'a' + i + 3 * (t - 2) + 1;
				help_telephone_number_to_str(data / 10, level - 1, list, n);
			}			
			break;
		case 9 :
			for (i = 0; i < 4; i++) {
				//打印前面的组合；打印该数
				list[n - level] = 'a' + i + 3 * (t - 2) + 1;
				help_telephone_number_to_str(data / 10, level - 1, list, n);
			}			
			break;						
		}	
	}
}

void telephone_number_to_str(int data) 
{
	int t_data = data;
	int count = 0;
	while (!(t_data / 10 == 0 && t_data % 10 == 0)) {
		t_data /= 10;
		count++;
	}
	char* list = (char*)malloc(sizeof(char) * count);

	help_telephone_number_to_str(data, count, list, count);
	 
	free(list);
}

//验证括号匹配度
int assert_parentheses_matching(char* str)
{
	int count = 0;
	char * buf_str = str;
	while (*buf_str++ != '\0') 
		count++;
	int nonius = 1;
	char* simulate_stack = (char*)malloc(sizeof(char) * (count + 1));	
	for (buf_str = str; * buf_str != '\0'; buf_str++) {
		//取出值，如果是([{入栈}])出栈对应的,
		if (* buf_str == '(' || * buf_str == '[' || * buf_str == '{') 
			simulate_stack[nonius++] = * buf_str;
		if (* buf_str == ')' || * buf_str == ']' || * buf_str == '}') {
			nonius--;
			if (!((simulate_stack[nonius] == '(' && * buf_str == ')') ||
				(simulate_stack[nonius] == '[' && * buf_str == ']') ||
				(simulate_stack[nonius] == '{' && * buf_str == '}')))
				return 0;
		}						
	}
	free(simulate_stack);
	return 1;
	
}

void help_create_n_pairs_parentheses(char* list, int n, int left_count, int right_count)
{
	//左括号个数只能大于等于右括号个数，且如果一旦等于就只能插入左括号
	//如果大于的话可以左括号可以右括号
	if (n < left_count + right_count)
		return;
	else if (n == left_count + right_count) {
		//递归终止是左右括号填满了整个数组
		if (left_count == right_count) {
			int i = 0;
			for (i = 0; i < n; i++) 
				printf("%c ", list[i]);	
			printf("\n");
		}
		return;
	} else {
		//当前浮标恰好就是left_count + right_count + 1
		int nonius = left_count + right_count;		
		if (left_count > right_count) {		
			//左括号个数大于右括号个数，左右括号都能插入
			list[nonius] = '(';
			help_create_n_pairs_parentheses(list, n, left_count + 1, right_count);			
			list[nonius] = ')';
			help_create_n_pairs_parentheses(list, n, left_count, right_count + 1);						
		} else if (left_count == right_count) {			
			list[nonius] = '(';
			help_create_n_pairs_parentheses(list, n, left_count + 1, right_count);					
		}				
	}		
}

//生成匹配小括号集合
void create_n_pairs_parentheses(int n)
{
	n /= 2;
	n *= 2;	
	char* list = (char*)malloc(sizeof(char) * (n + 1));
	help_create_n_pairs_parentheses(list, n, 0, 0);
	free(list);
}

//统计已排序数组中数据，略过重复数据
int statistics_n_number_in_array(int* array, int left, int right)
{
	int count = 0;
	int slow_nonius = left, fast_nonius = left;
	for ( slow_nonius = left, fast_nonius = left; \
		fast_nonius <= right; fast_nonius++) {
		if (array[fast_nonius] == array[slow_nonius])
			continue;
		else {
			count++;
			slow_nonius = fast_nonius;			
		}						
	}
	count++;
	return count;
}

//数据去重 
void remove_duplicates_from_sorted_array_1(int* array, int left, int right)
{
	int MAX_INT = ~(1 << 31);
	int slow_nonius = left, fast_nonius = left, i = left;
	for (fast_nonius = left; fast_nonius <= right; fast_nonius++) {
		if (slow_nonius != fast_nonius && \
			array[slow_nonius] == array[fast_nonius])
			array[fast_nonius] = MAX_INT;
		if (array[slow_nonius] != array[fast_nonius])
			slow_nonius = fast_nonius;
	}
	
	for (slow_nonius = left, fast_nonius = left; \
		fast_nonius <= right; fast_nonius++) {
			
		while (array[slow_nonius] != MAX_INT)
			slow_nonius++;			
		fast_nonius = slow_nonius;		
		while (array[fast_nonius] == MAX_INT) 
			fast_nonius++;
		
		array[slow_nonius] = array[fast_nonius];
		array[fast_nonius] = MAX_INT;
		
	}	
	//验证 
	for (i = left; i <= right; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");	
	
}

void remove_duplicates_from_sorted_array_2(int* array, int left, int right)
{
	int MAX_INT = ~(1 << 31);
	int slow_nonius = left, fast_nonius = left, i = left;
	for (slow_nonius = left,fast_nonius = left; \
		fast_nonius <= right; fast_nonius++) {
		if (array[slow_nonius] != array[fast_nonius])
			array[++slow_nonius] = array[fast_nonius];
	}
	while (++slow_nonius <= right) 
		array[slow_nonius] = ~(1 << 31);		
	//验证 
	for (i = left; i <= right; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");		
}

//去除指定数据 
void remove_num_from_sorted_array(int* array, int left, int right, int x)
{
	int start = left ,end = right, i = left;
	while (start < end) {
		while (start <= right && array[start] != x)
			start++; 
		while (end >= left && array[end] == x)
			end--;
		if (start < end) {
			array[start] = array[end];		
			array[end--] = ~(1 << 31);			
		}
	}
	//验证 
	for (i = left; i <= right; i++) {
		printf("%d ", array[i]);
	}
	printf("\n");	 	
}

//字符串匹配，其中关于next数组求解存在不理解 
int search_first_index_from_str_1(char* str1, char* str2)
{
	if (str2 == NULL)
		return 0;
	else {
		char* index = str1;
		char* buf_str1 = str1,* buf_str2 = str2;
		while (*buf_str1 != '\0') {
			if (*buf_str1 != *buf_str2) 
				buf_str1++;
			index = buf_str1;			
			while (*buf_str1 != '\0' && \
				*buf_str2 != '\0' && \
				*buf_str1++ == *buf_str2++); 
			if (*buf_str2 == '\0')	
				return index - str1;
			else
				buf_str2 = str2;
			if (*buf_str2 != '\0')	
				buf_str1 = ++index;
		}
		return -1;			
	}	
}

int search_first_index_from_str_2(char* str1, char* str2)
{
	if (str2 == NULL)
		return 0;
	int i = 0, j = 0;
	while (str1[i] != '\0' && str2[j] != '\0') {
		if (str1[i] == str2[j]) {
			i++;
			j++;
		} else {
			i = i - j + 1;
			j = 0;
		}
	}	
	if (str2[j] == '\0')				
		return i - j;
	return -1;
}

int search_first_index_from_str_3(char* str1, char* str2)
{
	if (str2 == NULL) 
		return 0;
	int count = 0, i = 0, j = 1;
	char* buf_str = str2;
	while(*buf_str++ != '\0')
		count++;
	int* help = (int*)malloc(sizeof(int) * count);
	help[0] = 0;
	while (str2[j] != '\0') {
		if (str2[i] == str2[j]) {
			help[j] = help[j - 1] + 1;
			i++; 
			j++;
		} else {
			i = 0;				
			help[j++] = 0;						
		}		
	}
	
	//for (i = 0; str2[i] != '\0'; i++)
	//	printf("%d", help[i]);
		
	i = 0;
	j = 0;
	while (str1[i] != '\0' && str2[j] != '\0') {
		if (j == 0 || str1[i] == str2[j]) {
			i++;
			j++;
		} else {
			j = help[j - 1];		
		}
	}	
	if (str2[j] == '\0')				
		return i - j;
	return -1;	
}

int search_first_index_from_str_4(char* str1, char* str2)
{
	if (str2 == NULL)
		return 0;
	int count = 0, i = 0, j = 1;
	char* buf_str = str2;
	while (*buf_str++ != '\0')
		count++;
	int* help = (int*)malloc(sizeof(int) * count);
	help[0] = -1;
	while (str2[j + 1] != '\0') {
		if (str2[i] == str2[j]) {
			help[j + 1] = help[j] + 1;
			i++; 
			j++;
		} else {
			i = 0;				
			help[j++] = 0;						
		}		
	}
	
	//for (i = 0; str2[i] != '\0'; i++)
	//	printf("%d", help[i]);
		
	i = 0;
	j = 0;
	while (str1[i] != '\0' && \
		(j == -1 || str2[j] != '\0')) {
		if (j == -1 || str1[i] == str2[j]) {
			i++;
			j++;
		} else {
			j = help[j];		
		}
	}	
	if (str2[j] == '\0')				
		return i - j;
	return -1;	
}

//俩数相除
int data1_devide_data2(int data1, int data2) 
{
	if (data2 == 0)
		return -1;
	else {
		int flag = 0;
		if (data1 < 0 && data2 < 0) {
			data1 = ~data1 + 1;	
			data2 = ~data2 + 1;		
		} else if (data1 < 0 && data2 > 0) {
			data1 = ~data1 + 1;
			flag = 1;
		} else if (data1 > 0 && data2 < 0) {
			data2 = ~data2 + 1;		
			flag = 1;
		}		
		int buf_data2 = data2;
		int result = 0;
		while (data1 > data2)
			data2 = data2 << 1;
		while (data2 >= buf_data2) {		
			result = result << 1;
			if (data2 <= data1) {
				data1 -= data2;			
				result += 1;
			}
			data2 = data2 >> 1;			
		}
		if (flag == 1)
			return ~(result) + 1;
		return result;						
	}	
} 

//匹配字符串，模式字符串是多个字符串
//要求模式字符串串联
//未尝试官方解法 
void search_all_str_from_str(char*str1, char*str2[], int n) 
{
	int count = 0, i = 0;
	char* buf = str2[0];
	while(*buf++ != '\0')
		count++;
	char* all_str2 = \
		(char*)malloc(sizeof(char) * (n * count + 1));
	all_str2[0] = '\0';
	for (i = 0; i < n; i++)	
		strcat(all_str2, str2[i]);
	int last_index = 0, index = 0;
	
	while (1) {
		last_index = index;//保存之前的偏移量 
		index = search_first_index_from_str_2(str1 + index, all_str2);
		if (-1 == index)
			break;
		else {				
			last_index += index;//累加寻找到之后的偏移量 
			index += last_index + 1;			
			printf("%d \n", last_index);							
		}
	}
	free(all_str2);		
}

//下一个排列 
//调整部分数组最好的方式是，保证有序后翻转 
void next_array_sort(int* array, int left, int right)
{
	if (right - left + 1 >= 2) {		
		int i = 0, j = 0, k = 0, temp = 0;
		//若不存在下一个最小序列
		for (i = left; i + 1 <= right; i++)
			if (array[i] < array[i + 1])
				break;										
		if (i + 1 > right) {
	 		//逆序数组 
			for (j = left, k = right; j < k; j++, k--) {
				temp = array[j];
				array[j] = array[k];
				array[k] = temp;					
			}
			//验证 
			//for (i = left; i <= right; i++)
			//	printf("%d ", array[i]);
			//printf("\n");
			
			return;				
		} 
	 						
		//先找到其数，从最右端开始，寻找一个
		//array[nonius1] < array[nonius2]
		for (i = right; i - 1 >= left; i--)
			if (array[i - 1] < array[i]) {
				for (j = i; j <= right; j++)
					if (j + 1 > right || (array[i - 1] < array[j] && array[i -1] > array[j + 1])) {
						temp = array[i - 1];
						array[i - 1] = array[j];
						array[j] = temp;
						//逆序i - 1之后的数据
						for (j = i, k = right; j < k; j++, k--) {
							temp = array[j];
							array[j] = array[k];
							array[k] = temp;					
						}
						//验证 
						//for (i = left; i <= right; i++)
						//	printf("%d ", array[i]);
						//printf("\n");	
						
						return;						
					}					
			}
	}		
} 

//最长有效括号对 
int longest_vaild_parenthesis(char* str) 
{
	int count = 0, index_stack = 0, index_buf = 0;
	int count_vaild = 0, i = 0;
	char* buf = str;
	while (*buf++ != '\0')
		count++;
	char* buf_memory = (char*)malloc(sizeof(char) * (count + 1));
	buf = str; 
	count = 0;
	while (*buf != '\0') {
		if (*buf == '(') {
			//直接压栈
			index_stack++; 
			buf_memory[index_buf++] = '(';
		} else if (*buf ==')') {
			//该条件判断右括号溢出,因为右括号一定需要左括号的匹配 
			if (index_stack > 0 ) { 
				//出栈并将其写入临时数组中
				index_stack--;
				//该标签判断左括号溢出
				//因为左括号要么有匹配到的右括号，要么就多余在最后面 
				count_vaild++;
				buf_memory[index_buf++] = ')'; 	
				count++;			
			}			
		}
		buf++;
	}
	buf_memory[index_buf++] ='\0';		
	for (i = 0; i < index_buf && count_vaild > 0; i++) {
		if (buf_memory[i] == ')') 
			count_vaild--;
		str[i] = buf_memory[i];
	} 		
	str[i++] ='\0';	
							
	free(buf_memory);
	return count * 2;
} 

//某个排序数组在某一个位置旋转了，即
//前面的一部分数据跑到了后面去了,出现俩个排序数组接在一起 
int binary_search(int* array, int left, int right, int x) 
{
	if (left >= right && array[left] != x)
		return -1;	
	else {
		int center = (left + right) / 2;
		if (x == array[center])
			return center;
		else if (x < array[center])
			return binary_search(array, left, center - 1, x); 
		else 	
			return binary_search(array, center + 1, right, x); 
	}	
}

int search_special_point(int* array, int left, int right, int old_left, int old_right)
{
	//寻找三个点进行二分，即取出center，left~center，center~right中的 
	if (left >= right)
		return -1;
	else {	
		int fake_center = (left + right) / 2;
		int left_center = (left + fake_center) / 2;
		int right_center = (fake_center + right) / 2;
		
		//断言特殊点
		if (fake_center - 1 >= old_left && array[fake_center - 1] <= array[fake_center] && \
			fake_center + 1 <= old_right && array[fake_center + 1] <= array[fake_center]) 
		 	return fake_center;	
		if (left_center - 1 >= old_left && array[left_center - 1] <= array[left_center] && \
			left_center + 1 <= old_right && array[left_center + 1] <= array[left_center]) 
		 	return left_center;		 	
		if (right_center - 1 >= old_left && array[right_center - 1] <= array[right_center] && \
			right_center + 1 <= old_right && array[right_center + 1] <= array[right_center]) 
		 	return right_center;		 	
			
		if (left_center <= right_center) {
			//特殊点在俩端	
			int index1 = search_special_point(array, left, left_center, old_left, old_right);
			int index2 = search_special_point(array, right_center, right, old_left, old_right);
			return index1 == -1 ? index1 : index2;
		} else if (left_center > right_center){
			//特殊点在中间 
			return search_special_point(array, left_center, right_center, old_left, old_right);
		}				
	}	
}

int search_data_from_rotated_array_1(int* array, int left, int right, int x) 
{	
	//寻找特殊点
	int index = search_special_point(array, left, right, left, right);	 
	//寻找查找重点 
	if (x >= array[left]) 
		return binary_search(array, left, index, x); 
	else	
		return binary_search(array, index + 1, right, x); 	
}

int search_data_from_rotated_array_2(int* array, int left, int right, int x)
{
	if (left > right)
		return -1;
	else if (left == right)
		return array[left] = x ? left : -1;
	else {
		int fake_center = (left + right) / 2;
		if (array[fake_center] == x)
			return fake_center;
		//假如left ~ center为有序数组
		if (array[left] <= array[fake_center]) {
			//如果满足x在范围内
			if (array[left] <= x && x <= array[fake_center]) 
				return search_data_from_rotated_array_2(array, left, fake_center - 1, x);
			else
				return search_data_from_rotated_array_2(array, fake_center + 1, right, x);	
		} else {
		//if (array[fake_center] < array[right]) {
			if (array[fake_center] < x && x <= array[right]) 
				return search_data_from_rotated_array_2(array, fake_center + 1, right, x);
			else							
				return search_data_from_rotated_array_2(array, left, fake_center - 1, x);			
		}		
	}
}

//判断是否是数独,直接暴力匹配的 
int assert_little_is_suduko(char* array, int col_left, int col_right, \
	int row_left, int row_right)
{
	if (row_right - row_left + 1 != 3)
		return 0;
	if (col_right - col_left + 1 != 3)
		return 0;
	
	int buf[9] = {0}, i = 0, j = 0, temp = 0;
	for (i = row_left; i <= row_right; i++)
		for (j = col_left; j <= col_right; j++)
			if(array[i * 9 + j] != '.')
				buf[ array[i * 9 + j] - '1']++;			
			
	for (i = 0; i < 9; i++)
		if (buf[i] > 1)
			return 0;
	return 1;
}

int assert_is_suduko(char* array, int col_left, int col_right, \
	int row_left, int row_right) 
{
	if (row_right - row_left + 1 != 9)
		return 0;
	if (col_right - col_left + 1 != 9)
		return 0;	
	
	int i = 0, j = 0;
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			if(assert_little_is_suduko(array,col_left + j * 3, col_left + (j + 1) * 3 - 1,
				row_left + i * 3, row_left + (i + 1) * 3 - 1) == 0)
				return 0;
									
	for (i = row_left; i <= row_right; i++) {
		int buf[9] = {0};	
		for (j = col_left; j <= col_right; j++)	{
			if (array[i * 9 + j] != '.')
				buf[array[i * 9 + j] - '1']++;						
		}	
		for (j = 0; j < 9; j++)
				if (buf[j] > 1)
					return 0;		
	}	

	for (i = col_left; i <= col_right; i++) {
		int buf[9] = {0};	
		for (j = row_left; j <= row_right; j++)	{
			if (array[i * 9 + j] != '.')
				buf[array[i * 9 + j] - '1']++;						
		}	
		for (j = 0; j < 9; j++)
				if (buf[j] > 1)
					return 0;		
	}
						
	return 1;	
}

//解数独
int solve_suduko(char* array, int col_left, int col_right, \
	int row_left, int row_right)
{
	if (row_right - row_left + 1 != 9)
		return -1;
	if (col_right - col_left + 1 != 9)
		return -1;	
	
	//如果已经不满足是一个数独了 
	if (0 == assert_is_suduko(array, col_left, col_right, row_left, row_right)) 
		return 0;
		
	int i = 0, j = 0;
	for (i = row_left; i <= row_right; i++) 	
		for (j = col_left; j <= col_right; j++)
			if (array[i * 9 + j] != '.')
				continue;
			else {
				//找到一个位置，准备填数 
				//先找一个合适的数，剪枝 
				int buf[9] = {0};
				int buf_i = i / 3, buf_j = j /3, buf_t = 0;
				
				for (buf_i = i / 3; buf_i < (i + 1) / 3; buf_i++)
					for (buf_j = j / 3; buf_j < (j + 1) / 3; buf_j++)
						if(array[buf_i * 9 + buf_j] != '.')
							buf[array[buf_i * 9 + buf_j] - '1']++;
				
				for (buf_i = 0; buf_i < 9; buf_i++)
						if(array[buf_i * 9 + j] != '.')
							buf[array[buf_i * 9 + j] - '1']++;
				for (buf_j = 0; buf_j < 9; buf_j++)
						if(array[i * 9 + buf_j] != '.')
							buf[array[i * 9 + buf_j] - '1']++;	
				
				//将其中的这系列数一个个尝试 
				for (buf_t = 0; buf_t < 9; buf_t++) {
					if (buf[buf_t] == 0) {
						//填数
						array[i * 9 + j] = (char)(buf_t + '1');
						//如果下一次填的数失败，该节点状态回退 
						if (0 == solve_suduko(array, col_left, col_right, row_left, row_right))
							array[i * 9 + j] = '.';
						else
							return 1;						
					}
				}									
				//找不到要填的数，即该位死掉，回到上一层	
				return 0;							
			}
			
	//数据全填满了	
	for (i = row_left; i <= row_right; i++) {
		for (j = col_left; j <= col_right; j++) 	
			printf("%c ", array[i * 9 + j]); 
		printf(" \n");
	}		
	return 1;	
		
	
}
 
//报数

//组合数，组合数2
//数组无重复数字，所有数都大于0，且是顺序的
//因为 x = 0 + 0 + 0 + ……会死锁，且无意义
//考虑从数据中查找其重复数据的时候，应该考虑回溯递归
int help_search_all_combain_number(int* array, int left, int right, int x, 
	int * temp, int number, int current_index, int flag)
{
	if (current_index >= number)
		return -1;
	else if (x < 0) 
		return -1;
	else if (x == 0) {
		int i = 0;
		for (i = 0; i < current_index; i++)
			printf("%d", temp[i]);
		printf("\n");
		return 1;
	} else {
		int i = 0;
		for (i = left; i <= right; i++) {
			if (array[i] <= x) {
				//保存该数
				x -= array[i];
				temp[current_index++] = array[i];
				
				//if (x >= array[i])
				//进入下一层,在之前先判断，下一层是从哪个位置开始
				//查找重复数据时			
				if (flag== 0) {
					if(1 != help_search_all_combain_number(array, i, right, x, 
					temp, number, current_index, flag)) {
						//该数不行，还原现场
						current_index--;	
						x += array[i];	
					}
				} else if (flag == 1) {
					if(1 != help_search_all_combain_number(array, i + 1, right, x, 
					temp, number, current_index, flag)) {
						//该数不行，还原现场
						current_index--;	
						x += array[i];											
					}
				} else
					return -1;				
			} else 
				return 0;			
		}
	}
}

void search_all_combain_number(int* array, int left, int right, int x)
{
	//因为所有数据是正数，寻枝的过程需要准备足够的线性空间
	int* temp = (int*)malloc(sizeof(int) * (x / array[left] + 1));
	//保证其是顺序
	quick_sort(array, left, right);
	//flag == 0，组合可重复但数组数据不重复，
	//flag == 1，组合不可重复单数组数据可重复
	help_search_all_combain_number(array, left, right, x, temp, x + 1, 0, 1);		
	free(temp);
}
  
//数组中寻找最小缺失正数
int search_first_missing_positive(int* array, int left, int right)
{
	int i = 0, temp = 0;
	//一次遍历，将数分成
	for (i = left; i <= right; i++) {
		if (array[i] <= 0)
			continue;
		else {
			//交换array[i] 中值对应的数组下标，若越界则不考虑
			if(array[i] + left > right)
				continue;
			else {
				//交换array[i] 与 array[array[i] + left]的值
				//最后基本上，小数会换到前面来
				temp = array[array[i] + left];
				array[array[i] + left] = array[left];
				array[i] = temp;
			}							
		}
	}
	
	for (i = left + 1; i <= right; i++)
		printf("%d ", array[i]);
	
	for (i = left + 1; i <= right; i++)
		if (array[i] > 0)
			continue;
		else {
			return (i - 1 >= 0)? (array[i - 1] + 1) : 1;
			
		}
}
 
//接雨水
int trapping_rain_water_1(int* array, int left, int right)
{
	//从头扫到尾，然后从尾扫到头，只加不减
	int i = 0, max = 0, all_container = 0;
	int * buf_array_1 = (int*)malloc(sizeof(int) * (right - left + 1));
	int * buf_array_2 = (int*)malloc(sizeof(int) * (right - left + 1));
	for (i = left, max = 0; i <= right; i++) {
		if (array[i] > max)		
			max = array[i];
		buf_array_1[i - left] = max;
	}
	for (i = right, max = 0; i >= left; i--) {
		if (array[i] > max)		
			max = array[i];
		buf_array_2[i - left] = max;
	} 
	for (i = 0; i < right - left + 1; i++) {
		int min = buf_array_1[i] < buf_array_2[i] ? \
			buf_array_1[i] : buf_array_2[i];
		all_container += (min - array[i + left]);	
	}
	return all_container;
}
 
int trapping_rain_water_2(int* array, int left, int right) 
{
	int all_container = 0;
	while (left < right) {
		if (array[left] < array[right]) {
			int temp = left + 1;
			while (temp <= right)
				if (array[temp] <= array[left])
					all_container += array[left] - array[temp++];
				else {
					left = temp;
					break;
				}					
		} else {
			int temp = right - 1;
			while (temp >= left)
				if (array[temp] <= array[right])
					all_container += array[left] - array[temp--];
				else {
					right = temp;
					break;
				}						
		}		
	}
	return all_container;
}
 
int trapping_rain_water_3(int* array, int left, int right) 
{
	int all_container = 0;
	int * buf_array = (int*)malloc(sizeof(int) * (right - left + 1));
	int index = -1;
	for (buf_array[++index] = left++; left <= right; left++) {
		if (index >=0 && array[buf_array[index]] < array[left]) {
			//如果当前栈中只有一个元素，则不能形成坑
			if (index > 1) {
				int k = array[buf_array[index--]];
				all_container += (left - buf_array[index] - 1) * \
					((array[buf_array[index]] <= array[left] ? \
					array[buf_array[index--]] : array[left]) - k);			
			}
		}
		buf_array[++index] = left;
	}
	return all_container;
}
 
//外卡匹配，匹配*和.，其中*通配任意个字符，.通配单个字符
bool wildcard_matching(char* str1, char* str2)
{
	char *buf_str1 = str1,*buf_str2 = str2;
	int count1 = 0, count2 = 0, i = 0, j = 0;
	int nonius1 = 0, nonius2 = 0, result = 0;
	while (*buf_str1++ != '\0')
		count1++;
	while (*buf_str2++ != '\0')
		count2++;
	
	bool** dp = (bool**)malloc(sizeof(bool*) * (count1 + 1));
	for (i = 0; i < count1 + 1; i++)
		dp[i] = (bool*)malloc(sizeof(bool) * (count2 + 1));
	
	for (i = 0; i < count1 + 1; i++)
		for (j = 0; j < count2 + 1; j++)
			if (i == 0 || j == 0)
				dp[i][j] = 1;
			else	
				dp[i][j] = 0;
		
	//转移方程
	for (i = 1; i < count1 + 1; i++)
		for (j = 1; j < count2 + 1; j++) {
		if (*(str1 + i - 1) == '*' || *(str2 + j - 1) == '*')
			dp[i][j] = dp[i - 1][j] || dp[i][j - 1];
		else if(*(str1 + i - 1) == '.' || *(str2 + j - 1) == '.' || \
			*(str1 + i - 1) == *(str2 + j - 1))
			dp[i][j] = dp[i - 1][j - 1];			
	}	
	
	for (i = 0; i < count1 + 1; i++) {
		for (j = 0; j < count2 + 1; j++)
			printf("%d ", dp[i][j]);
		printf("\n");
	}	
	
	result = dp[count1][count2];
	
	for (i = 0; i < count1 + 1; i++)
		free(dp[i]);	
	free(dp);
	
	return result;
}
 
//跳跃游戏2，用最少的步数跳到最后
int jump_game_ii(int* array, int left, int right)
{
	//贪心，关于i级阶梯，是跳还是不跳，选取最划算的
	 int pre = left, cur = left, count = 0, i = left;
	while (cur < right) {		
		//注：此次的pre是实际上的本次cur，
		//因为要更新cur所以无法使用cur
		for (i = left, pre = cur; i <= pre; i++)
				if(cur < i + array[i])
					cur = i + array[i];	
		count += 1;
		if (cur == pre)
			return -1;
	}	
	return count;
}
 
//查找全排列，flag = 0会重复数据，flag = 1避开重复
void help_search_permutations(int* array, bool* is_used, int left, int right, \
	int* temp, int number,bool* help, int flag) 
{
	if (number >= right - left + 1) {
		int i = 0;
		for (i = left; i <= right; i++)
			printf("%d ",temp[i - left]);
		printf("\n");
	} else {
		if (flag == 0) {
			int i = 0;
			for (i = left; i <= right; i++) {
				if (is_used[i - left] == 0) {
					is_used[i - left] = 1;
					temp[number] = array[i];
					help_search_permutations(array, is_used, left, right, temp, number + 1, help, flag); 
					is_used[i - left] = 0;
				}			
			}		
		} else if (flag == 1){
			int i = 0;
			for (i = left; i <= right; i++) {				
				if (is_used[i - left] == 0) {
					//若同一层出现俩个可用的相同数据避开它
					if (i - left - 1 >= 0 && array[i - left - 1] == array[i - left] && is_used[i - left - 1] == 0) {
						if (help[i - left - 1] == 1)
							help[i - left - 1] = 0;
						else
							continue;
					}	
					is_used[i - left] = 1;
					temp[number] = array[i];
					help_search_permutations(array, is_used, left, right, temp, number + 1, help, flag); 
					is_used[i - left] = 0;																															
				}
			}
		}
	}	
}
 
void search_permutations(int* array, int left, int right)
{
	bool* is_used = (bool*)calloc((right - left + 1), sizeof(bool));
	bool* help = (bool*)calloc((right - left + 1), sizeof(bool));
	int* temp = (int*)calloc( (right - left + 1), sizeof(int));
	
	help_search_permutations(array, is_used, left, right, temp, 0, help, 1);
} 
 
//旋转矩阵：转置矩阵,然后逆序每行或每列（看情况）
//画图即可表示出

//实现pow(x ,y)
int calculate_data_pow(int x, int y)
{
	if (y == 0)
		return 1;
	else if (y == 1)
		return x;
	else if (y == 2)
		return x * x;
	else	
		if (y % 2 == 0)
			return calculate_data_pow(x, y / 2) * \
				calculate_data_pow(x, y / 2);
		else
			return calculate_data_pow(x, y / 2) * \
				calculate_data_pow(x, y / 2 + 1);			
}

//n皇后问题
int help_confirm_n_queen(int n, bool **chess_board, \
	bool* row, bool*col, int old_n)
{	
	if (n <= 0) {
		int i, j;		
		//到了最后了
		for (i = 0; i < old_n; i++) {
			for (j = 0; j< old_n; j++)		
				printf("%d ", chess_board[i][j]);
			printf("\n");
		}	
		printf("\n");
		return 1;
	} else {
		//说明前一个女皇位置有效，且未到最后，则可以找下一个女皇
		int count = 0, i, j;	
		for (i = old_n - n; i < old_n; i++) {
			if (row[i] == 1) //已经使用该行
				continue;
			for (j = 0; j < old_n; j++) {
				if (col[j] == 1) //已经使用该列
					continue;				
				//优先检察数据合理性
				//检验新放入的女皇N(i,j)是否满足要求，即以该点求俩对角线的数据是否存在				
				int buf_i, buf_j, flag = 0;				
				for (buf_i = i - 1, buf_j = j - 1; flag == 0 && \
					buf_i >= 0 && buf_j >= 0; buf_i--, buf_j--)
						if (chess_board[buf_i][buf_j] == 1)
							flag = 1;
				for (buf_i = i + 1, buf_j = j - 1;flag == 0 && \
					buf_i < old_n && buf_j >= 0; buf_i++, buf_j--)
						if (chess_board[buf_i][buf_j] == 1) 
							flag = 1;
				for (buf_i = i - 1, buf_j = j + 1;flag == 0 &&  \
					buf_i >= 0 && buf_j< old_n; buf_i--, buf_j++)
						if (chess_board[buf_i][buf_j] == 1) 
							flag = 1;
				for (buf_i = i + 1, buf_j = j + 1;flag == 0 &&  \
					buf_i < old_n && buf_j< old_n; buf_i++, buf_j++)
						if (chess_board[buf_i][buf_j] == 1) 					
							flag = 1;
				//现在获得一个可行的位置，是时候往下走了
				if (flag == 0) {				
					row[i] = 1;
					col[j] = 1;
					chess_board[i][j] = 1;					
					count += \
						help_confirm_n_queen(n - 1, chess_board, row, col, old_n);			
					//还原现场	
					chess_board[i][j] = 0;
					row[i] = 0;
					col[j] = 0;				
				}							
			}
		}
		return count;
	}	
}

int confirm_n_queen(int n)
{
	//在n * n个棋盘中存放n个皇后，使其不冲突即可
	int i, j, count = 0;	
	bool** chess_board = (bool**)calloc(n, sizeof(bool*));
	for (i = 0; i < n; i++)
		chess_board[i] =  (bool*)calloc(n, sizeof(bool));
	bool* row = (bool* )calloc(n, sizeof(bool));
	bool* col = (bool* )calloc(n, sizeof(bool));
	//首先保证其，行与列一定不冲突，即保证一行一列中只有一个女皇

	count = help_confirm_n_queen(n, chess_board, row, col, n);	

	for (i = 0; i < n; i++)
		free(chess_board[i]);	
	free(chess_board);
	free(row);
	free(col);
		
	return count;
}

//最大子数组
//dp[i]=dp[i?1]+nums[i]	,ifdp[i?1]≥0
//			nums[i]				,ifdp[i?1]<0
int maximum_subarray_1(int* array, int left, int right)
{
	if (left >= right)
		array[left];	
	int pre = left, pre_max = array[left];
	int cur = left, cur_max = array[left];
	int i = 0;
	for (i = left; i <= right; i++) {
		if (cur_max > pre_max) {
			pre = cur;
			pre_max = cur_max;
		}		
		if (cur_max < 0) {
			cur = i;
			cur_max = array[i];
		}
		else
			cur_max += array[i];	
	}
	//printf("%d - %d\n\n", pre, pre_max);
	return pre_max;
}

int maximum_subarray_2(int* array, int left, int right)
{
	if (left >= right)
		return array[left];
	else {
		int middle = (left + right) / 2;
		// 一定会包含 array[middle] 这个元素
        int sum = 0, i = 0;
        int left_sum = (1 << 31);
		int right_sum = (1 << 31);
        // 左半边包含 array[middle] 元素，最多可以到什么地方
        // 走到最边界，看看最值是什么
        // 计算以 middle 结尾的最大的子数组的和
        // 右半边不包含 array[middle] 元素，最多可以到什么地方
        // 计算以 middle+1 开始的最大的子数组的和		
        for (sum = 0, i = middle; i >= left; i--) {
            sum += array[i];
            if (sum > left_sum)
                left_sum = sum;           
        }  
        for (i = middle + 1, sum = 0; i <= right; i++) {
            sum += array[i];
            if (sum > right_sum)
                right_sum = sum;       
        }
		int max_middle = left_sum + right_sum;	
		int max_left = maximum_subarray_2(array, left, middle);
		int max_right = maximum_subarray_2(array, middle + 1, right);		
		
		printf("%d - %d - %d\n", left, middle, max_left);
		printf("%d - %d - %d\n", middle, middle + 1, max_middle);
		printf("%d - %d - %d\n\n", middle + 1, right, max_right);
		
		printf("%d - %d - %d\n\n", max_left, max_middle, max_right);
		int max = (max_left > max_right) ? max_left : max_right;
		return max > max_middle ? max : max_middle;
	}
}

//螺旋矩阵
void spiral_matrix_i()
{
	int row = 3, col = 4;	
	int matrix[3][4] = {
		1,2,3,4,
		5,6,7,8,
		9,10,11,12};
	int i = 0, j = 0, buf = 0;
	int buf_row = row - 1, buf_col = col - 1;
	// [i][j] ~ [buf_row][buf_col]
	//画完一圈后，俩个顶点往对角中心缩一格
	while (i < buf_row && j < buf_col) {
		//画第一行
		for (buf = j; buf <= buf_col; buf++)
			printf("%d", matrix[i][buf]);
		//画最后一列
		for (buf = i +1; buf <= buf_row; buf++)
			printf("%d", matrix[buf][buf_col]);
		//画最后一行
		for (buf = buf_col - 1; buf >= j; buf--)
			printf("%d", matrix[buf_row][buf]);
		//画第一列
		for (buf = buf_row - 1; buf > i; buf--)
			printf("%d",matrix[buf][j]);		
	
		i++;
		buf_row--;
		j++;
		buf_col--;
	}
	
	if (i == buf_row && j == buf_col)
		printf("%d", matrix[i][j]);
	else if (i == buf_row)
		for (buf = j; buf <= buf_col; buf++)
			printf("%d", matrix[i][buf]);
	else if (j == buf_col)	
		for (buf = i; buf <= buf_row; buf++)
			printf("%d",matrix[buf][j]);
}

//跳跃游戏：能否跳到最后
//dp[i] = max(dp[i - 1], nums[i - 1]) - 1
int jump_game_i(int* array, int left, int right)
{
	int* dp = (int*)calloc((right - left + 1), sizeof(int));
	int result = -1, i = 0;
	for (dp[0] = array[left], i = 1; i < right - left + 1; i++)
		dp[i] = (dp[i - 1] > array[left + i - 1] ? \
			dp[i - 1] : array[left + i - 1]) - 1;
	result = dp[right - left] < 0 ? 0 : 1;
	free(dp);
	return result;
}


typedef struct interval {
	int interval_start;
	int interval_end;
	struct interval * next;
	struct interval * prev;
} interval;

//插入区间
struct interval * insert_one_interval(struct interval * queue, \
	int target_start,int target_end)
{
	//将结点插入
	if (queue == NULL) {
		//创建节点
		struct interval * buf = \
			(struct interval *)malloc(sizeof(struct interval));
		buf->interval_start = target_start;
		buf->interval_end = target_end;
		buf->next = NULL;
		buf->prev = NULL;			
		return buf;		
	} else {		
		struct interval * help = queue;
		//找到第一个区间起点比该节点大的位置
		while (help->next != NULL)
			if (help->interval_start	> target_start)
				break;
			else
				help = help->next;
		//找到了
		if (help->interval_start	< target_start) {
			//在help之后，理论上说，此时help后应该是NULL
			if (help->interval_end < target_start) {			
				//创建节点
				struct interval * buf = \
					(struct interval *)malloc(sizeof(struct interval));
				buf->interval_start = target_start;
				buf->interval_end = target_end;
				buf->next = NULL;
				buf->prev = NULL;	
				//区间不重合，直接插入
				help->next = buf;
				buf->prev = help;
			} else {
				//区间重合，直接修改结点即可，因为前面保证了插入区间起始点
				//不会跨过help结点
				help->interval_end = target_end > help->interval_end ? \
					target_end : help->interval_end;
			}
			return queue;		
		} else {
			struct interval * need_adjust = NULL;
			//在help之前，则此时，可能是最开始，也可能是中间
			if (help->prev == NULL) {
				//出现在开始，
				if (target_end < help->interval_start) {
					//创建节点
					struct interval * buf = \
						(struct interval *)malloc(sizeof(struct interval));
					buf->interval_start = target_start;
					buf->interval_end = target_end;
					buf->next = NULL;
					buf->prev = NULL;										
					//区间不重合，直接插入
					buf->next = help;
					help->prev = buf;
					return buf;
				} else {
					help->interval_start = target_start;	
					if (target_end <= help->interval_end)
						return queue;
					help->interval_end = target_end;
					//插入在头节点时，插入区间尾部大于后一区间尾部
					//可能会出现新的重合空间，需要对该冲突结点修改
					need_adjust = help;				
				}											
			} else {
				//出现在中间，有四种情况	
				//完全不重合
				if (help->prev->interval_end < target_start && \
					target_end < help->interval_start){
					//创建节点
					struct interval * buf = \
						(struct interval *)malloc(sizeof(struct interval));
					buf->interval_start = target_start;
					buf->interval_end = target_end;
					buf->next = NULL;
					buf->prev = NULL;										
					//区间不重合，直接插入
					help->prev->next = buf;
					buf->prev = help->prev;
					buf->next = help;
					help->prev = buf;
					return queue;
				//与前面重合与后面不重合
				} else if (help->prev->interval_end >= target_start && \
					target_end < help->interval_start) {
					help->prev->interval_end = target_end > help->prev->interval_end ? \
						target_end : help->prev->interval_end;
					return queue;					
				//与前面不重合与后面重合	
				} else if (help->prev->interval_end < target_start && \
					target_end >= help->interval_start) {
					help->interval_start = target_start;
					if (help->interval_end >= target_end)
						return queue;	
					help->interval_end = target_end;
					need_adjust = help;
				} else {					
					//都重合，此时区间向前合并，
					//保证help还和原来一样本身是不向后冲突的
					help->prev->interval_end = target_end;
					//
					need_adjust = help->prev;				
				}
				
				//最后需要调整了，因为冲突，使得其会向后冲突
				//优先队列保证其区间不可能向前冲突，但无法保证不向后冲突
				while (need_adjust != NULL) {
					if (need_adjust->next->interval_end <= \
						need_adjust->next->interval_end) {
						//一次调整就无需再调整
						need_adjust->next->interval_start = \
							need_adjust->interval_start;
						//删除need->adjust
						if (need_adjust->prev == NULL) {
							need_adjust->next->prev = NULL;
							queue = need_adjust->next;
						} else {
							need_adjust->next->prev = \
								need_adjust->prev;
							need_adjust->prev->next = \
								need_adjust->next;					
						}
						//移除结点
						free(need_adjust);
						return queue;												
					} else {
						//调整完后还需调整
						need_adjust->next->interval_start = \
							need_adjust->interval_start;
						need_adjust->next->interval_end = \
							need_adjust->interval_end;							
						need_adjust = need_adjust->next;
						//删除need->adjust
						if (need_adjust->prev == NULL) {
							need_adjust->next->prev = NULL;
							queue = need_adjust->next;
						} else {
							need_adjust->next->prev = \
								need_adjust->prev;
							need_adjust->prev->next = \
								need_adjust->next;					
						}
						//移除结点
						free(need_adjust);					
					}										
				}			
			}			
		}
	}	
} 

void insert_interval(int* array_start, int* array_end, int left, int right)
{
	//直接构造优先队列，利用数据
	int i = 0;
	struct interval * queue = NULL;
	for (i = left; i <= right; i++)  
		queue = insert_one_interval(queue, array_start[i], array_end[i]);
		
	struct interval * buf = queue;
	while (buf != NULL) {
		printf("%d - %d\n", buf->interval_start, buf->interval_end);
		buf = buf->next;
	}
	
	while (queue != NULL) {	
		buf = queue->next;
		free(queue);
		queue = buf;
	}	

}

//合并区间：可以以起始位置作参考，对区间排序，然后合并
//或者将所有区间，一个个用上面的方法插入

//填螺旋矩阵
void spiral_matrix_ii(int n)
{
	int i = 0, j = 0, buf = 0, start = 1;
	int buf_row = n - 1, buf_col = n - 1;
	int** matrix = (int**)calloc(n,sizeof(int*));
	for (buf = 0; buf < n; buf++)
		matrix[buf] = (int*)calloc(n,sizeof(int));
	
	// [i][j] ~ [buf_row][buf_col]
	//画完一圈后，俩个顶点往对角中心缩一格
	while (i < buf_row && j < buf_col) {
		//画第一行
		for (buf = j; buf <= buf_col; buf++)
			matrix[i][buf] = start++;
		//画最后一列
		for (buf = i +1; buf <= buf_row; buf++)
			matrix[buf][buf_col] = start++;
		//画最后一行
		for (buf = buf_col - 1; buf >= j; buf--)
			matrix[buf_row][buf] = start++;
		//画第一列
		for (buf = buf_row - 1; buf > i; buf--)
			matrix[buf][j] = start++;		
	
		i++;
		buf_row--;
		j++;
		buf_col--;
	}
	
	if (i == buf_row && j == buf_col)
		matrix[i][j] = start++;
	else if (i == buf_row)
		for (buf = j; buf <= buf_col; buf++)
			matrix[i][buf] = start++;
	else if (j == buf_col)	
		for (buf = i; buf <= buf_row; buf++)
			matrix[buf][j] = start++;
	
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++)
			printf("%d ", matrix[i][j]);
		printf("\n");
	}
		
}

//求全排列中的第k个序列，其中排列数是1-n的自然数
//使用位的递推公式计算即可

//机器人爬格子，一次只能爬一个格子
//dp[i][j] = dp[i - 1][j] + dp[i][j - 1]
int unique_path_i(int m,int n)
{
	int i = 0, j = 0, result = 0;
	int** dp = (int**)calloc(m + 1, sizeof(int*));
	for (i = 0; i < m + 1; i++)
		dp[i] = (int*)calloc(n + 1, sizeof(int));
	dp[0][1] = 1;//为了帮助起始位置给值的
	for (i = 1; i < m + 1; i++)
		for (j = 1; j < n + 1; j++)
			dp[i][j] = dp[i - 1][j] + dp[i][j - 1];	
	result = dp[m][n];
	for (i = 0; i < m + 1; i++)
		free(dp[i]);	
	free(dp);
	return result;
}

int unique_path_ii(int m,int n, int*obstacle_x, int*obstacle_y, int left, int right)
{
	int i = 0, j = 0, result = 0;
	int** dp = (int**)calloc(m + 1, sizeof(int*));
	for (i = 0; i < m + 1; i++)
		dp[i] = (int*)calloc(n + 1, sizeof(int));
	for (i = left; i <= right; i++)
		if (1 <= obstacle_x[i] && obstacle_x[i] < m + 1 && \
			1 <= obstacle_y[i] && obstacle_y[i] < n + 1) 
			dp[obstacle_x[i]][obstacle_y[i]] = -1;

	dp[0][1] = 1;//为了帮助起始位置给值的
	if (dp[1][i] = -1)
		return -1;
	for (i = 1; i < m + 1; i++)
		for (j = 1; j < n + 1; j++)
			if (dp[i][j] != -1)
				dp[i][j] = (dp[i - 1][j] != -1 ? dp[i - 1][j] : 0) + \
					(dp[i][j - 1] != -1 ? dp[i][j - 1] : 0);
	result = dp[m][n];
	for (i = 0; i < m + 1; i++)
		free(dp[i]);
	free(dp);
	return result;
}

//dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + chess_board[i][j]
int minimum_unique_path_i(int** chess_board, int m, int n)
{
	int i = 0, j = 0, result = 0;
	int** dp = (int**)calloc(m + 1, sizeof(int*));
	for (i = 0; i < m + 1; i++)
		dp[i] = (int*)calloc(n + 1, sizeof(int));
	for (i = 0; i < m + 1; i++)
		dp[i][0] = ~(1 << 31);
	for (j = 1; j < n + 1; j++)
		dp[0][j] = ~(1 << 31);	
	dp[0][1] = 0;//为了帮助起始位置给值的
	for (i = 1; i < m + 1; i++)
		for (j = 1; j < n + 1; j++)
			dp[i][j] = dp[i - 1][j] < dp[i][j - 1] ? \
				dp[i - 1][j] : dp[i][j - 1] + chess_board[i][j];	
	result = dp[m][n];
	for (i = 0; i < m + 1; i++)
		free(dp[i]);	
	free(dp);
	return result;
}

//判断是否是合法数字
bool vaild_number(char* str)
{
	//以下情况不知其完备性
	int first_touch_number = 0;
	char * p_str = str;
	for (p_str = str;*p_str != '\0'; p_str++) {
		//普通数字，无区别对待
		if ('1' <= *p_str && *p_str <= '9') {
			first_touch_number = 1;
			continue;
		}
		//最开始的数字部分不应该有连续的0,第一次的0不能直接接数
		if ('0' == *p_str)
			if (first_touch_number == 0) {
			if (*(p_str + 1) != 'e' && \
				('0' > *(p_str + 1) || *(p_str + 1) > '9'))		
			continue;
		} else 
			continue;
		
		//小数点俩边必有数字
		if ('.' == *p_str) {
			if ((p_str > str && '0' <= *(p_str - 1) && \
				*(p_str - 1) <= '9') && ('0' <= *(p_str + 1) && \
				*(p_str + 1) <= '9')) 
				continue;
		}
		//指数左边一定有数字，右边可能有数字或者负号
		if ('e' == *p_str) {
			if ((p_str > str && '0' <= *(p_str - 1) && \
				*(p_str - 1) <= '9' ) && (('0' <= *(p_str + 1) && \
				*(p_str + 1) <= '9') || *(p_str + 1) == '-')) {
					first_touch_number = 0;
					continue;	
				}						
		}
		//最开始可能有负号，不能有连续负号
		//负号左边只可能是指数或者没有，负号右边只能是非0数字
		if ('-' == *p_str) {
			if (((p_str == str) || *(p_str - 1) == 'e') && \
				('1' <= *(p_str + 1) && *(p_str + 1) <= '9'))
				continue;
		}
		//跳过全部的continue说明存在问题
		return 0;
	}
	
	return 1;	 
}

//二进制求和
char half_bit_add(char* a, char* b)
{
	if (*a == '0' && *b == '0')
		return '0';
	else if ((*a == '1' && *b =='0') || \
		(*a == '0' && *b == '1')) {
		*a = '1';
		*b = '0';
		return '0';
	} else if (*a == '1' && *b == '1') {
		*a = '0';
		*b = '0';
		return '1';		
	}
	return '0';
}

char full_bit_add(char* a, char* b, char* c)
{
	if (*a == '0' && *b == '0' && *c == '0') {
		return '0';
	} else if ((*a == '1' && *b == '0' && *c == '0') || \
		(*a == '0' && *b == '1' && *c == '0') || \
		(*a == '0' && *b == '0' && *c == '1')) {
		*a = '1';
		*b = '0';
		*c = '0';
		return '0';
	} else if ((*a == '1' && *b == '1' && *c == '0') || \
		(*a == '0' && *b == '1' && *c == '1') || \
		(*a == '1' && *b == '0' && *c == '1')) {
		*a = '0';
		*b = '0';
		*c = '1';			
		return '1';
	} else if (*a == '1' && *b == '1' && *c == '1') {
		*a = '1';
		*b = '0';
		*c = '1';		
		return '1';
	}
	return '0';
}

char* calculate_bit_sum(char* str1, char* str2, int* result)
{
	int count1 = 0, count2 = 0;
	char* str_a = str1, *str_b = str2, str_c = 0;
	while (*str_a++ != '\0')
		count1++;
	while (*str_b++ != '\0')
		count2++;	
	//指向最后一个字符串
	//str_a--;
	//str_b--;	
	
	if (count1 < count2) {
		//表示str_a指向长度最长的一个二进制字符数组
		char* temp = str_a;
		str_a = str_b;
		str_b = temp;
		
		while (str_b >= str1) {
			str_c = full_bit_add(str_a, str_b, &str_c);				
			str_a--;
			str_b--;			
		}	
					
		while (str_a >= str2&& str_c != '0') {
			str_c = half_bit_add(str_a, &str_c);	
			str_a--;			
		}
					
		if (str_c == '1') 
			*result = 1;
		else
			*result = 0;
		return str2;	
	} else {
		while (str_b >= str2) {
			str_c = full_bit_add(str_a, str_b, &str_c);				
			str_a--;
			str_b--;			
		}		
		
		while (str_a >= str1 && str_c != '0') {
			str_c = half_bit_add(str_a, &str_c);	
			str_a--;			
		}	
		
		if (str_c == '1') 
			*result = 1;		
		else
			*result = 0;
		return str1;
	}
}

//俩端对齐，字符串不能太长
void text_justification(char* str, int max_length)
{
	char* p_str_start = str, * p_str_end = str;		
	while (*p_str_end != '\0') {
		//移动p_str_end到于p_str_start相距max_length个字节的距离
		//如果可以移动这么多的话
		int count = 0, count_space = 0, offset = 0, offset1 = 0, i = 0;
		while (count < max_length && *p_str_end != '\0') {
			if (*p_str_end == ' ')
				count_space++;							
			p_str_end++; 
			count++;
		}
		//最后始终会放弃一个空格
		count_space--;	
		//找到了一行，现在只需要调整最后的格子		
		if (count == max_length && *p_str_end != '\0') {
			if (count_space <= 0) {
				printf("errno: the word is out of enge\n");
				return;
			}			
			if (*p_str_end == ' ') {
					p_str_end--;
					offset1 = 1;				
			} else {
				while (p_str_end >= str && *p_str_end != ' ') {
					p_str_end--;
					offset++;						
				}
				p_str_end--;					
				offset1 = offset % count_space;		
			}			
			offset /= count_space;	
			while (*p_str_start != '\0' && \
				p_str_start <= p_str_end) {
				//遇到字符串中的空格插入补足空格，否则不操作
				if (*p_str_start == ' ') {				
					for (i = 0; i < offset; i++) 
						printf(" ");
					if (offset1-- >= 1)
						printf(" ");
				}
				printf("%c", *p_str_start++);
			}
			//跳过后面的空格，指向下一个单词开始
			while (*(++p_str_end) == ' ');	
			p_str_start = p_str_end;
			printf("\n");		
			
		} else {
			while (*p_str_start != '\0')
				printf("%c", *p_str_start++);
			break;		
		}	
	}	
}

//求平方根，牛顿迭代
// a = x^2 => fx = x^2 - a ==>  约等于 0时
//使用斜率公式fx - fx0 = f‘x * (x - x0) ，当在零点时fx为0
//利用fx为0获得递推公式
//递推公式为xn1 = (xn + x / xn) / 2
float calculate_sqrt(int x)
{
	float xn1 = 1.0 ,xn = 0;
	do {
		xn = xn1;
		xn1 = (xn + x / xn) / 2;		
	} while (abs(xn1 - xn) > 1e-5);
	return xn1;
}

//爬楼梯，一次走1步或2步
int climbing_stairs(int n)
{
	int i = 0, result = 0;
	int * dp = (int*)calloc(n + 1, sizeof(int));
	dp[0] = 0;
	dp[1] = 1;
	for (i = 2; i < n + 1; i++)
		dp[i] = dp[i - 1] + dp[i - 2];
	result = dp[n];
	free(dp);
	return result;
}

//规范路径，将路径标准化
void simplify_path(char* str)
{
	int count = 0, i = 0, j = 0;
	char* buf = str;
	while (*buf++ != '\0')
		count++;
	int index_start = 0,index_end = 0;
	int stack_index = -1,temp = 0;
	char* str_buf = (char*)calloc(count + 1, sizeof(char));
	int* stack_index_start = (int*)calloc(count + 1, sizeof(int));
	int* stack_index_end = (int*)calloc(count + 1, sizeof(int));
	
	while (index_start < count + 1 && index_end < count + 1) {
		if (str[index_start] == '/') {
			//摸到一个/查看栈中是否存在冲突
			if (stack_index < 0 || str[stack_index_start[stack_index]] != '/') {
			//无冲突，将其放入栈中
				++stack_index;
				stack_index_start[stack_index] = index_start;
				stack_index_end[stack_index] = index_end;			
			}	
			index_start++;				
			index_end = index_start;				
		} else if (str[index_start] == '.') {
			//遇到点，如果是一个直接丢了,即不管它
			//遇到..先不管，弹栈再说，如果可以的话
			if (index_start + 1 < count + 1 && str[index_start + 1] == '.') {			
				int flag = 0;
				while (stack_index >= 0) {
					//至少弹掉一个字符串路径，反正不是字符串路径就是/
					if (str[stack_index_start[stack_index]] != '/') {
						stack_index--;						
						break;
					}
						stack_index--;	
				}
				//会不会弹掉根？
				if (stack_index < 0)
					stack_index = 0;					
			}	
			index_start++;				
			index_end = index_start;				
		} else {
			//将名字入栈，目前index_start指向名字头
			if (index_end < count + 1)
				index_end++;
			if (index_end >= count + 1 || str[index_end] == '/') {
				//入栈字符串
				++stack_index;
				stack_index_start[stack_index] = index_start;
				stack_index_end[stack_index] = index_end - 1;				
				index_start = index_end;
			}		
		}		
	}
	
	for (i = 0; i <= stack_index ; i++) {
		for (j = stack_index_start[i]; j <= stack_index_end[i]; j++) {
			str_buf[temp++] = str[j];
		}
	}

	for (i = 0; str_buf[i] != '\0'; i++)
		str[i] = str_buf[i];
	str[i] = '\0';
	if (str_buf[i - 1] == '/')
		str[i - 1] = '\0';

	printf("%s ", str);
	
	free(str_buf);
	free(stack_index_start);
	free(stack_index_end);
	
}

//字符串转化
/*转移方程：
//dp[i][j]表示字符串1匹配前i个字符，字符串2匹配前j个字符所需最小操作数
1. s1[ i ] == s2[ j ] 此时不需要做变化，dp[ i ] [ j ] = dp[ i -1 ] [ j -1 ]。
2. s1[ i ] ! = s2[ j ]分为三种情况：
第一种：删除s1[ i ]， 那么dp[ i ][ j ]  = dp [ i -1 ] [ j ] + 1
第二种：替换s1[ i ]，那么dp[ i ] [ j ]  = dp [ i -1 ] [ j -1 ] + 1
第三种：插入元素，那么dp[ i ] [ j ] = dp[ i ] [ j -1 ] + 1
*/			   
int edit_distance(char* str1, char* str2)
{
	int count1 = 0, count2 = 0, i = 0, j = 0, result = 0;
	char* buf_str1 = str1, * buf_str2 = str2;
	while (*buf_str1++ != '\0') 
		count1++;
	while (*buf_str2++ != '\0') 
		count2++;	
	int** dp = (int**)calloc(count1 + 1, sizeof(int*));
	for (i = 0; i < count1 + 1; i++) 
		dp[i] = (int*)calloc(count2 + 1, sizeof(int));

	for (i = 0; i < count1 + 1; i++)
		dp[i][0] = i;
	for (j = 0; i < count2 + 1; j++)
		dp[0][j] = j;	
	
	for (i = 1; i < count1 + 1; i++)
		for (j = 1; j < count2 + 1; j++)
			if (str1[i - 1] == str2[j - 1])
				dp[ i ][ j ] = dp[i - 1][j - 1];
			else {
				int min = dp[i - 1][ j ] < dp[i - 1][j - 1] ? \
					dp[i - 1][j] : dp [i - 1][j - 1];
				min = min < dp[i][j - 1] ? min : dp[i][j - 1];
				dp[i][j] = min + 1;
			}
	
	/*
	for (i = 0; i < count1 + 1; i++) {
		for (j = 0; j < count2 + 1; j++)	
			printf("%d", dp[i][j]);
		printf("\n");
	}
	*/
	
	result = dp[count1][count2];
	for (i = 0; i < count1 + 1; i++) 
		free(dp[i]);		
	free(dp);	
	return result;
}

//矩阵清零，按行扫描，遇到0清空该行与该列
//然后矩阵被划分成俩个部分在左右下角，递归分治不知可行

//矩阵数据按列左至右，行上至下排序规则
//二分查找特定数据：先对第一列进行二分查找，然后对
//恰好小于该数的列开始二分遍历其行查找

//颜色分类:共红0白1蓝2三种颜色，以数字代替
//只需要把所有0换到前面，所有2换到后面即可
void sort_colors(int* array, int left, int right)
{
	int old_left = left, old_right = right;
	int start = left, end = right, temp = 0;
	while (start <= old_right) {
		if (array[start] == 0) {
			//交换left与start
			temp = array[left];
			array[left] = array[start];
			array[start] = temp;
			left++;
		}
		start++;		
	}
	while (end >= old_left) {	
		if (array[end] == 2) {
			//交换right与end
			temp = array[right];
			array[right] = array[end];
			array[end] = temp;
			right--;				
		}
		end--;	
	}
	
	while (old_left <= old_right)
		printf("%d", array[old_left++]);
	
}

//寻找包含str2中所有字符的最短str1部分
void minimum_window_substring(char* str1, char* str2)
{
	char* buf_str1 = str1, *buf_str2 = str2;
	char* buf_start = str1;
	char* need_help = buf_str1;
	int min_length = ~(1 << 31);	
	char* min_start = NULL,* min_end = NULL;
	
	bool* the_alphabet_table = (bool*)calloc(26, sizeof(bool));
	int* is_used =  (int*)calloc(26, sizeof(int));
	while (*buf_str2 != '\0' && 'a' <= *buf_str2 && *buf_str2 <= 'z') 
		the_alphabet_table[*buf_str2++ - 'a'] = 1;
	
	while (*buf_str1 != '\0') {
		//遇到敏感字，就需要处理，普通字符跳过即可
		if (the_alphabet_table[*buf_str1 - 'a'] == 1) {
			//记录本敏感字出现次数
			is_used[*buf_str1 - 'a']++;
			//出现重复数据，可能需要调整
			if (is_used[*buf_str1 - 'a'] > 1) {
				//考虑调整，游标向后移直到遇见特殊情况
				while (*buf_start != '\0' && buf_start <= buf_str1) {
					//遇到敏感字	
					if (the_alphabet_table[*buf_start - 'a'] == 1) {
						//遇到敏感字，看是否可以缩小
						if (is_used[*buf_start - 'a'] == 1)
							break;							
						else 
							is_used[*buf_start - 'a']--;				
					}
					//累赘，需要放弃
					buf_start++;				
				}
				//跳转完成，检查新的字节串是否满足需求
				if (buf_str1 - buf_start <= min_length) {
					min_length = buf_str1 - buf_start;
					min_start = buf_start;
					min_end = buf_str1;
				}			
			}			
		}
		buf_str1++;
	}
	
	if ((min_start == NULL || min_end == NULL) ||
		min_start > min_end)
		printf(" ");
	else {
		while (min_start <= min_end)
			printf("%c", *min_start++);
	}
	
	free(the_alphabet_table);
	free(is_used);
}

//组合数，求数据组合
//求全部组合，从n中取m个数
int help_search_all_combination(int* array, int left, int right, \
	int* temp, int number, int range, int index)
{
	if (number >= range) {
		int i = 0;
		for (i = left; i < range; i++)
			printf("%d ",temp[i - left]);
		printf("\n");
	} else {
		int i = 0, j = 0;
		for (i = index; i <= right; i++) {
			temp[number] = array[i];			
			//下层约束，下层永远只可取该层后面的数据
			for (j = i + 1; j <= right; j++) {
				if (1 == help_search_all_combination( \
					array, left, right, temp, number + 1, range, j))
					break;
			}
		}		
	}
	return 1;
}

void combination_from_n_get_m(int* array, int left, int right, int x)
{
	//先对数据进行排序
	quick_sort(array, left, right);
	int n = right - left + 1;
	if (x > n)
		x = n;
	int* temp = (int*)calloc((right - left + 1), sizeof(int));
	help_search_all_combination(array, left, right, temp, 0, x, 0);	

}

//子集合，求全部的子集合
int help_search_all_subset_i(int* array, int left, int right, \
	int* temp, int number, int index)
{
	if (number <= right) {
		int i = 0, j = 0;
		for (i = index; i <= right; i++) {
			temp[number] = array[i];				
			//下层约束，下层永远只可取该层后面的数据
			for (j = i + 1; j <= right; j++) {
				if (1 == help_search_all_subset_i( \
					array, left, right, temp, number + 1, j))
					break;
			}
			
			//打印迄今为止的子集合
			int i = 0;
			for (i = left; i <= number; i++)
				printf("%d ",temp[i - left]);
			printf("\n");				
		}		
	}
	return 1;
}

void search_all_subset_i(int* array, int left, int right)
{
	if (right <= left) {
		printf("only have empty set");
		return;
	}
	//先对数据进行排序
	quick_sort(array, left, right);
	int* temp = (int*)calloc((right - left + 1), sizeof(int));
	help_search_all_subset_i(array, left, right, temp, 0, 0);	

}

//单词搜索
int help_word_search(char* two_dimensional_grid, bool* is_used, \
	int row, int col, int current_row, int current_col, char* str_point, int flag)
{
	//特殊情况，这里是第一层，表示已经开始了
	if (flag == 0) {
		flag = 1;
		int i = 0, j = 0, result = 0;
		for (i  = 0; i < row; i++)
			for (j  = 0; j < col; j++)
				if (two_dimensional_grid[i * col + j] == *str_point) {
				//寻到一个可能的起始点了，开始吧			
				is_used[i * col + j] = 1;
				result += help_word_search(two_dimensional_grid, is_used, \
					row, col, i, j, str_point, flag);
				//现场还原
				is_used[i * col + j] = 0;					
		}
		return result;
	} else {
		//寻枝，利用从上面来的坐标，以其为中心寻找下一个点，如果可能的话
		int i = current_row, j = current_col;
		if (two_dimensional_grid[i * col + j] == *str_point) {		
			if (*(str_point + 1) == '\0') {
				//前面全匹配好了，找到啦		
				int i = 0, j = 0;
				for (i  = 0; i < row; i++)
					for (j  = 0; j < col; j++)
						if (is_used[i * col + j] == 1) 
							printf("xray:%d - yray%d\n", i, j);
				printf("\n");
				return 1;					
			} else {
				//子孙自有子孙福
				int result = 0;
				is_used[i * col + j] = 1;			
				if (i - 1 >= 0)
					result += help_word_search(two_dimensional_grid, is_used, \
						row, col, i - 1, j, str_point + 1, flag);
				if (i + 1 < row)
					result += help_word_search(two_dimensional_grid, is_used, \
						row, col, i + 1, j, str_point + 1, flag);						
				if (j - 1 >= 0) 						
					result += help_word_search(two_dimensional_grid, is_used, \
						row, col, i, j - 1, str_point + 1, flag);
				if (j + 1 < col)						
					result += help_word_search(two_dimensional_grid, is_used, \
						row, col, i, j + 1, str_point + 1, flag);				
				//现场还原		
				is_used[i * col + j] = 0;	
				return result;			
			}  		
		}
		return 0;
	}
}

int word_search(char* two_dimensional_grid, int row, int col, char* str)
{
	bool* is_used = (bool*)calloc(row * col, sizeof(bool));
	int result = help_word_search(two_dimensional_grid, is_used, \
		row, col, 0, 0, str, 0);
	free(is_used);
	return result;
}

//删除量多余2的项
int remove_duplications_from_sorted_array_ii(int* array, int left, int right)
{
	//删除重复项使其项小于等于2
	//因为已经排序，所以可以夸单位比较
	//理论上说出现诸如array[i] = array[i + 2]的情况下
	//需要到后面找非等于的情况，然后向前覆盖
	int result = right - left + 1, i = 0;
	for (i = left; i <= right; ++i) {
		if (i + 2 <= right && array[i] == array[i + 2]) {
		//出现多余重复项
		//现在起，array[i + 2]及其后面相同的数据都需要被后面的数据覆盖掉
		int j = i + 2, t = i + 2;

		while (j <= right && array [j] == array[i])
			j++;
		result -= j - i - 2;
		
			if (j > right) {
				//无法覆盖了
				break;
			} else {
				while (j <= right)
					array[t++] = array[j++];
				i = i + 1;
			}
		}	
	}

	for (i = left; i <= right; ++i)
		printf("%d ", array[i]);
	printf("\n");
	
	return result;
}

//查找已经旋转的数组中某一值，数组数据会重复
//未测试
int search_data_from_rotated_array_3(int* array, int left, int right, int x)
{
	if (left > right)
		return -1;
	else if (left == right)
		return array[left] = x ? left : -1;
	else {
		int fake_center = (left + right) / 2;
		while (array[fake_center] == array[left] && \
			array[fake_center] == array[right]) {
			if (left < right) {
				left++;
				right--;
			} else 
				return -1;			
		}		
		if (array[fake_center] == x)
			return fake_center;
		//假如left ~ center为有序数组
		if (array[left] <= array[fake_center]) {
			//如果满足x在范围内
			if (array[left] <= x && x <= array[fake_center]) 
				return search_data_from_rotated_array_2(array, left, fake_center - 1, x);
			else
				return search_data_from_rotated_array_2(array, fake_center + 1, right, x);	
		} else {
		//if (array[fake_center] < array[right]) {
			if (array[fake_center] < x && x <= array[right]) 
				return search_data_from_rotated_array_2(array, fake_center + 1, right, x);
			else							
				return search_data_from_rotated_array_2(array, left, fake_center - 1, x);			
		}		
	}
}

//求直方图的矩形面积
//神奇的思想，使用递增栈巧妙完成横向最大距离求举
int calculate_largest_rectangle_in_histogram(int* array, int left, int right)
{
	int result = 0, stack_index = -1, index = left, help_index = index, flag = 0;
	int* simulate_stack = (int*)calloc(right - left + 1, sizeof(int));
	int* help_simulate_stack = (int*)calloc(right - left + 1, sizeof(int));
	
	for (index = left; index <= right || flag == 0; index++) {
		int current_data = 0;
		//数据有效时，取有效数据，到最后还要取一次0清空栈的
		if (index <= right) 
			current_data = array[index];
		else
			flag = 1;
		printf("current_data is :%d \n", current_data);
		//取值，与栈顶元素比较
		if (stack_index == -1 || \
			simulate_stack[stack_index] <= current_data) {
			//栈顶不存在数据，数据入栈
			//当前数据大于栈顶数据，数据入栈
			simulate_stack[++stack_index] = current_data;
			help_simulate_stack[stack_index] = index;
		} else {
			//现在出现了冲突需要处理了
			int flag_break = 0;
			while (flag_break == 0 && \
				simulate_stack[stack_index] > current_data) {
				int current_high, current_width, current_size;
				//只有一个元素了
				if (stack_index == 0) {
					//取元素且元素出栈
					current_high = simulate_stack[stack_index--];
					current_width = index;
					current_size = current_width * current_high;
					if (current_size > result)
						result = current_size;
					flag_break = 1;
				} else {
					//取元素且元素出栈
					current_width = \
						index - help_simulate_stack[stack_index - 1] - 1; 
					current_high = simulate_stack[stack_index--];					
					current_size = current_width * current_high;					
					if (current_size > result)
						result = current_size;		
				}	
				printf("pop: high:%d;width:%d;size:%d \n", \
					current_high, current_width, current_size);
			}
			//当前数据大于栈顶数据，数据入栈
			simulate_stack[++stack_index] = current_data;
			help_simulate_stack[stack_index] = index;		
		}		
	}	
	free(help_simulate_stack);
	//free(simulate_stack);
	return result;
}

//求一个（0,1）二维矩阵最大的矩形，即相邻1形成的矩形
int calculate_maximum_rectangle(char* array, int row, int col)
{
	int i = 0, j = 0, result = 0;
	int* left_rectangle = \
		(int*)calloc((row + 1) * (col + 1), sizeof(int));
	int* right_rectangle = \
		(int*)calloc((row + 1) * (col + 1), sizeof(int));
	int* height_rectangle = \
		(int*)calloc((row + 1) * (col + 1), sizeof(int));
	int* area_rectangle = \
		(int*)calloc((row + 1) * (col + 1), sizeof(int));
	
	for (i = 0; i < row + 1; i++) 
		right_rectangle[i * (col + 1) + 0] = ~(1 << 31);
	for (j = 0; j < col + 1; j++) 
		right_rectangle[0 * (col + 1) + j] = ~(1 << 31);		
		
	//处理left，一行一行处理，从左至右处理	
	for (i = 1; i < row + 1; i++) {
		int current_left = 0;
		for (j = 1; j < col + 1; j++)
			if (array[(i - 1) * col + (j - 1)] == '0') {
				current_left = j + 1 - 1;
				left_rectangle[i * (col + 1) + j] = 0;
			} else {
				left_rectangle[i * (col + 1) + j] = ((current_left >= \
				left_rectangle[(i - 1) * (col + 1) + j]) ? \
				current_left : left_rectangle[(i - 1) * (col + 1) + j]);			
			}
	}
	
	//处理right，一行一行处理，从右至左处理	
	for (i = 1; i < row + 1; i++) {
		int current_right = col;
		for (j = col; j >= 1; j--)			
			if (array[(i - 1) * col + (j - 1)] == '0') {
				current_right = j - 1;
				right_rectangle[i * (col + 1) + j] = col;
			} else {
				right_rectangle[i * (col + 1) + j] = ((current_right <= \
				right_rectangle[(i - 1) * (col + 1) + j]) ? \
				current_right : right_rectangle[(i - 1) * (col + 1) + j]);							
			}
	}
	
	//处理height，一行一行处理，从左至右处理
	for (i = 1; i < row + 1; i++)
		for (j = 1; j < col + 1; j++)			
			if (array[(i - 1) * col + (j - 1)] == '0') 
				height_rectangle[i * (col + 1) + j] = 0;
			else
				height_rectangle[i * (col + 1) + j] = 1 + \
					height_rectangle[(i - 1) * (col + 1) + j];							
	
	//处理area，一行一行处理，从左至右处理
	for (i = 1; i < row + 1; i++)
		for (j = 1; j < col + 1; j++) {		
			area_rectangle[i * (col + 1) + j] =  height_rectangle[i * (col + 1) + j] * \
				(right_rectangle[i * (col + 1) + j] - left_rectangle[i * (col + 1) + j]);
			if (area_rectangle[i * (col + 1) + j] > result)
				result = area_rectangle[i * (col + 1) + j];
		}
	
	//
	for (i = 1; i < row + 1; i++) {
		for (j = 1; j < col + 1; j++) 
			printf("%d ", left_rectangle[i * (col + 1) + j]);
		printf("\n");
	}
	printf("\n");
	for (i = 1; i < row + 1; i++) {
		for (j = 1; j < col + 1; j++) 
			printf("%d ", right_rectangle[i * (col + 1) + j]);
		printf("\n");
	}	
	printf("\n");
	for (i = 1; i < row + 1; i++) {
		for (j = 1; j < col + 1; j++) 
			printf("%d ", height_rectangle[i * (col + 1) + j]);
		printf("\n");
	}	
	printf("\n");
	for (i = 1; i < row + 1; i++) {
		for (j = 1; j < col + 1; j++) 
			printf("%d ", area_rectangle[i * (col + 1) + j]);
		printf("\n");
	}
	printf("\n");
	
	
	free(left_rectangle);	
	free(right_rectangle);
	free(height_rectangle);	
	free(area_rectangle);

	return result;
}

//单次分割链表（快速排序的思想）
//无论分开调整还是原地调整等，都是差不多的，
//因为如果数据越规整，原地调整效率就高，否则分开调整效率就高
//原地调整采用双指针法，慢指针寻找一个比目标小的，快指针寻找
//一个比目标大的，调整即可，最后调整分割点
//分割调整更直观简单，为了提高效率要使用六个指针维护分割
//俩俩一组，分成三组，一组头尾指针保存拆下来的小于目标值的数据
//一组头尾指针保存拆下来的大于目标值的数据
//一组头尾指针保存拆下来的等于目标值的数据（采用这组指针可以方便
//在递归之时寻找分割点，因为如果数据重复度高，该步骤会提高效率，在
//普通的情况下，可以减少复杂且难理解的算法，提高开发速度）

//格雷码
//可以采用时钟脉冲的方式去实现(计算机硬件如此实现)
//或者使用算法公式G(i) = i ^ (i/2);
void show_gray_code(int n)
{
	if (n > 30 || n < 0)
		return;
	int i = 0;
	for (i = 0; i < (1 << n); i++) 
		printf("%d", i ^ (i / 2));	
}

//搅乱字符串，将字符串，以二叉树的形式翻转
//存在点使其分成俩部分，这俩部分都会是scramble的
//遍历字符串找到一个点，恰好使得俩部分都是scramble的即可
//其中最好的比较是否是scramble是对其字符串排序
//未完成，待定

//子集合，求全部的子集合
int help_search_all_subset_ii(int* array, int left, int right, \
	int* temp, int number, int index)
{
	if (number <= right) {
		int i = 0, j = 0, last_data = ~(1 << 31);
		for (i = index; i <= right; i++) {	
			//相同的数据，在同一层只能使用一次		
			if (last_data == array[i])
				continue;
			else {
				last_data = array[i];			
			}
			temp[number] = array[i];	
		
			//下层约束，下层永远只可取该层后面的数据
			for (j = i + 1; j <= right; j++) {
				if (1 == help_search_all_subset_ii( \
					array, left, right, temp, number + 1, j))
					break;
			}
			
			//打印迄今为止的子集合
			int i = 0;
			for (i = left; i <= number; i++)
				printf("%d ",temp[i - left]);
			printf("\n");					
		}		
	}
	return 1;
}

void search_all_subset_ii(int* array, int left, int right)
{
	if (right <= left) {
		printf("only have empty set");
		return;
	}
	//先对数据进行排序
	quick_sort(array, left, right);
	int* temp = (int*)calloc((right - left + 1), sizeof(int));
	help_search_all_subset_ii(array, left, right, temp, 0, 0);	
}

//未做防错检查，字符串只能有0 - 9的数字
int decode_ways(char* str)
{
	char* buf = str;
	int count = 0, i = 0, result = 0;
	while (*buf++ != '\0')
		count++;
	int* dp = (int*)calloc(count, sizeof(int));
	//初始化，取前俩个字符比较即可
	//开始就出现0，完蛋
	if (str[0] == '0')
		return 0;
	else
		dp[0] = 1;
	//开始为1，本字符任意匹配都行
	if (str[0] == '1')
		dp[1] = 2;
	//开始为2，本字符受限匹配
	if (str[0] == '2' && ('0' <= str[1] && str[1] <= '6'))
		dp[1] = 2;
	else 
		dp[1] = 1;
	
	for (i = 2; i < count ; i++) {
		//如果本位为0，则前一位只能为1,2
		if (str[i] == '0') {			
			if (str[i - 1] == '1' || str[i - 1] == '2')
				dp[i] = dp[i - 2];			
			else 
				return 0;
			continue;
		}	
		//如果本位为1 - 6 ，前一位为1,2
		if ('1' <= str[i] && str[i] <= '6') {
			if (str[i - 1] == '1' || str[i - 1] == '2')
				dp[i] = dp[i - 1] + dp[i - 2];
			else 
				dp[i] = dp[i - 1];
			continue;
		}
		//如果本位为7以上，前一位为1
		if ('1' <= str[i] && str[i] <= '6')  {
			if (str[i - 1] == '1')
				dp[i] = dp[i - 1] + dp[i - 2];			
			else
				dp[i] = dp[i - 1];
			continue;
		}	
	}
	
	for (i = 0; i < count; i++)
		printf("%d ", dp[i]);

	result = dp[count - 1];
	free(dp);
	return result;
}

//复原ip地址
int help_str_to_num(char* str, int str_left, int str_right)
{
	int result = 0;
	while (str_left <= str_right) {
		result *= 10;
		if ('0' <= str[str_left] <= '9')
			result += str[str_left] - '0';
		else
			return -1;
		str_left++;
	}
	return result;
}

void help_restore_ip_address(char* str, int str_left, int str_right, 
	int* temp, int index)
{
	if (index >= 4) {
		if (str_left > str_right) {
			int i = 0;
			for (i = 0; i < 4; i++)
				printf("%d - ", temp[i]);
			printf("\n");			
		}
	} else {
		//从中取一个合适的数据
		int t = str_left, data = 0;
		while (1) {
			data = help_str_to_num(str, str_left, t);
			//取到一个合理的值		
			if (!(0 <= data && data <= 255))
				break;
			//存下该值
			temp[index] = data;
			//去下一层
			help_restore_ip_address(str, t + 1, str_right, 
				temp, index + 1);
			//选下一个值
			t++;	
		}
	}
}

void restore_ip_address(char* str)
{
	int count = 0;
	char* buf = str;
	int temp_ip[4] = {0};
	while (*buf++ != '\0')
		count++;
	help_restore_ip_address(str, 0, count - 1, temp_ip, 0);	
}

//不同的二叉搜索树
//一个由1-n的数据组成的二叉搜索树
//则按数据插入多少为层次
//设计好二叉树，插入与移除函数
//循环从头到尾，设置标志哪些数据被使用了
//每一层插入不同的数据
//这些数据都是从剩余可使用数据中取出

//假设n个节点存在二叉查找树的个数是G(n)
//1为根节点，2为根节点，…，n为根节点
//当1为根节点时，其左子树节点个数为0，右子树节点个数为n-1
//当2为根节点时，其左子树节点个数为1，右子树节点为n-2
//所以可得G(n)=G(0)∗G(n−1)+G(1)∗(n−2)+...+G(n−1)∗G(0) G(n)=G(0)∗G(n−1)+G(1)∗(n−2)+...+G(n−1)∗G(0)      G(n) = G(0) *G(n-1)+G(1)*(n-2)+...+G(n-1)*G(0)<math><semantics><mrow><mi>G</mi><mo stretchy="false">(</mo><mi>n</mi><mo stretchy="false">)</mo><mo>=</mo><mi>G</mi><mo stretchy="false">(</mo><mn>0</mn><mo stretchy="false">)</mo><mo>∗</mo><mi>G</mi><mo stretchy="false">(</mo><mi>n</mi><mo>−</mo><mn>1</mn><mo stretchy="false">)</mo><mo>+</mo><mi>G</mi><mo stretchy="false">(</mo><mn>1</mn><mo stretchy="false">)</mo><mo>∗</mo><mo stretchy="false">(</mo><mi>n</mi><mo>−</mo><mn>2</mn><mo stretchy="false">)</mo><mo>+</mo><mi mathvariant="normal">.</mi><mi mathvariant="normal">.</mi><mi mathvariant="normal">.</mi><mo>+</mo><mi>G</mi><mo stretchy="false">(</mo><mi>n</mi><mo>−</mo><mn>1</mn><mo stretchy="false">)</mo><mo>∗</mo><mi>G</mi><mo stretchy="false">(</mo><mn>0</mn><mo stretchy="false">)</mo></mrow><annotation encoding="application/x-tex">G(n) = G(0) *G(n-1)+G(1)*(n-2)+...+G(n-1)*G(0)</annotation></semantics></math>G(n)=G(0)∗G(n−1)+G(1)∗(n−2)+...+G(n−1)∗G(0)
//即G --》 dp，其中n为0或1都是存在值为1的数
int calculate_all_unique_binary_search_trees(int n)
{
	int* dp = (int*)calloc(n + 1, sizeof(int));
	int i = 0, result = 0, k = 0;
	dp[0] = 1;
	dp[1] = 1;
	for (i = 2; i < n + 1; i++) {
		//左右子树对称，其实是相同的，但人为分了左右使其不同	
		for (result = 0, k = 0; k < i; k++)
			//不包括本节点
		if (i - 1 - k >= 0)
			dp[i] += dp[k]  * dp[i - 1 - k];
	}
	
	for (i = 0; i < n + 1; i++)
		printf("%d ", dp[i]);
	
	result = dp[n];
	free(dp);
	return result;
}

//字符串交错，匹配
//使用动态规划
int match_interleaving_str(char* str1, char* str2, char* str_all)
{
	int count1 = 0, count2 = 0, count_all = 0;
	int i = 0, j = 0, result = 0;
	char* buf = str1;
	while (*buf++ != '\0')
		count1++;
	buf = str2;
	while (*buf++ != '\0')
		count2++;
	buf = str_all;
	while (*buf++ != '\0')
		count_all++;
	
	if (count1 + count2 != count_all)
		return 0;
	
	bool** dp = (bool**)calloc(count1 + 1, sizeof(bool*));
	for (i = 0; i < count1 + 1; i++)
		dp[i] = (bool*)calloc(count2 + 1, sizeof(bool));
	
	//对dp[i][j]断定时，str_all应在str_all[i + j]
	//其中前一行和前一列中都是作为初始化的
	for (i = 1; i < count1 + 1; i++)
		dp[i][0] = (str1[i - 1] == str_all[i - 1] ? 1 : 0);
	for (j = 1; j < count2 +1; j++)
		dp[0][j] = (str2[j - 1] == str_all[j - 1] ? 1 : 0);
	//此处初始化会为填表提供必要的原始数据
	
	for (i= 1; i < count1 + 1; i++)
		for (j = 1; j < count2 +1; j++) {
			dp[i][j] = \
				((dp[i - 1][j] == 1 && str1[i - 1] == str_all[i + j - 1]) || \
				(dp[i][j - 1] == 1 && str2[j - 1] == str_all[i + j - 1]));			
		}
	
	//打印表做参考
	for (i= 0; i < count1 + 1; i++) {
		for (j = 0; j < count2 +1; j++)
			printf("%d", dp[i][j]);
		printf("\n");
	}
	
	result = dp[count1][count2];
	
	for (i = 0; i < count1 + 1; i++)
		free(dp[i]);
	free(dp);
	return result;
}

//对称二叉树
//按层次遍历也许也是一种可行思路，因为每一层一定是双数
//对一层，从首尾走到中间即可，只需要比较当前数据与是否存在对称子树
//即 1---n 中 结点1若存在左子树则结点n必存在右子树同理如此
//这样避免对下级取值比较的繁琐操作，而层次遍历需要用到栈
//则在这里使用大结点指针数组模拟栈，等使用时转化为数组则提高效率很方便
//同样的需要俩个栈进行交换使用，它们互相保存下一层结点，这
//只需要使用一个变量循环选择俩个数组即可，退出值仅匹配失败与栈为空

//根据前序和中序遍历确定树
void help_confirm_a_tree_use_traversal(int* pre_order, int* in_order, \
	int pre_order_start, int pre_order_end, \
	int in_order_start, int in_order_end, \
	int level)
{
	//没有左右子树，即它是叶子了，这个应该为退出条件	
	if (pre_order_start > pre_order_end) {
		printf("level:%d - data:%d \n", level, pre_order[pre_order_start]);		
		return;
	} else {
		//从先序遍历中取出根
		int i = 0, left_length = 0, root = pre_order[pre_order_start];
		//中序遍历中找到根，则可以确定左右子树，如果存在的话
		for (i = in_order_start; i <= in_order_end; i++)
			if (in_order[i] == root)
				break;
		//先序遍历组成为 本节点 + 左子树 + 右子树
		//现在：in_order[in_order_start] - in_order[i - 1]为左子树
		// 		  in_order[i + 1] - in_order[in_order_end]为右子树

		//现在我可以通过中序左子树大小确定先序遍历中左子树部分是哪个
		//：pre_order[pre_order_start + 1] - pre_order[left_length + pre_order_start]
		//：pre_order[left_length + pre_order_start + 1] - pre_order[pre_order_end]		
					
		//现在左子树存在了，处理左子树
		if (i - 1 >= in_order_start) {
			left_length = i - 1 - in_order_start;
			help_confirm_a_tree_use_traversal(pre_order, in_order, \
				pre_order_start + 1, pre_order_start + 1 + left_length, \
				in_order_start, i - 1, \
				level + 1);				
		}
		
		//处理本节点
		printf("level:%d - data:%d \n", level, pre_order[pre_order_start]);		
	
		//处理右子树，左子树要么存在，长度为left_length或者为0
		if (i + 1 <= in_order_end) {
			help_confirm_a_tree_use_traversal(pre_order, in_order, \
				pre_order_start + 1 + left_length + 1, pre_order_end, \
				i + 1, in_order_end, \
				level + 1);				
		}				
	}
}

void confirm_a_tree_use_traversal(int* pre_order, int* in_order, \
	int left, int right)
{
	help_confirm_a_tree_use_traversal(pre_order, in_order, \
	left, right, left, right, 0);
}

//二叉树的自底向上的层次遍历
//可以改造队列，队列保存一个标记用来标记一层的结尾
//额外准备俩个栈，一个用于逆向一层节点，一个用于保存所有按
//层逆序的所有数据，所有其中最关键的实现就是队列标记，标记层
//理解，某一层数据全部出队列后，则最后入队列的便是下一层层尾
//便很清晰的保留了层尾

//链表寻中，快慢指针即可

//给定一个二叉树和一个目标和
//找到所有从根节点到叶子节点路径总和等于给定目标和的路径。
//DFS即可，用数组保留路径，递归更新当前点位和目标和即可

//二叉树原地展开为链表，
//则对当前节点转化为链表的条件是左右子树已经为链表了
//此时摘下右结点接入到左结点最底层左子节点即可
//所以递归先处理左右子树后处理本节点

//不同的子序列，不改变字符相对位置通过删掉部分字符可使其与参考字符相等
//dp[i][j] = dp[i][j - 1] + (T[i - 1] == S[j - 1] ? dp[i - 1][j - 1] : 0)
int distinct_subsequences(char* str1, char* str2)
{
	int count1 = 0, count2 = 0;
	char* buf = str1;
	while (*buf++ != '\0')
		count1++;
	buf = str2;
	while (*buf++ != '\0')
		count2++;		
	int i = 0, j = 0, result = 0;
	
	int** dp = (int**)calloc(count1 + 1, sizeof(int*));
	for (i = 0; i < count1 + 1; i++)
		dp[i] = (int*)calloc(count2 + 1, sizeof(int));
	
	//初始化
	for (i = 0; i < count1 + 1; i++)
			dp[i][0] = 1;
				
	//填表
	//dp[i][j] = dp[i][j - 1] + (T[i - 1] == S[j - 1] ? \
		dp[i - 1][j - 1] : 0)
	for (i = 1; i < count1 + 1; i++)
		for (j = 1; j < count2 + 1; j++) 
			dp[i][j] = dp[i - 1][j] + (str1[i - 1] == str2[j - 1] ? \
				dp[i - 1][j - 1] : 0);			

	//调节
	for (i = 0; i < count1 + 1; i++) {
		for (j = 0; j < count2 + 1; j++)	
			printf("%d ",dp[i][j]);
		printf("\n");
	}

	result = dp[count1][count2];
	for (i = 0; i < count1 + 1; i++)
		free(dp[i]);	
	free(dp);
	return result;
}

//第某列杨辉三角
void pascals_triangle(int n)
{
	int* array = (int*)malloc(n * sizeof(int));
	int i = 0, j = 0;
	for (i = 0; i < n; i++)
		array[i] = 1;
	
	for (i = 1; i <= n - 1; i++)
		for (j = i - 1; j >= 1; j--)
			array[j] += array[j - 1];
	
	for (i = 0; i < n; i++)	
		printf("%d ", array[i]);
	
	free(array);	
}

//改造杨辉三角成图，寻找最小路径
//dp[i][j] += min(dp[i - 1][j - 1] + dp[i - 1][j]);
int search_pascals_triangle_min_load(int* gridding, int row, int col, int n)
{
	//n为总行数或者总列数
	int result = 0, i = 0, j = 0;	
	int** dp = (int**)calloc(n + 1, sizeof(int*));
	for (i = 0; i < n + 1; i++)
		dp[i] = (int*)calloc(n + 1, sizeof(int));
	for (i = 0; i < n + 1; i++)
		for (j = 0; j < n + 1; j++)
			dp[i][j] = ~(1 << 31);
	dp[1][1] = gridding[0 * n + 0];
	
	for (i = 2; i < n + 1; i++)
		for (j = 1; j <= i; j++)
			dp[i][j] = gridding[(i - 1) * n + (j - 1)] + \
				(dp[i - 1][j - 1] < dp[i - 1][j] ? \
				dp[i - 1][j - 1] : dp[i - 1][j]);
	
	
	for (i = 0; i < n + 1; i++) {
		for (j = 0; j < n + 1; j++)	
			printf("%d ", dp[i][j]);
		printf("\n");
	}
	
	
	for (i = 0; i < n + 1; i++)
		free(dp[i]);	
	free(dp);
	return result;
}

//卖股票方式，选择某天买入股票某天卖出股票，找寻最大利益
void search_best_time_to_buy_and_sell_stock_i(int* array, int left, int right)
{
	//遍历寻找最小值
	int min = ~(1 << 31), min_index = -1;
	int max_profit = 0, max_index = -1;
	int i = 0;
	for (i = left; i <= right; i++) {
		//更新最小值
		if (array[i] < min) {
			min = array[i];
			min_index = i;
		}	
		//更新最大值	
		if (array[i] - min > max_profit) {
			max_profit = array[i] - min;
			max_index = i;
		}
	}
	if (min_index != -1 && max_index != -1)
		printf("%d - %d - %d", min_index, max_index , max_profit);
}

//仅可同时持股一支，可多次买入卖出
void search_best_time_to_buy_and_sell_stock_ii(int* array, int left, int right)
{
	//遍历寻找最小值
	int min = ~(1 << 31), min_index = -1;
	int max_profit = 0, max_index = -1;
	int i = 0;
	for (i = left; i <= right; i++) {
		//更新最小值
		if (array[i] < min) {		
			min = array[i];
			min_index = i;
		}	
		
		//更新最大值	
		if (array[i] - min >= max_profit) {
			max_profit = array[i] - min;
			max_index = i;
		}
		
		//遇到比最大值小，则前面产生一笔可靠利润
		if (min_index < max_index && \
			array[i] - min < max_profit) {
			if (min_index != -1 && max_index != -1)
				printf("%d - %d - %d\n", \
					min_index, max_index , max_profit);
						
			max_profit = array[i];
			min = array[i];
			min_index = i;			
		}
	}
}

//仅可同时持股一支，仅俩次买入卖出
//dp[i][k][0]表示第 i 天，经过最多 k 次交易，0不持有股票的最大收益
//dp[i][k][1]表示第 i 天，经过最多 k 次交易，1持有股票的最大收益
//最后应该返回的是dp[n-1][k][0]，最后持有股票收益是负的，所以不是1
//k 定义是买入的时候增加 1次
//i = 0 时，不持有股票，dp[i][k][0]=0，持有股票，dp[i][k][1]=-prices[0]
//i > 0 时：
//dp[i][k][0] = max(dp[i-1][k][0], dp[i-1][k][1]+prices[i])
//不持有股票，--------- 前一天没持有 or  前一天持有，然后卖出
//dp[i][k][1] = max(dp[i-1][k][1], dp[i-1][k-1][0]-prices[i])
//持有股票，--------- 前一天持有 or  前一天不持有，然后买入（交易k+1
int search_best_time_to_buy_and_sell_stock_iii(int* array, int left, int right, int k)
{
	int i = 0, j = 0, result = 0;		
	int max_profit = 0, buy_day = -1, sell_day = -1;	
	//申请三维内存空间
	int*** dp = (int***)calloc(right - left + 1, sizeof(int**));
	for (i = 0; i < right - left + 1; i++)
		dp[i] = (int**)calloc(k + 1, sizeof(int*));
	for (i = 0; i < right - left + 1; i++)
		for (j = 0; j < k + 1; j++)
			dp[i][j] = (int*)calloc(2, sizeof(int));
		
	//i = 0
	for (j = 0; j < k + 1; j++) {
		dp[0][j][0] = 0;
		dp[0][j][1] = -array[left];
	}
		
	//i > 0
	for (i = 1; i < right - left + 1; i++)
		for (j = 1; j < k + 1; j++) {
			dp[i][j][0] = \
				(dp[i - 1][j][0] > (dp[i - 1][j][1] + array[i + left])) ? \
				dp[i - 1][j][0] : (dp[i - 1][j][1] + array[i + left]);
			dp[i][j][1] = \
				(dp[i - 1][j][1] > (dp[i - 1][j - 1][0] - array[i + left])) ? \
				dp[i - 1][j][1] : (dp[i - 1][j - 1][0] - array[i + left]);
		}
	
	result = dp[right - left][k][0];
	
	//释放空间
	for (i = 0; i < right - left + 1; i++) 
		for (j = 0; j < k + 1; j++)
			free(dp[i][j]);
	for (i = 0; i < right - left + 1; i++) 	
		free(dp[i]);			
	free(dp);
	
	return result;
}

//二叉树求最大路径
//不是从根节点开始，而是从任意结点开始
//前节点为根的子树的最大值
//int curmax = root->val;
//if(left > 0)
//	curmax += left;
//if(right > 0)
//	curmax += right;
//max(root->val, max(left+root->val, right+root->val))

//回文字符串确认
bool is_palindrome(char* str)
{
	int count = 0, start = 0, end = 0;
	char* buf = str;
	while (*buf++ != '\0')
		count++;
	
	//则范围在0 --- count - 1
	start = 0;
	end = count - 1;
	while (start < end) {
		//找第一个字符
		while (!(('0' <= str[start] && str[start] <= '9') || \
			('a' <= str[start] && str[start] <= 'z') || \
			('A' <= str[start] && str[start] <= 'Z')))
			start++;
		//找最后一个字符
		while (!(('0' <= str[end] && str[end] <= '9') || \
			('a' <= str[end] && str[end] <= 'z') || \
			('A' <= str[end] && str[end] <= 'Z')))
			end--;		
		
		if (start >= end)
			break;
		printf("%c - %c\n", str[start], str[end]);
		//是否满足条件
		if (str[start] == str[end] || \
			(str[start] - 'a' == str[end] - 'A') || \
			(str[start] - 'A' == str[end] - 'a')) {
			start++;
			end--;
		} else
			return 0;
	}
	return 1;
}

//单词接龙，查找最小一个或全部，查找指定，等等
int help_how_many_str_is_different(char* str1, char* str2)
{
	int count = 0;
	while (*str1 != '\0' && *str2 != '\0')
		if (*str1++ != *str2++)
			count++;
	return count;
}

void help_word_ladder_ii(char* *str_all, int left, int right, \
	bool* is_used, int* used_index, int index, char* str_start, char* str_end)
{
	if (0 == help_how_many_str_is_different(str_end, str_start)) {
		int i = 0;
		//注：如果说只找最小的，那么需要额外数据结构保存user_index数组
		//以及最小的user_index，当然可以先单独查找最小index
		//当然会浪费点效率
		for (i = 0; i < index; i++) 
			printf("%s\n", str_all[used_index[i]]);
		printf("\n");		
	} else {
		int i = 0;
		for (i = left; i <= right; i++) {
			if (is_used[i] != 0)
				continue;
			else {
				if (1 != help_how_many_str_is_different(str_all[i], str_start))
					continue;
				is_used[i] = 1;
				used_index[index] = i;			
				help_word_ladder_ii(str_all, left, right, \
					is_used, used_index, index + 1, str_all[i], str_end);
				is_used[i] = 0;
			}		
		}
	}
}

void word_ladder_ii(char* *str_all, int left, int right, \
	char* str_start, char* str_end)
{
	//最小关系按理说每个单词应该使用一次即可，否则会死陷入
	bool* is_used = (bool*)calloc(right - left + 1, sizeof(bool));
	int* used_index = (int*)calloc(right - left + 1, sizeof(int));
	
	help_word_ladder_ii(str_all, left, right, \
		is_used, used_index, 0, str_start, str_end);
	
	
	
}

//求最长连续序列
//将数据hash到表中，
//然后遍历数据，取一个数据且该数据在hash表中
//从表中移除该数据
//不断查询该数据前一个数据与后一个数据是否在表中
//如果存在则移除直到不存在，则一次局部最长计算完毕
//取下一个在表中的数据，循环最后表中将不再存在数据
//最大长度为局部最长中最大长度
int help_longest_consecutive_sequence(\
	struct hash_node** help, int n, int data)
{
	//实现一次查找，通过不断修改data
	int index =  data % n;
	struct hash_node* prev_buf = NULL;		
	struct hash_node* buf = help[index];	
	while (buf != NULL) {
		if (buf->index == data) {
			//我们需要移除该结点
			if (prev_buf == NULL)          
				help[index] = buf->next;
			else
				prev_buf->next = buf->next;
			free(buf);
			return 1;
		} else {
			prev_buf = buf;	
			buf = buf->next;				
		}
	}
	return 0;	
}
	
int longest_consecutive_sequence(int* array, int left, int right)
{
	//构造哈希表
	int n = right - left + 1, index = 0;
	int result = 0, current_val = 1, i = 0;
	struct hash_node** help= \
		(struct hash_node**)calloc(n, sizeof(struct hash_node*));	
	//把数据全部丢到表中，其中选取
	//最简单的映射规则  data % right - left + 1
	for (i = left; i <= right; i++) {
		index = array[i] % n;
		//建点
		struct hash_node* buf = \
			(struct hash_node*)calloc(1, sizeof(struct hash_node));			
		buf->index = array[i];
		buf->next = NULL;	
		//数据头插进去
		buf->next = help[index];
		help[index] = buf;
	}
		
	//再遍历
	for (i = left; i <= right; i++) {	
		//查本数是否存在，存在则移除，不存在的话那就没事了
		if (0 == help_longest_consecutive_sequence(\
			help, n, array[i]))
			continue;
		int data1 = array[i] - 1, is_valid1 = 1;
		int data2 = array[i] + 1, is_valid2 = 1;
		int flag = 2;
		//查该数前一个与后一个是否存在
		//只要有一个存在就一直查下去
		while (flag > 0) {	
			flag = 0;
			if (is_valid1 == 1)
				if (1 == help_longest_consecutive_sequence(\
					help, n, data1)) {
					data1--;
					current_val++;
					flag++;
				} else 
					is_valid1 = 0;
			
			if (is_valid2 == 1)
				if (1 == help_longest_consecutive_sequence(\
					help, n, data2)) {
					data2++;
					current_val++;
					flag++;
				} else 
					is_valid2 = 0;
		}		
		//更新result
		if (result < current_val) {
			result = current_val;
			current_val = 1;	
		}
	}	
	
	//清理表
	for (i = 0; i < n; i++) {		
		struct hash_node* buf = NULL;
		while (help[i] != NULL) {
			buf = help[i]->next;
			free(help[i]);
			help[i] = buf;				
		}
	}
	free(help);
	return result;
}

//被围绕的区域
//如果该区域被围绕，则该区域被填空，但靠近边界不算围绕
int help_surrounded_region(char* map, int x, int y, \
	int last_x, int last_y, int flag)
{
	if (flag == 0) {
		//开始了
		int i = 0, j = 0;
		for (i = 0; i <= x; i++)
			for (j = 0; j <= y; j++) {
				if (map[i * (y + 1) + j] != '0')
					continue;
				else {
					map[i * (y + 1) + j] = 'x';
					if (0 == help_surrounded_region(map, x, y, \
						i, j, 1))
						map[i * (y + 1) + j] = '0';
				}
			}
		return -1;
	} else {
		//一定不可碰到边界
		if (last_x - 1 < 0 || last_x + 1 > x || \
			last_y - 1 < 0 || last_y + 1 > y) {
			map[last_x * (y + 1) + last_y] = '0';
			return 0;
		}
		//走到最后了
		if (map[(last_x - 1) * (y + 1) + last_y] == 'x' && \
			map[(last_x + 1) * (y + 1) + last_y] == 'x' && \
			map[last_x * (y + 1) + (last_y - 1)] == 'x' && \
			map[last_x * (y + 1) + (last_y + 1)] == 'x')
			return 1;
		
		//则现在，去探测它的四个方位
		
		if (map[(last_x - 1) * (y + 1) + last_y] == '0') {
			map[(last_x - 1) * (y + 1) + last_y] = 'x';
			if (0 == help_surrounded_region(map, x, y, \
				last_x - 1, last_y, 1)) {
				map[(last_x - 1) * (y + 1) + last_y] = '0';
				return 0;
			}	
		}
		
		if (map[(last_x + 1) * (y + 1) + last_y] == '0') {
			map[(last_x + 1) * (y + 1) + last_y] = 'x';
			if (0 == help_surrounded_region(map, x, y, \
				last_x + 1, last_y, 1)) {
				map[(last_x + 1) * (y + 1) + last_y] = '0';
				return 0;
			}	
		}		
		
		if (map[last_x * (y + 1) + (last_y - 1)] == '0') {
			map[last_x * (y + 1) + (last_y - 1)] = 'x';
			if (0 == help_surrounded_region(map, x, y, \
				last_x, last_y - 1, 1)) {
				map[last_x * (y + 1) + (last_y - 1)] = '0';
				return 0;
			}
		}	
		
		if (map[last_x * (y + 1) + (last_y + 1)] == '0') {
			map[last_x * (y + 1) + (last_y + 1)] = 'x';
			if (0 == help_surrounded_region(map, x, y, \
				last_x, last_y + 1, 1)) {
				map[last_x * (y + 1) + (last_y + 1)] = '0';
				return 0;
			}
		}			
			
		return 1;
	}
	
}

void surrounded_region(char* map, int x, int y)
{
	help_surrounded_region(map, x, y, 0, 0, 0);
	int i = 0, j = 0;
	for (i = 0; i <= x; i++) {
		for (j = 0; j <= y; j++)
			printf("%c ", map[i * (y + 1) + j]);
		printf("\n");
	}		
}

//分割字符串出回文串
bool help_is_palindrome(char* str, int x, int y)
{
	if (x > y)
		return 0;
	if (x == y)
		return 1;
	while (x < y)
		if (str[x++] != str[y--])
			return 0;
	return 1;
}

void help_palindrome_patitioning(char* str , int count, \
	int* index_start, int* index_end, int index, int now)
{
	if (now >= count) {
		//全部字符串都被放入str中
		//现在打印出来
		int i = 0, j = 0;
		for (i = 0; i <= index; i++) {
			//printf("%d - %d",index_start[i], index_end[i]);
			for (j = index_start[i]; j <= index_end[i]; j++)
				printf("%c", str[j]);
			printf("\n");
		}
		printf("\n");
	} else {	
		//查找回文字符串
		int i = now;
		//从当前开始，是否能找到某一节点使其范围形成palindrome
		for (i = now; i < count; i++) {
			//现在我已经得到一个范围为 i - j 了
			if (1 == help_is_palindrome(str, now, i)) {
				//有效
				index++;					
				index_start[index] = now;
				index_end[index] = i;
				help_palindrome_patitioning(str, count, \
					index_start, index_end, index, i + 1);					
				index--;
			}			
		}						
	}		
}

void palindrome_patitioning_i(char * str)
{
	int count = 0;
	char* buf = str;
	while (*buf++ != '\0')
		count++;
	
	int* index_start = (int*)calloc(count + 1, sizeof(int));
	int* index_end = (int*)calloc(count + 1, sizeof(int));

	help_palindrome_patitioning(str, count, \
		index_start, index_end, -1, 0);	
	
	free(index_start);
	free(index_end);
}

//分割字符串出回文串，寻找最小分割次数
//dp[i]表示到 i 为止的子串最少需要分割多少次
//如果一个子串为回文串，dp[i] = 0
//如果不是，遍历所有的 j （j <= i）
//如果s[j,i]是回文串，dp[i] = min(dp[i], dp[j-1]+1)
int palindrome_patitioning_ii(char * str) 
{
	int count = 0, i = 0, j = 0, result = 0;
	char* buf = str;
	while (*buf++ != '\0')
		count++;
	int* dp = (int*)malloc(count * sizeof(int));
	for (i = 0; i < count; i++)
		dp[i] = ~(1 << 31);
	//注：在0 - i 不为回文串时，并不一定是寻找最大的回文字段
	//就保证分割次数会最小，遍历是无奈之举
	for (i = 0; i < count; i++) {
		if (1 == help_is_palindrome(str, 0, i)) {
			dp[i] = 0;
			continue;
		}
		for (j = 0; j <= i; j++)
		{
			if (1 == help_is_palindrome(str, j, i))
				dp[i] = dp[i] < (dp[j - 1] + 1) ?
					dp[i] : (dp[j - 1] + 1);
		}	
	}
	for (i = 0; i < count; i++) 
		printf("%d", dp[i]);
	result = dp[count - 1];
	free(dp);
	return result;
}

//克隆无向图
//与遍历无向图是类似的，可以广度优先（简单些）（队列）
//也可以深度优先，微难点（堆栈）
//它们的退出条件都是起始点
//因为是无向，所以无论怎么走都会回到原点

//加油站
//i结点有gas[i]升汽油，而去往下一节点需要cost[i]升汽油
//从某一节点出发，是否可以环绕一圈
void search_all_useful_gasnode(int* gas, int* cost, int n)
{
	int have_gas = 0, need_gas = 0, have_node = 0;
	int i = 0, j = 0, flag = 0;
	//从某一节点出发
	for (i = 0; i < n - 1; i++) {
		//准备出发
		have_gas = 0;
		need_gas = 0;
		flag = 0;
		j = i;
		while (flag == 0 || i != j) {
			have_gas += gas[j];
			need_gas = cost[j];		
			//如果可以走到就走
			if (have_gas >= need_gas) 
				have_gas -= need_gas;			
			 else 
				//走不到 break;
				break;			
			//要越界了
			j++;
			if (j == n) {
				j = 0;			
				flag = 1;
			}
		}
		//正常出来了
		if (flag == 1 && i == j) {
			printf("comfirm a node : %d \n", i);
			have_node++;
		}	
	}
	//如果一个点都没有
	if (have_node == 0)
		printf("i can not find a useful node \n");
}

//发糖果
//直接采用俩次遍历
//若采用一次遍历，对于从左至右遍历，
//右边比左边，遇到大于等于的情况下正常处理，遇到小于的情况下
//需要记录浮标，连续小于的情况，以待后续处理
void send_out_candy(int* array, int left, int right)
{
	int i = 0;
	int* dp = (int*)malloc((right - left + 1) * sizeof(int));
	for (i = 0; i < right - left + 1; i++)
		dp[i] = 1;
	
	for (i = 0; i < right - left; i++)
		//如果右边的比左边的大	
		if (array[i + 1 + left] > array[i + left])
			dp[i + 1]++;
	
	for (i = right - left; i >= 1; i--)
		//如果左边的比右边的大	
		if (array[i - 1 + left] > array[i + left])
			dp[i - 1] = dp[i] + 1;		
	
	for (i = 0; i < right - left + 1; i++)
		printf("%d ", dp[i]);
	
	free(dp);
}

//找数字
//数组中恰好一个数出现一次，其他数出现了俩次
//a ^ a = 0
//a ^ 0 = a
int find_single_number_i(int* array, int left, int right)
{
	int result = 0, i = 0;
	for (i = left; i <= right; i++)
		result ^= array[i];
	return result;
}

//数组中恰好一个数出现一次，其他数出现了三次(其他次数一致)
int find_single_number_ii(int* array, int left, int right, int count_other)
{
	//按位计算，任意一个出现三次的数其某一位一定出现3 * n 个1
	//如果产生多余则那是多出的那个数
	int result = 0, i = 0, j = 0;
	for (i = 0; i <= 31; i++) {
	//现在计算第i位
		int count = 0;
		int bit_x = (1 << i);
		for (j = left; j <= right; j++)
			if ((bit_x & array[j]) != 0)
				count++;	
		if (count % count_other != 0)
			result |= bit_x;
	}
	return result;
}

//深拷贝、复制含随机指针的链表
//边复制链表边对链表进行哈希映射
//之后在处理随机结点时，查哈希表则可以把对其的处理时间复杂度降低到O(1)
//建议的映射是对其地址映射，旧地址与新地址的映射，因为指针恰好使用的就是地址

//查找长字符是否可拆分成字典内指定字符串（可重复）
//以及可拆分的全部组合
bool help_match_two_str_is_equal(char* str1, char* str2)
{
	while (*str1 != '\0' && *str2 != '\0')
		if (*str1++ != *str2++) 
			return 0;	
	if (*str1 != '\0' || *str2 != '\0')
		return 0;
	return 1;
}

bool help_one_str_is_exist(char** str_all, int n, char* need_comfirm)
{
	int i = 0;
	for (i = 0; i < n; i++) 
		if (1 == help_match_two_str_is_equal(str_all[i], need_comfirm))
			return 1;
	return 0;
}

bool word_break_i(char** str_all, int n, char* str)
{
	//确定分割范围，在need_comfirm范围内
	int count = 0, result = 0;
	int i = 0, j = 0, k = 0;
	char* buf = str;
	while (*buf++ != '\0')
		count++;
	//现在分割范围则在，0 - count - 1 内
	//为方便默认分割为 0 - x - 1 ， x - count - 1
	char* help_buffer = (char*)calloc(count + 1, sizeof(char));
	bool* dp = (bool*)calloc(count + 1, sizeof(bool));
	dp[0] = 1;
	for (i = 0; i < count + 1; i++) {
		for (j = 0; j <= i; j++) {
			//先需保证第一部分可分割
			if (dp[j] == 0)	
				continue;
			//改造函数参数则可无需如此大费周章，在此处选择方便
			//总体为了测试dp
			
			//拷贝j + 1 到 i 的字符到buffer里面，与str左偏移无关
			for (k = 0; k + j + 1 <= i; k++)
				//此处是因为str起始位置为0，需要往左偏移一个单元
				help_buffer[k] = str[k + j];
			help_buffer[k] = '\0';	
			printf("%s\n", help_buffer);
			
			if (1 == help_one_str_is_exist(str_all, n, help_buffer)) {
				dp[i] = 1;			
				break;
			}	
		}
	}
	
	for (i = 0; i < count + 1; i++)
		printf("%d", dp[i]);
	
	result = dp[count];
	free(help_buffer);
	free(dp);
	return result;
}

void help_word_break_ii(char** str_all, int n, char* str, char* help_buffer, \
	bool* dp, int dp_n, int* comfirm_start, int* comfirm_end, int index, int start)
{
	if (start >= dp_n - 1) {
		int i = 0, j = 0;
		for (i = 0; i <= index; i++) {
			for (j = comfirm_start[i]; j <= comfirm_end[i]; j++)
				printf("%c", str[j]);
			printf(" ");
		}
		printf("\n");
	} else {
		int i = 0, j = 0, k = 0;
		for (i = start; i < dp_n; i++) {
			for (j = start; j <= i; j++) {
				//先需保证第一部分可分割
				if (dp[j] == 0)	
					continue;
				//改造函数参数则可无需如此大费周章，在此处选择方便
				//总体为了测试dp
				
				//拷贝j + 1 到 i 的字符到buffer里面，与str左偏移无关
				for (k = 0; k + j + 1 <= i; k++)
					//此处是因为str起始位置为0，需要往左偏移一个单元
					help_buffer[k] = str[k + j];
				help_buffer[k] = '\0';				
				
				if (1 == help_one_str_is_exist(str_all, n, help_buffer)) {
					dp[i] = 1;
					index++;
					comfirm_start[index] = j;
					comfirm_end[index] = i - 1;
					//探测后一部分区域
						
					/*调试
					printf("%s\n", help_buffer);
					for (k = 0; k < dp_n; k++)
						printf("%d ",dp[k]);
					printf("\n");					
					printf("%d - %d -%d \n", i, index, dp_n - 1);
					*/
					
					help_word_break_ii(str_all, n, str, help_buffer, \
						dp, dp_n, comfirm_start, comfirm_end, index, i);				
					//还原现场
					dp[i] = 0;
					index--;							
				}	
			}		
		}
	}
}

void word_break_ii(char** str_all, int n, char* str)
{
	//确定分割范围，在need_comfirm范围内
	int count = 0;
	char* buf = str;
	while (*buf++ != '\0')
		count++;
	//现在分割范围则在，0 - count - 1 内
	//为方便默认分割为 0 - x - 1 ， x - count - 1
	char* help_buffer = (char*)calloc(count + 1, sizeof(char));
	bool* dp = (bool*)calloc(count + 1, sizeof(bool));
	int* comfirm_start = (int*)calloc(count + 1, sizeof(int));
	int* comfirm_end = (int*)calloc(count + 1, sizeof(int));	
	dp[0] = 1;
	
	help_word_break_ii(str_all, n, str, help_buffer,\
		dp, count + 1, comfirm_start, comfirm_end, -1, 0);
	
	free(comfirm_start);
	free(comfirm_end);
	free(help_buffer);
	free(dp);
}

//链表是否有环，快慢指针

//找环起点
//这里还是要设快慢指针，
//不过这次要记录两个指针相遇的位置，
//当两个指针相遇了后，让其中一个指针从链表头开始
//此时再相遇的位置就是链表中环的起始位置

//反序链表：L1 …… Ln --》 L1 Ln L2 Ln-1 L3 Ln-2
//快慢指针找中点，断开，逆序后半部分，然后俩俩归并到一起即可

//组合数据结构，哈希链表：
//对链表所有结点进行哈希映射到哈希表中
//这使得对链表的查找变成常数时间
//但需要附加数据结构

//实现LRU缓存机制，若使用O(1)必须使用散列表
//另外的一种是使用树代替哈希表，但是查找速度可能慢点
//这种数据结构使用于对内存描述符管理的线性区链表的维护所使用

//链表插入排序与归并排序，注意基本的结点弹出与插入即可

//求一组二维点中最大共线的数量
//将全部斜率组存下来，就存在一个链表中，仅仅是为了编写方便
//若提升效率可采用其他的数据结构
int help_get_greatest_common_divisor(int data1, int data2)
{
	if (data1 == 0 || data2 == 0)
		return 1;
	else {
		int remainder, temp;
		//辗转相除求最大公约数
		while(data2 != 0) {
			//大数放在a，小数放在b，余数放在g
			if (data1 < data2) {
				temp = data1;
				data1 = data2;
				data2 = temp;
			}		
			remainder = data1%data2;	
			data1 = data2;
			data2 = remainder;
		}
		return data1;		
	}
}
		
struct slope
{
	int numerator;
	int denominator;
	struct slope* next;
	int count;
	//保存该斜率对应共线点
	bool* is_used;
};

int max_point_on_a_line(int* datax, int* datay, int n)
{
	int result = 0, i = 0, j = 0;	
	struct slope* slope_list = NULL;	
	for (i = 0; i < n; i++) {
		for (j = 0; j < i; j++) {
			//现在俩个点取到了，分别为(datax[j],datay[j]) - (datax[i],datay[i])
			//我们默认用后面的减去前面的作为斜率组
			int numerator = datax[i] - datax[j];
			int denominator = datay[i] - datay[j];
			int max_common_divisor = \
				help_get_greatest_common_divisor(abs(numerator), abs(denominator));
			//斜率组最简化
			numerator /= max_common_divisor;
			denominator /= max_common_divisor;
			
			printf("xx:%d - %d\n", numerator, denominator);
			//将斜率组插入到数据结构中
			if (slope_list == NULL) {
				slope_list = (struct slope*)calloc(1, sizeof(struct slope));
				slope_list->numerator = numerator;
				slope_list->denominator = denominator;				
				slope_list->next = NULL;
				slope_list->count = 2;
				slope_list->is_used = (bool*)calloc(n, sizeof(bool));
				slope_list->is_used[i] = 1;				
				slope_list->is_used[j] = 1;							
				if (result < 2)
					result = 2;						
			} else {
				struct slope* buf = slope_list;
				struct slope* buf_prev = slope_list;
				//查链表
				while (buf != NULL) {
					if (numerator == buf->numerator && \
						denominator == buf->denominator) {
						if (0 == buf->is_used[i]){
							buf->count += 1;						
							buf->is_used[i] = 1;						
						}
						if (0 == buf->is_used[j]) {
							buf->count += 1;						
							buf->is_used[j] = 1;						
						}
						if (result < buf->count)
							result = buf->count;
						break;
					}
					buf_prev = buf;
					buf = buf->next;
				}
				//没查到
				if (buf == NULL) {
				buf_prev->next = (struct slope*)calloc(1, sizeof(struct slope));
				buf_prev->next->numerator = numerator;
				buf_prev->next->denominator = denominator;
				buf_prev->next->count = 2;
				buf_prev->next->next = NULL;
				buf_prev->next->is_used = (bool*)calloc(n, sizeof(bool));		
				buf_prev->next->is_used[i] = 1;
				buf_prev->next->is_used[j] = 1;
				if (result < 2) 
					result = 2;						
				}		
			}		
		}	
	}	
	//释放空间
	struct slope* buf = slope_list;
	while (slope_list != NULL) {
		buf = slope_list->next;
		free(slope_list->is_used);
		free(slope_list);
		slope_list = buf;
	}	
	return result;
}

//逆波兰表达式求值
//其他地方写过calculate.h

//翻转字符串中的单词
//以每个单词为单位翻转
void reserve_str_according_word(char* str)
{
	int count = 0;
	char* buf_buf = str;
	while (*buf_buf++ != '\0')
		count++;
	//现在已经确认有效范围在0 - count - 1
	int i = 0, j = 0, buf_index = 0;
	char* buf = (char*)calloc(count, sizeof(char));
	int simulate_stack_index = -1;
	int* simulate_stack_start = (int*)calloc(count, sizeof(int));
	int* simulate_stack_end = (int*)calloc(count, sizeof(int));
	//现在需要做一件事情，确认压栈条件
	//因为是以空格作为分割，所以
	//压栈start的条件是自己是空格下一位不是空格，压入下一位
	//压栈end的条件是自己不是空格下一位是空格
	for (i = 0; i < count; i++) {
		//本位非空格
		if (str[i] != ' ') {
			//前一位不存在或者为空格
			if (i - 1 < 0 || str[i - 1] == ' ')
				simulate_stack_start[++simulate_stack_index] = i;
			//后一位不存在或者为空格
			if (i + 1 >= count || str[i + 1] == ' ')
				simulate_stack_end[simulate_stack_index] = i;
		}	
	}	
	//出栈
	for (i = simulate_stack_index; i >= 0; i--) {
		for (j = simulate_stack_start[i]; \
			j <= simulate_stack_end[i]; \
			j++)
			buf[buf_index++] = str[j];
			buf[buf_index++] = ' ';
	}
	buf[--buf_index] = '\0';
	strcpy(str, buf);	
	
	printf("%s\n", buf);
	printf("%s\n", str);
	
	free(simulate_stack_start);
	free(simulate_stack_end);
}

//求子序列最大乘积
//包含每个数的序列的最大乘积记为dpmax[i]，最小乘积dpmin[i]
//nums[i] > 0 时，dpmax[i]=max(dpmax[i−1])*nums[i],nums[i]) 
//nums[i] < 0 时，dpmax[i]=max(dpmin[i−1])*nums[i],nums[i]) 
//同时记得维护dpmin[i] (相反的操作)
int maximum_product_subarray(int* array, int left, int right)
{
	int result = 0, i = 0;
	int* dp_max = (int*)calloc(right - left + 1, sizeof(int));
	int* dp_min = (int*)calloc(right - left + 1, sizeof(int));
	dp_max[0] = array[left];
	dp_min[0] = array[left];
	
	for (i = 1; i < right - left + 1; i++) {
		if (array[left + i] > 0) {
			dp_max[i] = (dp_max[i - 1] * array[left + i] > array[left + i]) ? \
				(dp_max[i - 1] * array[left + i]) : array[left + i];
				
			dp_min[i] = (dp_min[i - 1] * array[left + i] < array[left + i]) ? \
				(dp_min[i - 1] * array[left + i]) : array[left + i];				
		}
		
		if (array[left + i] < 0) {
			dp_max[i] = (dp_min[i - 1] * array[left + i] > array[left + i]) ? \
				(dp_min[i - 1] * array[left + i]) : array[left + i];	
				
			dp_min[i] = (dp_max[i - 1] * array[left + i] < array[left + i]) ? \
				(dp_max[i - 1] * array[left + i]) : array[left + i];		
		}
		
		if (result < dp_max[i])
			result = dp_max[i];
	}
	
	/*调试
	for (i = left; i < right + 1; i++)
		printf("%d ", array[i]);	
	printf("\n");
	for (i = 0; i < right - left + 1; i++)
		printf("%d ", dp_max[i]);
	printf("\n");
	for (i = 0; i < right - left + 1; i++)
		printf("%d ", dp_min[i]);	
	printf("\n");
	*/
	
	free(dp_max);
	free(dp_min);
	return result;
}

//某个排序数组在某一个位置旋转了，即
//前面的一部分数据跑到了后面去了,出现俩个排序数组接在一起 
//查找旋转数组的最小值
//最小值一定在旋转点的右边
//旋转点是唯一特殊的点，它是一定大于俩边的，仅此一个数据
int search_min_from_rotated_array(int* array, int left, int right, \
	int old_left, int old_right)
{
	if (left >= right)
		return array[right];
	//二分查找即可，
	else {
		int fake_center = (right + left) / 2;
		//是否fake_center是我要找的点
		if (fake_center - 1 >= old_left && \
			fake_center + 1 <= old_right && \
			array[fake_center - 1] <= array[fake_center] && \
			array[fake_center + 1] <= array[fake_center])
				return array[fake_center + 1];

				
		//左边有序，我需要去找右边
		if (array[left] <= array[fake_center])
			return search_min_from_rotated_array(\
				array, fake_center, right, old_left, old_right);
		//右边有序，我要去找左边
		if (array[fake_center] <= array[right])
			return search_min_from_rotated_array(\
				array, left, fake_center, old_left, old_right);

	}
}

//在上述数据在相同的时候
//二分搜索会陷入死锁，所以需要，当遇到相同的元素时
//向俩端移位，所以左端遇到同一数需要向左移位，右端遇到同一数需要向右移位

//最小栈，本身是一个栈，对栈中最小数据进行查找需要在常数时间
//栈仅仅只能对栈顶操作，所以说，不可以投机取巧保存类似于数组下标
//索引这类似的东西
//例：数据 3 5 1 2 2
//按从左至右依次入栈，则最终栈数据为 3 3  5 3  1 1  2 1  2 1

//翻转二叉树
//给定一个二叉树，其中所有的右节点
//要么是具有兄弟节点（拥有相同父节点的左节点）的叶节点，要么为空
//即上下翻转左边部分，所有右边部分最终变成新的左边部分

//最多包含n个不同字符的最长子序列
void longest_substring_with_at_nost_two_distinct_characters(\
	char* str, int n)
{
	char* help_buf = str;
	
	char* buf = str;	
	int max_length = 0;
	char* max_left = buf;	
	int cur_length = 0;
	char* cur_left = buf;	
	
	int current_n = 0;
	bool* is_used = (bool*)calloc(26, sizeof(bool));
	int* all_alphabeta = (int*)calloc(26, sizeof(int));

	while (*buf != '\0') {
		//滑动窗口
		if (is_used[*buf - 'a'] == 1) {
			//遇到一个旧的数
			all_alphabeta[*buf - 'a']++;
			cur_length++;			
			buf++;
		} else {
			//遇到一个新的数，现在需要分别讨论
			if (current_n + 1 <= n) {
				//可行的一个新的数
				cur_length++;
				//使用标记
				is_used[*buf - 'a'] = 1;
				//计数标记
				all_alphabeta[*buf - 'a'] = 1;	
				//当前存在标记
				current_n++;	
				
				buf++;
			} else {
				//多了，需要开始替换了，从前面开始清除，清除到索引少于限制为之	
				while (1) {
					//当前数减少
					cur_length--;
					//减少当前符号对应的数
					if (--all_alphabeta[*cur_left - 'a'] <= 0) {
						//遇到第一个计数归0的，该次清除达到目标
						all_alphabeta[*cur_left - 'a'] = 0;
						is_used[*cur_left - 'a'] = 0;
						current_n--;
						cur_left++;
						break;
					}
					cur_left++;			
				}
			}							
		}
		//更新最大值
		if (cur_length > max_length) {
			max_left = cur_left;
			max_length = cur_length;
		}			
	}
	
	//验证
	for (buf = max_left; buf < max_left + max_length; buf++)
		printf("%c", *buf);
	
	free(all_alphabeta);
	free(is_used);
}

//俩个链表的交点
//其他地方做过，思路为：若俩个链表相交，则相交部分最多为
//从后至前的最大共有数，则可使某一长链表先移动到一个位置
//从该位置到最后的链表长度等于短链表的长度
//然后同步移动指针即可，指针保存地址相同即证明有交点

//判断俩个字符串是否通过只操作一步实现切换
bool one_edit_distance(char* str1, char* str2)
{
	
	int count1 = 0, count2 = 0, flag = 0;
	char* buf = str1;
	while (*buf != '\0')
		count1++;
	buf = str2;
	while (*buf != '\0')
		count2++;
	
	if (abs(count1 - count2) > 1)
		return 0;
	else if (abs(count1 - count2) == 1) {
		//长的去掉一个字符，即可和短的一样
		if (count1 > count2) {
			while (*str1 != '\0' && *str2 != '\0')
				if (*str1 != *str2) {
					str1++;
					flag++;
				} else {
					str1++;
					str2++;
				}
			if (flag == 1)
				return 1;
			return 0;
		} else {
			while (*str1 != '\0' && *str2 != '\0')
				if (*str1 != *str2) {
					str2++;
					flag++;
				} else {
					str1++;
					str2++;
				}
			if (flag == 1)
				return 1;
			return 0;					
		}	
	} else {	
		while (*str1 != '\0' && *str2 != '\0')
			if (*str1 != *str2)
					flag++;
			else {
				str1++;
				str2++;
			}
		if (flag == 1)
			return 1;
		return 0;		
	}
		
	
	
}

//寻找某一个峰值，即它会比相邻左右数都大
//二分查找即可
//如果nums[i] > nums[i+1]，则在i之前一定存在峰值元素
//如果nums[i] < nums[i+1]，则在i+1之后一定存在峰值元素

//最大间距，求未排序的俩数中的最大间距
//先使用桶排序其数组

//将分数算出小数来然后转成字符串
void fraction_to_recurring_decimal(int numerator, int denominator, int n)
{
	//尝试先计算出来并保存下来，作为原始字符串
	char* raw_str = (char*)calloc(n + 1, sizeof(char));
	char* new_str = (char*)calloc(n + 1, sizeof(char));
	int index = 0;
	//正化
	if ((numerator > 0 && denominator < 0) || \
		(numerator < 0 && denominator > 0))
		raw_str[index++] = '-';
	numerator = abs(numerator);
	denominator = abs(denominator);
	//先把整数部分全算出来
	if (numerator > denominator) {
		int interger = numerator / denominator;
		numerator = numerator % denominator;
		//把该整数按10为单位填入，采用栈逆序它
		int* help_stack = (int*)calloc(n,sizeof(int));
		int i = -1;
		while (interger > 0 && i < n) {
			help_stack[++i] = interger % 10;	
			interger /= 10;
		}
		while (i >= 0 && index <n)
			raw_str[index++] = '0' + help_stack[i--];	
		free(help_stack);
	} else 
		//总不能直接写小数部分
		raw_str[index++] = '0';
	//写小数点
	raw_str[index++] = '.';	
	
	//开始写小数部分，现在是刚好
	//numerator < denominator,此时我们,每一位对numerator扩大10倍
	while (index < n) {
		numerator *= 10;
		int one_data = numerator / denominator;
		numerator = numerator % denominator;
		raw_str[index++] = '0' + one_data;
	}
	
	//验证计算是否成功
	printf("%s\n", raw_str);
	
	//将包括.的前面一部分拷贝
	for (index = 0; index < n; index++) {
		new_str[index] = raw_str[index];
		if (raw_str[index] == '.') {
			index++;
			break;
		}
	}
	
	//开始尝试，从这个很大的数组中寻找一个循环
	//因为无限循环小数有可能会出现前面相当一部分不循环
	//所以使用快慢索引去匹配，这里使用暴力搜索，可以采用KMP
	int i = index, j = i, k = 0, flag = 0;
	while (i < n) {
		//从i开始寻找下一个与i相等的
		j++;
		while (j < n && raw_str[i] != raw_str[j])
			j++;
		
		//现在从i开始到j - 1这段可能会重复
		for (k = 0; k < j - i; k++) 
			if (raw_str[i + k] != raw_str[j + k])
				break;
		//唯一证明其成功的条件	
		if (k == j - i) {
			//若遇到相同的数，其实并无法判断是否是循环，可能只是一部分
			//所以考虑错开比较，再一次验证其对错
			int other_j = 2 * j - i;
			//现在从i开始到j - 1这段可能会重复
			for (k = 0; k < other_j - i; k++) 
				if (raw_str[i + k] != raw_str[other_j + k])
					break;		
			if (k == other_j - i)
				flag = 1;
		}
		
		if (flag == 1) {
			//先写入非循环部分
			while (index < i)
				new_str[index] = raw_str[index++];
			//证明成功，现在，把它写入new_str
			new_str[index++] = '(';
			while (index <= j)
				new_str[index] = raw_str[(index++) - 1];
			new_str[index++] = ')';			
			break;
		}
		//匹配失败，但并不能证明i无效，需要去往下一个j
		if (j >= n) {
			j = ++i;
			continue;
		}		
	}
	
	
	
	printf("%s\n", new_str);
	
	free(new_str);
	free(raw_str);
}

//俩数之和，数已经升序排序
bool two_sum_ii(int* array, int left, int right, int target)
{
	bool result = 0;
	//排序
	quick_sort(array, left, right);
	//查找
	int left_index = left, right_index = right;
	while (left_index <= right_index) {
		//现在可以配对了
		if (array[left_index] + array[right_index] == target) {
			printf("%d - %d", left_index, right_index);
			result = 1;
		} else if (array[left_index] + array[right_index] < target)
			left_index++;
		else if (array[left_index] + array[right_index] > target)
			right_index--;
	}
	return result;
}

//求对应序列
//十进制转为二十六进制
//1 -》A …… 26 -》 Z
void find_needed_sequence_i(int x, int n)
{
	int* simulate_stack = (int*)calloc(n, sizeof(int));
	int index = -1;
	while (x > 0) {
		if (x > 26) { 
		//这里的26进制确切的说需要包括26本身
			simulate_stack[++index] = x % 26;
			x /= 26;				
		} else {
			simulate_stack[++index] = x;
			break;
		}
	}
	while (index > -1)
		printf("%c", simulate_stack[index--] + 'A' - 1);
	free(simulate_stack);
}

//求众数，也可采用map计数，此时只需要常量保存众数即可，无要求
//摩尔投票求众数，要求数比n / 2 要多
int find_majority_element(int* array, int left, int right)
{
	int result = 0, count = 0;
	int i = 0;
	for (result = array[left], count = 0, i = left + 1; \
		i <= right; i++) {
		if (array[i] == result)
			count++;
		else
			count--;
		if (count < 0) {
			result = array[i];
			count = 0;
		}
	}
	
	return result;
}

//俩数之和，要求编写类完成插入数据与查找数据的操作
//意思是我可能会多次对数据进行访问，所以对数据访问效率是需要注意的
//此时我可以考虑，在类对象初始化和每次插入数据时对其维护一个哈希表
//保证每次访问target - array[i] 是否存在的时间将为常数时间

//26进制转10进制,注意起点为1而非0
int find_needed_sequence_ii(char* str)
{
	//可以使用逆序的方法，多一次遍历
	//也可以继续使用stack简化计算
	int count = 0, result = 0, i = 0;
	char* buf = str;
	while (*buf++ != '\0')
		count++;
	//此时数据从高到低依次为 0 - count - 1
	for (i = 0; i < count; i++) {
		result *= 26;
		result +=(str[i] - 'A' + 1);
	}
	return result;
	
}

//计算n阶乘有多少个0在末尾
//所有数中唯一可以出现尾部增0仅一种情况，可剥离出一对2 * 5
//则只需要统计全部的2个数与5个数中最小的数即可
//但是因为其倍率关系，2永远比5是多的
int help_factorial_trailing_zeroes(int n, int x)
{
	if (n == 0)  
		return 0;
	else {
		//当前数中2与5个数最小 
		//累加上前n个数中2 与 5 个数最小
		int count = help_factorial_trailing_zeroes(n - 1, x);
		int buf = n;
		while (buf >= x)
			if (buf % x != 0)  
				break;
			else {
				count++;
				buf /= x;
			}			
		return count; 
	}
}

int factorial_trailing_zeroes(int n)
{
	//int count_two = help_factorial_trailing_zeroes(n, 2);
	//int count_five = help_factorial_trailing_zeroes(n, 5);
	//return count_two <= count_five ? \
		count_two : count_five;
	return 	help_factorial_trailing_zeroes(n, 5);
}

//二叉搜索树迭代器，
//因为next()和has_next()是O(1)且空间复杂度为O(n)
//考虑中序遍历按顺序保存其值，记住其插入与删除时对其维护即可
//这不违反空间复杂度，且时间复杂度在常数级，只需要一个游标nonius即可
//另外一种方法是维护一个栈，因为是中序遍历，维护栈是完全可行的
//但对二叉树的插入删除依然需要维护

//地下城游戏，机器人模式走到最后，求所需最少生命值
//这里考虑逆序走
int dunqeon_game(int* grid, int row, int col)
{
	int i = 0, j = 0, result = 0;	
	//申请空间
	int** dp = (int**)calloc(row + 2, sizeof(int));
	for (i = 0; i < row + 2; i++)
		dp[i] = (int*)calloc(col + 2, sizeof(int));

	//初始化
	for (i = 0; i < row + 2; i++)
		dp[i][col + 1] = ~(1 << 31);
	for (j = 0; j < col + 2; j++)		
		dp[row + 1][j] = ~(1 << 31);
	//公主房间特殊考虑
	dp[row + 1][col] = 1;
	dp[row][col + 1] = 1;
	//应该考虑从最后往前走，那么这会很有效	
	//max(1, min(dp[i+1][j], dp[i][j+1]) - dungeon[i][j])。
	//dp[i][j]表示走到 坐标处的所需的最低生命值
	//即当前点往下或者往右走
	for (i = row; i >= 0; i--)
		for (j = col; j >= 0; j--) {			
			int max_health_point = \
				dp[i+1][j] < dp[i][j+1] ? dp[i+1][j] : dp[i][j+1];
			max_health_point -= grid[i * (col + 1) + j];	
			dp[i][j] = 1 > max_health_point ? 1 : max_health_point;				
		}
	//看一看		
	for (i = 0; i < row + 2; i++) {
		for (j = 0; j < col + 2; j++)	
			printf("%d ", dp[i][j]);
		printf("\n");
	}
	result = dp[0][0];
	//释放空间
	for (i = 0; i < row + 2; i++) 
		free(dp[i]);
	free(dp);
	return result;
}

//实现最大的组合数
int help_compare_two_number(int data1, int data2)
{
	//一个int长至多为十几位 4字节 32位 
	int simulate_stack1[15] = {0}, index1 = -1;
	int simulate_stack2[15] = {0}, index2 = -1;
	char data1data2[30] = {0}, d_index1 = -1;
	char data2data1[30] = {0}, d_index2 = -1;
	
	int buf = abs(data1);
	while (buf > 0) {
		simulate_stack1[++index1] = buf % 10;
		buf /= 10;
	}
	buf = abs(data2);
	while (buf > 0) {
		simulate_stack2[++index2] = buf % 10;
		buf /= 10;
	}	
	//现在数据保存在了栈中
	//开始写data1data2
	int i = 0;
	for (i = index1; i >= 0; i--)
		data1data2[++d_index1] = simulate_stack1[i] + '0';
	for (i = index2; i >= 0; i--)
		data1data2[++d_index1] = simulate_stack2[i] + '0';	
	//开始写data2data1
	for (i = index2; i >= 0; i--)
		data2data1[++d_index2] = simulate_stack2[i] + '0';	
	for (i = index1; i >= 0; i--)
		data2data1[++d_index2] = simulate_stack1[i] + '0';
	//现在可以比较了	
	
	//printf("%s - %s - %d\n", data1data2, data2data1, strcmp(data1data2, data2data1));
	return strcmp(data1data2, data2data1);
}

void largest_number(int* array, int left, int right)
{
	//自定义的排序方法
	//即对俩数大小定义为连接字符串ab： a+b > b+a 则 a > b
	//为了使其简单化，直接暴力排序，来首冒泡
	int i = 0, j = 0;
	for (i = left; i <= right; i++)
		for (j = 0; i + j <= right; j++)
			if (help_compare_two_number(array[i], array[i + j]) < 0) {
				int temp = array[i];
				array[i] = array[i + j];
				array[i + j] = temp;
			}

	for (i = left; i <= right; i++)
		printf("%d", array[i]);
}

//左右手翻转字符串，以单词为单位翻转，空格保留
void reverse_words_in_a_string(char* str)
{
	int flag = 0;
	char* buf1 = str, *buf2 = str, *buf = NULL;
	while (flag == 0) {
		buf2 = buf1;
		//找到一个字符串头与尾巴
		while (*buf2 != ' ' && *buf2 != '\0')
			buf2++;	
		//到了最后了
		if (*buf2 == '\0')
			flag = 1;
		buf = buf2;
		//交换
		buf2--;
		while (buf1 < buf2) {
			char temp = *buf1;
			*buf1 = *buf2;
			*buf2 = temp;
			buf1++;
			buf2--;
		}
		buf1 = buf;
		buf1++;
	}
	
	buf1 = str;
	buf2 = str;
	while (*buf2 != '\0')
		buf2++;
	buf2--;
	
	while (buf1 < buf2) {
			char temp = *buf1;
			*buf1 = *buf2;
			*buf2 = temp;
			buf1++;
			buf2--;			
	}
	
	printf("%s", str);	
}

//重复的DNA序列
//注:这里的序列的话，是不可以重合的，且序列的长度是固定的
void repeated_dna_sequences(char* str, int n)
{
	int count = 0;
	char* buf = str;
	while (*buf++ != '\0')
		count++;
	//这里使用滑窗的方式，每入一个新的之后出一个旧的
	//因为只有四种状态，完完全全使用俩个游标即可维护
	if (n <= 16) {
		int i = 0, mask = 0;
		for (i = 0; i < n * 2; i++) 
			mask = (mask << 1) | 1;
		int data_need_comfirm = 0, index = 0;
		int data_buf = 0;	
		//先获得我需要去确认的数
		if (index + n - 1 < count)
			for (i = 0; i < n; i++) {
				data_need_comfirm <<= 2;				
				if (str[index + i] == 'A')
					data_need_comfirm += 0;
				if (str[index + i] == 'C')
					data_need_comfirm += 1;			
				if (str[index + i] == 'G')
					data_need_comfirm += 2;
				if (str[index + i] == 'T')
					data_need_comfirm += 3;				
			}
		//已经初始化need_confirm前十个数完毕了
		while (index + 2 * n - 1 < count) {
			//依照新的index去初始化data_buf;
			if (index + 2 * n - 1 < count)
				for (i = n; i < 2 * n; i++) {
					data_buf <<= 2;
					if (str[index + i] == 'A')
						data_buf += 0;
					if (str[index + i] == 'C')
						data_buf += 1;
					if (str[index + i] == 'G')
						data_buf += 2;
					if (str[index + i] == 'T')
						data_buf += 3;
				}					
			//对一个index，进行全部buf纠察
			for (i = 2 * n; index + i < count; i++) {
				//对初始化的就开始进行纠察
				if ((data_need_comfirm & mask) == \
					(data_buf & mask)) {
					//存在可能且很大可能出现重复，所以可以考虑
					//将数据暂且存储起来，带到最后统一做一次查重即可
					//因为我只需要保存该数据就一定能获得对应的序列
					printf("%d - %d\n", index, index + n);		
					break;
				}				
				//从index + 2 * n 开始移位
				data_buf <<= 2;				
				if (str[index + i] == 'A')
					data_buf += 0;
				if (str[index + i] == 'C')
					data_buf += 1;			
				if (str[index + i] == 'G')
					data_buf += 2;
				if (str[index + i] == 'T')
					data_buf += 3;				
			}
			index++;
			data_need_comfirm <<= 2;				
			if (str[index + n - 1] == 'A')
				data_need_comfirm += 0;
			if (str[index + n - 1] == 'C')
				data_need_comfirm += 1;			
			if (str[index + n - 1] == 'G')
				data_need_comfirm += 2;
			if (str[index + n - 1] == 'T')
				data_need_comfirm += 3;				
		}		
	} else {
		//不能用位，考虑队列的数据结构
		//或者回归原始的哈希映射
	}
}

//买卖股票的最佳时机，可以购买k次股票，但只能持有1只股票
//在上面已经做过了

//数组移动k个位置
//与字符串左右手翻转一样

//颠倒二进制位
//取位换就行了

//计算位1的个数
int calculate_bit_set_from_data(int x) 
{
	//统计最高位考虑使用右移，数右移均是补0
	int mask = 1, i = 0, result = 0;
	for (i = 0; i < 31; i++) {
		if (mask == mask & x)
			result++;
		mask <<= 1;
	}
	return result;
}

//打家劫舍
//dp[i]表示经过i房间时，可以获得的最大金额
//显然有 dp[0]=nums[0];dp[1]=max(nums[0],nums[1]) 
//偷，dp[i]=dp[i−2]+nums[i] 
//不偷，dp[i]=dp[i−1] 
int house_robber_i(int* array, int left, int right)
{
	if (left >= right)
		return array[left];
	int result = 0, i = 0, j = 0;
	int* dp = (int*)calloc(right - left + 1, sizeof(int));
	dp[left - left] = array[left];
	dp[left + 1 - left] = array[left] > array[left + 1] ? \
		array[left] : array[left + 1];
	
	for (i = left + 2; i <= right; i++)
		dp[i - left] = (dp[i - left - 2] + array[i]) > (dp[i - left - 1]) ?
			(dp[i - left - 2] + array[i]) : (dp[i - left - 1]);
	
	result = dp[right - left];
	free(dp);
	return result;
}

//二叉树的右视图
//层次遍历的改造，有效数据为每一层的最后一个数据

//查岛屿数量（被0围起来的1块，默认边界是0包围的）
int help_number_of_islands(bool* grid, int row, int col, int flag, \
	int i, int j)
{
	if (flag == 0) {
		//初始化时，即第一层
		int next_i = 0, next_j = 0, result = 0;
		for (next_i = 0; next_i <= row; next_i++)
			for (next_j = 0; next_j <= col; next_j++)
				if (grid[next_i * (col + 1) + next_j] == 1) {
					//找到可行点，则
					grid[next_i * (col + 1) + next_j] = 0;
					result += help_number_of_islands(grid, row, col, 1, \
						next_i, next_j);								
				}
		return result;		
	} else {
		//检察上一层来的点，是否有效
		int result = 0, count = 0;
		if (i - 1 >= 0 && grid[(i - 1) * (col + 1) + j] == 1) {		
			grid[(i - 1) * (col + 1) + j] = 0;
			result |= help_number_of_islands(grid, row, col, 1, i - 1, j);
		} else 
			count++;
		if (i + 1 <= row && grid[(i + 1) * (col + 1) + j] == 1) {
			grid[(i + 1) * (col + 1) + j] = 0;
			result |= help_number_of_islands(grid, row, col, 1, i + 1, j);	
		} else 
			count++;		
		if (j - 1 >= 0 && grid[i * (col + 1) + j - 1] == 1) {
			grid[i * (col + 1) + j - 1] = 0;
			result |= help_number_of_islands(grid, row, col, 1, i, j - 1);		
		} else 
			count++;	
		if (j + 1 <= col && grid[i * (col + 1) + j + 1] == 1) {
			grid[i * (col + 1) + j + 1] = 0;
			result |= help_number_of_islands(grid, row, col, 1, i, j + 1);
		} else 
			count++;		
		if (count == 4)
			return 1;
		return result;
	}
}

int number_of_islands(bool* grid, int row, int col)
{
	return help_number_of_islands(grid, row, col, 0, 0, 0);
}

//范围数求与
//对一个包含边界的范围内全部整数求与运算
//延伸为内核堆栈求其基地址获得进程描述符，通过掩码运算即可
//补充：连续序列一定会有一个共有头缀，即全部数与后的结果
//它即可使用掩码依次排除
int bitwise_and_of_numbers_range(int left, int right)
{
	//掩码
	int mask = ~0;
	while ((mask & left) != (mask & right))
		mask <<= 1;
	return mask & left;
}

//快乐数
//某个数，对它每个位平方运算后求和
//重复上面操作，最后若结果为1则为快乐数
//需要数据结构记录中间数，因为一旦产生循环立马需要终止且退出
//为提升查找效率可使用哈希表
bool happy_node(int data) 
{
	//哈希表大小暂时随便选取一个，就选data的10倍的大小
	int n = data, result = 0, i = 0, index = 0;
	struct hash_node* buf = NULL;
	struct hash_node** help= \
		(struct hash_node**)calloc(n * 10, sizeof(struct hash_node*));	
	//散列函数为  结果 % (data * 10 - 1)
	while (1) {
		//旧数插入表中
		index = abs(data) % (n * 10 - 1);
		buf = (struct hash_node*)calloc(1, sizeof(struct hash_node));
		//借用前面的哈希结构体，其中的数据成员是index

		buf->index = data;
		buf->next = NULL;
		//数据头插进去
		if (help[index] != NULL)
			buf->next = help[index];
		help[index] = buf;
		
		//根据旧数生成新数
		int mask = 1;
		int new_data = 0;
		while (data > 0) {
			new_data += (data % 10) * (data % 10);  			
			data /= 10;
		}
		data = new_data;			
		//如果出现1
		if (data == 1) {
			result = 1;	
			break;
		}
	
		//查表，如果数据在表中，则退出
		index = abs(data) % (n * 10 - 1);	
		buf = help[index];	
		while (buf != NULL) {
			if (buf->index == data)
				break;
			else
				buf = buf->next;				
		}
		//判断
		if (buf != NULL && buf->index == data) {
			result = 0;
			break;
		}

	}
	
	//清理表
	for (i = 0; i < n * 10; i++) {
		if (help[i] == NULL)
			continue;
		else {
			struct hash_node* buf = NULL;
			while (help[i] != NULL) {
				buf = help[i]->next;
				free(help[i]);
				help[i] = buf;
			}		
		}
	}
	free(help);
	return result;
}

//判断是否是可映射的，即字符通过映射后是否可以相等
bool is_omorphic_strings(char* str1, char* str2)
{
	int hashmap_1[256] = {0};
	int hashmap_2[256] = {0};
	char* buf1 = str1;
	while (*buf1 != '\0')
		hashmap_1[*buf1++]++;
	char* buf2 = str2;
	while (*buf2 != '\0')
		hashmap_2[*buf2++]++;
	
	buf1 = str1;
	buf2 = str2;
	while (*buf1 != '\0' && *buf2 != '\0')
		if (hashmap_1[*buf1++] != hashmap_2[*buf2++])
			return 0;
	return 1;
}

//逆转链表
//迭代和递归

//有向图
//是否存在环，深度优先或者广度优先直接找就可以了
//存在环一定有，某结点开始，到未来的一个时候回到某结点

//字典树
//***
//根节点不包含字符，其他结点仅一个字符
//全路径便是某一个字符串

//最短目标和子序列
//贪心思想
//总和大了，sum减去左边界，左端边界+1
//总和小了，右边界+1，sum加上右边界
int minimum_size_subarray_sum(int* array, int left, int right, int target)
{
	int cur_left = left, cur_right = left;
	int min_left = 1 << 31, min_right = ~0;
	int sum = array[left];
	while (1) {
		//计算总和		
		if (sum < target) {
			cur_right++;
			//右边界超标了退出吧
			if (cur_right > right)
				break;				
			sum += array[cur_right];
		} else if (sum > target) {
			sum -= array[cur_left];			
			cur_left++;		
		} else {
			//找到一个符合要求的范围
			if (abs(min_right - min_left) > abs(cur_right - cur_left)) {
				min_left = cur_left;
				min_right = cur_right;
			}
			sum -= array[cur_left];	
			cur_left++;
		}
	
	}
	
	if (min_left != ~0) 
		printf("%d - %d", min_left, min_right);
	return min_right - min_left + 1;
}

//打家劫舍，现在最开始一个和最后一个是连在一起的
//第一个房间偷，最后一个不偷，或者第一个不偷最后一个偷
int house_robber_ii(int* array, int left, int right)
{
	int max1 = house_robber_i(array, left, right - 1);
	int max2 = house_robber_i(array, left + 1, right);
	return max1 > max2 ? max1 : max2;
}

//一个字符串前面需要补最少多少字符可以满足其
//整体是回文子串
//这里主要是查找回文子串前缀最长可以有多么长
//使用KMP算法可以，未实现
//把s和其转置r连接起来，中间加上一个其他字符，形成一个新的字符串t，
//还需要一个和t长度相同的一位数组 next，
//其中 next[i] 表示从 t[i] 到开头的子串的相同前缀后缀的个数，
//最后把不相同的个数对应的字符串添加到s之前即可
int shortest_palindrome_i(char* str)
{
	int count = 0;
	char* buf = str;
	while (*buf++ != '\0')
		count++;
	//查被 从0 - count - 1开始查，让其往前缩直到遇到第一个回文子串
	//这里不用算法，暴力查找一次
	int i = 0;
	for (i = count - 1; i >= 0; i--) {
		//去证明0 - i是不是回文字符串
		int k = 0, j = i;
		while (k <= j)
			if (str[k] != str[j])
				break;
			else {
				k++;
				j--;
			}
		//证明成功	
		if (k > j) {
			return count - (i + 1);
		}			
	}
	return 1;	
}

//补偿：KMP的next数组
//注意array即next数组的范围是从left+1 到 right+1的
void comfirm_next_array(char* str, int* array, int left, int right)
{
	//默认str与array是一样的长度的，且其范围都是left - right的
	
	//1我们能确定next数组第一二位一定分别为0，1
	//后面求解每一位的next值时，根据前一位进行比较。
	//next数组从1开始起
	array[left + 1] = 0;
	array[left + 2] = 1;
	//2从第三位开始	
	int i = 0;
	for (i = left + 3; i <= right + 1; i++) {
		char prev_char = str[i - 2];
		int k = i - 1;
		int prev_k = array[k];
		//将前一位与其next值对应的内容进行比较
		//直到找到某个位上内容的next值对应的内容与前一位相等为止，		
		while (prev_k > 0)		
			if (prev_char == str[prev_k - 1]) {			
				//2.1如果相等，则该位的next值就是前一位的next值加上1
				//则这个位对应的值加上1即为需求的next值；			
				array[i] = array[k] + 1;
				break;
			} else {
				//2.2如果不等，向前继续寻找next值对应的内容来与前一位进行比较，
				k = prev_k;
				prev_k = array[k];
			}
	
		if (prev_k == 0 && prev_char == str[prev_k - 1]) 
			array[i] = array[k] + 1;
		//如果找到第一位都没有找到与前一位相等的内容
		//那么求解的位上的next值为1。						
		else if (prev_k < 0 || (prev_k == 0 && prev_char != str[prev_k - 1]))
			array[i] = 1;
	}

	/*验证
	for (i = left; i <= right; i++) 
		printf("%c", str[i]);	
	printf("\n");	
	for (i = left + 1; i <= right + 1; i++) 
		printf("%d", array[i]);	
	printf("\n");	
	*/
}

int shortest_palindrome_ii(char* str)
{
	int count = 0;
	char* buf = str;
	while (*buf++ != '\0')
		count++;
	int i = 0;
	char* str_t = (char*)calloc(count + 1,sizeof(char));
	for (i = 0; i < count; i++)
		str_t[count - 1 - i] = str[i];
	char* all_str = (char*)calloc(count * 2 + 1, sizeof(char));
	int* next = (int*)calloc(count * 2 + 2, sizeof(int));
	strcat(all_str, str);
	strcat(all_str, "#");
	strcat(all_str, str_t);	
	//计算next数组
	comfirm_next_array(all_str, next, 0, count * 2);
	
	/*验证
	printf("%s\n", str);	
	printf("%s\n", str_t);
	printf("%s\n",all_str);	
	for (i = 1; i < count * 2 + 2; i++)
		printf("%d",next[i]);
	*/
	
	free(str_t);
	free(all_str);
	free(next);
	return count - next[count * 2 + 1];
}

//数组中的第k个最大元素
int k_th_largest_element_in_an_array(\
	int* array, int left, int right, int k)
{
	//使用快速排序找即可，
	//第k个最大元素映射到快速排序下恰好就是地址k-1的位置
	//如果保证了k - 1的位置是满足快排点的，就达到了要求
	
	//所以我们可以采用快排思想，一次性排除一部分数据，缩小范围到
	//靠近k - 1即可，注意我们并不需要对数组进行排序，但有可能对一小部分数据
	//排序，如果希望在小范围内处理容易一些的话
	
	//在sort.h中有一个partition函数实现单词数据归类
	//它会返回其分割点下标, -2(left >= right), -1(数据太小没必要分)
	while (1) {
		int index = partition(array, left, right);	
		if (index == -2)
			return array[k - 1];
		else if (index == -1) {
			bubble_sort(array, left, right);	
			return array[k - 1];
		} else if (index == k - 1)
			return array[k - 1];
		else if (index < k - 1)
				left = index;
		else if (index > k - 1)
				right = index;
	}
}

//组合总和，1 - n 中的数字，只可使用一次
//保证指定组成数的个数k，和为某一值sum的全部组合
//即从1 - n 中取k个数，即结果为sum
void help_combination_sum(bool* is_used, int n, \
	int start, int k, int sum, int rest)
{
	if (k == 0 && rest == sum) {
		int i = 0;
		for (i = 0; i < n; i++)
			if (is_used[i])
				printf("%d", i);
		printf("\n");
	} else {
		int i = 0;
		for (i = start; i <= n; i++) {
			//剪枝
			if (rest + i > sum)
				break;			
			is_used[i] = 1;
			help_combination_sum(is_used, n, \
				i + 1, k - 1, sum, rest + i);			
			is_used[i] = 0;		
		}		
	}	
}

void combination_sum(int n, int k, int sum)
{
	bool* is_used = (bool*)calloc(n + 1, sizeof(bool));
	help_combination_sum(is_used, n, \
		1, k, sum, 0);	
	free(is_used);
}

//存在重复元素
//使用哈希表，遍历数组将数据插入到表前先查查是否存在于数组
//存在就证明存在重复元素了

//天际线问题
struct skyline_node
{
	int x_point;
	int y_point;
	struct skyline_node* next;
};

struct skyline_node* help_help_merge_skyline( \
	struct skyline_node* left_list, struct skyline_node* right_list)
{	
	if (left_list == NULL)
		return right_list;
	if (right_list == NULL)
		return left_list;
	//因为要尾插，所以采用双指针，减少遍历
	struct skyline_node* new_list_first = NULL;
	struct skyline_node* new_list_last = NULL;	
	struct skyline_node* buf_left = left_list;
	struct skyline_node* buf_right = right_list;
	//合并俩部分为一部分
	int left_height = 0, right_height = 0, cur_height = 0;
	int buf_x_left = buf_left->x_point;
	int buf_x_right = buf_right->x_point;	
	while (buf_left != NULL || buf_right != NULL) {


		//取一个最小的x
		if (buf_left != NULL && buf_x_left <= buf_x_right) {		
			left_height = buf_left->y_point;	
			left_list = buf_left->next;
			int max_height = left_height > right_height ? \
				left_height : right_height;			
			//是否要更新
			//现在我要使用buf_left了			
			//我会考虑是，释放掉它还是插到别的地方去			
			if (max_height != cur_height) {
				//需要更新，该点有效
				//修改buf_left并将其插入到总链表中
				buf_left->y_point = max_height;
				buf_left->next = NULL;
				cur_height = max_height;
				if (new_list_last == NULL) {
					new_list_first = buf_left;
					new_list_last = buf_left;
				} else { 
					new_list_last->next = buf_left;
					new_list_last = buf_left;
				}	
			} else
				free(buf_left);			
			buf_left = left_list;
			if (buf_left == NULL)
				buf_x_left = ~(1 << 31);
			else
				buf_x_left = buf_left->x_point;
		}
		if (buf_right != NULL && buf_x_left >= buf_x_right) {
			right_height = buf_right->y_point;
			right_list = right_list->next;		
			int max_height = left_height > right_height ? \
				left_height : right_height;
			//是否要更新
			//现在我要使用buf_right了			
			//我会考虑是，释放掉它还是插到别的地方去			
			if (max_height != cur_height) {
				//需要更新，该点有效
				//修改buf_right并将其插入到总链表中
				buf_right->y_point = max_height;
				buf_right->next = NULL;
				cur_height = max_height;
				if (new_list_last == NULL) {
					new_list_first = buf_right;
					new_list_last = buf_right;
				} else { 
					new_list_last->next = buf_right;
					new_list_last = buf_right;
				}	
			} else
				free(buf_right);						
			buf_right = right_list;
			if (buf_right == NULL)
				buf_x_right = ~(1 << 31);
			else
				buf_x_right = buf_right->x_point;			
		}
	}
	return new_list_first;
}

struct skyline_node* help_merge_skyline( \
	int* left_border, int* right_border, int* height, \
	int left, int right)
{
	if (left > right)
		//木有楼，此时不存在天际线
		return NULL;
	else if (left == right) {
		//仅一栋楼，此时天际线为
		struct skyline_node* buf1 = \
			(struct skyline_node*)calloc(1, sizeof(struct skyline_node));
		struct skyline_node* buf2 = \
			(struct skyline_node*)calloc(1, sizeof(struct skyline_node));			
		buf1->x_point = left_border[left];
		buf1->y_point = height[left];
		buf2->x_point = right_border[left];
		buf2->y_point = 0;
		buf1->next = buf2;
		buf2->next = NULL;		
		//应该是尾插入	
		return buf1;
	} else {
		//开始分治，将其分成俩半
		int center = (left + right) / 2;
		//治理左半部分
		struct skyline_node* left_partition = \
			help_merge_skyline(left_border, right_border, height, \
			left, center);	
		//治理右半部分	
		struct skyline_node* right_partition = \
			help_merge_skyline(left_border, right_border, height, \
			center + 1, right);
		//合并俩部分
		return help_help_merge_skyline(left_partition, right_partition);	
	}	
}

void skyline(int* left_border, int* right_border, int* height, \
	int left, int right)
{
	//调整一下哈，以左边界为参考排下序
	//此处为了减少编码直接选择了暴力排序，应该考虑OlogN的排序算法
	//以左边界排序而已，顶多是进行了三次数据交换了
	int i = 0, j = 0;
	for (i = left; i <= right; i++)
		for (j = 0; i + j <= right; ++j)
			if (left_border[i] > left_border[i + j]) {
					swap(left_border, i, i + j);	
					swap(right_border, i, i + j);	
					swap(height, i, i + j);					
	}
	
	//折半分治，最后合并
	//结果存放在其链表之中	
	struct skyline_node* list = \
		help_merge_skyline(left_border, right_border, height, \
		left, right);
	
	struct skyline_node* buf = list;
	while (buf != NULL) {
		list = list->next;
		//验证数据
		printf("%d - %d\n", buf->x_point, buf->y_point);
		free(buf);
		buf = list;
	}
	
}

//查找重复数
//俩个重复数之间他们的坐标差不可以超过k
//最多可存在俩组重复数，还是用哈希表就可以了

//查找重复数
//给定一个整数数组，判断数组中是否有两个不同的索引 i 和 j，
//使得 nums [i] 和 nums [j] 的差的绝对值最大为 t，
//并且 i 和 j 之间的差的绝对值最大为 ķ。
//1.将符合范围内的i到j的值插入到快速搜索数据结构中
//滑动窗户后往后，前出一个数据，后进一个数据
//维持条件索引差最大为k就行
//2.其中对i的数据纠察，是否出现不满足下述行为的数据对
//其中对于 num[i] num[j] 和 t ，我们需要在快速搜索数据结构中
//其中在高级语言中有标准库实现了set的lower_bound方法
//完成范围查找,它会找到第一个不满足条件的点位，迭代器的方式，
//则在这个范围内全部的数据都会是满足要求的数据
//3. abs(num[i] - num[j]) <= t     ===>
//		num[j] - t <= num[i] <= num[j] + t
//满足的范围在low_bound(num[j] - t) - (low_bound(num[j] + t) - 1)

//矩阵求最大1的正方形
//dp[i][j] = min(dp[i - 1][j], dp[i][j - 1], dp[i - 1][ j - 1])
int maximal_square(char* array, int row, int col)
{
	int i = 0, j = 0, result = 0, min = 0;
	int** dp = (int**)calloc(row + 2, sizeof(int*));
	for (i = 0; i < row + 2; i++) 
		dp[i] = (int*)calloc(col + 2, sizeof(int));
		
	for (i = 1; i < row + 2; i++)	
		for (j = 1; j < col + 2; j++)
			if (array[(i - 1) * (col + 1) + (j - 1)] == '1') {
				min = dp[i - 1][j] < dp[i][j - 1] ? \
					dp[i - 1][j] : dp[i][j - 1];
				min = min <	dp[i - 1][ j - 1] ? \
					min :	dp[i - 1][ j - 1];
				dp[i][j] = 1 + min;		
				if (result < dp[i][j])
					result = dp[i][j];
			}
	
	/*
	for (i = 0; i < row + 2; i++) {	
		for (j = 0; j < col + 2; j++)
			printf("%d ", dp[i][j]);
		printf("\n");
	}
	*/
	
	for (i = 0; i < row + 2; i++) 
		free(dp[i]);	
	free(dp);
	return result;
}

//完全二叉树的结点个数
//完全二叉树的左右孩子也是一个完全二叉树
//从下层往上算就可，递归算很简单

//俩个矩形的总面积，他们可能会重叠
int rectangle_area(int rect1_x1, int rect1_x2, int rect1_y1, int rect1_y2, \
	int rect2_x1, int rect2_x2, int rect2_y1, int rect2_y2)
{
	//假设不重叠的话
	int result = (rect1_x2 - rect1_x1) * (rect1_y2 - rect1_y1);
	result += (rect2_x2 - rect2_x1) * (rect2_y2 - rect2_y1);
	//判断是否重叠
	//看是否出现一个矩形顶点被含于另一个矩形中	
	int offset = 0;
	
	//矩形2的左下角rect2_x1,rect2_y1顶点包含在矩形1中
	if ((rect1_x1< rect2_x1 && rect2_x1 < rect1_x2) && \
		(rect1_y1< rect2_y1 && rect2_y1 < rect1_y2))
		offset = (rect1_x2 - rect2_x1) * (rect1_y2 - rect2_y1);
	//矩形2的左上角rect2_x1,rect2_y2顶点包含在矩形1中
	if ((rect1_x1< rect2_x1 && rect2_x1 < rect1_x2) && \
		(rect1_y1< rect2_y2 && rect2_y2 < rect1_y2))	
		offset = (rect1_x2 - rect2_x1) * (rect2_y2 - rect1_y1);
	//矩形2的右下角rect2_x2,rect2_y1顶点包含在矩形1中
	if ((rect1_x1< rect2_x2 && rect2_x2 < rect1_x2) && \
		(rect1_y1< rect2_y1 && rect2_y1 < rect1_y2))		
		offset = (rect2_x2 - rect1_x1) * (rect1_y2 - rect2_y1);
	//矩形2的右上角rect2_x2,rect2_y1顶点包含在矩形1中
	if ((rect1_x1< rect2_x2 && rect2_x2 < rect1_x2) && \
		(rect1_y1< rect2_y2 && rect2_y2 < rect1_y2))			
		offset = (rect2_x2 - rect1_x1) * (rect2_y2 - rect1_y1);
		
	return result - offset;		
}

//使用队列实现栈
//使用俩个队列维护即可，只能使用标准的入队列和出队列
//在新元素入队前，队中所有数据全部出队并顺序入队到辅助队列中
//新元素入队，辅助队列数据重新出队并入队到原队列
//如是使用链表模拟队列，可以考虑把队列数据全部看做一个大数据出队
//然后再入队，等价操作就是头插

//翻转二叉树
//使用递归，先翻转左右结点，再翻转本节点
//使用队列，按层遍历翻转即可

//汇总区间
//都排序好了，也不重复，遍历查就可以了，只要记录区间头和上一元素值
//迭代下去就可以知道下一元素是否被包含，一旦不包含记录就是区间尾
//然后就是下一区间

//求众数
//摩尔投票法，则现在思考了可以使用摩尔投票法
//票选多位候选人
void find_majority_element_ii(int* array, int left, int right)
{
	int result1 = 0, result2 = 0, count1 = 0, count2 = 0;
	int i = 0;
	//初始化起点,从0开始此时没有有效数据所以计数为0	
	//中间更新候选者时，更新一次自己算一次，计数更新为1
	for (count1 = 0, count2 = 0, i = left; i <= right; i++) {
		if (array[i] == result1)
			count1++;
		else if (array[i] == result2)
			count2++;
		else if (count1 == 0) {
			result1 = array[i];
			count1 = 1;
		} else if (count2 == 0) {
			result2 = array[i];
			count2 = 1;
		} else {
			count1--;
			count2--;		
		}	
	}
	//判断投票是否有效
	for (count1 = 0, count2 = 0, i = left; i <= right; i++) {
		if (array[i] == result1)
			count1++;
		else if (array[i] == result2)
			count2++;
	}
	if (count1 >= (right - left + 1) / 3)
		printf("%d ", result1);
	if (count2 >= (right - left + 1) / 3)
		printf("%d ", result2);

}

//求k个最小的数
//中序递归遍历，往下走就可以了，只需要
//使用一个计数器纪录结点的遍历情况，其中多结点并不受影响
//先遍历左孩子，再遍历根，后遍历右孩子

//断言一个数是不是2的幂
//对于二的幂都是尾数为0，仅2^0 为 1
//所以不断地右移即可，去掉符号位
bool is_power_of_two(int data)
{
	//去掉符号位，保证右移补零
	if (data < 0)
		data = ~data + 1;
	while (data > 0) {
		if (data == 1)
			return 1;			
		else if (data & 1 == 1)
			return 0;
		else
			data >>= 1;	
	}	
	return 1;
	
}

//栈实现队列
//按这里说栈只能压入与弹出，那么依然是先来俩个栈
//一个栈负责所有入队列操作（就是压栈）
//出队列则是将该栈数据全部依次弹出，然后依次压入辅助栈
//最后弹出辅助栈栈顶元素即可完成出队列，之后将辅助栈数据还原
//到主栈中即可

//求1到n中全部1的个数
//在timo.hpp中写了

//回文链表
//快慢指针找到链表中间，逆序前半部分，
//然后一个指针在开始一个指针在中间
//滑动比较即可


//最近公共祖先
//定义为：“对于有根树 T 的两个结点 p、q，
//最近公共祖先表示为一个结点 x，
//满足 x 是 p、q 的祖先且 x 的深度尽可能大（
//一个节点也可以是它自己的祖先）。”

//二叉搜索树
//如果2值分别在root值两侧，或者有一个值就是root，那么root就是答案
//如果都小于root，在左子树里递归查找
//如果都大于root，在右子树里递归查找
//二叉树
//如果左右子树都找到了，就返回root
//如果只有一边找到就返回非空的那边
//如果都没有找到就返回NULL

//除自身以外数组的乘积
//不可以用除法运算符
void product_of_array_except_self(int* array, int left, int right)
{
	//我们算把结果存在结果数组中
	int* result = (int*)calloc(right - left + 1, sizeof(int));
	int i = 0, buf = 1;
	//先将各数的前部积求出来，存在结果数组中
	for (result[0] = 1, i = 1; i < right - left + 1; i++)
		result[i] = array[i - 1 - left] * result[i - 1];
	//现在计算个数的后部积
	for (i = right - left - 1; i >= 0; i--) {
		result[i] *= (array[i + 1 - left] * buf);
		buf *= array[i + 1 - left];
	}

	for (i = 0; i < right - left + 1; i++)
		printf("%d ", result[i]);	
}

//滑动窗口
//将双端队列看做栈，单调递减栈法。
//1.过了窗口的范围的删除
//2.在窗口范围内的k个，每加入一个数nums[i]，只保留前面比它大的，
//<=nums[i] 的没必要存在，该窗口内它是比较大的
void sliding_window_maximum(int* array, int left, int right, int k)
{
	//这里所有数据顶多入队出队一次，那么完全可以使用数组模拟队列
	//使用原数组就可做队列，我只需要俩个游标nonius即可
	int queue_start = 0;
	int queue_end = 0;
	
	//先初始化，这里顶多到k - 2就可以了,即有k - 1个数 
	while (queue_end < k - 1) {
		//入队一个元素
		queue_end++;
		//出队所有比当前入队元素小的
		while (array[queue_start] < array[queue_end])
			queue_start++;
	}
	
	while (queue_end <= right) {
		printf("%d ",array[queue_start]);
		//入队一个元素
		queue_end++;
		//出队所有比当前入队元素小的
		while (array[queue_start] < array[queue_end])
			queue_start++;		
		
	}
	
	
}

//二维矩阵找目标值
//其中二维矩阵满足行和列都是从小到大排序了的
//可以按与L型相似的路径去查找，因为左下角或右上角
//这俩个点恰好衔接一行与一列，一次查一行+一列然后缩小范围
//还可以分治，取矩阵中心，这样子一次可以排除一块范围
//将剩下的范围分成几部分继续分治即可

//给合法表达式添加括号使其变成不同运算，打印所有结果
void help_difference_way_to_add_parentheses(char* str, \
	int left, int right, int flag_count, int is_first_level, \
	int* result_set)
{
	if (flag_count == 0) {
		//取范围left-right的数
		int data = 0, i = 0;
		for (i = left; i <= right; i++)
			if ('0' < str[i] && str[i] < '9') {
				data *= 10;
				data += str[i] - '0';
			}	
		if (result_set == NULL) {
			printf("%d ", data);			
		} else {
			//结果存入结果集中
			int k = 0;
			for (k = 0; k < right - left + 1; k++)
				if (result_set[k] == (1 << 31)) {
					result_set[k] = data;
					break;
			}					
		}	
	} else {
		int count = 0, i = 0, j = 0, k = 0, t = 0;
		int* result_left = (int*)calloc(right - left + 1, sizeof(int));	
		int* result_right = (int*)calloc(right - left + 1, sizeof(int));		
		for (i = left; i <= right; i++) {
			if (str[i] == '+' || str[i] == '-' || \
				str[i] == '*' || str[i] == '/') {		
				//初始化下层结果集
				for (t = 0; t < right - left + 1; t++) {
					result_left[t] = (1 << 31);
					result_right[t] = (1 << 31);
				}	
				//准备结果集
				count++;
				//这是我遇到的第count个符号，前面还有count - 1个符号
				//后面还有flag_count - count个符号				
				help_difference_way_to_add_parentheses(str, \
					left, i - 1, count - 1, 0, result_left);			
				help_difference_way_to_add_parentheses(str, \
					i + 1, right, flag_count - count, 0, result_right);			
				//取结果集中所有不为0的数做组合
				for (j = 0; j < right - left + 1; j++) {
					if (result_left[j] == (1 << 31))
						continue;
					for (k = 0; k < right - left + 1; k++) {
						if (result_right[k] == (1 << 31))
							continue;
						int data1 = result_left[j];
						int data2 = result_right[k];
						int data = 0;
						switch (str[i]) {
						case '+':
							data = data1 + data2;
							break;
						case '-':
							data = data1 - data2;	
							break;								
						case '*':
							data = data1 * data2;
							break;
						case '/':
							data = data1 / data2;
							break;
						default:
							break;
						}	
						if (is_first_level == 1)
							printf("%d ", data);	
						else {
							//结果存入结果集中
							for (t = 0; t < right - left + 1; t++)
								if (result_set[t] == (1 << 31)) {
									result_set[t] = data;
									break;
							}						
						}		
					}		
				}
			}
		}
		free(result_left);
		free(result_right);		
	}	
}

void difference_way_to_add_parentheses(char* str)
{
	int count = 0, flag_count = 0;
	char* buf = str;
	while (*buf++ != '\0') {
		count++;		
		if (*buf == '+' || *buf == '-' || \
			*buf == '*' || *buf == '/')
			flag_count++;
	}
	//现在需要对0 - count_1的字符串进行处理了
	help_difference_way_to_add_parentheses(str, \
		0, count - 1, flag_count, 1, NULL);
}

//验证是否是变位词
//如果是Unicode的话，考虑将其使用int类型的容器装载
//先将其转为int然后hash映射到int的容器中即可,multiset(红黑树)
bool vaild_anagram(char* str1, char* str2)
{
	char* hash_str1[256] = {0};
	char* hash_str2[256] = {0};
	char* buf = str1;
	while(*buf != '\0')
		hash_str1[*buf++]++;
	buf = str2;
	while(*buf != '\0')
		hash_str2[*buf++]++;
	int i = 0;
	for (i = 0; i < 256; i++)
		if (hash_str1[i] != hash_str2[i])
			return 0;
	return 1;
}

//最短单词距离
//在顺序容器中存放各种字符串，会重复，求俩个不同字符串最近的距离
//将其所有出现的下标分别存放在一个容器中，设好标号
//然后根据归并思想，俩俩比较找最小距离即可
//第二种，假如本函数给多次调用（密集查找），可以直接实现一个哈希表
//这里就避免了每一次的遍历数组，我只需要将哈希表中所有出现下标取出
//存放在一个容器中俩俩比较即可
//如果说现在查找的单词是相同的，那么与前面的查找方法不一样，此时就
//需要取出对应的下标存于数组，遍历找最短距离即可，所以代码整合时需要一开始
//判断是否是相同的字符串查找
int shortest_word_distance(char** dir, int n, char* str1, char* str2)
{
	//简便着想，将哈希表直接以字符串首字母作参考映射算了
	struct hash_node* buf = NULL;
	struct hash_node** help = \
		(struct hash_node**)calloc(256, sizeof(struct hash_node*));
	int i = 0, j = 0;
	for (i = 0; i < n; i++) {
		if (dir[i] == NULL)
			continue;
		int index = *dir[i];
		buf = (struct hash_node*)calloc(1, sizeof(struct hash_node));
		buf->index = i;
		buf->next = NULL;
		//将结点放入哈希映射中，头插法
		buf->next = help[index];
		help[index] = buf;
	}
	
	//查出与str1相等的字符串的下标，存放在数组中
	int* buf_str1 = (int*)malloc(n * sizeof(int));
	int* buf_str2 = (int*)malloc(n * sizeof(int)); 
	for (i = 0; i < n; i++) {
		buf_str1[i] = -1;
		buf_str2[i] = -1;
	}

	//查str1
	int index1 = *str1, nonius1 = -1;
	buf = help[index1];
	while (buf != NULL) {
		if (0 == strcmp(dir[buf->index], str1)) {
			buf_str1[++nonius1] = buf->index;
		}
		buf = buf->next;
	}

	int index2 = *str2, nonius2 = -1;
	buf = help[index2];
	while (buf != NULL) {
		if (0 == strcmp(dir[buf->index], str2)) {
			buf_str2[++nonius2] = buf->index;
		}
		buf = buf->next;
	}	

	//排序buf_str1和buf_str2
	bubble_sort(buf_str1, 0, nonius1);
	bubble_sort(buf_str2, 0, nonius2);

	int result = ~(1 << 31);
	if (0 != strcmp(str1, str2)) {		
		//则现在只需要找俩个数组的最小值了		
		i = 0; 
		j = 0;
		while (i <= nonius1 && j <= nonius2) {
			if (buf_str1[i] < buf_str2[j]) {
				if (buf_str2[j] - buf_str1[i] < result)
					result = buf_str2[j] - buf_str1[i];
				if (i <= nonius1)	
					i++;	
			} else {
				if (buf_str1[i] - buf_str2[j] < result)
					result = buf_str1[i] - buf_str2[j];
				if (j <= nonius2)	
					j++;
			}
		}		
	} else 
		//仅需要遍历一个数组即可
		for (i = 0; i + 1 <= nonius1; i++) 
			if (buf_str1[i + 1] - buf_str2[i] < result)
				result = buf_str1[i + 1] - buf_str2[i];	

	//清理表
	for (i = 0; i < n * 10; i++) {
		if (help[i] == NULL)
			continue;
		else {
			struct hash_node* buf = NULL;
			while (help[i] != NULL) {
				buf = help[i]->next;
				free(help[i]);
				help[i] = buf;
			}		
		}
	}
	free(help);
	return result;
}

//对称数
//旋转180度和原来一样
//求回文数的特殊情况，相等则必须为018，不相等必须一个为6一个为9
//求n位可以有多少个对称数，并打印出来
bool strobogrammatic_number_i(int data)
{
	//直接搞个数组把数一位一位装起来
	int buf[20] = {0}, index = -1, i = 0;
	while (data > 0) {
		buf[++index] = data % 10;
		data /= 10;
	}
	//开始判断吧
	while (i < index) {
		if (buf[i] == buf[index]) {
			if (!(buf[i] == 0 || \
				buf[i] == 1 || buf[i] == 8))
				return 0;
		} else {
			if (!((buf[i] == 6 || buf[index] == 9) || \
				(buf[i] == 9 || buf[index] == 6)))
				return 0;
		}
		i++;
		index--;
	}
	return 1;
}

void help_strobogrammatic_number_ii(int* help, int old_left, int old_right, \
	int new_left, int new_right)
{
	if (new_left > new_right) {
		int i = 0;
		for (i = old_left; i <= old_right; i++) {
			printf("%d", help[i]);
		}
		printf("\n");
	} else {
		int buf[5] = {0,1,6,8,9}, i = 0;
		for (i = 0; i < 5; i++) {
		//最外层不能有0
		if (new_left == old_left && new_right == old_right) 
			if (buf[i] == 0)
				continue;
			//出现奇数，中间仅1位了6和9就会失效
			if (new_right == new_left) 
				if (buf[i] == 6 || buf[i] == 9)
					continue;
			//普通情况
			help[new_left] = buf[i];
			if (buf[i] != 6 && buf[i] != 9)	
				help[new_right] = buf[i];
			else
				help[new_right] = 6 + 9 - buf[i];
			//填下一层
			help_strobogrammatic_number_ii(help, old_left, old_right, \
				new_left + 1, new_right - 1);				
		}	
		
	}
}	

void strobogrammatic_number_ii(int n)
{
	if (n <= 0)
		return;
	else if (n == 1)
		printf("0 1 8");
	else {
		int* help = (int*)calloc(n, sizeof(int));
		help_strobogrammatic_number_ii(help, 0, n - 1, \
			0, n - 1);
		free(help);		
	}	
}

int help_strobogrammatic_number_iii(int old_left, int old_right, \
	int new_left, int new_right)
{
	if (new_left > new_right) {
		return 1;
	} else {
		//确定new_left和new_right的值
		int count = 0;
		int buf[5] = {0,1,6,8,9}, i = 0;
		for (i = 0; i < 5; i++) {
			//最外层不能有0
			if (new_left == old_left && new_right == old_right) 
				if (buf[i] == 0)
					continue;
			//出现奇数，中间仅1位了6和9就会失效
			if (new_right == new_left) 
				if (buf[i] == 6 || buf[i] == 9)
					continue;
			count += help_strobogrammatic_number_iii(old_left, old_right, \
				new_left + 1, new_right - 1);				
		}
		return count;		
	}
}	

int strobogrammatic_number_iii(int n)
{
	if (n <= 0)
		return 0;
	else if (n == 1)
		return 3;
	else
		return help_strobogrammatic_number_iii(0, n - 1, 0, n - 1);	
}

//查找字符串数组集中偏移字符串，
//即字符串每个字符加同一偏移量会转化为另一偏移字符串
//使用is_used[]去掉重复项,使用hashmap映射它们，同一系列的数据使用可自排序的容器盛装
//建议使用红黑树等查询以及调整速度够快的数据结构如set

//同值子树，该子树的所有节点都拥有相同的数值。
//则叶子层一定是同值子树，非叶子层取决于，左子树与右子树本身是同值子树
//且左右子树结点与本节点值同值，即可证明本节点是同值子树
//使用全局变量计数即可

//会议室问题
//有许多会，有起始和结束时间，现在判断这些会能不能开（会不会冲突）
//在保证不冲突的情况下需要使用多少会议室
//先排序
//开始时间一样，先结束的在前；开始早的在前
//优先队列存储会议结束的时间，堆顶是结束时间早的
//下一个会议开始时间早于堆顶的房间结束时间，该会议新开一个room，push进队列
//最后返回队列的size
int meeting_problem(int* start_time, int* end_time, int left, int right)
{
	//重排，为了简便直接使用冒泡排序
	int i = 0, j = 0;
	for (i = left; i <= right; i++)
		for (j = 0; i + j <= right; ++j)
			if (start_time[i] > start_time[i + j]) {
				swap(start_time, i, i + j);
				swap(end_time, i, i + j);
			}
	//继续排序，开始时间相同则结尾
	int start = 0, end = 0;
	for (start = left; start < right; start++) {
		if (start_time[start] == start_time[start + 1]) {
			for (end = start + 1; end <= right; end++)
				if (start_time[start] != start_time[end])
					break;
		//重排
		for (i = start; i < end; i++)
			for (j = 0; i + j < end; ++j)
				if (end_time[i] > end_time[i + j]) {
					swap(start_time, i, i + j);
					swap(end_time, i, i + j);
				}
		start = end - 1;				
		}
	}

	int simulate_stack_top = ~(1 << 31);
	int index = 0;
	for (i = left; i <= right; i++) {
		if (start_time[i] < simulate_stack_top) {
			index++;
			simulate_stack_top = end_time[i];
		}
	}
	return index;
}

//求一数的全部的组的玩备因子集
void help_factor_combination(int* record, int index, int current_data, int start, int end)
{
	if (current_data <= 1) {
		int i = 0;
		for (i = 0; i < index; i++)
			printf("%d ", record[i]);
		printf("\n");
	} else {
		int i = 0;
		for (i = start; i < end; i++) {
			if (current_data % i == 0) {
				record[index] = i;
				if (current_data != i) //剪枝所用
					start = i;
				help_factor_combination(record, index + 1, current_data / i, start, end);
			}
		}
	}
}

void factor_combination(int data)
{
	//为了方便使用数组，但数据大应该使用链表或数学优化
	int* record = (int*)calloc(data, sizeof(int));
	help_factor_combination(record, 0, data, 2, data - 1);
	free(record);
}

//验证前序遍历序列二叉搜索树
bool  verify_preorder_sequence_in_binary_search_tree(int* tree,int left, int right)
{
	int* simulate_stack = (int*)calloc(right -left + 1, sizeof(int));
	int index = -1, min_data = (1 << 31);
	int result = 1, i = 0;
	for (i = left; i <= right; i++) {
		//利用右比左大即可
		//栈无元素或者栈顶元素大于当前元素		
		if (index < 0 || simulate_stack[index] > tree[i]) {
			if (tree[i] > min_data)
				simulate_stack[++index] = tree[i];
			else {
				result = 0;		
				break;			
			}				
		} else {
			//如果栈顶元素小于当前元素
			while (index >= 0 && simulate_stack[index] < tree[i])
				min_data = simulate_stack[index--];
			simulate_stack[++index] = tree[i];	
		}
	}
	free(simulate_stack);
	return result;
} 

//给连续的房子刷墙，且墙不可以相邻同色
//dp[i][c] 表示刷完 i 房子后，i 房子是 c 颜色时，花费最小多少
//dp[i][c] = min(dp[i - 1][q] + house[i][c]) q != c;
//0红1绿2蓝，对应的钱数
int paint_house(int* house[3], int left, int right)
{
  int i = 0, j = 0, k = 0;
  int** dp = (int**)calloc(right - left + 1, sizeof(int*));
  for (i = 0; i < right - left + 1; i++)
    dp[i] = (int*)calloc(3, sizeof(int));
  int result = ~(1 << 31);
  for (i = 1; i < right -left + 1; i++)
      for (j = 0; j < 3; j++) {
        int min[2] = {0}, index = -1;
        for (k = 0; k < 3; k++)
          if (k != j)
            min[++index] = dp[i - 1][k];
        dp[i][j] = (min[0] < min[1] ? min[0] : min[1]) + \
          house[i - 1 + left][j];
      }
  /*验证
  for (i = 0; i < right -left + 1; i++) {
      for (j = 0; j < 3; j++)
        printf("%d ",dp[i][j]);
      printf("\n");
  }
  */

  //最小值
  for(i = 0; i < 3; i++)
    if (result > dp[right - left][i])
      result = dp[right - left][i];

  for (i = 0; i < right - left + 1; i++)
    free(dp[i]);
  free(dp);
  return result;
}

//二叉树所有路径
//深度优先遍历即可

//将数各位相加，直到只剩下一位
int add_digits(int x)
{
	if (x != 0)
		return (x - 1) % 9 + 1;
	return x;
}

//求和小于目标值的三元组
int sum3_small_than_target(int* array, int left, int right, int target)
{
	//先排序
	quick_sort(array, left, right);
	int result = 0;
	int i = 0, j = 0, k = 0;
	for (i = left; i <= right - 2; i++) {
	//固定第一个数
		int buf_left = i + 1, buf_right = right;
		while (buf_left < buf_right) {
			if (array[i] + array[buf_left] + array[buf_right] < target) {
				result += buf_right - buf_left;
				buf_left++;
			} else
				buf_right--;
		}
	}
	return result;
}

//求数组中俩个不同的数字
//仅俩个数字出现一次，其他数字均出现俩次
void single_number_iii(int*array, int left, int right)
{
	int sum = 0, sum1 = 0, sum2 = 0, i = 0;
	for (i = left; i <= right; i++)
		sum ^= array[i];
	//取位，取最低的位1即可，为了方便
	sum &= -sum;
	//现在根据这个标签分组就可以求出结果了
	for (i = left; i <= right; i++) 
		if ((array[i] & sum) != 0)
			sum1 ^= array[i]; 
		else
			sum2 ^= array[i];
	
	printf("%d - %d", sum1, sum2);
}

//判断图是否可以变成树
//通过深度优先搜索即可，只需要准备一个使用标记集合即可
//所有数据皆联通且不成环即为树

//求丑叔集，求x个丑叔
void ugly_number_set(int n)
{
	int* ugly_set = (int*)malloc(n * sizeof(int));
	ugly_set[0] = 1;
	int i = 0, j = 0;
	for (i = 1; i < n; i++)	{
		//下一个数会是，比前一个数大的最小数
		int next_and_two = 0, next_and_three = 0, next_and_five = 0;
		for (j = i - 1; j >= 0; j--)
			if (ugly_set[j] * 2 <= ugly_set[i - 1])
				break;
		next_and_two = ugly_set[++j] * 2;
		for (j = i - 1; j >= 0; j--)
			if (ugly_set[j] * 3 <= ugly_set[i - 1])
				break;
		next_and_three = ugly_set[++j] * 3;
		for (j = i - 1; j >= 0; j--)
			if (ugly_set[j] * 5 <= ugly_set[i - 1])
				break;
		next_and_five = ugly_set[++j] * 5;
		ugly_set[i] = \
			next_and_two < next_and_three ? \
			(next_and_two < next_and_five ? next_and_two : next_and_five) : \
			(next_and_three < next_and_five ? next_and_three : next_and_five);
	}
	for (i = 0; i < n; i++)	
		printf("%d ",ugly_set[i]);
	free(ugly_set);
}

//粉刷房子，每个房子有k中颜色，相邻房子颜色不能相同
//还是用动态规划，dp[i]的最小值只需要参考前最小的俩个值即可
int paint_house_ii(int * houses, int house_number, int k)
{
  int i = 0, j = 0;	
  int min1 = ~(1 << 31), color1 = -1, \
  	min2 = ~(1 << 31), color2 = -1; 
  int** dp = (int**)calloc(house_number, sizeof(int*));
  for (i = 0; i < house_number; i++)
    dp[i] = (int*)calloc(k, sizeof(int));
  int result = ~(1 << 31);

  //初始化
  for(i = 0; i < k; i++) 
	dp[0][i] = houses[0 * k + i];
  for(i = 0; i < k; i++) {
	//min2 永远比 min1 大
	if (min2 < min1) {
		int temp = min2;
		min2 = min1;
		min1 = temp;
		temp = color1;
		color1 = color2;
		color2 = temp;
	}
	if (min1 > dp[0][i]) {
		min2 = min1;
		color2 = color1;
		min1 = dp[0][i];
		color1 = i;
	} else if (min2 > dp[0][i]) {
		min2 = dp[0][i];
		color2 = i;
	}	
  }
  for (i = 1; i < house_number; i++) {
	  printf("%d - %d\n", min1 , min2);
	  int cur_min1 = ~(1 << 31), cur_color1 = -1, \
	  	cur_min2 = ~(1 << 31), cur_color2 = -1;
      for (j = 0; j < k; j++) {
		//当前dp[i][j]最小值一定是 前一最小（不同色）加本节点
		//或者前一次小（因为遇到了同色）加本节点
		if(j != color1)
			dp[i][j] = min1 + houses[i * k + j];
		else 
			dp[i][j] = min2 + houses[i * k + j];
		//生成本层的新min
		//min2 永远比 min1 大
		if (cur_min2 < cur_min1) {
			int temp = cur_min2;
			cur_min2 = cur_min1;
			cur_min1 = temp;
			temp = cur_color1;
			cur_color1 = cur_color2;
			cur_color2 = temp;
		}
		if (cur_min1 > dp[i][j]) {
			cur_min2 = cur_min1;
			cur_color2 = cur_color1;
			cur_min1 = dp[i][j];
			cur_color1 = j;
		} else if (cur_min2 > dp[i][j]) {
			cur_min2 = dp[i][j];
			cur_color2 = j;
		}

	  }
	  //更新到下层
	  min1 = cur_min1;
	  min2 = cur_min2;
	  color1 = cur_color1;
	  color2 = cur_color2;
  }
	printf("%d - %d\n", min1 , min2);
  
  /*验证
  for (i = 0; i < house_number; i++) {
      for (j = 0; j < k; j++)
        printf("%d ",dp[i][j]);
      printf("\n");
  }
  */

  //最小值
  result = min1;
  for (i = 0; i < house_number; i++)
    free(dp[i]);
  free(dp);
  return result;
}

//字符串全排列是否可以组成出回文串
bool palindrome_permutation_i(char* str)
{
	int count = 0;
	char* buf = str;
	int hash_to_str[256] = {0};
	while (*buf != '\0') {
		hash_to_str[*buf++]++;
		count++;
	}

	int special_count = 0, i = 0;
	for (i = 0; i < 256; i++) {	
		if (hash_to_str[i] % 2 != 0) {
			special_count++;
			if (special_count >= 2)
				return 0;
		}	
	}
	if (special_count == 0 || \
		(special_count == 1 && count % 2 != 0))
		return 1;
	return 0;	
}

//求出全部字符串回文串的全排列
void help_palindrome_permutation_ii(int* hash_to_map, int* temp, \
	int left, int right, int old_left, int olde_right)
{
	if (left > right) {
		while (old_left <= olde_right)
			printf("%c ",temp[old_left++]);
		printf("\n");
	} else {
		int i = 0;
		for (i = 0; i < 256; i++) {
			if (hash_to_map[i] == 0)
				continue;
			if (hash_to_map[i] == 1)	
				if (left != right)
					continue;
				else {
					temp[left] = i;
					hash_to_map[i]--;
					help_palindrome_permutation_ii(hash_to_map, temp, \
						left + 1, right - 1, old_left, olde_right);	
					hash_to_map[i]++;		
				}
			else {
				temp[left] = i;
				temp[right] = i;
				hash_to_map[i] -= 2;
				help_palindrome_permutation_ii(hash_to_map, temp, \
					left + 1, right - 1, old_left, olde_right);	
				hash_to_map[i] += 2;							
			}	
		}
	}
}

void palindrome_permutation_ii(char* str)
{
	//还是验证是否是有效的回文串
	int count = 0;
	char* buf = str;
	int hash_to_str[256] = {0};
	while (*buf != '\0') {
		hash_to_str[*buf++]++;
		count++;
	}

	int special_count = 0, i = 0;
	for (i = 0; i < 256; i++) {	
		if (hash_to_str[i] % 2 != 0) {
			special_count++;
			if (special_count >= 2)
				return;
		}	
	}
	
	if (special_count == 0 || \
		(special_count == 1 && count % 2 != 0)) {
		//验证成功，现在开始才可以实现回回文串操作了
		//创建一个暂存空间
		int* temp = (int*)calloc(count, sizeof(int));
		help_palindrome_permutation_ii(hash_to_str, temp, 0, count - 1, \
			0, count - 1);
	}
}

//求一个数组中，0-n中缺失的一个数
//等差数列求和与实际和做差为缺失数 s = （首项+末项）*个数/2

//另类顺序字符串
//即有向图，直接优先搜索即可获取优先级

//查找二叉搜索树中，与target相差最小，的k个值
//使用stack，中序遍历bst，是有序的
//将差值最小的k个元素的 <差值，自身值> 插入优先队列
//队列满了k个，且差值为正，且大于堆顶，可以提前结束

//整数转换英文表示
//要有集合 将数据 ，按 千 截取 ，用栈保存即可，对 千 以内的数解码
//进一步拆分出百 十 

//H指数
//1、将其发表的所有SCI论文按被引次数从高到低排序；
//2、从前往后查找排序后的列表，直到某篇论文的序号大于该论文被引次数。
//所得序号减一即为H指数。
//若排序号还可采用二分搜索
int h_index(int* array, int left, int right)
{
	//先排序
	quick_sort(array, left, right);
	int i = left, j = right;
	while (i < j)
		swap(array,i++,j--);

	for (i = left; i <= right; i++) {
		if (i > array[i])
			return i - 1;
	}
}


//粉刷栅栏
//前两个颜色一样，dp[i-2] 的方案数，
//dp[i-2]*1*(k-1)，i 跟他们必须不一样（k-1种选择）
//
//前两个颜色不一样，i-2 占了一种颜色， i-1 占了一种颜色，
//i 还能选择 k-1 种颜色（可以跟 i-2 一样），
//方案数为 dp[i-1]*(k-1)
int paint_fence(int numbers, int k)
{
	if (numbers <= 0)
		return 0;
	else if (numbers == 1)
		return k;
	int* dp = (int*)calloc(numbers, sizeof(int));
	int result = 0, i = 0;

	dp[0] = k;
	dp[1] = k * k;
	for (i = 2; i < numbers; i++) {
		dp[i] = dp[i - 2] * 1 * (k - 1) + \
			dp[i - 1] * (k - 1);
	}

	result = dp[numbers - 1];
	free(dp);
	return result;
}

//寻找名人，有向图中寻找一个点，可以到任何地方去但不能从
//任何地方来，建表保存状态一个个淘汰
//或者根据名人的特性：设定候选人 res 为0，
//原理是先遍历一遍，对于遍历到的人i，
//若候选人 res 认识i，则将候选人 res 设为i，完成一遍遍历后，
//来检测候选人 res 是否真正是名人，如果判断不是名人，
//则返回 -1，如果并没有冲突，返回 res

//求一个正整数最少由几个完全平方的正整数组成
//四平方和定理，任意一个正整数均可表示为4个整数的平方和，
//其实是可以表示为4个以内的平方数之和，
//那么就是说返回结果只有 1,2,3 或4其中的一个
//
//将第一个值初始化为0，其余值都初始化为 INT_MAX, 
//i从0循环到n，j从1循环到 i+j*j <= n 的位置，
//dp[i+j∗j]=min(dp[i+j∗j],dp[i]+1);
int prefect_squares(int data)
{
	//首先我们将数字化简一下，
	//由于一个数如果含有因子4，那么我们可以把4都去掉
	//最后结果就是小于4的那个因子
	//还有一个可以化简的地方就是，
	//如果一个数除以8余7的话，那么肯定是由4个完全平方数组成	
	if (data % 8 == 7)
		return 4;
	while (data % 4 == 0)
		data /= 4;
	int i= 0;
	for (i = 0; i * i <= data; i++) {
		int j = sqrt(data - i * i);
		if (i * i + j * j == data)	
			return !!i + !!j;
	}
	return 3;
}

//摆动排序
// nums[0] <= nums[1] >= nums[2] <= nums[3]....
//当i为奇数时，nums[i] >= nums[i - 1]
//当i为偶数时，nums[i] <= nums[i - 1]
//那么只要对每个数字，根据其奇偶性，跟其对应的条件比较，
//如果不符合就和前面的数交换位置即可









#ifdef __cplusplus
	}
#endif
#endif









