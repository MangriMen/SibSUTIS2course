#pragma once
#include <iostream>

template<typename T>
class List
{
public:
	List();
	~List();

	//�������� ������� �������� � ������
	void pop_front();

	//���������� �������� � ����� ������
	void push_back(T data);

	// �������� ������
	void clear();

	// �������� ���������� ��������� � ������
	int size() { return length; }

	// ������������� �������� [] 
	T& operator[](const int index);

	//���������� �������� � ������ ������
	void push_front(T data);

	//���������� �������� � ������ �� ���������� �������
	void insert(T data, int index);

	//�������� �������� � ������ �� ���������� �������
	void remove_at(int index);

	//�������� ���������� �������� � ������
	void pop_back();
private:
	class Node
	{
	public:
		T data;
		Node* next;
		Node* prev;

		Node(T data = T(), Node* next = nullptr, Node* prev = nullptr)
		{
			this->data = data;
			this->next = next;
			this->prev = prev;
		}

	};

	size_t length;
	Node* head;
	Node* tail;
};
