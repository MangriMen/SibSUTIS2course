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

	enum class Sort {
		Null,
		Merge,
		Tree,
		FIO,
		departmentNumber,
		place,
		birthDate
	};
	
	Employee();

	Employee(ifstream& databaseFile);

	void GetFromFile(ifstream& databaseFile);

	void Print();

	static int intResultCompare(string first, string second);
	static int intResultCompare(int first, int second);

	static bool Compare(Employee first, Employee second, Sort type);
	static int Compare(Employee employer, string FIO);
};

