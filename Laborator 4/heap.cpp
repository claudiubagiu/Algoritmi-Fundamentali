#include <climits>
#include <algorithm>
#include "intLinkedList.h"
#include "Profiler.h"

using namespace std;

int left(int i)
{
	return (2 * i + 1);
}

int right(int i)
{
	return (2 * i + 2);
}

void minHeapify(IntLinkedList* a[], int heapSize, int i, Operation* opComp, Operation* opAttr)
{
	int l = left(i);
	int r = right(i);
	int smallest;
	if (l < heapSize)
	{
		opComp->count();
		if (*intLinkedList_front(a[l]) < *intLinkedList_front(a[i]))
			smallest = l;
		else
			smallest = i;
	}
	else
		smallest = i;
	if (r < heapSize)
	{
		opComp->count();
		if (*intLinkedList_front(a[r]) < *intLinkedList_front(a[smallest]))
			smallest = r;

	}
	if (smallest != i)
	{
		opAttr->count(3);
		swap(a[i], a[smallest]);
		minHeapify(a, heapSize, smallest, opComp, opAttr);
	}
}

void buildMinHeap_bottomUp(IntLinkedList* a[], int heapSize, Operation* opComp, Operation* opAttr)
{
	for (int i = heapSize / 2 - 1; i >= 0; i--)
	{
		minHeapify(a, heapSize, i, opComp, opAttr);
	}
}

void heapSort(IntLinkedList* a[], int heapSize, int maxSize, Operation* opComp, Operation* opAttr)
{
	for (int i = heapSize / 2 - 1; i >= 0; i--)
	{
		minHeapify(a, heapSize, i, opComp, opAttr);
	}
	for (int i = maxSize - 1; i >= 1; i--)
	{
		opAttr->count(3);
		swap(a[0], a[i]);
		heapSize--;
		minHeapify(a, heapSize, 0, opComp, opAttr);
	}
}

int extractMin(IntLinkedList* a[], int& heapSize, Operation* opComp, Operation* opAttr)
{
	if (heapSize == 0)
		return INT_MAX;
	if (heapSize == 1)
	{
		heapSize--;
		return *intLinkedList_front(a[0]);
	}
	opAttr->count(2);
	int root = *intLinkedList_front(a[0]);
	a[0] = a[heapSize - 1];
	heapSize--;
	minHeapify(a, heapSize, 0, opComp, opAttr);
	return root;
}