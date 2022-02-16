#ifndef __DATASTRUCT_BSTARTREE_HPP_
#define __DATASTRUCT_BSTARTREE_HPP_

#include<iostream>

//B*��
//���ݶ��� criterion = 2*keyLength/3;������ȡ3�ͻ�������
//��Ϊȡ3������ʱ criterion >= keyLength,

using namespace std;

template<typename T, typename V>
class bstartree;

template<typename T, typename V>
class list
{
	private:
		T index;
		list<T,V>* prev;
		list<T,V>* next;
		bstartree<T,V>* nextLevel;
		V* data;
		
	public:
		list()
		{
			this->index = T(-1);
			this->data = NULL;
			this->prev = NULL;
			this->next = NULL;
			this->nextLevel = NULL;
		}

		~list()
		{
		}

		void setNextLevel(list<T,V>* Node, bstartree<T,V>* Next)
		{
			Node->nextLevel = Next;
		}
		
		T getIndex(list<T,V>* Node)
		{
			if (Node != NULL)
				return Node->index;
			else
				return T(-1);
		}
		
		list<T,V>* getNodeNext(list<T,V>* Node)
		{
			if (Node != NULL)
				return Node->next;
			else
				return NULL;
		}

		list<T,V>* getNodePrev(list<T,V>* Node)
		{
			if (Node != NULL)	
				return Node->prev;
			else
				return NULL;		
		}		
		
		bstartree<T,V>* getNextLevel(list<T,V>* Node)
		{
			if (Node != NULL)				
				return Node->nextLevel;
			else
				return NULL;			
		}
		
		list<T,V>* createNode(T Index, list<T,V>* Prev = NULL, list<T,V>* Next = NULL, V* Data = NULL)
		{
			list<T,V>* buf = new list();		
			buf->index = Index;
			buf->prev = Prev;
			buf->next = Next;
			buf->data = Data;
			return buf;
		}
		
		bool deleteNode(T Index, list<T,V>** Start, list<T,V>** End)
		{
			list<T,V>* buf = *Start;
			if (Start == NULL || End == NULL || *Start == NULL || *End == NULL)
				return false;
			else if (*Start == *End && Index == (*Start)->index) {
				buf = *Start;
				*Start = NULL;
				*End = NULL;
				delete buf;	
				return true;
			} else if (Index == (*Start)->index) {
				buf = *Start;
				*Start = (*Start)->next;
				(*Start)->prev = NULL;
				delete buf;
				return true;
			} else if (Index == (*End)->index) {
				buf = *End;
				*End = (*End)->prev;
				(*End)->next = NULL;
				delete buf;
				return true;
			} else 
				while (buf != *End) {
					//�������Σ�һ���ܴ�Start �� End
					if (buf->index == Index) {
							buf->prev->next = buf->next;
							buf->next->prev = buf->prev;
						delete buf;
						return true;
					}	
					buf = buf->next;
				}
			return false;
		}
			
		void addNodeToList(list<T,V>* Node, list<T,V>** Start, list<T,V>** End)
		{
			if (Start == NULL || End == NULL || Node == NULL)
				return;
			else if (*Start == NULL && *End == NULL) {
				*Start = Node;
				*End = Node;
			} else if (*Start != NULL && *End != NULL) {
				if (Node->index <= (*Start)->index) {
					Node->next = *Start;
					(*Start)->prev = Node;
					*Start = Node;	
				} else if (Node->index >= (*End)->index) {
					Node->prev = *End;
					(*End)->next = Node;
					*End = Node;
				} else {
					list<T,V>* buf = *Start;
					if (buf->index > Node->index) {
							Node->next = buf;
							Node->prev = buf->prev;
							buf->prev->next = Node;
							buf->prev = Node;
					}
					while (buf != *End) {
						buf = buf->next;						
						if (buf->index > Node->index) {
							Node->next = buf;
							Node->prev = buf->prev;
							buf->prev->next = Node;
							buf->prev = Node;
							break;
						}
					}						
				}
			}
		}
		
		void deleteList(list<T,V>** Start, list<T,V>** End)
		{
			list<T,V>* buf = *Start;
			list<T,V>* temp = buf;
			*Start = NULL;
			*End = NULL;
			while (buf != NULL) {
				temp = buf;
				buf = buf->next;
				delete temp;
			}
		}
		
