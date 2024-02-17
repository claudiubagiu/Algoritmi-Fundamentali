#include <iostream>
#include <iomanip>
#include "Profiler.h"
#include <time.h>

using namespace std;

#define HASH_SIZE 10007

struct Entry
{
	int id = -1;
};

int h(int k)
{
	return k % HASH_SIZE;
}

int h(int k, int i)
{
	return (h(k) + 2 * i + 3 * i * i) % HASH_SIZE;
}

void hashInsert(Entry* T, int k, int n)
{
	int i = 0, j;
	do
	{
		j = h(k, i);
		if (T[j].id == -1 || T[j].id == -2)
		{
			T[j].id = k;
			return;
		}
		else
			i++;
	} while (i < n);
	cout << "Error! " << endl;
	return;
}

int hashSearch(Entry T[], int k, int n, int& nrOp)
{
	int i = 0, j;
	do
	{
		nrOp++;
		j = h(k, i);
		if (T[j].id == k)
			return j;
		if (T[j].id == -1)
			return -1;
		i++;
	} while (i < n);
	return -1;
}


void hashDelete(Entry T[], int k, int n)
{
	int i = 0, j;
	do
	{
		j = h(k, i);
		if (T[j].id == k)
		{
			T[j].id = -2;
			return;
		}
		else
			i++;
	} while (i < n);
	return;
}

Entry* generateHashTable(double factorUmplere)
{
	int v[HASH_SIZE];
	Entry* T = new Entry[HASH_SIZE];
	FillRandomArray(v, HASH_SIZE, 0, 50000, true, 0);
	int i = 0;
	while ((double)i / HASH_SIZE < factorUmplere)
	{
		hashInsert(T, v[i], HASH_SIZE);
		i++;
	}
	return T;
}

Entry* deleteHashTableElements(Entry* T, double factorUmplere)
{
	double size = 0;
	for (int i = 0; i < HASH_SIZE; i++)
	{
		if (T[i].id > -1)
			size++;
	}
	while (size / HASH_SIZE > factorUmplere)
	{
		int random = rand() % 50000;
		int op = 0;
		if (hashSearch(T, random, HASH_SIZE, op) != -1)
		{
			hashDelete(T, random, HASH_SIZE);
			size--;
		}
	}
	return T;
}

void generateTable(Entry T[])
{
	double efortMediu = 0.0;
	double efortMaxim = 0.0;
	int efortTotal = 0;

	int elementsFound = 0;
	while (elementsFound < 1500)
	{
		int element = rand() % 50000;
		int nrOp = 0;
		if (hashSearch(T, element, HASH_SIZE, nrOp) > -1)
		{
			elementsFound++;
			efortTotal = efortTotal + nrOp;
			if (efortMaxim < nrOp)
				efortMaxim = nrOp;
		}
	}
	efortMediu = (double)efortTotal / 1500.0;
	cout << left << setw(24) << efortMediu << left << setw(24) << efortMaxim;


	efortMediu = 0.0;
	efortMaxim = 0.0;
	efortTotal = 0;

	int elementsUnfound = 0;
	while (elementsUnfound < 1500)
	{
		int element = rand() % 10000;
		int nrOp = 0;
		if (hashSearch(T, element, HASH_SIZE, nrOp) < 0)
		{
			elementsUnfound++;
			if (efortMaxim < nrOp)
				efortMaxim = nrOp;
			efortTotal = efortTotal + nrOp;
		}

	}
	efortMediu = (double)efortTotal / 1500.0;
	cout << left << setw(24) << efortMediu << left << setw(24) << efortMaxim << endl;
}

void perf()
{
	cout << left << setw(24) << "Factor de umplere";
	cout << left << setw(24) << "Efort mediu gasite";
	cout << left << setw(24) << "Efort maxim gasite";
	cout << left << setw(24) << "Efort mediu negasite";
	cout << left << setw(24) << "Efort maxim negasite" << endl;
	Entry* T = generateHashTable(0.8);
	cout << left << setw(24) << 0.8;
	generateTable(T);
	free(T);
	T = generateHashTable(0.85);
	cout << left << setw(24) << 0.85;
	generateTable(T);
	free(T);
	T = generateHashTable(0.9);
	cout << left << setw(24) << 0.9;
	generateTable(T);
	free(T);
	T = generateHashTable(0.95);
	cout << left << setw(24) << 0.95;
	generateTable(T);
	free(T);
	T = generateHashTable(0.99);
	cout << left << setw(24) << 0.99;
	generateTable(T);
	free(T);

	T = generateHashTable(0.99);
	T = deleteHashTableElements(T, 0.8);
	cout << left << setw(24) << "Stergere";
	generateTable(T);
	free(T);
}

void demo()		// trebuie schimbat HASH_SIZE in 13
{
	Entry T[13];
	hashInsert(T, 12, 13);
	hashInsert(T, 17, 13);
	hashInsert(T, 4, 13);
	hashInsert(T, 1, 13);
	hashInsert(T, 5, 13);
	hashInsert(T, 3, 13);

	for (int i = 0; i < 13; i++)
		cout << T[i].id << " ";
	cout << endl;

	int op1 = 0, op2 = 0;
	cout << hashSearch(T, 17, 13, op1) << " " << op1 << endl;
	cout << hashSearch(T, 4, 13, op2) << " " << op2 << endl;

	hashDelete(T, 4, 13);
	cout << hashSearch(T, 4, 13, op2) << " " << op2;

}

int main()
{
	srand(static_cast<unsigned int>(time(NULL)));
	perf();
	return 0;
}