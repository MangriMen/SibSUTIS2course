#include <iostream>
#include <fstream>
#include "DatabaseNode.h"
using namespace std;

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