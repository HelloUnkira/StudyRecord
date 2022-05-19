#ifndef __DATASTRUCT_MAP_HPP_
#define __DATASTRUCT_MAP_HPP_

#include<iostream>

//�����ٽӶ��ر�ṹ 

using namespace std;

template<typename T>
class mapTableVertex;
	
template<typename T>
class mapTableEdge
{
	private:
		bool mark;
		mapTableVertex<T>* oneVertex;
		mapTableVertex<T>* anotherVertex;
		mapTableEdge<T>* oneEdge;
		mapTableEdge<T>* anotherEdge;
		int weight;
	public:
		mapTableEdge()
		{
			this->mark = false;
			this->oneVertex = NULL;
			this->anotherVertex = NULL;
			this->oneEdge = NULL;
			this->anotherEdge = NULL;
			this->weight = 1;
		}
	
		~mapTableEdge()
		{
		}
	
		mapTableEdge<T>* getOneEdge()
		{
			return this->oneEdge;
		}
	
		mapTableEdge<T>* getAnotherEdge()
		{
			return this->anotherEdge;
		}
	
		//�����ٽӶ��ر�ͼ������ʹ۲⵽���½���
		//������������ʮ�����������ٽӶ��ر��������
		//���ڽڵ���ֻ���ڵ�����Ĵ洢���������Խڵ�0��Ϊ����
		//	01xx, 03xx, ��һ����������ʱ���� 20xx��Ӧ�Ƚ�����02xx
		//��Ϊ������ͼ���� 20xx��02xx�ȼۣ�����������������
		mapTableEdge<T>* addSomeEdge(mapTableEdge<T>* list, mapTableEdge<T>* newNode)
		{
			if (list == NULL)
				return newNode;
			else {
				mapTableEdge<T>* buf = list;
				//β�弴�ɣ��������� 
				while(1) {
					if ((buf->oneVertex == newNode->oneVertex && \
						buf->anotherVertex == newNode->anotherVertex) || \
						(buf->oneVertex == newNode->anotherVertex && \
						buf->anotherVertex == newNode->oneVertex))
						return list;
					if (buf->oneVertex == newNode->oneVertex || \
						buf->oneVertex == newNode->anotherVertex)
						if (buf->oneEdge != NULL)
							buf = buf->oneEdge;	
						else {
							buf->oneEdge = newNode;
							return list;
						}
					else if (buf->anotherVertex == newNode->anotherVertex || \
						buf->anotherVertex == newNode->anotherVertex)
						if (buf->anotherEdge != NULL)
							buf = buf->anotherEdge;	
						else {
							buf->anotherEdge = newNode;
							return list;
						}
				}
			}					
		}
		
		void addEdge(mapTableEdge<T>* newNode) 
		{		
			(*newNode->oneVertex).setFirstEdge(\
				addSomeEdge((*newNode->oneVertex).getFirstEdge(), newNode));
			(*newNode->anotherVertex).setFirstEdge(\
				addSomeEdge((*newNode->anotherVertex).getFirstEdge(), newNode));
		}
	
		mapTableEdge<T>* removeSomeEdge(mapTableEdge<T>* list, mapTableEdge<T>* newNode, mapTableVertex<T>* vertex)
		{
			//�Ƴ����� 
			if (list == NULL)
				return NULL;
			else if ((list->oneVertex == newNode->oneVertex && \
				list->anotherVertex == newNode->anotherVertex) || \
				(list->oneVertex == newNode->anotherVertex && \
				list->anotherVertex == newNode->oneVertex)) {
				if (vertex == list->oneVertex)
					return list->oneEdge;
				if (vertex == list->anotherVertex)
					return list->anotherEdge;
			} else {
				mapTableEdge<T>* buf_prev = list;
				mapTableEdge<T>* buf = list->oneEdge;
				while (buf != NULL) {					
					if ((buf->oneVertex == newNode->oneVertex && \
						buf->anotherVertex == newNode->anotherVertex) || \
						(buf->oneVertex == newNode->anotherVertex && \
						buf->anotherVertex == newNode->oneVertex)) {
							if (buf_prev->oneEdge == buf) {
								if (buf_prev->oneVertex == buf->oneVertex)								
									buf_prev->oneEdge = buf->oneEdge;
								else if (buf_prev->oneVertex == buf->anotherVertex)
									buf_prev->oneEdge = buf->anotherEdge;								
							} else if (buf_prev->anotherEdge == buf) {
								if (buf_prev->oneVertex == buf->oneVertex)								
									buf_prev->anotherEdge = buf->oneEdge;
								else if (buf_prev->oneVertex == buf->anotherVertex)
									buf_prev->anotherEdge = buf->anotherEdge;								
							}							
							return list;
						}
					if (buf->oneVertex == newNode->oneVertex || \
						buf->oneVertex == newNode->anotherVertex) {
							buf_prev = buf;
							buf = buf->oneEdge;	
					} else if (buf->anotherVertex == newNode->anotherVertex || \
						buf->anotherVertex == newNode->anotherVertex) {
							buf_prev = buf;
							buf = buf->anotherEdge;													
					}
				}			
			}	 		 
		}	
	
