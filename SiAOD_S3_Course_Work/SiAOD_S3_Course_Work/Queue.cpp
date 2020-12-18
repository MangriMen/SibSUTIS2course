#include "Queue.h"

void addToQueue(queue& queue, Employee employee)
{
	if (queue.head == nullptr) {
		stack* temp = new stack;
		temp->data = employee;
		temp->next = nullptr;
		queue.head = temp;
		queue.tail = temp;
	}
	else {
		stack* temp = new stack;
		temp->data = employee;
		temp->next = nullptr;
		queue.tail->next = temp;
		queue.tail = queue.tail->next;
	}
}

void out(queue& queue)
{
	stack* current = queue.head;
	int i = 1;
	while (current != nullptr) {
		cout << "\t" << i;
		current->data.Print();
		cout << endl;
		current = current->next;
		i++;
	}
}

void fillIndexArray(queue queue, Employee** employersI)
{
	stack* current = queue.head;
	int i = 0;
	while (current != nullptr) {
		employersI[i] = &current->data;
		current = current->next;
		i++;
	}
}

int queueFromIndex(queue& queue, Employee** employersI, int index, int n)
{
	int numOfFounded = 0;
	for (int i = index; i < n - 1; i++) {
		if (employersI[i]->departmentNumber == employersI[i + 1]->departmentNumber) { numOfFounded++; }
		else break;
	}

	for (int i = index; i < (index + numOfFounded); i++) {
		addToQueue(queue, *employersI[i]);
	}
	return numOfFounded;
}

int queueFromKey(Employee** employersI, queue& foundedEmployers, int key, int numOfEmployers)
{
	int firstFounded = BinarySearch(*&employersI, key, numOfEmployers);

	int numOfFounded = 0;
	if (firstFounded != -1) {
		numOfFounded = queueFromIndex(foundedEmployers, employersI, firstFounded, numOfEmployers);
	}

	return numOfFounded;
}

void Delete(stack** head)
{
	if (*head == NULL)
		return;
	stack* p = *head, * temp;
	while (p) {
		temp = p;
		p = p->next;
		delete temp;
	}
	*head = NULL;
}
