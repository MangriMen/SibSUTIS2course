#include <iostream>
#include <ctime>
#include <math.h>
using namespace std;

bool VR = 1, HR = 1;
int* A;

struct Vertex {
	int data = 0;
	int balance = 0;
	Vertex* left = nullptr;
	Vertex* right = nullptr;
};

void doubleBTreeAddNode(Vertex*& pointer, int D) {
	if (pointer == nullptr) {
		pointer = new Vertex;
		pointer->data = D;
		pointer->left = nullptr;
		pointer->right = nullptr;
		pointer->balance = 0;
		VR = 1;
	} else if (pointer->data > D) {
		doubleBTreeAddNode(pointer->left, D);
		if (VR == 1) {
			if (pointer->balance == 0) {
				Vertex* tempPointer = pointer->left;
				pointer->left = tempPointer->right;
				tempPointer->right = pointer;
				tempPointer->balance = 1;
				pointer = tempPointer;
				VR = 0;
				HR = 1;
			} else {
				pointer->balance = 0;
				VR = 1;
				HR = 0;
			}
		} else HR = 0;
	} else if (pointer->data < D) {
		doubleBTreeAddNode(pointer->right, D);
		if (VR == 1) {
			pointer->balance = 1;
			VR = 0;
			HR = 1;
		} else if (HR == 1) {
			if (pointer->balance == 1) {
				Vertex* tempPointer = pointer->right;
				pointer->right = tempPointer->left;
				pointer->balance = 0;
				tempPointer->balance = 0;
				tempPointer->left = pointer;
				pointer = tempPointer;
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
		cout << "������ �����.\n";
		return;
	}
	cout << "����� " << traversalType << " ����:\n";
	traversal(pointer, traversalType);
	cout << "\n\n���������� ������: " << amount(pointer);
	cout << "\n\n����� ���������: " << sum(pointer);
	cout << "\n\n������ ������: " << height(pointer);
	cout << "\n\n������������ ������ ������: " << treeHeightSum(pointer, 1) / amount(pointer);
}



int main() {
	srand(unsigned int(time(NULL)));
	setlocale(LC_ALL, "Russian");

	int numberOfVertex = 12;
	int traversalType = 2;
	int vertexToDelete = 0;

	cout << "������� ����� ������: ";
	cin >> numberOfVertex;
	cout << "������� ��� ������� (1 - ������ ����, 2 - ������ ������, 3 - ����� �����): ";
	cin >> traversalType;

	A = new int[numberOfVertex];
	fillRand(numberOfVertex);
	//A = new int[10]{ 21, 60, 79, 14, 7, 34, 40, 35, 29, 56 };
	//A = new int[12]{ 17, 1, 15, 10, 12, 20, 16, 13, 6, 4, 3, 25 };

	Vertex* pointer = new Vertex;
	pointer->data = A[0];
	pointer->right = nullptr;
	pointer->left = nullptr;

	cout << "\n����������� ������ �:\n";
	for (int i = 0; i < numberOfVertex; ++i) {
		cout << A[i] << " ";
	}

	// ���������� ������ � ������
	for (int i = 0; i < numberOfVertex; i++) {
		doubleBTreeAddNode(pointer, A[i]);
	}

	dispalyVertexInfo(pointer, traversalType);

	// ���������� �������� ������
	//while (amount(pointer)) {
	//	cout << endl << endl << "������ ������ ��� ��������: ";
	//	cin >> vertexToDelete;
	//	cout << endl << endl;
	//	(pointer, vertexToDelete);
	//	dispalyVertexInfo(pointer, traversalType);
	//}

	// ��� ����������������� �������� ����� ������
	//for (int i = 0; i < numberOfVertex; ++i) {
	//	(pointer, A[i]);
	//	dispalyVertexInfo(pointer, traversalType);
	//}

	cout << "\n\n-----------------------------------------------------\n\n";

	delete pointer;
	delete A;
}