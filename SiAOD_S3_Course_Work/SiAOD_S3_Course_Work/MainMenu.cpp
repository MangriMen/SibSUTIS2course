#include <fstream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <thread>
#include "MainMenu.h"
#include "Form.h"
#include "Employee.h"
#include "BTree.h"
#include "Queue.h"
#include "MergeSort.h"
#include "Utils.h"

using namespace std;
using namespace sf;

void printTable(form::Table table, Employee** employers, int n, int start, int end) {
	int row = 0;
	size_t cellH = 43;
	String name;
	String place;
	
	if (start >= 0 && end <= n+(20-n%20)) {
		for (int i = start; i < end; i++) {
			if (i < n) {
				table.getCells()[0][row].setText(to_string(i + 1));
				table.getCells()[0][row].setSize(Vector2f(60, static_cast<float>(cellH)));

				cp866_to_utf8(name, employers[i]->FIO, sizeof(employers[i]->FIO));
				table.getCells()[1][row].setText(name);
				table.getCells()[1][row].setSize(Vector2f(300, static_cast<float>(cellH)));

				table.getCells()[2][row].setText(to_string(employers[i]->departmentNumber));
				table.getCells()[2][row].setSize(Vector2f(100, static_cast<float>(cellH)));

				cp866_to_utf8(place, employers[i]->place, sizeof(employers[i]->place));
				table.getCells()[3][row].setText(place);
				table.getCells()[3][row].setSize(Vector2f(300, static_cast<float>(cellH)));

				table.getCells()[4][row].setText(employers[i]->birthDate);
				table.getCells()[4][row].setSize(Vector2f(100, static_cast<float>(cellH)));
			} else {
				table.getCells()[0][row].setText(to_string(i + 1));
				table.getCells()[0][row].setSize(Vector2f(60, static_cast<float>(cellH)));

				table.getCells()[1][row].setText(" ");
				table.getCells()[1][row].setSize(Vector2f(300, static_cast<float>(cellH)));

				table.getCells()[2][row].setText(" ");
				table.getCells()[2][row].setSize(Vector2f(100, static_cast<float>(cellH)));

				table.getCells()[3][row].setText(" ");
				table.getCells()[3][row].setSize(Vector2f(300, static_cast<float>(cellH)));

				table.getCells()[4][row].setText(" ");
				table.getCells()[4][row].setSize(Vector2f(100, static_cast<float>(cellH)));
			}

			row++;
		}
		row = 0;
	}
}

enum class Mode{
	All,
	Founded
};

void MainMenu::RunThread()
{
	m_thread.launch();
}

