#pragma once
#include "Employee.h"

int BinarySearch(Employee** employersI, short int key, size_t size) {
	int L = 0, R = size - 1, m = 0;
	while (L < R)
	{
		m = (L + R) / 2;
		if (employersI[m]->departmentNumber < key) L = m + 1;
		else R = m;
	}
	if (employersI[R]->departmentNumber == key) return R;
	else return -1;
}