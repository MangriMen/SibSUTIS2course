#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <set>

using namespace std;

struct Vertex
{
	Vertex* right;
	Vertex* left;
	int data;
	int w;
};

void traversalTB(Vertex* p);
void traversalLR(Vertex* p);
void traversalBT(Vertex* p);
int treeSize(Vertex* p);
int treeCheckSum(Vertex* p);
int treeHeight(Vertex* p);
int averagePathLenght(Vertex* p, int level);
void fillZero(int* A, int size);
void fillInc(int* A, int size);
void fillDec(int* A, int size);
void fillRand(int* A, int size, int min, int max);

int* w;
int* V;
int* l;
int** Aw, ** Ap, ** Ar;
size_t n;

Vertex* root = nullptr;

void recSDP(Vertex*& p, int D, int weight)
{
	if (p == nullptr)
	{
		p = new Vertex;
		p->data = D;
		p->w = weight;
		p->left = nullptr;
		p->right = nullptr;
	}
	else if (D < p->data)recSDP(p->left, D, weight);
	else if (D > p->data)recSDP(p->right, D, weight);
}

void AwCalc(int n)
{
	Aw = new int* [n];

	for (size_t i = 0; i < n; ++i)
	{
		Aw[i] = new int[n];
	}

	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			Aw[i][j] = 0;
		}
	}

	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			if (i == j)Aw[i][j] = 0;
		}
	}

	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = i + 1; j < n; ++j)
		{
			Aw[i][j] = Aw[i][j - 1] + w[j];
		}
	}

}

void ApNArCalc(int n)
{
	Ap = new int* [n];
	Ar = new int* [n];

	for (size_t i = 0; i < n; ++i)
	{
		Ap[i] = new int[n];
		Ar[i] = new int[n];
	}

	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			Ap[i][j] = 0;
			Ar[i][j] = 0;
		}
	}

	int i = 0, j = 0;
	while (i < n - 1)
	{
		j = i + 1;
		Ap[i][j] = Aw[i][j];
		Ar[i][j] = j;
		i++;
	}

	int h = 2, m, min, x, k;
	i = 0; j = 0;
	while (h < n)
	{
		while (i < n - h)
		{
			j = i + h;
			m = Ar[i][j - 1];
			min = Ap[i][m - 1] + Ap[m][j];
			k = m + 1;
			while (k < Ar[i + 1][j])
			{
				x = Ap[i][k - 1] + Ap[k][j];
				if (x < min)
				{
					m = k;
					min = x;
				}
			}
			Ap[i][j] = min + Aw[i][j];
			Ar[i][j] = m;
			i++;
		}
		i = 0;
		h++;
	}
}

void verticesOut(int n)
{
	for (size_t i = 1; i < n; ++i)
	{
		cout << V[i] << "(" << w[i] << ")" << " ";
	}
}

void matrixOut(int** a, int n)
{
	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = 0; j < n; ++j)
		{
			cout << setw(4) << a[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void init()
{
	cout << "Enter number of elements: ";
	cin >> n;
	cout << endl;

	w = new int[++n];
	V = new int[n];
	
	fillRand(V, n, 0, 99);
	fillRand(w, n, 10, 89);

	for (size_t i = 0; i < n; ++i)
	{
		for (size_t j = i; j < n; ++j)
		{
			if (V[i] < V[j])
			{
				int tmp = V[j];
				V[j] = V[i];
				V[i] = tmp;
			}
			//if (w[i] < w[j])
			//{
			//	int tmp = w[j];
			//	w[j] = w[i];
			//	w[i] = tmp;
			//}
		}
	}
}

void createTree(int L, int R)
{
	int k;
	if (L < R)
	{
		k = Ar[L][R];
		recSDP(root, V[k], w[k]);
		createTree(L, k - 1);
		createTree(k, R);
	}
}

int searchLevel(Vertex* p, int key)
{
	int h = 1;
	while (p != nullptr)
	{
		if (key < p->data)
		{
			p = p->left;
			h++;
		}
		else if (key > p->data)
		{
			p = p->right;
			h++;
		}
		else return h;
	}
}

double treeHeight()
{
	double W = 0;
	l = new int[n];
	int h = 0;
	for (size_t i = 1; i < n; ++i)
	{
		l[i] = searchLevel(root, V[i]);
		W += w[i];
		h += w[i] * l[i];
	}
	return h / (double)W;
}

void traversalW(Vertex* p) {
	if (p != nullptr) {
		traversalW(p->left);
		cout << setw(5) << "(" << p->w << ")" << " ";
		traversalW(p->right);
	}
}

int main()
{
	srand((unsigned)time(NULL));

	init();
	cout << "Vertex array: ";
	verticesOut(n);

	AwCalc(n);
	ApNArCalc(n);

	cout << endl << endl;
	cout << "Aw:" << endl;
	matrixOut(Aw, n);
	cout << "Ap:" << endl;
	matrixOut(Ap, n);
	cout << "Ar:" << endl;
	matrixOut(Ar, n);

	createTree(0, n - 1);

	cout << endl << "Tree: ";
	cout << endl;
	traversalLR(root);
	cout << endl;
	traversalW(root);
	cout << endl << endl;

	int comp = 0;

	if (Ap[0][n - 1] / (double)Aw[0][n - 1] > treeHeight()) {
		comp = 1;
	}
	else if (Ap[0][n - 1] / (double)Aw[0][n - 1] < treeHeight()) {
		comp = -1;
	}
	else comp = 0;

	cout << "Average tree height: " << setprecision(4) << treeHeight() << endl << endl;
	cout << "Tree weight: " << Aw[0][n - 1] << endl;
	cout << "Weighted tree height: " << Ap[0][n - 1] << endl << endl;
	cout << "Average tree height (matrix): " << setprecision(4) << Ap[0][n - 1] / (double)Aw[0][n - 1] << endl;
	cout << Ap[0][n - 1] / (double)Aw[0][n - 1] << (comp == 0 ? " = " : (comp == -1 ? " < " : " > ")) << treeHeight();
	cout << endl;

	return 0;
}

void traversalTB(Vertex* p) {
	if (p != nullptr) {
		cout << (char)p->data << " ";
		traversalTB(p->left);
		traversalTB(p->right);
	}
}

void traversalLR(Vertex* p) {
	if (p != nullptr) {
		traversalLR(p->left);
		cout << setw(5) << " " << p->data << " " << " ";
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

void fillRand(int* A, int size, int min, int max) {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	set<int> temp;
	for (int i = 0; temp.size() < size; i++)
	{
		temp.insert(rand() % max + min);
	}
	vector<int> temp1(temp.size());
	copy(temp.begin(), temp.end(), temp1.begin());
	shuffle(temp1.begin(), temp1.end(), std::default_random_engine(seed));
	for (int i = 0; i < size; i++)
	{
		A[i] = temp1[i];
	}
}