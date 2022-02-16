#ifndef __DATASTRUCT_LIST_HPP_
#define __DATASTRUCT_LIST_HPP_

#include<iostream>

using namespace std;

template <typename T>
class list
{
	private:
		static int number;
		T data;
		list* next;
		list* last;	
	public:	
	//构造函数及析构函数 
	list()
	{
		this->data = T(0);
		this->next = NULL;
		this->last = NULL;
	}
	
	list(T N, list* Np, list* Lp)
	{	
		this->data = N;
		this->next = Np;
		this->last = Lp;
	}
	
	~list()
	{
	}	
	
	//访问函数
	T getData()
	{ 
		return this->data;
	} 
	
	list* getNextP()
	{
		return this->next;
	}	
	
	list* getLastP()
	{
		return this->last;
	}
	
	//修改函数
	void putData(T N)
	{
		this->data=N;
	} 
	
	void putNextP(list* P)
	{
		this->next=P;
	}
	
	void putLastP(list* P)
	{
		this->last=P;
	}
	
	//功用
	//创建链表
	//返回一个二维数组，仅包含表头表尾指针 
	list** createList()
	{
		list<T>**alllist = new list<T>*[2];
		list<T>* start   = new list<T>(-1, NULL, NULL);
		list<T>* end     = new list<T>(-1, NULL, NULL);
		(*start).putNextP(end);
		(*end).putLastP(start);
		number = 0;
		alllist[0] = start;
	 	alllist[1] = end;
		return alllist;
	} 
	
	//删除链表 
	void destoryList(list*List, list**AllList)
	{
		list<T>* temp = NULL;		
		while (List != NULL) {
			temp = List;	
			List = List->next;
			delete temp;
		}
		temp = NULL;
		delete [] AllList;
		AllList = NULL;
		List = NULL;
		number = 0;
	}
	
	//插入节点 
	//插入的数据索引如果不在范围内
	//索引小于此范围则插入链表头
	//否则大于的话插入列表尾 
	int insertNode(list*List, list*Node, int x)
	{		
		list<T> * plist = List;
		int buf = 0;
				
		if (x <= 0) {//插入到最开始 
			Node->next = plist->next;
			Node->last = plist;
			plist->next->last = Node;
			plist->next = Node;
		} else if (x >= number) {//插入到最后面 
			while (plist->next != NULL)
				plist = plist->next;
			Node->next = plist;
			Node->last = plist->last;
			plist->last->next = Node;
			plist->last = Node;		
		} else {//插入到对应节点后面 
			while (buf < x && (plist = plist->next) != NULL)
				buf++;
			Node->next = plist->next;
			Node->last = plist;
			plist->next->last = Node;
			plist->next = Node;
		} 		
		number++;		
		return number;
	}		
	
	//删除节点 
	//若不在范围内则不做操作 
	int deleteNode(list*List, int x)
	{
		list<T> * plist = List;
		int buf = 0;
		if (x <= 0)
			return;		
		while (buf < x && plist != NULL) {
			buf++;
			plist = plist->next;
		} if (buf == x && plist != NULL)
			if (plist->next != NULL) {
				plist->next->last = plist->last;
				plist->last->next = plist->next;
				delete plist;	
				plist = NULL;
				number--;
				return number;		
			}		
		return number;
	} 	
	
	//遍历链表 
	void traversalList(list*List)
	{
		list<T>*plist = List->next;
		while (plist->next) {
			cout << (*plist).getData() << endl;
			plist = plist->next;			
		}
	 } 	
	//遍历查找节点
	bool selectNode(list*List, list*Node){
		list<T>*plist = List->next;
		while (plist->next) {
			if ((*plist).getData() == (*Node).getData())
				return true;
			plist = plist->next;			
		}	
		return false;
	} 
		
	//遍历更新节点 
	void updateNode(list*List, list*oldNode, list*newNode)
	{
		list<T>*plist = List->next;
		while (plist->next) {
			if ((*plist).getData() == (*oldNode).getData())
				(*plist).putData((*newNode).getData());
			plist = plist->next;			
		}			
	} 
	
	//定点遍历更新节点 
	void updateKnowNode(list*List, list*Node, int x)
	{
		list<T> * plist = List;
		int buf = 0;
		if (x <= 0)
			return;		
		while (buf <x && plist != NULL) {
			buf++;
			plist = plist->next;
		}
		if (buf == x && plist != NULL)
			if (plist->next != NULL)
				plist->data = Node->data;	
	} 
	
};
//该静态成员保持，链表的长度，初始化为0 
template <typename T>
	int list<T>::number = 0;

	//队列与堆栈改造
	//(*aHa).insertList(MyList,buf,-1);
	//此时所有数据皆是从链表头，或者链表尾插入
	//完成入队列或压栈操作 
		
	//当为堆栈时，查询最前面一个节点 
	//并删除最前面一个节点
	//完成堆栈弹出 
	
	//当为队列时，查询最后一个节点数据
	//并删除最后一个节点即
	//完成出队列	
	//而创建时，表头与表尾是完全对称的 
	//所以只需要一开始使用表头插入数据
	//使用表尾弹出数据即可
	//则队列操作可退化为堆栈操作 

#endif
