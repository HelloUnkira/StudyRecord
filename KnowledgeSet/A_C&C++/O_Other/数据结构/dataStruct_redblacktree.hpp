#ifndef __DATASTRUCT_REDBLACKTREE_HPP_
#define __DATASTRUCT_REDBLACKTREE_HPP_

#include<iostream>
#include<stdlib.h>

using namespace std;

template <typename T>
class rbtree
{
	private:		
		//����ֱ��ʹ�ñ�������data��Ϊkey��ʵ�ֶ�������				
		T data;	
		/* ����0Ϊ��ɫ�Ľ�㣬1Ϊ��ɫ�Ľ�� */ 
		bool rbColor;
		rbtree* thisParent; 
		rbtree* leftChild;
		rbtree* rightChild;

		
	public:
		//���켰�������� 
		rbtree()
		{
			this->data 		 = T(0);
			this->rbColor 	 = 0;
			this->thisParent = NULL; 
			this->leftChild  = NULL;
			this->rightChild = NULL;
		}
	
		rbtree(T Data)
		{
			this->data 		 = Data;
			this->rbColor 	 = 0;
			this->thisParent = NULL; 
			this->leftChild  = NULL;
			this->rightChild = NULL;						
		}
	
		~rbtree()
		{
		}
	
		rbtree* createTree(T Data) 
		{
			//�����Ե����˸��ڵ㣬��ʵ���������� 
			rbtree* Root = new rbtree(Data);
			Root->rbColor = 1; 
			return Root;
		}
	
		void deleteTree(rbtree* Root)
		{
			if (Root == NULL)
				return;
			else {
				//ɾ���������� 
				deleteTree(Root->leftChild);
				deleteTree(Root->rightChild);
				//ɾ���Լ� 
				delete Root;
				Root = NULL; 				
			}						
		}
	
		void traversalTree(rbtree* Root)
		{
			if (Root == NULL) 
				return;
			else {
				cout<<Root->data<<"-"<<Root->rbColor<<endl;
				if (Root->leftChild != NULL)
					cout<<Root->leftChild->data<<"-"\
						<<Root->leftChild->rbColor<<endl;
				if (Root->rightChild != NULL)
					cout<<Root->rightChild->data<<"-"\
						<<Root->rightChild->rbColor<<endl;					
				cout<<endl;	
					
				traversalTree(Root->leftChild);		
				traversalTree(Root->rightChild);								
			}			
		} 
 
		rbtree* leftRotate(rbtree* Root)
		{
			if (Root == NULL)
				return Root;
			else {
				rbtree* buf = Root->rightChild;
				if (buf != NULL) {
					Root->rightChild = buf->leftChild;
					if (buf->leftChild != NULL)					 
						buf->leftChild->thisParent = Root;	
									
					Root->thisParent = buf;
					buf->leftChild = Root;
					return buf;										
				}
				return Root;	
			}									
		}
		
		rbtree* rightRotate(rbtree* Root)		
		{
			if (Root == NULL)
				return Root;
			else {
				rbtree* buf = Root->leftChild;
				if (buf != NULL) {
					Root->leftChild = buf->rightChild;	
					if (buf->rightChild != NULL)
						buf->rightChild->thisParent = Root;
						
					buf->rightChild = Root;	
					Root->thisParent = buf;		
					return buf;				
				}
				return Root;
			}						
		}
		
		rbtree* onlyInsertNode(rbtree* Root, rbtree Node)
		{
			if (Root == NULL)
				return NULL;
			//�����ظ����� ,�ҵ���ͬ�Ľ�㲻�����룬ֱ�ӷ���NULL
			else if (Node.data == Root->data)
				return NULL;				
			//������Ϊ���Ҹý��С�ڴ˱���㣬��ʾ�����Բ嵽�������� 
			else if (Root->leftChild == NULL && \
				Node.data <= Root->data) {
						rbtree* node = new rbtree(Node.data);
						node->rbColor = 0;	
						node->thisParent = Root;					
						Root->leftChild = node;					
						return node;		
			//������Ϊ���Ҹý����ڴ˱���㣬��ʾ�����Բ嵽�������� 							
			} else if (Root->rightChild == NULL && \
				Node.data > Root->data) {
						rbtree* node = new rbtree(Node.data);
						node->rbColor = 0;	
						node->thisParent = Root;					
						Root->rightChild = node;				
						return node;						
			} else {
			//��������						
				if (Node.data <= Root->data) {
					if (Root->leftChild != NULL)
						return onlyInsertNode(Root->leftChild,Node);													
				} else {			
					if (Root->rightChild != NULL)
						return onlyInsertNode(Root->rightChild,Node);														
				}								
			}		
	
		}
		
