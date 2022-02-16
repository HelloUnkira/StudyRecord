#ifndef __DATASTRUCT_MAP_HPP_
#define __DATASTRUCT_MAP_HPP_

#include<iostream>

//����ʮ������ṹ 

using namespace std;

template<typename T>
class mapTableVertex;

//�����ڵ���࣬�����Ǳ� 
template<typename T>
class mapTableEdge
{
	private: 
		mapTableVertex<T>* tailVertex; //���ķ����ߣ�������ʱ	
		mapTableVertex<T>* headVertex; //���Ľ����ߣ������ʱ 
		mapTableEdge<T>* headLink; //����һ��ʱ���ӱ� 
		mapTableEdge<T>* tailLink; //���һ��ʱ���ӱ� 
		int weight; //Ȩֵ�����أ�Ĭ�����ﲻʹ�� 	
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
		
		//����һ�����ȼ��ڣ�����ʼ�ڵ㽨��һ���³��ȣ��Խ����ڵ㽨��һ�������
		mapTableEdge<T>* addOutDegree(mapTableEdge<T>* list, mapTableEdge<T>* newNode)
		{
			if (list == NULL)
				return newNode;
			else {
				mapTableEdge<T>* buf = list;
				//β�弴�ɣ��������� 
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
				//β�弴�ɣ��������� 
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
			//��ӳ���
			(*newNode->tailVertex).setFirstOutDegree(\
				addOutDegree((*newNode->tailVertex).getFirstOutDegree(), newNode));
			//������
			(*newNode->headVertex).setFirstInDegree(\
				addInDegree((*newNode->headVertex).getFirstInDegree(), newNode));
		}
		
		//�Ƴ�һ�����ڵ�ȼ��ڶϿ�����Ӧ����Ⱥͳ��ȣ���Ҫ��ϵ����ָ��ýڵ�Ķ����ǽڵ�ָ��ȥ�� 
		mapTableEdge<T>* removeOutDegree(mapTableEdge<T>* list, mapTableEdge<T>* newNode)
		{
			//�Ƴ����� 
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
			//�Ƴ����� 
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
			//�Ƴ�����
			(*newNode->tailVertex).setFirstOutDegree(\
				removeOutDegree((*newNode->tailVertex).getFirstOutDegree(), newNode));
			//�Ƴ����
			(*newNode->headVertex).setFirstInDegree(\
				removeInDegree((*newNode->headVertex).getFirstInDegree(), newNode));
			delete newNode;			
		}
		
		//������ϵ 
		void createEdge(mapTableVertex<T>* tailVertex, mapTableVertex<T>* headVertex)
		{
			if (tailVertex == NULL || headVertex == NULL)
				return;
			mapTableEdge<T>* buf = new mapTableEdge<T>;
			buf->tailVertex = tailVertex;
			buf->headVertex = headVertex;
			addEdge(buf);
		} 
		
		//�ϳ���ϵ 
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
		
		//��ʾ��ϵ
		void printEdge(mapTableVertex<T>* vertex)
		{
			if (vertex == NULL)
				return; 
			cout<<endl<<"�ýڵ��ȫ�������£�";
			cout<<endl<<"����Ϊ��"; 
			//��ӡȫ������
			for (mapTableEdge<T>* buf = (*vertex).getFirstOutDegree(); \
				buf != NULL; buf = buf->tailLink) 
				cout<<endl<<(*buf->tailVertex).getData()
					<<" "<<(*buf->headVertex).getData()<<"	";			
			//��ӡȫ����� 
			cout<<endl<<"���Ϊ��";
			for (mapTableEdge<T>* buf = (*vertex).getFirstInDegree(); \
				buf != NULL; buf = buf->headLink) 
				cout<<endl<<(*buf->tailVertex).getData()
					<<" "<<(*buf->headVertex).getData()<<"	";			
		} 
				  
};

//����������� 
template<typename T>
class mapTableVertex
{
	private:
		T data;
		mapTableEdge<T>* firstInDegree; //�������ͷ 
		mapTableEdge<T>* firstOutDegree; //��������ͷ 		
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
			//ֻ�ܱ������г������һ�����Ƴ� 
			//����״���ݣ�����Ƚ�֯��һ����
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
	
		//Ϊ�˼򻯲�����ڸ��ӵģ���ڵ������ 
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
			cout<<endl<<"�ڵ���!!!";	
		} 
		
		void removeVertex(T x)
		{
			for (int i = 0; i < vertexNumber; i++)
				if (vertex[i] != NULL && (*vertex[i]).getData() == x) {
					(*vertex[i]).destoryVertex(vertex[i]);
					vertex[i] = NULL;	
				}
			cout<<endl<<"û�ҵ��ڵ�!!!";						
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
