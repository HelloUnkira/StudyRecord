#ifndef __DATASTRUCT_MAP_HPP_
#define __DATASTRUCT_MAP_HPP_

#include<iostream>

//�����ڽӱ�ṹ 

using namespace std;

template<typename T>
class map;

template<typename T>
class mapTableQueue
{
	private:
		map<T>* member;
		mapTableQueue* next;
		static mapTableQueue* first;
		static mapTableQueue* last;
	public:
		mapTableQueue()
		{
			this->member = NULL;
			this->next = NULL;
		}
		
		mapTableQueue(map<T>* Vertex)
		{
			this->member = Vertex;
			this->next = NULL;
		}	
			
		~mapTableQueue()
		{
		}
		
		void enqueueNode(mapTableQueue* queue, map<T>* vertex) 
		{
			mapTableQueue* buf = new mapTableQueue(vertex); 
			//��û�нڵ������ 
			if (first == NULL) {
				first = buf;
				last = buf;
			} else 
			//�������β�� 
				last->next = buf;				
		}
		
		map<T>* dequeueNode(mapTableQueue* queue)
		{
			//������û�нڵ� 
			if (first == NULL)
				return NULL;
			else {
			//��������ͷ�� 
				mapTableQueue* buf = first; 
				first = first->next;
				if (first == NULL)
					last = NULL;
				map<T>* result = buf->member;	
				delete buf;
				return result;	
			}				
		}
		
		void destoryQueue(mapTableQueue* queue)
		{
			//ɾ���Ļ�����ͷɾ
			mapTableQueue* help = queue;
			mapTableQueue* helpNext = NULL;
			while (help != NULL) {
				helpNext = help->next;
				delete help;
				help = helpNext;
			} 			
		}		
		
		bool isEmpty()
		{
			if (first == NULL && last == NULL)
				return 1;
			return 0;
		}
		
};

template<typename T>
class mapTableList
{
	private:
		map<T>* member;
		int weight;
		mapTableList *prev, * next;
	public:
		mapTableList()
		{
			this->member = NULL;
			this->weight = 0;
			this->prev = NULL;
			this->next = NULL;
		}
		
		mapTableList(map<T>* Vertex, int Weight)
		{
			this->member = Vertex;
			this->weight = Weight;
			this->prev = NULL;
			this->next = NULL;
		}		
		
		~mapTableList()
		{
		}
		
		map<T>* getVertex()
		{
			return this->member;
		}
		
		int getWeight()
		{
			return this->weight;
		}
		
		mapTableList* getPrev()
		{
			return this->prev; 
		} 
		
		mapTableList* getNext()
		{
			return this->next; 
		}		
		
		mapTableList* insertNode(mapTableList* list, map<T>* vertex, int weight)
		{
			mapTableList* buf = new mapTableList(vertex,weight);	
			if (list == NULL) {
				return buf;
			} else {
				//����Ȩֵ��С�����������				
				mapTableList* help = list;				
				mapTableList* help_prev = NULL;
				mapTableList* help_next = NULL;
				//����������ҵ���һ����weight��Ľ�� 
				while(help != NULL) {
					help_next = help->next;
					if (weight <= help->weight)
						break;
					help_prev = help;						
					help = help->next;					
				}
				//���һ�����Ļ� 
				if(help == NULL) {			
					help = help_prev;
					help->next = buf;
					buf->prev = help;
					return list;
				} else if (help->prev == NULL) {
				//���뵽�ʼ�Ļ�  
					help->prev = buf;
					buf->next = help;
					return buf;
				} else {
					help_prev->next = buf;
					help->prev = buf;						
					buf->prev = help_prev;
					buf->next = help;
					return list;										
				}										
			}																							
		} 
		
