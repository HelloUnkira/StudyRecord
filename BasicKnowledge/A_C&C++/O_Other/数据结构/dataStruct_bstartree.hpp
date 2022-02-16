#ifndef __DATASTRUCT_BSTARTREE_HPP_
#define __DATASTRUCT_BSTARTREE_HPP_

#include<iostream>

//B*树
//根据定义 criterion = 2*keyLength/3;但这里取3就会有问题
//因为取3及以下时 criterion >= keyLength,

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
					//绝对信任，一定能从Start 到 End
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
					//断开后交换俩个链表
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
		//定义B树单维度长
		static int keyLength;
		static int criterion;
		//索引集,子节点指针集,下一兄弟	
		list<T,V>* childStart;
		list<T,V>* childEnd;
		bool isLastLevel;
		bstartree<T,V>* thisFather;
		bstartree<T,V>* leftBrother;
		bstartree<T,V>* rightBrother;
		
		int currentNum; //最大为length个
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
			//如果最后数据插入是在数据头部，那么其父节点必须要更新，如果有的话
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
			//满了，要分裂了	
			list<T,V>* help;
			//先看看有没有父节点,因为满节点一定会操作
			if (Root->thisFather == NULL) {
				Root->thisFather = new bstartree<T,V>();						
				//将本节点的拷贝传入父节点
				insertIndexToListSet(Root->thisFather, \
					(*help).getIndex(Root->childStart), Root);											
			}																		
			//叶子节点分裂，所有最后都是叶子节点，不具后继
			//非叶子节点分裂，所有最后都是非叶子节点，具有后继
			if (Root->rightBrother == NULL) {
				Root->rightBrother = new bstartree<T,V>();
				Root->rightBrother->leftBrother = Root;
				Root->rightBrother->thisFather = Root->thisFather;						
				Root->rightBrother->isLastLevel = Root->isLastLevel;	
				//数据迁移，将本节点部分数据迁移到兄弟去
				(*help).breakList(criterion, &Root->childStart, \
					&Root->childEnd, &Root->rightBrother->childStart, \
					&Root->rightBrother->childEnd);
				Root->currentNum -= (keyLength - criterion);
				Root->rightBrother->currentNum += (keyLength - criterion);				
				//是叶节点分裂还是非叶子节点分裂
				//修改孩子的父亲为新父亲
				if (Root->isLastLevel == false)						
					(*help).changeNewFather(&Root->rightBrother->childStart, \
						&Root->rightBrother->childEnd, Root->rightBrother);								
				//将兄弟节点加入到父节点
				insertIndexToListSet(Root->rightBrother->thisFather, \
					(*help).getIndex(Root->rightBrother->childStart), \
					Root->rightBrother);
				return adjustInsertIndexSet(Root->rightBrother->thisFather);							
			} else if (Root->rightBrother->currentNum > criterion) {
				//自己与兄弟都负重不堪，创建一个新兄弟，承担后代替原有兄弟
				bstartree<T,V>* buf = new bstartree<T,V>();
				buf->thisFather = Root->rightBrother->thisFather;
				buf->isLastLevel = Root->rightBrother->isLastLevel;
				//数据迁移，将本节点部分数据迁移到新兄弟中去
				(*help).breakList(criterion, &Root->childStart, \
					&Root->childEnd, &buf->childStart, &buf->childEnd);
				Root->currentNum -= (keyLength - criterion);
				buf->currentNum += (keyLength - criterion);					
				//也为原兄弟分担一部分数据迁移
				list<T,V>* tempStart = NULL,*tempEnd = NULL;
				T temp = (*help).getIndex(Root->rightBrother->childStart);
				(*help).breakList(keyLength - criterion, \
					&Root->rightBrother->childStart, \
					&Root->rightBrother->childEnd, &tempStart, &tempEnd, true);						
				(*help).mergeList(&buf->childStart, &buf->childEnd, \
					&tempStart, &tempEnd);
				Root->rightBrother->currentNum -= criterion;
				buf->currentNum += criterion;							
				//是叶节点分裂还是非叶子节点分裂
				if (Root->isLastLevel == true)
					(*help).changeNewFather(&buf->childStart, &buf->childEnd, buf);							
				//跟新关系
				buf->leftBrother = Root;
				buf->rightBrother = Root->rightBrother;
				Root->rightBrother->leftBrother = buf;
				Root->rightBrother = buf;				
				//更新原兄弟的父亲，新兄弟随原兄弟找父亲
				levelUpdateIndexFromListSet(buf->rightBrother->thisFather, \
					temp, (*help).getIndex(buf->rightBrother->childStart), \
					buf->rightBrother);
				insertIndexToListSet(buf->thisFather, \
					(*help).getIndex(buf->childStart), buf);	
				return adjustInsertIndexSet(buf->rightBrother->thisFather);
			} else {
				//让兄弟分担点
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
				//是叶节点分裂还是非叶子节点分裂
				//修改孩子的父亲为新父亲
				if (Root->isLastLevel == true)
					(*help).changeNewFather(&Root->rightBrother->childStart, \
						&Root->rightBrother->childEnd, Root->rightBrother);				
				//兄弟的父亲要跟新一下
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
				//最后一层，叶子节点，直接插入数据
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
					//到这里肯定是非叶子节点，那么原纪录全部都要跟新，
					//因为父记录一般不会是单个，但因为设计的原因
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
					//到这里肯定是非叶子节点，那么原纪录全部都要跟新，
					//因为父记录一般不会是单个，但因为设计的原因
					levelUpdateIndexFromListSet(Root->thisFather, temp, \
						(*help).getIndex(Root->childStart), Root);				
				return true;
			}
			return false;
		}

		bstartree<T,V>* adjustRemoveInsertIndexSet(bstartree<T,V>* Root)
		{
			//看情况合并右兄弟即可，（其实左右兄弟无非多加一个指针，代码翻一倍即可）
			if (Root->currentNum >= criterion) {
				bstartree<T,V>* buf = Root;
				while(buf->thisFather != NULL)
					buf = buf->thisFather;
				return buf;	
			} else if (Root->leftBrother == NULL && Root->rightBrother == NULL) {
				//可以确定的是，仅有最顶层节点会出现孤立状态				
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
				//自己数据少了，先问兄弟借一个,先看看右兄弟
				if (Root->currentNum + Root->rightBrother->currentNum >= keyLength) {cout<<"*1"<<endl;
					//借出兄弟第一个节点就可以了，不过要跟新兄弟
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
					//跟新一下兄弟
					levelUpdateIndexFromListSet(Root->rightBrother->thisFather, \
						temp, (*help).getIndex(Root->rightBrother->childStart), Root->rightBrother);
					return adjustRemoveInsertIndexSet(Root);
				} else if (Root->currentNum + Root->rightBrother->currentNum < keyLength) {cout<<"*2"<<endl;
					//兄弟节点没有富裕索引，合并，此时则是将删除操作提到上层去了
					list<T,V>* help;
					bstartree<T,V>* ptemp = Root->rightBrother;
					T temp = (*help).getIndex(Root->rightBrother->childStart);
					if (Root->isLastLevel == false)
						(*help).changeNewFather(&Root->rightBrother->childStart, \
							&Root->rightBrother->childEnd, Root);
					(*help).mergeList(&Root->childStart, &Root->childEnd, \
						&Root->rightBrother->childStart, &Root->rightBrother->childEnd);
					Root->currentNum += Root->rightBrother->currentNum;
					//断除关系
					bstartree<T,V>* rightBrotherFather = Root->rightBrother->thisFather;					
					Root->rightBrother = Root->rightBrother->rightBrother;
					if (Root->rightBrother != NULL)
						Root->rightBrother->leftBrother = Root;	
					delete ptemp;
					//向父层删除节点
					levelremoveIndexToListSet(rightBrotherFather, temp);
					return adjustRemoveInsertIndexSet(rightBrotherFather);	
															
				}	
				
							
			if (Root->leftBrother != NULL)
				//自己数据少了，先问兄弟借一个,再看看左兄弟
				if (Root->currentNum + Root->leftBrother->currentNum >= keyLength) {cout<<"*3"<<endl;
					//借出兄弟第一个节点就可以了，不过要跟新兄弟
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
					//跟新一下自己
					levelUpdateIndexFromListSet(Root->thisFather, \
						temp, (*help).getIndex(Root->childStart), Root);
					return adjustRemoveInsertIndexSet(Root);
				} else if (Root->currentNum + Root->leftBrother->currentNum < keyLength) {cout<<"*4"<<endl;
					//兄弟节点没有富裕索引，合并，此时则是将删除操作提到上层去了
					//注意，这里是交付自己的孩子给左节点，而不是删掉左节点，但没有本质区别
					list<T,V>* help;
					T temp = (*help).getIndex(Root->childStart);
					if (Root->isLastLevel == false)
						(*help).changeNewFather(&Root->childStart, \
							&Root->childEnd, Root->leftBrother);					
					(*help).mergeList(&Root->leftBrother->childStart, \
						&Root->leftBrother->childEnd, &Root->childStart, &Root->childEnd);
					Root->leftBrother->currentNum += Root->currentNum;
					//断除关系
					Root->leftBrother->rightBrother = Root->rightBrother;					
					if (Root->rightBrother != NULL)
						Root->rightBrother->leftBrother = Root->leftBrother;					
					bstartree<T,V>* rootThisFather = Root->thisFather;						
					delete Root;
					//向父层删除节点
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
				//最后一层，叶子节点，直接插入数据
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
				//先序打印
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
	//根据定义 criterion = 2*keyLength/3;但这里取3就会
	//criterion = keyLength,有问题
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