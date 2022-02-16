#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

//�Զ���ͷ�ļ�����
typedef int elemtype;

typedef struct node
{
	elemtype data;
	struct node* next;
	struct node* prev;
}double_list_node;

typedef struct head
{
	int num;
	struct node* first;
	struct node* last;	
}double_list_head;

struct head * create_list();
void print_list(struct head * list);
void insert_node(struct head * list, elemtype data);
void destory_list(struct head* list);
bool delete_single_node(struct head* head, elemtype data);
void delete_node(struct head* head, elemtype buf_data);
struct node* search_node(struct head* head, elemtype buf_data);


//main.c����

int main()
{
	struct head * list = create_list();	
	int aa[10] = {2,54,6,2,56,3,6,3,2,1};
	int i = 0;
	while (i < 10) {
		insert_node(list, aa[i]);
		i++;
	}
	
	delete_node(list, 2);
	
	print_list(list);
	
	destory_list(list);
	return 0;
}

//�Զ���Դ�ļ�����
//�������
//#include<����>

struct head * create_list()
{
	struct head * buf = \
		(struct head * )malloc(sizeof(struct head * ));
	buf->first = NULL;
	buf->last = NULL;
	buf->num = 0;
	return buf;
}

void print_list(struct head * list)
{
	struct node* help = list->first;
	printf("this list have %d members\n", list->num);
	if (list->first != NULL && list->last != NULL)
		printf("first: %d and last: %d\n", list->first->data, list->last->data);
	while (help != NULL) {
		printf("%d - ", help->data);
		help = help->next;
	}
	printf("\n");
	help = list->last;
	while (help != NULL) {
		printf("%d - ", help->data);
		help = help->prev;
	}	
	
}

void insert_node(struct head * list, elemtype data)
{
	struct node* start = list->first;
	struct node* buf = \
		(struct node*)malloc(sizeof(struct node));
	buf->data = data;
	buf->next = NULL;
	buf->prev = NULL;
	
	if (list->first == NULL) {
		//������Ԫ��
		list->first = buf;
		list->last = buf;
	} else  if ( list->first->next == NULL) {
		//�����һ��Ԫ�أ����²���Ԫ��С��ԭͷԪ��
		if (buf->data <  list->first->data) {
			buf->next =  list->first;
			list->first->prev = buf;
			//����first
			list->first = buf;			
		} else {
			buf->prev = list->first;
			list->first->next = buf;
			//����last
			list->last = buf;
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
			buf->next =  list->first;
			list->first->prev = buf;
			//����first
			list->first = buf;	
		} else if (start->next == NULL) {
			buf->prev = start;
			start->next = buf;
			//����last
			list->last = buf;
		} else {
			//�м����
			buf->prev = start;
			buf->next = start->next;
			start->next->prev = buf;
			start->next = buf;
		}								
	}
	list->num++;
}

void destory_list(struct head* list)
{
	//��ɾ������
	struct node* start = list->first;
	struct node* help = start;
	while (start != NULL) {
		help = start;
		start = start->next;
		free(help);
	}
	free(list);
}

bool delete_single_node(struct head* head, elemtype data)
{
	struct node* start = head->first;
	
	if (head->first == NULL)
		return 0;
	else  if ( head->first->next == NULL) {
		if (data != head->first->data) 
			return 0;
		else {
			//��һ��Ԫ�أ�ֱ��ɾ��
			head->first = NULL;
			head->last = NULL;
			free(start);
		}
	} else {
		/*��֤start������������node ����start��start->next*/
		while (start->next != NULL) {
			/* ��ͷ�ڵ㶼С */
			if (start->data == data)
				break;
			/* ���㷶Χ */			
			if (start->data != data && start->next->data == data)
				break;
			start = start->next;
		}
		/* ��һ�������		*/
		if (start->data == data) {
			head->first = start->next;	
			start->next->prev = NULL;
			free(start);
		} else if (start->next == NULL) 
			return 0;
		else {
			struct node* buf = start->next;
			start->next = buf->next;
			if (buf->next != NULL)
				buf->next->prev = start;			
			free(buf);
			/* ɾ�����һ��Ԫ�� */
			if (start->next == NULL) {
				head->last = start;								
			}
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



