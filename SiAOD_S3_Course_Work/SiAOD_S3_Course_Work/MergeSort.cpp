#include "MergeSort.h"

void MergeSort(stack** head, int n, int type) {
	stack* a = NULL, * b = NULL, * head_p = *head;
	int p, i, m, q, r;
	Split(head_p, &a, &b, n);
	queue c[2];
	p = 1;
	c[0].tail = (stack*)&c[0].head;
	while (p < n) {
		c[0].tail = (stack*)&c[0].head;
		c[1].tail = (stack*)&c[1].head;
		i = 0, m = n;
		while (m > 0) {
			if (m >= p)
				q = p;
			else
				q = m;
			m -= q;
			if (m >= p)
				r = p;
			else
				r = m;
			m -= r;
			Merge(&a, q, &b, r, &c[i].tail, type);
			i = 1 - i;
		}
		a = c[0].head, b = c[1].head;
		p *= 2;
	}
	c[0].tail->next = NULL;
	*head = c[0].head;
}

void StackToQueue(stack** head, stack** tail)
{
	(*tail)->next = *head;
	*tail = *head;
	*head = (*head)->next;
	(*tail)->next = NULL;
}

void Split(stack* head, stack** a, stack** b, int& n) {
	stack* k, * p;
	*a = head, * b = head->next;
	n = 1;
	k = *a;
	p = *b;
	while (p) {
		++n;
		k->next = p->next;
		k = p;
		p = p->next;
	}
}


void Merge(stack** head_a, int q, stack** head_b, int r, stack** c_tail, int type) {
	while (q != 0 && r != 0) {
		if (Employee::Compare((*head_a)->data, (*head_b)->data, type)) {
			StackToQueue(head_a, c_tail);
			q--;
		}
		else {
			StackToQueue(head_b, c_tail);
			r--;
		}
	}
	while (q > 0) {
		StackToQueue(head_a, c_tail);
		q--;
	}
	while (r > 0) {
		StackToQueue(head_b, c_tail);
		r--;
	}
}