#include <iostream>
#include <ctime>
#include <algorithm>
#include <vector>
#include <set>
#include <chrono>
#include <random>
#include <iomanip>
using namespace std;

int* A;
int* w;
int* V;
int* l;
int** Aw, ** Ap, ** Ar;

struct Vertex {
	int data = 0;
	int weight = 0;
	Vertex* left = nullptr;
	Vertex* right = nullptr;
};

Vertex* root = nullptr;

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

void A1_add(Vertex*& root, int* V, int* W, int size) {
	for (int i = 0; i < size; i++) {
		randomSearchTreeRecursive(root, V[i], W[i]);
	}
}

void A2_add(Vertex*& root, int* VSecond, int* WSecond, int left, int right) {
	double wes = 0, sum = 0;
	if (left <= right) {
		for (int i = left; i <= right; ++i) {
			wes = wes + WSecond[i];
		}
		int i = 0;
		for (i = left; i < right; ++i) {
			if ((sum <= (wes / 2) && (sum + WSecond[i] >= (wes / 2)))) {
				break;
			}
			sum = sum + WSecond[i];
		}
		randomSearchTreeRecursive(root, VSecond[i], WSecond[i]);
		A2_add(root, VSecond, WSecond, left, i - 1);
		A2_add(root, VSecond, WSecond, i + 1, right);
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

void AWCompute(int n) {
	Aw = new int* [n];
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

void APARCompute(int n) {
	Ap = new int* [n];
	Ar = new int* [n];
	for (int i = 0; i < n; ++i) {
		Ap[i] = new int[n];
		Ar[i] = new int[n];
	}
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			Ap[i][j] = 0;
			Ar[i][j] = 0;
		}
	}

	int i = 0, j = 0;
	while (i < n - 1) {
		j = i + 1;
		Ap[i][j] = Aw[i][j];
		Ar[i][j] = j;
		i++;
	}
	int h = 2, m, min, x, k;
	i = 0; j = 0;
	while (h < n) {
		while (i < n - h) {
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

void printMatrix(int** a, int n) {
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			cout << a[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

void createTree(int left, int right) {
	int k;

	if (left < right) {
		k = Ar[left][right];
		randomSearchTreeRecursive(root, V[k], w[k]);
		createTree(left, k - 1);
		createTree(k, right);
	}
}

int searchLevel(Vertex* pointer, int key) {
	int h = 1;

	while (pointer != nullptr) {
		if (key < pointer->data) {
			pointer = pointer->left;
			h++;
		}
		else if (key > pointer->data) {
			pointer = pointer->right;
			h++;
		}
		else return h;
	}
}


double treeHeight(Vertex* pointer, int level) {
	return pointer != nullptr ? ((level * pointer->weight) + treeHeight(pointer->left, level + 1) + treeHeight(pointer->right, level + 1)) : 0;
}

int weightSum(Vertex* pointer, int level) {
	return pointer != nullptr ? (pointer->weight + weightSum(pointer->left, level + 1) + weightSum(pointer->right, level + 1)) : 0;
}

int sum(Vertex* pointer) {
	return pointer != nullptr ? pointer->data + sum(pointer->left) + sum(pointer->right) : 0;
}

int amount(Vertex* pointer) {
	return pointer != nullptr ? 1 + amount(pointer->left) + amount(pointer->right) : 0;
}

double treeHeightSum(Vertex* pointer, int level) {
	return pointer != nullptr ? level + treeHeightSum(pointer->left, level + 1) + treeHeightSum(pointer->right, level + 1) : 0;
}

int height(Vertex* pointer) {
	return pointer != nullptr ? 1 + max(height(pointer->left), height(pointer->right)) : 0;
}

void dispalyTreeInfo(Vertex* root, int traversalType) {
	cout << "\n-------------------------------------------------------------\n\n";
	if (root == nullptr) {
		cout << "Дерево пусто.\n";
		return;
	}
	cout << "Обход " << traversalType << " типа:\n";
	traversal(root, traversalType);
	cout << "\n\nКоличество вершин: " << amount(root);
	cout << "\n\nСумма элементов: " << sum(root);
	cout << "\n\nВысота дерева: " << height(root);
	cout << "\n\nСредняя высота дерева: " << treeHeightSum(root, 1) / (double)amount(root);
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

int main() {
	srand(unsigned int(time(NULL)));
	setlocale(LC_ALL, "Russian");

	int numberOfVertex = 0;
	int traversalType = 0;

	cout << "Введите число вершин: ";
	cin >> numberOfVertex;
	cout << "Введите тип прохода (1 - сверху вниз, 2 - справа налево, 3 - снизу вверх): ";
	cin >> traversalType;

	w = new int[numberOfVertex + 1];
	V = new int[numberOfVertex + 1];
	fillRand(numberOfVertex + 1, V);
	fillRand(numberOfVertex + 1, w);
	int* V1 = new int[numberOfVertex + 1], * W1 = new int[numberOfVertex + 1];
	int* V2 = new int[numberOfVertex + 1], * W2 = new int[numberOfVertex + 1];
	int temp;
	
	for (int i = 0; i < numberOfVertex + 1; i++) {
		for (int j = i; j < numberOfVertex + 1; ++j) {
			if (V[i] < V[j]) {
				temp = V[j];
				V[j] = V[i];
				V[i] = temp;
			}
		}
	}

	AWCompute(numberOfVertex + 1);
	APARCompute(numberOfVertex + 1);
	cout << "\n\nМатрица Aw:\n";
	printMatrix(Aw, numberOfVertex + 1);
	cout << "Матрица Ap:\n";
	printMatrix(Ap, numberOfVertex + 1);
	cout << "Матрица Ar:\n";
	printMatrix(Ar, numberOfVertex + 1);

	cout << "\n\n--------------------------------\n";
	cout << "\nДерево оптимального поиска:\n";
	createTree(0, numberOfVertex);
	cout << "\n--------------------------------\n\n";
	cout << "Обход " << traversalType << " типа:\n";
	traversal(root, traversalType);
	cout << "\n\nКоличество вершин: " << amount(root);
	cout << "\n\nСумма элементов: " << sum(root);
	cout << "\n\nВысота по матрице: " << (double)Ap[0][numberOfVertex] / Aw[0][numberOfVertex] << endl;
	cout << "\nВысота дерева: " << treeHeight(root, 1) / (double)weightSum(root, 1);

	delete root;
	root = nullptr;

	//Копируем
	for (int i = 0; i < numberOfVertex; i++) {
		V1[i] = V[i + 1];
		W1[i] = w[i + 1];
		V2[i] = V[i + 1];
		W2[i] = w[i + 1];
	}
	//Сортировка весов
	for (int i = 0; i < numberOfVertex; i++) {
		for (int j = 0; j < numberOfVertex; j++) {
			if (W1[i] < W1[j]) {
				temp = W1[i];
				W1[i] = W1[j];
				W1[j] = temp;
				temp = V1[i];
				V1[i] = V1[j];
				V1[j] = temp;
			}
		}
	}
	//Сортировка вершин
	for (int i = 0; i < numberOfVertex; i++) {
		for (int j = 0; j < numberOfVertex; j++) {
			if (V2[i] < V2[j]) {
				temp = W2[i];
				W2[i] = W2[j];
				W2[j] = temp;
				temp = V2[i];
				V2[i] = V2[j];
				V2[j] = temp;
			}
		}
	}

	cout << "\n\n-------------------------------------------------------------\n";
	cout << "\nДерево A1:\n";
	cout << "\nИзначальный массив:\n";
	for (int i = 0; i < numberOfVertex; ++i) {
		cout << V1[i] << "(" << W1[i] << ")" << " ";
	}
	A1_add(root, V1, W1, numberOfVertex);
	dispalyTreeInfo(root, traversalType);

	delete root;
	root = nullptr;

	cout << "\n\n-------------------------------------------------------------\n";
	cout << "\nДерево A2:\n";
	cout << "\nИзначальный массив:\n";
	for (int i = 0; i < numberOfVertex; ++i) {
		cout << V2[i] << "(" << W2[i] << ")" << " ";
	}
	A2_add(root, V2, W2, 0, numberOfVertex - 1);
	dispalyTreeInfo(root, traversalType);
	
	cout << "\n\n-------------------------------------------------------------\n";

	delete root;
}