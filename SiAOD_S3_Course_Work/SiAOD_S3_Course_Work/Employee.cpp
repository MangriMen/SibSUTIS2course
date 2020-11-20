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

bool Employee::Compare(Employee first, Employee second, Sort type)
{
	switch (type)
	{
	case Sort::Merge:
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
	case Sort::Tree:
		if ((string)first.place < (string)second.place) {
			return true;
		}
		else if ((string)first.place > (string)second.place) {
			return false;
		}
		else {
			return (string)first.FIO < (string)second.FIO;
		}
		break;
	default:
		break;
	}
}

bool Employee::Compare(Employee first, string value, Sort type)
{
	switch (type)
	{
	case Sort::FIO:
		return (string)first.FIO < value;
	case Sort::departmentNumber:
		return first.departmentNumber < atoi(value.c_str());
	case Sort::place:
		return (string)first.place < value;
	case Sort::birthDate:
		return (string)first.birthDate < value;
	default:
		break;
	}
}
