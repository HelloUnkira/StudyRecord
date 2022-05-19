/* ��Ӧ�ļ���ͷ�ļ��Զ������ */
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

/* �Զ���ͷ�ļ��ڴ˲���*/

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

/*main.c�����ڴ� */
int main()
{
	int i = 0;
	struct head* list =	create_list_head();
	int aa[10] = {2,54,5,1,45,4,9,6,3,2};
	
	
	print_list_and_head(list);
	
	while (i < 10) {
		insert_element_to_list(list, aa[i++]);
		print_list_and_head(list);
		printf("\n");
	}
	
	delete_node(list, 2);
	print_list_and_head(list);
	
	destory_list_head(list); 
}



/* �Զ���Դ�ļ������ڴ�*/
//include"����"
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
		
		/*��֤start������������node ����start��start->next*/
		while (start->next != NULL) {
			/* ��ͷ�ڵ㶼С */
			if (start->data > buf->data)
				break;
			/* ���㷶Χ */
			if (start->data <= buf->data && start->next->data >= buf->data)
				break;
			start = start->next;
		}
		/* ��һ��ʼ�Ͳ��У� */
		if (start->data > buf->data) {
			buf->next = start;
			head->first = buf;
		} else if (start->next == NULL) {
			/* �ҵ�����˶�û�ҵ� */
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
		
		/*��֤start������������node ����start��start->next*/
		while (start->next != NULL) {
			/* ��ͷ�ڵ㶼С */
			if (start->data == buf_data)
				break;
			/* ���㷶Χ */			
			if (start->data != buf_data && start->next->data == buf_data)
				break;
			start = start->next;
		}
		/* ��һ�������		*/
		if (start->data == buf_data) {
			head->first = start->next;		
			free(start);
		} else if (start->next == NULL) 
			return 0;
		else {
			struct node* buf = start->next;
			start->next = start->next->next;
			free(buf);
			/* ɾ�����һ��Ԫ�� */
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








