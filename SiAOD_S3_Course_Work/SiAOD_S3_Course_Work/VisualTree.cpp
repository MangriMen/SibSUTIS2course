#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include "Employee.h"
#include "VisualTree.h"
#include "Form.h"
#include "BTree.h"

using namespace std;

void VisualTree::getElements(TreeVertex* p, int level, int height, vector<pair<Employee*, int>>& graph) {
	if (p == nullptr) {
		if (level <= height) {
			graph.push_back(make_pair(nullptr, level));
		}
		return;
	}
	graph.push_back(make_pair(&p->data, level));
	getElements(p->left, level + 1, height, graph);
	getElements(p->right, level + 1, height, graph);
}

void VisualTree::traversalGraph(TreeVertex* root) {
	int height = BTree::treeHeight(root);

	const int q = 2;
	int an = pow(q, height - 1);
	int sum = ((an * q) - 1) / (q - 1);

	vector<pair<Employee*, int>> graph;

	getElements(root, 1, height, graph);

	for (int i = 1; i <= height; i++) {
		int el = ceil(sum / pow(2, i)) * 40;
		int offset = floor(sum / pow(2, (i - 1 < 0 ? 0 : i - 1))) * 40 + 40;
		int geom = pow(2, (i - 1 < 0 ? 0 : i - 1));
		for (int x = 0, of = 0, j = 0; x < graph.size(); x++) {
			if (graph[x].second == i) { nodes[x]->circle.setPosition(el + ((offset + 1) * of), i * 120); of++; }
		}
	}
}

void VisualTree::Run()
{
	window.create(VideoMode(1024, 800), "VisualTree");
	window.setVerticalSyncEnabled(true);
	View view(FloatRect(0, 0, 1024, 800));
	srand(time(0));

	Build(*&reader->btree.root, 1, BTree::treeHeight(*&reader->btree.root));
	traversalGraph(*&reader->btree.root);
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
			case Event::MouseWheelScrolled:
				if (event.mouseWheelScroll.wheel == Mouse::VerticalWheel) {
					if (event.mouseWheelScroll.delta > 0)
						zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, (1.f / 1.1f));
					else if (event.mouseWheelScroll.delta < 0)
						zoomViewAt({ event.mouseWheelScroll.x, event.mouseWheelScroll.y }, window, 1.1f);
				}
				break;
			case Event::Resized:
				view.reset(FloatRect(0.f, 0.f, event.size.width, event.size.height));
				view.zoom(1.0f);
				window.setView(view);
				break;
			default:
				break;
			}
			for (int i = 0; i < size - 1; i++) {
				if (nodes[i]->circle.getGlobalBounds().contains(worldPos)) {
					if (event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left) {
						if (nodes[i]->vertex != nullptr)
							cout << nodes[i]->vertex->data.FIO << " " << nodes[i]->vertex->data.departmentNumber << endl;
					}
				}
			}
		}

		window.clear(form::fColor::Background);

		for (int i = 0; i < size-1; i++) {
			window.draw(nodes[i]->circle);
			//window.draw(nodes[i]->text);
		}

		window.display();
	}
}

void VisualTree::RunThread()
{
	m_thread.launch();
}
