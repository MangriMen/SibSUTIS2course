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


void randomSearchTreeRecursive(Vertex*& pointer, int data, int weight) {
	if (pointer == nullptr) {
		pointer = new Vertex;
		pointer->data = data;
		pointer->weight = weight;
		pointer->left = nullptr;
		pointer->right = nullptr;
	}
	else if (data < pointer->data)randomSearchTreeRecursive(pointer->left, data, weight);
	else if (data > pointer->data)randomSearchTreeRecursive(pointer->right, data, weight);
}

void AWCompute(size_t size) {
	Aw = new int* [size];
	for (int i = 0; i < size; ++i) {
		Aw[i] = new int[size];
	}
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			Aw[i][j] = 0;
		}
	}
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (i == j)
				Aw[i][j] = 0;
		}
	}
	for (int i = 0; i < size; ++i) {
		for (int j = i + 1; j < size; ++j) {
			Aw[i][j] = Aw[i][j - 1] + w[j];
		}
	}
}

void APARCompute(size_t size) {
	Ap = new int* [size];
	Ar = new int* [size];
	for (int i = 0; i < size; ++i) {
		Ap[i] = new int[size];
		Ar[i] = new int[size];
	}


	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			Ap[i][j] = 0;
			Ar[i][j] = 0;
		}
	}

	int i = 0, j = 0;
	while (i < size - 1) {
		j = i + 1;
		Ap[i][j] = Aw[i][j];
		Ar[i][j] = j;
		i++;
	}
	
	int h = 2, m, min, x, k;
	i = 0; j = 0;
	while (h < size) {
		while (i < size - h) {
			j = i + h;
			m = Ar[i][j - 1];
			min = Ap[i][m - 1] + Ap[m][j];
			k = m + 1;
			while (k <= Ar[i + 1][j]) {
				x = Ap[i][k - 1] + Ap[k][j];
				if (x < min) {
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

void createTree(int L, int R) {
	int k;

	if (L < R) {
		k = Ar[L][R];
		randomSearchTreeRecursive(root, V[k], w[k]);
		createTree(L, k - 1);
		createTree(k, R);
	}
}

void traversal(Vertex* pointer, int type) {
	if (pointer != nullptr) {
		if (type == 1) cout << pointer->data << " ";
		traversal(pointer->left, type);
		if (type == 2) cout << pointer->data << " ";
		traversal(pointer->right, type);
		if (type == 3) cout << pointer->data << " ";
	}
}

double searchLevel(Vertex* pointer, int key) {
	int h = 1;
	
	while (pointer != nullptr) {
		if (key < pointer->data) {
			pointer = pointer->left;
			h++;
		} else if (key > pointer->data) {
			pointer = pointer->right;
			h++;
		} else return h;
	}
}

double treeHeight(Vertex* pointer, int size) {
	double W = 0;
	int* l = new int[size];
	int h = 0;
	
	for (int i = 1; i < size; ++i) {
		l[i] = searchLevel(pointer, V[i]);
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

void dispalyTreeInfo(Vertex* pointer, int traversalType, size_t size) {
	cout << "\n-----------------------------------------------------\n\n";
	cout << "Обход " << traversalType << " типа:\n";
	traversal(root, traversalType);
	cout << "\n\nВысота по матрице: " << (double)Ap[0][size - 1] / Aw[0][size - 1] << endl;
	cout << "\nВысота дерева: " << treeHeight(root, size);
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

	AWCompute(numberOfVertex);
	APARCompute(numberOfVertex);

	cout << "\n\nAw:\n";
	printMatrix(Aw, numberOfVertex);
	cout << "Ap:\n";
	printMatrix(Ap, numberOfVertex);
	cout << "Ar:\n";
	printMatrix(Ar, numberOfVertex);

	createTree(0, numberOfVertex - 1);
	dispalyTreeInfo(pointer, traversalType, numberOfVertex);

	cout << "\n\n-----------------------------------------------------\n\n";

	delete pointer;
	delete root;
	delete[] V;
	delete[] w;
}