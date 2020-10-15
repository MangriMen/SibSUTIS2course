#include <iostream>
#include <ctime>
#include <math.h>
using namespace std;

bool isIncreased = true;
bool decreased = true;
int* A;

struct Vertex {
	int data = 0;
	int balance = 0;
	Vertex* left = nullptr;
	Vertex* right = nullptr;
};

void leftLeftTurn(Vertex*& pointer) {
	Vertex* tempPointer = pointer->left;
	pointer->balance = 0;
	tempPointer->balance = 0;
	pointer->left = tempPointer->right;
	tempPointer->right = pointer;
	pointer = tempPointer;
}

void rightRightTurn(Vertex*& pointer) {
	Vertex* tempPointer = pointer->right;
	pointer->balance = 0;
	tempPointer->balance = 0;
	pointer->right = tempPointer->left;
	tempPointer->left = pointer;
	pointer = tempPointer;
}

void leftRightTurn(Vertex*& pointer) {
	Vertex* tempPointer = pointer->left;
	Vertex* r = tempPointer->right;

	pointer->balance = (r->balance < 0 ? 1 : 0);
	tempPointer->balance = (r->balance > 0 ? -1 : 0);

	r->balance = 0;
	tempPointer->right = r->left;
	pointer->left = r->right;
	r->left = tempPointer;
	r->right = pointer;
	pointer = r;
}

void rightLeftTurn(Vertex*& pointer) {
	Vertex* tempPointer = pointer->right;
	Vertex* r = tempPointer->left;

	pointer->balance = (r->balance > 0 ? -1 : 0);
	tempPointer->balance = (r->balance < 0 ? 1 : 0);

	r->balance = 0;
	pointer->right = r->left;
	tempPointer->left = r->right;
	r->left = pointer;
	r->right = tempPointer;
	pointer = r;
}

void AVLTreeAddNode(Vertex*& pointer, int data) {
	if (pointer == nullptr) {
		pointer = new Vertex;
		pointer->data = data;
		pointer->balance = 0;
		pointer->left = nullptr;
		pointer->right = nullptr;
		isIncreased = true;
	}
	else if (pointer->data > data) {
		AVLTreeAddNode(*&pointer->left, data);
		if (isIncreased) {
			if (pointer->balance > 0) {
				pointer->balance = 0;
				isIncreased = false;
			}
			else if (pointer->balance == 0) {
				pointer->balance = -1;
				isIncreased = true;
			}
			else {
				if (pointer->left->balance < 0) {
					cout << endl;
					leftLeftTurn(pointer);
					cout << "LL ";
					isIncreased = false;
				}
				else {
					cout << endl;
					leftRightTurn(pointer);
					cout << "LR ";
					isIncreased = false;
				}
			}
		}
	}
	else if (pointer->data < data) {
		AVLTreeAddNode(*&pointer->right, data);
		if (isIncreased) {
			if (pointer->balance < 0) {
				pointer->balance = 0;
				isIncreased = false;
			}
			else if (pointer->balance == 0) {
				pointer->balance = 1;
				isIncreased = true;
			}
			else {
				if (pointer->right->balance > 0) {
					cout << endl;
					rightRightTurn(pointer);
					cout << "RR ";
					isIncreased = false;
				}
				else {
					cout << endl;
					rightLeftTurn(pointer);
					cout << "RL ";
					isIncreased = false;
				}
			}
		}
	}
}

void oneLeftLeftTurn(Vertex*& pointer) {
	Vertex* tempPointer = pointer->left;
	if (tempPointer->balance == 0) {
		pointer->balance = -1;
		tempPointer->balance = 1;
		decreased = false;
	} else {
		pointer->balance = 0;
		tempPointer->balance = 0;
	}
	pointer->left = tempPointer->right;
	tempPointer->right = pointer;
	pointer = tempPointer;
}

