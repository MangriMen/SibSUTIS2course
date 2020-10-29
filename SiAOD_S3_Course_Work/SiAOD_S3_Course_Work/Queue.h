#pragma once
#include "Employee.h"
#include "Utils.h"

struct stack {
	stack* next;
	Employee data;
};

struct queue {
	stack* head{};
	stack* tail{};
};

void addToQueue(queue& queue, Employee employee);

void out(queue& queue);

void fillIndexArray(queue queue, Employee** employersI);

int queueFromIndex(queue& queue, Employee** employersI, int index, int n);

int queueFromKey(Employee** employersI, queue& foundedEmployers, int key, int numOfEmployers);

void Delete(stack** head);