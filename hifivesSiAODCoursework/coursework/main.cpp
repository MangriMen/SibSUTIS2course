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
                    cout << "\t" << "Full name: " << "\t\t\t\t" << "Dep. No: " << "\t"
                        << "Post: " << "\t\t\t" << "Date of birth: " << "\n";
                    for (int i = 0; i < size; i++) {
                        cout << i + 1 << ")";
                        this[i].nodePrint();
                    }
                } break;
                case 1: {
                    cout << "\t" << "Full name: " << "\t\t\t\t" << "Dep. No: " << "\t"
                        << "Post: " << "\t\t\t" << "Date of birth: " << "\n";
                    for (int i = leftBorder; i < rightBorder; i++) {
                        cout << i + 1 << ")";
                        this[i].nodePrint();
                    }
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
        return 1;
    }

    nodes->ShowMenu(dbFile, nodesNumber);

    dbFile.clear();
    delete[] nodes;
}