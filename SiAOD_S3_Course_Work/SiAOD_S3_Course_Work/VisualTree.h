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
		Text text;
		
		Node();

		Node(TreeVertex *vertex_, int level_, Font &font) {
			vertex = vertex_;
			level = level_;
			circle.setRadius(20);
			circle.setPointCount(30);
			circle.setFillColor(Color::White);
			circle.setOutlineColor(Color::Black);
			circle.setOutlineThickness(-4);
			circle.setOrigin(circle.getLocalBounds().width * 0.5f, circle.getLocalBounds().height * 0.5f);
			String tmp;
			cp866_to_utf8(tmp, vertex->data.FIO, sizeof(vertex->data.FIO));
			text.setFont(font);
			text.setString(tmp);
			cout << vertex->data.FIO;
			//text.setOrigin(text.getLocalBounds().width * 0.5f, text.getLocalBounds().height * 0.5f);
			text.setFillColor(Color::Black);
		}
	};
	int shift = 100;

	Node** nodes = new Node*[4000];
	int size = 0;

	void Run();
public:
	Reader* reader;

	VisualTree() : m_thread(&VisualTree::Run, this), reader(nullptr) { font.loadFromFile("fonts/Roboto-Medium.ttf"); };

	void RunThread();

	void Build(TreeVertex* root, int level, string direction) {
		
		if (root == nullptr) {
			return;
		}
		Node* tmp = new Node(root, level, *&font);
		if (direction == "R") {
			tmp->circle.setPosition(0, 100);
			tmp->text.setPosition(0, 100);
		}
		else if (direction == "L") {
			tmp->circle.setPosition(-level*shift, level*100);
			tmp->text.setPosition(-level * shift, level * 100);
		}
		else if (direction == "R") {
			tmp->circle.setPosition(level * shift, level * 100);
			tmp->text.setPosition(level * shift, level * 100);
		}
		nodes[size] = *&tmp;
		size++;
		Build(root->left, level + 1, "L");
		Build(root->right, level + 1, "R");
	}
};
