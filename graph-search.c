#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 10 //헤드 포인터 배열 최대 크기
#define FALSE 0
#define TRUE 1

//그래프 노드 구조체 선언
typedef struct graphNode
{
	int vertex;				//정점
	struct graphNode *link; //다음 인접 정점 연결 링크
} graphNode;

//그래프 구조체 선언
typedef struct graphType
{
	int n;							   //정점 개수
	graphNode *headNode[MAX_VERTICES]; //정점에 대한 헤드 포인터 배열
	int visited[MAX_VERTICES];		   //정점 방문 표시
} graphType;

/*******************************************/
// DFS에 쓰이는 스택 구현
typedef int element;

typedef struct stackNode
{
	element data;
	struct stackNode *link;
} stackNode;

stackNode *top;

int isStackEmpty();
void push(element item);
element pop(void);

////////////////////////////////////////////

// BFS에 쓰이는 LinkedQueue 구현
typedef struct QNode
{
	element data;
	struct QNode *link;
} QNode;

typedef struct QType
{
	QNode *front;
	QNode *rear;
} QType;

QType *createLinkedQueue();
int isLQEmpty(QType *LQ);
void enQueue(QType *LQ, element item);
element deQueue(QType *LQ);

/*******************************************/

int initializeGraph(graphType **graph);
int freeGraph(graphType *graph);
int insertVertex(graphType *graph, int v);
int insertEdge(graphType *graph, int v1, int v2);
void printGraph(graphType *graph);
int dfs(graphType *graph, int v);
int bfs(graphType *graph, int v);

