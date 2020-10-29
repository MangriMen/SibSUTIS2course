#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <map>
#include <random>

using namespace std;

int* A;

bool rost = true;
bool umen = false;

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

void avl_ll1_turn(Vertex*& root) {
	Vertex* q = root->left;
	if (q->balance == 0) {
		root->balance = -1;
		q->balance = 1;
		umen = false;
	}
	else {
		root->balance = 0;
		q->balance = 0;
	}
	root->left = q->right;
	q->right = root;
	root = q;
}

void avl_rr1_turn(Vertex*& root) {
	Vertex* q = root->right;
	if (q->balance == 0) {
		root->balance = 1;
		q->balance = -1;
		umen = false;
	}
	else {
		root->balance = 0;
		q->balance = 0;
	}
	root->right = q->left;
	q->left = root;
	root = q;
}

void avl_bl_turn(Vertex*& root) {
	if (root->balance == -1) {
		root->balance = 0;
	}
	else if (root->balance == 0) {
		root->balance = 1;
		umen = false;
	}
	else if (root->balance == 1) {
		if (root->right->balance >= 0) {
			cout << endl << "rr1" << endl;
			avl_rr1_turn(root);
		}
		else {
			cout << endl << "rl" << endl;
			avl_rl_turn(root);
		}
	}
}

void avl_br_turn(Vertex*& root) {
	if (root->balance == 1) {
		root->balance = 0;
	}
	else if (root->balance == 0) {
		root->balance = -1;
		umen = false;
	}
	else if (root->balance == -1) {
		if (root->left->balance <= 0) {
			cout << endl << "ll1" << endl;
			avl_ll1_turn(root);
		}
		else {
			cout << endl << "lr" << endl;
			avl_lr_turn(root);
		}
	}
}

void Del(Vertex*& r, Vertex*& q) {
	if (r->right != nullptr) {
		Del(r->right, q);
		if (umen) {
			avl_br_turn(r);
		}
	}
	else {
		q->data = r->data;
		q = r;
		r = r->left;
		umen = true;
	}
}

