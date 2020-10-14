#include <iostream>
#include <fstream>
#include <conio.h>
#include "DatabaseNode.h"
using namespace std;

struct stack {
    stack* next;
    DatabaseNode data;
};

struct queue {
    stack* head;
    stack* tail;
};

void addToStack(stack** head, DatabaseNode node) {
    stack* temp = new stack;
    temp->data = node;
    temp->next = *head;
    *head = temp;
}

void printStackData(stack** head, int leftBorder, int rightborder) {
    stack* current = *head;
    for (int i = leftBorder; i < rightborder; ++i) {
        cout << "#" << i + 1;
        current->data.nodePrint();
        current = current->next;
    }
}

void DigitalSort(stack*& S, bool chooseDigit = 0, int q_length = 64) {
    int KDI[64];
    for (int i = 63; i >= 0; --i) { KDI[i] = i; }

    int j, i, d, k;
    stack* p;
    queue Q[256];
    for (j = q_length - 1; j >= 0; j--) {
        for (i = 0; i < 256; i++)
            Q[i].tail = (stack*)&Q[i].head;
        p = S;
        k = KDI[j];
        while (p) {
            d = (chooseDigit ? p->data.nameDigit[k] : p->data.depDigit[k]);
            Q[d].tail->next = p;
            Q[d].tail = p;
            p = p->next;
        }
        p = (stack*)&S;
        for (i = 0; i < 256; i++) {
            if (Q[i].tail != (stack*)&Q[i].head) {
                p->next = Q[i].head;
                p = Q[i].tail;
            }
        }
        p->next = NULL;
    }
}

void fillIndexArray(stack** head, DatabaseNode** node) {
    stack* current = *head;
    int i = 0;
    while (current != nullptr) {
        node[i] = &current->data;
        current = current->next;
        i++;
    }
}

int BinarySearch(DatabaseNode** nodeArr, int key, size_t size) {
    int L = 1, m = 0;
    int R = size - 1;
    while (L <= R) {
        m = (L + R) / 2;
        if (nodeArr[m]->departmentNumber < key) {
            L = m + 1;
        }
        else {
            R = m - 1;
        }
    }
    if (nodeArr[m]->departmentNumber == key) { return m; }
}

void ShowMenu(ifstream& opendFileStream, stack** operatingStack, size_t size) {
    int chooseNumber = 0;
    int leftBorder = 0;
    int rightBorder = 20;
    stack** tempStack;
    cout << "A Company Database." << endl << endl;

    while (true) {
        cout << "Enter the appropriate number:" << endl
             << "\"1\" to print information about next 20 workers" << endl
             << "\"2\" to print information about previous 20 workers" << endl
             << "\"3\" to print information about all personal" << endl
             << "\"4\" to search specific employees by their department" << endl
             << "\"0\" to exit the program" << endl;
        char chooseNumber = _getch();
        system("CLS");

        switch (chooseNumber) {
        case '0': return;
        case '1':
            if (rightBorder < size) {
                leftBorder = rightBorder;
                rightBorder += 20;
                cout << "\t" << "Full name: " << "\t\t" << "Dep. No: " << "\t"
                    << "Post: " << "\t\t\t" << "Date of birth: " << "\n";
                printStackData(operatingStack, leftBorder, rightBorder);
            }
            else {
                cout << "Final range achived" << endl;
            }
            break;
        case '2':
            if (leftBorder > 0) {
                rightBorder = leftBorder;
                leftBorder -= 20;
                cout << "\t" << "Full name: " << "\t\t" << "Dep. No: " << "\t"
                    << "Post: " << "\t\t\t" << "Date of birth: " << "\n";
                printStackData(operatingStack, leftBorder, rightBorder);
            }
            else {
                cout << "Starting elements achived" << endl;
            }
            break;
        case '3':
            cout << "\t" << "Full name: " << "\t\t" << "Dep. No: " << "\t"
                << "Post: " << "\t\t\t" << "Date of birth: " << "\n";
            printStackData(operatingStack, 0, size);
            break;
        case '4':
            int choosenDepNumber;
            cout << "\t Enter the department number: ";
            cin >> choosenDepNumber;
        default:
            cout << "\n\nError: unsupported argument entered\n\n";
            break;
        }
    }
}

int main() {
    ifstream dbFile("testBase2.dat", ios::in | ios::binary);
    if (!dbFile) {
        dbFile.clear();
        cout << "Error: database file was not found";
        return 1;
    }

    dbFile.ignore(numeric_limits<streamsize>::max());
    streamsize length = dbFile.gcount();
    dbFile.clear();
    dbFile.seekg(0, ios_base::beg);
    int nodesNumber = length / sizeof(DatabaseNode);
    stack* nodes = nullptr;
    stack* nodesReference = nullptr;

    for (int i = 0; i < nodesNumber; ++i) {
        DatabaseNode tempNode;
        tempNode.nodeFill(dbFile);
        addToStack(&nodes, tempNode);
        addToStack(&nodesReference, tempNode);
    }
    
    cout << "Choose a structure to work with:" << endl << endl
        << "\"1\" for default list" << endl
        << "\"2\" for a sorted list" << endl
        << "\"0\" for exiting program." << endl;
    char choosenStruct = _getch();
    system("CLS");
    
    switch (choosenStruct) {
    case '1':
        ShowMenu(dbFile, &nodesReference, nodesNumber);
        break;
    case '2':
        DigitalSort(nodes, 0); // 1 for another sort
        ShowMenu(dbFile, &nodes, nodesNumber);
        break;
    case '0':
    default:
        break;
    }

    dbFile.close();
}