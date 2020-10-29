#pragma once
#include <iostream>
#include <fstream>

using namespace std;

class Employee
{
public:
	char FIO[30];
	short int departmentNumber;
	char place[22];
	char birthDate[10];
	
	Employee();

	Employee(ifstream& databaseFile);

	void GetFromFile(ifstream& databaseFile);

	void Print();

	static bool Compare(Employee first, Employee second);
};

