#include <iostream>
#include "Profiler.h"

using namespace std;

Profiler p("sortingAlgorithms");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

/**
 * ---------------------
 * Selection Sort
 *
 * Time Complexity
 * Best: O(n^2)
 * Worst: O(n^2)
 * Average: O(n^2)
 * Space Complexity: O(1)
 * Stability: No
 *
 *
 * Worst Case Complexity: O(n^2)
 * Atunci cand vectorul este sortat decrescator
 * Suma atrib = 3(n-1)
 *
 * Best Case Complexity: O(n^2)
 * Atunci cand vectorul este sortat crescator
 * Suma atrib = 0
 * Suma comp = n(n-1)/2
 *
 * Average Case Complexity: O(n^2)
 * Atunci cand vectorul nu este sortat
 * ---------------------
 * Bubble Sort
 *
 * Time Complexity
 * Best: O(n)
 * Worst: O(n^2)
 * Average: O(n^2)
 * Space Complexity: O(1)
 * Stability: Yes
 *
 * Worst Case Complexity: O(n^2)
 * Atunci cand vectorul este sortat decrescator
 *
 * Best Case Complexity: O(n)
 * Atunci cand vectorul este sortat crescator
 *
 * Average Case Complexity: O(n^2)
 * Atunci cand vectorul nu este sortat
 * ---------------------
 * Insertion Sort
 *
 * Time Complexity
 * Best: O(n)
 * Worst: O(n^2)
 * Average: O(n^2)
 * Space Complexity: O(1)
 * Stability: Yes
 *
 * Worst Case Complexity: O(n^2)
 * Atunci cand vectorul este sortat descrescator
 * Suma comp = n(n-1)/2
 * Suma atrib = n(n-1)/2 + 2(n-1)
 *
 * Best Case Complexity: O(n)
 * Atunci cand vectorul este sortat crescator
 * Suma comp = n-1
 * Suma atrib = 2(n-1)
 *
 * Average Case Complexity: O(n^2)
 * Atunci cand vectorul nu este sortat
 * ---------------------
*/

void selectionSort(int v[], int n)
{
	Operation opComp = p.createOperation("selectionSortComp", n);
	Operation opAttr = p.createOperation("selectionSortAttr", n);
	int min;
	for (int i = 0; i < n - 1; i++)
	{
		min = i;
		for (int j = i + 1; j < n; j++)
		{
			opComp.count();
			if (v[j] < v[min])
				min = j;
		}
		if (min != i)
		{
			opAttr.count(3);
			swap(v[min], v[i]);
		}
	}
}

int binarySearch(int v[], int left, int right, int element, Operation* opComp)
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

void bubbleSort(int v[], int n)
{
	Operation opAttr = p.createOperation("bubbleSortAttr", n);
	Operation opComp = p.createOperation("bubbleSortComp", n);
	for (int i = 0; i < n - 1; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			opComp.count();
			if (v[j] > v[j + 1])
			{
				opAttr.count(3);
				swap(v[j], v[j + 1]);
			}
		}
	}
}

void demo()
{
	int v[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int n = 10;
	// bubbleSort(v, n);
	insertionSort(v, n);
	// selectionSort(v, n);
	for (int i = 0; i < n; i++)
		cout << v[i] << " ";
}

void perf(int order)
{
	int v[MAX_SIZE], v1[MAX_SIZE], v2[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(v, n, 10, 50000, false, order);
			for (int i = 0; i < n; i++)
			{
				v1[i] = v[i];
				v2[i] = v[i];
			}
			selectionSort(v, n);
			insertionSort(v1, n);
			bubbleSort(v2, n);
		}
	}

	p.divideValues("selectionSortAttr", NR_TESTS);
	p.divideValues("selectionSortComp", NR_TESTS);
	p.addSeries("Selection Sort", "selectionSortAttr", "selectionSortComp");

	p.divideValues("insertionSortAttr", NR_TESTS);
	p.divideValues("insertionSortComp", NR_TESTS);
	p.addSeries("Insertion Sort", "insertionSortAttr", "insertionSortComp");

	p.divideValues("bubbleSortAttr", NR_TESTS);
	p.divideValues("bubbleSortComp", NR_TESTS);
	p.addSeries("Bubble Sort", "bubbleSortAttr", "bubbleSortComp");

	p.createGroup("Atribuiri", "selectionSortAttr", "insertionSortAttr", "bubbleSortAttr");
	p.createGroup("Comparatii", "selectionSortComp", "insertionSortComp", "bubbleSortComp");
	p.createGroup("Total", "Selection Sort", "Insertion Sort", "Bubble Sort");
}

void perf_all()
{
	perf(UNSORTED);
	p.reset("Best");
	perf(ASCENDING);
	p.reset("Worst");
	perf(DESCENDING);
	p.showReport();
}


int main()
{
	//perf_all();
	// demo();
	return 0;
}