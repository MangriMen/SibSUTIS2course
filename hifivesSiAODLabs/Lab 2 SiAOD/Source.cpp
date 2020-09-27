#include <iostream>
#include <ctime>
#include <algorithm>
using namespace std;

int* A;

struct Vertex {
	int data;
	Vertex* left;
	Vertex* right;
};

Vertex* newElem(int data) {
	Vertex* pointer = new Vertex;
	pointer->data = data;
	pointer->left = nullptr;
	pointer->right = nullptr;
	return pointer;
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

int main() {
	setlocale(LC_ALL, "Russian");

	Vertex* pointer;
	Vertex* root;

	root = newElem(5);
	root->right = newElem(6);
	root->left = newElem(1);

	pointer = root->left;
	pointer->right = newElem(3);

	pointer = pointer->right;
	pointer->right = newElem(4);
	pointer->left = newElem(2);

	cout << "Дерево поиска" << endl << endl << "Сверху вниз" << endl;
	traversal(root, 1);
	cout << endl << endl << "Слева направо" << endl;
	traversal(root, 2);
	cout << endl << endl << "Снизу вверх" << endl;
	traversal(root, 3);
}