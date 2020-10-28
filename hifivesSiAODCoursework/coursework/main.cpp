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
    int i = 0;
    while (current != nullptr && i < leftBorder) {
        current = current->next;
        ++i;
    }
    while (current != nullptr && i < rightborder) {
        current->data.nodePrint();
        current = current->next;
        ++i;
    }
}

void printArrayData(DatabaseNode** nodeArray, int leftBorder, int rightBorder) {
    for (int i = leftBorder; i < rightBorder; ++i) {
        cout << "#" << i + 1;
        nodeArray[i]->nodePrint();
    }
}

void DigitalSort(stack*& S, int q_length = 32) {
    int* KDI = new int[q_length];
    for (int i = 0; i < q_length; ++i) { KDI[i] = i; }
    
    int j = 0, i = 0, d = 0, k = 0;
    stack* p;
    queue Q[256];
    for (j = q_length - 1; j >= 0; j--) {
        for (i = 0; i < 256; i++)
            Q[i].tail = (stack*)&Q[i].head;
        p = S;
        k = KDI[j];
        while (p) {
            d = p->data.depDigit[k];
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
    delete[] KDI;
}

short int BinarySearch(DatabaseNode** nodeArr, int key, size_t size) {
    int L = 1, m = 0;
    int R = size - 1;

    while (L <= R) {
        m = (L + R) / 2;
        if (nodeArr[m]->departmentNumber < key) {
            L = m + 1;
        } else {
            R = m - 1;
        }
    }
    if (nodeArr[m]->departmentNumber == key)
        return m;
    else if (nodeArr[m + 1]->departmentNumber == key)
        return m + 1;
    return 0;
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

void ShowMenu(ifstream& opendFileStream, stack** operatingStack, size_t size) {
    int chooseNumber = 0;
    int leftBorder = 0;
    int rightBorder = 20;
    int tempLeftBorder = 0;
    int tempRightBorder = 0;
    bool first20 = true;
    bool isSearchActive = false;
    DatabaseNode** nodesArray = new DatabaseNode*[size];
    fillIndexArray(operatingStack, nodesArray);

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
        case '0':
            delete[] nodesArray;
            return;
        case '1':
            if (rightBorder < size) {
                cout << "\t" << "Full name: " << "\t\t" << "Dep. No: " << "\t"
                    << "Post: " << "\t\t\t" << "Date of birth: " << "\n";

                if (first20) {
                    printArrayData(nodesArray, 0, 20);
                    first20 = false;
                } else {
                    leftBorder = rightBorder;
                    rightBorder += 20;
                    printArrayData(nodesArray, leftBorder, rightBorder);
                }
            } else {
                cout << "Final range achived" << endl;
            }
            break;
        case '2':
            if (leftBorder > 0) {
                cout << "\t" << "Full name: " << "\t\t" << "Dep. No: " << "\t"
                    << "Post: " << "\t\t\t" << "Date of birth: " << "\n";
                
                rightBorder = leftBorder;
                leftBorder -= 20;
                printArrayData(nodesArray, leftBorder, rightBorder);
                
                if (leftBorder == 0) {
                    first20 = true;
                }
            } else {
                cout << "Starting elements achived" << endl;
            }
            break;
        case '3':
            cout << "\t" << "Full name: " << "\t\t" << "Dep. No: " << "\t"
                << "Post: " << "\t\t\t" << "Date of birth: " << "\n";
            printArrayData(nodesArray, 0, size);
            break;
        case '4':
            int choosenDepNumber;
            int searchResult;
            isSearchActive = !isSearchActive;
            if (!isSearchActive) {
                cout << "Exiting search mode.\n";
                leftBorder = tempLeftBorder;
                rightBorder = tempRightBorder;
                break;
            }
            tempLeftBorder = leftBorder;
            tempRightBorder = rightBorder;

            cout << "\t Enter the department number: ";
            cin >> choosenDepNumber;
            searchResult = BinarySearch(nodesArray, choosenDepNumber, size) - (2 * bool(!choosenDepNumber));
            cout << "Employee with department number of " << choosenDepNumber <<
                    " are loocated at the " << searchResult + 1 << "th position.\n";
            rightBorder = searchResult;
            first20 = false;
            _getch();
            break;
        default:
            cout << "\n\nError: unsupported argument entered\n\n";
            break;
        }
    }
    delete[] nodesArray;
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
        DigitalSort(nodes);
        ShowMenu(dbFile, &nodes, nodesNumber);
        break;
    case '0':
    default:
        break;
    }

    dbFile.close();
}