		rbtree* adjustInsertNode(rbtree* Root, rbtree* Node)
		{
			if (Node == NULL)
				return Root;
			else if (Node->thisParent == NULL) {
			//���1������ڵ��Ǹ��ڵ�
			//���������ý��ȾɫΪ��ɫ
				Node->rbColor = 1;
			} else if (Node->thisParent->rbColor == 1) {
			//���2������ڵ㸸�ڵ��Ǻ�ɫ	
			//��������ʲô������ 
			} else if (Node->thisParent->rbColor == 0) {
			//���3������ڵ�ĸ��ڵ��Ǻ�ɫ
			//�������࣬ÿ���������
				rbtree* fatherNode = Node->thisParent;
				rbtree* ancestorNode = fatherNode->thisParent; 
				if (ancestorNode == NULL)
					//�������Ӧ���֣���Ϊ��ɫ���һ���и��ڵ�
					return Root;
				rbtree* uncleNode = \
					//�游���������Ǹ��ڵ㣬���ҽ���������� 
					ancestorNode->leftChild == fatherNode ? \
					ancestorNode->rightChild : ancestorNode->leftChild;				
				//���ڵ����游�ڵ������ 
				if (fatherNode == ancestorNode->leftChild)													 				
				if (uncleNode != NULL && uncleNode->rbColor == 0) {
					//�������Ǻ�ɫ 
					//�����裺
					//1����"���ڵ�"��Ϊ��ɫ
					fatherNode->rbColor = 1; 
					//2����"����ڵ�"��Ϊ��ɫ
					uncleNode->rbColor = 1;
					//3����"�游�ڵ�"��Ϊ��ɫ
					ancestorNode->rbColor = 0; 
					//4�����游�ڵ���Ϊ��ǰ�ڵ���� 
					Root = adjustInsertNode(Root, ancestorNode);
				} else {
					//����ڵ㲻����(��Ϊ��ɫ)�����Ǻ�ɫ
					//��������ۣ���ǰ�ڵ��Ǹ��ڵ�����ӻ����Һ���
					if (fatherNode->rightChild == Node) {
						//��ǰ�ڵ��Ǹ��ڵ���Һ��� 
						//�����裺
						//1����"���ڵ�"��Ϊ�µĵ�ǰ�ڵ�
						//2���Խڵ�Ϊ֧���������				
						int flag = \
							ancestorNode->leftChild == fatherNode ? 0 : 1;
						fatherNode = leftRotate(fatherNode); 
						if (flag == 0) {
							ancestorNode->leftChild = fatherNode;	
							fatherNode->thisParent = ancestorNode;
						} 
						if (flag == 1) {
							ancestorNode->rightChild = fatherNode;	
							fatherNode->thisParent = ancestorNode;							
						}
						Node = fatherNode->leftChild;											
					}					
					if (fatherNode->leftChild == Node) {
						//��ǰ�ڵ��Ǹ��ڵ������ 
						//�����裺
						//1����"���ڵ�"��Ϊ��ɫ
						fatherNode->rbColor = 1; 
						//2����"�游�ڵ�"��Ϊ��ɫ
						ancestorNode->rbColor = 0; 
						//3�����游�ڵ�������� 
						//������ת��ı䵱ǰ�ڵ�����һ�ڵ�Ĺ�ϵ����ת��������
						rbtree* buf_ancestorNode = ancestorNode->thisParent;						
						//�ж��游�ڵ�ĸ��ڵ���ĸ��������游�ڵ㣬0��1�� 
						int flag = 0;
						if (buf_ancestorNode != NULL)
							flag = buf_ancestorNode->leftChild == ancestorNode ? 0 : 1;								
						ancestorNode = rightRotate(ancestorNode);
						//����游����Ǹ����Ļ� 
						if (buf_ancestorNode == NULL) {
							Root = ancestorNode;							
							Root->thisParent = NULL;
						} else {
							//������Ӧ��ϵ��
							if (flag == 0) {	
								buf_ancestorNode->leftChild = ancestorNode;						
								ancestorNode->thisParent = buf_ancestorNode;
							}
							if (flag == 1){
								buf_ancestorNode->rightChild = ancestorNode;
								ancestorNode->thisParent = buf_ancestorNode;
							}							
						}																 
					}									
				}
								
				//���ڵ����游�ڵ���Һ��� 
				if (fatherNode == ancestorNode->rightChild)													 				
				if (uncleNode != NULL && uncleNode->rbColor == 0) {
					//�������Ǻ�ɫ 
					//�����裺
					//1����"���ڵ�"��Ϊ��ɫ
					fatherNode->rbColor = 1; 
					//2����"����ڵ�"��Ϊ��ɫ
					uncleNode->rbColor = 1;
					//3����"�游�ڵ�"��Ϊ��ɫ
					ancestorNode->rbColor = 0; 
					//4�����游�ڵ���Ϊ��ǰ�ڵ���� 
					Root = adjustInsertNode(Root, ancestorNode);
				} else {
					//����ڵ㲻����(��Ϊ��ɫ)�����Ǻ�ɫ
					//��������ۣ���ǰ�ڵ��Ǹ��ڵ�����ӻ����Һ���
					if (fatherNode->leftChild == Node) {
						//��ǰ�ڵ��Ǹ��ڵ������ 
						//�����裺
						//1����"���ڵ�"��Ϊ�µĵ�ǰ�ڵ�
						//2���Խڵ�Ϊ֧���������				
						int flag = \
							ancestorNode->leftChild == fatherNode ? 0 : 1;						
						fatherNode = rightRotate(fatherNode); 
						if (flag == 0) {
							ancestorNode->leftChild = fatherNode;	
							fatherNode->thisParent = ancestorNode;
						} 
						if (flag == 1) {
							ancestorNode->rightChild = fatherNode;	
							fatherNode->thisParent = ancestorNode;							
						}
						Node = fatherNode->rightChild;										
					}					
					if (fatherNode->rightChild == Node) {
						//��ǰ�ڵ��Ǹ��ڵ���Һ��� 
						//�����裺
						//1����"���ڵ�"��Ϊ��ɫ
						fatherNode->rbColor = 1; 
						//2����"�游�ڵ�"��Ϊ��ɫ
						ancestorNode->rbColor = 0; 
						//3�����游�ڵ�������� 
						//������ת��ı䵱ǰ�ڵ�����һ�ڵ�Ĺ�ϵ����ת��������
						rbtree* buf_ancestorNode = ancestorNode->thisParent;
						//�ж��游�ڵ�ĸ��ڵ���ĸ��������游�ڵ㣬0��1�� 
						int flag = 0;
						if (buf_ancestorNode != NULL)
							flag = buf_ancestorNode->leftChild == ancestorNode ? 0 : 1;									
						ancestorNode = leftRotate(ancestorNode);
						//����游����Ǹ����Ļ� 
						if (buf_ancestorNode == NULL) {
							Root = ancestorNode;							
							Root->thisParent = NULL;
						} else {
							//������Ӧ��ϵ��
							if (flag == 0) {
								buf_ancestorNode->leftChild = ancestorNode;
								ancestorNode->thisParent = buf_ancestorNode;
							}
							if (flag == 1){
								buf_ancestorNode->rightChild = ancestorNode;
								ancestorNode->thisParent = buf_ancestorNode;
							}							
						}
					}
				}
			}
			return Root;
		} 
					
