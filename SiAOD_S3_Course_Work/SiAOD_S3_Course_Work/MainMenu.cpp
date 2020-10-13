#include <fstream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include "MainMenu.h"
#include "Form.h"
#include "Employee.h"
#include "List.h"

using namespace std;
using namespace sf;

struct stack {
	stack* next;
	Employee data;
};

struct queue {
	stack* head{};
	stack* tail{};
};

void Split(stack* head, stack** a, stack** b, int& n);
void Merge(stack** head_a, int q, stack** head_b, int r, stack** c_tail, string field);
int BinarySearch(Employee** employersI, short int key, size_t size);

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

void printTable(form::Table table, Employee** employers, int n, int start, int end) {
	int row = 0;
	size_t cellH = 43;
	String name;
	String place;
	
	if (start >= 0 && end <= n+(20-n%20)) {
		for (int i = start; i < end; i++) {
			if (i < n) {
				table.getCells()[0][row].setText(to_string(i + 1));
				table.getCells()[0][row].setSize(Vector2f(60, cellH));

				cp866_to_utf8(name, employers[i]->FIO, sizeof(employers[i]->FIO));
				table.getCells()[1][row].setText(name);
				table.getCells()[1][row].setSize(Vector2f(300, cellH));

				table.getCells()[2][row].setText(to_string(employers[i]->departmentNumber));
				table.getCells()[2][row].setSize(Vector2f(100, cellH));

				cp866_to_utf8(place, employers[i]->place, sizeof(employers[i]->place));
				table.getCells()[3][row].setText(place);
				table.getCells()[3][row].setSize(Vector2f(300, cellH));

				table.getCells()[4][row].setText(employers[i]->birthDate);
				table.getCells()[4][row].setSize(Vector2f(100, cellH));
			} else {
				table.getCells()[0][row].setText(to_string(i + 1));
				table.getCells()[0][row].setSize(Vector2f(60, cellH));

				table.getCells()[1][row].setText(" ");
				table.getCells()[1][row].setSize(Vector2f(300, cellH));

				table.getCells()[2][row].setText(" ");
				table.getCells()[2][row].setSize(Vector2f(100, cellH));

				table.getCells()[3][row].setText(" ");
				table.getCells()[3][row].setSize(Vector2f(300, cellH));

				table.getCells()[4][row].setText(" ");
				table.getCells()[4][row].setSize(Vector2f(100, cellH));
			}

			row++;
		}
		row = 0;
	}
}

void MergeSort(stack** head, int n, string field) {
	stack* a = NULL, * b = NULL, * head_p = *head;
	int p, i, m, q, r;
	Split(head_p, &a, &b, n);
	queue c[2];
	p = 1;
	c[0].tail = (stack*)&c[0].head;
	while (p < n) {
		c[0].tail = (stack*)&c[0].head;
		c[1].tail = (stack*)&c[1].head;
		i = 0, m = n;
		while (m > 0) {
			if (m >= p)
				q = p;
			else
				q = m;
			m -= q;
			if (m >= p)
				r = p;
			else
				r = m;
			m -= r;
			Merge(&a, q, &b, r, &c[i].tail, field);
			i = 1 - i;
		}
		a = c[0].head, b = c[1].head;
		p *= 2;
	}
	c[0].tail->next = NULL;
	*head = c[0].head;
}

void Delete(stack** head) {
	if (*head == NULL)
		return;
	stack* p = *head, * temp;
	while (p) {
		temp = p;
		p = p->next;
		delete temp;
	}
	*head = NULL;
}

void Split(stack* head, stack** a, stack** b, int& n) {
	stack* k, * p;
	*a = head, * b = head->next;
	n = 1;
	k = *a;
	p = *b;
	while (p) {
		++n;
		k->next = p->next;
		k = p;
		p = p->next;
	}
}

void StackToQueue(stack** head, stack** tail) {
	(*tail)->next = *head;
	*tail = *head;
	*head = (*head)->next;
	(*tail)->next = NULL;
}

bool compareEmployee(Employee first, Employee second, string field) {
	if (field == "FIO") {
		return (string)first.FIO <= (string)second.FIO;
	}
	else if (field == "departmentNumber") {
		return first.departmentNumber <= second.departmentNumber;
	}
	else return false;
}

void Merge(stack** head_a, int q, stack** head_b, int r, stack** c_tail, string field) {
	while (q != 0 && r != 0) {
		if (compareEmployee((*head_a)->data, (*head_b)->data, field)) {
			StackToQueue(head_a, c_tail);
			q--;
		}
		else {
			StackToQueue(head_b, c_tail);
			r--;
		}
	}
	while (q > 0) {
		StackToQueue(head_a, c_tail);
		q--;
	}
	while (r > 0) {
		StackToQueue(head_b, c_tail);
		r--;
	}
}

void addToQueue(queue &queue, Employee employee) {
	if (queue.head == nullptr) {
		stack* temp = new stack;
		temp->data = employee;
		temp->next = nullptr;
		queue.head = temp;
		queue.tail = temp;
	}
	else {
		stack* temp = new stack;
		temp->data = employee;
		temp->next = nullptr;
		queue.tail->next = temp;
		queue.tail = queue.tail->next;
	}
}

