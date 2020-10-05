#include <iostream>
#include <fstream>
#include "DatabaseNode.h"
#include "List.h"
using namespace std;

void ShowMenu() {

}

int main() {
    ifstream dbFile("testBase2.dat", ios::in | ios::binary);

    dbFile.ignore(numeric_limits<streamsize>::max());
    streamsize length = dbFile.gcount();
    dbFile.clear();
    dbFile.seekg(0, ios_base::beg);
    int nodesNumber = length / sizeof(DatabaseNode);
    
    List<DatabaseNode> nodes;
    for (int i = 0; i < nodesNumber; ++i) {
        DatabaseNode tempNode;
        tempNode.nodeFill(dbFile);
        nodes.push_back(tempNode);
    }

    if (!dbFile) {
        dbFile.clear();
        cout << "Error: database file was not found";
        return 1;
    }

    ShowMenu(dbFile, nodesNumber);

    dbFile.clear();
    nodes.clear();
}