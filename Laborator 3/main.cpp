#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

using namespace std;

Profiler p("Average");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

/*
 * QuickSort
 *
 * Best:	O(n*log n)
 * Worst: O(n^2)
 * Average: O(n*log n)
 * Space Complexity: O(log n)
 * Stability: No
 *
 *
 * HeapSort
 *
 * Best: O(n*log n)
 * Worst: O(n*log n)
 * Average: O(n*log n)
 * Space Complexity: O(1)
 * Stability: No
 *
 * RandomizedSelect
 *
 * Worst: O(n^2)
 * Average: O(n)
 *
 * Insertion Sort
 *
 * Time Complexity
 * Best: O(n)
 * Worst: O(n^2)
 * Average: O(n^2)
 * Space Complexity: O(1)
 * Stability: Yes
 *
 * QuickSort este mult mai rapid decat HeapSort.
 * InsertionSort recursiv are acelasi numar de operatii ca si InsertionSort iterativ.
 * Timpul la InsertionSort recursiv este aproape la fel ca timpul la InsertionSort iterativ.
 * Worst Case QuickSort: sir sortat crescator
 *
 *
 */

int left(int i)
{
	return (2 * i + 1);
}

int right(int i)
{
	return (2 * i + 2);
}

void maxHeapify(int a[], int heapSize, int i, Operation *opComp, Operation *opAttr)
{
	int l = left(i);
	int r = right(i);
	int largest;
	if (l < heapSize)
	{
		opComp->count();
		if (a[l] > a[i])
			largest = l;
		else
			largest = i;
	}
	else
		largest = i;
	if (r < heapSize)
	{
		opComp->count();
		if (a[r] > a[largest])
			largest = r;
	}
	if (largest != i)
	{
		opAttr->count(3);
		swap(a[i], a[largest]);
		maxHeapify(a, heapSize, largest, opComp, opAttr);
	}
}

void buildMaxHeap_bottomUp(int a[], int heapSize) // Bottom-Up
{
	Operation opAttr = p.createOperation("heapSortAttr", heapSize);
	Operation opComp = p.createOperation("heapSortComp", heapSize);
	for (int i = heapSize / 2 - 1; i >= 0; i--)
	{
		maxHeapify(a, heapSize, i, &opComp, &opAttr);
	}
}

void heapSort(int a[], int heapSize, int maxSize)
{
	Operation opAttr = p.createOperation("heapSortAttr", heapSize);
	Operation opComp = p.createOperation("heapSortComp", heapSize);
	for (int i = heapSize / 2 - 1; i >= 0; i--)
	{
		maxHeapify(a, heapSize, i, &opComp, &opAttr);
	}
	for (int i = maxSize - 1; i >= 1; i--)
	{
		opAttr.count(3);
		swap(a[0], a[i]);
		heapSize--;
		maxHeapify(a, heapSize, 0, &opComp, &opAttr);
	}
}

//////////////////////////////////////////////////////////

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
	Operation opAttr = p.createOperation("quickSortAttr", b + 1);
	Operation opComp = p.createOperation("quickSortComp", b + 1);
	quickSort(v, a, b, &opComp, &opAttr);
}

///////////////////////////////////////////////

int random(int a, int b)
{
	return ((rand() % (b - a + 1)) + a);
}

int randomizedPartition(int a[], int p, int r, Operation *opComp, Operation *opAttr)
{
	int i = random(p, r);
	opAttr->count(3);
	swap(a[i], a[r]);
	return partition(a, p, r, opComp, opAttr);
}

int randomizedSelect(int v[], int a, int b, int i)
{
	Operation opAttr = p.createOperation("heapSortAttr", b + 1);
	Operation opComp = p.createOperation("heapSortComp", b + 1);
	if (a == b)
	{
		return v[a];
	}
	int q = randomizedPartition(v, a, b, &opComp, &opAttr);
	int k = q - a + 1;
	if (i == k)
		return v[q];
	else if (i < k)
		return randomizedSelect(v, a, q - 1, i);
	else
		return randomizedSelect(v, q + 1, b, i - k);
}

void randomizedQuickSort(int v[], int a, int b)
{
	Operation opAttr = p.createOperation("heapSortAttr", b + 1);
	Operation opComp = p.createOperation("heapSortComp", b + 1);
	if (a < b)
	{
		int q = randomizedPartition(v, a, b, &opComp, &opAttr);
		randomizedQuickSort(v, a, q - 1);
		randomizedQuickSort(v, q + 1, b);
	}
}

