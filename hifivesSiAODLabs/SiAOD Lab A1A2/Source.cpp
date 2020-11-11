#include <iostream>
#include <ctime>
#include <math.h>
#include <iomanip>
#include <chrono>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <set>
using namespace std;

int* wAFirst;
int* VAFirst;
int* wASecond;
int* VASecond;
int** Aw, ** Ap, ** Ar;
int* w;
int* V;

struct Vertex {
	int data = 0;
	int weight = 0;
	Vertex* left = nullptr;
	Vertex* right = nullptr;
};

Vertex* root{ nullptr };


void randomSearchTreeRecursive(Vertex*& pointer, int D, int weight) {
	if (pointer == nullptr) {
		pointer = new Vertex;
		pointer->data = D;
		pointer->weight = weight;
		pointer->left = nullptr;
		pointer->right = nullptr;
	}
	else if (D < pointer->data)randomSearchTreeRecursive(pointer->left, D, weight);
	else if (D > pointer->data)randomSearchTreeRecursive(pointer->right, D, weight);
}

void A1_add(Vertex*& root, int* VFirst, int* WFirst, size_t size) {
	for (int i = 0; i < size; i++) {
		randomSearchTreeRecursive(root, VFirst[i], WFirst[i]);
	}
}

void A2_add(Vertex*& root, int* VSecond, int* WSecond, int L, int R) {
	double wes = 0, sum = 0;
	if (L <= R) {
		for (int i = L; i <= R; ++i) {
			wes = wes + WSecond[i];
		}
		int i = 0;
		for (i = L; i < R; ++i) {
			if ((sum <= (wes / 2) && (sum + WSecond[i] >= (wes / 2)))) {
				break;
			}
			sum = sum + WSecond[i];
		}
		randomSearchTreeRecursive(root, VSecond[i], WSecond[i]);
		A2_add(root, VSecond, WSecond, L, i - 1);
		A2_add(root, VSecond, WSecond, i + 1, R);
	}
}

void AWCompute(int n) {
	Aw = new int*[n];
	for (int i = 0; i < n; ++i) {
		Aw[i] = new int[n];
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			Aw[i][j] = 0;
		}
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (i == j)
				Aw[i][j] = 0;
		}
	}
	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			Aw[i][j] = Aw[i][j - 1] + w[j];
		}
	}
}

void APARCompute(int n)
{
	Ap = new int* [n];
	Ar = new int* [n];
	for (size_t i = 0; i < n; ++i)
	{
		Ap[i] = new int[n];
		Ar[i] = new int[n];
	}
	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			Ap[i][j] = 0;
			Ar[i][j] = 0;
		}
	}
	int i = 0, j = 0;
	while (i < n - 1)
	{
		j = i + 1;
		Ap[i][j] = Aw[i][j];
		Ar[i][j] = j;
		i++;
	}
	int h = 2, m, min, x, k;
	i = 0; j = 0;
	while (h < n)
	{
		while (i < n - h)
		{
			j = i + h;
			m = Ar[i][j - 1];
			min = Ap[i][m - 1] + Ap[m][j];
			k = m + 1;
			while (k <= Ar[i + 1][j])
			{
				x = Ap[i][k - 1] + Ap[k][j];
				if (x < min)
				{
					m = k;
					min = x;
				}
				k++;
			}
			Ap[i][j] = min + Aw[i][j];
			Ar[i][j] = m;
			i++;
		}
		i = 0;
		h++;
	}
}

