#include "List.h"

template<typename T>
List<T>::List() {
	length = 0;
	head = nullptr;
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
	} else {
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
	} else {
		Node* previous = this->head;

		for (int i = 0; i < index - 1; i++) {
			previous = previous->next;
		}

		Node* newNode = new Node(data, previous->next);

		previous->next = newNode;

		length++;
	}
}

template<typename T>
void List<T>::remove_at(int index) {
	if (index == 0)	{
		pop_front();
	} else {
		Node* previous = this->head;
		for (int i = 0; i < index - 1; i++) {
			previous = previous->next;
		}

		Node* toDelete = previous->next;

		previous->next = toDelete->next;

		delete toDelete;

		length--;
	}

}

template<typename T>
void List<T>::pop_back() {
	remove_at(length - 1);
}
