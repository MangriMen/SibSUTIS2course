#pragma once
#include "Employee.h"

class TreeVertex
{
public:
	TreeVertex* right;
	TreeVertex* left;
	int balance;
	Employee data;
};