		void breakList(T index, list<T,V>** Start1, list<T,V>** End1, \
						list<T,V>** Start2, list<T,V>** End2, bool isReserve = false)
		{
			if (Start1 == NULL || End1 == NULL || *Start1 == NULL || *End1 == NULL)
				return;
			else {				
				list<T,V>* help = *Start1;
				while (--index > 0 && help != NULL)
					help = help->next;
				if (help == NULL || help->next == NULL) {
					*Start2 = NULL;
					*End2 = NULL;
				} else {
					*End2 = *End1;
					*End1 = help;
					*Start2 = help->next;
					(*End1)->next = NULL;
					(*Start2)->prev = NULL;
				}
				if (isReserve == true) {
					//�Ͽ��󽻻���������
					help = *Start1;
					*Start1 = *Start2;
					*Start2 = help;
					help = *End1;
					*End1 = *End2;
					*End2 = help;					
				}
			}
		}
		
		void getLastNode(list<T,V>** Start1, list<T,V>** End1, \
						list<T,V>** Start2, list<T,V>** End2)
		{
			if (Start1 == NULL || End1 == NULL || *Start1 == NULL || *End1 == NULL)
				return;
			else {	
				*Start2 = *End1;			
				*End2 = *End1;
				*End1 = (*End1)->prev;
				(*End1)->next = NULL;
				(*Start2)->prev = NULL;
			}			
		}
		
		void mergeList(list<T,V>** Start1, list<T,V>** End1, \
						list<T,V>** Start2, list<T,V>** End2)
		{
			if (Start1 == NULL || End1 == NULL || *Start1 == NULL || *End1 == NULL
				||Start2 == NULL || End2 == NULL || *Start2 == NULL || *End2 == NULL)
				return;			
			else {
				(*End1)->next = *Start2;
				(*Start2)->prev = *End1;
				*End1 = *End2;
				*Start2 = NULL;
				*End2 = NULL;
			}	
		}

		void changeNewFather(list<T,V>** Start, list<T,V>** End, bstartree<T,V>* newFather)
		{
			if (Start == NULL || End == NULL || \
				*Start == NULL || *End == NULL || newFather == NULL)
				return;
			bstartree<T,V>* help;
			list<T,V>* buf = *Start;
			(*help).setThisFather(buf->nextLevel, newFather);
			while (buf != *End) {
				buf = buf->next;
				(*help).setThisFather(buf->nextLevel, newFather);
			}
		}
		
		void printList(list<T,V>** Start, list<T,V>** End)
		{
			if (Start == NULL || End == NULL || *Start == NULL || *End == NULL)
				return;		
			list<T,V>* buf = *Start;
			cout<<buf->index<<" ";	
			while (buf != *End) {
				buf = buf->next;				
				cout<<buf->index<<" ";				
			}		
		}
	
};


template<typename T, typename V>
class bstartree
{
	private:
		//����B����ά�ȳ�
		static int keyLength;
		static int criterion;
		//������,�ӽڵ�ָ�뼯,��һ�ֵ�	
		list<T,V>* childStart;
		list<T,V>* childEnd;
		bool isLastLevel;
		bstartree<T,V>* thisFather;
		bstartree<T,V>* leftBrother;
		bstartree<T,V>* rightBrother;
		
		int currentNum; //���Ϊlength��
	public:
		bstartree()
		{
			this->currentNum = 0;	
			this->thisFather = NULL;
			this->leftBrother = NULL;
			this->rightBrother = NULL;
			this->childStart = NULL;
			this->childEnd = NULL;
			this->isLastLevel = false;
		}
	
		~bstartree()
		{
		}
	
		void setThisFather(bstartree<T,V>* Node,bstartree<T,V>* newFather)
		{
			if (Node != NULL)
				Node->thisFather = newFather;
		}
	
		bstartree<T,V>* creatBStarTree(int KeyLength)
		{
			if (KeyLength <= 2)
				return NULL;
			else {
				keyLength = KeyLength;
				criterion = keyLength / 3 * 2 + 1;
				bstartree<T,V>* buf = new bstartree<T,V>();
				buf->isLastLevel = true;
				return buf;
			}
		}
	
