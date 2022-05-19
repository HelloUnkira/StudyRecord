#ifndef __DATASTRUCT_SPLAYTREE_HPP_
#define __DATASTRUCT_SPLAYTREE_HPP_

#include<iostream>

//��չ�����������������󶼻���нڵ���չ

using namespace std;

template<typename T, typename V>
class splaytree
{
	private:
		//����������ϵ
		V* data;
		T index;
		splaytree<T,V>* thisFather;
		splaytree<T,V>* leftChild;
		splaytree<T,V>* rightChild;
	public:
		splaytree()
		{
			this->data = NULL;
			this->index = T(-1);
			this->thisFather = NULL;
			this->leftChild = NULL;
			this->rightChild = NULL;
		}
		
		~splaytree()
		{
		}
	
		splaytree<T,V>* createSplayTree()
		{
			//ֱ�����������㶨 �� ^v^
			return NULL;
		}
	
		void deleteSplayTree(splaytree<T,V>* Root)
		{
			if (Root == NULL)
				return;
			else if (Root->leftChild == NULL && \
				Root->rightChild == NULL) 
				delete Root;
			else {
				deleteSplayTree(Root->leftChild);
				deleteSplayTree(Root->rightChild);
				delete Root;
			}	
		}
	
		//��ת���������Ĳ�����Ҳ�����ɳ�
		//Zag
		void singleLeftRotate(splaytree<T,V>* oldRoot, \
								splaytree<T,V>** newRoot)
		{
			if (oldRoot == NULL || oldRoot->rightChild == NULL)
				return;
			else {
				splaytree<T,V>* buf = oldRoot->rightChild;
				oldRoot->rightChild = buf->leftChild;
				if (buf->leftChild != NULL)
					buf->leftChild->thisFather = oldRoot;
				buf->leftChild = oldRoot;
				oldRoot->thisFather = buf;
				*newRoot = buf;
				(*newRoot)->thisFather = NULL;
			}			
		}
		//Zig
		void singleRightRotate(splaytree<T,V>* oldRoot, \
								splaytree<T,V>** newRoot)
		{		
			if (oldRoot == NULL || oldRoot->leftChild == NULL)
				return;
			else {
				splaytree<T,V>* buf = oldRoot->leftChild;	
				oldRoot->leftChild = buf->rightChild;
				if (buf->rightChild != NULL)
					buf->rightChild->thisFather = oldRoot;
				buf->rightChild = oldRoot;
				oldRoot->thisFather = buf;
				*newRoot = buf;
				(*newRoot)->thisFather = NULL;
				return;
			}
		}

		//ZagZag
		void complexZagZag(splaytree<T,V>* oldRoot, \
								splaytree<T,V>** newRoot) 
		{
			//�������ת���������Һ��ӣ��Һ��ӵ��Һ��Ӵ���
			if (oldRoot == NULL || oldRoot->rightChild == NULL || \
				oldRoot->rightChild->rightChild == NULL)
				return;
			else {
				splaytree<T,V>* buf = NULL;
				singleLeftRotate(oldRoot, &buf);
				singleLeftRotate(buf, &buf);
				*newRoot = buf;
			}										
		}

		//ZigZig
		void complexZigZig(splaytree<T,V>* oldRoot, \
								splaytree<T,V>** newRoot)
		{
			//�������ת������������,���ӵ����Ӵ���
			if (oldRoot == NULL || oldRoot->leftChild == NULL || \
				oldRoot->leftChild->leftChild == NULL) 
				return;
			else {
				splaytree<T,V>* buf = NULL;
				singleRightRotate(oldRoot, &buf);
				singleRightRotate(buf, &buf);
				*newRoot = buf;				
			}
		}						
		
		//ZigZag
		void complexZigZag(splaytree<T,V>* oldRoot, \
								splaytree<T,V>** newRoot)
		{
			//���������ת���������Һ��ӣ��Һ��ӵ����Ӵ���
			if (oldRoot == NULL || oldRoot->rightChild == NULL || \
				oldRoot->rightChild->leftChild == NULL) 
				return;
			else {
				splaytree<T,V>* buf = NULL;
				singleRightRotate(oldRoot->rightChild, &buf);
				oldRoot->rightChild = buf;
				buf->thisFather = oldRoot;
				singleLeftRotate(oldRoot, &buf);
				*newRoot = buf;	
			}
		}						
	
		//ZagZig
		void complexZagZig(splaytree<T,V>* oldRoot, \
								splaytree<T,V>** newRoot)
		{
			//���������ת�����������ӣ����ӵ��Һ��Ӵ���
			if (oldRoot == NULL || oldRoot->leftChild == NULL || \
				oldRoot->leftChild->rightChild == NULL) 
				return;
			else {
				splaytree<T,V>* buf = NULL;
				singleLeftRotate(oldRoot->leftChild, &buf);
				oldRoot->leftChild = buf;
				buf->thisFather = oldRoot;
				singleRightRotate(oldRoot, &buf);
				*newRoot = buf;	
			}				
		}
		