		mapTableList* deleteNode(mapTableList* list, map<T>* vertex) 
		{
			if (list == NULL)
				return NULL;
			else {
				//���ݱ�Ž��в���				
				mapTableList* help = list;		
				mapTableList* help_prev = NULL;					
				mapTableList* help_next = NULL;					
				while(help != NULL) {
					if ((*(help->member)).getVertex() == (*vertex).getVertex())
						break;
					help = help->next;						
				}
				//���һ����㶼û�� 
				if (help == NULL) 
					return list; 
				else {
					help_prev = help->prev;	
					help_next = help->next;	
				}
				//������ɾ������ 
				//��һ��Ԫ������ѡ��Ԫ��		
				delete help;					
				if(help_prev == NULL && help_next == NULL) 
					return NULL;
				else if (help_prev == NULL) {
					//ɾ��ͷ�ڵ�
					help_next->prev = NULL;
					return help_next;
				} else if (help_next == NULL) {
					//ɾ�����һ�����
					help_prev->next = NULL;	
					return list;						
				} else {
					//ɾ���м�ڵ� 
					help_prev->next = help_next;
					help_next->prev = help_prev;
					return list; 
				}						
																		
			}			
		}
		
		mapTableList* searchNode(mapTableList* list, map<T>* vertex)
		{
			mapTableList* help = list;
			while (help != NULL) {
				if ((*(help->member)).getVertex() == (*vertex).getVertex())
					return help;
				help = help->next;
			}
			return NULL;
		} 

		void deleteList(mapTableList* list)
		{
			if (list == NULL) 
				return;
			else {				
				mapTableList* help = list;
				mapTableList* help_next = help->next; 				
				while (help != NULL) {
					//����ͷɾ����������κ�ָ��
					delete help;
					help = help_next;
					if (help_next != NULL)
						help_next = help_next->next; 
				}								
			}			
		}
		
		void printList(mapTableList* list)
		{
			mapTableList* help = list;	
			while (help != NULL) {
				cout<<(*(help->member)).getVertex()<<"-"<<help->weight<<" ";
				help = help->next;
			}		
			cout<<endl;
		}
		
};

template<typename T>
class map
{
	private:
		T data;
		char vertex; 
		static mapTableList<T>** edge;
		static bool* edgeHelp;
		static int number;	
		
	public:
		map()
		{
			this->vertex = char(0);
			this->data = T(0);
		}
		
		map(char Vertex)
		{
			this->vertex = Vertex;
			this->data = T(0);
		}
		
		~map()
		{
		}

		char getVertex()
		{
			return this->vertex;
		}
	
		//����һ�����Դ洢n������ͼ 
		map* createMap(int n)
		{
			number = n;
			//������n�����Ŀ�����
			map* buf = new map[n];
			//������n�������ڽӱ�,��չ�ϵ 
			edge = new mapTableList<T>*[n];
			edgeHelp = new bool[n];
			for (int i = 0; i < n; i++) {
				edge[i] = NULL;				
				edgeHelp[i] = 0;
			}
			return buf;
		}
	
		void deleteMap(map* mapSet)
		{
			for (int i = 0; i < number; i++)
				(**edge).deleteList(edge[i]);
			delete [] edge;
			delete [] edgeHelp;
			delete [] mapSet;
		}
	
		//�����㣬�������Ƚ�������Ĺ���
		void insertVertex(map* mapSet, map Vertex)
		{
			//������Ĳ��� 
			if (Vertex.vertex == char(0))
				return;
			//����ͼ�����е�һ����λ�ã�����еĻ� 
			int index = 0; 
			for (index = 0; index < number; index++)
				if (mapSet[index].vertex == char(0))
					break;
			//�Ҳ���
			if (index >= number)
				return;
			//������½���ظ��� 
			for (int i = 0; i < number; i++)
				if (mapSet[i].vertex == Vertex.vertex)
					return;				
			//�޸Ķ�Ӧ�������ݼ��� 
			mapSet[index].vertex = Vertex.vertex; 
		} 
			
