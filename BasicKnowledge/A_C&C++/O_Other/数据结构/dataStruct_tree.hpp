#ifndef __DATASTRUCT_TREE_HPP_
#define __DATASTRUCT_TREE_HPP_

#include<iostream>

using namespace std;
/*创建树
判树空
求树根
求树高
求树结点
求父结点
求子结点
插入结点*/
//使用二叉树为例 
template <typename T> 
class tree
{
	private:
		T data;
		//节点
		tree* itsFather;
	 	tree* leftChild;
	 	tree* rightChild;
		int table;		
	public:
		//构造函数与析构函数 
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
		
		//创建树，返回根节点 
	tree* createTree()
	{
		tree* Root = new tree(T(-1));
		Root->Level = -1;
		return Root;
	} 
	
		//摧毁树，摧毁根节点 
	int destoryTree(tree* Root)
	{
	//若是根节点且无孩子，直接删除 
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
	
		//插入节点
	void insertNode(tree* Root, tree* Node)
	{		
		//根无节点，直接插入根中 
		if(Root->leftChild == NULL)
			Root->leftChild = Node;
		if(Root->rightChild == NULL)
			Root->rightChild = Node;	
		int i = 0;
		while (int(pow(2, ++i)) < Node->table); 
							
		}
				
};



#endif
