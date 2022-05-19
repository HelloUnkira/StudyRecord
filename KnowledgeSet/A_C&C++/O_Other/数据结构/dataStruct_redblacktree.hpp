#ifndef __DATASTRUCT_REDBLACKTREE_HPP_
#define __DATASTRUCT_REDBLACKTREE_HPP_

#include<iostream>
#include<stdlib.h>

using namespace std;

template <typename T>
class rbtree
{
	private:		
		//这里直接使用本身数据data作为key，实现二叉排序				
		T data;	
		/* 定义0为红色的结点，1为黑色的结点 */ 
		bool rbColor;
		rbtree* thisParent; 
		rbtree* leftChild;
		rbtree* rightChild;

		
	public:
		//构造及析构函数 
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
			//象征性的整了个节点，事实上它并无用 
			rbtree* Root = new rbtree(Data);
			Root->rbColor = 1; 
			return Root;
		}
	
		void deleteTree(rbtree* Root)
		{
			if (Root == NULL)
				return;
			else {
				//删除左右子树 
				deleteTree(Root->leftChild);
				deleteTree(Root->rightChild);
				//删除自己 
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
			//忽略重复数据 ,找到相同的结点不做插入，直接返回NULL
			else if (Node.data == Root->data)
				return NULL;				
			//左子树为空且该结点小于此本结点，表示结点可以插到其左子树 
			else if (Root->leftChild == NULL && \
				Node.data <= Root->data) {
						rbtree* node = new rbtree(Node.data);
						node->rbColor = 0;	
						node->thisParent = Root;					
						Root->leftChild = node;					
						return node;		
			//右子树为空且该结点大于此本结点，表示结点可以插到其右子树 							
			} else if (Root->rightChild == NULL && \
				Node.data > Root->data) {
						rbtree* node = new rbtree(Node.data);
						node->rbColor = 0;	
						node->thisParent = Root;					
						Root->rightChild = node;				
						return node;						
			} else {
			//往下走呗						
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
			//情况1：插入节点是根节点
			//处理方法，该结点染色为黑色
				Node->rbColor = 1;
			} else if (Node->thisParent->rbColor == 1) {
			//情况2：插入节点父节点是黑色	
			//处理方法，什么都不做 
			} else if (Node->thisParent->rbColor == 0) {
			//情况3：插入节点的父节点是红色
			//分俩大类，每类三种情况
				rbtree* fatherNode = Node->thisParent;
				rbtree* ancestorNode = fatherNode->thisParent; 
				if (ancestorNode == NULL)
					//该情况不应出现，因为红色结点一定有父节点
					return Root;
				rbtree* uncleNode = \
					//祖父结点的左孩子是父节点，则右结点是叔叔结点 
					ancestorNode->leftChild == fatherNode ? \
					ancestorNode->rightChild : ancestorNode->leftChild;				
				//父节点是祖父节点的左孩子 
				if (fatherNode == ancestorNode->leftChild)													 				
				if (uncleNode != NULL && uncleNode->rbColor == 0) {
					//叔叔结点是红色 
					//处理步骤：
					//1：将"父节点"设为黑色
					fatherNode->rbColor = 1; 
					//2：将"叔叔节点"设为黑色
					uncleNode->rbColor = 1;
					//3：将"祖父节点"设为红色
					ancestorNode->rbColor = 0; 
					//4：将祖父节点设为当前节点调整 
					Root = adjustInsertNode(Root, ancestorNode);
				} else {
					//叔叔节点不存在(视为黑色)或者是黑色
					//分情况讨论：当前节点是父节点的左孩子还是右孩子
					if (fatherNode->rightChild == Node) {
						//当前节点是父节点的右孩子 
						//处理步骤：
						//1：将"父节点"作为新的当前节点
						//2：对节点为支点进行左旋				
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
						//当前节点是父节点的左孩子 
						//处理步骤：
						//1：将"父节点"设为黑色
						fatherNode->rbColor = 1; 
						//2：将"祖父节点"设为红色
						ancestorNode->rbColor = 0; 
						//3：对祖父节点进行右旋 
						//进行旋转会改变当前节点与上一节点的关系，旋转完后还需调整
						rbtree* buf_ancestorNode = ancestorNode->thisParent;						
						//判断祖父节点的父节点的哪个孩子是祖父节点，0左1右 
						int flag = 0;
						if (buf_ancestorNode != NULL)
							flag = buf_ancestorNode->leftChild == ancestorNode ? 0 : 1;								
						ancestorNode = rightRotate(ancestorNode);
						//如果祖父结点是根结点的话 
						if (buf_ancestorNode == NULL) {
							Root = ancestorNode;							
							Root->thisParent = NULL;
						} else {
							//调整对应关系：
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
								
				//父节点是祖父节点的右孩子 
				if (fatherNode == ancestorNode->rightChild)													 				
				if (uncleNode != NULL && uncleNode->rbColor == 0) {
					//叔叔结点是红色 
					//处理步骤：
					//1：将"父节点"设为黑色
					fatherNode->rbColor = 1; 
					//2：将"叔叔节点"设为黑色
					uncleNode->rbColor = 1;
					//3：将"祖父节点"设为红色
					ancestorNode->rbColor = 0; 
					//4：将祖父节点设为当前节点调整 
					Root = adjustInsertNode(Root, ancestorNode);
				} else {
					//叔叔节点不存在(视为黑色)或者是黑色
					//分情况讨论：当前节点是父节点的左孩子还是右孩子
					if (fatherNode->leftChild == Node) {
						//当前节点是父节点的左孩子 
						//处理步骤：
						//1：将"父节点"作为新的当前节点
						//2：对节点为支点进行右旋				
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
						//当前节点是父节点的右孩子 
						//处理步骤：
						//1：将"父节点"设为黑色
						fatherNode->rbColor = 1; 
						//2：将"祖父节点"设为红色
						ancestorNode->rbColor = 0; 
						//3：对祖父节点进行左旋 
						//进行旋转会改变当前节点与上一节点的关系，旋转完后还需调整
						rbtree* buf_ancestorNode = ancestorNode->thisParent;
						//判断祖父节点的父节点的哪个孩子是祖父节点，0左1右 
						int flag = 0;
						if (buf_ancestorNode != NULL)
							flag = buf_ancestorNode->leftChild == ancestorNode ? 0 : 1;									
						ancestorNode = leftRotate(ancestorNode);
						//如果祖父结点是根结点的话 
						if (buf_ancestorNode == NULL) {
							Root = ancestorNode;							
							Root->thisParent = NULL;
						} else {
							//调整对应关系：
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
			//先完成插入工作《然后对该节点进行调整 
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
			//查找该部分树的最小节点
			if (Root == NULL)
				return NULL;
			else if (Root->leftChild == NULL)
				return Root;
			else
				return searchMinMaxNode(Root->leftChild);
		}
		
		rbtree* tryDeleteNode(rbtree* Root, rbtree Node)
		{
			//查找要删除的结点 
			rbtree* repareDeletedNode = searchDeleteNode(Root, Node); 
			//找不到要删除的结点，就return 
			if (repareDeletedNode == NULL)
				return NULL;
			//该结点是叶子结点，最终就是删除该结点 
			if (repareDeletedNode->leftChild == NULL && \
				repareDeletedNode->rightChild == NULL) 
				return repareDeletedNode;
			else if (repareDeletedNode->leftChild != NULL && \
				repareDeletedNode->rightChild != NULL) {
				//该结点有俩个孩子		
				//找到要删除的结点的后继结点 
				rbtree* hisCompany = \
					searchMinMaxNode(repareDeletedNode->rightChild);
				//交换该数与后继结点中的值，不交换颜色	
				//此时被删除元素变成其后继结点
				if (hisCompany != NULL)
					repareDeletedNode->data = hisCompany->data; 
				return hisCompany;
			} else 
				//该结点仅有一个孩子 
				return repareDeletedNode;
		}
		
		bool finishDeleteNode(rbtree** Root, rbtree Node, bool* yesno, \
			rbtree** returnHisParent, rbtree** returnHisChild)
		{
			*yesno = 1;
			//通过转化，最终要删除结点至多有一个孩子
			rbtree* repare = tryDeleteNode(*Root, Node);
			if (repare == NULL) {
				*yesno = 0;
				return 0;
			}
			int rbColor = repare->rbColor;
			//它的族系 P,S,N,Sl,Sr
			rbtree*	hisParent = repare->thisParent;
			rbtree* hisChild = NULL;
			//要么没孩子要么只有一个 
			if (repare->leftChild != NULL)
				hisChild = repare->leftChild;
			if (repare->rightChild != NULL)
				hisChild = repare->rightChild; 		
			if (repare->rbColor == 1 && (hisChild != NULL && hisChild->rbColor == 0)) {
				hisChild->rbColor = 1;
				*yesno = 0;				
			} 
			//删除该结点，将其移除出树中	
			if (hisChild != NULL)
				hisChild->thisParent = hisParent;
			if (hisParent != NULL) {
				if (hisParent->leftChild == repare)
					hisParent->leftChild = hisChild;
				else if (hisParent->rightChild == repare)
					hisParent->rightChild = hisChild;				
			} else {
				//删除了根节点，因为只有根节点其父亲为空
				if (hisChild == NULL)
					*Root = NULL;
				else
					*Root = hisChild;
				hisParent = hisChild;
				hisChild = NULL;
			}
			delete repare;
			//删除该结点后，树需要重新调整，回给它的子父结点 
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
									 			
			//修正：
			if (color == 0) {
			//如果删除结点为红色，只需染色孩子结点即可 				
				if (NNode != NULL)
					NNode->rbColor = 1;	
				return Root;			
			} else {
			//如果删除结点为黑色，分六种情况				
				if (NParent == NULL) 
				//情况1：删除的结点为根节点，则所有路径都删除了一个黑色结点
					return Root;
				if (NSibling != NULL && NSibling->rbColor == 0) {
				//情况2：删除的结点的兄弟结点为红色	
				//染色兄弟结点为红色，父节点为黑色
				//孩子结点是父节点的左孩子左旋父节点
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
					//孩子结点是父节点的右孩子右旋父节点
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
				//情况3：其父亲兄弟，兄弟孩子都为黑色
				//染色兄弟 
					NSibling->rbColor = 0;	
				//重定向到parent,继续调整 
					if (NParent != NULL)
						return adjustDeleteNode(Root, NParent, NParent->thisParent, 1); 
					else 
						return adjustDeleteNode(Root, NParent, NULL, 1);
				}
				
				if ((NParent != NULL && NParent->rbColor == 0) && \
					(NSibling == NULL || NSibling->rbColor == 1) && \
					(NSiblingL == NULL || NSiblingL->rbColor == 1) && \
					(NSiblingR == NULL || NSiblingR->rbColor == 1)) {				
				//情况4：其父亲是红色，兄弟孩子都是黑色 
				//兄弟和父亲颜色交换 
					NParent->rbColor = 1; 
					if (NSibling != NULL)
						NSibling->rbColor = 0;
				}
						
				if ((NSibling == NULL || NSibling->rbColor == 1)) { 
				//情况5：兄弟是黑色
					if ((NSiblingL != NULL && NSiblingL->rbColor == 0) && \
						(NSiblingR == NULL || NSiblingR->rbColor == 1) && \
						(NParent != NULL && NNode == NParent->leftChild)) {
					//兄弟左儿子是红色，右儿子是黑色，孩子结点是父亲左结点 					
					//对兄弟做右旋转 
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
					//兄弟右儿子是红色，左儿子是黑色，孩子结点是父亲右结点 					
					//对兄弟做左旋转 
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
				//情况6：兄弟结点是黑色的
					if ((NSiblingR != NULL && NSiblingR->rbColor == 0) && \
						(NNode == NParent->leftChild)) {
					//兄弟的右儿子是红色，子节点是父节点的左儿子
					//对父亲进行左旋转 
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
					//兄弟的左儿子是红色，子节点是父节点的右儿子
					//对父亲进行右旋转 
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
			//通过转化，最终要删除结点至多有一个孩子
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
