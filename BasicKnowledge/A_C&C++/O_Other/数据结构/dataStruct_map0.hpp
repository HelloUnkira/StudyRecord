#ifndef __DATASTRUCT_MAP_HPP_
#define __DATASTRUCT_MAP_HPP_

#include<iostream>

//基于邻接表结构 

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
			//还没有节点入队列 
			if (first == NULL) {
				first = buf;
				last = buf;
			} else 
			//入队列是尾插 
				last->next = buf;				
		}
		
		map<T>* dequeueNode(mapTableQueue* queue)
		{
			//队列中没有节点 
			if (first == NULL)
				return NULL;
			else {
			//出队列是头出 
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
			//删除的话，从头删
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
				//根据权值大小进行排序插入				
				mapTableList* help = list;				
				mapTableList* help_prev = NULL;
				mapTableList* help_next = NULL;
				//往后遍历，找到第一个比weight大的结点 
				while(help != NULL) {
					help_next = help->next;
					if (weight <= help->weight)
						break;
					help_prev = help;						
					help = help->next;					
				}
				//最后一个结点的话 
				if(help == NULL) {			
					help = help_prev;
					help->next = buf;
					buf->prev = help;
					return list;
				} else if (help->prev == NULL) {
				//插入到最开始的话  
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
				//根据标号进行查找				
				mapTableList* help = list;		
				mapTableList* help_prev = NULL;					
				mapTableList* help_next = NULL;					
				while(help != NULL) {
					if ((*(help->member)).getVertex() == (*vertex).getVertex())
						break;
					help = help->next;						
				}
				//最后一个结点都没有 
				if (help == NULL) 
					return list; 
				else {
					help_prev = help->prev;	
					help_next = help->next;	
				}
				//不满足删除条件 
				//仅一个元素且是选定元素		
				delete help;					
				if(help_prev == NULL && help_next == NULL) 
					return NULL;
				else if (help_prev == NULL) {
					//删除头节点
					help_next->prev = NULL;
					return help_next;
				} else if (help_next == NULL) {
					//删除最后一个结点
					help_prev->next = NULL;	
					return list;						
				} else {
					//删除中间节点 
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
					//反复头删，无需调整任何指针
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
	
		//创建一个可以存储n个结点的图 
		map* createMap(int n)
		{
			number = n;
			//构造有n个结点的空数组
			map* buf = new map[n];
			//构造有n个结点的邻接表,清空关系 
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
	
		//载入结点，结点采用先进先载入的规则
		void insertVertex(map* mapSet, map Vertex)
		{
			//无意义的插入 
			if (Vertex.vertex == char(0))
				return;
			//查找图集合中第一个空位置，如果有的话 
			int index = 0; 
			for (index = 0; index < number; index++)
				if (mapSet[index].vertex == char(0))
					break;
			//找不到
			if (index >= number)
				return;
			//插入的新结点重复了 
			for (int i = 0; i < number; i++)
				if (mapSet[i].vertex == Vertex.vertex)
					return;				
			//修改对应结点的数据即可 
			mapSet[index].vertex = Vertex.vertex; 
		} 
			
		//修改结点名称 
		void changeVertex(map* mapSet, map oldVertex, map newVertex)
		{
			//查找图集合中第一个相匹配的位置 
			int index = 0; 
			for (index = 0; index < number; index++)
				if (mapSet[index].vertex == oldVertex.vertex)
					break;
			//空间不足
			if (index >= number)
				return;
			//修改的新结点重复了 
			for (int i = 0; i < number; i++)
				if (mapSet[i].vertex == newVertex.vertex)
					return;

			//修改对应结点的数据即可 			
			mapSet[index].vertex = newVertex.vertex; 															
		}
	
		//建立vertexFrom 到 vertexTo 之间的路径并标识路径长度 
		void addEdgeForVertex(map* mapSet, map vertexFrom, map vertexTo, int weight)
		{
			//无意义的关系建立 
			if (vertexFrom.vertex == char(0) || vertexTo.vertex == char(0) || \
				vertexFrom.vertex == vertexTo.vertex)
				return;
			//查找图集合中VertexFrom 
			int index1 = 0; 
			for (index1 = 0; index1 < number; index1++)
				if (mapSet[index1].vertex == vertexFrom.vertex)
					break;
			//找不到结点 
			if (index1 >= number)
				return; 
			//查找图集合中VertexTo
			int index2 = 0; 
			for (index2 = 0; index2 < number; index2++)
				if (mapSet[index2].vertex == vertexTo.vertex)
					break;
			//找不到结点 
			if (index2 >= number)
				return; 		
						
			//对对应的索引index建立他们的联系即可
			edge[index1] = (**edge).insertNode(edge[index1], &mapSet[index2], weight);				
									
		}
	
		void removeEdgeForVertex(map* mapSet, map vertexFrom, map vertexTo)
		{
			//无意义的关系解除 
			if (vertexFrom.vertex == char(0) || vertexTo.vertex == char(0) || \
				vertexFrom.vertex == vertexTo.vertex)
				return;
			//查找图集合中指定位置，如果有的话 
			int index = 0; 
			for (index = 0; index < number; index++)
				if (mapSet[index].vertex == vertexFrom.vertex)
					break;
			//找不到结点 
			if (index >= number)
				return; 
			else //对对应的索引index删除他们的联系即可
				edge[index] = (**edge).deleteNode(edge[index], &vertexTo);				
													
		} 
	
		int searchEdgeForVertex(map* mapSet, map vertexFrom, map vertexTo) 
		{
			//无意义的关系查找 
			if (vertexFrom.vertex == char(0) || vertexTo.vertex == char(0) || \
				vertexFrom.vertex == vertexTo.vertex)
				return -1;
			//查找图集合中指定位置，如果有的话 
			int index = 0; 
			for (index = 0; index < number; index++)
				if (mapSet[index].vertex == vertexFrom.vertex)
					break;
			//找不到结点 
			if (index >= number)
				return -1; 
			else {
				//对对应的索引index建立他们的联系即可
				mapTableList<T>* buf = (**edge).searchNode(edge[index], &vertexTo);
				if (buf == NULL)
					return -1;
				else 
					return (*buf).getWeight();
								
			}
		}
 
 		int searchMapIndex(map* mapSet, char vertex)
 		{
			//无意义的关系查找 
			if (vertex == char(0))
				return -1;		
			//查找图集合中指定位置，如果有的话 
			int index = 0; 
			for (index = 0; index < number; index++)
				if (mapSet[index].vertex == vertex)
					break;
			//找不到结点 
			if (index >= number)
				return -1;		
			else
				return index;
		}

		void removeVertex(map* mapSet, map Vertex)
		{
			//无意义的删除 
			if (Vertex.vertex == char(0))
				return;		
			//查找图集合中出现的目标节点，如果有的话 
			int index = 0; 
			for (index = 0; index < number; index++)
				if (mapSet[index].vertex == Vertex.vertex)
					break;
			//找不到
			if (index >= number)
				return;		
			//现在，假设所有节点都指向它，它指向所有节点，为它断开节点
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
			//访问了本节点				
			cout<<vertex<<" ";
			edgeHelp[index] = 1;
					
			for (mapTableList<T>* buf = edge[index]; buf != NULL; buf = (*buf).getNext()) {		
				//开始访问下一个可去往的点
				index = searchMapIndex(mapSet, ((*buf).getVertex())->vertex);
				if (index == -1)
					return;
				//如果没有访问，开始访问vertex的链接点
				if (0 == edgeHelp[index])
					depthfirstSort(mapSet, ((*buf).getVertex())->vertex);
					//访问了本邻接点，然后从其下一个邻接点开始继续走			
			} 												
		}
		
		void depthfirstSortMap(map* mapSet, char vertex)
		{
			//重置标签 
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
			//访问了本节点，然后标记本节点，然后将结点入队列
			cout<<vertex<<" ";
			edgeHelp[index] = 1;	
			//首结点入队列									
			(*queue).enqueueNode(queue, &mapSet[index]); 			
			while (!(*queue).isEmpty()) {				
				help = (*queue).dequeueNode(queue);								 
				//开始访问下一个可去往的点
				index = searchMapIndex(mapSet, help->vertex);
				if (index == -1)
					continue;
				//访问邻接节点，并依次入队
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
			//重置标签 
			for (int i = 0; i < number; i++)
		 		edgeHelp[i] = 0;		
			breadthfirstSort(mapSet, vertex);					
		}
			
		void finishDijkstraShortestPath(map* mapSet, char vertex)
		{
			//重置标签，假设一开始，所有最短路径都没有确定
			for (int i = 0; i < number; i++)
		 		edgeHelp[i] = 0;
			//创建最短路径数组，路径长度最大值应该是多少目前不确定 
			//初始化，所有路径应该更新为到起始结点 			
			char path[number][number] = {0};
			for (int i = 0; i < number; i++) {
			 	path[i][0] = vertex;				
				for (int j = 1; j < number; j++)
					path[i][j] = '\0';
			}				  
			//创建最短路径长度数组，并保持为int的最大值 
			int Dijkstra[number] = {0};			
			for (int i = 0; i < number; i++) 
				Dijkstra[i] = 0x7fffffff;
			//先从初始节点开始更新下一级路径，寻找最小值便是当前最短路径		
			int index = searchMapIndex(mapSet, vertex);			
			if (index == -1)
				return;				
			Dijkstra[index] = 0; 
			//保存一次确定中，其最小值及其对应的最短路径	
			int min = 0x7fffffff;			
			int minIndex = index;	
			//因为把初始化和后循环累计到一起，所以需要区分一下，以统一算法
			while (1) {	
				//如果找到了一个需要被确定的节点但该节点未被确定 
				//标记为确定，且在之后，准备确定它
				index = minIndex;
				if (edgeHelp[index] == 0)
					edgeHelp[index] = 1;					
				else {					 
					//从edgeHelp中找一个未使用的下标
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
				//开始确定该节点 
				min = 0x7fffffff;
				for (mapTableList<T>* buf = edge[index]; buf != NULL; buf = (*buf).getNext()) {					 					
					//找到这个节点邻接表个各个数据，获取路径值，获取邻接点对应数组中索引
					int nonius = 1;
					int length = (*buf).getWeight();
					char label = ((*buf).getVertex())->vertex; 														
					int temp = searchMapIndex(mapSet, ((*buf).getVertex())->vertex);
					//找不到结点就退出该次	
					if (temp == -1)
						continue;
					//先更新最小值，如果需要更新的话，初始化的情况，避免min在后面的影响 
					if (length < min) {
							minIndex = temp;
							min = length;
					}
					//更新最短路径数组值,更新最短路径 
					if (Dijkstra[index] + length < Dijkstra[temp]) {				
						Dijkstra[temp] = Dijkstra[index] + length;
						//保留路径,当前 A - degeTO的最短路径被修改成为了 A - degeFrom - degeTo
								int nonius = 0;
								//不需要考虑路径溢出，除非成环否则正常情况不出现重复节点
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
			//重置标签，假设一开始，所有最短路径都没有确定
			for (int i = 0; i < number; i++)
		 		edgeHelp[i] = 0;
			//创建最短路径数组，路径长度最大值应该是多少目前不确定 
			//初始化，所有路径应该更新为到起始结点 			
			char path[number][number] = {0};
			for (int i = 0; i < number; i++) {
			 	path[i][0] = vertex;				
				for (int j = 1; j < number; j++)
					path[i][j] = '\0';
			}				  
			//创建最短路径长度数组，并保持为int的最大值 
			int BellmanFord[number] = {0};			
			for (int i = 0; i < number; i++) 
				BellmanFord[i] = 0x7fffffff;
			//先从初始节点开始更新下一级路径，寻找最小值便是当前最短路径			
			int index = searchMapIndex(mapSet, vertex);			
			if (index == -1)
				return false;	
			BellmanFord[index] = 0;
			//开始探索
			for (int i = 0; i < number; i++) {
				bool isFinished = true;			
				//对每一条边u - v ,如果存在的话
				for (int edgeFrom = 0; edgeFrom < number; edgeFrom++)
					if (mapSet[edgeFrom].vertex != char(0) && 
						edge[edgeFrom] != NULL && BellmanFord[edgeFrom] != 0x7fffffff) {
						for (mapTableList<T>* buf = edge[edgeFrom]; \
							buf != NULL; buf = (*buf).getNext()) {
							char label = ((*buf).getVertex())->vertex;
							int edgeTo = searchMapIndex(mapSet, label);								
							int length = (*buf).getWeight();
							//确定点edgeFrom 到 edgeTo
							if (BellmanFord[edgeFrom] + length < BellmanFord[edgeTo]) {
								BellmanFord[edgeTo] = BellmanFord[edgeFrom] + length;
								isFinished = false;
								//保留路径,当前 A - degeTO的最短路径被修改成为了 A - degeFrom - degeTo
								int nonius = 0;
								//不需要考虑路径溢出，除非成环否则正常情况不出现重复节点
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
						//确定点edgeFrom 到 edgeTo 
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
			//构造临接矩阵
			int Floyd[number][number] = {0};
			//初始化各个边信息
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
			//小心环的出现
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
	
	//这些只是逻辑副本，方便操作使用，本身不属于map集合内
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


