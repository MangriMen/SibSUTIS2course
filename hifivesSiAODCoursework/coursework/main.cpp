#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
#include <algorithm>
#include "DatabaseNode.h"
using namespace std;

bool isIncreased = true;

struct stack {
    stack* next;
    DatabaseNode data;
};

struct queue {
    stack* head;
    stack* tail;
};

struct Vertex {
    DatabaseNode *data = nullptr;
    int balance = 0;
    Vertex* left = nullptr;
    Vertex* right = nullptr;
};

void leftLeftTurn(Vertex*& pointer) {
    Vertex* tempPointer = pointer->left;
    pointer->balance = 0;
    tempPointer->balance = 0;
    pointer->left = tempPointer->right;
    tempPointer->right = pointer;
    pointer = tempPointer;
}

void rightRightTurn(Vertex*& pointer) {
    Vertex* tempPointer = pointer->right;
    pointer->balance = 0;
    tempPointer->balance = 0;
    pointer->right = tempPointer->left;
    tempPointer->left = pointer;
    pointer = tempPointer;
}

void leftRightTurn(Vertex*& pointer) {
    Vertex* tempPointer = pointer->left;
    Vertex* r = tempPointer->right;

    pointer->balance = (r->balance < 0 ? 1 : 0);
    tempPointer->balance = (r->balance > 0 ? -1 : 0);

    r->balance = 0;
    tempPointer->right = r->left;
    pointer->left = r->right;
    r->left = tempPointer;
    r->right = pointer;
    pointer = r;
}

void rightLeftTurn(Vertex*& pointer) {
    Vertex* tempPointer = pointer->right;
    Vertex* r = tempPointer->left;

    pointer->balance = (r->balance > 0 ? -1 : 0);
    tempPointer->balance = (r->balance < 0 ? 1 : 0);

    r->balance = 0;
    pointer->right = r->left;
    tempPointer->left = r->right;
    r->left = pointer;
    r->right = tempPointer;
    pointer = r;
}

bool compare(DatabaseNode* first, DatabaseNode* second) {
    string temp1 = " ";
    string temp2 = " ";
    temp2 = temp2 + first->birthDate[6] + first->birthDate[7] + first->birthDate[3] + first->birthDate[4] + first->birthDate[0] + first->birthDate[1];
    temp1 = temp1 + second->birthDate[6] + second->birthDate[7] + second->birthDate[3] + second->birthDate[4] + second->birthDate[0] + second->birthDate[1];
    if (temp2 < temp1) {
        return  true;
    } else if (temp2 > temp1) {
        return false;
    }
}

void AVLTreeAddNode(Vertex*& pointer, DatabaseNode *data) {
    if (pointer == nullptr) {
        pointer = new Vertex;
        pointer->data = data;
        pointer->balance = 0;
        pointer->left = nullptr;
        pointer->right = nullptr;
        isIncreased = true;
    }
    else if (!compare(pointer->data, data)) {
        AVLTreeAddNode(*&pointer->left, data);
        if (isIncreased) {
            if (pointer->balance > 0) {
                pointer->balance = 0;
                isIncreased = false;
            }
            else if (pointer->balance == 0) {
                pointer->balance = -1;
                isIncreased = true;
            }
            else {
                if (pointer->left->balance < 0) {
                    //cout << endl;
                    leftLeftTurn(pointer);
                    //cout << "LL ";
                    isIncreased = false;
                }
                else {
                    //cout << endl;
                    leftRightTurn(pointer);
                    //cout << "LR ";
                    isIncreased = false;
                }
            }
        }
    } else if (compare(pointer->data, data)) {
        AVLTreeAddNode(*&pointer->right, data);
        if (isIncreased) {
            if (pointer->balance < 0) {
                pointer->balance = 0;
                isIncreased = false;
            } else if (pointer->balance == 0) {
                pointer->balance = 1;
                isIncreased = true;
            } else {
                if (pointer->right->balance > 0) {
                    //cout << endl;
                    rightRightTurn(pointer);
                    //cout << "RR ";
                    isIncreased = false;
                } else {
                    //cout << endl;
                    rightLeftTurn(pointer);
                    //cout << "RL ";
                    isIncreased = false;
                }
            }
        }
    }
}

void traversal(Vertex* pointer, int type) {
    if (pointer != nullptr) {
        if (type == 1) {
            cout << "(" << pointer->balance << ") " << endl;
            pointer->data->nodePrint();
        }
        traversal(pointer->left, type);
        if (type == 2) {
            cout << "(" << pointer->balance << ") " << endl;
            pointer->data->nodePrint();
        }
        traversal(pointer->right, type);
        if (type == 3) {
            cout << "(" << pointer->balance << ") " << endl;
            pointer->data->nodePrint();
        }
    }
}

