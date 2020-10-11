#include <fstream>
#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "Form.h"
#include "Employee.h"
#include "List.h"

static void cp866_to_utf8(String& out, const char* in, size_t size) {
	static const wchar_t utf8[69] = {
		L"\u0410\u0411\u0412\u0413\u0414\u0415\u0416\u0417\u0418\u0419\u041A\u041B\u041C\u041D\u041E\u041F"
		L"\u0420\u0421\u0422\u0423\u0424\u0425\u0426\u0427\u0428\u0429\u042A\u042B\u042C\u042D\u042E\u042F"
		L"\u0430\u0431\u0432\u0433\u0434\u0435\u0436\u0437\u0438\u0439\u043A\u043B\u043C\u043D\u043E\u043F"
		L"\u0440\u0441\u0442\u0443\u0444\u0445\u0446\u0447\u0448\u0449\u044A\u044B\u044C\u044D\u044E\u044F"
		L"\u0020\u002D\u2593\u002F"
	};

	out.clear();
	
	int i = 0;
	while (i < size) {
		int cp866 = (uint8_t)in[i];
		int utf8Index;

		if (cp866 > 127 && cp866 < 176) {
			utf8Index = cp866 - 128;
		}
		else if (cp866 > 223) {
			utf8Index = cp866 - 48 - 128;
		}
		else if (cp866 == 32) {
			utf8Index = 64;
		}
		else if (cp866 == 45) {
			utf8Index = 65;
		}
		else if (cp866 == 47) {
			utf8Index = 67;
		}
		else if (i == size - 1) {
			utf8Index = 64;
		}
		else {
			utf8Index = 66;
		}

		out += utf8[utf8Index];
		i++;
	}
}

using namespace std;
using namespace sf;

void printTable(form::Table table, List<Employee> &employers, int start, int end) {
	int row = 0;
	size_t cellH = 43;
	String name;
	String place;
	if (start >= 0 && end <= 4000) {
		for (int i = start; i < end; i++) {

			table.getCells()[0][row].setText(to_string(i + 1));
			table.getCells()[0][row].setSize(Vector2f(60, cellH));

			cp866_to_utf8(name, employers[i].FIO, sizeof(employers[i].FIO));
			table.getCells()[1][row].setText(name);
			table.getCells()[1][row].setSize(Vector2f(300, cellH));

			table.getCells()[2][row].setText(to_string(employers[i].departmentNumber));
			table.getCells()[2][row].setSize(Vector2f(100, cellH));

			cp866_to_utf8(place, employers[i].place, sizeof(employers[i].place));
			table.getCells()[3][row].setText(place);
			table.getCells()[3][row].setSize(Vector2f(300, cellH));

			table.getCells()[4][row].setText(employers[i].birthDate);
			table.getCells()[4][row].setSize(Vector2f(100, cellH));

			row++;
		}
		row = 0;
	}
}

void MainMenu::Run(RenderWindow& window)
{
	bool isMenu = true;

	ifstream databaseFile("testBase2.dat", ios::in | ios::binary);

	if (!databaseFile) {
		cerr << "databaseFile not found";
		databaseFile.clear();
	}

	databaseFile.ignore(numeric_limits<streamsize>::max());
	streamsize length = databaseFile.gcount();
	databaseFile.clear();
	databaseFile.seekg(0, ios_base::beg);

	size_t numOfEmployers = length / sizeof(Employee);

	List<Employee> employers;

	for (int i = 0; i < numOfEmployers; i++) {
		Employee tempEmployee;
		tempEmployee.getEmployeeFromFile(databaseFile);
		employers.push_back(tempEmployee);
	}

	databaseFile.close();

	size_t start = 0;
	size_t end = 20;
	
	form::Table table(5, 20);

	form::Label alert(L"Нажмите Enter \n \tдля следующих 20 пунктов", Vector2f(50, 50), 24, form::Align::Left);

	printTable(table, employers, start, end);

	table.update();
	table.setPosition(Vector2f(
		(window.getSize().x / 2) - (table.width() / 2),
		(window.getSize().y / 2) - (table.height() / 2)
		));

	while (isMenu) {
		Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				employers.~List();
				window.close();
				return;
				break;
			case Event::KeyPressed:
				switch (event.key.code)
				{
				case Keyboard::Right:
					if (start < numOfEmployers - 20 && end < numOfEmployers) {
						start = end;
						end += 20;
					}
					printTable(table, employers, start, end);

					table.setPosition(Vector2f(
						(window.getSize().x / 2) - (table.width() / 2),
						(window.getSize().y / 2) - (table.height() / 2)
					));
					break;
				case Keyboard::Left:
					if (start >= 20) {
						end = start;
						start -= 20;
					}
					printTable(table, employers, start, end);

					table.setPosition(Vector2f(
						(window.getSize().x / 2) - (table.width() / 2),
						(window.getSize().y / 2) - (table.height() / 2)
					));
					break;
				default:
					break;
				}
				//cout << start << " " << end << endl;
			default:
				break;
			}
		}

		window.clear(form::fColor::Background);

		//window.draw(alert.getText());

		for (int col = 0; col < table.cols(); col++) {
			for (int row = 0; row < table.rows(); row++) {
				window.draw(table.getCells()[col][row].getButton());
				window.draw(table.getCells()[col][row].getText());
			}
		}

		window.display();
	}
}

