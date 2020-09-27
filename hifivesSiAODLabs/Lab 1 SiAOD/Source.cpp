#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <cstddef>
using namespace  std;

struct Node {
    Node* next;
    long int data;
};

class List {
public:
    Node* head = nullptr;
    Node* tail = nullptr;
    long int size = 0;

    void addElement(long int tempData) {
        Node* newPointer = new Node;
        newPointer->next = nullptr;
        newPointer->data = tempData;

        if (size != 0) {
            tail->next = newPointer;
            tail = newPointer;
            ++size;
        }
        else {
            ++size;
            head = newPointer;
            tail = newPointer;
        }
    }

    void selectSortList() {
        Node* a, * b;
        Node* pointer1, * pointer2, * r1, * r2;

        for (pointer1 = r1 = head; pointer1 != nullptr; pointer1 = pointer1->next) {
            a = pointer2 = r2 = pointer1;
            for (b = pointer1->next; b != nullptr; b = b->next) {
                if (b->data < pointer2->data) {
                    r2 = a;
                    pointer2 = b;
                }
                a = b;
            }

            if (pointer1 != pointer2) {
                if (pointer1 == head)
                    head = pointer2;
                else
                    r1->next = pointer2;

                b = pointer2->next;
                if (pointer1 == r2) {
                    pointer2->next = pointer1;
                    pointer1->next = b;
                }
                else {
                    a = pointer1->next;
                    r1->next = pointer2;
                    r2->next = pointer1;
                    pointer1->next = b;
                    pointer2->next = a;
                }
                pointer1 = pointer2;
            }
            r1 = pointer1;
        }
    }

    long int checkSumList() {
        long int sum = 0;
        Node* newPointer = head;
        while (newPointer != nullptr) {
            sum += newPointer->data;
            newPointer = newPointer->next;
        }
        return sum;
    }

    long int runNumberList(long int size) {
        long int i = 0, s_counter = 1;
        Node* front_ptr = head;
        Node* newPointer = front_ptr->next;
        while (i < size - 1) {
            if (front_ptr->data > newPointer->data) s_counter++;
            ++i;
            front_ptr = front_ptr->next;
            newPointer = front_ptr->next;
        }
        return s_counter;
    }

    long int sumNumberList(long int size) {
        long int i = 0, s_counter = 1;
        Node* front_ptr = head;
        Node* newPointer = front_ptr->next;
        while (i < size - 1) {
            if (front_ptr->data > newPointer->data) s_counter++;
            ++i;
            front_ptr = front_ptr->next;
            newPointer = front_ptr->next;
        }
        return s_counter;
    }

    void printList() {
        for (Node* newPointer = head; newPointer != nullptr; newPointer = newPointer->next) cout << newPointer->data << " ";
    }

}List;

void fillRandList(int count) {
    for (long int i = 0; i < count; ++i) {
        int data = rand() % 100;
        List.addElement(data);
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned)time(NULL));
    int count;

    cout << "Кол-во элементов: ";
    cin >> count;
    fillRandList(count);
    
    cout << "Изначальный лист:\n";
    List.printList();
    cout << "\nСумма элементов листа:\n" << List.checkSumList();
    cout << "\nКол-во серий:\n" << List.runNumberList(count) << endl;

    cout << "Лист после сортировки:\n";
    List.selectSortList();
    List.printList();
    cout << "\nСумма элементов листа:\n" << List.checkSumList();
    cout << "\nКол-во серий:\n" << List.runNumberList(count) << endl;

    return 0;
}