		rbtree* insertNode(rbtree* Root, rbtree Node)
		{
			//����ɲ��빤����Ȼ��Ըýڵ���е��� 
			return adjustInsertNode(Root, onlyInsertNode(Root, Node));									
		}
		
		rbtree* searchDeleteNode(rbtree* Root, rbtree Node) 
		{
			if (Root == NULL)
				return NULL;
			else if (Root->data == Node.data)
				return Root;
			else {
				if (Root->leftChild != NULL && \
					Node.data <= Root->data)
					return searchDeleteNode(Root->leftChild, Node);
				if (Root->rightChild != NULL && \
					Node.data >= Root->data)
					return searchDeleteNode(Root->rightChild, Node);
				return NULL;
			}			
		}
		
		rbtree* searchMinMaxNode(rbtree* Root)
		{
			//���Ҹò���������С�ڵ�
			if (Root == NULL)
				return NULL;
			else if (Root->leftChild == NULL)
				return Root;
			else
				return searchMinMaxNode(Root->leftChild);
		}
		
		rbtree* tryDeleteNode(rbtree* Root, rbtree Node)
		{
			//����Ҫɾ���Ľ�� 
			rbtree* repareDeletedNode = searchDeleteNode(Root, Node); 
			//�Ҳ���Ҫɾ���Ľ�㣬��return 
			if (repareDeletedNode == NULL)
				return NULL;
			//�ý����Ҷ�ӽ�㣬���վ���ɾ���ý�� 
			if (repareDeletedNode->leftChild == NULL && \
				repareDeletedNode->rightChild == NULL) 
				return repareDeletedNode;
			else if (repareDeletedNode->leftChild != NULL && \
				repareDeletedNode->rightChild != NULL) {
				//�ý������������		
				//�ҵ�Ҫɾ���Ľ��ĺ�̽�� 
				rbtree* hisCompany = \
					searchMinMaxNode(repareDeletedNode->rightChild);
				//�����������̽���е�ֵ����������ɫ	
				//��ʱ��ɾ��Ԫ�ر�����̽��
				if (hisCompany != NULL)
					repareDeletedNode->data = hisCompany->data; 
				return hisCompany;
			} else 
				//�ý�����һ������ 
				return repareDeletedNode;
		}
		