int height(Vertex* pointer) {
    return pointer != nullptr ? 1 + max(height(pointer->left), height(pointer->right)) : 0;
}

int amount(Vertex* pointer) {
    return pointer != nullptr ? 1 + amount(pointer->left) + amount(pointer->right) : 0;
}

double treeHeightSum(Vertex* pointer, int level) {
    return pointer != nullptr ? level + treeHeightSum(pointer->left, level + 1) + treeHeightSum(pointer->right, level + 1) : 0;
}

void dispalyVertexInfo(Vertex* pointer, int traversalType) {
    cout << endl << "-----------------------------------------------------" << endl << endl;
    if (pointer == nullptr) {
        cout << "Дерево пусто." << endl;
        return;
    }
    cout << "Traversal type: " << traversalType << endl;
    traversal(pointer, traversalType);
    cout << endl << endl << "Number of vertexes: " << amount(pointer) + 1;
    cout << endl << endl << "Tree height: " << height(pointer);
    cout << endl << endl << "Average tree height: " << treeHeightSum(pointer, 1) / amount(pointer) << endl << endl << endl;
}

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

void digitalSort(stack*& S, int q_length = 32) {
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

short int binarySearch(DatabaseNode** nodeArr, int key, size_t size) {
    int L = 1, m = 0;
    int R = (int)size - 1;

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
    int searchResult = 0;
    int searchResultLast = 0;

    Vertex* pointer = nullptr;

    DatabaseNode** nodesArray = new DatabaseNode*[size];
    fillIndexArray(operatingStack, nodesArray);

    cout << "A Company Database." << endl << endl;
    while (true) {
        cout << "Enter the appropriate number:" << endl
             << "\"1\" to print information about next 20 workers" << endl
             << "\"2\" to print information about previous 20 workers" << endl
             << "\"3\" to print information about all personal" << endl
             << "\"4\" to search specific employees by their department" << endl
             << "\"5\" to make a tree of employees (use only after search)" << endl
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
                    if (rightBorder > size) rightBorder = size;

                    printArrayData(nodesArray, leftBorder, rightBorder);
                }
            } else {
                cout << "Final range achived" << endl;
            }
            break;
        case '2':
            if (leftBorder > 0) {
                cout << "\t" << "Full name: " << "\t\t" << "Dep. No: " << "\t"
                    << "Post: " << "\t\t\t" << "Date of birth: " << endl;
                
                rightBorder = leftBorder;
                leftBorder -= 20;
                if (leftBorder < 0) leftBorder = 0;

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
                << "Post: " << "\t\t\t" << "Date of birth: " << endl;
            printArrayData(nodesArray, 0, (int)size);
            break;
        case '4':
            int choosenDepNumber;
            isSearchActive = !isSearchActive;
            if (!isSearchActive) {
                cout << "Exiting search mode." << endl << endl;
                leftBorder = tempLeftBorder;
                rightBorder = tempRightBorder;
                searchResult = 0;
                searchResultLast = 0;
                break;
            }
            tempLeftBorder = leftBorder;
            tempRightBorder = rightBorder;

            cout << "\t Enter the department number (dozens from 0 to 240): ";
            cin >> choosenDepNumber;
            if (!bool(choosenDepNumber % 10) && choosenDepNumber < 250) {
                searchResult = binarySearch(nodesArray, choosenDepNumber, size) + bool(choosenDepNumber);
                searchResultLast = searchResult;
                while (searchResultLast != 4000 && nodesArray[searchResultLast]->departmentNumber == nodesArray[searchResult]->departmentNumber) {
                    ++searchResultLast;
                }
                cout << "Employee with department number of " << choosenDepNumber <<
                    " are located at the " << searchResult << "th position." << endl << endl;
                rightBorder = searchResult;
            } else {
                cout << "Invalid number." << endl << endl;
                isSearchActive = !isSearchActive;
            }

            first20 = false;
            break;
        case '5':
            if (!searchResultLast) {
                cout << "Works only in search mode!\n\n";
                break;
            }

            for (int i = searchResult; i < searchResultLast; ++i) {
                AVLTreeAddNode(pointer, nodesArray[i]);
            }

            dispalyVertexInfo(pointer, 2);

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
        digitalSort(nodes);
        ShowMenu(dbFile, &nodes, nodesNumber);
        break;
    case '0':
    default:
        break;
    }

    dbFile.close();
}