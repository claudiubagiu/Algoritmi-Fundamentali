#include "Profiler.h"

struct Node
{
	int data;
	struct Node* next;
};

struct IntLinkedList
{
	Node* first;
	Node* last;
};

/**
 * Creates an empty linked list for integers.
 * @return A pointer to the created empty linked list.
 */
IntLinkedList* intLinkedList_new()
{
	IntLinkedList* list = new IntLinkedList;
	if (list == nullptr)
		return nullptr;
	list->first = nullptr;
	list->last = nullptr;
	return list;
}

IntLinkedList* intLinkedList_new(Operation* opComp, Operation* opAttr)
{
	IntLinkedList* list = new IntLinkedList;
	if (list == nullptr)
		return nullptr;
	opAttr->count();
	list->first = nullptr;
	opAttr->count();
	list->last = nullptr;
	return list;
}

/**
 * Frees the memory occupied by the linked list.
 * @param list A pointer to the linked list to be freed.
 */
void intLinkedList_free(IntLinkedList* list)
{
	Node* current = list->first;
	while (current != nullptr)
	{
		Node* next = current->next;
		delete current;
		current = next;
	}
	if (list != nullptr)
		delete list;
}

/**
 * Returns the number of elements in the linked list.
 * @param list A pointer to the linked list.
 * @return The number of elements in the linked list.
 * @exception If the linked list is NULL, 0 is returned.
 */
int intLinkedList_size(IntLinkedList* list)
{
	if (list == nullptr)
		return 0;
	int size = 0;
	Node* current = list->first;
	while (current != nullptr)
	{
		size++;
		current = current->next;
	}
	return size;
}

/**
 * Returns a pointer to the first element in the linked list.
 * @param list A pointer to the linked list.
 * @return A pointer to the first element in the linked list. If the linked list is empty, NULL is returned.
 * @exception If the linked list is NULL, NULL is returned.
 */
int* intLinkedList_front(IntLinkedList* list)
{
	if (list == nullptr || list->first == nullptr)
		return nullptr;
	return &list->first->data;
}

/**
 * Returns a pointer to the last element in the linked list.
 * @param list A pointer to the linked list.
 * @return A pointer to the last element in the linked list. If the linked list is empty, NULL is returned.
 * @exception If the linked list is NULL, NULL is returned.
 */
int* intLinkedList_back(IntLinkedList* list)
{
	if (list == nullptr || list->last == nullptr)
		return nullptr;
	return &list->last->data;
}

/**
 * Adds a new value at the beginning of the linked list.
 * @param list A pointer to the linked list.
 * @param value The value to be added.
 */
void intLinkedList_push_front(IntLinkedList* list, int value)
{
	Node* new_node = new Node;
	if (new_node == nullptr)
		return;
	new_node->data = value;
	new_node->next = list->first;
	list->first = new_node;
	if (list->last == nullptr)
		list->last = new_node;
}

/**
 * Removes the first element in the linked list.
 * @param list A pointer to the linked list.
 */
void intLinkedList_pop_front(IntLinkedList* list, Operation* opComp, Operation* opAttr)
{
	if (list == nullptr || list->first == nullptr)
		return;
	opAttr->count();
	Node* to_remove = list->first;
	opAttr->count();
	list->first = to_remove->next;
	opComp->count();
	if (list->last == to_remove)
	{
		opAttr->count();
		list->last = nullptr;
	}
	delete to_remove;
}

void intLinkedList_pop_front(IntLinkedList* list)
{
	if (list == nullptr || list->first == nullptr)
		return;
	Node* to_remove = list->first;
	list->first = to_remove->next;
	if (list->last == to_remove)
		list->last = nullptr;
	delete to_remove;
}

/**
 * Appends the specified element to the end of the linked list.
 * @param list A pointer to the linked list.
 * @param value The value to be added.
 */
void intLinkedList_push_back(IntLinkedList* list, int value, Operation* opComp, Operation* opAttr)
{
	Node* new_node = new Node;
	if (new_node == nullptr)
		return;
	opAttr->count();
	new_node->data = value;
	opAttr->count();
	new_node->next = nullptr;
	opComp->count();
	if (list->last == nullptr)
	{
		opAttr->count();
		list->first = new_node;
	}
	else
	{
		opAttr->count();
		list->last->next = new_node;
	}
	opAttr->count();
	list->last = new_node;
}