		bool finishDeleteNode(rbtree** Root, rbtree Node, bool* yesno, \
			rbtree** returnHisParent, rbtree** returnHisChild)
		{
			*yesno = 1;
			//ͨ��ת��������Ҫɾ�����������һ������
			rbtree* repare = tryDeleteNode(*Root, Node);
			if (repare == NULL) {
				*yesno = 0;
				return 0;
			}
			int rbColor = repare->rbColor;
			//������ϵ P,S,N,Sl,Sr
			rbtree*	hisParent = repare->thisParent;
			rbtree* hisChild = NULL;
			//Ҫôû����Ҫôֻ��һ�� 
			if (repare->leftChild != NULL)
				hisChild = repare->leftChild;
			if (repare->rightChild != NULL)
				hisChild = repare->rightChild; 		
			if (repare->rbColor == 1 && (hisChild != NULL && hisChild->rbColor == 0)) {
				hisChild->rbColor = 1;
				*yesno = 0;				
			} 
			//ɾ���ý�㣬�����Ƴ�������	
			if (hisChild != NULL)
				hisChild->thisParent = hisParent;
			if (hisParent != NULL) {
				if (hisParent->leftChild == repare)
					hisParent->leftChild = hisChild;
				else if (hisParent->rightChild == repare)
					hisParent->rightChild = hisChild;				
			} else {
				//ɾ���˸��ڵ㣬��Ϊֻ�и��ڵ��丸��Ϊ��
				if (hisChild == NULL)
					*Root = NULL;
				else
					*Root = hisChild;
				hisParent = hisChild;
				hisChild = NULL;
			}
			delete repare;
			//ɾ���ý�������Ҫ���µ������ظ������Ӹ���� 
			*returnHisParent = hisParent;
			*returnHisChild = hisChild;
			return rbColor;
		}

