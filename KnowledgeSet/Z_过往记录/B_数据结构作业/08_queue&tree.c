#include<stdio.h>
#include<stdlib.h>

//�Զ���ͷ�ļ�
typedef int elemtype;

typedef struct queue_head
{
	int num;
	struct queue* first;
	struct queue* last;
}help_queue_head;

typedef struct tree
{
	elemtype data;
	struct tree * left_child;
	struct tree * right_child;
} use_tree;

typedef struct queue
{
	struct tree* data;
	struct queue * next;
} help_queue;

void enqueue(struct queue_head* head, struct tree* data);
struct tree* dequeue(struct queue_head* head);
struct tree* create_tree(elemtype* start, int left, int right);
void print_tree(struct tree* root);

int level_order();
void print_reverse_tree(struct tree* root);

//main.c
int main()
{

	//printf("%d=", level_order());
	
	/*
	int test[10] = {1, 3, 4, 5, 6, 7 ,8, 11, 12, 34};
	struct tree* root = create_tree(test, 0, 9);	
	print_reverse_tree(root);
	*/
	
	
}



//�Զ���Դ�ļ�

void enqueue(struct queue_head* head, struct tree* data)
{
	if (head == NULL)
		return;
	struct queue* buf = (struct queue*)malloc(sizeof(struct queue));
	buf->data = data;
	buf->next = NULL;
	
	if (head->first == NULL) {
		head->num = 1;
		head->first = buf;
		head->last = buf;
	} else {
		//β�����ݣ�ͷ������
		struct queue* help = head->last;
		help->next = buf;
		head->last = buf;
		head->num++;
	}	
}

struct tree* dequeue(struct queue_head* head)
{
		if (head == NULL)
			return;
		else if (head->num == 0)
			return;
		else {
		struct queue* help = head->first;
		struct tree* data = help->data;
		head->first = help->next;
		head->num--;
		free(help);
		return data;
		}
}

struct tree* create_tree(elemtype* start, int left, int right)
{
	if (left > right) 
		return NULL;
	else if (left == right){
		struct tree* buf = (struct tree*)malloc(sizeof( struct tree));	 
		buf->data = start[left];
		buf->left_child = NULL;
		buf->right_child = NULL;		
		return buf;
	} else{
		int center = (left + right) / 2;
		struct tree* buf = (struct tree*)malloc(sizeof( struct tree));	 
		buf->data = start[center];
		buf->left_child = create_tree(start, left, center - 1);
		buf->right_child = create_tree(start,center + 1, right);
		return buf;
	}	
}

void print_tree(struct tree* root)
{
	if (root == NULL)
		return;
	else {
		printf("%d -- ", root->data);
		print_tree(root->left_child);
		print_tree(root->right_child);
	}
}

int level_order()
{
	//ֱ��ʹ�����鴴��������
	int width = 0, current = 0;
	int test[10] = {1, 3, 4, 5, 6, 7 ,8, 11, 12, 34};
	struct tree* root = create_tree(test, 0, 9);
	print_tree(root);
	struct queue_head* head = \
		(struct queue_head*)malloc(sizeof(struct queue_head));
	head->num = 0;
	head->first = 0;
	head->last = 0;
	struct tree* help = NULL;	
	printf("----\n");
	enqueue(head, root);
	width = 1;
	current = 1;
	while (1) {
		if (head->num == 0)
			break;
		while (head->num > 0) {
			help = dequeue(head);
			if (help == NULL)
				continue;
				printf("%d - ", help->data);
			if (help->left_child != NULL) {
				enqueue(head, help->left_child);			
				current++;
			}
			if (help->right_child != NULL) {
				enqueue(head,help->right_child);				
				current++;
			}
		width = width < head->num ? head->num : width;		
		}				
	}	
	return width;
}

void print_reverse_tree(struct tree* root)
{
	//ʹ������ģ��ջ
	struct tree* simulate_stack[100] = {0};
	//������0��ʼ�����Ϊ-1����ջ��
	int index = -1;
	struct tree* current_p = root;
	
	/*
	
	//�����������
	//�����Ͻ�������������ȱ������������ٱ�������㣬�����������	
	while (!(current_p == NULL && index < 0)) {
		//�����������ڣ����ܱ��������
		while (current_p != NULL) {
			//��ǰ�����ջ
			index++;
			simulate_stack[index] = current_p;
			current_p = current_p->left_child;
		}
		if (index >= 0)
		{
			//��ջ�����
			current_p = simulate_stack[index];
			index--;
			//�������������ˣ����ʱ����
			printf("%d - \n",current_p->data);	
			//������������Ӧ���赣����current_p���ֿ�ָ���쳣
			//��Ϊ��ջ�е�����һ�������ݽ��
			//����current_p������Һ���һ��Ҫ��Ϊ������ʹ�䵯��
			current_p = current_p->right_child;
		}	
	}
	
	*//*
	
	//�����������
	//�����Ͻ����ȱ������ڵ㣬������������������������
	while (!(current_p == NULL && index < 0)) {
		//�����������ڣ����ܱ��������
		while (current_p != NULL) {
			//��ǰ�����ջ
			printf("%d - \n",current_p->data);	
			index++;
			simulate_stack[index] = current_p;
			current_p = current_p->left_child;
		}
		if (index >= 0)
		{
			//��ջ�����
			current_p = simulate_stack[index];
			index--;
			//�������������ˣ����ʱ����
			//������������Ӧ���赣����current_p���ֿ�ָ���쳣
			//��Ϊ��ջ�е�����һ�������ݽ��
			//����current_p������Һ���һ��Ҫ��Ϊ������ʹ�䵯��
			current_p = current_p->right_child;
		}	
	}		
	
	*/
		
}