void printMatrix(int** matrix, size_t size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			cout << setw(4) << matrix[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

void createTree(Vertex* root, int L, int R) {
	int k;
	if (L < R) {
		k = Ar[L][R];
		randomSearchTreeRecursive(root, V[k], w[k]);
		createTree(root, L, k - 1);
		createTree(root, k, R);
	}
}

/*
void CreateTree(int L, int R)
{
	int k;
	if (L < R)
	{
		k = Ar[L][R];
		recursiveSDP(root, V[k], w[k]);
		CreateTree(L, k - 1);
		CreateTree(k, R);
	}
}
*/

void traversal(Vertex* pointer, int type) {
	if (pointer != nullptr) {
		if (type == 1) cout << pointer->data << " ";
		traversal(pointer->left, type);
		if (type == 2) cout << pointer->data << " ";
		traversal(pointer->right, type);
		if (type == 3) cout << pointer->data << " ";
	}
}

double SearchLevel(Vertex* p, int key)
{
	int h = 1;
	while (p != nullptr)
	{
		if (key < p->data)
		{
			p = p->left;
			h++;
		}
		else if (key > p->data)
		{
			p = p->right;
			h++;
		}
		else return h;
	}
}

double TreeHeight(Vertex* pointer, int size) {
	double W = 0;
	int* l = new int[size];
	int h = 0;
	for (int i = 1; i < size; ++i) {
		l[i] = SearchLevel(pointer, V[i]);
		W += w[i];
		h += w[i] * l[i];
	}

	return (double)h / W;
}

void fillInc(int size, int* arr) {
	for (int i = 0; i < size; i++) {
		arr[i] = i + 1;
	}
}

void fillRand(int size, int* arr) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	set<int> temp;
	for (int i = 0; temp.size() < size; i++) {
		temp.insert(rand() % (size * size));
	}
	vector<int> temp1(temp.size());
	copy(temp.begin(), temp.end(), temp1.begin());
	shuffle(temp1.begin(), temp1.end(), std::default_random_engine(seed));
	for (int i = 0; i < size; i++) {
		arr[i] = temp1[i];
	}
}

int height(Vertex* pointer) {
	return pointer != nullptr ? 1 + max(height(pointer->left), height(pointer->right)) : 0;
}

int amount(Vertex* pointer) {
	return pointer != nullptr ? 1 + amount(pointer->left) + amount(pointer->right) : 0;
}

int sum(Vertex* pointer) {
	return pointer != nullptr ? pointer->data + sum(pointer->left) + sum(pointer->right) : 0;
}

double treeHeightSum(Vertex* pointer, int level) {
	return pointer != nullptr ? level + treeHeightSum(pointer->left, level + 1) + treeHeightSum(pointer->right, level + 1) : 0;
}

//void dispalyTreeInfo(Vertex* pointer, int traversalType, size_t size) {
//	cout << "\n-----------------------------------------------------\n\n";
//	cout << "Обход " << traversalType << " типа:\n";
//	traversal(pointer, traversalType);
//	//cout << "\n\nВысота по матрице: " << (double)Ap[0][size - 1] / Aw[0][size - 1] << endl;
//	cout << "\nВысота дерева: " << TreeHeight(pointer, size);//<< TreeHeight(root, size);
//}

void dispalyTreeInfo(Vertex* root, int traversalType) {
	cout << "\n-----------------------------------------------------\n\n";
	if (root == nullptr) {
		cout << "Дерево пусто.\n";
		return;
	}
	cout << "Обход " << traversalType << " типа:\n";
	traversal(root, traversalType);
	cout << "\n\nКоличество вершин: " << amount(root);
	cout << "\n\nСумма элементов: " << sum(root);
	cout << "\n\nВысота дерева: " << height(root);
	cout << "\n\nСредняя высота дерева: " << treeHeightSum(root, 1) / amount(root);
}

int main() {
	srand(unsigned int(time(NULL)));
	setlocale(LC_ALL, "Russian");

	int numberOfVertex = 0;
	int traversalType = 0;

	cout << "Введите число вершин: ";
	cin >> numberOfVertex;
	cout << "Введите тип прохода (1 - сверху вниз, 2 - справа налево, 3 - снизу вверх): ";
	cin >> traversalType;

	//V = new int[10]{ 21, 60, 79, 14, 7, 34, 40, 35, 29, 56 };
	//V = new int[12]{ 17, 1, 15, 10, 12, 20, 16, 13, 6, 4, 3, 25 };


	w = new int[++numberOfVertex];
	V = new int[numberOfVertex];
	fillRand(numberOfVertex, V);
	fillRand(numberOfVertex, w);

	Vertex* pointer = new Vertex;
	pointer->data = V[0];
	pointer->right = nullptr;
	pointer->left = nullptr;

	for (int i = 0; i < numberOfVertex; ++i) {
		for (int j = i; j < numberOfVertex; ++j) {
			if (V[i] < V[j]) {
				int tmp = V[j];
				V[j] = V[i];
				V[i] = tmp;
			}
		}
	}

	cout << "\nИзначальный массив V:\n";
	for (int i = 1; i < numberOfVertex; ++i) {
		cout << V[i] << "(" << w[i] << ")" << " ";
	}

	numberOfVertex -= 1;
	AWCompute(numberOfVertex);
	APARCompute(numberOfVertex);

	A1_add(pointer, V, w, numberOfVertex);
	dispalyTreeInfo(pointer, traversalType);

	cout << "\n\n-----------------------------------------------------\n\n";

	delete pointer;
	delete[] V;
	delete[] w;
}