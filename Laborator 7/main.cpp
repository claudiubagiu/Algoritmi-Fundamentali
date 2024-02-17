#include <iostream>
#include <vector>
#include "Profiler.h"

using namespace std;

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler p("AVL");

/*
 * Pentru a crea arborele echilibrat am folosit Divide and Conquer.
 * Pentru OS_Delete am folosit ștergerea dintr-un arbore binar de căutare, apeland OS_Select.
 * OS_Select seamana cu Randomized_Select
 */

struct Node
{
	int key;
	Node *left;
	Node *right;
	int size;
};

int calculateSize(Node *root, Operation *opComp, Operation *opAttr)
{
	if (root == nullptr)
		return 0;
	opAttr->count();
	root->size = 1;
	opComp->count();
	if (root->left)
	{
		opAttr->count();
		root->size = root->size + root->left->size;
	}
	opComp->count();
	if (root->right)
	{
		opAttr->count();
		root->size = root->size + root->right->size;
	}
	return root->size;
}

Node *buildTree(int left, int right, Operation *opComp, Operation *opAttr)
{
	if (left > right)
		return nullptr;

	int middle = left + (right - left) / 2;
	Node *root = new Node;
	opAttr->count();
	root->key = middle;

	opAttr->count();
	root->left = buildTree(left, middle - 1, opComp, opAttr);
	opAttr->count();
	root->right = buildTree(middle + 1, right, opComp, opAttr);

	opAttr->count();
	root->size = 1;
	opComp->count();
	if (root->left)
	{
		opAttr->count();
		root->size = root->size + root->left->size;
	}
	opComp->count();
	if (root->right)
	{
		opAttr->count();
		root->size = root->size + root->right->size;
	}

	return root;
}

Node *useBuildTree(int left, int right)
{
	Operation opComp = p.createOperation("buildTreeComp", right);
	Operation opAttr = p.createOperation("buildTreeAttr", right);
	Node *root = buildTree(left, right, &opComp, &opAttr);
	return root;
}

Node *OS_Select(Node *root, int i, Operation *opComp, Operation *opAttr)
{
	opComp->count();
	if (root == nullptr)
		return nullptr;
	int r;
	opComp->count();
	opAttr->count();
	if (root->left == nullptr)
		r = 1;
	else
		r = root->left->size + 1;

	opComp->count();
	if (i == r)
		return root;
	else
	{
		opComp->count();
		if (i < r)
			return OS_Select(root->left, i, opComp, opAttr);
		else
			return OS_Select(root->right, i - r, opComp, opAttr);
	}
}

Node *useOS_Select(Node *root, int i, int size)
{
	Operation opComp = p.createOperation("OS_SelectComp", size);
	Operation opAttr = p.createOperation("OS_SelectAttr", size);
	return OS_Select(root, i, &opComp, &opAttr);
}

Node *findMin(Node *root, Operation *opComp, Operation *opAttr)
{

	while (root->left != nullptr)
	{
		opComp->count();
		opAttr->count();
		root = root->left;
	}
	opComp->count();
	return root;
}

Node *OS_Delete(Node *root, int key, Operation *opComp, Operation *opAttr)
{
	if (root == nullptr)
		return nullptr;
	opComp->count();
	if (root->key > key)
	{
		opAttr->count();
		root->left = OS_Delete(root->left, key, opComp, opAttr);
	}
	else
	{
		opComp->count();
		if (root->key < key)
		{
			opAttr->count();
			root->right = OS_Delete(root->right, key, opComp, opAttr);
		}
		else
		{
			opComp->count();
			if (root->left == nullptr && root->right == nullptr)
			{
				delete root;
				return nullptr;
			}
			else
			{
				opComp->count();
				if (root->left == nullptr)
				{
					opAttr->count();
					Node *temp = root->right;
					delete root;
					return temp;
				}
				else
				{
					opComp->count();
					if (root->right == nullptr)
					{
						opAttr->count();
						Node *temp = root->left;
						delete root;
						return temp;
					}
					else
					{
						opAttr->count();
						Node *temp = findMin(root->right, opComp, opAttr);
						opAttr->count();
						root->key = temp->key;
						opAttr->count();
						root->right = OS_Delete(root->right, temp->key, opComp, opAttr);
					}
				}
			}
		}
	}
	root->size = calculateSize(root, opComp, opAttr);
	return root;
}

Node *useOS_Delete(Node *root, int i, int size)
{
	Operation opComp = p.createOperation("OS_DeleteComp", size);
	Operation opAttr = p.createOperation("OS_DeleteAttr", size);
	int key = OS_Select(root, i, &opComp, &opAttr)->key;
	Node *node = OS_Delete(root, key, &opComp, &opAttr);
	return node;
}

void displayBinaryTree(Node *root, int depth = 0)
{
	if (root == nullptr)
		return;
	for (int i = 0; i < depth; i++)
		cout << "    ";
	cout << root->key << "(size = " << root->size << ")" << endl;
	displayBinaryTree(root->right, depth + 1);
	displayBinaryTree(root->left, depth + 1);
}

void perf()
{
	int v[MAX_SIZE];
	int n;
	for (n = STEP_SIZE; n < MAX_SIZE; n += STEP_SIZE)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(v, n, 1, n, true, UNSORTED);
			for (int i = 0; i < n; i++)
			{
				int s = v[i] - i;
				if (s < 1)
					v[i] = 1;
				else
					v[i] = v[i] - i;
			}
			Node *root = useBuildTree(1, n);
			for (int i = 0; i < n; i++)
			{
				Node *node;
				node = useOS_Select(root, v[i], n);
				root = useOS_Delete(root, v[i], n);
			}
		}
	}

	p.divideValues("buildTreeAttr", NR_TESTS);
	p.divideValues("buildTreeComp", NR_TESTS);
	p.addSeries("BuildTree", "buildTreeAttr", "buildTreeComp");

	p.divideValues("OS_SelectAttr", NR_TESTS);
	p.divideValues("OS_SelectComp", NR_TESTS);
	p.addSeries("OS_Select", "OS_SelectAttr", "OS_SelectComp");

	p.divideValues("OS_DeleteAttr", NR_TESTS);
	p.divideValues("OS_DeleteComp", NR_TESTS);
	p.addSeries("OS_Delete", "OS_DeleteAttr", "OS_DeleteComp");

	p.createGroup("Atribuiri", "buildTreeAttr", "OS_SelectAttr", "OS_DeleteAttr");
	p.createGroup("Comparatii", "buildTreeComp", "OS_SelectComp", "OS_DeleteComp");
	p.createGroup("Total", "BuildTree", "OS_Select", "OS_Delete");

	p.showReport();
}

void demo()
{
	Node *root = useBuildTree(1, 11);
	displayBinaryTree(root);
	cout << endl;

	cout << endl;

	root = useOS_Delete(root, 4, root->size);
	cout << endl;
	root = useOS_Delete(root, 4, root->size);

	displayBinaryTree(root);
	cout << endl;

	Node *node = useOS_Select(root, 4, root->size);

	cout << node->key;
}

int main()
{

	perf();
	// demo();

	return 0;
}