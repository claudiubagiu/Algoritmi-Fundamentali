#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

using namespace std;

Profiler p("Lab9");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define STEP_SIZE_THRESHOLD 10
#define NR_TESTS 5

/*
 * Am facut o structura de date Node care are cheia, pointer la parinte si copiii.
 * Pentru inserare am folosit inserarea de la Binary Search Tree.
 * Operatiile de numarare sunt la fel atat pentru iterativ, cat si pentru recursiv.
 * Pentru QuickSort Hibridizat, thresholdul minim este 10.
 * QuickSort Hibridizat este mult mai rapid decat QuickSort, atat pentru timpul de rulare, cat și pentru numărul de operații.
 */

struct Node
{
	int key;
	Node *left;
	Node *right;
	Node *parent;

	Node(int key)
	{
		this->key = key;
		left = right = parent = nullptr;
	}
	Node(int key, Node *parent)
	{
		this->key = key;
		left = right = nullptr;
		this->parent = parent;
	}
};

void insert(Node *&root, int key)
{
	Node *node = new Node(key);
	if (root == nullptr)
	{
		root = node;
		return;
	}
	Node *prev = nullptr;
	Node *temp = root;
	while (temp)
	{
		prev = temp;
		if (temp->key > key)
			temp = temp->left;
		else if (temp->key < key)
			temp = temp->right;
	}
	node->parent = prev;
	if (prev->key > key)
		prev->left = node;
	else
		prev->right = node;
}

void recursiveInorder(Node *root, Operation *op, bool isDemo)
{
	if (root == nullptr)
		return;
	recursiveInorder(root->left, op, isDemo);
	if (isDemo)
		cout << root->key << " ";
	else
		op->count();
	recursiveInorder(root->right, op, isDemo);
}

void useRecursiveInorder(Node *root, int size, bool isDemo = 0)
{
	Operation op = p.createOperation("recursiveInorder", size);
	recursiveInorder(root, &op, isDemo);
}

void iterativeInorder(Node *root, int size, bool isDemo = 0)
{
	Operation op = p.createOperation("iterativeInorder", size);
	int d = 1;
	Node *node = root;
	do
	{
		if (d == 1)
		{
			if (node->left != nullptr)
				node = node->left;
			else
				d = 2;
		}
		if (d == 2)
		{
			if (isDemo)
				cout << node->key << " ";
			else
				op.count();
			if (node->right != nullptr)
			{
				node = node->right;
				d = 1;
			}
			else
				d = 3;
		}
		if (d == 3)
		{
			if (node->parent != nullptr)
			{
				if (node == node->parent->left)
					d = 2;
				node = node->parent;
			}
		}
	} while (!(node == root && d == 3));
}

void demoBinarySearchTree()
{
	Node *root = NULL;
	insert(root, 30);
	insert(root, 50);
	insert(root, 15);
	insert(root, 20);
	insert(root, 10);
	insert(root, 40);
	insert(root, 60);

	iterativeInorder(root, 7, 1);
	cout << endl;
	useRecursiveInorder(root, 7, 1);
}

void perf_iterativeVsRecursive()
{
	int v[MAX_SIZE];
	int n;
	Node *root = nullptr;
	for (n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(v, n, 10, 50000, true, UNSORTED);
			for (int i = 0; i < n; i++)
				insert(root, v[i]);
			iterativeInorder(root, n);
			useRecursiveInorder(root, n);
			root = nullptr;
		}
	}

	p.divideValues("iterativeInorder", NR_TESTS);
	p.divideValues("recursiveInorder", NR_TESTS);
	p.createGroup("Total", "iterativeInorder", "recursiveInorder");

	p.showReport();
}

////////////////////////////////////////////////////////////////////////////////////////////

void insertionSort(int v[], int low, int n, Operation *opComp, Operation *opAttr)
{
	for (int i = low + 1; i < n + 1; i++)
	{
		opAttr->count();
		int val = v[i];
		int j = i;
		while (j > low && v[j - 1] > val)
		{
			opAttr->count();
			opComp->count();
			v[j] = v[j - 1];
			j--;
		}
		opComp->count();
		opAttr->count();
		v[j] = val;
	}
}

