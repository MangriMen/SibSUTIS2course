#include <iostream>
#include <fstream>
#define EVER ;;
using namespace std;

class DatabaseNode {
public:
    char employeeFullName[30];
    short int departmentNumber;
    char post[22];
    char birthDate[10];

    void ShowMenu(ifstream& opendFileStream, int size = 0) {
        int chooseNumber = 0;
        int leftBorder = 0;
        int rightBorder = 20;
        int tempLeftBorder = 0;
        int tempRightBorder = 0;

        for (int i = 0; i < size; ++i) {
            this[i].nodeFill(opendFileStream);
        }

        cout << "A Company Database.\n\n" 
             << "Enter the appropriate number:\n" 
             << "\"1\" to print workers information\n"
             << "\"2\" to select the next 20 workers\n"
             << "\"3\" to print information about all employees\n"
             << "\"4\" to search information about specific employees\n"
             << "\"0\" for exiting program.\n";
        for (EVER) {
            cin >> chooseNumber;
            switch (chooseNumber) {
                case 0: {
                    return;
                }
                case 3: {
                    tempLeftBorder = leftBorder;
                    tempRightBorder = rightBorder;
                    leftBorder = 0;
                    rightBorder = size;
                }
                case 1: {
                    cout << "\t" << "Full name: " << "\t\t\t\t" << "Dep. No: " << "\t"
                        << "Post: " << "\t\t\t" << "Date of birth: " << "\n";
                    for (int i = leftBorder; i < rightBorder; i++) {
                        cout << i + 1 << ")";
                        this[i].nodePrint();
                    }
                    leftBorder = tempLeftBorder;
                    rightBorder = tempRightBorder;
                } break;
                case 2: {
                    if (rightBorder < size) {
                        leftBorder = rightBorder;
                        rightBorder += 20;
                        cout << "Next workers will be in numeric range of (" << leftBorder << ", " << rightBorder << ")\n\n";
                    } else {
                        cout << "Final range achived.";
                    }
                } break;
                case 4: {
                    //here be sort
                } break;
                default: {
                    cout << "\n\nError: unsupported argument entered\n\n"; 
                } break;
            }
        }
    }


protected:
    void nodePrint() {
        cout << "\t" << this->employeeFullName;
        cout << "\t\t" << this->departmentNumber;
        cout << "\t\t" << this->post;
        cout << "\t" << this->birthDate << endl << endl;
    }

    void nodeFill(ifstream& opendFileStream) {
        short int* temp = new short int[sizeof(short int)];
        opendFileStream.read(this->employeeFullName, sizeof(this->employeeFullName));
        for (int j = 0; j < sizeof(this->departmentNumber); j++) { temp[j] = opendFileStream.get(); }
        this->departmentNumber = temp[0] + temp[1];
        opendFileStream.read(this->post, sizeof(this->post));
        opendFileStream.read(this->birthDate, sizeof(this->birthDate));
    }

};

int main() {
    ifstream dbFile("testBase2.dat", ios::in | ios::binary);
    dbFile.ignore(numeric_limits<streamsize>::max());
    streamsize length = dbFile.gcount();
    dbFile.clear();
    dbFile.seekg(0, ios_base::beg);
    int nodesNumber = length / sizeof(DatabaseNode);
    DatabaseNode* nodes = new DatabaseNode[nodesNumber];

    if (!dbFile) {
        dbFile.clear();
        cout << "Error: database file was not found";
        return 23;
    }

    nodes->ShowMenu(dbFile, nodesNumber);

    dbFile.clear();
    delete[] nodes;
}

/*
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "Program.h";

using namespace std;

struct Employee
{
    char FIO[30];
    short int departmentNumber;
    char place[22];
    char birthDate[10];
};

int main() {
    ifstream databaseFile("testBasew2.dat", ios::in | ios::binary);

    databaseFile.ignore(numeric_limits<streamsize>::max());
    streamsize length = databaseFile.gcount();
    databaseFile.clear();
    databaseFile.seekg(0, ios_base::beg);

    if (!databaseFile) {
        cerr << "databaseFile not found";
        databaseFile.clear();
        return 404;
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

    for (int i = 0; i < numOfEmployees; i++) {
        cout << "Employee " << i + 1 << ":" << endl;
        cout << "\t" << "Full name: " << employee[i].FIO << endl;
        cout << "\t" << "Deparment number: " << employee[i].departmentNumber << endl;
        cout << "\t" << "Place: " << employee[i].place << endl;
        cout << "\t" << "Birthdate: " << employee[i].birthDate << endl;
        cout << endl;
    }

    databaseFile.clear();
    delete[] employee;
}
*/