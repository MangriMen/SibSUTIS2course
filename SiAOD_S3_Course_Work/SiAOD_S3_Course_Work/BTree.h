#pragma once
#include "Employee.h"

class BTree
{
public:
	int HR = true;
	int VR = true;
	class Vertex
	{
	public:
		Vertex* right;
		Vertex* left;
		int balance;
		Employee data;
	};
	Vertex* root = nullptr;

	void Print(Vertex* p) {
		if (p != nullptr) {
			Print(p->left);
			p->data.Print();
			cout << endl;
			Print(p->right);
		}
	}

	void From(Employee** A, int size) {
		for (int i = 0; i < size; i++) {
			Add(root, *A[i]);
		}
	}

	int treeSize(Vertex* p) {
		if (p == nullptr) {
			return 0;
		}
		return 1 + treeSize(p->left) + treeSize(p->right);
	}

	int treeHeight(Vertex* p) {
		if (p == nullptr) {
			return 0;
		}
		return 1 + max(treeHeight(p->left), treeHeight(p->right));
	}

	int averagePathLenght(Vertex* p, int level) {
		if (p == nullptr) {
			return 0;
		}
		return level + averagePathLenght(p->left, level + 1) + averagePathLenght(p->right, level + 1);
	}

	void Info() {
		cout << "Tree size: " << treeSize(root);
		cout << endl << endl << "Tree height: " << treeHeight(root);
		cout << endl << endl << "Tree average height: " << averagePathLenght(root, 1) / (double)(treeSize(root));
		cout << endl << endl;
	}

	bool compareEmployee(Employee first, Employee second) {
		if (first.departmentNumber < second.departmentNumber) {
			return true;
		}
		else if (first.departmentNumber > second.departmentNumber) {
			return false;
		}
		else {
			return (string)first.FIO < (string)second.FIO;
		}
	}

	void Add(Vertex*& p, Employee D)
	{
		if (p == nullptr)
		{
			p = new Vertex;
			p->data = D;
			p->left = nullptr;
			p->right = nullptr;
			p->balance = 0;
			VR = 1;
		}
		else if (!compareEmployee(p->data, D))
		{
			Add(p->left, D);
			if (VR == 1)
			{
				if (p->balance == 0)
				{
					Vertex* q = p->left;
					p->left = q->right;
					q->right = p;
					p = q;
					q->balance = 1;
					VR = 0;
					HR = 1;
				}
				else
				{
					p->balance = 0;
					VR = 1;
					HR = 0;
				}
			}
			else HR = 0;
		}
		else if (compareEmployee(p->data, D))
		{
			Add(p->right, D);
			if (VR == 1)
			{
				p->balance = 1;
				HR = 1;
				VR = 0;
			}
			else if (HR == 1)
			{
				if (p->balance == 1)
				{
					Vertex* q = p->right;
					p->right = q->left;
					p->balance = 0;
					q->balance = 0;
					q->left = p;
					p = q;
					VR = 1;
					HR = 0;
				}
				else HR = 0;
			}
		}
	}
};