		splaytree<T,V>* splayTree(splaytree<T,V>* Node)
		{
			//����,�������
			
			//���ڵ��Ǹ��ڵ�
			if (Node->thisFather == NULL)
				return Node;
			else if (Node->thisFather->thisFather == NULL) {
				//�ýڵ��Ǹ��ڵ�����
				splaytree<T,V>* newRoot = NULL;				
				
				if (Node == Node->thisFather->leftChild)
					singleRightRotate(Node->thisFather, &newRoot);
				else
					singleLeftRotate(Node->thisFather, &newRoot);
				
				return newRoot;
			} else {
				//�������ڵ�ĸ��ڵ�
				bool assert = false;
				splaytree<T,V>* newRoot = NULL;
				splaytree<T,V>* ancestor = \
					Node->thisFather->thisFather->thisFather;
				//�游���ڣ�Ϊ1������Ϊ0���Һ���
				if (ancestor != NULL)
					assert = (Node->thisFather->thisFather == \
						Node->thisFather->thisFather->thisFather->leftChild);
										
				if (Node == Node->thisFather->leftChild)
					if (Node->thisFather == \
						Node->thisFather->thisFather->leftChild)
						complexZigZig(Node->thisFather->thisFather, &newRoot);
					else
						complexZigZag(Node->thisFather->thisFather, &newRoot);
				else if (Node == Node->thisFather->rightChild) 
					if (Node->thisFather == \
						Node->thisFather->thisFather->rightChild)
						complexZagZag(Node->thisFather->thisFather, &newRoot);
					else
						complexZagZig(Node->thisFather->thisFather, &newRoot);	
					
					
				if (ancestor == NULL)
					return newRoot;
				else {
					newRoot->thisFather = ancestor;
					if (assert == false)
						ancestor->rightChild = newRoot;
					else
						ancestor->leftChild = newRoot;	
					return splayTree(newRoot);
				}
			}
		}
		
		
		
		
		splaytree<T,V>* insertNodeToSpalyTree(splaytree<T,V>* Root,
												T Index, V* Data = NULL)
		{
			if (Root == NULL) {
				splaytree<T,V>* buf = new splaytree<T,V>();
				buf->index = Index;
				buf->data = Data;
				return buf;
			} else if (Index <= Root->index) {
				if (Root->leftChild == NULL) {
					splaytree<T,V>* buf = new splaytree<T,V>();
					buf->index = Index;
					buf->data = Data;
					Root->leftChild = buf;
					buf->thisFather = Root;
					//return splayTree(Root->leftChild);
					return Root;
				} else {
					//return insertNodeToSpalyTree(Root->leftChild, Index, Data);
					insertNodeToSpalyTree(Root->leftChild, Index, Data);
					return Root;					
				}
			} else if (Index >= Root->index) {
				if (Root->rightChild == NULL) {
					splaytree<T,V>* buf = new splaytree<T,V>();
					buf->index = Index;
					buf->data = Data;
					Root->rightChild = buf; 
					buf->thisFather = Root;
					//return splayTree(Root->rightChild);
					return Root;
				} else {
					//return insertNodeToSpalyTree(Root->rightChild, Index, Data);	
					insertNodeToSpalyTree(Root->rightChild, Index, Data);
					return Root;					
				}	
			}
		}
		
		splaytree<T,V>* searchNodeToSpalyTree(splaytree<T,V>* Root, T Index)
		{
			if (Root == NULL)
				return NULL;
			else if (Root->index == Index)
				return splayTree(Root);
			else if (Index < Root->index)
				return searchNodeToSpalyTree(Root->leftChild, Index);
			else if (Index > Root->index)
				return searchNodeToSpalyTree(Root->rightChild, Index);
		}
		
		
		
		
		void printSplayTree(splaytree<T,V>* Root, int level = 0)
		{
			if (Root == NULL)
				return;
			else if (Root->leftChild == NULL && \
				Root->rightChild == NULL) {
				cout<<"level:"<<level<<endl;		
				cout<<Root->index<<" ";
			} else {
				cout<<"level:"<<level<<endl;		
				cout<<Root->index<<" ";	
				printSplayTree(Root->leftChild, level + 1);
				printSplayTree(Root->rightChild, level + 1);
			}	
		}
		
};


void testSplayTree()
{
	splaytree<int,int>* aHa = (*aHa).createSplayTree();
	
	for (int i = 7; i > 0; i-=1)
		aHa = (*aHa).insertNodeToSpalyTree(aHa, i);
	
	aHa = (*aHa).searchNodeToSpalyTree(aHa, 1);

	(*aHa).printSplayTree(aHa);
	(*aHa).deleteSplayTree(aHa);
}

#endif