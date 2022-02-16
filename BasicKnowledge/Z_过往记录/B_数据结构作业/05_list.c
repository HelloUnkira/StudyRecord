/* 对应文件的头文件自定义添加 */
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

/* 自定义头文件在此部分*/

typedef int elemtype;

typedef struct node
{
	elemtype data;
	struct node* next;
}lint_list_node;

typedef struct head
{
	struct node* first;
	struct node* last;
	int num;
}link_list_head;

struct head* create_list_head();
void print_list_and_head(struct head* head);
void insert_element_to_list(struct head* head, elemtype buf_data);
void destory_list_head(struct head* head);
bool delete_single_node(struct head* head, elemtype buf_data);
void delete_node(struct head* head, elemtype buf_data);
struct node* search_node(struct head* head, elemtype buf_data);

//以下是本次作业部分，其余借鉴6-3-作业
struct head* merge_list(struct head* list1, struct head* list2);
void change_merge_list(struct head* list);
bool change_list(struct head* list1, struct head* list2);
bool list_have_loop(struct head* list);
struct head* union_map_list(struct head* list1, struct head* list2);

/*main.c部分在此 */
int main()
{
	

	struct head* list = NULL;
	struct head* list1 =	create_list_head();
	struct head* list2 =	create_list_head();
	int aa1[6] = {1,3,5,7,9,11};
	int aa2[3] = {5,6,9};

	int i = 0;	
	while (i < 6) {
		insert_element_to_list(list1, aa1[i]);
		i++;
	}
	i = 0;
	while (i < 3) {
		insert_element_to_list(list2, aa2[i]);
		i++;
	}
	
	print_list_and_head(list1);
	print_list_and_head(list2);	
	
	
	//printf("\n%d\n", (int)change_list(list1, list2));
	//list = merge_list(list1, list2);
	list = union_map_list(list1, list2);
	

	//print_list_and_head(list1);
	//print_list_and_head(list2);
	print_list_and_head(list);
	
	destory_list_head(list1); 
	destory_list_head(list2); 	
	destory_list_head(list); 	
	/*
	int i = 0;
	struct head* list =	create_list_head();
	int aa[10] = {1,3,-5,7,9,-11,13,15,-17,19};
	
	while (i < 10) {
		insert_element_to_list(list, aa[i]);
		i++;
	}	
	print_list_and_head(list);	
	change_merge_list(list);	
	print_list_and_head(list);	
	destory_list_head(list); 
	*/
	
	
	
	
}



/* 自定义源文件部分在此*/
//include"……"
struct head* create_list_head()
{
	struct head* buf = \
		(struct head*)malloc(sizeof(struct head));
	buf->first = NULL;
	buf->last = NULL;
	buf->num = 0;
	return buf;
}

void print_list_and_head(struct head* head)
{
	printf("list have %d members \n", head-> num);
	if (head->first != NULL)
		printf("first: %d and last %d \n", head->first->data, head->last->data);
	struct node* start = head->first;
	while (start != NULL){
			printf("%d - ",start->data);
			start = start->next;
	} 
	printf("\n");
}

void insert_element_to_list(struct head* head, elemtype buf_data)
{
	struct node* buf = \
		(struct node*)malloc(sizeof(struct node));	
	buf->next = NULL;
	buf->data = buf_data;
	
	struct node* start = head->first;
	
	if (head->first == NULL) {
		head->first = buf;
		head->last = buf;
	} else  if ( head->first->next == NULL) {
		if (buf->data <  head->first->data) {
			buf->next =  head->first;
			head->first = buf;
			head->last = buf->next;			
		} else {
			head->first->next = buf;
			head->last = buf;
		}
	} else {
		
		/*保证start中至少有俩个node ，即start和start->next*/
		while (start->next != NULL) {
			/* 比头节点都小 */
			if (start->data > buf->data)
				break;
			/* 满足范围 */
			if (start->data <= buf->data && start->next->data >= buf->data)
				break;
			start = start->next;
		}
		/* 是一开始就不行？ */
		if (start->data > buf->data) {
			buf->next = start;
			head->first = buf;
		} else if (start->next == NULL) {
			/* 找到最后了都没找到 */
			start->next = buf;
			head->last = buf;			
		} else {
			buf->next = start->next;
			start->next = buf;
		}			
					

	}
	head->num++;
}

