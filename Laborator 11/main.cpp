#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include "Profiler.h"

using namespace std;

#define MAX_ROWS 100
#define MAX_COLS 100

/*
 * Am implementat DFS cu structurile din tema anterioara.
 * Pentru topologicalSort am facut o noua functie DFS care adauga in lista ultimele elemente finalizate.
 */

typedef struct
{
	int rows;
	int cols;
	int mat[MAX_ROWS][MAX_COLS];
} Grid;

typedef struct
{
	int row;
	int col;
} Point;

enum
{
	COLOR_WHITE = 0,
	COLOR_GRAY,
	COLOR_BLACK
};

struct Node
{
	int key;
	int adjSize;
	struct Node **adj;

	int color;
	int discovered;
	int finished;
	struct Node *parent;

	int index;
	int lowlink;
	int comp;
	bool onStack;

	Node(int key)
	{
		this->key = key;
		adjSize = 0;
		adj = nullptr;
		color = COLOR_WHITE;
		discovered = 0;
		finished = 0;
		parent = nullptr;
		index = -1;
		lowlink = -1;
		comp = 0;
		onStack = false;
	}
	Node()
	{
	}
};

typedef struct
{
	int nrNodes;
	Node **v;
} Graph;

void dfs_visit(Graph *graph, Node *u, int &time)
{
	time++;
	u->discovered = time;
	u->color = COLOR_GRAY;
	for (int i = 0; i < u->adjSize; i++)
	{
		if (u->adj[i]->color == COLOR_WHITE)
		{
			u->adj[i]->parent = u;
			dfs_visit(graph, u->adj[i], time);
		}
	}
	u->color = COLOR_BLACK;
	time++;
	u->finished = time;
}

void dfs(Graph *graph)
{
	for (int i = 0; i < graph->nrNodes; i++)
	{
		graph->v[i]->color = COLOR_WHITE;
		graph->v[i]->parent = nullptr;
	}
	int time = 0;
	for (int i = 0; i < graph->nrNodes; i++)
	{
		if (graph->v[i]->color == COLOR_WHITE)
			dfs_visit(graph, graph->v[i], time);
	}
}

void dfs_visit_topologicalSort(Graph *graph, Node *u, int &time, list<Node *> *list)
{
	time++;
	u->discovered = time;
	u->color = COLOR_GRAY;
	for (int i = 0; i < u->adjSize; i++)
	{
		if (u->adj[i]->color == COLOR_WHITE)
		{
			u->adj[i]->parent = u;
			dfs_visit_topologicalSort(graph, u->adj[i], time, list);
		}
	}
	u->color = COLOR_BLACK;
	time++;
	u->finished = time;
	list->push_front(u);
}

void dfs_topologicalSort(Graph *graph, list<Node *> *list)
{
	for (int i = 0; i < graph->nrNodes; i++)
	{
		graph->v[i]->color = COLOR_WHITE;
		graph->v[i]->parent = nullptr;
	}
	int time = 0;
	for (int i = 0; i < graph->nrNodes; i++)
	{
		if (graph->v[i]->color == COLOR_WHITE)
			dfs_visit_topologicalSort(graph, graph->v[i], time, list);
	}
}

list<Node *> topologicalSort(Graph *graph)
{
	list<Node *> list;

	dfs_topologicalSort(graph, &list);

	return list;
}

void printAdjacencyList(Graph *graph)
{
	cout << "Adjacency List: \n";
	for (int i = 0; i < graph->nrNodes; i++)
	{
		cout << "Node " << graph->v[i]->key << ": ";
		for (int j = 0; j < graph->v[i]->adjSize; j++)
		{
			cout << graph->v[i]->adj[j]->key << " ";
		}
		cout << endl;
	}
}

void addEdge(Node *u, Node *v)
{
	u->adj[u->adjSize++] = v;
	v->adj[v->adjSize++] = u;
}

void printDFSResultTree(Graph *graph)
{
	cout << "DFS Result Tree:\n";
	for (int i = 0; i < graph->nrNodes; i++)
	{
		Node *currentNode = graph->v[i];
		cout << "Node " << currentNode->key << ": ";
		if (currentNode->parent != nullptr)
		{
			cout << "Parent: " << currentNode->parent->key;
		}
		else
		{
			cout << "Root Node";
		}
		cout << ", Discovery Time: " << currentNode->discovered;
		cout << ", Finish Time: " << currentNode->finished;
		cout << endl;
	}
}