int main()
{
	char command;
	int v = 0;
	int v1, v2;
	graphType *graph = NULL;
	printf("\n[----- [JinYeongKim] [2019038030] -----]\n");

	do
	{
		printf("----------------------------------------------------------------\n");
		printf("                       Graph Searches                           \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize Graph     = z                                       \n");
		printf(" Insert Vertex        = v           Insert Edge           = e   \n");
		printf(" Depth First Search   = d           Breath First Search   = b   \n");
		printf(" Print Graph          = p           Quit                  = q   \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch (command)
		{
		case 'z':
		case 'Z':
			initializeGraph(&graph);
			break;
		case 'v':
		case 'V':
			insertVertex(graph, v);
			v++;
			break;
		case 'e':
		case 'E':
			printf("\nEdge = (V1, V2)");
			printf("\nV1, V2 = ");
			scanf("%d %d", &v1, &v2);
			insertEdge(graph, v1, v2);
			break;
		case 'd':
		case 'D':
			printf("\nDFS search : ");
			dfs(graph, 0);
			printf("\n");
			break;
		case 'b':
		case 'B':
			printf("\nBFS search : ");
			bfs(graph, 0);
			printf("\n");
			break;

		case 'p':
		case 'P':
			printGraph(graph);
			break;
		case 'q':
		case 'Q':
			printf("\nFree graph memory...\n\n");
			freeGraph(graph);
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	} while (command != 'q' && command != 'Q');

	return 1;
}

//그래프 생성
int initializeGraph(graphType **graph)
{
	//그래프가 이미 만들어져 있을 때 새로 만들기
	if (*graph != NULL)
		freeGraph(*graph);

	//그래프에 메모리 할당하고 새로운 그래프 만들기
	*graph = (graphType *)malloc(sizeof(graphType));
	(*graph)->n = 0;
	for (int v = 0; v < MAX_VERTICES; v++)
	{
		(*graph)->headNode[v] = NULL;
		(*graph)->visited[v] = FALSE;
	}
	return 1;
}

//그래프 메모리 해제
int freeGraph(graphType *graph)
{
	//해제할 메모리가 없을 때
	if (graph == NULL)
		return -1;

	//메모리 해제
	for (int i = 0; i < MAX_VERTICES; i++)
	{
		free(graph->headNode[i]);
	}
	free(graph);
	return 1;
}

//정점 추가
int insertVertex(graphType *graph, int v)
{
	//그래프 initialize 안 했을 때
	if (graph == NULL)
	{
		printf("\nInitialize first!\n");
		return -1;
	}

	//정점 개수가 최대 개수 10개를 넘을 때
	if ((graph->n) > (MAX_VERTICES - 1))
	{
		printf("\nVertex Excess!\n");
		return -1;
	}

	//정점 추가
	printf("\nInserting Vertex %d...\n", graph->n);
	graph->n++;
	return 1;
}

//간선 (v1, v2) 추가
int insertEdge(graphType *graph, int v1, int v2)
{
	//그래프 initialize 안 했을 때
	if (graph == NULL)
	{
		printf("\nInitialize first!\n");
		return -1;
	}

	//해당 정점이 없을 때
	if (v1 >= graph->n || v2 >= graph->n)
	{
		printf("\nVertex not in graph. Add more vertex.\n");
		return -1;
	}

	//해당 간선이 이미 존재할 때
	graphNode *p = graph->headNode[v1];
	while (p)
	{
		if (p->vertex == v2)
		{
			printf("\nThe edge already exists.\n");
			return -1;
		}
		p = p->link;
	}

	graphNode *node = (graphNode *)malloc(sizeof(graphNode));
	node->vertex = v2;
	node->link = graph->headNode[v1]; //삽입 간선에 대한 노드를 리스트의 첫 번째 노드로 연결
	graph->headNode[v1] = node;
	return 1;
}

//그래프 출력
void printGraph(graphType *graph)
{
	graphNode *p;
	//인접리스트 개수만큼 반복
	for (int i = 0; i < graph->n; i++)
	{
		printf("\nHead Vertex %d", i);
		p = graph->headNode[i];
		while (p)
		{
			printf(" -> %d", p->vertex);
			p = p->link;
		}
		printf("\n");
	}
}

/*********************************************************************/
//그래프 깊이 우선 탐색
int dfs(graphType *graph, int v)
{
	//그래프 initialize 안 됐을 때
	if (graph == NULL)
	{
		printf("\nInitialize first!\n");
		return -1;
	}

	graphNode *p;
	top = NULL;
	push(v);				  //시작 정점 push
	graph->visited[v] = TRUE; //방문한 v 의 배열값 TRUE (마킹)
	printf("%d ", v);

	//스택이 공백 아닐 때 동안 반복
	while (!isStackEmpty())
	{
		p = graph->headNode[v];
		//인접 노드가 있을 때 반복
		while (p)
		{
			//정점 p를 방문하지 않았을 때
			if (!graph->visited[p->vertex])
			{
				push(p->vertex);				  // push
				graph->visited[p->vertex] = TRUE; // visited 마킹
				printf("%d ", p->vertex);
				v = p->vertex;
				p = graph->headNode[v];
			}
			//정점 p를 이미 방문했을 때
			else
				p = p->link;
		}
		//순회를 모두 마쳤을 때 정점 v pop
		v = pop();
	}

	// visited 배열 초기화
	for (int v = 0; v < MAX_VERTICES; v++)
		graph->visited[v] = FALSE;
	return 1;
}

//그래프 너비 우선 탐색
int bfs(graphType *graph, int v)
{
	//그래프 initialize 안 됐을 때
	if (graph == NULL)
	{
		printf("\nInitialize first!\n");
		return -1;
	}

	graphNode *p;
	QType *Q = createLinkedQueue(); //큐 생성
	graph->visited[v] = TRUE;
	printf("%d ", v);
	enQueue(Q, v); //시작 정점 큐에 삽입

	//큐가 공백이 아닐 때 동안 반복
	while (!isLQEmpty(Q))
	{
		v = deQueue(Q);
		//인접 정점이 있는 동안 반복
		for (p = graph->headNode[v]; p; p = p->link)
			//정점 p가 방문하지 않은 정점일 때
			if (!graph->visited[p->vertex])
			{
				graph->visited[p->vertex] = TRUE; //마킹
				printf("%d ", p->vertex);
				enQueue(Q, p->vertex);
			}
	}

	// bfs 후 초기화
	for (int v = 0; v < MAX_VERTICES; v++)
		graph->visited[v] = FALSE;
	return 1;
}

/*********************************************************************/
//스택 구현
int isStackEmpty()
{
	if (top == NULL)
		return TRUE;
	else
		return FALSE;
}

void push(element item)
{
	stackNode *temp = (stackNode *)malloc(sizeof(stackNode));

	temp->data = item;
	temp->link = top;
	top = temp;
}

element pop()
{
	element item;
	stackNode *temp = top;

	if (isStackEmpty())
	{
		printf("\n\nStack is empty!\n");
		return 0;
	}
	else
	{
		item = temp->data;
		top = temp->link;
		free(temp);
		return item;
	}
}

/*********************************************************************/
//링크드큐 구현
QType *createLinkedQueue()
{
	QType *LQ = (QType *)malloc(sizeof(QType));
	LQ->front = NULL;
	LQ->rear = NULL;
	return LQ;
}

int isLQEmpty(QType *LQ)
{
	if (LQ->front == NULL)
		return -1;
	else
		return 0;
}

void enQueue(QType *LQ, element item)
{
	QNode *newNode = (QNode *)malloc(sizeof(QNode));
	newNode->data = item;
	newNode->link = NULL;
	if (LQ->front == NULL)
	{
		LQ->front = newNode;
		LQ->rear = newNode;
	}
	else
	{
		LQ->rear->link = newNode;
		LQ->rear = newNode;
	}
}

element deQueue(QType *LQ)
{
	QNode *node = LQ->front;
	element item;
	if (isLQEmpty(LQ))
		return;
	else
	{
		item = node->data;
		LQ->front = LQ->front->link;
		if (LQ->front == NULL)
			LQ->rear = NULL;
		free(node);
		return item;
	}
}
