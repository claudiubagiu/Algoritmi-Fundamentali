#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include "Profiler.h"

using namespace std;

Profiler p("Lab10");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

/*
 * Am implementat functia de makeSet in care parintele este egal cu indexul, iar rank-ul este 0.
 * Functia findSet cauta in ce Set se afla elementul respectiv si toata ramura va primi ca parinte radacina.
 * Functia unionSet uneste 2 seturi. In multimea care rank-ul este mai mare i se va adauga radacinii setul cu rank-ul mai mic.
 */

struct Node
{
	int parent;
	int rank;
};

struct Edge
{
	int u;
	int v;
	int weight;
};

struct EdgeComparator
{
	bool operator()(const Edge &lhs, const Edge &rhs) const
	{
		if (lhs.u < rhs.u)
			return true;
		if (lhs.u == rhs.u && lhs.v < rhs.v)
			return true;
		return false;
	}
};

bool compareEdges(const Edge &a, const Edge &b)
{
	return a.weight < b.weight;
}

void makeSet(Node set[], int x)
{
	set[x].parent = x;
	set[x].rank = 0;
}

int findSet(Node set[], int x)
{
	if (x != set[x].parent)
	{
		set[x].parent = findSet(set, set[x].parent);
	}
	return set[x].parent;
}

int findSet(Node set[], int x, Operation *opComp, Operation *opAttr)
{
	opComp->count();
	if (x != set[x].parent)
	{
		opAttr->count();
		set[x].parent = findSet(set, set[x].parent);
	}
	return set[x].parent;
}

void link(Node set[], int x, int y, Operation *opComp, Operation *opAttr)
{
	opComp->count();
	if (set[x].rank > set[y].rank)
	{
		opAttr->count();
		set[y].parent = x;
	}
	else
	{
		opAttr->count();
		set[x].parent = y;
		opComp->count();
		if (set[x].rank == set[y].rank)
		{
			opAttr->count();
			set[y].rank = set[y].rank + 1;
		}
	}
}

void link(Node set[], int x, int y)
{
	if (set[x].rank > set[y].rank)
		set[y].parent = x;
	else
	{
		set[x].parent = y;
		if (set[x].rank == set[y].rank)
			set[y].rank = set[y].rank + 1;
	}
}

void unionSet(Node set[], int x, int y)
{
	link(set, findSet(set, x), findSet(set, y));
}

void unionSet(Node set[], int x, int y, Operation *opComp, Operation *opAttr)
{
	link(set, findSet(set, x), findSet(set, y), opComp, opAttr);
}

int partition(vector<Edge> &edges, int p, int r, Operation *opComp, Operation *opAttr)
{
	opAttr->count();
	Edge x = edges[r];
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		opComp->count();
		if (edges[j].weight <= x.weight)
		{
			i++;
			opAttr->count(3);
			swap(edges[i], edges[j]);
		}
	}
	opAttr->count(3);
	swap(edges[i + 1], edges[r]);
	return (i + 1);
}

int partition(vector<Edge> &edges, int p, int r)
{
	Edge x = edges[r];
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		if (edges[j].weight <= x.weight)
		{
			i++;
			swap(edges[i], edges[j]);
		}
	}
	swap(edges[i + 1], edges[r]);
	return (i + 1);
}

void quickSort(vector<Edge> &edges, int a, int b)
{
	if (a < b)
	{
		int q = partition(edges, a, b);
		quickSort(edges, a, q - 1);
		quickSort(edges, q + 1, b);
	}
}

void quickSort(vector<Edge> &edges, int a, int b, Operation *opComp, Operation *opAttr)
{
	if (a < b)
	{
		int q = partition(edges, a, b, opComp, opAttr);
		quickSort(edges, a, q - 1, opComp, opAttr);
		quickSort(edges, q + 1, b, opComp, opAttr);
	}
}

