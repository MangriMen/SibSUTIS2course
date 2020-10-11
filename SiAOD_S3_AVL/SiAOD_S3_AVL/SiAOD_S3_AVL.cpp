#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <map>
#include <random>

using namespace std;

int* A;
vector<int> pos;
bool rost = true;

struct Vertex {
	int data = 0;
	int balance = 0;
	Vertex* left = nullptr;
	Vertex* right = nullptr;
};

void avl_ll_turn(Vertex*& root) {
	Vertex* q = root->left;
	root->balance = 0;
	q->balance = 0;
	root->left = q->right;
	q->right = root;
	root = q;
}

void avl_rr_turn(Vertex*& root) {
	Vertex* q = root->right;
	root->balance = 0;
	q->balance = 0;
	root->right = q->left;
	q->left = root;
	root = q;
}

void avl_lr_turn(Vertex*& root)
{
	Vertex* q = root->left;
	Vertex* r = q->right;

	if (r->balance < 0)
	{
		root->balance = 1;
	}
	else
	{
		root->balance = 0;
	}
	if (r->balance > 0)
	{
		q->balance = -1;
	}
	else
	{
		q->balance = 0;
	}
	r->balance = 0;
	q->right = r->left;
	root->left = r->right;
	r->left = q;
	r->right = root;
	root = r;
}

void avl_rl_turn(Vertex*& root)
{
	Vertex* q = root->right;
	Vertex* r = q->left;

	if (r->balance > 0) {
		root->balance = -1;
	}
	else {
		root->balance = 0;
	}
	if (r->balance < 0) {
		q->balance = 1;
	}
	else {
		q->balance = 0;
	}
	r->balance = 0;
	root->right = r->left;
	q->left = r->right;
	r->left = root;
	r->right = q;
	root = r;
}

void avl_add(Vertex*& root, int data) {
	if (root == nullptr) {
		root = new Vertex;
		root->data = data;
		root->left = nullptr;
		root->right = nullptr;
		root->balance = 0;
		rost = true;
	}
	else if (root->data > data) {
		avl_add(*&root->left, data);
		if (rost) {
			if (root->balance > 0) { root->balance = 0; rost = false; }
			else if (root->balance == 0) { root->balance = -1; rost = true; }
			else {
				if (root->left->balance < 0) { avl_ll_turn(root); rost = false; cout << "LL" << endl; }
				else { avl_lr_turn(root); rost = false; cout << "LR" << endl; }
			}
		}
	}
	else if (root->data < data) {
		avl_add(*&root->right, data);
		if (rost) {
			if (root->balance < 0) { root->balance = 0; rost = false; }
			else if (root->balance == 0) { root->balance = 1; rost = true; }
			else {
				if (root->right->balance > 0) { avl_rr_turn(root); rost = false; cout << "RR" << endl; }
				else { avl_rl_turn(root); rost = false; cout << "RL" << endl; }
			}
		}
	}
	else { cout << "Вершина уже есть в дереве" << endl; }
}

void traversalTB(Vertex* p) {
	if (p != nullptr) {
		cout << (char)p->data << "(" << p->balance << ")" << " ";
		traversalTB(p->left);
		traversalTB(p->right);
	}
}

void traversalLR(Vertex* p) {
	if (p != nullptr) {
		traversalLR(p->left);
		cout << p->data << "(" << p->balance << ")" << " ";
		traversalLR(p->right);
	}
}

void traversalBT(Vertex* p) {
	if (p != nullptr) {
		traversalBT(p->left);
		traversalBT(p->right);
		cout << p->data << "(" << p->balance << ")" << " ";
	}
}