		//�޸Ľ������ 
		void changeVertex(map* mapSet, map oldVertex, map newVertex)
		{
			//����ͼ�����е�һ����ƥ���λ�� 
			int index = 0; 
			for (index = 0; index < number; index++)
				if (mapSet[index].vertex == oldVertex.vertex)
					break;
			//�ռ䲻��
			if (index >= number)
				return;
			//�޸ĵ��½���ظ��� 
			for (int i = 0; i < number; i++)
				if (mapSet[i].vertex == newVertex.vertex)
					return;

			//�޸Ķ�Ӧ�������ݼ��� 			
			mapSet[index].vertex = newVertex.vertex; 															
		}
	
		//����vertexFrom �� vertexTo ֮���·������ʶ·������ 
		void addEdgeForVertex(map* mapSet, map vertexFrom, map vertexTo, int weight)
		{
			//������Ĺ�ϵ���� 
			if (vertexFrom.vertex == char(0) || vertexTo.vertex == char(0) || \
				vertexFrom.vertex == vertexTo.vertex)
				return;
			//����ͼ������VertexFrom 
			int index1 = 0; 
			for (index1 = 0; index1 < number; index1++)
				if (mapSet[index1].vertex == vertexFrom.vertex)
					break;
			//�Ҳ������ 
			if (index1 >= number)
				return; 
			//����ͼ������VertexTo
			int index2 = 0; 
			for (index2 = 0; index2 < number; index2++)
				if (mapSet[index2].vertex == vertexTo.vertex)
					break;
			//�Ҳ������ 
			if (index2 >= number)
				return; 		
						
			//�Զ�Ӧ������index�������ǵ���ϵ����
			edge[index1] = (**edge).insertNode(edge[index1], &mapSet[index2], weight);				
									
		}
	
		void removeEdgeForVertex(map* mapSet, map vertexFrom, map vertexTo)
		{
			//������Ĺ�ϵ��� 
			if (vertexFrom.vertex == char(0) || vertexTo.vertex == char(0) || \
				vertexFrom.vertex == vertexTo.vertex)
				return;
			//����ͼ������ָ��λ�ã�����еĻ� 
			int index = 0; 
			for (index = 0; index < number; index++)
				if (mapSet[index].vertex == vertexFrom.vertex)
					break;
			//�Ҳ������ 
			if (index >= number)
				return; 
			else //�Զ�Ӧ������indexɾ�����ǵ���ϵ����
				edge[index] = (**edge).deleteNode(edge[index], &vertexTo);				
													
		} 
	
		int searchEdgeForVertex(map* mapSet, map vertexFrom, map vertexTo) 
		{
			//������Ĺ�ϵ���� 
			if (vertexFrom.vertex == char(0) || vertexTo.vertex == char(0) || \
				vertexFrom.vertex == vertexTo.vertex)
				return -1;
			//����ͼ������ָ��λ�ã�����еĻ� 
			int index = 0; 
			for (index = 0; index < number; index++)
				if (mapSet[index].vertex == vertexFrom.vertex)
					break;
			//�Ҳ������ 
			if (index >= number)
				return -1; 
			else {
				//�Զ�Ӧ������index�������ǵ���ϵ����
				mapTableList<T>* buf = (**edge).searchNode(edge[index], &vertexTo);
				if (buf == NULL)
					return -1;
				else 
					return (*buf).getWeight();
								
			}
		}
 
 		int searchMapIndex(map* mapSet, char vertex)
 		{
			//������Ĺ�ϵ���� 
			if (vertex == char(0))
				return -1;		
			//����ͼ������ָ��λ�ã�����еĻ� 
			int index = 0; 
			for (index = 0; index < number; index++)
				if (mapSet[index].vertex == vertex)
					break;
			//�Ҳ������ 
			if (index >= number)
				return -1;		
			else
				return index;
		}

