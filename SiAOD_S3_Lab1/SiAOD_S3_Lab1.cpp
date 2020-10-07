#include <iostream>
#include <ctime>
#include <vector>
#include <map>
#include <string>

using namespace std;

int* A;
vector<int> pos;

struct Vertex {
	int data;
	Vertex* left;
	Vertex* right;
};
																																																	void secretTraversal(Vertex* p) {
																																																		if (p != nullptr) {
																																																			secretTraversal(p->left);
																																																			cout << (char)(p->data + 191) << " ";
																																																			secretTraversal(p->right);
																																																		}
																																																	}
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

void recursiveSDP(Vertex*& p, int index) {
	if (p == nullptr)
	{
		p = new Vertex;
		p->data = index;
		p->left = nullptr;
		p->right = nullptr;
	}
	else if (index < p->data)recursiveSDP(p->left, index);
	else if (index > p->data)recursiveSDP(p->right, index);
}

void addSDP(Vertex* Root, int data) {
	Vertex** p = &Root;

	while (*p != nullptr) {
		if (data < ((*p)->data)) {
			p = &((*p)->left);
		}
		else if (data > ((*p)->data)) {
			p = &((*p)->right);
		}
		else break;
	}
	if ((*p) == nullptr) {
		*p = new Vertex;
		(*p)->data = data;
		(*p)->right = nullptr;
		(*p)->left = nullptr;
	}
}