void demo()
{
	Graph graph;
	graph.nrNodes = 6;
	graph.v = new Node *[6];

	for (int i = 0; i < 6; i++)
	{
		graph.v[i] = new Node;
		graph.v[i]->key = i + 1;
		graph.v[i]->adjSize = 0;
		graph.v[i]->adj = new Node *[6];
		graph.v[i]->color = COLOR_WHITE;
		graph.v[i]->discovered = 0;
		graph.v[i]->finished = 0;
		graph.v[i]->parent = nullptr;
	}

	addEdge(graph.v[0], graph.v[1]);
	addEdge(graph.v[0], graph.v[2]);
	addEdge(graph.v[1], graph.v[3]);
	// addEdge(graph.v[2], graph.v[4]);
	addEdge(graph.v[3], graph.v[5]);

	printAdjacencyList(&graph);

	dfs(&graph);

	printDFSResultTree(&graph);

	list<Node *> list = topologicalSort(&graph);

	cout << "Topological sort: \n";
	for (auto node : list)
	{
		cout << node->key << " ";
	}

	for (int i = 0; i < 6; i++)
	{
		delete[] graph.v[i]->adj;
		delete graph.v[i];
	}
	delete[] graph.v;
}

void insertEdges(Graph *graph, int n)
{
	for (int i = 0; i < graph->nrNodes - 1; i++)
	{
		graph->v[i]->adjSize = graph->v[i]->adjSize + 1;
		graph->v[i]->adj = (Node **)realloc(graph->v[i]->adj, graph->v[i]->adjSize * sizeof(Node *));
		graph->v[i]->adj[graph->v[i]->adjSize - 1] = graph->v[i + 1];

		graph->v[i + 1]->adjSize = graph->v[i + 1]->adjSize + 1;
		graph->v[i + 1]->adj = (Node **)realloc(graph->v[i + 1]->adj, graph->v[i + 1]->adjSize * sizeof(Node *));
		graph->v[i + 1]->adj[graph->v[i + 1]->adjSize - 1] = graph->v[i];
	}
	int nrOfEdges = n - graph->nrNodes;
	int i = 0;
	while (i < nrOfEdges)
	{
		bool unique = true;
		int node1 = rand() % graph->nrNodes;
		int node2 = rand() % graph->nrNodes;
		for (int j = 0; j < graph->v[node1]->adjSize; j++)
		{
			if (graph->v[node1]->adj[j] == graph->v[node2])
				unique = false;
		}
		for (int j = 0; j < graph->v[node2]->adjSize; j++)
		{
			if (graph->v[node2]->adj[j] == graph->v[node1])
				unique = false;
		}
		if (node1 == node2)
			unique = false;
		if (unique == true)
		{
			graph->v[node1]->adjSize = graph->v[node1]->adjSize + 1;
			graph->v[node1]->adj = (Node **)realloc(graph->v[node1]->adj, graph->v[node1]->adjSize * sizeof(Node *));
			graph->v[node1]->adj[graph->v[node1]->adjSize - 1] = graph->v[node2];

			graph->v[node2]->adjSize = graph->v[node2]->adjSize + 1;
			graph->v[node2]->adj = (Node **)realloc(graph->v[node2]->adj, graph->v[node2]->adjSize * sizeof(Node *));
			graph->v[node2]->adj[graph->v[node2]->adjSize - 1] = graph->v[node1];

			i++;
		}
	}
}

void free_graph(Graph *graph)
{
	if (graph->v != NULL)
	{
		for (int i = 0; i < graph->nrNodes; ++i)
		{
			if (graph->v[i] != NULL)
			{
				if (graph->v[i]->adj != NULL)
				{
					free(graph->v[i]->adj);
					graph->v[i]->adj = NULL;
				}
				graph->v[i]->adjSize = 0;
				free(graph->v[i]);
				graph->v[i] = NULL;
			}
		}
		free(graph->v);
		graph->v = NULL;
	}
	graph->nrNodes = 0;
}

void dfs_visit(Graph *graph, Node *u, int &time, Operation *op)
{
	op->count(3);
	time++;
	u->discovered = time;
	u->color = COLOR_GRAY;
	for (int i = 0; i < u->adjSize; i++)
	{
		op->count();
		if (u->adj[i]->color == COLOR_WHITE)
		{
			op->count();
			u->adj[i]->parent = u;
			dfs_visit(graph, u->adj[i], time, op);
		}
	}
	op->count(3);
	u->color = COLOR_BLACK;
	time++;
	u->finished = time;
}

void dfs(Graph *graph, Operation *op)
{
	for (int i = 0; i < graph->nrNodes; i++)
	{
		op->count(2);
		graph->v[i]->color = COLOR_WHITE;
		graph->v[i]->parent = nullptr;
	}
	int time = 0;
	op->count();
	for (int i = 0; i < graph->nrNodes; i++)
	{
		op->count();
		if (graph->v[i]->color == COLOR_WHITE)
			dfs_visit(graph, graph->v[i], time, op);
	}
}

