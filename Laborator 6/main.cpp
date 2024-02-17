#include <iostream>
#include <vector>

using namespace std;

/*
 * Complexitatea la functia createTree este O(n). Aceasta functie creeaza arborele bottom-up.
 * Complexitatea la functia KArayTreeToBinaryTree este O(n). Aceasta functie creeaza arborele top-down. LeftChild este primul copil al arborelui multicai,
 * iar RightChild este fratele nodului curent.
 */

class Node
{
private:
	int key;
	vector<Node *> children;

public:
	Node(int key)
	{
		this->key = key;
	}

	vector<Node *> getChildren()
	{
		return children;
	}

	int getKey()
	{
		return key;
	}

	void addChild(Node *child)
	{
		children.push_back(child);
	}
};

class BinaryTreeNode
{
private:
	int key;
	BinaryTreeNode *left;
	BinaryTreeNode *right;

public:
	BinaryTreeNode(int key)
	{
		this->key = key;
		this->left = this->right = nullptr;
	}
	int getKey()
	{
		return key;
	}
	BinaryTreeNode *getLeftChild()
	{
		return left;
	}
	BinaryTreeNode *getRightChild()
	{
		return right;
	}
	void addLeftChild(BinaryTreeNode *child)
	{
		left = child;
	}
	void addRightChild(BinaryTreeNode *child)
	{
		right = child;
	}
};

void createNode(int parent[], int index, vector<Node *> &nodes, Node **root)
{
	if (nodes[index] != nullptr)
		return;
	nodes[index] = new Node(index + 1);

	if (parent[index] == -1)
	{
		*root = nodes[index];
		return;
	}
	if (nodes[parent[index] - 1] == nullptr)
		createNode(parent, parent[index] - 1, nodes, root);
	Node *parentNode = nodes[parent[index] - 1];
	parentNode->addChild(nodes[index]);
}

Node *createTree(int parent[], int n)
{
	vector<Node *> nodes(n, nullptr);
	Node *root = nullptr;

	for (int i = 0; i < n; i++)
		createNode(parent, i, nodes, &root);
	return root;
}

BinaryTreeNode *KArayTreeToBinaryTree(Node *root)
{
	if (root == nullptr)
		return nullptr;

	BinaryTreeNode *binaryTreeRoot = new BinaryTreeNode(root->getKey());

	if (root->getChildren().size() > 0)
		binaryTreeRoot->addLeftChild(KArayTreeToBinaryTree(root->getChildren()[0]));

	BinaryTreeNode *node = binaryTreeRoot->getLeftChild();

	for (int i = 1; i < root->getChildren().size(); i++)
	{
		Node *child = root->getChildren()[i];
		node->addRightChild(KArayTreeToBinaryTree(child));
		node = node->getRightChild();
	}
	return binaryTreeRoot;
}

void displayParentArray(int parent[], int size, int root = -1, int depth = 0)
{
	for (int i = 0; i < size; i++)
		if (parent[i] == root)
		{
			for (int j = 0; j < depth; j++)
				cout << "    ";
			cout << i + 1 << endl;
			displayParentArray(parent, size, i + 1, depth + 1);
		}
}

void displayKaryTree(Node *root, int depth = 0)
{
	if (root == nullptr)
		return;

	cout << root->getKey();

	const vector<Node *> children = root->getChildren();

	cout << endl;

	for (int i = 0; i < children.size(); ++i)
	{
		for (int j = 0; j <= depth; ++j)
			cout << "    ";
		displayKaryTree(children[i], depth + 1);
	}
}

void displayBinaryTree(BinaryTreeNode *root, int depth = 0)
{
	if (root == nullptr)
		return;
	for (int i = 0; i < depth; i++)
		cout << "    ";
	cout << root->getKey() << endl;
	displayBinaryTree(root->getLeftChild(), depth + 1);
	displayBinaryTree(root->getRightChild(), depth);
}

void demo()
{
	int parent[] = {2, 7, 5, 2, 7, 7, -1, 5, 2};
	int n = sizeof(parent) / sizeof(parent[0]);

	cout << "Reprezentarea dupa Parent Array: " << endl;
	displayParentArray(parent, n);
	cout << endl;

	Node *root = createTree(parent, n);
	cout << "Reprezentarea dupa K-Array Tree: " << endl;
	displayKaryTree(root);
	cout << endl;

	BinaryTreeNode *binaryTreeRoot = KArayTreeToBinaryTree(root);
	cout << "Reprezentarea dupa Binary Tree: " << endl;
	displayBinaryTree(binaryTreeRoot);
	cout << endl;
}

int main()
{
	demo();
	return 0;
}