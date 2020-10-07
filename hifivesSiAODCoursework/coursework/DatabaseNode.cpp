#include <iostream>
#include <fstream>
#include "DatabaseNode.h"
using namespace std;

void DatabaseNode::nodePrint() {
    cout << "\t" << this->employeeFullName;
    cout << "\t\t" << this->departmentNumber;
    cout << "\t\t" << this->post;
    cout << "\t" << this->birthDate << "\n\n";
}

void DatabaseNode::nodeFill(ifstream& opendFileStream) {
    opendFileStream.read(this->employeeFullName, sizeof(this->employeeFullName));
    this->departmentNumber = (short int)opendFileStream.get() + (short int)opendFileStream.get();
    opendFileStream.read(this->post, sizeof(this->post));
    opendFileStream.read(this->birthDate, sizeof(this->birthDate));
}
