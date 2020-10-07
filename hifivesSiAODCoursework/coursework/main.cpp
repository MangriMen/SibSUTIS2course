#include <iostream>
#include <fstream>
#include "DatabaseNode.h"
#include "List.h"
using namespace std;

void ShowMenu(ifstream& opendFileStream, List<DatabaseNode>& list, size_t size) {
    int chooseNumber = 0;
    int leftBorder = 0;
    int rightBorder = 20;

    cout << "A Company Database.\n\n"
        << "Enter the appropriate number:\n"
        << "\"1\" to print workers information\n"
        << "\"2\" to select the next 20 workers\n"
        << "\"3\" to print information about all employees\n"
        << "\"4\" to search information about specific employees\n"
        << "\"0\" for exiting program.\n";

    while (true) {
        cin >> chooseNumber;
        switch (chooseNumber) {
        case 0: {
            return;
        }
        case 1: {
            cout << "\t" << "Full name: " << "\t\t\t\t" << "Dep. No: " << "\t"
                << "Post: " << "\t\t\t" << "Date of birth: " << "\n";
            for (int i = leftBorder; i < rightBorder; i++) {
                cout << i + 1 << ")";
                list[i].nodePrint();
            }
        } break;
        case 2: {
            if (rightBorder < size) {
                leftBorder = rightBorder;
                rightBorder += 20;
                cout << "Next workers will be in numeric range of (" << leftBorder << ", " << rightBorder << ")\n\n";
            }
            else {
                cout << "Final range achived.";
            }
        } break;
        case 3: {
             cout << "\t" << "Full name: " << "\t\t\t\t" << "Dep. No: " << "\t"
                 << "Post: " << "\t\t\t" << "Date of birth: " << "\n";
             for (int i = 0; i < size; i++) {
                 cout << i + 1 << ")";
                 list[i].nodePrint();
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


//void digitalSort(List<DatabaseNode>& list, size_t size)
//{
//    DatabaseNode mx = list[0];
//    for (int i = 1; i < size; i++)
//        if (list[i]->data->departmentNumber > mx->data->departmentNumber)
//            mx = list[i]->data->departmentNumber;
//
//    for (int exp = 1; mx / exp > 0; exp *= 10) {
//        int* output = new int[size];
//        int i, count[10] = { 0 };
//
//        for (i = 0; i < size; i++)
//            count[(list[i]->data / exp) % 10]++;
//
//        for (i = 1; i < 10; i++)
//            count[i] += count[i - 1];
//
//        for (i = size - 1; i >= 0; i--) {
//            output[count[(list[i]->data / exp) % 10] - 1] = arr[i];
//            count[(arr[i] / exp) % 10]--;
//        }
//
//        for (i = 0; i < size; i++)
//            list[i]->data = output[i];
//    }
//}

function radixSort(int[] A) {
    for (i = 1 to m)
        for j = 0 to k - 1
            C[j] = 0
        for j = 0 to n - 1
            d = digit(A[j], i)
            C[d]++
            count = 0
        for j = 0 to k - 1
            tmp = C[j]
            C[j] = count
            count += tmp
        for j = 0 to n - 1
            d = digit(A[j], i)
            B[C[d]] = A[j]
            C[d]++
            A = B
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
    int choosedStructureNumber;
    List<DatabaseNode> nodesSorted;
    List<DatabaseNode> nodes;
    for (int i = 0; i < nodesNumber; ++i) {
        DatabaseNode tempNode;
        tempNode.nodeFill(dbFile);
        nodes.push_back(tempNode);
    }
    
    cout << "Choose a structure to work with:\n\n"
        << "\"1\" for default list\n"
        << "\"2\" for a sorted list\n"
        << "\"0\" for exiting program.\n";;
    cin >> choosedStructureNumber;
    switch (choosedStructureNumber) {
    case 1: {
        ShowMenu(dbFile, nodes, nodesNumber);
    } break;
    case 2: {
        ShowMenu(dbFile, nodesSorted, nodesNumber);
    } break;
    case 0:
    default:
        break;
    }

    dbFile.close();
    nodes.~List();
}