////////////////////////////////////////////////

int binarySearchTimer(int v[], int left, int right, int element)
{
	while (left <= right)
	{
		int middle = left + (right - left) / 2;
		if (v[middle] == element)
			return middle + 1;
		else if (v[middle] < element)
		{
			left = middle + 1;
		}
		else
			right = middle - 1;
	}
	return left;
}

void insertionSortTimer(int v[], int n)
{
	int min, j, indexMin;
	for (int i = 1; i < n; i++)
	{
		min = v[i];
		j = i - 1;
		indexMin = binarySearchTimer(v, 0, j, min);

		while (j >= indexMin)
		{
			v[j + 1] = v[j];
			j--;
		}
		v[j + 1] = min;
	}
}

void recursiveInsertionSortTimer(int v[], int n)
{
	if (n <= 1)
		return;

	recursiveInsertionSortTimer(v, n - 1);

	int min = v[n - 1];
	int j = n - 2;
	int indexMin = binarySearchTimer(v, 0, j, min);

	while (j >= indexMin)
	{
		v[j + 1] = v[j];
		j--;
	}
	v[j + 1] = min;
}

///////////////////////////////////////////////////

int binarySearch(int v[], int left, int right, int element, Operation *opComp)
{
	while (left <= right)
	{
		int middle = left + (right - left) / 2;
		opComp->count();
		if (v[middle] == element)
			return middle + 1;
		else if (v[middle] < element)
		{
			left = middle + 1;
			opComp->count();
		}
		else
			right = middle - 1;
	}
	return left;
}

void insertionSort(int v[], int n)
{
	int min, j, indexMin;
	Operation opComp = p.createOperation("insertionSortComp", n);
	Operation opAttr = p.createOperation("insertionSortAttr", n);
	for (int i = 1; i < n; i++)
	{
		opAttr.count();
		min = v[i];
		j = i - 1;
		indexMin = binarySearch(v, 0, j, min, &opComp);

		while (j >= indexMin)
		{
			opAttr.count();
			v[j + 1] = v[j];
			j--;
		}
		opAttr.count();
		v[j + 1] = min;
	}
}

void recursiveInsertionSort(int v[], int n, Operation *opComp, Operation *opAttr)
{
	if (n <= 1)
		return;

	recursiveInsertionSort(v, n - 1, opComp, opAttr);

	opAttr->count();
	int min = v[n - 1];
	int j = n - 2;
	int indexMin = binarySearch(v, 0, j, min, opComp);

	while (j >= indexMin)
	{
		opAttr->count();
		v[j + 1] = v[j];
		j--;
	}
	opAttr->count();
	v[j + 1] = min;
}

void useRecursiveInsertionSort(int v[], int n)
{
	Operation opComp = p.createOperation("recursiveInsertionSortComp", n);
	Operation opAttr = p.createOperation("recursiveInsertionSortAttr", n);
	recursiveInsertionSort(v, n, &opComp, &opAttr);
}

///////////////////////////////////////////////////

void generate(int v[], int p, int r)
{
	if (r <= p)
		return;
	int mij = (p + r) / 2;
	generate(v, p, mij - 1);
	generate(v, mij + 1, r);
	swap(v[r], v[mij]);
}

void printArray(int v[], int n)
{
	for (int i = 0; i < n; i++)
		cout << v[i] << " ";
	cout << endl;
}

void demo()
{

	int v[] = {1, 52, 7, 33, 561, 76, 152, 1, 2, 2, 1, 892, 90};
	int v1[] = {1, 52, 7, 33, 561, 76, 152, 1, 2, 2, 1, 892, 90};
	int v2[] = {1, 52, 7, 33, 561, 76, 152, 1, 2, 2, 1, 892, 90};
	int v3[] = {1, 52, 7, 33, 561, 76, 152, 1, 2, 2, 1, 892, 90};
	int v4[] = {1, 52, 7, 33, 561, 76, 152, 1, 2, 2, 1, 892, 90};
	int n = sizeof(v) / sizeof(v[0]);
	cout << "Vectorul initial: " << endl;
	printArray(v, n);
	// buildMaxHeap_bottomUp(v, n);
	heapSort(v, n, n);
	cout << "HeapSort: " << endl;
	printArray(v, n);
	useQuickSort(v1, 0, n - 1);
	cout << "QuickSort: " << endl;
	printArray(v1, n);
	cout << "RandomizedSelect: " << randomizedSelect(v2, 0, n - 1, 2) << endl;
	randomizedQuickSort(v2, 0, n - 1);
	cout << "RandomizedQuickSort: " << endl;
	printArray(v2, n);
	insertionSort(v3, n);
	cout << "InsertionSort: " << endl;
	printArray(v3, n);
	recursiveInsertionSortTimer(v4, n);
	cout << "RecursiveInsertionSort: " << endl;
	printArray(v4, n);
}