void destory_list_head(struct head* head) 
{
	struct node* start = head->first;
	struct node* help = NULL;
	
	while(start != NULL) {
		help = start->next;
		free(start);
		start = help;
	}
	free(head);
}

bool delete_single_node(struct head* head, elemtype buf_data)
{
	struct node* start = head->first;
	
	if (head->first == NULL)
		return 0;
	else  if ( head->first->next == NULL) {
		if (buf_data != head->first->data) 
			return 0;
		else {
			head->first = NULL;
			head->last = NULL;
			free(start);
		}
	} else {
		
		/*保证start中至少有俩个node ，即start和start->next*/
		while (start->next != NULL) {
			/* 比头节点都小 */
			if (start->data == buf_data)
				break;
			/* 满足范围 */			
			if (start->data != buf_data && start->next->data == buf_data)
				break;
			start = start->next;
		}
		/* 第一种情况？		*/
		if (start->data == buf_data) {
			head->first = start->next;		
			free(start);
		} else if (start->next == NULL) 
			return 0;
		else {
			struct node* buf = start->next;
			start->next = start->next->next;
			free(buf);
			/* 删除最后一个元素 */
			if (start->next == NULL)
				head->last = start;
	}						
	}
	head->num--;
	return 1;
}

void delete_node(struct head* head, elemtype buf_data)
{
	while(delete_single_node(head, buf_data));
}

struct node* search_node(struct head* head, elemtype buf_data)
{
	struct node* buf = head->first;
	while (buf != NULL) {
		if (buf->data <= buf_data || buf->data == buf_data)
			return buf;
		buf = buf->next;
	}
	return NULL;
}


//以下是本次作业部分，其余借鉴6-3-作业
static struct node* pop_first_node(struct head* list)
{
	struct node* help = list->first;
	/* 无节点*/
	if (help == NULL)
		return NULL;
	else if (help->next == NULL) {
		/* 仅一个节点*/
		list->first = NULL;
		list->last = NULL;
		list->num--;
		return help;
	} else {
		list->first = help->next;
		help->next = NULL;
		list->num--;
		return help;
	}
}

static void insert_last_node(struct head* list, struct node* node)
{
	if (list->last == NULL) {
		list->first = node;
		list->last = node;	
	} else {
		list->last->next = node;
	list->last = node;				
	}
		list->num++;
}

struct head* merge_list(struct head* list1, struct head* list2)
{
	/*
     *真正的归并排序最好是，从头开始一个个结点摘下并插入到新结点
	 *只需要完成，比较list1和list2找到最小的值结点，弹出首结点函数，完成list尾插入
	 *这里就不涉及任何多余操作，高效
	 */

	struct head* list = create_list_head();
	struct head* help;

	while (1) {
			if (list1->first == NULL && list2->first == NULL)
				break;
			else if (list1->first == NULL)
				help = list2;
			else if (list2->first == NULL)
				help = list1;
			else
				help = list1->first->data <= list2->first->data ? list1 : list2;
				
			insert_last_node(list, pop_first_node(help));
	}
	
	return list;		
	/*
	// 这里是利用6-3-作业的方便处
	struct head* list;
	struct node* help1 = list1->first;
	struct node* help2 = list2->first;
	
	while(help1 != NULL) {
		insert_element_to_list(list, help1->data);		
		help1 = help1->next;
	}
	while(help2 != NULL) {
		insert_element_to_list(list, help2->data);		
		help2 = help2->next;
	}
	destory_list_head(list1);
	destory_list_head(list2);
	return  list;
	*/	
}

void change_merge_list(struct head* list)
{
	/* 负数集合*/
	struct head* list1 = create_list_head();
	/* 正数集合*/
	struct head* list2 = create_list_head();
	struct head* help = NULL;
	struct node* node = list->first;
	while (1) {
		node = list->first;
		if (node != NULL)
			help = node->data < 0 ? list1 : list2;
		else
			break;
		insert_last_node(help, pop_first_node(list));
	}
	printf("111\n");
	print_list_and_head(list1);	
	print_list_and_head(list2);	
	
	/* 全部摘下*/
	list->first = list1->first;
	list->last = list1->last;
	
	list1->first = NULL;
	list1->last = NULL;	
	
	if (list->last != NULL) {
		list->last->next = list2->first;		
		list->last = list2->last;	
	}
	else {
		list->first = list2->first;
		list->last = list2->last;			
	}
	
	list2->first = NULL;
	list2->last = NULL;		
	destory_list_head(list1); 
	destory_list_head(list2); 	
	
}