Vertex* deleteSDP(Vertex* root, int X) {
	//Vertex** p = &root;
	//Vertex* q = nullptr, * right = nullptr, * s = nullptr;
	//while (*p != nullptr)
	//{
	//	if ((*p)->data > X)
	//	{
	//		p = &((*p)->left);
	//	}
	//	else if ((*p)->data < X)
	//	{
	//		p = &((*p)->right);
	//	}
	//	else break;
	//}
	//if (*p != nullptr)
	//{
	//	q = *p;
	//	if (q->left == nullptr)
	//	{
	//		*p = q->right;
	//	}
	//	else if (q->right == nullptr)//1
	//	{
	//		*p = q->left;
	//	}
	//	else
	//	{
	//		right = q->left;
	//		s = q;
	//		if (right->right == nullptr)//3
	//		{
	//			right->right = q->right;
	//			*p = right;
	//		}
	//		else
	//		{
	//			while (right->right != nullptr)//2
	//			{
	//				s = right;
	//				right = right->right;
	//			}
	//			s->right = right->left;
	//			right->left = q->left;
	//			right->right = q->right;
	//			*p = right;
	//		}
	//	}
	//	delete q;
	//}
	//return *p;
	Vertex* q, * p, * r;

	if (root == NULL)
		return root;

	if (X == root->data) {
		if (root->right == NULL)
			q = root->left;
		else {
			p = root->right;
			if (p->left == NULL) {
				p->left = root->left;
				q = p;
			}
			else {
				r = p->left;
				while (r->left != NULL) {
					p = r;
					r = p->left;
				}
				p->left = r->right;
				r->left = root->left;
				r->right = root->right;
				q = r;
			}
		}
		delete root;
		return q;
	}
	else if (X < root->data)
		root->left = deleteSDP(root->left, X);
	else
		root->right = deleteSDP(root->right, X);
	return root;
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
		cout << p->data<< " ";
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

void traversalGraph(Vertex* p) {
	if (p != nullptr) {
		traversalGraph(p->left);
		pos.push_back(p->data);
		traversalGraph(p->right);
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
		A[i] = rand()% 99;
	}
}

Vertex* find(Vertex* p, int X) {
	while (p != nullptr)
	{
		if (X < p->data) { p = p->left; }
		else if (X > p->data) { p = p->right; }
		else break;
	}
	return p;
}

Vertex* findRec(Vertex* p, int X) {
	if (p != nullptr) {
		if (p->data == X) { return p; };

		if (X < p->data) {
			findRec(p->left, X);
		}
		else if (X > p->data) {
			findRec(p->right, X);
		}
	}
	else return nullptr;
}

int getLevel(Vertex* p, int X) {
	int level = 1;
	while (p != nullptr)
	{
		if (X < p->data) { p = p->left; level++; }
		else if (X > p->data) { p = p->right; level++; }
		else break;
	}
	return level;
}

int main() {
	srand(unsigned int(time(NULL)));
	//system("chcp 1251");
	int numberOfVertex = 0, X = 0;
	map<int, int> graph;

	//cout << "Enter number of vertices: " << endl;
	//cin >> numberOfVertex;
	//cout << endl;
	numberOfVertex = 10;

	//A = new int[numberOfVertex];
	A = new int[10] { 44, 56, 40, 60, 23, 8, 48, 87, 66, 35 };

	//fillRand(A, numberOfVertex);

	//A[0] = 12;
	//A[1] = 6;
	//A[2] = 3;
	//A[3] = 11;
	//A[4] = 9;
	//A[5] = 14;
	//A[6] = 4;
	//A[7] = 15;
	//A[8] = 17;
	//A[9] = 29;
	//A[10] = 1;
	//A[11] = 5;
	//A = { 44, 56, 40, 60, 23, 8, 48, 87, 66, 35 };

	for (int i = 0; i < numberOfVertex; i++) {
		cout << A[i] << " ";
	}
	cout << endl;

	Vertex* root = new Vertex;
	root->data = A[0];
	root->right = nullptr;
	root->left = nullptr;

	for (int i = 1; i < numberOfVertex; i++) {
		addSDP(root, A[i]);
	}

	/*for (int i = 1; i < numberOfVertex; i++) {
		recursiveSDP(root, A[i]);
	}*/

	//Vertex* root = ISDP(0, numberOfVertex - 1);
	//
	///*cout << "Enter X to find element: ";
	//cin >> X;
	//cout << endl;
	//Vertex* found = find(root, X);*/

	cout << "Left to right: ";
	traversalLR(root);
	cout << endl << endl;

	//cout << "X: " << 98 << endl;
	//if (findRec(root, 98) != nullptr) {
	//	find(root, 98)->data = 111;
	//	cout << "Vertex found" << endl << endl;
	//}
	//else {
	//	cout << "Vertex not found" << endl << endl;
	//}

	//cout << "Left to right: ";
	//traversalLR(root);
	//cout << endl << endl;

	int height = treeHeight(root);
	
	for (int i = 0; i < numberOfVertex; i++) {
		int level = getLevel(root, A[i]);
		graph.insert({ A[i], level });
	}

	for (int j = 0; j < pos.size(); j++) {
		cout << (graph[pos[j]]) << " ";
	}

	traversalGraph(root);

	cout << endl;
	for (int i = 1; i <= height; i++) {
		cout << i << "\t";
		for (int j = 0; j < pos.size(); j++) {
			if (graph[pos[j]] == i) {
				cout << pos[j];
			}
			else {
				cout << string(j+1, ' ');
			}
		}
		cout << endl << endl;
	}
	cout << endl;

	cout << "Tree size: " << treeSize(root);
	cout << endl << endl << "Check sum: " << treeCheckSum(root);
	cout << endl << endl << "Tree height: " << treeHeight(root);
	cout << endl << endl << "Tree average height: " << averagePathLenght(root, 1) / (double)(treeSize(root));
	cout << endl << endl;

	while (1) {
		int key;
		cout << "Enter the key: ";
		cin >> key;
		if (findRec(root, key) != nullptr) {
			//system("cls");
			for (int i = 0; i < numberOfVertex; i++) {
				cout << A[i] << " ";
			}
			cout << endl;
			cout << endl;
			root = deleteSDP(root, key);
			if (root == nullptr) {
				cout << "Tree is empty" << endl;
				break;
			}
			cout << "Left to right: ";
			traversalLR(root);
			cout << endl << endl;

			graph.clear();
			for (int i = 0; i < numberOfVertex; i++) {
				int level = getLevel(root, A[i]);
				graph.insert({ A[i], level });
			}
			pos.clear();
			traversalGraph(root);

			cout << endl;
			for (int i = 1; i <= height; i++) {
				cout << i << "\t";
				for (int j = 0; j < pos.size(); j++) {
					if (graph[pos[j]] == i) {
						cout << pos[j];
					}
					else {
						cout << string(j + 1, ' ');
					}
				}
				cout << endl << endl;
			}
			cout << endl;

			cout << "Tree size: " << treeSize(root);
			cout << endl << endl << "Check sum: " << treeCheckSum(root);
			cout << endl << endl << "Tree height: " << treeHeight(root);
			cout << endl << endl << "Tree average height: " << averagePathLenght(root, 1) / (double)(treeSize(root));
			cout << endl << endl;
			cout << endl;
		}
		else {
			cout << "Vertex not found" << endl;
		}
	}

	//for (int i = 0; i < 12; i++) {
	//	cout << endl;
	//	root = deleteSDP(root, A[i]);

	//	if (root == nullptr) {
	//		cout << "Tree is empty" << endl;
	//		break;
	//	}

	//	cout << "Left to right: ";
	//	traversalLR(root);
	//	cout << endl << endl;

	//	cout << "Tree size: " << treeSize(root);
	//	cout << endl << endl << "Check sum: " << treeCheckSum(root);
	//	cout << endl << endl << "Tree height: " << treeHeight(root);
	//	cout << endl << endl << "Tree average height: " << averagePathLenght(root, 1) / (double)(treeSize(root));
	//	cout << endl << endl;
	//	cout << endl;
	//}

	delete[] A;
	delete root;
}