void oneRightRightTurn(Vertex*& pointer) {
	Vertex* tempPointer = pointer->right;
	if (tempPointer->balance == 0) {
		pointer->balance = 1;
		tempPointer->balance = -1;
		decreased = false;
	} else {
		pointer->balance = 0;
		tempPointer->balance = 0;
	}
	pointer->right = tempPointer->left;
	tempPointer->left = pointer;
	pointer = tempPointer;
}

void avl_bl_turn(Vertex*& pointer) {
	if (pointer->balance == -1) {
		pointer->balance = 0;
	}
	else if (pointer->balance == 0) {
		pointer->balance = 1;
		decreased = false;
	}
	else if (pointer->balance == 1) {
		if (pointer->right->balance >= 0) {
			oneRightRightTurn(pointer);
			cout << endl << "rr1" << endl;
		}
		else {
			rightLeftTurn(pointer);
		}
	}
}

void avl_br_turn(Vertex*& pointer) {
	if (pointer->balance == 1) {
		pointer->balance = 0;
	}
	else if (pointer->balance == 0) {
		pointer->balance = -1;
		decreased = false;
	}
	else if (pointer->balance == -1) {
		if (pointer->left->balance <= 0) {
			oneLeftLeftTurn(pointer);
			cout << endl << "ll1" << endl;
		}
		else {
			leftRightTurn(pointer);
		}
	}
}

void Del(Vertex*& r, Vertex*& tempPointer) {
	if (r->right != nullptr) {
		Del(r->right, tempPointer);
		if (decreased) {
			avl_br_turn(r);
		}
	}
	else {
		tempPointer->data = r->data;
		tempPointer = r;
		r = r->left;
		decreased = true;
	}
}

void AVLTreeDeleteNode(Vertex*& pointer, int data) {
	if (pointer == nullptr) { return; }

	if (data < pointer->data) {
		AVLTreeDeleteNode(pointer->left, data);
		if (decreased) { avl_bl_turn(pointer); }
	} else if (data > pointer->data) {
		AVLTreeDeleteNode(pointer->right, data);
		if (decreased) { avl_br_turn(pointer); }
	} else {
		Vertex* tempPointer = pointer;
		if (tempPointer->left == nullptr) {
			pointer = tempPointer->right;
			decreased = true;
		}
		else if (tempPointer->right == nullptr) {
			pointer = tempPointer->left;
			decreased = true;
		}
		else {
			Del(tempPointer->left, tempPointer);
			if (decreased) { avl_bl_turn(pointer); }
		}
		delete tempPointer;
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

	/*A = new int[numberOfVertex];
	fillRand(numberOfVertex);*/
	//A = new int[10]{ 21, 60, 79, 14, 7, 34, 40, 35, 29, 56 };
	A = new int[12]{ 17, 1, 15, 10, 12, 20, 16, 13, 6, 4, 3, 25 };

	Vertex* pointer = new Vertex;
	pointer->data = A[0];
	pointer->right = nullptr;
	pointer->left = nullptr;

	cout << "\nИзначальный массив А:\n";
	for (int i = 0; i < numberOfVertex; ++i) {
		cout << A[i] << " ";
	}

	for (int i = 0; i < numberOfVertex; ++i) {
		AVLTreeAddNode(pointer, A[i]);
	}

	dispalyVertexInfo(pointer, traversalType);

	// Выборочное удаление вершин
	/*while (amount(pointer)) {
		cout << endl << endl << "Ведите высоту для удаления: ";
		cin >> vertexToDelete;
		cout << endl << endl;
		AVLTreeDeleteNode(pointer, vertexToDelete);
		dispalyVertexInfo(pointer, traversalType);
	}*/
	
	// Для последовательного удаления всего дерева
	for (int i = 0; i < numberOfVertex; ++i) {
		AVLTreeDeleteNode(pointer, A[i]);
		dispalyVertexInfo(pointer, traversalType);
	}

	cout << "\n\n-----------------------------------------------------\n\n";

	delete pointer;
	delete A;
}