static bool single_change_list(struct node* list1, struct node* list2)
{
	/* 完成从list1开始到list2依次比较 */
	struct node* help1 = list1;
	struct node* help2 = list2;
	while (help1 != NULL && help2 != NULL) {
		if (help1->data == help2->data) {
			help1 = help1->next;
			help2 = help2->next;
			continue;			
		} else
			break;
	}
	/* 比较list2为list1部分序列，只有list2完全比较成功才行，任何别的不正常情况都视为比较失败*/
	if (help2 == NULL)
		return 1;
	return 0;
}

bool change_list(struct head* list1, struct head* list2)
{
	/* 节约时间可以使用动态规划,只是消耗一定空间，否则使用强比较也可以 */
	
	/* 强比较,耗费时间过多 */
	/*
	struct node* help = list1;
	while (help != NULL) {
		// 从node开始完成一次比较 
		if (single_change_list(help, list2))
			return 1;
		help->next;
	}
	return 0;	
	*/

	/* 动态规划 */
	/* 需要做一系列准备工作，其中构建二维数组，长宽分别为list1->num和list2->num */
	bool yes_or_no = 0;
	struct node* help = NULL;
	int i = 0, j = 0;	
	/* 动态分配空间 */
	int **dp = (int **)malloc(sizeof(int*) * (list1->num + 2));
	for (i = 0; i < list1->num + 2; i++) {
		dp[i] = (int *)malloc(sizeof(int) * (list2->num + 2));	
	}
	/* 初始化dp*/
	for (i = 0; i < list1->num + 2; i++)
		for (j = 0; j < list2->num + 2; j++)
			dp[i][j] = 0;
	
	/* 将dp第一行从第二个开始对其覆盖为list1的数据 */
	/* 将dp第一列从第二个开始对其覆盖为list2的数据 */
	for (i = 2, help = list1->first; help != NULL; i++, help = help->next)
		dp[i][0] = help->data;
	for (i = 2, help = list2->first; help != NULL; i++, help = help->next)
		dp[0][i] = help->data;
	
	/* 填dp表即可，方程为下*/
	for (i = 2; i < list1->num + 2; i++) 
		for (j = 2; j < list2->num + 2; j++) 			
			dp[i][j] =  (dp[i][0] ==dp[0][j] ? 1 : 0) + (dp[i-1][j]  > dp[i][j-1] ? dp[i-1][j] : dp[i][j-1]);			

	/*
	//校验对错的
	for (i = 0; i < list1->num + 2; i++) {
		for (j = 0; j < list2->num + 2; j++) {
			printf("%d   ", dp[i][j]);	
		}		
		printf("\n");
	}
	*/
	
	/* 释放空间 */
	for (i = 0; i < list1->num + 2; i++) {
		free(dp[i]);	
	}	
	free(dp);

	return (dp[list1->num + 1][list2->num + 1] == list2->num ? 1 : 0);
}

bool list_have_loop(struct head* list)
{
	struct node* p1 = list->first;
	struct node* p2 = list->first;
	if (p2->next == NULL)
		return 0;
	else
		p2 = p2->next;
	while (p2 != NULL) {
		if (p1 == p2)
			return 1;
		p1 = p1->next;
		if (p2->next == NULL)
			return 0;
		else
			p2 = p2->next->next;
	}
	return 0;
}

struct head* union_map_list(struct head* list1, struct head* list2)
{
	struct head* list = create_list_head();
	struct node* help = NULL;	
	struct node* help1 = list1->first;
	struct node* help2 = list2->first;
	elemtype buf = 0;
	bool flag = 1;
	// 不断移动寻找相等的，小的对应指针往后移动
	while (help1 != NULL && help2 != NULL) {
		if (help1->data == help2->data)
			if (buf != help1->data) {
				/* 非重复插入 */
				flag = 0;			
				buf = help1->data;							
			}
		/* 数据创建并载入中 */
		if (flag == 0) {
			insert_element_to_list(list, buf);
			/* 设置防重插入操作 */
			flag = 1;
		}
		//移动较小数据的指针
		if (help1->data == help2->data){
			help1 = help1->next;
			help2 = help2->next;							
		} else if (help1->data < help2->data)
			help1 = help1->next;
		else
			help2 = help2->next;		
	}		
	return list;
}




