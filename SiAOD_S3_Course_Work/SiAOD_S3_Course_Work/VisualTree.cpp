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
	if (isFullscreen) {
		window.create(VideoMode(1024, 800), "VisualTree", Style::Default);
	}
	else {
		window.create(VideoMode(sf::VideoMode::getFullscreenModes()[0]), "VisualTree", Style::None);
	}
	isFullscreen = !isFullscreen;
}

void VisualTree::Run()
{
	ContextSettings settings;
	settings.antialiasingLevel = 16;
	window.create(VideoMode(1024, 800), "VisualTree", Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	View view(FloatRect(0, 0, 1024, 800));

	reader->btree.reload(reader->btree.root, Vector2f(0, 0), reader->btree.treeHeight(reader->btree.root));
	reader->btree.verts(reader->btree.root);

	info = new InfoCard();

	TreeVertex* choosed = nullptr;
	Color previous;

	bool moving = false;
	Vector2f oldPos;

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

					if (choosed != reader->btree.vertices[i]) {
						previous = reader->btree.vertices[i]->circle.getFillColor();
						info->setPosition(
								reader->btree.vertices[i]->circle.getPosition() + 
								Vector2f(
									reader->btree.vertices[i]->circle.getLocalBounds().width * 0.12f,
									-reader->btree.vertices[i]->circle.getLocalBounds().height * 0.12f
								)
							);

						info->SetEmployee(
							reader->btree.vertices[i]->data.FIO,
							reader->btree.vertices[i]->data.departmentNumber,
							reader->btree.vertices[i]->data.place,
							reader->btree.vertices[i]->data.birthDate
						);

						reader->btree.vertices[i]->circle.setFillColor(TreeColor::Choosed);
						reader->btree.vertices[i]->circle.setScale(1.5, 1.5);
						cout << reader->btree.vertices[i]->data.FIO << " " << reader->btree.vertices[i]->data.departmentNumber << " " << "\tBal: " << reader->btree.vertices[i]->balance << endl;
						if (reader->btree.vertices[i]->left != nullptr) {
							reader->btree.vertices[i]->left->circle.setFillColor(TreeColor::Left);
							reader->btree.vertices[i]->left->circle.setScale(1.5, 1.5);
							cout << "\tLeft: " << reader->btree.vertices[i]->left->data.FIO << " " << reader->btree.vertices[i]->left->data.departmentNumber << endl;
						}
						if (reader->btree.vertices[i]->right != nullptr) {
							reader->btree.vertices[i]->right->circle.setFillColor(TreeColor::Right);
							reader->btree.vertices[i]->right->circle.setScale(1.5, 1.5);
							cout << "\tRight: " << reader->btree.vertices[i]->right->data.FIO << " " << reader->btree.vertices[i]->right->data.departmentNumber << endl;
						}
						choosed = *&reader->btree.vertices[i];
					}
					else {
						choosed = nullptr;
					}

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