		void deleteBStarTree(bstartree<T,V>* Root)
		{
			if (Root == NULL)
				return;
			else if (Root->isLastLevel == true) {
				list<T,V>* help;
				(*help).deleteList(&Root->childStart, &Root->childEnd);
				delete Root;				
			} else{
				list<T,V>* help = Root->childStart;
				deleteBStarTree((*help).getNextLevel(help));
				while (help != Root->childEnd) {
					help = (*help).getNodeNext(help);					
					if ((*help).getNextLevel(help) != NULL)
						deleteBStarTree((*help).getNextLevel(help));
				} 
				(*help).deleteList(&Root->childStart, &Root->childEnd);
				delete Root;
			}		
		}


		
		bool updateIndexFromListSet(bstartree<T,V>* Root, T oldIndex, T newIndex, \
									bstartree<T,V>* Child)
		{
			if (Root == NULL)
				return false;
			list<T,V>* Node = (*Node).createNode(newIndex);
			(*Node).setNextLevel(Node, Child);
			if(true == (*Node).deleteNode(oldIndex, &Root->childStart, &Root->childEnd)) {
				(*Node).addNodeToList(Node, &Root->childStart, &Root->childEnd);
				return true;
			} else
				return false;
		}
		
		void levelUpdateIndexFromListSet(bstartree<T,V>* Root, T oldIndex, \
										T newIndex, bstartree<T,V>* Child)
		{
			if (Root == NULL)
				return;
			else {
				if(true == updateIndexFromListSet(Root, oldIndex, newIndex, Child))
					levelUpdateIndexFromListSet(Root->thisFather, oldIndex, newIndex, Root);
			}
		}
		
		void insertIndexToListSet(bstartree<T,V>* Root, T index, bstartree<T,V>* Child)
		{
			list<T,V>* Node = (*Node).createNode(index);
			T temp = T(-1);
			if(Root->childStart != NULL) 
				temp = (*Node).getIndex(Root->childStart);
			(*Node).setNextLevel(Node, Child);
			(*Node).addNodeToList(Node, &Root->childStart, &Root->childEnd);
			Root->currentNum++;		
			//���������ݲ�����������ͷ������ô�丸�ڵ����Ҫ���£�����еĻ�
			if (temp != T(-1) && Node == Root->childStart && temp != (*Node).getIndex(Node)) \
				levelUpdateIndexFromListSet(Root->thisFather, temp, \
				(*Node).getIndex(Node), Root);			
		}
			
