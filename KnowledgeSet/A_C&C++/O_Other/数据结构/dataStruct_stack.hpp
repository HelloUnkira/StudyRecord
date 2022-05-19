#ifndef __DATASTRUCT_STACK_HPP_
#define __DATASTRUCT_STACK_HPP_

#include<iostream>

using namespace std;

template <typename T>
class stack
{
	private:
		T data;
		stack* next;
	public:
	//构造函数及析构函数 
	stack()
	{
		this->data = T(0); 
		this->next = NULL;
	}
	
	stack(T Data, stack* Next)
	{
		this->data = Data; 
		this->next = Next;
	}
		
	~stack()
	{
	}
	
	//创建栈及删除栈 
	stack* createStack()
	{
		stack<T>* Root = new stack;
		Root->data = 0;
		Root->next = NULL;
		return Root;
	}
	
	void destoryStack(stack*Root)
	{
		if (Root == NULL)
			return;
		else {
			if (Root->next == NULL) {
				delete Root;
				Root = NULL;
				return;
			} else {
				while (1) {
					stack<T>* pbuf = Root;
					if(pbuf == NULL)
						return;
					pbuf = Root;
					Root = Root->next;
					delete pbuf;					
				}								
			}			
		}		
	}
	
	//压栈与弹出 
	void popDownNode(stack*Root, stack Node)
	{
		stack<T>* buf = NULL;
		stack<T>* newNode = new stack(Node.data, Node.next);
		if (Root == NULL)
			return;
		else {
			buf = Root->next;
			Root->next = newNode;
			newNode->next = buf;
		}		
	} 
	
	T popUpNode(stack*Root)
	{
		if (Root == NULL)
			return T(0);
		else if (Root->next == NULL)
			return T(0);
		else {
			stack<T>*buf = new stack(T(0), NULL);			
			buf->next = Root->next->next;
			buf->data = Root->next->data;
			delete Root->next;
			Root->next = buf->next;			
			return buf->data;				
		}					
	}
		
};


#endif
