#ifndef STACK_H_
#define STACK_H_

#include "bag.h"

template <class T>
class Stack:public Bag<T> {
  
public:
    Stack(int StackCapacity) :Bag(StackCapacity) {
        strcpy(name, "Stack");
    };
    ~Stack() {};
	T* Pop(T& dst) {
		// check if empty
		if (IsEmpty()) {
			Empty();
			return NULL;
		}
		// read data
		dst = array[--top];
		// print to stdout
		message(dst);

		return &dst;
	};// delete the element from stack
};


#endif