		bstartree<T,V>* adjustInsertIndexSet(bstartree<T,V>* Root) 
		{		
			if (Root->currentNum < keyLength) {
				bstartree<T,V>* buf = Root;
				while(buf->thisFather != NULL)
					buf = buf->thisFather;
				return buf;
			}
			//���ˣ�Ҫ������	
			list<T,V>* help;
			//�ȿ�����û�и��ڵ�,��Ϊ���ڵ�һ�������
			if (Root->thisFather == NULL) {
				Root->thisFather = new bstartree<T,V>();						
				//�����ڵ�Ŀ������븸�ڵ�
				insertIndexToListSet(Root->thisFather, \
					(*help).getIndex(Root->childStart), Root);											
			}																		
			//Ҷ�ӽڵ���ѣ����������Ҷ�ӽڵ㣬���ߺ��
			//��Ҷ�ӽڵ���ѣ���������Ƿ�Ҷ�ӽڵ㣬���к��
			if (Root->rightBrother == NULL) {
				Root->rightBrother = new bstartree<T,V>();
				Root->rightBrother->leftBrother = Root;
				Root->rightBrother->thisFather = Root->thisFather;						
				Root->rightBrother->isLastLevel = Root->isLastLevel;	
				//����Ǩ�ƣ������ڵ㲿������Ǩ�Ƶ��ֵ�ȥ
				(*help).breakList(criterion, &Root->childStart, \
					&Root->childEnd, &Root->rightBrother->childStart, \
					&Root->rightBrother->childEnd);
				Root->currentNum -= (keyLength - criterion);
				Root->rightBrother->currentNum += (keyLength - criterion);				
				//��Ҷ�ڵ���ѻ��Ƿ�Ҷ�ӽڵ����
				//�޸ĺ��ӵĸ���Ϊ�¸���
				if (Root->isLastLevel == false)						
					(*help).changeNewFather(&Root->rightBrother->childStart, \
						&Root->rightBrother->childEnd, Root->rightBrother);								
				//���ֵܽڵ���뵽���ڵ�
				insertIndexToListSet(Root->rightBrother->thisFather, \
					(*help).getIndex(Root->rightBrother->childStart), \
					Root->rightBrother);
				return adjustInsertIndexSet(Root->rightBrother->thisFather);							
			} else if (Root->rightBrother->currentNum > criterion) {
				//�Լ����ֵܶ����ز���������һ�����ֵܣ��е������ԭ���ֵ�
				bstartree<T,V>* buf = new bstartree<T,V>();
				buf->thisFather = Root->rightBrother->thisFather;
				buf->isLastLevel = Root->rightBrother->isLastLevel;
				//����Ǩ�ƣ������ڵ㲿������Ǩ�Ƶ����ֵ���ȥ
				(*help).breakList(criterion, &Root->childStart, \
					&Root->childEnd, &buf->childStart, &buf->childEnd);
				Root->currentNum -= (keyLength - criterion);
				buf->currentNum += (keyLength - criterion);					
				//ҲΪԭ�ֵֵܷ�һ��������Ǩ��
				list<T,V>* tempStart = NULL,*tempEnd = NULL;
				T temp = (*help).getIndex(Root->rightBrother->childStart);
				(*help).breakList(keyLength - criterion, \
					&Root->rightBrother->childStart, \
					&Root->rightBrother->childEnd, &tempStart, &tempEnd, true);						
				(*help).mergeList(&buf->childStart, &buf->childEnd, \
					&tempStart, &tempEnd);
				Root->rightBrother->currentNum -= criterion;
				buf->currentNum += criterion;							
				//��Ҷ�ڵ���ѻ��Ƿ�Ҷ�ӽڵ����
				if (Root->isLastLevel == true)
					(*help).changeNewFather(&buf->childStart, &buf->childEnd, buf);							
				//���¹�ϵ
				buf->leftBrother = Root;
				buf->rightBrother = Root->rightBrother;
				Root->rightBrother->leftBrother = buf;
				Root->rightBrother = buf;				
				//����ԭ�ֵܵĸ��ף����ֵ���ԭ�ֵ��Ҹ���
				levelUpdateIndexFromListSet(buf->rightBrother->thisFather, \
					temp, (*help).getIndex(buf->rightBrother->childStart), \
					buf->rightBrother);
				insertIndexToListSet(buf->thisFather, \
					(*help).getIndex(buf->childStart), buf);	
				return adjustInsertIndexSet(buf->rightBrother->thisFather);
			} else {
				//���ֵֵܷ���
				list<T,V>* tempStart = NULL,*tempEnd = NULL;
				T temp = (*help).getIndex(Root->rightBrother->childStart);
				(*help).breakList(criterion, \
					&Root->childStart, &Root->childEnd, &tempStart, &tempEnd);
				(*help).mergeList(&tempStart, &tempEnd, \
					&Root->rightBrother->childStart, \
					&Root->rightBrother->childEnd);
				Root->rightBrother->childStart = tempStart;
				Root->rightBrother->childEnd = tempEnd;
				Root->currentNum -= (keyLength - criterion);
				Root->rightBrother->currentNum += (keyLength - criterion);				
				//��Ҷ�ڵ���ѻ��Ƿ�Ҷ�ӽڵ����
				//�޸ĺ��ӵĸ���Ϊ�¸���
				if (Root->isLastLevel == true)
					(*help).changeNewFather(&Root->rightBrother->childStart, \
						&Root->rightBrother->childEnd, Root->rightBrother);				
				//�ֵܵĸ���Ҫ����һ��
				levelUpdateIndexFromListSet(Root->rightBrother->thisFather, \
					temp, (*help).getIndex(Root->rightBrother->childStart), \
					Root->rightBrother);						
				return adjustInsertIndexSet(Root->rightBrother);
			}			
		}
		
