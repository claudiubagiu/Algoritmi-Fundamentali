#include <stdlib.h>
#include <string.h>
#include <queue>
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include "bfs.h"

using namespace std;

/*
 * Pentru functia get_neighbors am verificat daca vecinii se afla in interiorul grid-ului si daca nu sunt ziduri.
 *
 * Functia bfs exploreaza toate nodurile posibile. Pentru fiecare nod, toti vecinii se marcheaza ca vizitati. Cand nu mai sunt vecini de vizitat, functia se opreste.
 *
 * Functia shortest_path cauta cel mai scurt drum de la un nod la altul. Am folosit 3 structuri de date:
 *	visited: cu acesta verificam daca elementele au fost vizitate;
 *	q: o coada pentru a ne folosi de bfs;
 *	parent: folosim unordered map pentru a stoca parintele pentru fiecare nod, care ne ajuta sa reconstruim shortest_path.
 * Daca start = end, vom reconstrui shortest_path cu ajutorul unordered map-ului. Altfel, cat exista vecini, ii vom vizita pe toti.
 *
 * Pentru a arata ca un cal poate ajunge pe orice pozitie a unei table de sah goale, vom rescrie functia get_neighbors. Acum, vecinii vor fi de fapt toate mutarile posibile ale unui cal pe tabla de sah.
 */

bool isInsideGrid(const Grid *grid, int row, int col)
{
	if (row < 0 || col < 0 || row >= grid->rows || col >= grid->cols)
		return 0;
	return 1;
}

int get_neighbors(const Grid *grid, Point p, Point neighb[])
{
	// TODO: fill the array neighb with the neighbors of the point p and return the number of neighbors
	// the point p will have at most 4 neighbors (up, down, left, right)
	// avoid the neighbors that are outside the grid limits or fall into a wall
	// note: the size of the array neighb is guaranteed to be at least 4
	int nrOfNeighbors = 0;
	if (isInsideGrid(grid, p.row, p.col) == 0)
		return 0;
	if (grid->mat[p.row][p.col] != 0)
		return 0;
	if (isInsideGrid(grid, p.row - 1, p.col) == 1 && grid->mat[p.row - 1][p.col] == 0)
	{
		neighb[nrOfNeighbors].row = p.row - 1;
		neighb[nrOfNeighbors].col = p.col;
		nrOfNeighbors++;
	}
	if (isInsideGrid(grid, p.row, p.col + 1) == 1 && grid->mat[p.row][p.col + 1] == 0)
	{
		neighb[nrOfNeighbors].row = p.row;
		neighb[nrOfNeighbors].col = p.col + 1;
		nrOfNeighbors++;
	}
	if (isInsideGrid(grid, p.row + 1, p.col) == 1 && grid->mat[p.row + 1][p.col] == 0)
	{
		neighb[nrOfNeighbors].row = p.row + 1;
		neighb[nrOfNeighbors].col = p.col;
		nrOfNeighbors++;
	}
	if (isInsideGrid(grid, p.row, p.col - 1) == 1 && grid->mat[p.row][p.col - 1] == 0)
	{
		neighb[nrOfNeighbors].row = p.row;
		neighb[nrOfNeighbors].col = p.col - 1;
		nrOfNeighbors++;
	}
	return nrOfNeighbors;
}