		void removeVertex(map* mapSet, map Vertex)
		{
			//�������ɾ�� 
			if (Vertex.vertex == char(0))
				return;		
			//����ͼ�����г��ֵ�Ŀ��ڵ㣬����еĻ� 
			int index = 0; 
			for (index = 0; index < number; index++)
				if (mapSet[index].vertex == Vertex.vertex)
					break;
			//�Ҳ���
			if (index >= number)
				return;		
			//���ڣ��������нڵ㶼ָ��������ָ�����нڵ㣬Ϊ���Ͽ��ڵ�
			for (int i = 0; i < number; i++) {
				removeEdgeForVertex(mapSet, mapSet[index], mapSet[i]);
				removeEdgeForVertex(mapSet, mapSet[i], mapSet[index]);
			}
			mapSet[index].vertex = char(0);
		}
		
		void depthfirstSort(map* mapSet, char vertex)
		{
		 	int index = searchMapIndex(mapSet, vertex);
			if (index == -1)
				return;
			//�����˱��ڵ�				
			cout<<vertex<<" ";
			edgeHelp[index] = 1;
					
			for (mapTableList<T>* buf = edge[index]; buf != NULL; buf = (*buf).getNext()) {		
				//��ʼ������һ����ȥ���ĵ�
				index = searchMapIndex(mapSet, ((*buf).getVertex())->vertex);
				if (index == -1)
					return;
				//���û�з��ʣ���ʼ����vertex�����ӵ�
				if (0 == edgeHelp[index])
					depthfirstSort(mapSet, ((*buf).getVertex())->vertex);
					//�����˱��ڽӵ㣬Ȼ�������һ���ڽӵ㿪ʼ������			
			} 												
		}
		
		void depthfirstSortMap(map* mapSet, char vertex)
		{
			//���ñ�ǩ 
			for (int i = 0; i < number; i++)
		 		edgeHelp[i] = 0;			
			depthfirstSort(mapSet, vertex);
		}

		void breadthfirstSort(map* mapSet, char vertex)
		{
			mapTableQueue<T>* queue = NULL;
			map* help = NULL;
		 	int index = searchMapIndex(mapSet, vertex);
			if (index == -1)
				return;
			mapTableList<T>* buf = edge[index];	
			//�����˱��ڵ㣬Ȼ���Ǳ��ڵ㣬Ȼ�󽫽�������
			cout<<vertex<<" ";
			edgeHelp[index] = 1;	
			//�׽�������									
			(*queue).enqueueNode(queue, &mapSet[index]); 			
			while (!(*queue).isEmpty()) {				
				help = (*queue).dequeueNode(queue);								 
				//��ʼ������һ����ȥ���ĵ�
				index = searchMapIndex(mapSet, help->vertex);
				if (index == -1)
					continue;
				//�����ڽӽڵ㣬���������
				for (mapTableList<T>* buf = edge[index]; buf != NULL; buf = (*buf).getNext()) {	
					int point = searchMapIndex(mapSet, (*buf).getVertex()->vertex);	
					if (0 == edgeHelp[point]) {
						cout<<(*buf).getVertex()->vertex<<" ";						
						edgeHelp[point] = 1;
					}
					(*queue).enqueueNode(queue, (*buf).getVertex());
				}						
			}					
		}		

		void breadthfirstSortMap(map* mapSet, char vertex)
		{			
			//���ñ�ǩ 
			for (int i = 0; i < number; i++)
		 		edgeHelp[i] = 0;		
			breadthfirstSort(mapSet, vertex);					
		}
			
