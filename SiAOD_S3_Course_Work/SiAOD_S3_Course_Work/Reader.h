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
		numOfEmployers = 12;

		employersI = new Employee * [numOfEmployers];

		for (int i = 0; i < numOfEmployers; i++) {
			addToQueue(employersQ, Employee(databaseFile));
		}

		databaseFile.close();

		MergeSort(&employersQ.head, numOfEmployers);

		fillIndexArray(employersQ, employersI);

		numOfFounded = queueFromKey(employersI, employersFoundedQ, 0, numOfEmployers);

		btree.From(employersQ);

		employersFoundedI = new Employee * [numOfFounded];

		fillIndexArray(employersFoundedQ, employersFoundedI);
	}
};