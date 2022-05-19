#ifndef __DATASTRUCT_MAP_HPP_
#define __DATASTRUCT_MAP_HPP_

#include<iostream>

//基于十字链表结构 

using namespace std;

template<typename T>
class mapTableVertex;

//管理弧节点的类，即就是边 
template<typename T>
class mapTableEdge
{
	private: 
		mapTableVertex<T>* tailVertex; //弧的发起者，即出度时	
		mapTableVertex<T>* headVertex; //弧的接收者，即入度时 
		mapTableEdge<T>* headLink; //出度一致时链接表 
		mapTableEdge<T>* tailLink; //入度一致时链接表 
		int weight; //权值，边重，默认这里不使用 	
	public:
		mapTableEdge()
		{
			this->tailVertex = NULL;
			this->headVertex = NULL;
			this->headLink = NULL;
			this->tailLink = NULL;			
			this->weight = 1;
		}
		
		~mapTableEdge()
		{
		}

		mapTableEdge<T>* getHeadLink()
		{
			return this->headLink;
		}
		
		mapTableEdge<T>* getTailLink()		
		{
			return this->tailLink;
		}
		
		//插入一个弧等价于，对起始节点建立一个新出度，对结束节点建立一个新入度
		mapTableEdge<T>* addOutDegree(mapTableEdge<T>* list, mapTableEdge<T>* newNode)
		{
			if (list == NULL)
				return newNode;
			else {
				mapTableEdge<T>* buf = list;
				//尾插即可，无需排序 
				while(buf->tailLink != NULL) {
					if (buf->headVertex == newNode->headVertex)
						return list;
					buf = buf->tailLink;					
				}
	
				buf->tailLink = newNode;	
				return list;
			}				
		}
			
		mapTableEdge<T>* addInDegree(mapTableEdge<T>* list, mapTableEdge<T>* newNode)
		{
			if (list == NULL)
				return newNode;
			else {
				mapTableEdge<T>* buf = list;
				//尾插即可，无需排序 
				while(buf->headLink != NULL) {
					if (buf->headVertex == newNode->headVertex) 
						return list;
					buf = buf->headLink;					
				}
	
				buf->headLink = newNode;
				return list;	
			}			
		}	
		 
		void addEdge(mapTableEdge<T>* newNode) 
		{
			//添加出度
			(*newNode->tailVertex).setFirstOutDegree(\
				addOutDegree((*newNode->tailVertex).getFirstOutDegree(), newNode));
			//添加入度
			(*newNode->headVertex).setFirstInDegree(\
				addInDegree((*newNode->headVertex).getFirstInDegree(), newNode));
		}
		
		//移除一个弧节点等价于断开它对应的入度和出度，主要关系的是指向该节点的而不是节点指出去的 
		mapTableEdge<T>* removeOutDegree(mapTableEdge<T>* list, mapTableEdge<T>* newNode)
		{
			//移除不了 
			if (list == NULL)
				return NULL;
			else if (list->headVertex == newNode->headVertex) 
				return list->tailLink;
			else {
				mapTableEdge<T>* buf_prev = list;
				mapTableEdge<T>* buf = list->tailLink;
				while (buf != NULL) {
					if (buf->headVertex == newNode->headVertex)
						buf_prev->tailLink = buf->tailLink;
					buf_prev = buf;
					buf = buf->tailLink;					
				}
				return list;			
			}	 
		}	
		 
		mapTableEdge<T>* removeInDegree(mapTableEdge<T>* list, mapTableEdge<T>* newNode)
		{
			//移除不了 
			if (list == NULL)
				return NULL;
			else if (list->headVertex == newNode->headVertex) 
				return list->headLink;
			else {
				mapTableEdge<T>* buf_prev = list;
				mapTableEdge<T>* buf = list->headLink;
				while (buf != NULL) {
					if (buf->headVertex == newNode->headVertex)
						buf_prev->headLink = buf->headLink;
					buf_prev = buf;
					buf = buf->headLink;					
				}
				return list;			
			}	 
		}	
		
		void removeEdge(mapTableEdge<T>* newNode) 
		{
			//移除出度
			(*newNode->tailVertex).setFirstOutDegree(\
				removeOutDegree((*newNode->tailVertex).getFirstOutDegree(), newNode));
			//移除入度
			(*newNode->headVertex).setFirstInDegree(\
				removeInDegree((*newNode->headVertex).getFirstInDegree(), newNode));
			delete newNode;			
		}
		
		//建立关系 
		void createEdge(mapTableVertex<T>* tailVertex, mapTableVertex<T>* headVertex)
		{
			if (tailVertex == NULL || headVertex == NULL)
				return;
			mapTableEdge<T>* buf = new mapTableEdge<T>;
			buf->tailVertex = tailVertex;
			buf->headVertex = headVertex;
			addEdge(buf);
		} 
		
		//断除关系 
		void destoryEdge(mapTableVertex<T>* tailVertex, mapTableVertex<T>* headVertex)
		{
			if (tailVertex == NULL || headVertex == NULL)
				return;
			mapTableEdge<T>* buf = new mapTableEdge<T>;
			buf->tailVertex = tailVertex;
			buf->headVertex = headVertex;
			removeEdge(buf);
			delete buf;
		}
		
		//显示关系
		void printEdge(mapTableVertex<T>* vertex)
		{
			if (vertex == NULL)
				return; 
			cout<<endl<<"该节点的全部度如下：";
			cout<<endl<<"出度为："; 
			//打印全部出度
			for (mapTableEdge<T>* buf = (*vertex).getFirstOutDegree(); \
				buf != NULL; buf = buf->tailLink) 
				cout<<endl<<(*buf->tailVertex).getData()
					<<" "<<(*buf->headVertex).getData()<<"	";			
			//打印全部入度 
			cout<<endl<<"入度为：";
			for (mapTableEdge<T>* buf = (*vertex).getFirstInDegree(); \
				buf != NULL; buf = buf->headLink) 
				cout<<endl<<(*buf->tailVertex).getData()
					<<" "<<(*buf->headVertex).getData()<<"	";			
		} 
				  
};