// int get_neighbors(const Grid* grid, Point p, Point neighb[])
//{
//	// Bonus: Unde poate ajunge un cal pe tabla?
//	int nrOfNeighbors = 0;
//	if (isInsideGrid(grid, p.row, p.col) == 0)
//		return 0;
//	if (grid->mat[p.row][p.col] != 0)
//		return 0;
//	if (isInsideGrid(grid, p.row - 1, p.col - 2) == 1 && grid->mat[p.row - 1][p.col - 2] == 0)
//	{
//		neighb[nrOfNeighbors].row = p.row - 1;
//		neighb[nrOfNeighbors].col = p.col - 2;
//		nrOfNeighbors++;
//	}
//	if (isInsideGrid(grid, p.row - 1, p.col + 2) == 1 && grid->mat[p.row - 1][p.col + 2] == 0)
//	{
//		neighb[nrOfNeighbors].row = p.row - 1;
//		neighb[nrOfNeighbors].col = p.col + 2;
//		nrOfNeighbors++;
//	}
//	if (isInsideGrid(grid, p.row + 1, p.col - 2) == 1 && grid->mat[p.row + 1][p.col - 2] == 0)
//	{
//		neighb[nrOfNeighbors].row = p.row + 1;
//		neighb[nrOfNeighbors].col = p.col - 2;
//		nrOfNeighbors++;
//	}
//	if (isInsideGrid(grid, p.row + 1, p.col + 2) == 1 && grid->mat[p.row + 1][p.col + 2] == 0)
//	{
//		neighb[nrOfNeighbors].row = p.row + 1;
//		neighb[nrOfNeighbors].col = p.col + 2;
//		nrOfNeighbors++;
//	}
//	if (isInsideGrid(grid, p.row - 2, p.col + 1) == 1 && grid->mat[p.row - 2][p.col + 1] == 0)
//	{
//		neighb[nrOfNeighbors].row = p.row - 2;
//		neighb[nrOfNeighbors].col = p.col + 1;
//		nrOfNeighbors++;
//	}
//	if (isInsideGrid(grid, p.row - 2, p.col - 1) == 1 && grid->mat[p.row - 2][p.col - 1] == 0)
//	{
//		neighb[nrOfNeighbors].row = p.row - 2;
//		neighb[nrOfNeighbors].col = p.col - 1;
//		nrOfNeighbors++;
//	}
//	if (isInsideGrid(grid, p.row + 2, p.col - 1) == 1 && grid->mat[p.row + 2][p.col - 1] == 0)
//	{
//		neighb[nrOfNeighbors].row = p.row + 2;
//		neighb[nrOfNeighbors].col = p.col - 1;
//		nrOfNeighbors++;
//	}
//	if (isInsideGrid(grid, p.row + 2, p.col + 1) == 1 && grid->mat[p.row + 2][p.col + 1] == 0)
//	{
//		neighb[nrOfNeighbors].row = p.row + 2;
//		neighb[nrOfNeighbors].col = p.col + 1;
//		nrOfNeighbors++;
//	}
//
//	return nrOfNeighbors;
// }