int partition(int a[], int p, int r, Operation *opComp, Operation *opAttr)
{
	opAttr->count();
	int x = a[r];
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		opComp->count();
		if (a[j] <= x)
		{
			i++;
			opAttr->count(3);
			swap(a[i], a[j]);
		}
	}
	opAttr->count(3);
	swap(a[i + 1], a[r]);
	return (i + 1);
}

void quickSortHybridization(int v[], int a, int b, Operation *opComp, Operation *opAttr, int threshold)
{
	if (a >= b)
		return;
	else if (b - a + 1 < threshold)
		insertionSort(v, a, b, opComp, opAttr);
	else
	{
		int q = partition(v, a, b, opComp, opAttr);
		quickSortHybridization(v, a, q - 1, opComp, opAttr, threshold);
		quickSortHybridization(v, q + 1, b, opComp, opAttr, threshold);
	}
}

void useQuickSortHybridization(int v[], int a, int b, int threshold = 10)
{
	Operation opComp = p.createOperation("quickSortHybridizationComp", b + 1);
	Operation opAttr = p.createOperation("quickSortHybridizationAttr", b + 1);
	quickSortHybridization(v, a, b, &opComp, &opAttr, threshold);
}

void useQuickSortHybridizationOptimumThreshold(int v[], int a, int b, int threshold)
{
	Operation opComp = p.createOperation("quickSortHybridizationComp", threshold);
	Operation opAttr = p.createOperation("quickSortHybridizationAttr", threshold);
	quickSortHybridization(v, a, b, &opComp, &opAttr, threshold);
}

void quickSort(int v[], int a, int b, Operation *opComp, Operation *opAttr)
{
	if (a < b)
	{
		int q = partition(v, a, b, opComp, opAttr);
		quickSort(v, a, q - 1, opComp, opAttr);
		quickSort(v, q + 1, b, opComp, opAttr);
	}
}

void useQuickSort(int v[], int a, int b)
{
	Operation opComp = p.createOperation("quickSortComp", b + 1);
	Operation opAttr = p.createOperation("quickSortAttr", b + 1);
	quickSort(v, a, b, &opComp, &opAttr);
}

void demoQuickSortHybridization()
{
	int v[100];
	FillRandomArray(v, 100, 0, 1000, false, 0);
	for (int i = 0; i < 99; i++)
	{
		if (i % 10 == 0)
			cout << endl;
		cout << v[i] << " ";
	}
	cout << endl;
	useQuickSortHybridization(v, 0, 99);
	for (int i = 0; i < 99; i++)
	{
		if (i % 10 == 0)
			cout << endl;
		cout << v[i] << " ";
	}
}

void perf_QuickSortHybridizationVsQuickSort()
{
	int v[MAX_SIZE];
	int v1[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(v, n, 10, 50000, false, UNSORTED);
			for (int i = 0; i < n; i++)
				v1[i] = v[i];
			useQuickSortHybridization(v, 0, n - 1);
			useQuickSort(v1, 0, n - 1);
		}
	}

	p.divideValues("quickSortHybridizationAttr", NR_TESTS);
	p.divideValues("quickSortHybridizationComp", NR_TESTS);
	p.addSeries("quickSortHybridization", "quickSortHybridizationAttr", "quickSortHybridizationComp");

	p.divideValues("quickSortAttr", NR_TESTS);
	p.divideValues("quickSortComp", NR_TESTS);
	p.addSeries("quickSort", "quickSortAttr", "quickSortComp");

	p.createGroup("Atribuiri", "quickSortHybridizationAttr", "quickSortAttr");
	p.createGroup("Comparatii", "quickSortHybridizationComp", "quickSortComp");
	p.createGroup("Total", "quickSortHybridization", "quickSort");

	p.showReport();
}