void outRab(Employee rab) {
	cout << "\t" << rab.FIO
		 << "\t" << rab.departmentNumber
		 << "\t" << rab.place
		 << "\t" << rab.birthDate;
}

void out(queue& queue) {
	stack* current = queue.head;
	int i = 0;
	while (current != nullptr) {
		cout << "\t" << i;
		outRab(current->data);
		cout << endl;
		current = current->next;
		i++;
	}
}

void fillIndexArray(queue queue, Employee** employersI) {
	stack* current = queue.head;
	int i = 0;
	while (current != nullptr) {
		employersI[i] = &current->data;
		current = current->next;
		i++;
	}
}

int BinarySearch(Employee** employersI, short int key, size_t size) {
	int L = 0, R = size - 1, m = 0;
	while (L < R)
	{
		m = (L + R) / 2;
		if (employersI[m]->departmentNumber < key) L = m + 1;
		else R = m;
	}
	if (employersI[R]->departmentNumber == key) return R;
	else return -1;
}


int queueFromIndex(queue& queue, Employee** employersI, int index, int n) {
	int numOfFounded = 0;
	for (int i = index; i < n-1; i++) {
		if (employersI[i]->departmentNumber == employersI[i + 1]->departmentNumber) {
			numOfFounded++;
		}
		else break;
	}

	for (int i = (index + numOfFounded) - 1; i >= index; i--) {
		addToQueue(queue, *employersI[i]);
	}
	return numOfFounded;
}

int queueFromKey(Employee** employersI, queue &foundedEmployers, int key, int numOfEmployers) {
	int firstFounded = BinarySearch(*&employersI, key, numOfEmployers);

	int numOfFounded = 0;
	if (firstFounded != -1) {
		numOfFounded = queueFromIndex(foundedEmployers, employersI, firstFounded, numOfEmployers);
	}

	return numOfFounded;
}

enum class Mode{
	All,
	Founded
};

void MainMenu::Run(RenderWindow& window)
{
	bool isMenu = true;

	Mode mode;
	int key = 0;

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

	queue employers;
	Employee** employersI = new Employee*[numOfEmployers];

	for (int i = 0; i < numOfEmployers; i++) {
		Employee tempEmployee;
		tempEmployee.getEmployeeFromFile(databaseFile);
		addToQueue(employers, tempEmployee);
	}

	databaseFile.close();

	MergeSort(&employers.head, numOfEmployers, "departmentNumber");

	fillIndexArray(employers, employersI);

	queue foundedEmployers;

	int numOfFounded = queueFromKey(employersI, foundedEmployers, key, numOfEmployers);

	Employee** foundedI = new Employee *[numOfFounded];

	fillIndexArray(foundedEmployers, foundedI);

	size_t start = 0;
	size_t end = 20;
	size_t startFounded = 0;
	size_t endFounded = 20;
	
	form::Table table(5, 20);
	
	mode = Mode::All;
	printTable(table, *&employersI, numOfEmployers, start, end);

	table.update();
	Vector2f defaultPosition = Vector2f(
		(window.getSize().x / 2) - (table.width() / 2),
		(window.getSize().y * 0.08)
	);

	table.setPosition(defaultPosition);

	form::Label keyText(L"Ключ поиска: "+ to_string(key), (table.getPosition()), 24, form::Align::Left);
	keyText.setPosition(table.getPosition() - Vector2f(keyText.getText().getLocalBounds().width - table.width(), 40));

	form::Label modeText(L"Все работники", (table.getPosition() - Vector2f(0, 40)), 24, form::Align::Left);

	while (isMenu) {
		Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				Delete(&employers.head);
				Delete(&foundedEmployers.head);
				delete[] employersI;
				delete[] foundedI;
				window.close();
				return;
				break;
			case Event::KeyPressed:
				switch (event.key.code)
				{
				case Keyboard::Right:
					switch (mode)
					{
					case Mode::All:
						if (start < numOfEmployers - 20 && end < numOfEmployers) {
							start = end;
							end += 20;
						}
						printTable(table, *&employersI, numOfEmployers, start, end);
						break;
					case Mode::Founded:
						if (startFounded < (long)numOfFounded - 20 && endFounded < numOfFounded) {
							startFounded = endFounded;
							endFounded += 20;
						}
						printTable(table, *&foundedI, numOfFounded, startFounded, endFounded);
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
						printTable(table, *&employersI, numOfEmployers, start, end);
						break;
					case Mode::Founded:
						if (startFounded >= 20) {
							endFounded = startFounded;
							startFounded -= 20;
						}
						printTable(table, *&foundedI, numOfFounded, startFounded, endFounded);
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
						printTable(table, *&foundedI, numOfFounded, startFounded, endFounded);
						mode = Mode::Founded;
						break;
					case Mode::Founded:
						printTable(table, *&employersI, numOfEmployers, start, end);
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

					Delete(&foundedEmployers.head);
					delete[] foundedI;
					
					numOfFounded = queueFromKey(employersI, foundedEmployers, key, numOfEmployers);
					foundedI = new Employee * [numOfFounded];
					fillIndexArray(foundedEmployers, foundedI);

					switch (mode)
					{
					case Mode::All:
						break;
					case Mode::Founded:
						startFounded = 0;
						endFounded = 20;
						printTable(table, *&foundedI, numOfFounded, startFounded, endFounded);
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