//////////////////////////////////////////////////

void perf_quickSortVsHeapSort()
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
			{
				v1[i] = v[i];
			}
			heapSort(v, n, n);
			useQuickSort(v1, 0, n - 1);
		}
	}

	p.divideValues("heapSortAttr", NR_TESTS);
	p.divideValues("heapSortComp", NR_TESTS);
	p.addSeries("HeapSort", "heapSortAttr", "heapSortComp");

	p.divideValues("quickSortAttr", NR_TESTS);
	p.divideValues("quickSortComp", NR_TESTS);
	p.addSeries("QuickSort", "quickSortAttr", "quickSortComp");

	p.createGroup("Atribuiri", "heapSortAttr", "quickSortAttr");
	p.createGroup("Comparatii", "heapSortComp", "quickSortComp");
	p.createGroup("Total", "HeapSort", "QuickSort");

	p.showReport();
}

void perf_insertionSort()
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
			{
				v1[i] = v[i];
			}
			insertionSort(v, n);
			useRecursiveInsertionSort(v1, n);
		}
	}

	p.divideValues("insertionSortAttr", NR_TESTS);
	p.divideValues("insertionSortComp", NR_TESTS);
	p.addSeries("InsertionSort", "insertionSortAttr", "insertionSortComp");

	p.divideValues("recursiveInsertionSortAttr", NR_TESTS);
	p.divideValues("recursiveInsertionSortComp", NR_TESTS);
	p.addSeries("RecursiveInsertionSort", "recursiveInsertionSortAttr", "recursiveInsertionSortComp");

	p.createGroup("Atribuiri", "insertionSortAttr", "recursiveInsertionSortAttr");
	p.createGroup("Comparatii", "insertionSortComp", "recursiveInsertionSortComp");
	p.createGroup("Total", "InsertionSort", "RecursiveInsertionSort");

	p.showReport();
}

void perfTimer_insertionSort()
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

			p.startTimer("insertionSortTimer", n);
			for (int i = 0; i < 100; i++)
			{
				insertionSortTimer(mat[i], n);
			}
			p.stopTimer("insertionSortTimer", n);

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

			p.startTimer("recursiveInsertionSortTimer", n);
			for (int i = 0; i < 100; i++)
			{
				recursiveInsertionSortTimer(mat[i], n);
			}
			p.stopTimer("recursiveInsertionSortTimer", n);

			for (int i = 0; i < 100; i++)
			{
				free(mat[i]);
			}
			free(mat);
		}
	}
	p.createGroup("Timer", "insertionSortTimer", "recursiveInsertionSortTimer");
	p.showReport();
}

void perf_quickSortBestCase()
{
	int v[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(v, n, 10, 50000, false, ASCENDING);
			generate(v, 0, n - 1);
			useQuickSort(v, 0, n - 1);
		}
	}

	p.divideValues("quickSortAttr", NR_TESTS);
	p.divideValues("quickSortComp", NR_TESTS);
	p.addSeries("QuickSort", "quickSortAttr", "quickSortComp");

	p.showReport();
}

void perf_quickSortWorstCase()
{
	int v[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(v, n, 10, 50000, false, ASCENDING);
			useQuickSort(v, 0, n - 1);
		}
	}

	p.divideValues("quickSortAttr", NR_TESTS);
	p.divideValues("quickSortComp", NR_TESTS);
	p.addSeries("QuickSort", "quickSortAttr", "quickSortComp");

	p.showReport();
}

int main()
{
	// srand(time(NULL));
	demo();
	// perf_quickSortVsHeapSort();
	// perf_quickSortBestCase();
	// perf_quickSortWorstCase();
	// perfTimer_insertionSort();
	// perf_insertionSort();
	return 0;
}
