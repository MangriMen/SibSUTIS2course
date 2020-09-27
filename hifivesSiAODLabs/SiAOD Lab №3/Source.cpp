#include <iostream>
#include <ctime>
#include <math.h>
using namespace std;

int* A;

struct Vertex {
	int data;
	Vertex* left;
	Vertex* right;
};

Vertex* ideallyBalancedSearchTree(int left, int right) {
	if (left > right) { return nullptr; }

	int treeMiddleElement = int(ceil((left + right) / double(2)));
	Vertex* pointer = new Vertex;

	pointer->data = A[treeMiddleElement];
	pointer->left = ideallyBalancedSearchTree(left, treeMiddleElement - 1);
	pointer->right = ideallyBalancedSearchTree(treeMiddleElement + 1, right);

	return pointer;
}

void balancedSearchTreeRecursive(Vertex*& pointer, int index) {
	if (pointer == nullptr) {
		pointer = new Vertex;
		pointer->data = index;
		pointer->left = nullptr;
		pointer->right = nullptr;
	}
	else if (index < pointer->data) balancedSearchTreeRecursive(pointer->left, index);
	else if (index > pointer->data) balancedSearchTreeRecursive(pointer->right, index);
}

void balancedSearchTreeDoublePointer(Vertex* root, int size) {
	for (int i = 1; i < size; i++) {
		Vertex** pointer = &root;

		while (*pointer != nullptr) {
			if (A[i] < ((*pointer)->data)) {
				pointer = &((*pointer)->left);
			}
			else if (A[i] > ((*pointer)->data)) {
				pointer = &((*pointer)->right);
			}
			else break;
		}
		if ((*pointer) == nullptr) {
			*pointer = new Vertex;
			(*pointer)->data = A[i];
			(*pointer)->right = nullptr;
			(*pointer)->left = nullptr;
		}
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

Vertex* search(Vertex* pointer, int index) {
	if (pointer == nullptr) { return pointer; }

	if (pointer->data == index) { return pointer; }

	//pointer->data > index ? search(pointer->left, index) : (pointer->data < index ? search(pointer->right, index) : pointer);
	if (pointer->data > index) {
		search(pointer->left, index);
		pointer->data = 0;
	} else if (pointer->data < index) {
		search(pointer->right, index);
		pointer->data = 0;
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

int main() {
	srand(unsigned int(time(NULL)));
	setlocale(LC_ALL, "Russian");

	int numberOfVertex;
	int traversalType;
	int elementToFind;

	cout << "������� ����� ������: ";
	cin >> numberOfVertex;
	cout << "������� ��� ������� (1 - ������ ����, 2 - ������ ������, 3 - ����� �����): ";
	cin >> traversalType;

	A = new int[numberOfVertex];
	fillRand(numberOfVertex);
	Vertex* root = new Vertex;
	root->data = A[0];
	root->right = nullptr;
	root->left = nullptr;

	// ����
	//root = ideallyBalancedSearchTree(0, numberOfVertex - 1);
	
	// ��� � ������� ������������
	balancedSearchTreeDoublePointer(root, numberOfVertex);
	
	// ��� � ����������� ��������
	//for (int i = 0; i < numberOfVertex; ++i) { balancedSearchTreeRecursive(root, A[i]); }
	
	cout << "\n����������� ������ �:\n";
	for (int i = 0; i < numberOfVertex; ++i) {
		cout << A[i] << " ";
	}
	cout << "\n\n����� " << traversalType <<" ����:\n";
	traversal(root, traversalType);
	cout << "\n\n���������� ������:\n" << amount(root);
	cout << "\n\n����� ���������:\n" << sum(root);
	cout << "\n\n������ ������:\n" << height(root);
	cout << "\n\n������������ ������ ������:\n" << treeHeightSum(root, 1) / amount(root);
	cout << "\n\n������� ����� ��� ������: ";
	cin >> elementToFind;
	Vertex* foundElement = search(root, elementToFind);
	if (foundElement != nullptr) {
		cout << "\n������� " << foundElement->data << " ��� ������ �� ������: " << foundElement << "\n";
	}
	else {
		cout << "\n������� " << elementToFind << " �� ������\n";
	}
	traversal(root, traversalType);


	delete root;
	delete A;
}