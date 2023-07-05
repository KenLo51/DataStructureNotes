#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <iostream>

template <class T>
struct ListNode{
    struct ListNode* prev;
    T data;
    struct ListNode* next;

    struct ListNode() : prev(NULL), next(NULL) {};
    struct ListNode(T data) : prev(NULL), next(NULL), data(data) {};
    struct ListNode(struct ListNode* prev, struct ListNode* next, T data) : prev(prev), next(next), data(data) {};
};

template <class T>
class List {
// defined variables
protected:
    ListNode<T>* rear, * front; // circular list implementation
    unsigned int size;

// defined functions
private:
public:
    // constructor
    List(){
        front = rear = new ListNode<T>;
        size = 0;
    };
    List(T* beg, T* end) {
        front = rear = new ListNode<T>;
        size = 0;
        for (beg; beg < end; beg++) {
            ListNode<T>* newNode = new ListNode<T>(rear, NULL, *beg);
            rear->next = newNode;
            rear = newNode;
            size++;
        }
    };
    List(List<T>& src) {
        front = rear = new ListNode<T>;
        size = 0;
        if (this == &src) return;

        ListNode<T>* nodePtr = src.Data();
        while (nodePtr != NULL) {
            this->Push(nodePtr->data, -1);
            nodePtr = nodePtr->next;
        }
    }

    // destructor
    ~List() {
        while (front != NULL) {
            ListNode<T>* p = front;
            front = front->next;
            delete p;
        }
    };

    // get the current size of list
    unsigned int Size() {
        return size;
    }

    // check if empty
    bool isEmpty() {
        return size == 0;
    }

    // get node
    ListNode<T>* Data(){
        if (front == NULL) return NULL;
        return front->next;
    }

    // read from list, negedge index can read from the end of list
    T Get(int index, ListNode<T>** returnPtr = NULL) {
        // check if valid operation
        if (size == 0) {
            std::cerr << "The List is empty\n";
            throw;
        }

        ListNode<T> *tempPtr = NULL;
        if (returnPtr == NULL) {
            returnPtr = &tempPtr;
        }

        // adject the index thad make sure it would not out of range [0, size)
        if (size == 1) index = 0;
        if (index > 0) index = index % size;
        if (index < 0) index = size - ((-index) % size);

        // decide the travel direction
        if (index < (size / 2)) {
            // begin travel from front
            (*returnPtr) = front->next;
            for (unsigned int i = 0; i < index; i++)
                (*returnPtr) = (*returnPtr)->next;
            return (*returnPtr)->data;
        }
        else {
            // begin travel from rear
            index = size - index - 1;
            (*returnPtr) = rear;
            for (unsigned int i = 0; i < index; i++)
                (*returnPtr) = (*returnPtr)->prev;
            return (*returnPtr)->data;
        }

        // never reach there
        ListNode<T> unknowData;
        return (*returnPtr)->data;
    };

    // read and delete
    T Pop(int index = -1) {
        // read data
        ListNode<T>* dataPtr;
        Get(index, &dataPtr);

        // adject index
        if (dataPtr == rear) rear = dataPtr->prev;
        dataPtr->prev->next = dataPtr->next;
        if (dataPtr->next != NULL) dataPtr->next->prev = dataPtr->prev;

        // delete node
        T dataTemp = dataPtr->data;
        delete dataPtr;
        size = size - 1;

        // return
        return dataTemp;
    };

    // operator overloading of Get()
    T operator[](int idx) {
        return Get(idx);
    }

    List<T>& operator=(const List<T>& src) {
        if (this == &src) return *this;
        this->Clear();

        ListNode<T>* nodePtr = src.front->next;
        while (nodePtr != NULL) {
            this->Push(nodePtr->data, -1);
            nodePtr = nodePtr->next;
        }

        return *this;
    }

    // delete all node
    void Clear() {
        while (front != NULL) {
            ListNode<T>* p = front;
            front = front->next;
            delete p;
        }
        front = rear = new ListNode<T>();
        size = 0;
    }

    // insert a item before index node
    void Push(const T& item, int index = 0) {

        
        if (index > 0) index = index % (size + 1);
        if (index < 0) index = (size + 1) - ((-index) % (size + 1));
        

        ListNode<T>* newNode = new ListNode<T>(item);
        ListNode<T>* insertPtr;

        if (index == size || size <= 0) {
            rear->next = newNode;
            newNode->prev = rear;
            newNode->next = NULL;

            rear = newNode;
            size = size + 1;
            return;
        }
        else if (index <= (size / 2)) {
            // begin travel with front
            insertPtr = front->next;
            for (unsigned int i = 0; i < index; i++)
                insertPtr = insertPtr->next;
        }
        else {
            // begin travel with rear
            index = size - index - 1;
            insertPtr = rear;
            for (unsigned int i = 0; i < index; i++)
                insertPtr = insertPtr->prev;
        }
        newNode->prev = insertPtr->prev;
        newNode->next = insertPtr;
        insertPtr->prev->next = newNode;
        insertPtr->prev = newNode;
        size = size + 1;
        return;
    };
};

#endif