		void removeEdge(mapTableEdge<T>* newNode) 
		{
			(*newNode->oneVertex).setFirstEdge(\
				removeSomeEdge((*newNode->oneVertex).getFirstEdge(), newNode, newNode->oneVertex));
			(*newNode->anotherVertex).setFirstEdge(\
				removeSomeEdge((*newNode->anotherVertex).getFirstEdge(), newNode, newNode->anotherVertex));			
			delete newNode;			
		}
	
		//������ϵ 
		void createEdge(mapTableVertex<T>* oneVertex, mapTableVertex<T>* anotherVertex)
		{
			if (oneVertex == NULL || anotherVertex == NULL)
				return;
			mapTableEdge<T>* buf = new mapTableEdge<T>;
			buf->oneVertex = oneVertex;
			buf->anotherVertex = anotherVertex;
			addEdge(buf);
		}
	
		//�ϳ���ϵ 
		void destoryEdge(mapTableVertex<T>* oneVertex, mapTableVertex<T>* anotherVertex)
		{
			if (oneVertex == NULL || anotherVertex == NULL)
				return;
			mapTableEdge<T>* buf = new mapTableEdge<T>;
			buf->oneVertex = oneVertex;
			buf->anotherVertex = anotherVertex;
			removeEdge(buf);
			delete buf;
		}
		
		//��ʾ��ϵ
		void printEdge(mapTableVertex<T>* vertex)
		{
			//Ϊ�˴ﵽ��ȷ��ʣ�Ӧ�ý��������ڵ���ȷ����һ��������λ��
			if (vertex == NULL)
				return; 
			cout<<endl<<"OneOrAnother:"<<(*vertex).getOneOrAnother()
				<<endl<<"�ýڵ��ȫ��������������,";
			mapTableEdge<T>* buf = (*vertex).getFirstEdge();
			while (buf != NULL) {					
				cout<<endl<<(*buf->oneVertex).getData()
					<<" "<<(*buf->anotherVertex).getData()<<"	";	
				if (buf->oneVertex == vertex || \
					buf->oneVertex == vertex)
					buf = buf->oneEdge;	
				else if (buf->anotherVertex == vertex || \
					buf->anotherVertex == vertex)
					buf = buf->anotherEdge;						
			}	
		} 
	
};

template<typename T>
class mapTableVertex
{
	private:
		T data;
		mapTableEdge<T>* firstEdge;
		bool OneOrAnother; // 1��One��0��Another
	public:
		mapTableVertex()
		{
			this->firstEdge = NULL;
			this->OneOrAnother = 1;
		}
		
		mapTableVertex(T data)
		{
			this->firstEdge = NULL;
			this->data = data;
			this->OneOrAnother = 1;
		}
	
		~mapTableVertex()
		{
		}
	
		T getData()
		{
			return this->data;
		}
	
		void setFirstEdge(mapTableEdge<T>* edge)
		{
			this->firstEdge = edge;
		}
	
		mapTableEdge<T>* getFirstEdge()
		{
			return this->firstEdge;
		}
	
		bool getOneOrAnother()
		{
			return this->OneOrAnother;
		}
	
		mapTableVertex<T>* createVertex(T data)
		{
			mapTableVertex<T>* buf = new mapTableVertex<T>;
			buf->firstEdge = NULL;
			buf->data = data;
			buf->OneOrAnother = 1;
			return buf;
		}
	
