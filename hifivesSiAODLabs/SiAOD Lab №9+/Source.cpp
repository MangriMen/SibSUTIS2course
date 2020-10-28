#include <iostream>
#include <ctime>
#include <math.h>
using namespace std;

bool VR = true, HR = true;
int* A;

struct Vertex {
	int data = 0;
	int balance = 0;
	Vertex* left = nullptr;
	Vertex* right = nullptr;
};

void B2insert(Vertex*& p, int D) {
	if (p == nullptr) {
		p = new Vertex;
		p->data = D;
		p->left = nullptr;
		p->right = nullptr;
		p->balance = 0;
		VR = 1;
	} else if (p->data > D) {
		B2insert(p->left, D);
		if (VR == 1) {
			if (p->balance == 0) {
				Vertex* q = p->left;
				p->left = q->right;
				q->right = p;
				p = q;
				q->balance = 1;
				VR = 0;
				HR = 1;
			} else {
				p->balance = 0;
				VR = 1;
				HR = 0;
			}
		} else HR = 0;
	} else if (p->data < D) {
		B2insert(p->right, D);
		if (VR == 1) {
			p->balance = 1;
			HR = 1;
			VR = 0;
		} else if (HR == 1) {
			if (p->balance == 1) {
				Vertex* q = p->right;
				p->right = q->left;
				p->balance = 0;
				q->balance = 0;
				q->left = p;
				p = q;
				VR = 1;
				HR = 0;
			} else HR = 0;
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

void dispalyVertexInfo(Vertex* pointer, int traversalType) {
	cout << "\n-----------------------------------------------------\n\n";
	if (pointer == nullptr) {
		cout << "Дерево пусто.\n";
		return;
	}
	cout << "Обход " << traversalType << " типа:\n";
	traversal(pointer, traversalType);
	cout << "\n\nКоличество вершин: " << amount(pointer);
	cout << "\n\nСумма элементов: " << sum(pointer);
	cout << "\n\nВысота дерева: " << height(pointer);
	cout << "\n\nМаксимальная высота дерева: " << treeHeightSum(pointer, 1) / amount(pointer);
}



int main() {
	srand(unsigned int(time(NULL)));
	setlocale(LC_ALL, "Russian");

	int numberOfVertex = 12;
	int traversalType = 2;
	int vertexToDelete = 0;

	cout << "Введите число вершин: ";
	cin >> numberOfVertex;
	cout << "Введите тип прохода (1 - сверху вниз, 2 - справа налево, 3 - снизу вверх): ";
	cin >> traversalType;

	A = new int[numberOfVertex];
	fillRand(numberOfVertex);
	//A = new int[10]{ 21, 60, 79, 14, 7, 34, 40, 35, 29, 56 };
	//A = new int[12]{ 17, 1, 15, 10, 12, 20, 16, 13, 6, 4, 3, 25 };

	Vertex* pointer = new Vertex;
	pointer->data = A[0];
	pointer->right = nullptr;
	pointer->left = nullptr;

	cout << "\nИзначальный массив А:\n";
	for (int i = 0; i < numberOfVertex; ++i) {
		cout << A[i] << " ";
	}

	// Добавление вершин в дерево
	for (int i = 0; i < numberOfVertex; i++) {
		B2insert(pointer, A[i]);
	}

	dispalyVertexInfo(pointer, traversalType);

	// Выборочное удаление вершин
	//while (amount(pointer)) {
	//	cout << endl << endl << "Ведите высоту для удаления: ";
	//	cin >> vertexToDelete;
	//	cout << endl << endl;
	//	B2insert(pointer, vertexToDelete);
	//	dispalyVertexInfo(pointer, traversalType);
	//}

	// Для последовательного удаления всего дерева
	//for (int i = 0; i < numberOfVertex; ++i) {
	//	B2insert(pointer, A[i]);
	//	dispalyVertexInfo(pointer, traversalType);
	//}

	cout << "\n\n-----------------------------------------------------\n\n";

	delete pointer;
	delete A;
}