		rbtree* adjustDeleteNode(rbtree* Root,rbtree* Node, rbtree* hisFatherNode, bool color)
		{			
			rbtree* NNode = Node;
			rbtree* NParent = hisFatherNode;
			rbtree* NSibling = NULL, *NSiblingL = NULL, *NSiblingR = NULL;
			if (NParent != NULL)
				NSibling = NParent->leftChild == NNode ? \
					NParent->rightChild : NParent->leftChild;
			if (NSibling != NULL) {
				NSiblingL = NSibling->leftChild;
				NSiblingR = NSibling->rightChild;					
			}					
									 			
			//������
			if (color == 0) {
			//���ɾ�����Ϊ��ɫ��ֻ��Ⱦɫ���ӽ�㼴�� 				
				if (NNode != NULL)
					NNode->rbColor = 1;	
				return Root;			
			} else {
			//���ɾ�����Ϊ��ɫ�����������				
				if (NParent == NULL) 
				//���1��ɾ���Ľ��Ϊ���ڵ㣬������·����ɾ����һ����ɫ���
					return Root;
				if (NSibling != NULL && NSibling->rbColor == 0) {
				//���2��ɾ���Ľ����ֵܽ��Ϊ��ɫ	
				//Ⱦɫ�ֵܽ��Ϊ��ɫ�����ڵ�Ϊ��ɫ
				//���ӽ���Ǹ��ڵ�������������ڵ�
					if (NNode == NParent->leftChild) {
						NParent->rbColor = 1;
						NSibling->rbColor = 0;
						rbtree * help = NParent->thisParent;					 	
						NParent = leftRotate(NParent);
						if (help == NULL) 
							Root = NParent;
						else {
							int flag = help->leftChild == NParent ? 0 : 1;							
							if (flag == 0) {
								help->leftChild = NParent;
								NParent->thisParent = help;						
							}
							if (flag == 1) {
								help->rightChild = NParent;
								NParent->thisParent = help;
							}
						}
						NSibling = NParent->rightChild;
						if (NSibling != NULL) {
							NSiblingL = NSibling->leftChild;
							NSiblingR = NSibling->rightChild;							
						} else {
							NSiblingL = NULL;
							NSiblingR = NULL;														
						}						
					}
					//���ӽ���Ǹ��ڵ���Һ����������ڵ�
					if (NNode == NParent->rightChild) {
						NParent->rbColor = 1;
						NSibling->rbColor = 0;					
						rbtree * help = NParent->thisParent;						 	
						NParent = rightRotate(NParent);
						if (help == NULL) 
							Root = NParent;
						else {
							int flag = help->leftChild == NParent ? 0 : 1;								
							if (flag == 0) {
								help->leftChild = NParent;
								NParent->thisParent = help;						
							}
							if (flag == 1) {
								help->rightChild = NParent;
								NParent->thisParent = help;
							}
						}
						NSibling = NParent->leftChild;
						if (NSibling != NULL) {
							NSiblingL = NSibling->leftChild;
							NSiblingR = NSibling->rightChild;							
						} else {
							NSiblingL = NULL;
							NSiblingR = NULL;														
						}												
					}					
				}
				
				if ((NParent == NULL || NParent->rbColor == 1) && \
					(NSibling == NULL || NSibling->rbColor == 1) && \
					(NSiblingL == NULL || NSiblingL->rbColor == 1) && \
					(NSiblingR == NULL || NSiblingR->rbColor == 1)) {
				//���3���丸���ֵܣ��ֵܺ��Ӷ�Ϊ��ɫ
				//Ⱦɫ�ֵ� 
					NSibling->rbColor = 0;	
				//�ض���parent,�������� 
					if (NParent != NULL)
						return adjustDeleteNode(Root, NParent, NParent->thisParent, 1); 
					else 
						return adjustDeleteNode(Root, NParent, NULL, 1);
				}
				
				if ((NParent != NULL && NParent->rbColor == 0) && \
					(NSibling == NULL || NSibling->rbColor == 1) && \
					(NSiblingL == NULL || NSiblingL->rbColor == 1) && \
					(NSiblingR == NULL || NSiblingR->rbColor == 1)) {				
				//���4���丸���Ǻ�ɫ���ֵܺ��Ӷ��Ǻ�ɫ 
				//�ֵܺ͸�����ɫ���� 
					NParent->rbColor = 1; 
					if (NSibling != NULL)
						NSibling->rbColor = 0;
				}
						
				if ((NSibling == NULL || NSibling->rbColor == 1)) { 
				//���5���ֵ��Ǻ�ɫ
					if ((NSiblingL != NULL && NSiblingL->rbColor == 0) && \
						(NSiblingR == NULL || NSiblingR->rbColor == 1) && \
						(NParent != NULL && NNode == NParent->leftChild)) {
					//�ֵ�������Ǻ�ɫ���Ҷ����Ǻ�ɫ�����ӽ���Ǹ������� 					
					//���ֵ�������ת 
						NSiblingL->rbColor = 1;
						NSibling->rbColor = 0;
						rbtree * help = NSibling->thisParent;					 	
						NSibling = rightRotate(NSibling);
						if (help == NULL) 
							Root = NSibling;
						else {
								help->rightChild = NSibling;
								NSibling->thisParent = help;
						}
						NSiblingL = NSibling->leftChild;
						NSiblingR = NSibling->rightChild;										
					}
					if ((NSiblingR != NULL && NSiblingR->rbColor == 0) && \
						(NSiblingL == NULL || NSiblingL->rbColor == 1) && \
						(NParent != NULL && NNode == NParent->rightChild)) {
					//�ֵ��Ҷ����Ǻ�ɫ��������Ǻ�ɫ�����ӽ���Ǹ����ҽ�� 					
					//���ֵ�������ת 
						NSiblingR->rbColor = 1;
						NSibling->rbColor = 0;					
						rbtree * help = NSibling->thisParent;					 	
						NSibling = leftRotate(NSibling);
						if (help == NULL) 
							Root = NSibling;
						else {
								help->leftChild = NSibling;
								NSibling->thisParent = help;
						}
						NSiblingL = NSibling->leftChild;
						NSiblingR = NSibling->rightChild;																
					}									
				}
				
				if ((NSibling == NULL || NSibling->rbColor == 1)) {
				//���6���ֵܽ���Ǻ�ɫ��
					if ((NSiblingR != NULL && NSiblingR->rbColor == 0) && \
						(NNode == NParent->leftChild)) {
					//�ֵܵ��Ҷ����Ǻ�ɫ���ӽڵ��Ǹ��ڵ�������
					//�Ը��׽�������ת 
						NSibling->rbColor = NParent->rbColor;	
						NParent->rbColor = 1;
						NSiblingR->rbColor = 1; 
						rbtree * help = NParent->thisParent;
						int flag = 0;
						if (help != NULL)
							flag = help->leftChild == NParent ? 0 : 1;							 	
						NParent = leftRotate(NParent);
						if (help == NULL) 
							Root = NParent;
						else {							
							if (flag == 0) {
								help->leftChild = NParent;
								NParent->thisParent = help;						
							}
							if (flag == 1) {
								help->rightChild = NParent;
								NParent->thisParent = help;
							}
						}																		
					}
					if ((NSiblingL != NULL && NSiblingL->rbColor == 0) && \
						(NNode == NParent->rightChild)) {
					//�ֵܵ�������Ǻ�ɫ���ӽڵ��Ǹ��ڵ���Ҷ���
					//�Ը��׽�������ת 
						NSibling->rbColor = NParent->rbColor;
						NParent->rbColor = 1;
						NSiblingL->rbColor = 1; 
						rbtree * help = NParent->thisParent;
						int flag = 0;
						if (help != NULL)
							flag = help->leftChild == NParent ? 0 : 1;									 	
						NParent = rightRotate(NParent);
						if (help == NULL) 
							Root = NParent;
						else {							
							if (flag == 0) {
								help->leftChild = NParent;
								NParent->thisParent = help;						
							}
							if (flag == 1) {
								help->rightChild = NParent;
								NParent->thisParent = help;
							}
						}																			
					}					
				}
			return Root;
			}
		}

