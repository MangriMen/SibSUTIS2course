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
		if ((string)first.FIO < (string)second.FIO) {
			return true;
		}
		else if ((string)first.FIO > (string)second.FIO) {
			return false;
		}
		else {
			return (string)first.place < (string)second.place;
		}
		break;
	default:
		cerr << "Unavailable type";
		exit(-100);
	}
}

int Employee::intResultCompare(string first, string second) {
	if (second.size() < first.size()) second += string(first.size() - second.size(), ' ');
	if (first < second) return 1;
	else if (first > second) return -1;
	else return 0;
}

int Employee::intResultCompare(int first, int second) {
	if (first < second) return 1;
	else if (first > second) return -1;
	else return 0;
}

int Employee::Compare(Employee employer, string FIO)
{
	return intResultCompare((string)employer.FIO, FIO);
}
