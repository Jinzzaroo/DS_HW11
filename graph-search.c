#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 10
#define FALSE 0
#define TRUE 1
short int visited[MAX_VERTICES];

typedef struct graphNode
{
	int vertex;
	struct graphNode* link;
} graphNode;

typedef struct graphType
{
	int n;
	graphNode* headNode[MAX_VERTICES];
} graphType;

int initializeGraph(graphType** graph);
int freeGraph(graphType* graph);
int insertVertex(graphType* graph, int v);
int insertEdge(graphType* graph, int v1, int v2);
void printGraph(graphType* graph);
int dfs(graphType* graph, int v);

int main()
{
	char command;
	int v = 0;
	int v1, v2;
	graphType* graph = NULL;
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



int initializeGraph(graphType** graph)
{
	if (*graph != NULL)
		freeGraph(*graph);
	*graph = (graphType*)malloc(sizeof(graphType));

	(*graph)->n = 0;
	for (int v = 0; v < MAX_VERTICES; v++)
		(*graph)->headNode[v] = NULL;
	return 1;
}

int freeGraph(graphType* graph)
{
	if (graph == NULL)
		return -1;

	for (int i = 0; i < MAX_VERTICES; i++)
	{
		free(graph->headNode[i]);
	}
	free(graph);
	return 1;
}

int insertVertex(graphType* graph, int v)
{
	if (graph == NULL)
	{
		printf("\nInitialize first!\n");
		return -1;
	}

	if ((graph->n) > (MAX_VERTICES - 1))
	{
		printf("\nVertex Excess!\n");
		return -1;
	}
	printf("\nInserting Vertex %d...\n", graph->n);
	graph->n++;
	return 1;
}

int insertEdge(graphType* graph, int v1, int v2)
{
	if (graph == NULL)
	{
		printf("\nInitialize first!\n");
		return -1;
	}



	graphNode* node = (graphNode*)malloc(sizeof(graphNode));
	if (v1 >= graph->n || v2 >= graph->n)
	{
		printf("\nVertex not in graph. Add more vertex.\n");
		return -1;
	}

	node->vertex = v2;
	node->link = graph->headNode[v1];
	graph->headNode[v1] = node;
	return 1;
}

void printGraph(graphType* graph)
{
	graphNode* p;
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

int dfs(graphType* graph, int v)
{
	if (graph == NULL)
	{
		printf("\nInitialize first!\n");
		return -1;
	}

	graphNode* p = NULL;
	visited[v] = TRUE;
	printf("%d ", v);
	for (p = graph->headNode[v]; p != NULL; p = p->link)
	{
		if (!visited[p->vertex])
			dfs(graph, p->vertex);
	}
	return 1;
}