void strongConnect(Graph *graph, Node *u, int &index, stack<Node *> &S, int &nrComponents)
{
	u->index = index;
	u->lowlink = index;
	S.push(u);
	u->onStack = true;

	for (int i = 0; i < u->adjSize; i++)
	{
		if (u->adj[i]->index == -1)
		{
			strongConnect(graph, u->adj[i], index, S, nrComponents);
			u->lowlink = min(u->lowlink, u->adj[i]->lowlink);
		}
		else if (u->adj[i]->onStack)
		{
			u->lowlink = min(u->lowlink, u->adj[i]->index);
		}
	}
	if (u->lowlink == u->index)
	{
		nrComponents++;
		cout << "Componenta conexa " << nrComponents << ": ";
		while (!S.empty())
		{
			Node *v = S.top();
			S.pop();
			v->onStack = false;
			v->comp = nrComponents;
			cout << v->key << " ";
			if (v == u)
			{
				cout << endl;
				break;
			}
		}
	}
}

void tarjan(Graph *graph)
{
	int index = 0;
	stack<Node *> S;
	int nrComponents = 0;
	for (int i = 0; i < graph->nrNodes; i++)
	{
		graph->v[i]->index = -1;
		graph->v[i]->lowlink = -1;
		graph->v[i]->onStack = false;
		graph->v[i]->comp = 0;
	}
	for (int i = 0; i < graph->nrNodes; i++)
	{
		if (graph->v[i]->index == -1)
			strongConnect(graph, graph->v[i], index, S, nrComponents);
	}
}

void demo3()
{
	Graph graph;
	graph.nrNodes = 6;
	graph.v = new Node *[6];

	for (int i = 0; i < 6; i++)
	{
		graph.v[i] = new Node;
		graph.v[i]->key = i;
		graph.v[i]->adjSize = 0;
		graph.v[i]->adj = new Node *[6];
		graph.v[i]->color = COLOR_WHITE;
		graph.v[i]->discovered = 0;
		graph.v[i]->finished = 0;
		graph.v[i]->parent = nullptr;
	}

	addEdge(graph.v[0], graph.v[1]);
	addEdge(graph.v[0], graph.v[2]);
	addEdge(graph.v[1], graph.v[3]);
	addEdge(graph.v[2], graph.v[4]);
	addEdge(graph.v[3], graph.v[5]);
	addEdge(graph.v[0], graph.v[5]);
	addEdge(graph.v[0], graph.v[5]);
	addEdge(graph.v[4], graph.v[5]);

	printAdjacencyList(&graph);

	tarjan(&graph);

	for (int i = 0; i < 6; i++)
	{
		delete[] graph.v[i]->adj;
		delete graph.v[i];
	}
	delete[] graph.v;
}

void performance()
{
	int n, i;
	Profiler p("bfs");

	for (n = 1000; n <= 4500; n += 100)
	{
		Operation op = p.createOperation("bfs-edges", n);
		Graph graph;
		graph.nrNodes = 100;
		graph.v = (Node **)malloc(graph.nrNodes * sizeof(Node *));
		for (i = 0; i < graph.nrNodes; ++i)
		{
			graph.v[i] = (Node *)malloc(sizeof(Node));
			memset(graph.v[i], 0, sizeof(Node));
		}
		insertEdges(&graph, n);

		dfs(&graph, &op);
		free_graph(&graph);
	}

	for (n = 100; n <= 200; n += 10)
	{
		Operation op = p.createOperation("bfs-vertices", n);
		Graph graph;
		graph.nrNodes = n;
		graph.v = (Node **)malloc(graph.nrNodes * sizeof(Node *));
		for (i = 0; i < graph.nrNodes; ++i)
		{
			graph.v[i] = (Node *)malloc(sizeof(Node));
			memset(graph.v[i], 0, sizeof(Node));
		}
		insertEdges(&graph, 4500);

		dfs(&graph, &op);
		free_graph(&graph);
	}

	p.showReport();
}

void demo2()
{
	Graph graph;
	graph.nrNodes = 10;
	graph.v = (Node **)malloc(graph.nrNodes * sizeof(Node *));
	for (int i = 0; i < graph.nrNodes; ++i)
	{
		graph.v[i] = new Node(i);
	}
	insertEdges(&graph, 16);
	printAdjacencyList(&graph);
}

int main()
{
	srand(time(nullptr));
	// demo();
	// demo2();
	// demo3();
	performance();
	return 0;
}
