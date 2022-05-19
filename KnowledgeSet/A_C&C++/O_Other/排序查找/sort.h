#ifndef __SORT_H_
#define __SORT_H_

#ifdef __cplusplus
	extern "C" {
#endif	

#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>

void swap(int* list, int i, int j)
{//交换数组指定下标的俩个数
	int buf = 0;
	buf = list[i];
	list[i] = list[j];
	list[j] = buf;
}

void bubble_sort(int* list, int left, int right)
{//冒泡O(n^2)
	int i = 0, j = 0;
	for (i = left; i <= right; i++)
		for (j = 0; i + j <= right; ++j)
			if (list[i] > list[i + j])
					swap(list, i, i + j);
}

void selection_sort(int* list, int left, int right)
{//选择o(n^2)	
	int i = 0, j = 0, temp = 0, buf = 0;
	for (i = left; i <= right; i++) {
		temp = list[i];
		for (j = 0; i + j <= right; ++j) {
			if (temp > list[i + j]) {
				buf = temp;
				temp = list[i + j];
				list[i + j] = buf;
			}
		}
		list[i] = temp;
	}	
}

void insertion_sort(int* list, int left, int right)
{//插入
	int n = right + 1;
	int i = left, j = 0, temp;
	for (i = left; i + 1<= right; i++) {
		temp = list[i + 1];	
		for (j = i; j >= left; j--) {
			if (temp < list[j])
				list[j + 1] = list[j];
			else break;
		}
		list[j + 1] = temp;
	}
}

int Hibbard(int x){//希尔增量序列的一种
	return (x + 1) / 2;
}

void shell_sort(int* list,int left,int right)
{//希尔
	int i , j, s, temp;
	int k = right - left;
	while (1) {		   
	k = Hibbard(k);
	for (s = left; s < left + k; s++) {	
	   	for (i = s; i + k <= right; i += k) {
		    temp = list[i + k];	
	    	for (j = i; j >= left; j -= k) {
			    if (temp < list[j])
			    	list[j + k] = list[j];
			    else break;
		    }
	    	list[j + k] = temp;
		}
	}	
	if (k <= 1)
		break;
	}
}

//分治法:
//分时以递归达到范围缩小的现象,
//治是对任意范围都可以治,在回溯时，小治为大治做准备
//将俩个数组合并为一个数组
//分治法关键是对治的设计
void merge(int* list, int*temp, int left, int center, int right)
{
	//左数组在left到center中，此数组已排序好
	//右数组在center+1到right中，此数组已排序好
	int i = left, j = center + 1, k = left;//
	while (1) {
		//取俩个值作比较后写入temp辅助数组中
		if (i > center) {
			temp[k] = list[j];
			j++;
		} else if (j > right) {	
			temp[k] = list[i];
			i++;
		} else if (list[i] < list[j]) {
			temp[k] = list[i];
			i++;
		} else {
			temp[k] = list[j];
			j++;
		}		
		k++;
		if (k > right) 
			break;
	}
	k = 0;
	while (1) {
		list[k] = temp[k];
		k++;
		if (k > right) 
			break;
	}
}

void merge_sort(int* list, int*temp, int left, int right)
{//归并
	int center = (left + right) / 2;		
	if (left >= right) 
		return;
	merge_sort(list, temp, left, center);
	merge_sort(list, temp, center + 1, right);	
	merge(list, temp, left, center, right);	
}

int partition(int* list, int left, int right)
{
	int bufleft = left, bufright = right;
	int middle = 0, index = 0;//middle=list[index];
	int center = (left + right) / 2;
	if (left >= right) 
		return -2;
	else if (right - left <= 10)
		return -1;
	//基准选取	
	if ((list[left] <= list[center] && list[center] <= list[right]) || \
		(list[right] <= list[center] && list[center] <= list[left])) {
		middle = list[center];
		index = center;
	} else if ((list[center] <= list[left] && list[left] <= list[right]) || \
		(list[right] <= list[left] && list[left] <= list[center])) {
		middle = list[left];	
		index = left;
	} else if ((list[center] <= list[right] && list[right] <= list[left]) || \
		(list[left] <= list[right] && list[right] <= list[center])) {
		middle = list[right];
		index = right;
	}
	//数据整理
	while (1) {			
		while (list[bufleft] <= middle)
			bufleft++;		
		while (list[bufright] >= middle)
			bufright--;	
		if (bufleft >= bufright)
			break;
		if (list[bufright] != middle && list[bufleft] != middle)
			swap(list, bufleft, bufright);
	}
	do {	
		if(bufright >= index){
			swap(list, bufright, index);
			index = bufright;
		}
		if(bufleft <= index){
			swap(list, bufleft, index);
			index = bufleft;
		}
	} while (0);
	return index;
}

void quick_sort(int* list, int left, int right)
{//快排
	if (left < right) {
		int n = right - left + 1;	
		int index = partition(list, left, right);
		if (index == -2)
			return;	
		else if (index == -1)
			bubble_sort(list, left, right);
		else {
			quick_sort(list, left, index-1);
			quick_sort(list, index+1, right);			
		}					
	} else
		return;
}

void heap_sort(int* list, int n)
{//堆排序
	int i = 0, i_max = 0;
	int scale = n-1;
	int father, left_child, right_child;
	//起始位置从i=0开始,则，左右孩子为2i+1,2i+2;
	//查找最大非叶子节点的层i_max
	while (1) {	
		if (2 * i + 2 < n) 
			i++;
		else {
			i_max = i;
			break;
		}
	}
	//调整数组为堆：（从下而上调整）	
	for (i = i_max; i>=0; --i){	
		father = i;
		while (1) {		
			left_child = 2 * father + 1;
			right_child = 2 * father + 2;
			if (right_child > scale && left_child > scale)
				break;//此节点不存在左右孩子
			else if (right_child > scale && left_child <= scale) {
				//此节点无右孩子
				if (list[left_child] < list[father])
					break;//父节点最大，无需调整
				else {
					swap(list, left_child, father);
					father = left_child;
				}
			} else if (right_child <= scale) {
				if (list[father] > list[right_child] && \
					list[father] > list[left_child])
					break;//父节点最大，无需调整					
				else if (list[right_child] > list[left_child]) {
					//孩子节点一定有一个大
					swap(list, father, right_child);
					father = right_child;
				} else {
					swap(list, father, left_child);
					father = left_child;
				}
			}
		}		
	}	
	i = n - 1;
	while (1) {
		if (i <= 0) 
			break;
	//交换堆头与堆尾，堆范围缩小1个	
		swap(list, 0, i);
		i--;
		father = 0;
	//重新调整数组为堆：（从上而下调整）
	//调整一次后（如果需要）孩子节点晋升父节点继续调整
	//如果不需要调整，内循环退出，并标志外循环无需继续
		while (1) {		
			left_child = 2 * father + 1;
			right_child = 2 * father + 2;
			if (right_child > i && left_child > i)
				break;//此节点不存在左右孩子
			else if (right_child > i && left_child <= i) {
				//此节点无右孩子
				if (list[left_child] < list[father])
					break;//父节点最大，无需调整
				else {
					swap(list, left_child, father);
					father = left_child;
				}
			} else if (right_child <= i) {
				if (list[father] > list[right_child] && \
				  	list[father] > list[left_child])
					break;//父节点最大，无需调整				
				else if (list[right_child] > list[left_child]) {
					//孩子节点一定有一个大
					swap(list, father, right_child);
					father = right_child;
				} else{
					swap(list, father, left_child);
					father = left_child;
				}
			}
		}		
	}
}

void counting_sort(int* list, int left, int right)
{//计数排序
	int i = 0, j = 0;
	unsigned char*count = NULL;
	int max = list[left], min = list[left];
	for (i = left + 1; i <= right; i++) {
		if (list[i] > max) max = list[i];
		if (list[i] < min) min = list[i];
	}
	count = (unsigned char*)malloc(\
		   sizeof(unsigned char)*(max - min + 1));
	//新数组数标偏移量要加min
	for (i = 0;i < max - min + 1; i++)
		count[i] = 0;
	for (i = left; i <= right; i++)
		count[list[i] - min] ++;
	for (i = 0, j = left; i < max - min + 1; i++) {
		while (1) {
			if (count[i] > 0) {
				list[j] = i + min;
				count[i]--;
				j++;
			} else 
				break;
		}
	}
	free(count);
}

//模拟数组列表
typedef struct bucket
{
	struct bucket * bk_next;
	int bk_number;
}bucket;

//创建链表并初始化
bucket* create_node(bucket bk)
{
	struct bucket*node = (bucket*)malloc(sizeof(bucket));
	node->bk_next = NULL;
	node->bk_number = bk.bk_number;
	return node;
}
//释放列表,未测试
static void free_node(bucket*bk)
{
	if (bk->bk_next == NULL) {
		free(bk);
		bk = NULL;
	}
}
//顺序插入元素
static void insert_node(bucket** bk_p, int n, bucket* bk)
{		
	//创建缓存指针，指向临时更改的数据结构体	
	struct bucket* p_move = NULL;
	p_move = bk_p[n];
	if (p_move == NULL)//如果表头无数据插到表头就行
		bk_p[n] = bk;
	else if (p_move->bk_number > bk->bk_number) {
		//如果一个数小于要插入的数且它是链表最后一个元素
		bk->bk_next = p_move;
		bk_p[n] = bk;
	} else while (1) {
		if (p_move->bk_next == NULL && \
			p_move->bk_number <= bk->bk_number) {
			//如果一个数小于要插入的数且它是链表最后一个元素
			p_move->bk_next = bk;
			break;
		} else if (p_move->bk_next != NULL && \
			p_move->bk_number <= bk->bk_number &&\
			p_move->bk_next->bk_number <= bk->bk_number) {
			//如果一个数以及它后面的数都小于该目标
			//移动至后一个数
			p_move = p_move->bk_next;
		} else {	
			bk->bk_next = p_move->bk_next;
			p_move->bk_next = bk;
			break;
		}	
	}
}
//顺序弹出元素
static int remove_node(bucket**bk_p, int n)
{
	int buf = bk_p[n]->bk_number;
	struct bucket* bkBuf = bk_p[n];
	//此链表只有一个元素了
	if(bk_p[n]->bk_next == NULL)
		bk_p[n] = NULL;
	else
		bk_p[n] = bk_p[n]->bk_next;
	free(bkBuf);
	return buf;
}
//辅助打印二维数组
static void help_printf(bucket** bk_p, int n)
{
	struct bucket* p_bucket;
	int i = 0;
	for (i = 0; i <= n; i++) {
		p_bucket = bk_p[i];
		while (1) {
			if (p_bucket == NULL) 
				break;
			printf("%d ", p_bucket->bk_number);
			p_bucket = p_bucket->bk_next;
		}
	}
	printf("\n");
}

//桶排序一般用于数据范围已知的情况
//不然一般情况下会过于吃内存，或排序有误
void single_bucket_sort(int* list, int left, int right)
{//单次桶排序
	int n = right-left + 1;
	int i, t, max = list[left], min = list[left];
	struct bucket** bk_pointer = NULL;
	struct bucket* bk_buf = NULL;
	for (i = left; i <= right; i++) {
		if (max < list[i]) 
			max = list[i];
		if (min > list[i]) 
			min = list[i];
	}
	//补充： 映射函数一般是 f = array[i] / k; 
	//k^2 = n; n是所有元素个数 
	if (max == min)
		return;
	//k的确定存在些问题
	//构建桶子
	bk_pointer = (bucket**)malloc(sizeof(bucket*) * (n + 1));
	//初始化桶
	for (i = 0; i < n + 1; i++)
		bk_pointer[i] = NULL;
	//这里需要链表的创建与删除，插入，提出	
	for (i = left; i <= right; i++) {
		int i_buf = (int)((list[i] - min) * n / (max - min));
		struct bucket buf;
		struct bucket* p_buf;
		buf.bk_number = list[i];
		buf.bk_next = NULL;
		p_buf=create_node(buf);	
		while (i_buf + 1 < n && bk_pointer[i_buf + 1] != NULL && \
			bk_pointer[i_buf + 1]->bk_number < buf.bk_number)
			i_buf++;
		insert_node(bk_pointer, i_buf, p_buf);
	}
	//help_printf(bk_pointer,n);
	i = 0;
	t = left;
	while (1) {
		if (i > n) 
			break; 
		while (1) {	
			if (bk_pointer[i] != NULL) {	
				list[t] = remove_node(bk_pointer, i);
				t++;
			} else
				break;
		}
		i++;
	}
	free(bk_pointer);
}
//递归桶排序未实现
void bucket_sort(int* list, int left, int right) 
{	
	single_bucket_sort(list, left, right);
}

typedef struct radix_bucket
{
	struct radix_bucket* next;
	int number;	
}radix_bucket;
//辅助打印二维数组
static void help_print_radix(radix_bucket** bk_p, int n)
{
	struct radix_bucket* p_bucket;
	int i = 0;
	for (i = 0; i <= n; i++) {
		p_bucket = bk_p[i];
		while (1) {
			if (p_bucket == NULL) 
				break;
			printf("%d ",p_bucket->number);
			p_bucket = p_bucket->next;
		}
	}
	printf("\n");
}

static radix_bucket* create_radix_node(radix_bucket Node)
{
    struct radix_bucket*node = (radix_bucket*)malloc(sizeof(radix_bucket));
	node->next = NULL;
	node->number = Node.number;
	return node;
}

static void insert_radix_node(radix_bucket**radix, int n, radix_bucket* Node)
{
	struct radix_bucket*pradix = radix[n];
	if (radix[n] == NULL)
 	    radix[n] = Node;
	else {
		while (pradix->next != NULL)
   			pradix = pradix->next;
		pradix->next = Node;
	}	
}

static int remove_radix_node(radix_bucket**radix, int n)
{
 	int buf = 0;
 	struct radix_bucket*pradix = radix[n];
	if (radix[n] == NULL)
		return -1;
	else {
		radix[n] = radix[n]->next;
		buf = pradix->number;
		free(pradix);
	}
	return buf;
}

void radix_sort(int* list, int left, int right, int index)
{	
	int buf_number = 1, buf_number1 = 1; 
	int cindex[13] = {2, 8, 10, 16, 32, 50, 64, 100, 128, 200, 256, 400, 512};
	int i, j, t = 0, length = 1, max = list[left];
	//������ά���� 
	struct radix_bucket**this_radix = \
		(radix_bucket**)malloc(sizeof(radix_bucket*) * cindex[index]);
	for (i = 0; i < cindex[index]; i++)
		this_radix[i] = NULL;
	for (i = left; i <= right; i++)
		if (max < list[i]) 
			max = list[i];
	while ((max /= cindex[index]) != 0)
        length++;
	while (length--) {
		buf_number *= cindex[index];
	   	for(i = left; i <= right; i++) {
			int x = list[i] % buf_number / buf_number1;						
			struct radix_bucket* p_buf_radix;
			struct radix_bucket buf_radix;
			buf_radix.next = NULL;
			buf_radix.number = list[i];
			p_buf_radix = create_radix_node(buf_radix);
			insert_radix_node(this_radix, x, p_buf_radix);
		}	
		for (t = 0, i = left; i <= right; i++) {	
			int x = remove_radix_node(this_radix, t);
			if (t >= cindex[index])
				break;
			if (x == -1) {
				t++;
				i--;
			} else
 				list[i] = x;
		}			
		buf_number1 *= cindex[index];
	}	
	free(this_radix);
}

	//bubble_sort(test,10,29);
	//counting_sort(test,5,29);
	//heap_sort(test,29);
	//insertion_sort(test,5,29);
	//quick_sort(test,5,29);
	//radix_sort(test,7,29,2);
	//selection_sort(test,6,29);
	//shell_sort(test,7,29);
	//single_bucket_sort(test,4,29);

#ifdef __cplusplus
	}
#endif	
#endif
