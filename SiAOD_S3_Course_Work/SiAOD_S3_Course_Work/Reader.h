#pragma once
#include <fstream>
#include "Queue.h"
#include "BTree.h"
#include "MergeSort.h"

class Reader {
public:
	size_t numOfEmployers;
	size_t numOfFounded;

	queue employersQ;
	Employee** employersI;

	queue employersFoundedQ;
	Employee** employersFoundedI;

	BTree btree;

	int windowsAreClosed = 0;

	Reader() {
		ifstream databaseFile("testBase2.dat", ios::in | ios::binary);

		if (!databaseFile) {
			cerr << "databaseFile not found";
			databaseFile.clear();
		}

		databaseFile.ignore(numeric_limits<streamsize>::max());
		streamsize length = databaseFile.gcount();
		databaseFile.clear();
		databaseFile.seekg(0, ios_base::beg);

		numOfEmployers = length / sizeof(Employee);

		employersI = new Employee * [numOfEmployers];

		for (int i = 0; i < numOfEmployers; i++) {
			addToQueue(employersQ, Employee(databaseFile));
		}

		databaseFile.close();

		MergeSort(&employersQ.head, static_cast<int>(numOfEmployers), Employee::Sort::Merge);

		fillIndexArray(employersQ, employersI);

		numOfFounded = queueFromKey(employersI, employersFoundedQ, 0, static_cast<int>(numOfEmployers));

		btree.From(employersFoundedQ);

		employersFoundedI = new Employee * [numOfFounded];

		fillIndexArray(employersFoundedQ, employersFoundedI);
	}

	~Reader() {
		windowsAreClosed = 2;
		FreeReader();
	}

	void FreeReader() {
		windowsAreClosed++;
		if (windowsAreClosed < 2) return;
		Delete(&employersQ.head);
		Delete(&employersFoundedQ.head);
		delete[] employersI;
		delete[] employersFoundedI;
	}
};