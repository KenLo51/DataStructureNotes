#ifndef DEQUE_H_
#define DEQUE_H_

#include "queue.h"

template <class T>
class Deque :public Queue<T> {
public:
    Deque(int DequeSize) :Queue(DequeSize) {
        front = rear = 0;
        strcpy(name, "Deque");
    };
    ~Deque() {};
    T* Popf(T& dst) {
        return Pop(dst);
    };         // delete the element from queue
    T* Popr(T& dst) {
        // check if empty
        if (IsEmpty()) {
            Empty();
            return NULL;
        }
        // update rear index
        rear = (rear <= 0) ? MaxSize - 1 : rear - 1;
        // read data
        dst = array[rear];
        // update used size
        top = top - 1;
        // print to stdout
        message(dst);

        return &dst;
    };         // delete the element from queue
    void Pushr(const T& item) {
        Push(item);
    };
    void Pushf(const T& item) {
        // check if full
        if (IsFull()) {
            Full(item);
            return;
        }
        // update front index
        front = (front <= 0) ? MaxSize - 1 : front - 1;
        // save data
        array[front] = item;
        // update used size
        top = top + 1;
    };
};

#endif