		void finishDijkstraShortestPath(map* mapSet, char vertex)
		{
			//���ñ�ǩ������һ��ʼ���������·����û��ȷ��
			for (int i = 0; i < number; i++)
		 		edgeHelp[i] = 0;
			//�������·�����飬·���������ֵӦ���Ƕ���Ŀǰ��ȷ�� 
			//��ʼ��������·��Ӧ�ø���Ϊ����ʼ��� 			
			char path[number][number] = {0};
			for (int i = 0; i < number; i++) {
			 	path[i][0] = vertex;				
				for (int j = 1; j < number; j++)
					path[i][j] = '\0';
			}				  
			//�������·���������飬������Ϊint�����ֵ 
			int Dijkstra[number] = {0};			
			for (int i = 0; i < number; i++) 
				Dijkstra[i] = 0x7fffffff;
			//�ȴӳ�ʼ�ڵ㿪ʼ������һ��·����Ѱ����Сֵ���ǵ�ǰ���·��		
			int index = searchMapIndex(mapSet, vertex);			
			if (index == -1)
				return;				
			Dijkstra[index] = 0; 
			//����һ��ȷ���У�����Сֵ�����Ӧ�����·��	
			int min = 0x7fffffff;			
			int minIndex = index;	
			//��Ϊ�ѳ�ʼ���ͺ�ѭ���ۼƵ�һ��������Ҫ����һ�£���ͳһ�㷨
			while (1) {	
				//����ҵ���һ����Ҫ��ȷ���Ľڵ㵫�ýڵ�δ��ȷ�� 
				//���Ϊȷ��������֮��׼��ȷ����
				index = minIndex;
				if (edgeHelp[index] == 0)
					edgeHelp[index] = 1;					
				else {					 
					//��edgeHelp����һ��δʹ�õ��±�
					int find = 0;
					for (find = 0; find < number; find++)
						if (edgeHelp[find] == 0)
							break;
					if (find >= number)
						break; 
					index = find;
					minIndex = find;
					edgeHelp[find] = 1;							
				}								
				//��ʼȷ���ýڵ� 
				min = 0x7fffffff;
				for (mapTableList<T>* buf = edge[index]; buf != NULL; buf = (*buf).getNext()) {					 					
					//�ҵ�����ڵ��ڽӱ���������ݣ���ȡ·��ֵ����ȡ�ڽӵ��Ӧ����������
					int nonius = 1;
					int length = (*buf).getWeight();
					char label = ((*buf).getVertex())->vertex; 														
					int temp = searchMapIndex(mapSet, ((*buf).getVertex())->vertex);
					//�Ҳ��������˳��ô�	
					if (temp == -1)
						continue;
					//�ȸ�����Сֵ�������Ҫ���µĻ�����ʼ�������������min�ں����Ӱ�� 
					if (length < min) {
							minIndex = temp;
							min = length;
					}
					//�������·������ֵ,�������·�� 
					if (Dijkstra[index] + length < Dijkstra[temp]) {				
						Dijkstra[temp] = Dijkstra[index] + length;
						//����·��,��ǰ A - degeTO�����·�����޸ĳ�Ϊ�� A - degeFrom - degeTo
								int nonius = 0;
								//����Ҫ����·����������ǳɻ�������������������ظ��ڵ�
								while (path[index][++nonius] != '\0')
									path[temp][nonius] = path[index][nonius];
								path[temp][nonius++] = label;
								if (nonius < number)
									path[temp][nonius] = '\0';																														
					}
				}													
			}	
							
			for (int i = 0 ; i < number; i++) {			
				if (Dijkstra[i] != 0x7fffffff) {
					cout<<":"<<mapSet[i].vertex;		
					cout<<Dijkstra[i]<<" "<<endl;
					if (Dijkstra[i] != 0) {
						for (int j = 0; j < number; j++)						
							cout<<path[i][j]<<" ";
					cout<<endl;													
					}
				}								
			}
			cout<<endl; 			
		}	
			
