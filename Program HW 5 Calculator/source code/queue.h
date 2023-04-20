#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdlib.h>
#include <iostream>

template <class T>
struct QueueNode{
    struct QueueNode* prev;
    T data;
    struct QueueNode* next;

    struct QueueNode() : prev(NULL), next(NULL) {};
    struct QueueNode(T data) : prev(NULL), next(NULL), data(data) {};
    struct QueueNode(struct QueueNode* prev, struct QueueNode* next, T data) : prev(prev), next(next), data(data) {};
};

template <class T>
class Queue {
// defined variables
protected:
    QueueNode<T>* rear, * front; // circular list implementation
    unsigned int size;

// defined functions
private:
public:
    // constructor
    Queue(){
        front = rear = new QueueNode<T>;
        size = 0;
    };
    Queue(T* beg, T* end) {
        front = rear = new QueueNode<T>;
        size = 0;
        for (beg; beg != end; beg++) {
            QueueNode<T>* newNode = new QueueNode<T>(rear, NULL, *beg);
            rear->next = newNode;
            rear = newNode;
            size++;
        }
    };

    // destructor
    ~Queue() {
        while (front != NULL) {
            QueueNode<T>* p = front;
            front = front->next;
            delete p;
        }
    };

    // get the current size of queue
    unsigned int Size() {
        return size;
    }

    // check if empty
    bool isEmpty() {
        return size == 0;
    }

    // get node
    QueueNode<T>* Data(){
        return front->next;
    }

    // read from queue, negedge index can read from the end of queue
    T Get(int index, QueueNode<T>** returnPtr = NULL) {
        // check if valid operation
        if (size == 0) {
            std::cerr << "The Queue is empty\n";
            throw;
        }

        QueueNode<T> *tempPtr = NULL;
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
        QueueNode<T> unknowData;
        return (*returnPtr)->data;
    };

    // read and delete
    T Pop(int index = -1) {
        // read data
        QueueNode<T>* dataPtr;
        Get(index, &dataPtr);

        // adject queue(list)
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

    // delete all node
    void Clear(Queue<T>& src) {
        while (front != NULL) {
            QueueNode<T>* p = front;
            front = front->next;
            delete p;
        }
        front = rear = new QueueNode<T>();
        size = 0;
    }

    // insert a item before index node
    void Push(const T& item, int index = 0) {

        
        if (index > 0) index = index % (size + 1);
        if (index < 0) index = (size + 1) - ((-index) % (size + 1));
        

        QueueNode<T>* newNode = new QueueNode<T>(item);
        QueueNode<T>* insertPtr;

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