void intLinkedList_push_back(IntLinkedList* list, int value)
{
	Node* new_node = new Node;
	if (new_node == nullptr)
		return;
	new_node->data = value;
	new_node->next = nullptr;
	if (list->last == nullptr)
	{
		list->first = new_node;
	}
	else
	{
		list->last->next = new_node;
	}
	list->last = new_node;
}


/**
 * Removes the last element in the linked list.
 * @param list A pointer to the linked list.
 */
void intLinkedList_pop_back(IntLinkedList* list)
{
	if (list->first == nullptr)
		return;
	if (list->first == list->last)
	{
		delete list->last;
		list->first = nullptr;
		list->last = nullptr;
		return;
	}
	Node* current = list->first;
	while (current->next != list->last)
		current = current->next;
	delete list->last;
	list->last = current;
	list->last->next = nullptr;
}

/**
 * Removes all elements from the linked list.
 * @param list A pointer to the linked list.
 */
void intLinkedList_clear(IntLinkedList* list)
{
	Node* current = list->first;
	while (current != nullptr)
	{
		Node* next = current->next;
		delete current;
		current = next;
	}
	list->first = nullptr;
	list->last = nullptr;
}

/**
 * Removes all occurrences of the specified element from the linked list.
 * @param list A pointer to the linked list.
 * @param value The value to be removed.
 */
void intLinkedList_remove(IntLinkedList* list, int value)
{
	Node* current = list->first;
	Node* previous = nullptr;
	while (current != nullptr)
	{
		if (current->data == value)
		{
			if (previous == nullptr)
				list->first = current->next;
			else
				previous->next = current->next;
			if (current->next == nullptr)
				list->last = previous;
			Node* next = current->next;
			delete current;
			current = next;
		}
		else
		{
			previous = current;
			current = current->next;
		}
	}
}

/**
 * Inserts the specified element at the specified position in the linked list.
 * @param list A pointer to the linked list.
 * @param position The position at which to insert the element.
 * @param value The value to be added.
 */
void intLinkedList_push_at(IntLinkedList* list, unsigned position, int value)
{
	if (list == nullptr || position > (unsigned)intLinkedList_size(list))
		return;
	if (position == 0)
	{
		intLinkedList_push_front(list, value);
		return;
	}
	if (position == intLinkedList_size(list))
	{
		intLinkedList_push_back(list, value);
		return;
	}
	Node* current = list->first;
	Node* previous = nullptr;
	if (previous == nullptr)
		return;
	unsigned i = 0;
	while (i < position - 1)
	{
		previous = current;
		current = current->next;
		i++;
	}
	Node* new_node = new Node;
	if (new_node == nullptr)
		return;
	new_node->data = value;
	new_node->next = current;
	previous->next = new_node;
}

/**
 * Checks if the list contains the specified element.
 * @param list Reference to a list.
 * @param value The value to be searched.
 * @return 1 if the list contains the value, 0 otherwise.
 * @exception If list is NULL, 0 is returned.
 */
int intLinkedList_contains(IntLinkedList* list, int value)
{
	if (list == nullptr || list->first == nullptr)
		return 0;
	Node* current = list->first;
	while (current != nullptr)
	{
		if (current->data == value)
			return 1;
		current = current->next;
	}
	return 0;
}

/**
 * Iterates through elements of the list and calls the provided function with the element's reference as a parameter.
 * @param list Reference to a list.
 * @param context Reference to a context to be passed to the function.
 * @param func Reference to a function to be called.
 *
 * The function should have the following signature:
 *    void func(int *element, void *context)
 */
void intLinkedList_forEach(IntLinkedList* list, void* context, void (*func)(int*, void*))
{
	if (list == nullptr || list->first == nullptr)
		return;
	Node* current = list->first;
	while (current != nullptr)
	{
		func(&current->data, context);
		current = current->next;
	}
}