		bstartree<T,V>* insertIndex(bstartree<T,V>* Root, T index)
		{
			if (Root == NULL)
				return NULL;
			else if (Root->isLastLevel == false) {
				list<T,V>* buf = Root->childStart;
				if (index < (*buf).getIndex(Root->childStart)) 
					return insertIndex((*buf).getNextLevel(Root->childStart), index);
				else if (index > (*buf).getIndex(Root->childEnd))
					return insertIndex((*buf).getNextLevel(Root->childEnd), index);
				else do {	
					list<T,V>* bufNext = (*buf).getNodeNext(buf);
					if (index >= (*buf).getIndex(buf) && \
						index <= (*buf).getIndex(bufNext))
						return insertIndex((*buf).getNextLevel(buf), index);					
				} while (buf != Root->childEnd);
			} else {
				//���һ�㣬Ҷ�ӽڵ㣬ֱ�Ӳ�������
				insertIndexToListSet(Root, index, NULL);
				return adjustInsertIndexSet(Root);
			}
		}
		

		
		bool levelremoveIndexToListSet(bstartree<T,V>* Root, T index)
		{
			list<T,V>* help;
			T temp = (*help).getIndex(Root->childStart);
			if (true == (*help).deleteNode(index, &Root->childStart, &Root->childEnd)) {
				Root->currentNum--;
				if (temp == index && Root->thisFather != NULL)
					//������϶��Ƿ�Ҷ�ӽڵ㣬��ôԭ��¼ȫ����Ҫ���£�
					//��Ϊ����¼һ�㲻���ǵ���������Ϊ��Ƶ�ԭ��
					levelUpdateIndexFromListSet(Root->thisFather, temp, \
						(*help).getIndex(Root->childStart), Root);
				return true;
			}
			return false;			
		}
		
		bool removeIndexToListSet(bstartree<T,V>* Root, T index)
		{
			list<T,V>* help;
			T temp = (*help).getIndex(Root->childStart);
			if (true == (*help).deleteNode(index, &Root->childStart, &Root->childEnd)) {
				Root->currentNum--;		
				if (temp == index && Root->thisFather != NULL)
					//������϶��Ƿ�Ҷ�ӽڵ㣬��ôԭ��¼ȫ����Ҫ���£�
					//��Ϊ����¼һ�㲻���ǵ���������Ϊ��Ƶ�ԭ��
					levelUpdateIndexFromListSet(Root->thisFather, temp, \
						(*help).getIndex(Root->childStart), Root);				
				return true;
			}
			return false;
		}

