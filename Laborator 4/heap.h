#pragma once

int left(int i);

int right(int i);

void minHeapify(IntLinkedList* a[], int heapSize, int i, Operation* opComp, Operation* opAttr);

void buildMinHeap_bottomUp(IntLinkedList* a[], int heapSize, Operation* opComp, Operation* opAttr);

void heapSort(IntLinkedList* a[], int heapSize, int maxSize, Operation* opComp, Operation* opAttr);

int extractMin(IntLinkedList* a[], int& heapSize, Operation* opComp, Operation* opAttr);