void getPos(Vertex* p) {
	if (p != nullptr) {
		getPos(p->left);
		pos.push_back(p->data);
		getPos(p->right);
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
	return level + averagePathLenght(p->left, level + 1) + averagePathLenght(p->right, level + 1);;
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

void fillZero(int* A, int size) {
	for (int i = 0; i < size; i++) {
		A[i] = 0;
	}
}

void fillInc(int* A, int size) {
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
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	vector<int> temp;
	for (int i = 0; i < size; i++) {
		temp.push_back(i);
	}
	shuffle(temp.begin(), temp.end(), std::default_random_engine(seed));
	for (int i = 0; i < size; i++) {
		A[i] = temp[i];
	}
}

void traversalGraph(Vertex* root, int numberOfVertex) {
	map<int, int> graph;
	pos.clear();

	int height = treeHeight(root);

	for (int i = 0; i < numberOfVertex; i++) {
		int level = getLevel(root, A[i]);
		graph.insert({ A[i], level });
	}

	for (int j = 0; j < pos.size(); j++) {
		cout << (graph[pos[j]]) << " ";
	}

	getPos(root);

	cout << endl;
	for (int i = 1; i <= height; i++) {
		cout << i << "\t";
		for (int j = 0; j < pos.size(); j++) {
			if (graph[pos[j]] == i) {
				cout << pos[j];
			}
			else {
				cout << string(5, '-');
			}
		}
		cout << endl << endl;
	}
	cout << endl;
}

void traversalGraphInside(Vertex* root, int numberOfVertex) {
	for (int i = 0; i < numberOfVertex / 3 - 1; i++) cout << " ";
	cout << (char)root->left->data;
	for (int i = 0; i < numberOfVertex / 3 + 1; i++) cout << " ";
	cout << (char)root->right->data;
	for (int i = 0; i < numberOfVertex / 3 - 1; i++) cout << " ";
	cout << endl;
}

void traversalGraph2(Vertex* root, int numberOfVertex) {
	if (root != nullptr) {
		for (int i = 0; i < numberOfVertex /2; i++) cout << " ";
		cout << (char)root->data;
		for (int i = 0; i < numberOfVertex /2; i++) cout << " ";
		cout << endl;
		for (int i = 0; i < numberOfVertex / 3 - 1; i++) cout << " ";
		cout << (char)root->left->data;
		for (int i = 0; i < numberOfVertex / 3 + 1; i++) cout << " ";
		cout << (char)root->right->data;
		for (int i = 0; i < numberOfVertex / 3 - 1; i++) cout << " ";
		cout << endl;
		traversalGraphInside(root->left, numberOfVertex);
		traversalGraphInside(root->right, numberOfVertex);
	}
}


int main()
{
	srand(unsigned int(time(NULL)));
	setlocale(LC_ALL, "RUSSIAN");
	int numberOfVertex = 0;

	cout << "Enter number of vertex: ";
	//cin >> numberOfVertex;
	cout << endl;

	A = new int[numberOfVertex];
	fillInc(A, numberOfVertex);

	A = new int[12]{ (int)'Л', (int)'Е', (int)'В', (int)'К', (int)'И', (int)'Н', (int)'Г', (int)'О', (int)'Р', (int)'Ь', (int)'А', (int)'Д' };
	numberOfVertex = 12;
	//A = new int[10]{ 44, 56, 40, 60, 23, 8, 48, 87, 66, 35 };
	//numberOfVertex = 10;

	//for (int i = 0; i < numberOfVertex; i++) {
	//	cout << A[i] << " ";
	//}
	//cout << endl;

	Vertex* root = nullptr;
	
	for (int i = 0; i < numberOfVertex; i++) {
		avl_add(root, A[i]);
	}

	traversalGraph2(root, numberOfVertex*4);
	cout << endl;

	cout << "Left to right: ";
	traversalLR(root);
	cout << endl << endl;

	cout << "Top to bottom: ";
	traversalTB(root);
	cout << endl << endl;

	cout << "Tree size: " << treeSize(root);
	cout << endl << endl << "Check sum: " << treeCheckSum(root);
	cout << endl << endl << "Tree height: " << treeHeight(root);
	cout << endl << endl << "Tree average height: " << averagePathLenght(root, 1) / (double)(treeSize(root));
	cout << endl << endl;

	delete root;
}
