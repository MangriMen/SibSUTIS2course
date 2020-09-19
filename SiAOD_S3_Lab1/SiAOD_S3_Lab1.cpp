#include <iostream>
#include <ctime>

using namespace std;

int* A;

struct Vertex {
	int data;
	Vertex* left;
	Vertex* right;
};

Vertex* newEl(int data) {
	Vertex* p = new Vertex;
	p->data = data;
	p->left = nullptr;
	p->right = nullptr;
	return p;
}

Vertex* ISDP(int left, int right) {
	if (left > right) {
		return nullptr;
	}

	int treeMiddleElement = int(ceil((left + right) / double(2)));
	Vertex* pointer = new Vertex;

	pointer->data = A[treeMiddleElement];
	pointer->left = ISDP(left, treeMiddleElement - 1);
	pointer->right = ISDP(treeMiddleElement + 1, right);

	return pointer;
}

void traversalTB(Vertex* p) {
	if (p != nullptr) {
		cout << p->data << " ";
		traversalTB(p->left);
		traversalTB(p->right);
	}
}
void traversalLR(Vertex* p) {
	if (p != nullptr) {
		traversalLR(p->left);
		cout << p->data << " ";
		traversalLR(p->right);
	}
}
void traversalBT(Vertex* p) {
	if (p != nullptr) {
		traversalBT(p->left);
		traversalBT(p->right);
		cout << p->data << " ";
	}
}

int treeSize(Vertex* p) {
	if (p == nullptr) {
		return 0;
	}
	return 1 + treeSize(p->left) + treeSize(p->right);
}

int treeCheckSum(Vertex* p) {
	if (p == nullptr) {
		return 0;
	}
	return p->data + treeCheckSum(p->left) + treeCheckSum(p->right);
}

int treeHeight(Vertex* p) {
	if (p == nullptr) {
		return 0;
	}
	return 1 + max(treeHeight(p->left), treeHeight(p->right));
}

int averagePathLenght(Vertex* p, int level) {
	if (p == nullptr) {
		return 0;
	}
	return level + averagePathLenght(p->left, level + 1) + averagePathLenght(p->right, level + 1);
}

void fillInc(int *A, int size) {
	for (int i = 0; i < size; i++) {
		A[i] = i + 1;
	}
}

void fillDec(int* A, int size) {
	int j = size;
	for (int i = 0; i < size; i++) {
		A[i] = j;
		j--;
	}
}

void fillRand(int* A, int size) {
	for (int i = 0; i < size; i++) {
		A[i] = rand()% 9;
	}
}

Vertex* find(Vertex* p, int key) {
	while (p != nullptr)
	{
		if (key < p->data) { p = p->left; }
		else if (key > p->data) { p = p->right; }
		else break;
	}
	return p;
}

Vertex* findRec(Vertex* p, int key) {
	if (p != nullptr) {
		if (p->data == key) { return p; };

		if (key < p->data) {
			findRec(p->left, key);
		}
		else if (key > p->data) {
			findRec(p->right, key);
		}
	}
	else return nullptr;
}

int main() {
	srand(unsigned int(time(NULL)));
	int numberOfVertex, key;

	cout << "Enter number of vertices: " << endl;
	cin >> numberOfVertex;
	cout << endl;

	A = new int[numberOfVertex];

	fillInc(A, numberOfVertex);
	Vertex* root = ISDP(0, numberOfVertex - 1);
	
	/*cout << "Enter key to find element: ";
	cin >> key;
	cout << endl;
	Vertex* found = find(root, key);*/

	cout << "Top to bottom: ";
	traversalTB(root);
	cout << endl << endl;
	cout << "Left to right: ";
	traversalLR(root);
	cout << endl << endl;
	cout << "Bottom to top: ";
	traversalBT(root);
	cout << endl << endl;

	cout << "Key: " << 98 << endl;
	if (findRec(root, 98) != nullptr) {
		//find(root, 98)->data = 111;
		cout << "Vertex found" << endl << endl;
	}
	else {
		cout << "Vertex not found" << endl << endl;
	}

	cout << "Key: " << 99 << endl;
	if (find(root, 99) != nullptr) {
		cout << "Vertex found" << endl << endl;
	}
	else {
		cout << "Vertex not found" << endl << endl;
	}

	cout << "Key: " << 110 << endl;
	if (find(root, 110) != nullptr) {
		cout << "Vertex found" << endl << endl;
	}
	else {
		cout << "Vertex not found" << endl << endl;
	}

	cout << "Key: " << 98 << endl;
	if (findRec(root, 98) != nullptr) {
		find(root, 98)->data = 111;
		cout << "Vertex found" << endl << endl;
	}
	else {
		cout << "Vertex not found" << endl << endl;
	}

	cout << "Left to right: ";
	traversalLR(root);
	cout << endl << endl;

	cout << "Tree size: " << treeSize(root);
	cout << endl << endl << "Check sum: " << treeCheckSum(root);
	cout << endl << endl << "Tree height: " << treeHeight(root);
	cout << endl << endl << "Tree average height: " << averagePathLenght(root, 1) / (double)(treeSize(root));
	cout << endl << endl;
}