		bstartree<T,V>* adjustRemoveInsertIndexSet(bstartree<T,V>* Root)
		{
			//������ϲ����ֵܼ��ɣ�����ʵ�����ֵ��޷Ƕ��һ��ָ�룬���뷭һ�����ɣ�
			if (Root->currentNum >= criterion) {
				bstartree<T,V>* buf = Root;
				while(buf->thisFather != NULL)
					buf = buf->thisFather;
				return buf;	
			} else if (Root->leftBrother == NULL && Root->rightBrother == NULL) {
				//����ȷ�����ǣ��������ڵ����ֹ���״̬				
				if (Root->isLastLevel == false && Root->currentNum == 1) {
					list<T,V>* help;
					bstartree<T,V>* buf = (*help).getNextLevel(Root->childStart);
					buf->thisFather = NULL;
					(*help).deleteList(&Root->childStart, &Root->childEnd);
					delete Root;
					return buf;
				} else
					return Root;
			}
					
			if (Root->rightBrother != NULL)
				//�Լ��������ˣ������ֵܽ�һ��,�ȿ������ֵ�
				if (Root->currentNum + Root->rightBrother->currentNum >= keyLength) {cout<<"*1"<<endl;
					//����ֵܵ�һ���ڵ�Ϳ����ˣ�����Ҫ�����ֵ�
					list<T,V>* tempStart = NULL, * tempEnd = NULL;
					list<T,V>* help;
					T temp = (*help).getIndex(Root->rightBrother->childStart);
					(*help).breakList(1,&Root->rightBrother->childStart, \
						&Root->rightBrother->childEnd, &tempStart, &tempEnd, true);
					if (Root->isLastLevel == false)
						(*help).changeNewFather(&tempStart, &tempEnd, Root);						
					(*help).mergeList(&Root->childStart, &Root->childEnd, &tempStart, &tempEnd);
					Root->rightBrother->currentNum--;
					Root->currentNum++;
					//����һ���ֵ�
					levelUpdateIndexFromListSet(Root->rightBrother->thisFather, \
						temp, (*help).getIndex(Root->rightBrother->childStart), Root->rightBrother);
					return adjustRemoveInsertIndexSet(Root);
				} else if (Root->currentNum + Root->rightBrother->currentNum < keyLength) {cout<<"*2"<<endl;
					//�ֵܽڵ�û�и�ԣ�������ϲ�����ʱ���ǽ�ɾ�������ᵽ�ϲ�ȥ��
					list<T,V>* help;
					bstartree<T,V>* ptemp = Root->rightBrother;
					T temp = (*help).getIndex(Root->rightBrother->childStart);
					if (Root->isLastLevel == false)
						(*help).changeNewFather(&Root->rightBrother->childStart, \
							&Root->rightBrother->childEnd, Root);
					(*help).mergeList(&Root->childStart, &Root->childEnd, \
						&Root->rightBrother->childStart, &Root->rightBrother->childEnd);
					Root->currentNum += Root->rightBrother->currentNum;
					//�ϳ���ϵ
					bstartree<T,V>* rightBrotherFather = Root->rightBrother->thisFather;					
					Root->rightBrother = Root->rightBrother->rightBrother;
					if (Root->rightBrother != NULL)
						Root->rightBrother->leftBrother = Root;	
					delete ptemp;
					//�򸸲�ɾ���ڵ�
					levelremoveIndexToListSet(rightBrotherFather, temp);
					return adjustRemoveInsertIndexSet(rightBrotherFather);	
															
				}	
				
							
			if (Root->leftBrother != NULL)
				//�Լ��������ˣ������ֵܽ�һ��,�ٿ������ֵ�
				if (Root->currentNum + Root->leftBrother->currentNum >= keyLength) {cout<<"*3"<<endl;
					//����ֵܵ�һ���ڵ�Ϳ����ˣ�����Ҫ�����ֵ�
					list<T,V>* tempStart = NULL, * tempEnd = NULL;
					list<T,V>* help;
					T temp = (*help).getIndex(Root->childStart);
					(*help).getLastNode(&Root->leftBrother->childStart, \
						&Root->leftBrother->childEnd, &tempStart, &tempEnd);
					if (Root->isLastLevel == false)
						(*help).changeNewFather(&tempStart, &tempEnd, Root);						
					(*help).mergeList(&tempStart, &tempEnd, &Root->childStart, &Root->childEnd);
					Root->childEnd = tempStart;
					Root->childEnd = tempEnd;
					Root->leftBrother->currentNum--;
					Root->currentNum++;
					//����һ���Լ�
					levelUpdateIndexFromListSet(Root->thisFather, \
						temp, (*help).getIndex(Root->childStart), Root);
					return adjustRemoveInsertIndexSet(Root);
				} else if (Root->currentNum + Root->leftBrother->currentNum < keyLength) {cout<<"*4"<<endl;
					//�ֵܽڵ�û�и�ԣ�������ϲ�����ʱ���ǽ�ɾ�������ᵽ�ϲ�ȥ��
					//ע�⣬�����ǽ����Լ��ĺ��Ӹ���ڵ㣬������ɾ����ڵ㣬��û�б�������
					list<T,V>* help;
					T temp = (*help).getIndex(Root->childStart);
					if (Root->isLastLevel == false)
						(*help).changeNewFather(&Root->childStart, \
							&Root->childEnd, Root->leftBrother);					
					(*help).mergeList(&Root->leftBrother->childStart, \
						&Root->leftBrother->childEnd, &Root->childStart, &Root->childEnd);
					Root->leftBrother->currentNum += Root->currentNum;
					//�ϳ���ϵ
					Root->leftBrother->rightBrother = Root->rightBrother;					
					if (Root->rightBrother != NULL)
						Root->rightBrother->leftBrother = Root->leftBrother;					
					bstartree<T,V>* rootThisFather = Root->thisFather;						
					delete Root;
					//�򸸲�ɾ���ڵ�
					levelremoveIndexToListSet(rootThisFather, temp);
					return adjustRemoveInsertIndexSet(rootThisFather);
				}
			
		}
		
