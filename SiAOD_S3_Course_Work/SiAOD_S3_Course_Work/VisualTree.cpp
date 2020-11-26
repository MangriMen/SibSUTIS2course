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
#include "TreeColor.h"
#include "InfoCard.h"

using namespace std;

bool isFullscreen = false;

void switchFullScreen(RenderWindow& window) {
	ContextSettings settings;
	settings.antialiasingLevel = 16;
	if (isFullscreen) {
		window.create(VideoMode(1024, 800), "VisualTree", Style::Default, settings);
	}
	else {
		window.create(VideoMode(sf::VideoMode::getFullscreenModes()[0]), "VisualTree", Style::None, settings);
	}
	window.setVerticalSyncEnabled(true);
	isFullscreen = !isFullscreen;
}

FloatRect rectL = FloatRect(INT_MAX, INT_MAX, -INT_MAX, -INT_MAX);
FloatRect rectR = FloatRect(INT_MAX, INT_MAX, -INT_MAX, -INT_MAX);

void VisualTree::findSides(TreeVertex* p, FloatRect& rect) {
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

void VisualTree::createRect(TreeVertex* p, string side) {
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


void VisualTree::align(TreeVertex* p, string side, float delta) {
	if (p != nullptr) {
		align(p->left, "L", delta);
		align(p->right, "R", delta);
		rectL = FloatRect(INT_MAX, INT_MAX, -INT_MAX, -INT_MAX);
		rectR = FloatRect(INT_MAX, INT_MAX, -INT_MAX, -INT_MAX);
		createRect(p->left, "L");
		createRect(p->right, "R");
		float offset = rectL.width - rectR.left;
		if (offset >= 0) {
			if (p->left != nullptr)
				p->left->circle.move(-offset * 0.5 - 80, 0);
			if (p->right != nullptr)
				p->right->circle.move(offset * 0.5 + 80, 0);
			reader->btree.rebuild(p->left, p->left->circle.getPosition(), reader->btree.treeHeight(p->left));
			reader->btree.rebuild(p->right, p->right->circle.getPosition(), reader->btree.treeHeight(p->right));
			p->Left[1].position = p->left->circle.getPosition();
			p->Right[1].position = p->right->circle.getPosition();
		}
	}
}

void VisualTree::hightlightNode(TreeVertex* node) {
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

	if (choosed != node && node != nullptr) {
		previous = node->circle.getFillColor();
		info->setPosition(
			node->circle.getPosition() +
			Vector2f(
				node->circle.getLocalBounds().width * 0.12f,
				-node->circle.getLocalBounds().height * 0.12f
			)
		);

		info->SetEmployee(
			node->data.FIO,
			node->data.departmentNumber,
			node->data.place,
			node->data.birthDate
		);

		node->circle.setFillColor(TreeColor::Choosed);
		node->circle.setScale(1.5, 1.5);
		node->data.Print();
		cout << "\t" << "Bal: " << node->balance << endl;
		if (node->left != nullptr) {
			node->left->circle.setFillColor(TreeColor::Left);
			node->left->circle.setScale(1.5, 1.5);
			cout << "\t\tLeft: ";
			node->left->data.Print();
			cout << "\t" << "Bal: " << node->left->balance << endl;
		}
		if (node->right != nullptr) {
			node->right->circle.setFillColor(TreeColor::Right);
			node->right->circle.setScale(1.5, 1.5);
			cout << "\t\tRight: ";
			node->right->data.Print();
			cout << "\t" << "Bal: " << node->right->balance << endl;
		}
		choosed = *&node;
	}
	else {
		choosed = nullptr;
	}
}

void VisualTree::Run()
{
	ContextSettings settings;
	settings.antialiasingLevel = 16;
	window.create(VideoMode(1024, 800), "VisualTree", Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	View view(FloatRect(0, 0, 1024, 800));

	reader->btree.rebuild(reader->btree.root, Vector2f(0, 0), reader->btree.treeHeight(reader->btree.root));
	reader->btree.verts(reader->btree.root);

	info = new InfoCard();

	Color previous;
	TreeVertex* founded = nullptr;

	bool moving = false;
	Vector2f oldPos;

	Clock clock;
	string key = "";

	string found_place;
	string found_FIO;

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
				case Keyboard::Enter:
					if (Keyboard::isKeyPressed(Keyboard::LAlt) || Keyboard::isKeyPressed(Keyboard::RAlt)) {
						switchFullScreen(window);
					}
					break;
				case Keyboard::F:
					key = "";
					cout << "Key: ";
					getline(cin, key);

					founded = reader->btree.search(reader->btree.root, key);

					if (founded == nullptr) cout << endl << "Node not found" << endl;
					else hightlightNode(founded);
					view.setCenter(founded->circle.getPosition());
					window.setView(view);
					break;
				case Keyboard::B:
					reader->btree.Print(reader->btree.root);
					reader->btree.Info();
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
			for (int i = 0; i < reader->btree.vertices.size(); i++) {
				if (reader->btree.vertices[i]->circle.getGlobalBounds().contains(worldPos) &&
					event.type == event.MouseButtonReleased && event.mouseButton.button == Mouse::Left &&
					reader->btree.vertices[i]) {

					system("cls");

					hightlightNode(reader->btree.vertices[i]);
				}
			}
		}

		window.clear(form::fColor::Background);

		for (int i = 0; i < reader->btree.vertices.size(); i++) {
			window.draw(reader->btree.vertices[i]->Left);
			window.draw(reader->btree.vertices[i]->Right);
		}

		for (int i = 0; i < reader->btree.vertices.size(); i++) {
			window.draw(reader->btree.vertices[i]->circle);
		}
		
		align(reader->btree.root, "Root", clock.restart().asSeconds());

		if (choosed != nullptr)
			info->Draw(window);

		window.display();
	}
	delete info;
}

void VisualTree::RunThread()
{
	m_thread.launch();
}