void grid_to_graph(const Grid *grid, Graph *graph)
{
	// we need to keep the nodes in a matrix, so we can easily refer to a position in the grid
	Node *nodes[MAX_ROWS][MAX_COLS];
	int i, j, k;
	Point neighb[8];

	// compute how many nodes we have and allocate each node
	graph->nrNodes = 0;
	for (i = 0; i < grid->rows; ++i)
	{
		for (j = 0; j < grid->cols; ++j)
		{
			if (grid->mat[i][j] == 0)
			{
				nodes[i][j] = (Node *)malloc(sizeof(Node));
				memset(nodes[i][j], 0, sizeof(Node)); // initialize all fields with 0/NULL
				nodes[i][j]->position.row = i;
				nodes[i][j]->position.col = j;
				++graph->nrNodes;
			}
			else
			{
				nodes[i][j] = NULL;
			}
		}
	}
	graph->v = (Node **)malloc(graph->nrNodes * sizeof(Node *));
	k = 0;
	for (i = 0; i < grid->rows; ++i)
	{
		for (j = 0; j < grid->cols; ++j)
		{
			if (nodes[i][j] != NULL)
			{
				graph->v[k++] = nodes[i][j];
			}
		}
	}

	// compute the adjacency list for each node
	for (i = 0; i < graph->nrNodes; ++i)
	{
		graph->v[i]->adjSize = get_neighbors(grid, graph->v[i]->position, neighb);
		if (graph->v[i]->adjSize != 0)
		{
			graph->v[i]->adj = (Node **)malloc(graph->v[i]->adjSize * sizeof(Node *));
			k = 0;
			for (j = 0; j < graph->v[i]->adjSize; ++j)
			{
				if (neighb[j].row >= 0 && neighb[j].row < grid->rows &&
					neighb[j].col >= 0 && neighb[j].col < grid->cols &&
					grid->mat[neighb[j].row][neighb[j].col] == 0)
				{
					graph->v[i]->adj[k++] = nodes[neighb[j].row][neighb[j].col];
				}
			}
			if (k < graph->v[i]->adjSize)
			{
				// get_neighbors returned some invalid neighbors
				graph->v[i]->adjSize = k;
				graph->v[i]->adj = (Node **)realloc(graph->v[i]->adj, k * sizeof(Node *));
			}
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

void bfs(Graph *graph, Node *s, Operation *op)
{
	// TOOD: implement the BFS algorithm on the graph, starting from the node s
	// at the end of the algorithm, every node reachable from s should have the color BLACK
	// for all the visited nodes, the minimum distance from s (dist) and the parent in the BFS tree should be set
	// for counting the number of operations, the optional op parameter is received
	// since op can be NULL (when we are calling the bfs for display purposes), you should check it before counting:
	// if(op != NULL) op->count();
	for (int i = 0; i < graph->nrNodes; i++)
	{
		if (op != NULL)
			op->count(3);
		graph->v[i]->color = COLOR_WHITE;
		graph->v[i]->dist = INT_MAX;
		graph->v[i]->parent = nullptr;
	}
	s->color = COLOR_GRAY;
	s->dist = 0;
	s->parent = nullptr;
	if (op != NULL)
		op->count(3);

	queue<Node *> Q;
	Q.push(s);
	if (op != NULL)
		op->count(6);
	while (!Q.empty())
	{
		Node *u = Q.front();
		Q.pop();
		if (op != NULL)
			op->count(6);
		for (int i = 0; i < u->adjSize; ++i)
		{
			Node *v = u->adj[i];
			if (op != NULL)
				op->count();
			if (v->color == COLOR_WHITE)
			{
				if (op != NULL)
					op->count(5);
				v->color = COLOR_GRAY;
				v->dist = u->dist + 1;
				v->parent = u;
				Q.push(v);
			}
		}
		if (op != NULL)
			op->count();
		u->color = COLOR_BLACK;
	}
}

void displayParentArray(int parent[], Point repr[], int size, int root = -1, int depth = 0)
{
	for (int i = 0; i < size; i++)
	{
		if (parent[i] == root)
		{
			for (int j = 0; j < depth; j++)
				cout << "    ";

			cout << "(" << repr[i].row << ", " << repr[i].col << ")" << endl;
			displayParentArray(parent, repr, size, i, depth + 1);
		}
	}
}

void print_bfs_tree(Graph *graph)
{
	// first, we will represent the BFS tree as a parent array
	int n = 0;			// the number of nodes
	int *p = NULL;		// the parent array
	Point *repr = NULL; // the representation for each element in p

	// some of the nodes in graph->v may not have been reached by BFS
	// p and repr will contain only the reachable nodes
	int *transf = (int *)malloc(graph->nrNodes * sizeof(int));
	for (int i = 0; i < graph->nrNodes; ++i)
	{
		if (graph->v[i]->color == COLOR_BLACK)
		{
			transf[i] = n;
			++n;
		}
		else
		{
			transf[i] = -1;
		}
	}
	if (n == 0)
	{
		// no BFS tree
		free(transf);
		return;
	}

	int err = 0;
	p = (int *)malloc(n * sizeof(int));
	repr = (Point *)malloc(n * sizeof(Node));
	for (int i = 0; i < graph->nrNodes && !err; ++i)
	{
		if (graph->v[i]->color == COLOR_BLACK)
		{
			if (transf[i] < 0 || transf[i] >= n)
			{
				err = 1;
			}
			else
			{
				repr[transf[i]] = graph->v[i]->position;
				if (graph->v[i]->parent == NULL)
				{
					p[transf[i]] = -1;
				}
				else
				{
					err = 1;
					for (int j = 0; j < graph->nrNodes; ++j)
					{
						if (graph->v[i]->parent == graph->v[j])
						{
							if (transf[j] >= 0 && transf[j] < n)
							{
								p[transf[i]] = transf[j];
								err = 0;
							}
							break;
						}
					}
				}
			}
		}
	}
	free(transf);
	transf = NULL;

	if (!err)
	{
		// TODO: pretty print the BFS tree
		// the parrent array is p (p[k] is the parent for node k or -1 if k is the root)
		// when printing the node k, print repr[k] (it contains the row and column for that point)
		// you can adapt the code for transforming and printing multi-way trees from the previous labs
		displayParentArray(p, repr, n);
	}

	if (p != NULL)
	{
		free(p);
		p = NULL;
	}
	if (repr != NULL)
	{
		free(repr);
		repr = NULL;
	}
}

int shortest_path(Graph *graph, Node *start, Node *end, Node *path[])
{
	// TODO: compute the shortest path between the nodes start and end in the given graph
	// the nodes from the path, should be filled, in order, in the array path
	// the number of nodes filled in the path array should be returned
	// if end is not reachable from start, return -1
	// note: the size of the array path is guaranteed to be at least 1000
	if (start == nullptr || end == nullptr)
		return -1;

	unordered_set<Node *> visited;
	queue<Node *> q;
	unordered_map<Node *, Node *> parent;

	q.push(start);
	visited.insert(start);
	parent[start] = nullptr;

	while (!q.empty())
	{
		Node *current = q.front();
		q.pop();

		if (current == end)
		{

			int pathLength = 0;
			for (Node *node = end; node != nullptr; node = parent[node])
			{
				path[pathLength++] = node;
			}

			for (int i = 0, j = pathLength - 1; i < j; ++i, --j)
			{
				swap(path[i], path[j]);
			}

			return pathLength;
		}

		for (int i = 0; i < current->adjSize; ++i)
		{
			Node *neighbor = current->adj[i];
			if (visited.find(neighbor) == visited.end())
			{
				visited.insert(neighbor);
				q.push(neighbor);
				parent[neighbor] = current;
			}
		}
	}

	return -1;
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
	int nrOfEdges = n - graph->nrNodes - 1;
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

void performance()
{
	int n, i;
	Profiler p("bfs");

	// vary the number of edges
	for (n = 1000; n <= 4500; n += 100)
	{
		Operation op = p.createOperation("bfs-edges", n);
		Graph graph;
		graph.nrNodes = 100;
		// initialize the nodes of the graph
		graph.v = (Node **)malloc(graph.nrNodes * sizeof(Node *));
		for (i = 0; i < graph.nrNodes; ++i)
		{
			graph.v[i] = (Node *)malloc(sizeof(Node));
			memset(graph.v[i], 0, sizeof(Node));
		}
		// TODO: generate n random edges
		// make sure the generated graph is connected
		insertEdges(&graph, n);

		bfs(&graph, graph.v[0], &op);
		free_graph(&graph);
	}

	// vary the number of vertices
	for (n = 100; n <= 200; n += 10)
	{
		Operation op = p.createOperation("bfs-vertices", n);
		Graph graph;
		graph.nrNodes = n;
		// initialize the nodes of the graph
		graph.v = (Node **)malloc(graph.nrNodes * sizeof(Node *));
		for (i = 0; i < graph.nrNodes; ++i)
		{
			graph.v[i] = (Node *)malloc(sizeof(Node));
			memset(graph.v[i], 0, sizeof(Node));
		}
		// TODO: generate 4500 random edges
		// make sure the generated graph is connected
		insertEdges(&graph, 4500);

		bfs(&graph, graph.v[0], &op);
		free_graph(&graph);
	}

	p.showReport();
}
