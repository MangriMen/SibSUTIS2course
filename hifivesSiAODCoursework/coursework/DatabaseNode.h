#pragma once
#include <iostream>
#include <fstream>
using namespace std;

class DatabaseNode
{
public:
    char employeeFullName[30];
    short int departmentNumber;
    char post[22];
    char birthDate[10];

    void nodeFill(ifstream& opendFileStream);

    void nodePrint();
};

