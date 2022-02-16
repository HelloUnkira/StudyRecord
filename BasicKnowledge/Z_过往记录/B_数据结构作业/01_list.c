/* 作业6-1 */
typedef int elem_type;
struct data_node
{
	struct data_node* next;
	elem_type data;	
}; 

/* 往有序链表中插入一个元素 */
/* 关于实参无法同步,所以使用返回值同步 */
struct data_node* insert_node(struct data_node* list, struct data_node* node)
{
	struct data_node* help = list;
	/* 此处与下处，指针间赋值操作类似赋值操作，无法同步到实参*/ 
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
	/* 至此为止，保证原链表至少存在 list 和 list->next俩个元素 */
	/* 保证在list->next合法下，找到所需的node介于此中间 */ 
	while (help->next != NULL) {
		if(help->data <= node->data && help->next->data >= node->data)
			return;
		help = help->next;
	} 
	/* 如果找到最后都没找到 */ 
	if (help->next == NULL)
		help->next = node;
	else {
		/* 找到了 */
		node->next = help->next; 
		help->next = node;
	} 
	return list;
}

/* 无序链表删除一个元素 */ 
void delete_one_node(struct data_node* list, struct data_node* node) 
{
	struct data_node* help_last = NULL;
	struct data_node* help = list;
	while (help != NULL) {
		if(help->data == node->data) {
			help_last->next = help->next;
			free(help);
			/* 通过加return语句选择性的是一次删除还是全部删除 */
		}
		help_last = help;
		help = help->next;	
	}
}

/* 反序一个单链表 */
struct data_node* reverse_list(struct data_node* list)
{
	/* 反序规则，需要一个空间temp作为中转，从链表头开始遍历 */
	/* 每次从旧链表中拿取出链表头，同时将其作为新链表采用头插法 */
	struct data_node* old_list = NULL;	
	struct data_node* old_list = list;
	struct data_node* new_list = NULL;
	
	/* 直到旧链表为空 */ 
	while ((temp = old_list) != NULL) {
		/* 断开旧链表的首元素 */ 
		//temp->next = NULL;
		old_list = old_list->next;
		/* 将temp插入到新链表中 */ 
		temp->next = new_list; 
		new_list = temp;
	} 
	return new_list;
}
















