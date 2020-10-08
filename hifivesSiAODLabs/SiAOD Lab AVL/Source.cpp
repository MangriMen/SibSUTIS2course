#include <iostream>
#include <ctime>
#include <math.h>
using namespace std;

bool isIncreased = true;
int* A;

struct Vertex {
	int data = 0;
	int balance = 0;
	Vertex* left = nullptr;
	Vertex* right = nullptr;
};

void AVLTreeAddNode(Vertex*& pointer, int data) {
	if (pointer == nullptr) {
		pointer = new Vertex;
		pointer->data = data;
		pointer->balance = 0;
		pointer->left = nullptr;
		pointer->right = nullptr;
		isIncreased = true;
	} else if (pointer->data > data) {
		AVLTreeAddNode(*&pointer->left, data);
		if (isIncreased) {
			if (pointer->balance > 0) {
				pointer->balance = 0;
				isIncreased = false;
			} else if (pointer->balance == 0) {
				pointer->balance = -1;
				isIncreased = true;
			} else {
				if (pointer->left->balance < 0) {
					//leftLeftTurn(pointer);
					isIncreased = false;
				} else {
					//leftRightTurn(pointer);
					isIncreased = false;
				}
			}
		}
	} else if (pointer->data < data) {
		AVLTreeAddNode(*&pointer->right, data);
		if (isIncreased) {
			if (pointer->balance < 0) {
				pointer->balance = 0;
				isIncreased = false;
			} else if (pointer->balance == 0) {
				pointer->balance = 1;
				isIncreased = true;
			} else {
				if (pointer->right->balance > 0) {
					//rightRightTurn(pointer);
					isIncreased = false;
				}
				else {
					//rightLeftTurn(pointer);
					isIncreased = false;
				}
			}
		}
	}
}

void traversal(Vertex* pointer, int type) {
	if (pointer != nullptr) {
		if (type == 1) cout << pointer->data << "(" << pointer->balance << ") ";
		traversal(pointer->left, type);
		if (type == 2) cout << pointer->data << "(" << pointer->balance << ") ";
		traversal(pointer->right, type);
		if (type == 3) cout << pointer->data << "(" << pointer->balance << ") ";
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

void fillInc(int size) {
	for (int i = 0; i < size; i++) {
		A[i] = i + 1;
	}
}

void fillRand(int size) {
	for (int i = 0; i < size; i++) {
		A[i] = rand() % (size * size) + size;
	}
}

void dispalyVertexInfo(Vertex* root, int traversalType) {
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
	cout << "\n\nМаксимальная высота дерева: " << treeHeightSum(root, 1) / amount(root);
}

int main() {
	srand(unsigned int(time(NULL)));
	setlocale(LC_ALL, "Russian");

	int numberOfVertex;
	int traversalType;

	cout << "Введите число вершин: ";
	cin >> numberOfVertex;
	cout << "Введите тип прохода (1 - сверху вниз, 2 - справа налево, 3 - снизу вверх): ";
	cin >> traversalType;

	A = new int[numberOfVertex];
	fillRand(numberOfVertex);
	//A = new int[10]{ 21, 60, 79, 14, 7, 34, 40, 35, 29, 56 };
	//A = new int[12]{ 17, 1, 15, 10, 12, 20, 16, 13, 6, 4, 3, 25 };

	Vertex* root = new Vertex;
	root->data = A[0];
	root->right = nullptr;
	root->left = nullptr;

	cout << "\nИзначальный массив А:\n";
	for (int i = 0; i < numberOfVertex; ++i) {
		cout << A[i] << " ";
	}

	for (int i = 0; i < numberOfVertex; ++i) {
		AVLTreeAddNode(root, A[i]);
	}

	dispalyVertexInfo(root, traversalType);

	cout << "\n\n-----------------------------------------------------\n\n";

	delete root;
	delete A;
}