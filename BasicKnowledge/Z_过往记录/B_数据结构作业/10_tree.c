#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

//�Զ���ͷ�ļ�
typedef int elemtype;

typedef struct tree
{
	elemtype data;
	struct tree * left_child;
	struct tree * right_child;
} use_tree;

struct tree* create_tree(elemtype* start, int left, int right);
void print_tree(struct tree* root);
int all_num_about_tree(struct tree* root);
void print_reverse_tree(struct tree* root);
bool is_entirely_tree(struct tree* root);
void insert_node(struct tree* root, elemtype data);
void delete_node(struct tree* root, elemtype data);

//main.c
int main()
{
	
	int test[12] = {1, 3, 4, 5, 6, 7 ,8, 9, 10, 11,12,13};
	struct tree* root = create_tree(test, 1, 11);	
	//print_reverse_tree(root);
	//printf("%d\n", all_num_about_tree(root));
	//printf("%d\n", is_entirely_tree(root));
}

//�Զ���Դ�ļ�

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

void print_reverse_tree(struct tree* root)
{
	//ʹ������ģ��ջ
	struct tree* simulate_stack[100] = {0};
	//������0��ʼ�����Ϊ-1����ջ��
	int index = -1;
	struct tree* current_p = root;
	
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
	
	/*
	
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

int all_num_about_tree(struct tree* root)
{
	if (root == NULL)
		return 0;
	else
		return (1 + all_num_about_tree(root->left_child) \
						+ all_num_about_tree(root->right_child));
}

bool is_entirely_tree(struct tree* root)
{
	bool yesno = 1;
	bool help = 0;
	//ʹ������ģ��ջ
	struct tree* simulate_stack[100] = {0};
	//������0��ʼ�����Ϊ-1����ջ��
	int index = -1;
	struct tree* current_p = root;
	
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
			if (current_p->left_child == NULL && \
				 current_p->right_child != NULL) {
				yesno = 0;				
			} 
			if (help == 1)
				if (current_p->left_child == NULL || \
					 current_p->right_child == NULL)
					yesno = 0;
			if (current_p->left_child == NULL || \
				 current_p->right_child == NULL) 
				help = 1;					 
			//������������Ӧ���赣����current_p���ֿ�ָ���쳣
			//��Ϊ��ջ�е�����һ�������ݽ��
			//����current_p������Һ���һ��Ҫ��Ϊ������ʹ�䵯��								
			current_p = current_p->right_child;		
		}	
	}
	return yesno;
}

void insert_node(struct tree* root, elemtype data)
{
		if (root == NULL) {
			struct tree* buf = \
				(struct tree*)malloc(sizeof(struct tree));
			buf->data = data;
			buf->left_child = NULL;
			buf->right_child = NULL;
		} else if (root->left_child == NULL && \
			data <= root->data)
			insert_node(root->left_child, data);			
		else if (root->right_child == NULL && \
			data >= root->data)
			insert_node(root->left_child, data);		
		else if (data <= root->data)		
			insert_node(root->left_child, data);
		else
			insert_node(root->left_child, data);			
}

static struct tree* search_node(struct tree* root, elemtype data)
{
	if (root == NULL)
		return;
	else if (root->data == data)
		return root;
	else if (root->left_child != NULL && \
		root->left_child->data == data)
		return root->left_child;
	else if (root->right_child != NULL && \
		root->right_child->data == data)
		return root->right_child;	
	else if (data < root->data)
		return search_node(root->left_child, data);
	else
		return search_node(root->right_child, data);
}

static struct tree* search_next_node(struct tree* node)
{
	struct tree* buf = node->right_child;
	while (buf->left_child != NULL)
		buf = buf->left_child;
	return buf;
}

struct tree* delete_node(struct tree* root, elemtype data)
{
	struct tree* node = search_node(root, data);
	struct tree* help = NULL;
		if (node->left_child != NULL && \
			node->left_child->data == data) {
			help = node->left_child;	
		}
		if (node->right_child != NULL && \
			node->right_child->data == data) {
			help = node->right_child;	
		}		
	//���н�һ����㣬������helpʧЧ
	if (root->data == data && \
		root->left_child == NULL && \
		root->right_child == NULL) {
		free(root);
		//����ֵ������Ϊ��������׼��
		return NULL;	
	} else if (help->left_child == NULL && \
		help->right_child == NULL) {
			//Ҷ�ӽ�㣬ֱ��ɾ����������Ҫ������ڵ������
			if (node->left_child->data == data)
				help = node->left_child;	
			if (node->right_child->data == data)
				help = node->right_child;							
			free(help);				
	} else if (help->left_child != NULL && \
		help->right_child != NULL) {
		//����������㣬ɾ������ִ����������Ҹ��汳��
		struct tree* buf = search_next_node(help);
		elemtype temp = help->data;
		help->data = buf->data;
		buf->data = temp;
		return delete_node(root, data);			
	} else {
		//����һ����㣬ֻ��Ҫ���¸��ڵ������ӽ���ϵ
		if (node->left_child == help)
			node->left_child = (help->left_child == NULL ? \
				help->left_child : help->right_child); 
		free(help);
	}
	return root;
}





