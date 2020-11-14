#include <iostream>
#include <fstream>
#include "Employee.h"

using namespace std;

Employee::Employee() : FIO(" "), departmentNumber(0), place(" "), birthDate(" ")
{
}

Employee::Employee(ifstream& databaseFile)
{
	GetFromFile(databaseFile);
}

void Employee::GetFromFile(ifstream& databaseFile)
{
	databaseFile.read(this->FIO, sizeof(this->FIO));
	this->departmentNumber = (short int)databaseFile.get() + (short int)databaseFile.get();
	databaseFile.read(this->place, sizeof(this->place));
	databaseFile.read(this->birthDate, sizeof(this->birthDate));
}

void Employee::Print()
{
	cout << "\t" << FIO
		<< "\t" << departmentNumber
		<< "\t" << place
		<< "\t" << birthDate;
}

bool Employee::Compare(Employee first, Employee second, int type)
{
	switch (type)
	{
	case 1:
		if (first.departmentNumber < second.departmentNumber) {
			return true;
		}
		else if (first.departmentNumber > second.departmentNumber) {
			return false;
		}
		else {
			return (string)first.FIO < (string)second.FIO;
		}
		break;
	case 2:
		return (string)first.place < (string)second.place;
		break;
	default:
		break;
	}
}
