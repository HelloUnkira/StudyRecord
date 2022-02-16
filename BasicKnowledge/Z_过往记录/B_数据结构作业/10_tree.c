#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

//自定义头文件
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

//自定义源文件

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
	//使用数组模拟栈
	struct tree* simulate_stack[100] = {0};
	//索引从0开始，如果为-1就是栈空
	int index = -1;
	struct tree* current_p = root;
	
	//尝试中序遍历
	//理论上讲：中序遍历是先遍历左子树，再遍历本结点，后遍历右子树	
	while (!(current_p == NULL && index < 0)) {
		//左子树还存在，不能遍历本结点
		while (current_p != NULL) {
			//当前结点入栈
			index++;
			simulate_stack[index] = current_p;
			current_p = current_p->left_child;
		}
		if (index >= 0)
		{
			//出栈本结点
			current_p = simulate_stack[index];
			index--;
			//左子树访问完了，访问本结点
			printf("%d - \n",current_p->data);	
			//访问右子树，应无需担心其current_p出现空指针异常
			//因为堆栈中弹出的一定是数据结点
			//且若current_p如果无右孩子一定要置为空以迫使其弹出
			current_p = current_p->right_child;
		}	
	}
	
	/*
	
	//尝试先序遍历
	//理论上讲：先遍历本节点，后遍历左子树，后遍历右子树
	while (!(current_p == NULL && index < 0)) {
		//左子树还存在，不能遍历本结点
		while (current_p != NULL) {
			//当前结点入栈
			printf("%d - \n",current_p->data);	
			index++;
			simulate_stack[index] = current_p;
			current_p = current_p->left_child;
		}
		if (index >= 0)
		{
			//出栈本结点
			current_p = simulate_stack[index];
			index--;
			//左子树访问完了，访问本结点
			//访问右子树，应无需担心其current_p出现空指针异常
			//因为堆栈中弹出的一定是数据结点
			//且若current_p如果无右孩子一定要置为空以迫使其弹出
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
	//使用数组模拟栈
	struct tree* simulate_stack[100] = {0};
	//索引从0开始，如果为-1就是栈空
	int index = -1;
	struct tree* current_p = root;
	
	//尝试中序遍历
	//理论上讲：中序遍历是先遍历左子树，再遍历本结点，后遍历右子树	
	while (!(current_p == NULL && index < 0)) {
		//左子树还存在，不能遍历本结点
		while (current_p != NULL) {
			//当前结点入栈
			index++;
			simulate_stack[index] = current_p;
			current_p = current_p->left_child;
		}
		if (index >= 0)
		{
			//出栈本结点
			current_p = simulate_stack[index];
			index--;
			//左子树访问完了，访问本结点
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
			//访问右子树，应无需担心其current_p出现空指针异常
			//因为堆栈中弹出的一定是数据结点
			//且若current_p如果无右孩子一定要置为空以迫使其弹出								
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
	//树中仅一个结点，会引起help失效
	if (root->data == data && \
		root->left_child == NULL && \
		root->right_child == NULL) {
		free(root);
		//返回值基本上为这类特例准备
		return NULL;	
	} else if (help->left_child == NULL && \
		help->right_child == NULL) {
			//叶子结点，直接删除，但是需要解除父节点的引用
			if (node->left_child->data == data)
				help = node->left_child;	
			if (node->right_child->data == data)
				help = node->right_child;							
			free(help);				
	} else if (help->left_child != NULL && \
		help->right_child != NULL) {
		//出现俩个结点，删除会出现大量调整，找个垫背的
		struct tree* buf = search_next_node(help);
		elemtype temp = help->data;
		help->data = buf->data;
		buf->data = temp;
		return delete_node(root, data);			
	} else {
		//出现一个结点，只需要更新父节点与孙子结点关系
		if (node->left_child == help)
			node->left_child = (help->left_child == NULL ? \
				help->left_child : help->right_child); 
		free(help);
	}
	return root;
}