		rbtree* deleteNode(rbtree* Root, rbtree Node)
		{
			//ͨ��ת��������Ҫɾ�����������һ������
			rbtree* hisFather = NULL;
			rbtree* needAdjustNode = NULL;
			bool yesno = 1;
			int flag = finishDeleteNode(&Root, Node, &yesno, &hisFather, &needAdjustNode);
			if (yesno == 1)
				Root = adjustDeleteNode(Root, needAdjustNode, hisFather, flag);
			return Root;
		} 
				
};

void testRedBlackTree()
{
	rbtree<int>* aHa = (*aHa).createTree(50);
	
	int test[20] = {0};
	
	for (int i = 0; i < 20; i++) {
		test[i] = rand() % 100;		
		cout<<test[i]<<" "<<endl;
	}
	
	for (int i = 0 ;i < 20; i++) {
		int data = test[i];
		rbtree<int> node(data);
		aHa = (*aHa).insertNode(aHa,node);
	}
	
	(*aHa).traversalTree(aHa);	
	cout<<"+++++++++"<<endl;
	
	for (int i = 0 ;i < 20; i++) {
		int data = test[i];
		rbtree<int> node(data);
		aHa = (*aHa).deleteNode(aHa,node);
		(*aHa).traversalTree(aHa);				
	cout<<"+++++++++"<<endl;			
	}
				
	(*aHa).deleteTree(aHa); 	
}
	
#endif
