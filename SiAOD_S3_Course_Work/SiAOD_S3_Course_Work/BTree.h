#pragma once
#include "Employee.h"
#include "TreeVertex.h"

class BTree
{
public:
	int HR = true;
	int VR = true;
	TreeVertex* root = nullptr;

	void Print(TreeVertex* p) {
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

	void From(queue &A) {
		root = nullptr;
		stack* current = A.head;
		while (current != nullptr) {
			Add(root, current->data);
			current = current->next;
		}
	}

	static int treeSize(TreeVertex* p) {
		if (p == nullptr) {
			return 0;
		}
		return 1 + treeSize(p->left) + treeSize(p->right);
	}

	static int treeHeight(TreeVertex* p) {
		if (p == nullptr) {
			return 0;
		}
		return 1 + max(treeHeight(p->left), treeHeight(p->right));
	}

	static int averagePathLenght(TreeVertex* p, int level) {
		if (p == nullptr) {
			return 0;
		}
		return level + averagePathLenght(p->left, level + 1) + averagePathLenght(p->right, level + 1);
	}

	void Info() {
		cout << endl;
		cout << "Tree size: " << treeSize(root);
		cout << endl << endl << "Tree height: " << treeHeight(root);
		cout << endl << endl << "Tree average height: " << averagePathLenght(root, 1) / (double)(treeSize(root));
		cout << endl << endl;
	}

	void Add(TreeVertex*& p, Employee D)
	{
		if (p == nullptr)
		{
			p = new TreeVertex;
			p->data = D;
			p->left = nullptr;
			p->right = nullptr;
			p->balance = 0;
			VR = 1;
		}
		else if (!Employee::Compare(p->data, D))
		{
			Add(p->left, D);
			if (VR == 1)
			{
				if (p->balance == 0)
				{
					TreeVertex* q = p->left;
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
		else if (Employee::Compare(p->data, D))
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
					TreeVertex* q = p->right;
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

