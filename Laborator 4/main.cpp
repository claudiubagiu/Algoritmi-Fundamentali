#include <iostream>
#include "intLinkedList.h"
#include "Profiler.h"
#include "heap.h"

using namespace std;

Profiler p("Average");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

/*
 * Complexitate: n * log(k)
 * Cu cat numarul de liste este mai mare, cu atat numarul operatiilor este mai mare.
 * Daca n = const si k creste din 10 in 10, cu cat k are o valoare mai mare, atunci numarul operatiilor va creste din ce in ce mai putin.
 *
 */

void printList(int *element, void *context)
{
	(void)context;
	std::cout << *element << " ";
}

void testIntLinkedList()
{
	IntLinkedList *list1[5];
	for (int i = 0; i < 5; i++)
	{
		list1[i] = intLinkedList_new();
	}
	IntLinkedList *list = intLinkedList_new();
	cout << "Size: " << intLinkedList_size(list) << endl;
	intLinkedList_push_front(list, 1);
	intLinkedList_push_front(list, 2);
	intLinkedList_push_back(list, 3);
	cout << "List: ";
	intLinkedList_forEach(list, nullptr, printList);
	cout << endl
		 << "First element: " << *intLinkedList_front(list) << endl;
	cout << "Last element: " << *intLinkedList_back(list) << endl;
	intLinkedList_push_front(list, 5);
	intLinkedList_push_front(list, 7);
	intLinkedList_push_back(list, 6);
	cout << "List: ";
	intLinkedList_forEach(list, nullptr, printList);
	cout << endl;
	intLinkedList_pop_back(list);
	intLinkedList_pop_front(list);
	cout << "List: ";
	intLinkedList_forEach(list, nullptr, printList);
	cout << endl;
	intLinkedList_remove(list, 3);
	cout << "List: ";
	intLinkedList_forEach(list, nullptr, printList);
	cout << endl;
	intLinkedList_push_at(list, 2, 56);
	cout << "List: ";
	intLinkedList_forEach(list, nullptr, printList);
	cout << endl;
	cout << "The list have element 56. ";
	if (intLinkedList_contains(list, 56) == 1)
		cout << "(True)" << endl;
	else
		cout << "(False)" << endl;
	cout << "The list have element 456. ";
	if (intLinkedList_contains(list, 456) == 1)
		cout << "(True)" << endl;
	else
		cout << "(False)" << endl;
	intLinkedList_clear(list);
	cout << "The size of the list: " << intLinkedList_size(list) << endl;
	cout << "List: ";
	intLinkedList_forEach(list, nullptr, printList);
	cout << endl;
	intLinkedList_free(list);
}

void printArray(int a[], int n)
{
	for (int i = 0; i < n; i++)
		cout << a[i] << " ";
	cout << endl;
}

IntLinkedList *KWayMerge(IntLinkedList *list[], int k, int n, Operation *opComp, Operation *opAttr)
{
	opAttr->count();
	IntLinkedList *output = intLinkedList_new(opComp, opAttr);
	int heapSize = k;
	buildMinHeap_bottomUp(list, k, opComp, opAttr);
	while (heapSize > 0)
	{
		opAttr->count();
		int min = *intLinkedList_front(list[0]);
		intLinkedList_pop_front(list[0], opComp, opAttr);
		intLinkedList_push_back(output, min, opComp, opAttr);
		opComp->count();
		if (intLinkedList_front(list[0]) == nullptr)
		{
			opAttr->count();
			list[0] = list[heapSize - 1];
			heapSize--;
		}
		minHeapify(list, heapSize, 0, opComp, opAttr);
	}
	return output;
}

void demo()
{
	IntLinkedList *list[4];
	for (int i = 0; i < 4; i++)
	{
		list[i] = intLinkedList_new();
	}
	intLinkedList_push_back(list[0], 5);
	intLinkedList_push_back(list[0], 6);
	intLinkedList_push_back(list[0], 6);
	intLinkedList_push_back(list[1], 1);
	intLinkedList_push_back(list[1], 2);
	intLinkedList_push_back(list[1], 4);
	intLinkedList_push_back(list[2], 4);
	intLinkedList_push_back(list[2], 5);
	intLinkedList_push_back(list[2], 7);
	intLinkedList_push_back(list[3], 3);
	intLinkedList_push_back(list[3], 7);
	intLinkedList_push_back(list[3], 12);
	for (int i = 0; i < 4; i++)
	{
		cout << "Lista " << i << ": ";
		intLinkedList_forEach(list[i], nullptr, printList);
		cout << endl;
	}

	Operation opComp = p.createOperation("KWayMergeComp", 52);
	Operation opAttr = p.createOperation("KWayMergeAttr", 52);
	IntLinkedList *output = KWayMerge(list, 4, 12, &opComp, &opAttr);
	cout << "List: ";
	intLinkedList_forEach(output, nullptr, printList);
	cout << endl;

	intLinkedList_free(output);
}

