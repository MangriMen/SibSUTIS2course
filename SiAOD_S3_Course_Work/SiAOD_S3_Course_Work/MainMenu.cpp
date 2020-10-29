#include <fstream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include "MainMenu.h"
#include "Form.h"
#include "Employee.h"
#include "BTree.h"
#include "CPConvert.h"

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
void Merge(stack** head_a, int q, stack** head_b, int r, stack** c_tail);
int BinarySearch(Employee** employersI, short int key, size_t size);

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

void MergeSort(stack** head, int n) {
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
			Merge(&a, q, &b, r, &c[i].tail);
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

bool compareEmployee(Employee first, Employee second) {
	if (first.departmentNumber < second.departmentNumber) {
		return true;
	}
	else if (first.departmentNumber > second.departmentNumber) {
		return false;
	}
	else {
		return (string)first.FIO < (string)second.FIO;
	}
}

void Merge(stack** head_a, int q, stack** head_b, int r, stack** c_tail) {
	while (q != 0 && r != 0) {
		if (compareEmployee((*head_a)->data, (*head_b)->data)) {
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
		if (employersI[i]->departmentNumber == employersI[i + 1]->departmentNumber) { numOfFounded++; }
		else break;
	}

	for (int i = index; i < (index + numOfFounded); i++) {
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

void MainMenu::RunThread()
{
	m_thread.launch();
}

void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, float zoom)
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

void MainMenu::Run()
{
	window.create(VideoMode(1024, 800), "Employers");
	window.setVerticalSyncEnabled(true);
	View view = window.getDefaultView();
	window.setView(view);
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

	BTree btree;

	queue employers;
	Employee** employersI = new Employee*[numOfEmployers];

	for (int i = 0; i < numOfEmployers; i++) {
		addToQueue(employers, Employee(databaseFile));
	}

	databaseFile.close();

	MergeSort(&employers.head, numOfEmployers);

	fillIndexArray(employers, employersI);

	btree.From(employersI, numOfEmployers);

	//btree.Print(btree.root);
	//btree.Info();

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
	//Vector2f center = window.getDefaultView().getCenter();

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
						if (startFounded < numOfFounded - (long)20 && endFounded < numOfFounded) {
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
	Delete(&employers.head);
	Delete(&foundedEmployers.head);
	delete[] employersI;
	delete[] foundedI;
}