void MainMenu::Run()
{
	window.create(VideoMode(1024, 800), "Employers");
	window.setVerticalSyncEnabled(true);
	View view = window.getDefaultView();
	window.setView(view);

	Mode mode;
	int key = 0;

	size_t start = 0;
	size_t end = 20;
	size_t startFounded = 0;
	size_t endFounded = 20;
	
	form::Table table(5, 20);
	
	mode = Mode::All;
	printTable(table, *&reader->employersI, static_cast<int>(reader->numOfEmployers), static_cast<int>(start), static_cast<int>(end));

	table.update();
	Vector2f defaultPosition = Vector2f(
		(static_cast<float>(window.getSize().x) * 0.5f) - (table.width() * 0.5f),
		(static_cast<float>(window.getSize().y) * 0.08f)
	);

	table.setPosition(defaultPosition);

	form::Label keyText(L"Ключ поиска: "+ to_string(key), (table.getPosition()), 24, form::Align::Left);
	keyText.setPosition(table.getPosition() - Vector2f(keyText.getText().getLocalBounds().width - table.width(), 40));

	form::Label modeText(L"Все работники", (table.getPosition() - Vector2f(0, 40)), 24, form::Align::Left);

	while (window.isOpen()) {
		Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				reader->FreeReader();
				return;
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
				view.reset(FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height)));
				view.zoom(1.0f);
				window.setView(view);
				break;
			case Event::KeyPressed:
				switch (event.key.code)
				{
				case Keyboard::B:
					reader->btree.Print(reader->btree.root);
					reader->btree.Info();
					break;
				case Keyboard::Right:
					switch (mode)
					{
					case Mode::All:
						if (start < static_cast<int>(reader->numOfEmployers) - 20 && end < static_cast<int>(reader->numOfEmployers)) {
							start = end;
							end += 20;
						}
						printTable(table, *&reader->employersI, static_cast<int>(reader->numOfEmployers), static_cast<int>(start), static_cast<int>(end));
						break;
					case Mode::Founded:
						if (startFounded < static_cast<int>(reader->numOfFounded) - (long)20 && endFounded < static_cast<int>(reader->numOfFounded)) {
							startFounded = endFounded;
							endFounded += 20;
						}
						printTable(table, *&reader->employersFoundedI, static_cast<int>(reader->numOfFounded), static_cast<int>(startFounded), static_cast<int>(endFounded));
						break;
					default:
						break;
					}

					table.setPosition(defaultPosition);
					break;
				case Keyboard::Left:
					switch (mode)
					{
					case Mode::All:
						if (start >= 20) {
							end = start;
							start -= 20;
						}
						printTable(table, *&reader->employersI, static_cast<int>(reader->numOfEmployers), static_cast<int>(start), static_cast<int>(end));
						break;
					case Mode::Founded:
						if (startFounded >= 20) {
							endFounded = startFounded;
							startFounded -= 20;
						}
						printTable(table, *&reader->employersFoundedI, static_cast<int>(reader->numOfFounded), static_cast<int>(startFounded), static_cast<int>(endFounded));
						break;
					default:
						break;
					}

					table.setPosition(defaultPosition);
					break;
				case Keyboard::Space:
					switch (mode)
					{
					case Mode::All:
						printTable(table, *&reader->employersFoundedI, static_cast<int>(reader->numOfFounded), static_cast<int>(startFounded), static_cast<int>(endFounded));
						mode = Mode::Founded;
						break;
					case Mode::Founded:
						printTable(table, *&reader->employersI, static_cast<int>(reader->numOfEmployers), static_cast<int>(start), static_cast<int>(end));
						mode = Mode::All;
						break;
					default:
						break;
					}
					break;
				case Keyboard::F:
					cout << "\033[1A\033[K" << string(80, ' ') << "\r";
					cout << "Enter the key: ";
					cin >> key;

					keyText.setText(L"Ключ поиска: " + to_string(key));
					keyText.setPosition(table.getPosition() - Vector2f(keyText.getText().getLocalBounds().width - table.width(), 40));

					Delete(&reader->employersFoundedQ.head);
					delete[] reader->employersFoundedI;
					
					reader->numOfFounded = queueFromKey(reader->employersI, reader->employersFoundedQ, key, static_cast<int>(static_cast<int>(reader->numOfEmployers)));
					reader->employersFoundedI = new Employee * [static_cast<int>(reader->numOfFounded)];
					fillIndexArray(reader->employersFoundedQ, reader->employersFoundedI);

					for (int i = 0; i < static_cast<int>(reader->numOfFounded); i++)
					{
						reader->btree.deleteSDP(reader->employersFoundedI[i], reader->btree.root);
					}
					reader->btree.root = nullptr;
					reader->btree.vertices.clear();

					reader->btree.From(reader->employersFoundedQ);

					reader->btree.rebuild(reader->btree.root, Vector2f(0, 0), reader->btree.treeHeight(reader->btree.root));
					reader->btree.verts(reader->btree.root);

					switch (mode)
					{
					case Mode::All:
						break;
					case Mode::Founded:
						startFounded = 0;
						endFounded = 20;
						printTable(table, *&reader->employersFoundedI, static_cast<int>(static_cast<int>(reader->numOfFounded)), static_cast<int>(startFounded), static_cast<int>(endFounded));
						break;
					default:
						break;
					}
					
					break;
				default:
					break;
				}
			default:
				break;
			}
		}

		switch (mode)
		{
		case Mode::All:
			modeText.setText(L"Все работники");
			break;
		case Mode::Founded:
			modeText.setText(L"Найденные по номеру отдела");
			break;
		default:
			break;
		}


		window.clear(form::fColor::Background);

		window.draw(keyText.getText());
		window.draw(modeText.getText());

		for (int col = 0; col < table.cols(); col++) {
			for (int row = 0; row < table.rows(); row++) {
				window.draw(table.getCells()[col][row].getButton());
				window.draw(table.getCells()[col][row].getText());
			}
		}

		window.display();
	}
}