//描述顶点的类 
template<typename T>
class mapTableVertex
{
	private:
		T data;
		mapTableEdge<T>* firstInDegree; //入度链表头 
		mapTableEdge<T>* firstOutDegree; //出度链表头 		
	public:
		mapTableVertex()
		{
			this->firstInDegree = NULL;
			this->firstOutDegree = NULL;
		} 
	
		mapTableVertex(T data)
		{
			this->firstInDegree = NULL;
			this->firstOutDegree = NULL;
			this->data = data;
		}
	
		~mapTableVertex()
		{
		}
	
		T getData()
		{
			return this->data;
		}
	
		mapTableEdge<T>* getFirstInDegree()
		{
			return this->firstInDegree;
		}
		
		mapTableEdge<T>* getFirstOutDegree()
		{
			return this->firstOutDegree;
		}
	
		void setFirstInDegree(mapTableEdge<T>* result)
		{
			this->firstInDegree = result;
		}
	
		void setFirstOutDegree(mapTableEdge<T>* result)
		{
			this->firstOutDegree = result;
		}
		
	
		mapTableVertex<T>* createVertex(T data)
		{
			mapTableVertex<T>* buf = new mapTableVertex<T>;
			buf->firstInDegree = NULL;
			buf->firstOutDegree = NULL;
			buf->data = data;			
			return buf;
		} 
		
		void destoryVertex(mapTableVertex<T>* vertex)
		{
			//只能遍历所有出度入度一个个移除 
			//网格状数据，出入度交织在一起了
			mapTableEdge<T>* buf = NULL;
			buf = vertex->firstOutDegree;
			while (buf != NULL) {
				mapTableEdge<T>* temp = buf;
				buf = (*buf).getTailLink();
				(*temp).removeEdge(temp);
			}
					
			buf = vertex->firstInDegree;
			while(buf != NULL) {
				mapTableEdge<T>* temp = buf;
				buf = (*buf).getHeadLink();
				(*temp).removeEdge(temp);	
			}
			
			delete vertex; 
		}	
};




template<typename T>
class map
{
	private:
		int vertexNumber;
		mapTableVertex<T>** vertex;
	public:	
		map()
		{
			this->vertexNumber = 0;
			this->vertex = 0;
		}
	
		//为了简化不搞过于复杂的，如节点调整等 
		map(int num)
		{
			num = num > 0 ? num : 0; 
			this->vertexNumber = num;
			this->vertex = new mapTableVertex<T>*[num];			
		}
		
		~map()
		{
		}
		
		void insertVertex(T x)
		{
			for (int i = 0; i < vertexNumber; i++)
				if (vertex[i] == NULL) {
					vertex[i] = (*vertex[i]).createVertex(x);					
					return;		
				}
			cout<<endl<<"节点满!!!";	
		} 
		
		void removeVertex(T x)
		{
			for (int i = 0; i < vertexNumber; i++)
				if (vertex[i] != NULL && (*vertex[i]).getData() == x) {
					(*vertex[i]).destoryVertex(vertex[i]);
					vertex[i] = NULL;	
				}
			cout<<endl<<"没找到节点!!!";						
		}
		
		void addEdge(T x, T y)
		{
			mapTableEdge<T>* help;
			mapTableVertex<T>* px = NULL;
			mapTableVertex<T>* py = NULL;	
			for (int i = 0; i < vertexNumber; i++)
				if (vertex[i] != NULL && (*vertex[i]).getData() == x)
					px = vertex[i];	
			for (int i = 0; i < vertexNumber; i++)
				if (vertex[i] != NULL && (*vertex[i]).getData() == y)
					py = vertex[i];
			if (px != NULL && py != NULL)
				(*help).createEdge(px, py);			
		}	
		
		void removeEdge(T x, T y)
		{
			mapTableEdge<T>* help;
			mapTableVertex<T>* px = NULL;
			mapTableVertex<T>* py = NULL;	
			for (int i = 0; i < vertexNumber; i++)
				if (vertex[i] != NULL && (*vertex[i]).getData() == x)
					px = vertex[i];	
			for (int i = 0; i < vertexNumber; i++)
				if (vertex[i] != NULL && (*vertex[i]).getData() == y)
					py = vertex[i];				
			(*help).destoryEdge(px, py);				
			
		}
		
		void printEdge()
		{
			mapTableEdge<T>* help;
			for (int i = 0; i < vertexNumber; i++)
				(*help).printEdge(vertex[i]);
						
		}	
};

void testMap()
{
	int n = 10;
	char vertex[n] = {'A','B','C','D','E'};	
	map<int>* aHa = new map<int>(n);
	
	
	(*aHa).insertVertex(0);
	(*aHa).insertVertex(1);
	(*aHa).insertVertex(2);
	(*aHa).insertVertex(3);
	
	(*aHa).addEdge(0,1);
	(*aHa).addEdge(0,2);
	(*aHa).addEdge(2,0);
	(*aHa).addEdge(2,3);
	(*aHa).addEdge(3,0);
	(*aHa).addEdge(3,1);
	(*aHa).addEdge(3,2);
	
	//(*aHa).removeEdge(0,1);
	
	(*aHa).removeVertex(0);
	
	
	(*aHa).printEdge();
	

}

#endif
