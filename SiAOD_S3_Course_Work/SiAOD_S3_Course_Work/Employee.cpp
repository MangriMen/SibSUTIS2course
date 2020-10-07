#include <iostream>
#include <fstream>
#include "Employee.h"

using namespace std;

void Employee::getEmployeeFromFile(ifstream& databaseFile)
{
	databaseFile.read(this->FIO, sizeof(this->FIO));
	this->departmentNumber = (short int)databaseFile.get() + (short int)databaseFile.get();
	databaseFile.read(this->place, sizeof(this->place));
	databaseFile.read(this->birthDate, sizeof(this->birthDate));
}
