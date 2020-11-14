#pragma once
#include "Queue.h"

void Split(stack* head, stack** a, stack** b, int& n);

void Merge(stack** head_a, int q, stack** head_b, int r, stack** c_tail, int type);

void MergeSort(stack** head, int n, int type);

void StackToQueue(stack** head, stack** tail);