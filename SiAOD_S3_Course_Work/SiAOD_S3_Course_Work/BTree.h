#pragma once
#include "Employee.h"
#include "TreeVertex.h"
#include "TreeColor.h"

constexpr auto OFFSET_X = 100;
constexpr auto OFFSET_Y = 80;

class BTree
{
public:
	int HR = true;
	int VR = true;
	TreeVertex* root = nullptr;
	vector<TreeVertex*> vertices;

	void Print(TreeVertex* p) {
		if (p != nullptr) {
			Print(p->left);
			p->data.Print();
			cout << endl;
			Print(p->right);
		}
	}

	void verts(TreeVertex* p) {
		if (p != nullptr) {
			verts(p->left);
			vertices.push_back(p);
			verts(p->right);
		}
	}

	void reload(TreeVertex* p, Vector2f pos, int level) {
		if (p != nullptr) {
			if (pos == Vector2f(0, 0)) {
				p->circle.setFillColor(TreeColor::Root);
			}
			level--;
			p->circle.setPosition(pos);
			p->Left[0].position = pos;
			p->Right[0].position = pos;

			if (p->left != nullptr) {
				p->Left[1].position = pos + Vector2f(-OFFSET_X * level, OFFSET_Y);
			}
			else {
				p->Left[1].position = p->Left[0].position;
			}

			if (p->right != nullptr) {
				if (p->balance == 1) {
					p->Right[1].position = pos + Vector2f(OFFSET_X * (level + 1), 0);
				}
				else {
					p->Right[1].position = pos + Vector2f(OFFSET_X * level, OFFSET_Y);
				}
			}
			else {
				p->Right[1].position = p->Right[0].position;
			}

			p->Left[0].color = TreeColor::Left;
			p->Right[0].color = TreeColor::Right;

			p->Left[1].color = TreeColor::Left;
			p->Right[1].color = TreeColor::Right;

			reload(p->left, p->circle.getPosition() + Vector2f(-OFFSET_X * level, OFFSET_Y), level);
			if (p->balance == 1) {
				reload(p->right, p->circle.getPosition() + Vector2f(OFFSET_X * ++level, 0), level);
			}
			else {
				reload(p->right, p->circle.getPosition() + Vector2f(OFFSET_X * level, OFFSET_Y), level);
			}
		}
	}

	void From(Employee** A, int size) {
		for (int i = 0; i < size; i++) {
			Add(root, *A[i], "Root", Vector2f(0, 0));
		}
	}

	void From(queue &A) {
		root = nullptr;
		stack* current = A.head;
		while (current != nullptr) {
			Add(root, current->data, "Root", Vector2f(0, 0));
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

	void Add(TreeVertex*& p, Employee D, string sibling, Vector2f parent)
	{
		if (p == nullptr)
		{
			p = new TreeVertex;
			p->data = D;
			p->left = nullptr;
			p->right = nullptr;
			p->balance = 0;

			p->Left.setPrimitiveType(Lines);
			p->Left.resize(2);

			p->Right.setPrimitiveType(Lines);
			p->Right.resize(2);

			p->circle.setRadius(20);
			p->circle.setOutlineColor(Color::Black);
			p->circle.setOutlineThickness(-4);
			p->circle.setOrigin(p->circle.getLocalBounds().width / 2, p->circle.getLocalBounds().height / 2);

			if (sibling == "R") {
				p->circle.setPosition(parent + Vector2f(OFFSET_X, OFFSET_Y));
			}
			else if (sibling == "L") {
				p->circle.setPosition(parent + Vector2f(-OFFSET_X, OFFSET_Y));
			}

			VR = 1;
		}
		else if (!Employee::Compare(p->data, D))
		{
			Add(p->left, D, "L", p->circle.getPosition());
			if (VR == 1)
			{
				if (p->balance == 0)
				{
					p->left->circle.setPosition(p->circle.getPosition());
					p->circle.setPosition(p->left->circle.getPosition() + Vector2f(OFFSET_X, 0));
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
					p->right->circle.setPosition(p->circle.getPosition() + Vector2f(OFFSET_X, OFFSET_Y));
					p->left->circle.setPosition(p->circle.getPosition() + Vector2f(-OFFSET_X, OFFSET_Y));
					p->balance = 0;
					VR = 1;
					HR = 0;
				}
			}
			else HR = 0;
		}
		else if (Employee::Compare(p->data, D))
		{
			Add(p->right, D, "R", p->circle.getPosition());
			if (VR == 1)
			{
				p->balance = 1;
				HR = 1;
				VR = 0;
				p->right->circle.setPosition(p->circle.getPosition() + Vector2f(OFFSET_X, 0));
			}
			else if (HR == 1)
			{
				if (p->balance == 1)
				{
					p->right->circle.setPosition(p->circle.getPosition());
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