/////////////////////////////////////////////////////////////////

void insertionSort(int v[], int low, int n)
{

	for (int i = low + 1; i < n + 1; i++)
	{
		int val = v[i];
		int j = i;
		while (j > low && v[j - 1] > val)
		{
			v[j] = v[j - 1];
			j--;
		}
		v[j] = val;
	}
}

int partition(int a[], int p, int r)
{
	int x = a[r];
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		if (a[j] <= x)
		{
			i++;
			swap(a[i], a[j]);
		}
	}
	swap(a[i + 1], a[r]);
	return (i + 1);
}

void quickSortHybridizationTimer(int v[], int a, int b)
{
	if (a >= b)
		return;
	else if (b - a + 1 < 30)
		insertionSort(v, a, b);
	else
	{
		int q = partition(v, a, b);
		quickSortHybridizationTimer(v, a, q - 1);
		quickSortHybridizationTimer(v, q + 1, b);
	}
}

void quickSortTimer(int v[], int a, int b)
{
	if (a < b)
	{
		int q = partition(v, a, b);
		quickSortTimer(v, a, q - 1);
		quickSortTimer(v, q + 1, b);
	}
}

void perfTimer_QuickSortHybridizationVsQuickSort()
{
	int v1[MAX_SIZE];
	int n;
	int **mat;
	for (n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(v1, n, 10, 50000, false, 0);

			mat = (int **)malloc(100 * sizeof(int *));
			for (int i = 0; i < 100; i++)
			{
				mat[i] = (int *)malloc(n * sizeof(int));
			}
			for (int i = 0; i < 100; i++)
			{
				memcpy(mat[i], v1, n * sizeof(int));
			}

			p.startTimer("quickSortTimer", n);
			for (int i = 0; i < 100; i++)
			{
				quickSortTimer(mat[i], 0, n - 1);
			}
			p.stopTimer("quickSortTimer", n);

			for (int i = 0; i < 100; i++)
			{
				free(mat[i]);
			}
			free(mat);

			mat = (int **)malloc(100 * sizeof(int *));
			for (int i = 0; i < 100; i++)
			{
				mat[i] = (int *)malloc(n * sizeof(int));
			}
			for (int i = 0; i < 100; i++)
			{
				memcpy(mat[i], v1, n * sizeof(int));
			}

			p.startTimer("quickSortHybridizationTimer", n);
			for (int i = 0; i < 100; i++)
			{
				quickSortHybridizationTimer(mat[i], 0, n - 1);
			}
			p.stopTimer("quickSortHybridizationTimer", n);

			for (int i = 0; i < 100; i++)
			{
				free(mat[i]);
			}
			free(mat);
		}
	}
	p.createGroup("Timer", "quickSortHybridizationTimer", "quickSortTimer");
	p.showReport();
}

void perf_QuickSortHybridizationThreshold()
{
	int v[MAX_SIZE];
	int v1[MAX_SIZE];
	int n;
	FillRandomArray(v, MAX_SIZE, 10, 50000, false, UNSORTED);
	for (n = STEP_SIZE_THRESHOLD; n < MAX_SIZE; n += STEP_SIZE_THRESHOLD)
	{
		for (int i = 0; i < MAX_SIZE; i++)
			v1[i] = v[i];
		useQuickSortHybridizationOptimumThreshold(v1, 0, MAX_SIZE - 1, n);
	}

	p.divideValues("quickSortHybridizationAttr", NR_TESTS);
	p.divideValues("quickSortHybridizationComp", NR_TESTS);
	p.addSeries("quickSortHybridization", "quickSortHybridizationAttr", "quickSortHybridizationComp");

	p.showReport();
}

int main()
{
	// demoQuickSortHybridization();
	// demoBinarySearchTree();
	// perf_iterativeVsRecursive();
	// perf_QuickSortHybridizationVsQuickSort();
	// perfTimer_QuickSortHybridizationVsQuickSort();
	// perf_QuickSortHybridizationThreshold();
	return 0;
}