IntLinkedList **generateList(int n, int k)
{
	IntLinkedList **list = new IntLinkedList *[k];
	for (int i = 0; i < k; i++)
	{
		list[i] = intLinkedList_new();
	}
	int v[MAX_SIZE];
	if (n % k == 0)
	{
		for (int i = 0; i < k; i++)
		{

			FillRandomArray(v, n / k, 10, 50000, false, 1);
			for (int j = 0; j < n / k; j++)
			{
				intLinkedList_push_back(list[i], v[j]);
			}
		}
	}
	else
	{
		for (int i = 0; i < k - 1; i++)
		{
			FillRandomArray(v, n / k, 10, 50000, false, 1);
			for (int j = 0; j < n / k; j++)
			{
				intLinkedList_push_back(list[i], v[j]);
			}
		}
		FillRandomArray(v, n / k + n % k, 10, 50000, false, 1);
		for (int j = 0; j < n / k + n % k; j++)
		{
			intLinkedList_push_back(list[k - 1], v[j]);
		}
	}
	return list;
}

void demo2()
{
	IntLinkedList **list = generateList(52, 5);
	for (int i = 0; i < 5; i++)
	{
		cout << "Lista " << i << ": ";
		intLinkedList_forEach(list[i], nullptr, printList);
		cout << endl;
	}

	Operation opComp = p.createOperation("KWayMergeComp", 52);
	Operation opAttr = p.createOperation("KWayMergeAttr", 52);
	IntLinkedList *output = KWayMerge(list, 5, 52, &opComp, &opAttr);
	cout << "List: ";
	intLinkedList_forEach(output, nullptr, printList);
	cout << endl;

	intLinkedList_free(output);
}

void perf1()
{
	int n;
	for (n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			Operation opComp = p.createOperation("KWayMergeComp5", n);
			Operation opAttr = p.createOperation("KWayMergeAttr5", n);
			IntLinkedList **list = generateList(n, 5);
			IntLinkedList *output = KWayMerge(list, 5, n, &opComp, &opAttr);
			intLinkedList_free(output);

			Operation opComp1 = p.createOperation("KWayMergeComp10", n);
			Operation opAttr1 = p.createOperation("KWayMergeAttr10", n);
			IntLinkedList **list1 = generateList(n, 10);
			IntLinkedList *output1 = KWayMerge(list1, 10, n, &opComp1, &opAttr1);
			intLinkedList_free(output1);

			Operation opComp2 = p.createOperation("KWayMergeComp100", n);
			Operation opAttr2 = p.createOperation("KWayMergeAttr100", n);
			IntLinkedList **list2 = generateList(n, 100);
			IntLinkedList *output2 = KWayMerge(list2, 100, n, &opComp2, &opAttr2);
			intLinkedList_free(output2);
		}
	}

	p.divideValues("KWayMergeAttr5", NR_TESTS);
	p.divideValues("KWayMergeComp5", NR_TESTS);
	p.addSeries("KWayMerge5", "KWayMergeAttr5", "KWayMergeComp5");

	p.divideValues("KWayMergeAttr10", NR_TESTS);
	p.divideValues("KWayMergeComp10", NR_TESTS);
	p.addSeries("KWayMerge10", "KWayMergeAttr10", "KWayMergeComp10");

	p.divideValues("KWayMergeAttr100", NR_TESTS);
	p.divideValues("KWayMergeComp100", NR_TESTS);
	p.addSeries("KWayMerge100", "KWayMergeAttr100", "KWayMergeComp100");

	p.createGroup("Comparatii", "KWayMergeComp5", "KWayMergeComp10", "KWayMergeComp100");
	p.createGroup("Atribuiri", "KWayMergeAttr5", "KWayMergeAttr10", "KWayMergeAttr100");
	p.createGroup("Total", "KWayMerge5", "KWayMerge10", "KWayMerge100");

	p.showReport();
}

void perf2()
{
	int n;
	for (n = 10; n < 500; n += 10)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			Operation opComp = p.createOperation("KWayMergeComp", n);
			Operation opAttr = p.createOperation("KWayMergeAttr", n);
			IntLinkedList **list = generateList(10000, n);
			IntLinkedList *output = KWayMerge(list, n, 10000, &opComp, &opAttr);
			intLinkedList_free(output);
		}
	}

	p.divideValues("KWayMergeAttr", NR_TESTS);
	p.divideValues("KWayMergeComp", NR_TESTS);
	p.addSeries("KWayMerge", "KWayMergeAttr", "KWayMergeComp");

	p.showReport();
}

int main()
{
	// testIntLinkedList();
	// demo();
	// demo2();
	// perf1();
	// perf2();
	return 0;
}
