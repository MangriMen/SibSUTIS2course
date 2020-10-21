#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <map>
#include <random>

using namespace std;

const int m = 4;

int* A;

int L, R;

struct Item {
	int Data;
	Page* Page;
};

struct Page {
	int k = 2 * m;
	Page* Pz;
	Item Elems[2 * m];
};

int BTree_Find(Page*& page, int data) {
	if (page == nullptr) return 0;
	else {
		L = 1, R = page->k + 1;
		while (L < R) {
			int i = (L + R) / 2;
			if (page->Elems[i].Data < data) L = i + 1;
			else R = i;
		}
		R = R - 1;
		if (R > 0 && page->Elems[R].Data == data) {
			return page->Elems[R].Data;
		}
		else {
			if (R == 0) BTree_Find(page->Pz, data);
			else BTree_Find(page->Elems[R].Page, data);
		}
	}
}

void BTree_Add(Page*& page, Item*& V, int data, bool isIncrease) {
	if (page == nullptr) {
		V->Data = data;
		V->Page = nullptr;
		isIncrease = true;
	}
	else {
		BTree_Find(page, data);
		if (R > 0 && page->Elems[R].Data == data);
		else {
			BTree_Add(page->Pz, u, data, isIncrease);
		}
	}
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

//void getElements(Vertex* p, int level, int height, vector<pair<int, int>>& graph) {
//	if (p == nullptr) {
//		if (level <= height) {
//			graph.push_back(make_pair(INT_MAX, level));
//		}
//		return;
//	}
//	graph.push_back(make_pair(p->data, level));
//	getElements(p->left, level + 1, height, graph);
//	getElements(p->right, level + 1, height, graph);
//}

//void traversalGraph(Vertex* root, int numberOfVertex, bool isChar) {
//	int height = treeHeight(root);
//
//	const int q = 2;
//	int an = pow(q, height - 1);
//	int sum = ((an * q) - 1) / (q - 1);
//
//	vector<pair<int, int>> graph;
//	vector<vector<int>> matrix(height + 2, vector<int>(sum + 2, INT_MAX));
//
//	getElements(root, 1, height, graph);
//
//	for (int i = 1; i <= height; i++) {
//		int el = ceil(sum / pow(2, i));
//		int offset = floor(sum / pow(2, (i - 1 < 0 ? 0 : i - 1)));
//		int geom = pow(2, (i - 1 < 0 ? 0 : i - 1));
//
//		for (int x = 0, of = 0; x < graph.size(); x++) {
//			if (graph[x].second == i) { matrix[i][el + ((offset + 1) * of)] = graph[x].first; of++; }
//		}
//	}
//
//	for (int i = 0; i <= height + 1; i++) {
//		for (int j = 0; j <= sum + 1; j++) {
//			matrix[i][j] != INT_MAX
//				? isChar ? (cout << (char)matrix[i][j] << "  ") : (cout << matrix[i][j] << "  ")
//				: (cout << "   ");
//		}
//		cout << endl << endl;
//	}
//}

int main()
{
    
}