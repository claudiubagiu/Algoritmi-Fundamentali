#include <iostream>
#include <limits.h>
#include "Profiler.h"

using namespace std;

/*
 * ------------------------
 * buildMaxHeap_bottomUp
 * ------------------------
 * Aceasta metoda are complexitate O(n). Este mult mai rapida decat buildMaxHeap_topDown.
 *
 * Worst-case time complexity: O(n)
 * Atunci cand elementele sunt sortate in ordine crescatoare
 *
 *
 * ------------------------
 * buildMaxHeap_topDown
 * ------------------------
 * Aceasta metoda are complexitate O(n * log(n)) deoarece pentru fiecare insert, trebuie sa parcurgem toata inaltimea arborelui, unde sunt n elemente.
 *
 * Worst-case time complexity: O(n * log(n))
 * Atunci cand elementele sunt sortate in ordine crescatoare
 *
 */

Profiler p("Heap");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

int left(int i)
{
	return (2 * i + 1);
}

int right(int i)
{
	return (2 * i + 2);
}

int parent(int i)
{
	return (i - 1) / 2;
}

void maxHeapify(int a[], int heapSize, int i, Operation* opComp, Operation* opAttr)
{
	int l = left(i);
	int r = right(i);
	int largest;
	opComp->count();
	if (l < heapSize && a[l] > a[i])
		largest = l;
	else
		largest = i;
	opComp->count();
	if (r < heapSize && a[r] > a[largest])
		largest = r;
	if (largest != i)
	{
		opAttr->count(3);
		swap(a[i], a[largest]);
		maxHeapify(a, heapSize, largest, opComp, opAttr);
	}
}

void buildMaxHeap_bottomUp(int a[], int heapSize) // Bottom-Up
{
	Operation opAttr = p.createOperation("BottomUpAttr", heapSize);
	Operation opComp = p.createOperation("BottomUpComp", heapSize);
	for (int i = heapSize / 2 - 1; i >= 0; i--)
	{
		maxHeapify(a, heapSize, i, &opComp, &opAttr);
	}
}

void heapSort(int a[], int heapSize, int maxSize)
{
	Operation opAttr = p.createOperation("heapSortAttr", heapSize);
	Operation opComp = p.createOperation("heapSortComp", heapSize);
	for (int i = maxSize - 1; i >= 1; i--)
	{
		opAttr.count(3);
		swap(a[0], a[i]);
		heapSize--;
		maxHeapify(a, heapSize, 0, &opComp, &opAttr);
	}
}

void heapIncreaseKey(int a[], int i, int key, Operation* opComp, Operation* opAttr)
{
	opComp->count();
	if (key < a[i])
	{
		cout << "New key is smaller than current key." << endl;
		return;
	}
	opAttr->count();
	a[i] = key;

	while (a[parent(i)] < a[i] && i > 0)
	{
		opComp->count();
		opAttr->count(3);
		swap(a[i], a[parent(i)]);
		i = parent(i);
	}
	opComp->count();
}

void maxHeapInsert(int a[], int& heapSize, int key, Operation* opComp, Operation* opAttr)
{
	heapSize++;
	opAttr->count();
	a[heapSize] = INT_MIN;
	heapIncreaseKey(a, heapSize, key, opComp, opAttr);
}

void buildMaxHeap_topDown(int a[], int heapSize, int maxSize) // Top-Down
{
	Operation opAttr = p.createOperation("TopDownAttr", heapSize);
	Operation opComp = p.createOperation("TopDownComp", heapSize);
	heapSize = 0;
	for (int i = 1; i < maxSize; i++)
	{
		maxHeapInsert(a, heapSize, a[i], &opComp, &opAttr);
	}
}

void printHeap(int a[], int heapSize)
{
	for (int i = 0; i < heapSize; i++)
	{
		cout << a[i] << " ";
	}
	cout << endl;
}

void demo()
{
	int a[] = { 1, 12, 93, 84, 5, 65, 7, 8, 856, 18, 8, 49 };
	int b[] = { 1, 12, 93, 84, 5, 65, 7, 8, 856, 18, 8, 49 };
	int heapSize = sizeof(a) / sizeof(a[0]);
	int maxSize = sizeof(a) / sizeof(a[0]);

	cout << "Vectorul initial: " << endl;
	printHeap(a, heapSize);

	buildMaxHeap_bottomUp(a, heapSize);
	cout << "MaxHeap Bottom-Up: " << endl;
	printHeap(a, heapSize);
	heapSort(a, heapSize, maxSize);
	cout << "HeapSort: " << endl;
	printHeap(a, heapSize);

	buildMaxHeap_topDown(b, heapSize, maxSize);
	cout << "MaxHeap Top-Down: " << endl;
	printHeap(b, heapSize);
	heapSort(b, heapSize, maxSize);
	cout << "HeapSort: " << endl;
	printHeap(b, heapSize);
}

void perf(int order)
{
	int v[MAX_SIZE]{};
	int v1[MAX_SIZE]{};
	int n;
	int heapSizeV, maxSizeV, heapSizeV1, maxSizeV1;
	for (n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(v, n, 10, 50000, false, order);
			for (int i = 0; i < n; i++)
			{
				v1[i] = v[i];
			}
			heapSizeV = n;
			maxSizeV = n;
			heapSizeV1 = n;
			maxSizeV1 = n;
			buildMaxHeap_bottomUp(v, heapSizeV);
			buildMaxHeap_topDown(v1, heapSizeV1, maxSizeV1);
		}
	}

	p.divideValues("BottomUpAttr", NR_TESTS);
	p.divideValues("BottomUpComp", NR_TESTS);
	p.addSeries("BottomUp", "BottomUpAttr", "BottomUpComp");

	p.divideValues("TopDownAttr", NR_TESTS);
	p.divideValues("TopDownComp", NR_TESTS);
	p.addSeries("TopDown", "TopDownAttr", "TopDownComp");

	p.createGroup("Atribuiri", "BottomUpAttr", "TopDownAttr");
	p.createGroup("Comparatii", "BottomUpComp", "TopDownComp");
	p.createGroup("Total", "BottomUp", "TopDown");
}

void perf_all()
{
	perf(UNSORTED);
	p.reset("Worst");
	perf(ASCENDING);
	p.showReport();
}

int main()
{
	//demo();
	//perf_all();
	return 0;
}