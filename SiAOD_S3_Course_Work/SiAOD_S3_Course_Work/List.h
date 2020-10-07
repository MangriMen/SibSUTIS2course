#pragma once
#include <iostream>
using namespace std;

template<typename T>
class List
{
public:
	List();
	~List();

	void pop_front();

	void push_back(T data);

	void clear();

	int size() { return length; }

	T& operator[](const int index);

	void push_front(T data);

	void insert(T data, int index);

	void remove_at(int index);

	void pop_back();

	void MergESorT();

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

		void swap(Node &first, Node &second);

		void mergeSort(Node** headRef);
		List<T>::Node* sortedMerge(Node* a, Node* b);
		void frontBackSplit(Node* source, Node** frontRef, Node** backRef);

	};
	size_t length;
	Node* head;
	Node* tail;
};

template<typename T>
List<T>::List() {
	length = 0;
	head = nullptr;
	tail = nullptr;
}

template<typename T>
List<T>::~List() {
	clear();
}

template<typename T>
void List<T>::pop_front() {
	Node* temp = head;

	head = head->next;

	delete temp;

	length--;

}

template<typename T>
void List<T>::push_back(T data) {
	if (head == nullptr) {
		head = new Node(data);
	}
	else {
		Node* current = this->head;

		while (current->next != nullptr) {
			current = current->next;
		}
		current->next = new Node(data, nullptr, current);
	}

	length++;
}

template<typename T>
void List<T>::clear() {
	while (length)
		pop_front();
}

template<typename T>
T& List<T>::operator[](const int index)
{
	int counter = 0;

	Node* current = this->head;

	while (current != nullptr)
	{
		if (counter == index)
		{
			return current->data;
		}
		current = current->next;
		counter++;
	}
}

template<typename T>
void List<T>::push_front(T data) {
	head = new Node(data, head);
	length++;
}

template<typename T>
void List<T>::insert(T data, int index) {
	if (index == 0) {
		push_front(data);
	}
	else {
		Node* previous = this->head;
		Node* next = this->head;

		for (int i = 0; i < index - 1; i++) {
			previous = previous->next;
		}
		next = previous->next->next;

		Node* newNode = new Node(data, previous->next);

		previous->next = newNode;
		next->prev = newNode;

		length++;
	}
}

template<typename T>
void List<T>::remove_at(int index) {
	if (index == 0) {
		pop_front();
	}
	else {
		Node* next = this->head;
		Node* previous = this->head;

		for (int i = 0; i < index - 1; i++) {
			previous = previous->next;
		}
		next = previous->next->next;

		Node* toDelete = previous->next;

		previous->next = toDelete->next;
		next->prev = toDelete->prev;

		delete toDelete;

		length--;
	}

}

template<typename T>
void List<T>::pop_back() {
	remove_at(length - 1);
}

template<typename T>
void List<T>::MergESorT()
{
	mergeSort(&this->head);
}

template<typename T>
List<T>::Node* List<T>::Node::sortedMerge(List<T>::Node* a, List<T>::Node* b)
{
	Node* result = NULL;

	/* Base cases */
	if (a == NULL)
		return (b);
	else if (b == NULL)
		return (a);

	/* Pick either a or b, and recur */
	if (a->data->departmentNumber <= b->data->departmentNumber) {
		result = a;
		result->next = sortedMerge(a->next, b);
	}
	else {
		result = b;
		result->next = sortedMerge(a, b->next);
	}
	return (result);
}

template<typename T>
void List<T>::Node::mergeSort(List<T>::Node** headRef)
{
	Node* head = *headRef;
	Node* a;
	Node* b;

	/* Base case -- length 0 or 1 */
	if ((head == NULL) || (head->next == NULL)) {
		return;
	}

	/* Split head into 'a' and 'b' sublists */
	frontBackSplit(head, &a, &b);

	/* Recursively sort the sublists */
	mergeSort(&a);
	mergeSort(&b);

	/* answer = merge the two sorted lists together */
	*headRef = sortedMerge(a, b);
}

template<typename T>
void List<T>::Node::frontBackSplit(List<T>::Node* source, List<T>::Node** frontRef, List<T>::Node** backRef)
{
	Node* fast;
	Node* slow;
	slow = source;
	fast = source->next;

	/* Advance 'fast' two nodes, and advance 'slow' one node */
	while (fast != NULL) {
		fast = fast->next;
		if (fast != NULL) {
			slow = slow->next;
			fast = fast->next;
		}
	}

	/* 'slow' is before the midpoint in the list, so split it in two
	at that point. */
	*frontRef = source;
	*backRef = slow->next;
	slow->next = NULL;
}

template<typename T>
void List<T>::Node::swap(List<T>::Node& first, List<T>::Node& second)
{
	Node temp = first;

	first->next = second->next;
	first->prev = second->prev;
	
	second->next = temp->next;
	second->prev = temp->prev;
}