		bool finishBellmanFordPath(map* mapSet, char vertex)
		{
			//���ñ�ǩ������һ��ʼ���������·����û��ȷ��
			for (int i = 0; i < number; i++)
		 		edgeHelp[i] = 0;
			//�������·�����飬·���������ֵӦ���Ƕ���Ŀǰ��ȷ�� 
			//��ʼ��������·��Ӧ�ø���Ϊ����ʼ��� 			
			char path[number][number] = {0};
			for (int i = 0; i < number; i++) {
			 	path[i][0] = vertex;				
				for (int j = 1; j < number; j++)
					path[i][j] = '\0';
			}				  
			//�������·���������飬������Ϊint�����ֵ 
			int BellmanFord[number] = {0};			
			for (int i = 0; i < number; i++) 
				BellmanFord[i] = 0x7fffffff;
			//�ȴӳ�ʼ�ڵ㿪ʼ������һ��·����Ѱ����Сֵ���ǵ�ǰ���·��			
			int index = searchMapIndex(mapSet, vertex);			
			if (index == -1)
				return false;	
			BellmanFord[index] = 0;
			//��ʼ̽��
			for (int i = 0; i < number; i++) {
				bool isFinished = true;			
				//��ÿһ����u - v ,������ڵĻ�
				for (int edgeFrom = 0; edgeFrom < number; edgeFrom++)
					if (mapSet[edgeFrom].vertex != char(0) && 
						edge[edgeFrom] != NULL && BellmanFord[edgeFrom] != 0x7fffffff) {
						for (mapTableList<T>* buf = edge[edgeFrom]; \
							buf != NULL; buf = (*buf).getNext()) {
							char label = ((*buf).getVertex())->vertex;
							int edgeTo = searchMapIndex(mapSet, label);								
							int length = (*buf).getWeight();
							//ȷ����edgeFrom �� edgeTo
							if (BellmanFord[edgeFrom] + length < BellmanFord[edgeTo]) {
								BellmanFord[edgeTo] = BellmanFord[edgeFrom] + length;
								isFinished = false;
								//����·��,��ǰ A - degeTO�����·�����޸ĳ�Ϊ�� A - degeFrom - degeTo
								int nonius = 0;
								//����Ҫ����·����������ǳɻ�������������������ظ��ڵ�
								while (path[edgeFrom][++nonius] != '\0')
									path[edgeTo][nonius] = path[edgeFrom][nonius];
								path[edgeTo][nonius++] = label;
								if (nonius < number)
									path[edgeTo][nonius] = '\0';
							}								
						}	
					}			
				if (isFinished == true)	
					break;
			}
				
			for (int edgeFrom = 0; edgeFrom < number; edgeFrom++) {
				if (mapSet[edgeFrom].vertex != char(0) && 
					edge[edgeFrom] == NULL) {
					for (mapTableList<T>* buf = edge[edgeFrom]; \
						buf != NULL; buf = (*buf).getNext()) {
						char edgeTo = ((*buf).getVertex())->vertex; 								
						int length = (*buf).getWeight();
						//ȷ����edgeFrom �� edgeTo 
						if (BellmanFord[edgeFrom] + length < BellmanFord[edgeTo]) {
							cout<<"find loop\n";
							return false;
						}								
					}	
				}			
			}				
								
			for (int i = 0 ; i < number; i++) {			
				if (BellmanFord[i] != 0x7fffffff) {
					cout<<":"<<mapSet[i].vertex;		
					cout<<BellmanFord[i]<<" "<<endl;
					 if (BellmanFord[i] != 0) {
						for (int j = 0; j < number; j++)						
							cout<<path[i][j]<<" ";
					 cout<<endl;													
					}
				}								
			}
			cout<<endl; 
			return true;
		}	
			
		void finishFloydShortestPath(map* mapSet)
		{
			//�����ٽӾ���
			int Floyd[number][number] = {0};
			//��ʼ����������Ϣ
			for (int i = 0; i < number; i++)
				for (int j = 0; j < number; j++) {
					int path = searchEdgeForVertex(mapSet, mapSet[i], mapSet[j]);
					if (path == -1)
						Floyd[i][j] = 0x7fffffff;
					else
						Floyd[i][j] = path;				
				}				
			
			for (int k = 0; k < number; k++) 
				for (int i = 0; i < number; i++) {
					if (i == k)
						continue;
					for (int j = 0; j < number; j++) {
						if (k == j || i == j)
							continue;
						int path_i_k = searchEdgeForVertex(mapSet, mapSet[i], mapSet[k]);
						int path_k_j = searchEdgeForVertex(mapSet, mapSet[k], mapSet[j]);
						if (path_i_k == -1 || path_k_j == -1)
							continue;
						else if (Floyd[i][j] > path_i_k + path_k_j)
							Floyd[i][j] = path_i_k + path_k_j;					
					}
				}			
					
			for (int i = 0; i < number; i++) {
				for (int j = 0; j < number; j++) 
					if (Floyd[i][j] == 0x7fffffff)
						cout<<"-"<<"\t";
					else
						cout<<Floyd[i][j]<<"\t";
				cout<<endl;
			}
		}