vector<Edge> Kruskal(Node set[], vector<Edge> &edges, int numVertices)
{
	Operation opComp = p.createOperation("kruskalComp", numVertices);
	Operation opAttr = p.createOperation("kruskalAttr", numVertices);
	vector<Edge> minimumSpanningTree;

	for (int i = 0; i < numVertices; ++i)
	{
		opAttr.count(2);
		makeSet(set, i);
	}

	// sort(edges.begin(), edges.end(), compareEdges);
	quickSort(edges, 0, edges.size() - 1, &opComp, &opAttr);

	for (const Edge &edge : edges)
	{
		int u = edge.u;
		int v = edge.v;

		opComp.count();
		if (findSet(set, u, &opComp, &opAttr) != findSet(set, v, &opComp, &opAttr))
		{
			unionSet(set, u, v, &opComp, &opAttr);
			opAttr.count();
			minimumSpanningTree.push_back(edge);
		}
	}

	return minimumSpanningTree;
}

vector<Edge> insertEdges(int n)
{
	set<Edge, EdgeComparator> edges;
	while (edges.size() < 4 * n)
	{
		Edge e;
		e.u = rand() % n;
		e.v = rand() % n;
		e.weight = rand() % 100 + 1;
		if (e.u < e.v)
			edges.insert(e);
	}

	vector<Edge> vectorEdges;
	for (const Edge &currentEdge : edges)
		vectorEdges.push_back(currentEdge);
	edges.clear();

	return vectorEdges;
}

void perf_Kruskal()
{
	int n;
	Node set[MAX_SIZE];
	vector<Edge> edges;
	vector<Edge> minimumSpanningTree;
	for (n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			edges = insertEdges(n);
			minimumSpanningTree = Kruskal(set, edges, n);
			edges.clear();
			minimumSpanningTree.clear();
		}
	}

	p.divideValues("kruskalAttr", NR_TESTS);
	p.divideValues("kruskalComp", NR_TESTS);
	p.addSeries("kruskal", "kruskalAttr", "kruskalComp");

	p.showReport();
}

void demoDisjointSet()
{
	const int setSize = 10;
	Node set[setSize];

	for (int i = 0; i < setSize; ++i)
		makeSet(set, i);

	cout << "Representatives after union operations:\n";
	for (int i = 0; i < setSize; ++i)
	{
		cout << "Element " << i << " belongs to set with representative: " << findSet(set, i) << endl;
	}

	unionSet(set, 0, 1);
	unionSet(set, 1, 2);
	unionSet(set, 0, 3);
	unionSet(set, 3, 4);
	unionSet(set, 5, 6);
	unionSet(set, 5, 3);

	cout << "Representatives after union operations:\n";
	for (int i = 0; i < setSize; ++i)
	{
		cout << "Element " << i << " belongs to set with representative: " << findSet(set, i) << endl;
	}
}

void demoKruskal()
{
	const int numVertices = 9;
	Node set[numVertices];
	vector<Edge> edges = {{0, 1, 4}, {0, 7, 8}, {1, 7, 11}, {1, 2, 8}, {7, 6, 1}, {7, 8, 7}, {8, 2, 2}, {8, 6, 6}, {6, 5, 2}, {2, 5, 4}, {2, 3, 7}, {3, 5, 14}, {5, 4, 10}, {3, 4, 9}};

	vector<Edge> minimumSpanningTree = Kruskal(set, edges, numVertices);

	for (const Edge &edge : minimumSpanningTree)
		cout << "Edge: " << edge.u << " - " << edge.v << " (Weight: " << edge.weight << ")\n";
}

void demoInsertEdges()
{
	int n = 10;
	vector<Edge> edges = insertEdges(n);
	for (const Edge &edge : edges)
		cout << "Edge: " << edge.u << " - " << edge.v << " (Weight: " << edge.weight << ")\n";
	cout << endl
		 << "Nr. of edges: " << edges.size();
}

int main()
{
	srand(time(NULL));
	// demoDisjointSet();
	// demoKruskal();
	// demoInsertEdges();
	perf_Kruskal();
	return 0;
}