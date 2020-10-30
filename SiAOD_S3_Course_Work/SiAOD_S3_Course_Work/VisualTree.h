#pragma once
#include <thread>
#include <SFML/Graphics.hpp>
#include "Reader.h"
#include "TreeVertex.h"

using namespace std;
using namespace sf;

class VisualTree
{
private:
	RenderWindow window;

	Thread m_thread;

	Font font;

	class Node {
	public:
		TreeVertex* vertex;
		int level;
		CircleShape circle;
		
		Node();

		Node(TreeVertex *vertex_, int level_, Font &font) {
			vertex = vertex_;
			level = level_;
			circle.setRadius(40);
			circle.setFillColor(Color::White);
			if (vertex == nullptr) {
				circle.setFillColor(Color::Red);
			}
			circle.setPointCount(30);
			circle.setOutlineColor(Color::Black);
			circle.setOutlineThickness(-4);
			circle.setOrigin(circle.getLocalBounds().width * 0.5f, circle.getLocalBounds().height * 0.5f);
		}
	};
	int shift = 100;

	int size = 0;

	void Run();
public:
	Node** nodes = new Node*[4000];
	Reader* reader;

	VisualTree() : m_thread(&VisualTree::Run, this), reader(nullptr) { font.loadFromFile("fonts/Roboto-Medium.ttf"); };

	void RunThread();

	void getElements(TreeVertex* p, int level, int height, vector<pair<Employee*, int>>& graph);
	void traversalGraph(TreeVertex* root);

	int deep = 0;
	void Build(TreeVertex* root, int level, int height) {
		if (root == nullptr) {
			if (level <= height) {
				Node* tmp = new Node(root, level, *&font);
				nodes[size] = *&tmp;
				size++;
			}
			return;
		}
		Node* tmp = new Node(root, level, *&font);
		nodes[size] = *&tmp;
		size++;
		Build(root->left, level + 1, height);
		Build(root->right, level + 1, height);
	}
};
