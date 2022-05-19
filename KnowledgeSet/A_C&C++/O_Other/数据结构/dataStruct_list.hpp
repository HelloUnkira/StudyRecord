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
	//���캯������������ 
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
	
	//���ʺ���
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
	
	//�޸ĺ���
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
	
	//����
	//��������
	//����һ����ά���飬��������ͷ��βָ�� 
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
	
	//ɾ������ 
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
	
	//����ڵ� 
	//�������������������ڷ�Χ��
	//����С�ڴ˷�Χ���������ͷ
	//������ڵĻ������б�β 
	int insertNode(list*List, list*Node, int x)
	{		
		list<T> * plist = List;
		int buf = 0;
				
		if (x <= 0) {//���뵽�ʼ 
			Node->next = plist->next;
			Node->last = plist;
			plist->next->last = Node;
			plist->next = Node;
		} else if (x >= number) {//���뵽����� 
			while (plist->next != NULL)
				plist = plist->next;
			Node->next = plist;
			Node->last = plist->last;
			plist->last->next = Node;
			plist->last = Node;		
		} else {//���뵽��Ӧ�ڵ���� 
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
	
	//ɾ���ڵ� 
	//�����ڷ�Χ���������� 
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
	
	//�������� 
	void traversalList(list*List)
	{
		list<T>*plist = List->next;
		while (plist->next) {
			cout << (*plist).getData() << endl;
			plist = plist->next;			
		}
	 } 	
	//�������ҽڵ�
	bool selectNode(list*List, list*Node){
		list<T>*plist = List->next;
		while (plist->next) {
			if ((*plist).getData() == (*Node).getData())
				return true;
			plist = plist->next;			
		}	
		return false;
	} 
		
	//�������½ڵ� 
	void updateNode(list*List, list*oldNode, list*newNode)
	{
		list<T>*plist = List->next;
		while (plist->next) {
			if ((*plist).getData() == (*oldNode).getData())
				(*plist).putData((*newNode).getData());
			plist = plist->next;			
		}			
	} 
	
	//����������½ڵ� 
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
//�þ�̬��Ա���֣�����ĳ��ȣ���ʼ��Ϊ0 
template <typename T>
	int list<T>::number = 0;

	//�������ջ����
	//(*aHa).insertList(MyList,buf,-1);
	//��ʱ�������ݽ��Ǵ�����ͷ����������β����
	//�������л�ѹջ���� 
		
	//��Ϊ��ջʱ����ѯ��ǰ��һ���ڵ� 
	//��ɾ����ǰ��һ���ڵ�
	//��ɶ�ջ���� 
	
	//��Ϊ����ʱ����ѯ���һ���ڵ�����
	//��ɾ�����һ���ڵ㼴
	//��ɳ�����	
	//������ʱ����ͷ���β����ȫ�ԳƵ� 
	//����ֻ��Ҫһ��ʼʹ�ñ�ͷ��������
	//ʹ�ñ�β�������ݼ���
	//����в������˻�Ϊ��ջ���� 

#endif
