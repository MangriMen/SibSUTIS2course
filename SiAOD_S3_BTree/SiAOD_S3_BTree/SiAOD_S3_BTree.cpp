#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <map>
#include <random>
#include <algorithm>

using namespace std;

int* A;
bool VR = true, HR = true;

struct Vertex
{
	Vertex* right;
	Vertex* left;
	int balance;
	int data;
};

void BTree_add(Vertex*& p, int D)
{
	if (p == nullptr)
	{
		p = new Vertex;
		p->data = D;
		p->left = nullptr;
		p->right = nullptr;
		p->balance = 0;
		VR = 1;
	}
	else if (p->data > D)
	{
		BTree_add(p->left, D);
		if (VR == 1)
		{
			if (p->balance == 0)
			{
				Vertex* q = p->left;
				p->left = q->right;
				q->right = p;
				p = q;
				q->balance = 1;
				VR = 0;
				HR = 1;
			}
			else
			{
				p->balance = 0;
				VR = 1;
				HR = 0;
			}
		}
		else HR = 0;
	}
	else if (p->data < D)
	{
		BTree_add(p->right, D);
		if (VR == 1)
		{
			p->balance = 1;
			HR = 1;
			VR = 0;
		}
		else if (HR == 1)
		{
			if (p->balance == 1)
			{
				Vertex* q = p->right;
				p->right = q->left;
				p->balance = 0;
				q->balance = 0;
				q->left = p;
				p = q;
				VR = 1;
				HR = 0;
			}
			else HR = 0;
		}
	}
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

int main()
{
	srand(unsigned int(time(NULL)));
	system("color F0");
	setlocale(LC_ALL, "RUSSIAN");
	int numberOfVertex = 0;
	int isChar = false;

	cout << "Enter number of vertex: ";
	//cin >> numberOfVertex;
	cout << endl;

	numberOfVertex = 3;

	A = new int[numberOfVertex] { 1, 2, 3 };
	//fillRand(A, numberOfVertex);

	//A = new int[12]{ (int)'Ë', (int)'Å', (int)'Â', (int)'Ê', (int)'È', (int)'Í', (int)'Ã', (int)'Î', (int)'Ð', (int)'Ü', (int)'À', (int)'Ä' };
	//numberOfVertex = 12;
	//isChar = true;
	//A = new int[10]{ 44, 56, 40, 60, 23, 8, 48, 87, 66, 35 };
	//numberOfVertex = 10;

	Vertex* root = nullptr;

	for (size_t i = 0; i < numberOfVertex; i++)
	{
		BTree_add(root, A[i]);
		cout << "Left to right: ";
		traversalLR(root);
		traversalGraph(root, numberOfVertex, isChar);
		cout << endl;
		cout << endl << endl;
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

	delete root;
	return 0;
}