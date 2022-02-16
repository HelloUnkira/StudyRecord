#ifndef __DATASTRUCT_TREE_HPP_
#define __DATASTRUCT_TREE_HPP_

#include<iostream>

using namespace std;
/*������
������
������
������
�������
�󸸽��
���ӽ��
������*/
//ʹ�ö�����Ϊ�� 
template <typename T> 
class tree
{
	private:
		T data;
		//�ڵ�
		tree* itsFather;
	 	tree* leftChild;
	 	tree* rightChild;
		int table;		
	public:
		//���캯������������ 
	tree()
	{
		this->data 		 = T(0);
		this->itsFather  = NULL;
		this->leftChild  = NULL;
		this->rightChild = NULL;
		this->Level		 = 0;
		this->table		 = 0;			
	}
		
	tree(T Data)
	{
		this->data		 = Data;
		this->itsFather  = NULL;
		this->leftChild	 = NULL;
		this->rightChild = NULL;
		this->Level		 = 0;
		this->table		 = 0;		
	}
		
	~tree()
	{
	}
		
		//�����������ظ��ڵ� 
	tree* createTree()
	{
		tree* Root = new tree(T(-1));
		Root->Level = -1;
		return Root;
	} 
	
		//�ݻ������ݻٸ��ڵ� 
	int destoryTree(tree* Root)
	{
	//���Ǹ��ڵ����޺��ӣ�ֱ��ɾ�� 
	if (Root->itsFather == NULL && \
		Root->leftChild == NULL && Root->rightChild == NULL){
			delete Root;
			return 0;	
		} else if (Root->leftChild == NULL && Root->rightChild == NULL)
				return 1;
		else {
			if (Root->leftChild && \
				(1 == destoryTree(Root->leftChild)));
				delete Root->leftChild;
			if (Root->rightChild && \
				(1 == destoryTree(Root->rightChild)));
				delete Root->rightChild;
			return 1;
		}
		delete Root;
	}
	
		//����ڵ�
	void insertNode(tree* Root, tree* Node)
	{		
		//���޽ڵ㣬ֱ�Ӳ������ 
		if(Root->leftChild == NULL)
			Root->leftChild = Node;
		if(Root->rightChild == NULL)
			Root->rightChild = Node;	
		int i = 0;
		while (int(pow(2, ++i)) < Node->table); 
							
		}
				
};



#endif
