#ifndef QUEUE_H_
#define QUEUE_H_

#include "bag.h"

template <class T>
class Queue:public Bag<T> {
    friend	ostream& operator<<<>(ostream&, const Queue <T>&);

public:
    Queue(int QueueSize):Bag(QueueSize) {
        front = rear = 0;
        strcpy(name, "Queue");
    };
    ~Queue() {};
    T* Pop(T& dst) { 
        // check if empty
        if (IsEmpty()) {
            Empty();
            return NULL;
        }
        // read data
        dst = array[front];
        // update front index
        front = (front + 1) % MaxSize;
        // update used size
        top = top - 1;
        // print to stdout
        message(dst);

        return &dst;
    };         // delete the element from queue
    void Push(const T& item) {
        // check if full
        if (IsFull()) {
            Full(item);
            return;
        }
        // save data
        array[rear] = item;
        // update rear index
        rear = (rear + 1) % MaxSize;
        // update used size
        top = top + 1;
    };
protected:
    int rear, front; // circular list implementation
};


template <class T>
ostream& operator<<<>(ostream& os, const Queue <T>& q) { 
    os << " All Data in " << right << setw(6) << q.name << '[' << q.MaxSize << "] are: ";
    for (int i = 0; i < q.MaxSize; i++)
        os << "(" << i << ") " << q.array[i] << setw(3) << " ";
    os << "\n Valid data among them are: ";
    for (int i = 0; i < q.top; i++)
        os << "(" << (i+q.front)%q.MaxSize << ") " << q.array[(i + q.front) % q.MaxSize] << setw(3) << " ";
    cout << "\n\n";
    return os;
};

#endif