		void finishTopologicalSort(map* mapSet)
		{
			//С�Ļ��ĳ���
			char* sortResult = new char[number];
			int current_index = 0;
			int current_num = 0;
			for (int index = 0; index < number; index++)
				if (mapSet[index].vertex == char(0))
					current_num++;
		
			while (current_num < number) 
				for (int index = 0; index < number; index++) {
					if (mapSet[index].vertex != char(0) && 
						edge[index] == NULL) {
						sortResult[current_index++] = \
							mapSet[index].vertex;
						removeVertex(mapSet, mapSet[index]);
						current_num++;
					}
				}
			for (int index = 0; index < current_index; index++)
				cout<<sortResult[index]<<" ";
			cout<<endl;		
		}
		
		void printVertex(map * mapSet)
		{
			cout<<endl<<endl;
			for (int i = 0; i< number; i++) {
				if (mapSet[i].vertex == char(0))
					continue;
				cout<<mapSet[i].vertex<<endl;
				(**edge).printList(edge[i]);			
			} 
		}	
 	
};

template<typename T>
	mapTableQueue<T>* mapTableQueue<T>::first = NULL;
template<typename T>
	mapTableQueue<T>* mapTableQueue<T>::last = NULL;	

template<typename T>
	mapTableList<T>** map<T>::edge = NULL;
template<typename T>
	bool* map<T>::edgeHelp = NULL;
template<typename T> 
	int map<T>::number = 0;



void testMap()
{
	int n = 10;
	char vertex[n] = {'A','B','C','D','E'};
	map<int>* aHa = (*aHa).createMap(n);
	
	for (int i = 0; i < n; i++) {
		map<int> help(vertex[i]);
		(*aHa).insertVertex(aHa, help);		
	}
	
	//��Щֻ���߼��������������ʹ�ã���������map������
	map<int> help1('A');	
	map<int> help2('B');
	map<int> help3('C');	
	map<int> help4('D');
	map<int> help5('E');		
	
	(*aHa).addEdgeForVertex(aHa, help1, help2, 5);
	(*aHa).addEdgeForVertex(aHa, help1, help4, 1);			
	(*aHa).addEdgeForVertex(aHa, help2, help3, 1);						
	(*aHa).addEdgeForVertex(aHa, help2, help4, 7);	
	(*aHa).addEdgeForVertex(aHa, help4, help3, 4);	
	(*aHa).addEdgeForVertex(aHa, help4, help5, 4);
	(*aHa).addEdgeForVertex(aHa, help3, help5, 4);
					
				
					
	//(*aHa).removeEdgeForVertex(aHa, help1, help4);
	//(*aHa).removeEdgeForVertex(aHa, help5, help1);	
	
	//cout<<(*aHa).searchEdgeForVertex(aHa, help3, help2)<<endl;
		
	//(*aHa).depthfirstSortMap(aHa, 'A');	
	
	//(*aHa).breadthfirstSortMap(aHa, 'A');
	
	//cout<<(*aHa).finishBellmanFordPath(aHa, 'A');
	
	(*aHa).finishDijkstraShortestPath(aHa, 'A');
	
	//(*aHa).finishFloydShortestPath(aHa);
	
	//(*aHa).finishTopologicalSort(aHa);
		
	(*aHa).printVertex(aHa);
	
	(*aHa).deleteMap(aHa);
}


















#endif