void avl_delete(Vertex*& root, int data) {
	if (root == nullptr) {
		return;
	}
	else if (data < root->data) {
		avl_delete(root->left, data);
		if (umen) {
			cout << endl << "bl" << endl;
			avl_bl_turn(root);
		}
	}
	else if (data > root->data) {
		avl_delete(root->right, data);
		if (umen) {
			cout << endl << "br" << endl;
			avl_br_turn(root);
		}
	}
	else {
		Vertex* q = root;
		if (q->left == nullptr) {
			root = q->right;
			umen = true;
		}
		else if (q->right == nullptr) {
			root = q->left;
			umen = true;
		}
		else {
			Del(q->left, q);
			if (umen) {
				cout << endl << "bl" << endl;
				avl_bl_turn(root);
			}
		}
		delete q;
	}
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
				if (root->left->balance < 0) { avl_ll_turn(root); rost = false; }
				else { avl_lr_turn(root); rost = false; }
			}
		}
	}
	else if (root->data < data) {
		avl_add(*&root->right, data);
		if (rost) {
			if (root->balance < 0) { root->balance = 0; rost = false; }
			else if (root->balance == 0) { root->balance = 1; rost = true; }
			else {
				if (root->right->balance > 0) { avl_rr_turn(root); rost = false; }
				else { avl_rl_turn(root); rost = false; }
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

void getElements(Vertex* p, int level, int height, vector<pair<int, int>>& graph) {
	if (p == nullptr) {
		if (level <= height) {
			graph.push_back(make_pair(INT_MAX, level));
		}
		return;
	}
	graph.push_back(make_pair(p->data, level));
	getElements(p->left, level + 1, height, graph);
	getElements(p->right, level + 1, height, graph);
}

void traversalGraph(Vertex* root, int numberOfVertex, bool isChar) {
	int height = treeHeight(root);

	const int q = 2;
	int an = pow(q, height - 1);
	int sum = ((an * q) - 1) / (q - 1);

	vector<pair<int, int>> graph;
	vector<vector<int>> matrix(height + 2, vector<int>(sum + 2, INT_MAX));

	getElements(root, 1, height, graph);

	for (int i = 1; i <= height; i++) {
		int el = ceil(sum / pow(2, i));
		int offset = floor(sum / pow(2, (i - 1 < 0 ? 0 : i - 1)));
		int geom = pow(2, (i - 1 < 0 ? 0 : i - 1));

		for (int x = 0, of = 0; x < graph.size(); x++) {
			if (graph[x].second == i) { matrix[i][el + ((offset + 1) * of)] = graph[x].first; of++; }
		}
	}

	for (int i = 0; i <= height + 1; i++) {
		for (int j = 0; j <= sum + 1; j++) {
			matrix[i][j] != INT_MAX
				? isChar ? (cout << (char)matrix[i][j] << "  ") : (cout << matrix[i][j] << "  ")
				: (cout << "   ");
		}
		cout << endl << endl;
	}
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

int main()
{
	srand(unsigned int(time(NULL)));
	system("color F0");
	setlocale(LC_ALL, "RUSSIAN");
	int numberOfVertex = 0;
	int isChar = false;

	cout << "Enter number of vertex: ";
	cin >> numberOfVertex;
	cout << endl;

	A = new int[numberOfVertex];
	fillRand(A, numberOfVertex);

	//A = new int[12]{ (int)'Л', (int)'Е', (int)'В', (int)'К', (int)'И', (int)'Н', (int)'Г', (int)'О', (int)'Р', (int)'Ь', (int)'А', (int)'Д' };
	//A = new int[12]{ (int)'М',(int)'Е',(int)'Н',(int)'Ь',(int)'Щ',(int)'И',(int)'К',(int)'О',(int)'В',(int)'Д',(int)'А',(int)'Л' };
	//numberOfVertex = 12;
	//isChar = true;
	//A = new int[10]{ 44, 56, 40, 60, 23, 8, 48, 87, 66, 35 };
	//numberOfVertex = 10;

	Vertex* root = nullptr;
	
	for (int i = 0; i < numberOfVertex; i++) {
		avl_add(root, A[i]);
	}

	cout << "Original array:\t";
	for (int i = 0; i < numberOfVertex; i++) {
		if (isChar) {
			cout << char(A[i]) << "(" << A[i] << ")" << " ";
		}
		else {
			cout << A[i] << " ";
		}
	}
	cout << endl << endl;

	cout << "Left to right: ";
	traversalLR(root);
	cout << endl << endl;

	traversalGraph(root, numberOfVertex, isChar);
	cout << endl;

	cout << "Tree size: " << treeSize(root);
	cout << endl << endl << "Check sum: " << treeCheckSum(root);
	cout << endl << endl << "Tree height: " << treeHeight(root);
	cout << endl << endl << "Tree average height: " << averagePathLenght(root, 1) / (double)(treeSize(root));
	cout << endl << endl;

	for (int i = 0; i < numberOfVertex; i++) {
		cout << "Left to right: ";
		traversalLR(root);
		cout << endl << endl;
		avl_delete(root, A[i]);
		traversalGraph(root, 0, isChar);
		cout << endl;
	}

	while (false) {
		int key;
		cout << "Enter the key: ";
		cin >> key;
		cout << endl;
		if (findRec(root, key) != nullptr) {
			//system("cls");
			cout << "Original array:\t";
			for (int i = 0; i < numberOfVertex; i++) {
				if (isChar) {
					cout << char(A[i]) << "(" << A[i] << ")" << " ";
				}
				else {
					cout << A[i] << " ";
				}
			}
			cout << endl << endl;
			avl_delete(root, key);
			if (root == nullptr) {
				cout << "Tree is empty" << endl;
				break;
			}
			cout << "Left to right: ";
			traversalLR(root);
			cout << endl << endl;

			traversalGraph(root, 0, isChar);

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

	delete root;
}
