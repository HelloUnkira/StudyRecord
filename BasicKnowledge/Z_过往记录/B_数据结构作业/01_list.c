/* ��ҵ6-1 */
typedef int elem_type;
struct data_node
{
	struct data_node* next;
	elem_type data;	
}; 

/* �����������в���һ��Ԫ�� */
/* ����ʵ���޷�ͬ��,����ʹ�÷���ֵͬ�� */
struct data_node* insert_node(struct data_node* list, struct data_node* node)
{
	struct data_node* help = list;
	/* �˴����´���ָ��丳ֵ�������Ƹ�ֵ�������޷�ͬ����ʵ��*/ 
	if (list == NULL) {
		list = node;
		return list;	
	}
	if (list->next == NULL) {
		if (list->data <= node->data)
			list->next = node;
		else {
			node->next = list;
			list = node;
		}
		return list;
	}
	/* ����Ϊֹ����֤ԭ�������ٴ��� list �� list->next����Ԫ�� */
	/* ��֤��list->next�Ϸ��£��ҵ������node���ڴ��м� */ 
	while (help->next != NULL) {
		if(help->data <= node->data && help->next->data >= node->data)
			return;
		help = help->next;
	} 
	/* ����ҵ����û�ҵ� */ 
	if (help->next == NULL)
		help->next = node;
	else {
		/* �ҵ��� */
		node->next = help->next; 
		help->next = node;
	} 
	return list;
}

/* ��������ɾ��һ��Ԫ�� */ 
void delete_one_node(struct data_node* list, struct data_node* node) 
{
	struct data_node* help_last = NULL;
	struct data_node* help = list;
	while (help != NULL) {
		if(help->data == node->data) {
			help_last->next = help->next;
			free(help);
			/* ͨ����return���ѡ���Ե���һ��ɾ������ȫ��ɾ�� */
		}
		help_last = help;
		help = help->next;	
	}
}

/* ����һ�������� */
struct data_node* reverse_list(struct data_node* list)
{
	/* ���������Ҫһ���ռ�temp��Ϊ��ת��������ͷ��ʼ���� */
	/* ÿ�δӾ���������ȡ������ͷ��ͬʱ������Ϊ���������ͷ�巨 */
	struct data_node* old_list = NULL;	
	struct data_node* old_list = list;
	struct data_node* new_list = NULL;
	
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
