		bstartree<T,V>* deleteIndex(bstartree<T,V>* Root, T index)
		{
			if (Root == NULL)
				return NULL;
			else if (Root->isLastLevel == false) {
				list<T,V>* buf = Root->childStart;
				list<T,V>* bufNext = (*buf).getNodeNext(buf);
				if (index < (*buf).getIndex(Root->childStart)) 
					return Root;
				else if (index >= (*buf).getIndex(Root->childEnd))
					return deleteIndex((*buf).getNextLevel(Root->childEnd), index);
				else {
					if (index >= (*buf).getIndex(buf) && \
						index < (*buf).getIndex(bufNext))
						return deleteIndex((*buf).getNextLevel(buf), index);					
					while (buf != Root->childEnd) {		
						buf = bufNext;					
						if (index >= (*buf).getIndex(buf) && \
							index < (*buf).getIndex(bufNext))
							return deleteIndex((*buf).getNextLevel(buf), index);
					}					
				}
			} else {
				//���һ�㣬Ҷ�ӽڵ㣬ֱ�Ӳ�������
				if (true == removeIndexToListSet(Root, index));
					return adjustRemoveInsertIndexSet(Root);
			}			
		}
		
		
		
		void printBStarTree(bstartree<T,V>* Root, int level = 0)
		{			
			list<T,V>* buf;		
			if (Root == NULL || Root->currentNum == 0)
				return;
			else if (Root->isLastLevel == true) {
				cout<<"level:"<<level<<"*"<<endl;				
				(*buf).printList(&Root->childStart, &Root->childEnd);	
				cout<<endl;
			} else {
				//�����ӡ
				cout<<"level:"<<level<<endl;				
				(*buf).printList(&Root->childStart, &Root->childEnd);			
				cout<<endl;
				buf = Root->childStart;				
				printBStarTree((*buf).getNextLevel(buf), level + 1);
				while (buf != Root->childEnd) {
					buf = (*buf).getNodeNext(buf);
					printBStarTree((*buf).getNextLevel(buf), level + 1);					
				}
			}	
		}	
};

template<typename T,typename V>
int bstartree<T,V>::keyLength = 0;
template<typename T,typename V>
int bstartree<T,V>::criterion = 0;


void testBStarTree()
{
	//���ݶ��� criterion = 2*keyLength/3;������ȡ3�ͻ�
	//criterion = keyLength,������
	bstartree<int,int>* aHa = (*aHa).creatBStarTree(5);
	
	cout<<"---"<<endl;	
	
	for (int i = 200; i > 130; i -= 5)
		aHa = (*aHa).insertIndex(aHa, i);
	//(*aHa).printBStarTree(aHa);
	
	cout<<"---"<<endl;	
		
	// aHa = (*aHa).deleteIndex(aHa, 185);
	// aHa = (*aHa).deleteIndex(aHa, 135);
	// aHa = (*aHa).deleteIndex(aHa, 140);
	// aHa = (*aHa).deleteIndex(aHa, 145);
	// aHa = (*aHa).deleteIndex(aHa, 150);	
	// aHa = (*aHa).deleteIndex(aHa, 155);
	// aHa = (*aHa).deleteIndex(aHa, 165);
	// aHa = (*aHa).deleteIndex(aHa, 170);	
	// aHa = (*aHa).deleteIndex(aHa, 175);	
	// aHa = (*aHa).deleteIndex(aHa, 195);	
	// aHa = (*aHa).deleteIndex(aHa, 190);	
	// aHa = (*aHa).deleteIndex(aHa, 160);	
	// aHa = (*aHa).deleteIndex(aHa, 175);	
	// aHa = (*aHa).insertIndex(aHa, 100);
	
	(*aHa).printBStarTree(aHa);

	//cout<<"---"<<endl;
	//(*aHa).printBStarTree(aHa);	
	cout<<"---"<<endl;
	(*aHa).deleteBStarTree(aHa);
	cout<<"---"<<endl;	
}

#endif