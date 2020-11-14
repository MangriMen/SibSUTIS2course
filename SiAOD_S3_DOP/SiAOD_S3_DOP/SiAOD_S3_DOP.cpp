#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <set>
#include <SFML/Graphics.hpp>
#include "TreeColor.h"
#include "InfoCard.h"
#include "Form.h"

using namespace std;
using namespace sf;

struct TreeVertex
{
	TreeVertex* right;
	TreeVertex* left;
	int data;
	int w;
	sf::CircleShape circle;
	sf::VertexArray Left;
	sf::VertexArray Right;
};

void traversalTB(TreeVertex* p);
void traversalLR(TreeVertex* p);
void traversalBT(TreeVertex* p);
int treeSize(TreeVertex* p);
int treeCheckSum(TreeVertex* p);
int treeHeight(TreeVertex* p);
int averagePathLenght(TreeVertex* p, int level);
void fillZero(int* A, int size);
void fillInc(int* A, int size);
void fillDec(int* A, int size);
void fillRand(int* A, int size, int min, int max);

int* w;
int* V;
int* l;
int** Aw, ** Ap, ** Ar;
size_t n;

constexpr auto OFFSET_X = 100;
constexpr auto OFFSET_Y = 80;

TreeVertex* root = nullptr;

void recSDP(TreeVertex*& p, int D, int weight, string sibling, sf::Vector2f parent)
{
	if (p == nullptr)
	{
		p = new TreeVertex;
		p->data = D;
		p->w = weight;
		p->left = nullptr;
		p->right = nullptr;

		p->Left.setPrimitiveType(sf::PrimitiveType::Lines);
		p->Left.resize(2);

		p->Right.setPrimitiveType(sf::Lines);
		p->Right.resize(2);

		p->circle.setRadius(20);
		p->circle.setOutlineColor(sf::Color::Black);
		p->circle.setOutlineThickness(-4);
		p->circle.setOrigin(p->circle.getLocalBounds().width / 2, p->circle.getLocalBounds().height / 2);

		if (sibling == "R") {
			p->circle.setPosition(parent + sf::Vector2f(OFFSET_X, OFFSET_Y));
		}
		else if (sibling == "L") {
			p->circle.setPosition(parent + sf::Vector2f(-OFFSET_X, OFFSET_Y));
		}
	}
	else if (D < p->data)recSDP(p->left, D, weight, "L", p->circle.getPosition());
	else if (D > p->data)recSDP(p->right, D, weight, "R", p->circle.getPosition());
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
			while (k <= Ar[i + 1][j])
			{
				x = Ap[i][k - 1] + Ap[k][j];
				if (x < min)
				{
					m = k;
					min = x;
				}
				k++;
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
		for (size_t j = i; j < n; ++j)
			if (V[i] < V[j])
				swap(V[i], V[j]);
}

vector<TreeVertex*> vertices;

void rebuild(TreeVertex* p, sf::Vector2f pos, int level) {
	if (p != nullptr) {
		if (pos == sf::Vector2f(0, 0)) {
			p->circle.setFillColor(TreeColor::Root);
		}
		level--;
		p->circle.setPosition(pos);
		p->Left[0].position = pos;
		p->Right[0].position = pos;

		if (p->left != nullptr) {
			p->Left[1].position = pos + sf::Vector2f(-OFFSET_X * level, OFFSET_Y);
		}
		else {
			p->Left[1].position = p->Left[0].position;
		}

		if (p->right != nullptr) {
			p->Right[1].position = pos + sf::Vector2f(OFFSET_X * level, OFFSET_Y);
		}
		else {
			p->Right[1].position = p->Right[0].position;
		}

		p->Left[0].color = TreeColor::Left;
		p->Right[0].color = TreeColor::Right;

		p->Left[1].color = TreeColor::Left;
		p->Right[1].color = TreeColor::Right;

		rebuild(p->left, p->circle.getPosition() + sf::Vector2f(-OFFSET_X * level, OFFSET_Y), level);
		rebuild(p->right, p->circle.getPosition() + sf::Vector2f(OFFSET_X * level, OFFSET_Y), level);
	}
}

void createTree(int L, int R)
{
	int k;
	if (L < R)
	{
		k = Ar[L][R];
		recSDP(root, V[k], w[k], "Root", Vector2f(0,0));
		createTree(L, k - 1);
		createTree(k, R);
	}
}

int searchLevel(TreeVertex* p, int key)
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

void traversalW(TreeVertex* p) {
	if (p != nullptr) {
		traversalW(p->left);
		cout << setw(5) << "(" << p->w << ")" << " ";
		traversalW(p->right);
	}
}

void verts(TreeVertex* p) {
	if (p != nullptr) {
		verts(p->left);
		vertices.push_back(p);
		verts(p->right);
	}
}

void zoomViewAt(Vector2i pixel, RenderWindow& window, float zoom)
{
	const sf::Vector2f beforeCoord{ window.mapPixelToCoords(pixel) };
	sf::View view{ window.getView() };
	view.zoom(zoom);
	window.setView(view);
	const sf::Vector2f afterCoord{ window.mapPixelToCoords(pixel) };
	const sf::Vector2f offsetCoords{ beforeCoord - afterCoord };
	view.move(offsetCoords);
	window.setView(view);
}

void getElements(TreeVertex* p, int level, int height, vector<pair<int, int>>& graph) {
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

void traversalGraph(TreeVertex* root, int numberOfVertex, bool isChar) {
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

void A1_add(TreeVertex*& root, int** V, size_t size) {
	for (int i = 0; i < size; i++)
		recSDP(root, V[i][0], V[i][1], "Root", Vector2f(0,0));
}

void A2_add(TreeVertex*& root, int** V, int L, int R) {
	double wes = 0, sum = 0;
	if (L <= R) {
		for (int i = L; i <= R; ++i) {
			wes = wes + V[i][1];
		}
		int i = 0;
		for (i = L; i < R; ++i) {
			if ((sum <= (wes / 2) && (sum + V[i][1] >= (wes / 2)))) {
				break;
			}
			sum = sum + V[i][1];
		}
		recSDP(root, V[i][0], V[i][1], "Root", Vector2f(0, 0));
		A2_add(root, V, L, i - 1);
		A2_add(root, V, i + 1, R);
	}
}

double treeAvgHeight(TreeVertex* p, int level) {
	if (p == nullptr) {
		return 0;
	}
	return (level * p->w) + treeAvgHeight(p->left, level + 1) + treeAvgHeight(p->right, level + 1);
}

double weightSum(TreeVertex* p, int level) {
	if (p == nullptr) {
		return 0;
	}
	return p->w + weightSum(p->left, level + 1) + weightSum(p->right, level + 1);
}

double treeAvgWHeight(TreeVertex* root) {
	return treeAvgHeight(root, 1) / weightSum(root, 1);
}

void Info(TreeVertex *&root) {
	int comp = 0;

	if (Ap[0][n - 1] / (double)Aw[0][n - 1] > treeHeight()) {
		comp = 1;
	}
	else if (Ap[0][n - 1] / (double)Aw[0][n - 1] < treeHeight()) {
		comp = -1;
	}
	else comp = 0;

	cout << "Average tree height(search): " << setprecision(4) << treeHeight() << endl << endl;
	cout << "Average tree height(rec): " << setprecision(4) << treeAvgWHeight(root) << endl << endl;
	cout << "Tree weight: " << Aw[0][n - 1] << endl;
	cout << "Weighted tree height: " << Ap[0][n - 1] << endl << endl;
	cout << "Average tree height (matrix): " << setprecision(4) << Ap[0][n - 1] / (double)Aw[0][n - 1] << endl;
	cout << Ap[0][n - 1] / (double)Aw[0][n - 1] << (comp == 0 ? " = " : (comp == -1 ? " < " : " > ")) << treeHeight();
	cout << endl;
}
double pervoe = 0, vtoroe = 0;

void approximateTree(TreeVertex *&root, int** V, size_t n, string type) {
	if (type == "A1") {
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (V[i][1] > V[j][1])
					swap(V[i], V[j]);
	}
	else if (type == "A2") {
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (V[i][0] < V[j][0])
					swap(V[i], V[j]);
	}

	cout << endl << "Vertex array: ";
	for (int i = 0; i < n; i++) {
		cout << V[i][0] << "(" << V[i][1] << ")" << " ";
	}
	cout << endl;

	if (type == "A1") {
		A1_add(root, V, n);

	}
	else if (type == "A2") {
		A2_add(root, V, 0, n - 1);
	}

	cout << endl << "Tree:" << endl;
	traversalLR(root);
	cout << endl;
	traversalW(root);
	cout << endl << endl;

	cout << "Tree size: " << treeSize(root);
	cout << endl << endl << "Check sum: " << treeCheckSum(root);
	cout << endl << endl << "Tree height: " << treeHeight(root);
	cout << endl << endl << "Tree average height: " << averagePathLenght(root, 1) / (double)(treeSize(root));
	cout << endl << endl;


	if (type == "A1") {
		pervoe = averagePathLenght(root, 1) / (double)(treeSize(root));
	}
	else if (type == "A2") {
		vtoroe = averagePathLenght(root, 1) / (double)(treeSize(root));
	}
}

void fillVertsRand(int** apV, size_t n) {
	set<int> randV, randW;

	for (int i = 0; randV.size() < n; i++)
		randV.insert(rand() % 99 + 0);

	for (int i = 0; randW.size() < n; i++)
		randW.insert(rand() % 89 + 10);

	vector<int> tempV(randV.size()), tempW(randW.size());

	copy(randV.begin(), randV.end(), tempV.begin());
	copy(randW.begin(), randW.end(), tempW.begin());

	unsigned seedV = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(tempV.begin(), tempV.end(), std::default_random_engine(seedV));

	unsigned seedW = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(tempW.begin(), tempW.end(), std::default_random_engine(seedW));

	//for (int i = 0; i < n; i++) {
	//	apV[i][0] = tempV[i];
	//	apV[i][1] = tempW[i];
	//}

	for (int i = 1; i < n+1; i++) {
		apV[i-1][0] = V[i];
		apV[i-1][1] = w[i];
	}
}

//vector<VertexArray*> highlightRects;

FloatRect rectL = FloatRect(INT_MAX, INT_MAX, -INT_MAX, -INT_MAX);
FloatRect rectR = FloatRect(INT_MAX, INT_MAX, -INT_MAX, -INT_MAX);

void clearRects() {
	rectL = FloatRect(INT_MAX, INT_MAX, -INT_MAX, -INT_MAX);
	rectR = FloatRect(INT_MAX, INT_MAX, -INT_MAX, -INT_MAX);
}

void buildRect(VertexArray &verts, const FloatRect& rect) {
	verts[0].position = Vector2f(rect.left, rect.top);
	verts[1].position = Vector2f(rect.width, rect.top);
	verts[2].position = Vector2f(rect.width, rect.height);
	verts[3].position = Vector2f(rect.left, rect.height);
	verts[4].position = Vector2f(rect.left, rect.top);
	//highlightRects.push_back(&verts);
}

void rectOut(FloatRect& rect) {
	cout << endl <<
		"LT: {" << rect.left << ", " << rect.top << "}, " <<
		"RT: {" << rect.width << ", " << rect.top << "}, " <<
		"RB: {" << rect.width << ", " << rect.height << "}, " <<
		"LB: {" << rect.left << ", " << rect.height << "}" << endl;
}

void findSides(TreeVertex *p, FloatRect &rect) {
	if (p != nullptr) {
		Vector2f pos = p->circle.getPosition();

		if (pos.x < rect.left)
			rect.left = pos.x;

		if (pos.y < rect.top)
			rect.top = pos.y;
		
		if (pos.x > rect.width)
			rect.width = pos.x;
		
		if (pos.y > rect.height)
			rect.height = pos.y;

		findSides(p->left, rect);
		findSides(p->right, rect);
	}
}

void createRect(TreeVertex* p, string side) {
	if (p != nullptr) {
		if (side == "Root") {
			return;
		}
		else if (side == "L") {
			findSides(p, rectL);
		}
		else if (side == "R") {
			findSides(p, rectR);
		}
	}
}


void align(TreeVertex* p, string side, VertexArray &left, VertexArray &right, float delta) {
	if (p != nullptr) {
		align(p->left, "L", left, right, delta);
		align(p->right, "R", left, right, delta);

		clearRects();
		
		createRect(p->left, "L");
		createRect(p->right, "R");
		
		buildRect(left, rectL);
		buildRect(right, rectR);
		
		float overlap = rectL.width - rectR.left;
		
		if (overlap >= 0) {
			float offset = overlap * 0.5 + 40;
			if (p->left != nullptr)
				p->left->circle.move(-offset, 0);
			
			if (p->right != nullptr)
				p->right->circle.move(offset, 0);
		
			rebuild(p->left, p->left->circle.getPosition(), treeHeight(p->left));
			rebuild(p->right, p->right->circle.getPosition(), treeHeight(p->right));
			
			p->Left[1].position = p->left->circle.getPosition();
			p->Right[1].position = p->right->circle.getPosition();
		}
	}
}

TreeVertex* deleteSDP(TreeVertex* root, int X) {
	TreeVertex* q, * p, * r;

	if (root == nullptr)
		return root;

	if (X == root->data) {
		if (root->right == nullptr)
			q = root->left;
		else {
			p = root->right;
			if (p->left == nullptr) {
				p->left = root->left;
				q = p;
			}
			else {
				r = p->left;
				while (r->left != nullptr) {
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

int main()
{
	setlocale(LC_ALL, "RUSSIAN");
	srand((unsigned)time(NULL));

	init();

	cout << "TreeVertex array: ";
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

	Info(root);

	cout << endl << endl << endl;

	--n;

	int** apV = new int* [n];

	for (int i = 0; i < n; i++) {
		apV[i] = new int[2];
	}
	apV[0][0] = 0;
	apV[0][1] = 0;

	//int* VR = new int[n] {14, 6, 15, 30, 27, 10, 15, 16, 3, 5, 1, 13, 20, 25};
	//int* WR = new int[n] {1, 1, 4, 1, 1, 3, 1, 3, 2, 1, 2, 1, 1, 1, };

	int* VR = new int[14]{ 10, 3, 15, 18, 6, 13, 7, 12, 4, 16, 30, 1, 5, 25 };
	int* WR = new int[14]{ 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

	for (int i = 0; i < n; i++) {
		apV[i][0] = VR[i];
		apV[i][1] = WR[i];
	}

	fillVertsRand(apV, n);

	TreeVertex* root1 = nullptr, * root2 = nullptr;
	
	cout << "A1: " << endl;
	approximateTree(root1, apV, n, "A1");
	cout << endl << "A2: " << endl;
	approximateTree(root2, apV, n, "A2");

	//AS_V = new int[12]{
	//'А',
	//'В',
	//'Е',
	//'И',
	//'К',
	//'Л',
	//'Н',
	//'О',
	//'П',
	//'Р',
	//'Т',
	//'У'
	//};
	//AS_W = new int[12]{ 4, 3, 2, 1, 2, 1, 2, 3, 1, 2, 1, 1 };


	ContextSettings settings;
	settings.antialiasingLevel = 16;
	RenderWindow window(VideoMode(1024, 800), "VisualTree", Style::Default, settings);
	//window.close();
	window.setVerticalSyncEnabled(true);
	View view(FloatRect(0, 0, 1024, 800));

	rebuild(root, Vector2f(0, 0), treeHeight(root));
	rebuild(root1, Vector2f(0, 0), treeHeight(root1));
	rebuild(root2, Vector2f(0, 0), treeHeight(root2));

	VertexArray left(LineStrip, 5);

	for (int i = 0; i < 5; i++) {
		left[i].color = Color::Green;
	}

	VertexArray right(LineStrip, 5);

	for (int i = 0; i < 5; i++) {
		right[i].color = Color::Yellow;
	}

	TreeVertex* choosed = nullptr;
	TreeVertex* choosedRoot = nullptr;
	Color previous;

	bool moving = false;
	bool isHighlight = false;
	Vector2f oldPos;

	form::Label treeType(L"Undef", Vector2f(-200, -200), 32);

	Clock clock;

	while (window.isOpen()) {
		Event event;
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		// convert it to world coordinates
		sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::MouseButtonPressed:
				if (event.mouseButton.button == Mouse::Button::Left) {
					oldPos = worldPos;
					moving = true;
				}
				break;
			case Event::MouseButtonReleased:
				if (event.mouseButton.button == Mouse::Button::Left) {
					moving = false;
				}
				break;
			case Event::MouseMoved:
				if (moving) {

					Vector2f newPos = worldPos;

					Vector2f deltaPos = (newPos - oldPos) * 0.9f;

					view = window.getView();

					view.setCenter(view.getCenter() - deltaPos);

					window.setView(view);

					oldPos = worldPos;
				}
				break;
			case Event::MouseWheelScrolled:
				if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
					if (event.mouseWheelScroll.delta > 0)
						zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, (1.f / 1.1f));
					else if (event.mouseWheelScroll.delta < 0)
						zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, 1.1f);
				}
				break;
			case Event::Resized:
				view.setSize(Vector2f(window.getSize()));
				window.setView(view);
				break;
			case Event::KeyPressed:
				switch (event.key.code)
				{
				case Keyboard::W:
					isHighlight = !isHighlight;
					break;
				case Keyboard::A:
					treeType.setText(L"Точный алгоритм");
					vertices.clear();
					verts(root);
					clearRects();
					choosedRoot = *&root;
					break;
				case Keyboard::S:
					treeType.setText(L"А1");
					vertices.clear();
					verts(root1);
					clearRects();
					choosedRoot = *&root1;
					break;
				case Keyboard::D:
					treeType.setText(L"А2");
					vertices.clear();
					verts(root2);
					clearRects();
					choosedRoot = *&root2;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			for (int i = 0; i < vertices.size(); i++) {
				if (vertices[i]->circle.getGlobalBounds().contains(worldPos) &&
					event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left &&
					vertices[i]) {

					//system("cls");

					if (choosed != nullptr) {
						choosed->circle.setFillColor(previous);
						choosed->circle.setScale(1, 1);
						if (choosed->left != nullptr) {
							choosed->left->circle.setFillColor(Color::White);
							choosed->left->circle.setScale(1, 1);
						}
						if (choosed->right != nullptr) {
							choosed->right->circle.setFillColor(Color::White);
							choosed->right->circle.setScale(1, 1);
						}
					}

					if (choosed != vertices[i]) {
						previous = vertices[i]->circle.getFillColor();

						vertices[i]->circle.setFillColor(TreeColor::Choosed);
						vertices[i]->circle.setScale(1.5, 1.5);
						cout << vertices[i]->data << "(" << vertices[i]->w << ")" << endl;
						if (vertices[i]->left != nullptr) {
							vertices[i]->left->circle.setFillColor(TreeColor::Left);
							vertices[i]->left->circle.setScale(1.5, 1.5);
							cout << "\tLeft: " << vertices[i]->left->data << "(" << vertices[i]->left->w << ")" << endl;
						}
						if (vertices[i]->right != nullptr) {
							vertices[i]->right->circle.setFillColor(TreeColor::Right);
							vertices[i]->right->circle.setScale(1.5, 1.5);
							cout << "\tRight: " << vertices[i]->right->data << "(" << vertices[i]->right->w << ")" << endl;
						}
						choosed = *&vertices[i];
					}
					else {
						choosed = nullptr;
					}

				}
			}
		}

		window.clear(Color(38, 50, 56));

		for (int i = 0; i < vertices.size(); i++) {
			window.draw(vertices[i]->Left);
			window.draw(vertices[i]->Right);
		}

		for (int i = 0; i < vertices.size(); i++) {
			window.draw(vertices[i]->circle);
		}

		if (isHighlight) {
			//for (auto i : highlightRects)
				//window.draw(*i);
			window.draw(left);
			window.draw(right);
		}

		if (choosedRoot != nullptr)
			align(choosedRoot, "Root", left, right, clock.restart().asSeconds());

		window.draw(treeType.getText());

		window.display();
	}

	for (int i = 0; i < n; i++) {
		root1 = deleteSDP(root1, apV[i][0]);
		root2 = deleteSDP(root2, apV[i][0]);
	}

	root1 = nullptr;
	root2 = nullptr;

	for (int i = 0; i < n; i++) {
		delete apV[i];
	}
	delete[] apV;

	return 0;
}

void traversalTB(TreeVertex* p) {
	if (p != nullptr) {
		cout << (char)p->data << " ";
		traversalTB(p->left);
		traversalTB(p->right);
	}
}

void traversalLR(TreeVertex* p) {
	if (p != nullptr) {
		traversalLR(p->left);
		cout << setw(5) << " " << p->data << " " << " ";
		traversalLR(p->right);
	}
}

void traversalBT(TreeVertex* p) {
	if (p != nullptr) {
		traversalBT(p->left);
		traversalBT(p->right);
		cout << p->data << " ";
	}
}

int treeSize(TreeVertex* p) {
	if (p == nullptr) {
		return 0;
	}
	return 1 + treeSize(p->left) + treeSize(p->right);
}

int treeCheckSum(TreeVertex* p) {
	if (p == nullptr) {
		return 0;
	}
	return p->data + treeCheckSum(p->left) + treeCheckSum(p->right);
}

int treeHeight(TreeVertex* p) {
	if (p == nullptr) {
		return 0;
	}
	return 1 + max(treeHeight(p->left), treeHeight(p->right));
}

int averagePathLenght(TreeVertex* p, int level) {
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