		void destoryVertex(mapTableVertex<T>* vertex)
		{
			//ֻ�ܱ������г������һ�����Ƴ� 
			//����״���ݣ�����Ƚ�֯��һ����
			mapTableEdge<T>* buf = NULL;
			buf = vertex->firstEdge;
			while (buf != NULL) {
				mapTableEdge<T>* temp = buf;
				buf = (*buf).getOneEdge();
				(*temp).removeEdge(temp);
			}
					
			buf = vertex->firstInDegree;
			while(buf != NULL) {
				mapTableEdge<T>* temp = buf;
				buf = (*buf).getAnotherEdge();
				(*temp).removeEdge(temp);	
			}
			
			delete vertex; 
		}	
		
		void AdjustOneAnother(mapTableVertex<T>* oneVertex, mapTableVertex<T>* anotherVertex)
		{
			if (oneVertex == NULL || anotherVertex == NULL)
				return;
			else {
				//��鿴����������ʲô���ο���һ���������ˣ������ϻ�������ͼ
				mapTableEdge<T>* oneEdge = oneVertex->firstEdge;
				mapTableEdge<T>* anotherEdge = anotherVertex->firstEdge;
				//���ޱߣ�����һ��˳���ż���
				if (oneEdge == NULL && anotherEdge == NULL)
					if (oneVertex->OneOrAnother == 1)
						anotherVertex->OneOrAnother = 0;
					else
						anotherVertex->OneOrAnother = 1;	
				//����һ���ߣ��ο��б�˳���ż���
				else if (oneEdge == NULL && anotherEdge != NULL)
					if (anotherVertex->OneOrAnother == 1)
						oneVertex->OneOrAnother = 0;
					else
						oneVertex->OneOrAnother = 1;
				//��������������һ����ʵ���Ժϲ�
				else if (oneEdge != NULL && anotherEdge == NULL)
					if (oneVertex->OneOrAnother == 1)
						anotherVertex->OneOrAnother = 0;
					else
						anotherVertex->OneOrAnother = 1;
				else if (oneEdge != NULL && anotherEdge != NULL)
					;	//�������Ӧ�������������Ϊ�Ѿ���ǰ�涨���˵�				
			}
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
			mapTableVertex<T>* phelp;
			mapTableVertex<T>* px = NULL;
			mapTableVertex<T>* py = NULL;	
			for (int i = 0; i < vertexNumber; i++)
				if (vertex[i] != NULL && (*vertex[i]).getData() == x)
					px = vertex[i];	
			for (int i = 0; i < vertexNumber; i++)
				if (vertex[i] != NULL && (*vertex[i]).getData() == y)
					py = vertex[i];
			(*phelp).AdjustOneAnother(px,py);
			if (px != NULL && py != NULL)
				(*help).createEdge(px, py);			
		}	
		
		void removeEdge(T x, T y)
		{
			mapTableEdge<T>* help;
			mapTableVertex<T>* phelp;
			mapTableVertex<T>* px = NULL;
			mapTableVertex<T>* py = NULL;	
			for (int i = 0; i < vertexNumber; i++)
				if (vertex[i] != NULL && (*vertex[i]).getData() == x)
					px = vertex[i];	
			for (int i = 0; i < vertexNumber; i++)
				if (vertex[i] != NULL && (*vertex[i]).getData() == y)
					py = vertex[i];				
			(*phelp).AdjustOneAnother(px,py);
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
	char vertex[10] = {'A','B','C','D','E'};	
	map<int>* aHa = new map<int>(n);
	
	
	(*aHa).insertVertex(0);
	(*aHa).insertVertex(1);
	(*aHa).insertVertex(2);
	(*aHa).insertVertex(3);
	(*aHa).insertVertex(4);
	
	(*aHa).addEdge(0,1);
	(*aHa).addEdge(0,3);
	(*aHa).addEdge(2,1);
	(*aHa).addEdge(2,3);
	(*aHa).addEdge(4,1);
	(*aHa).addEdge(2,4);
	
	(*aHa).removeEdge(0,1);
	(*aHa).removeEdge(0,3);
	
	//(*aHa).removeVertex(0);
	
	
	(*aHa).printEdge();
	

}





#endif