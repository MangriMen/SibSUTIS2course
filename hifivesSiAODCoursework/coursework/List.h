#pragma once
#include <iostream>

template<typename T>
class List
{
public:
	List();
	~List();

	//удаление первого элемента в списке
	void pop_front();

	//добавление элемента в конец списка
	void push_back(T data);

	// очистить список
	void clear();

	// получить количество елементов в списке
	int size() { return length; }

	// перегруженный оператор [] 
	T& operator[](const int index);

	//добавление элемента в начало списка
	void push_front(T data);

	//добавление элемента в список по указанному индексу
	void insert(T data, int index);

	//удаление элемента в списке по указанному индексу
	void remove_at(int index);

	//удаление последнего элемента в списке
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
