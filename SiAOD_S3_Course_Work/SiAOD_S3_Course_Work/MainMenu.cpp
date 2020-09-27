#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <conio.h>
#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "Form.h"

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

struct Employee
{
	char FIO[30];
	short int departmentNumber;
	char place[22];
	char birthDate[10];
};

void MainMenu::Run(RenderWindow& window)
{
	bool isMenu = true;

	ifstream databaseFile("testBase2.dat", ios::in | ios::binary);

	databaseFile.ignore(numeric_limits<streamsize>::max());
	streamsize length = databaseFile.gcount();
	databaseFile.clear();
	databaseFile.seekg(0, ios_base::beg);

	if (!databaseFile) {
		cerr << "databaseFile not found";
		databaseFile.clear();
	}

	int numOfEmployees = length / sizeof(Employee);

	Employee* employee = new Employee[numOfEmployees];

	short int* temp = new short int[sizeof(short int)];

	for (int i = 0; i < numOfEmployees; i++) {
		databaseFile.read(employee[i].FIO, sizeof(employee[i].FIO));
		for (int j = 0; j < sizeof(employee[i].departmentNumber); j++) { temp[j] = databaseFile.get(); }
		employee[i].departmentNumber = temp[0] + temp[1];
		databaseFile.read(employee[i].place, sizeof(employee[i].place));
		databaseFile.read(employee[i].birthDate, sizeof(employee[i].birthDate));
	}

	databaseFile.clear();
	databaseFile.close();
	delete[] temp;

	size_t start = 0;
	size_t end = 20;
	
	bool graphics = false;
	int type = 0;

	cout << "Choose what kind of output do you want to have. 1 for graphics, 0 for command: ";
	cin >> graphics;
	cout << endl;

	if (!graphics) {
		window.close();
		cout << "Enter type of output table: ";
		cin >> type;
		cout << endl;
		while (end <= numOfEmployees) {
			cout << endl;
			if (type == 0) {
				for (int i = start; i < end; i++) {
					cout << "Employee " << i + 1 << ":" << endl;
					cout << "\t" << "Full name: " << employee[i].FIO << endl;
					cout << "\t" << "Deparment number: " << employee[i].departmentNumber << endl;
					cout << "\t" << "Place: " << employee[i].place << endl;
					cout << "\t" << "Birthdate: " << employee[i].birthDate << endl;
					cout << endl;
				}
			}
			else if (type == 1) {
				for (int i = start; i < end; i++) {
					cout << "\t" << i + 1 << "\t"
						 << employee[i].FIO << "\t"
						 << employee[i].departmentNumber << "\t"
						 << employee[i].place << "\t"
						 << employee[i].birthDate << endl << endl;
				}
			}
			else if (type == 2) {
				cout << "\t----------------------------------------------------------------------------------\n";
				for (int i = start; i < end; i++) {
					cout << "\t|" << i + 1 << "\t"
						<< employee[i].FIO << "\t"
						<< employee[i].departmentNumber << "\t"
						<< employee[i].place << "\t"
						<< employee[i].birthDate << "|" << endl;
					cout << "\t----------------------------------------------------------------------------------\n";
				}
			}
			cout << "For next 20 employees press Enter";
			char ch = _getch();
			if (ch == 27) {
				exit(0);
			}
			else if (ch == '\r') {
				cout << "\r                                        \t" << end / 20 << "/" << numOfEmployees / 20 << endl;
				start = end;
				end = end + 20;
			}
		}
	}


	form::Table table(5, 20);

	form::Label alert(L"Нажмите Enter \n \tдля следующих 20 пунктов", Vector2f(50, 50), 24, form::Align::Left);
	
	String name;
	String place;
	size_t cellH = 43;

	start = 0;
	end = 20;
	int row = 0;
	for (int i = start; i < end; i++) {
		table.getCells()[0][row].setText(to_string(i+1));
		table.getCells()[0][row].setSize(Vector2f(60, cellH));
		cp866_to_utf8(name, employee[i].FIO, sizeof(employee[i].FIO));
		table.getCells()[1][row].setText(name);
		table.getCells()[1][row].setSize(Vector2f(300, cellH));
		table.getCells()[2][row].setText(to_string(employee[i].departmentNumber));
		table.getCells()[2][row].setSize(Vector2f(100, cellH));
		cp866_to_utf8(place, employee[i].place, sizeof(employee[i].place));
		table.getCells()[3][row].setText(place);
		table.getCells()[3][row].setSize(Vector2f(300, cellH));
		table.getCells()[4][row].setText(employee[i].birthDate);
		table.getCells()[4][row].setSize(Vector2f(100, cellH));
		row++;
	}
	row = 0;
	start = end;
	end = end + 20;

	table.setPosition(Vector2f(445, 7));


	while (isMenu) {
		Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				delete[] employee;
				return;
				break;
			case Event::KeyPressed:
				switch (event.key.code)
				{
				case Keyboard::Enter:
					if (end > numOfEmployees)
						break;
					for (int i = start; i < end; i++) {
						table.getCells()[0][row].setText(to_string(i + 1));
						table.getCells()[0][row].setSize(Vector2f(60, cellH));
						cp866_to_utf8(name, employee[i].FIO, sizeof(employee[i].FIO));
						table.getCells()[1][row].setText(name);
						table.getCells()[1][row].setSize(Vector2f(300, cellH));
						table.getCells()[2][row].setText(to_string(employee[i].departmentNumber));
						table.getCells()[2][row].setSize(Vector2f(100, cellH));
						cp866_to_utf8(place, employee[i].place, sizeof(employee[i].place));
						table.getCells()[3][row].setText(place);
						table.getCells()[3][row].setSize(Vector2f(300, cellH));
						table.getCells()[4][row].setText(employee[i].birthDate);
						table.getCells()[4][row].setSize(Vector2f(100, cellH));
						row++;
					}
					row = 0;
					start = end;
					end = end + 20;

					table.setPosition(Vector2f(445, 7));
					break;
				default:
					break;
				}
			default:
				break;
			}
		}

		window.clear(form::fColor::Background);

		window.draw(alert.getText());

		for (int col = 0; col < table.cols(); col++) {
			for (int row = 0; row < table.rows(); row++) {
				window.draw(table.getCells()[col][row].getButton());
				window.draw(table.getCells()[col][row].getText());
			}
		}

		window.display();
	}
}
