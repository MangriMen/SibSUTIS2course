#pragma once
class DatabaseNode
{
public:
    char employeeFullName[30];
    short int departmentNumber;
    char post[22];
    char birthDate[10];

    void ShowMenu(ifstream& opendFileStream, int size = 0);

    void nodeFill(ifstream& opendFileStream);

    void nodePrint();
};

