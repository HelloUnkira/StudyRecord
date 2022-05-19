#include<stdio.h>
#include<stdlib.h>

struct node
{
	struct node*	next;
	int	data;
};

struct node* create_list()
{
	int d;
	struct node* list = NULL;
	struct node* help = NULL;
	while (1) {
		scanf("%d", &d);
		if (0 == d)
			break;
		struct node* buf = \
			(struct node*)malloc(sizeof(struct node));
		buf->next = NULL;
		buf->data = d;
	
		if (NULL == list) {
			list = buf;			
			help = buf;
		} else {
			help->next = buf;
			help = buf;
		}
	}
	return list;
}

void print_list(struct node* list)
{
	while (NULL != list) {
		printf("%d", list->data);
		list = list->next;
	}
	printf("\n");
}

void destory_list(struct node* list)
{
	while (list != NULL) {
	struct node* buf = list;		
		list = list->next;
		free(buf);
	}
}

/* ����һ�������� */
struct node* reverse_list(struct node* list)
{
	/* ���������Ҫһ���ռ�temp��Ϊ��ת��������ͷ��ʼ���� */
	/* ÿ�δӾ���������ȡ������ͷ��ͬʱ������Ϊ���������ͷ�巨 */
	struct node* temp = NULL;	
	struct node* old_list = list;
	struct node* new_list = NULL;
	
	/* ֱ��������Ϊ�� */ 
	while ((temp = old_list) != NULL) {
		/* �Ͽ����������Ԫ�� */ 
		//temp->next = NULL;
		old_list = old_list->next;
		/* ��temp���뵽�������� */ 
		temp->next = new_list; 
		new_list = temp;
	} 
	return new_list;
}

struct node* test_add_all_number(char* str1, char* str2)
{

	char* end1 =str1;
	char* end2 =str2;
	struct node* list1 = NULL;
	struct node* list2 = NULL;
	struct node* help = NULL;

	struct node* list   = NULL;		
	struct node* buf1 = NULL;
	struct node* buf2 = NULL;	
	int c = 0;
	
	/* ʹ��ͷ�巨���ô�ʱ�����ʡ�� */
	while (*end1 != '\0') 
		end1++;
	end1--;
	while (*end2 != '\0') 
		end2++;
	end2--;	
	
		
	// �����ַ���str1��str2	
	//����str1, str2 ��Ӧ����
	while (str1 <= end1) {
		struct node* buf = \
			(struct node*)malloc(sizeof(struct node));
		buf->next = NULL;
		buf->data = *end1 - '0';
	
		if (NULL == list1) {
			list1 = buf;			
			help = buf;
		} else {
			help->next = buf;
			help = buf;
		}
		end1--;
	}
	
	while (str2 <= end2) {
		struct node* buf = \
			(struct node*)malloc(sizeof(struct node));
		buf->next = NULL;
		buf->data = *end2 - '0';
	
		if (NULL == list2) {
			list2 = buf;			
			help = buf;
		} else {
			help->next = buf;
			help = buf;
		}
		end2--;
	}	

	//������
	buf1 = list1;
	buf2 = list2;
	c = 0;
	while (1) {
		if (buf1 == NULL && buf2 == NULL && c == 0)
			break;
		int num = c + \
			(buf1 != NULL ? buf1->data : 0) + \
			(buf2 != NULL ? buf2->data : 0) ;
		
		c = num / 10;
		num = num % 10;
		
		if (buf1 != NULL)
			buf1 = buf1->next;
		if (buf2 != NULL)
			buf2 = buf2->next;		
		
		//����
		struct node* buf = \
			(struct node*)malloc(sizeof(struct node));
		buf->next = NULL;
		buf->data = num;
	
		if (NULL == list) {
			list = buf;			
			help = buf;
		} else {
			help->next = buf;
			help = buf;
		}	
	}
	
	destory_list(list1);
	destory_list(list2);
	
	/* ����ʹ��ͷ�巨���ô�ʱ�����ʡ�� */
	list = reverse_list(list);
	
	return list;

}

int main()
{
	/*	
	struct node* list =  create_list();	
	print_list(list);
	
	list = reverse_list(list);
	print_list(list);

			
	destory_list(list);	
	*/
	
	struct node* list = test_add_all_number("9999", "77777");
	print_list(list);

	return 0;
}



