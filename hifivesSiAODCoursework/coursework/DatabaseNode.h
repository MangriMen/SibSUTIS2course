#pragma once
#include <iostream>
#include <fstream>
using namespace std;

class DatabaseNode
{
public:
    union {
        char employeeFullName[30];
        unsigned char nameDigit[sizeof(employeeFullName)];
    };
    union {
        short int departmentNumber;
        unsigned char depDigit[sizeof(departmentNumber)];
    };
    char post[22];
    char birthDate[10];

    void nodeFill(ifstream& opendFileStream);

    void nodePrint();
};

