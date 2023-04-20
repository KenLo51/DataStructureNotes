#ifndef BAG_H
#define BAG_H

#include <iostream>
#include <string.h>
#include <iomanip>
using namespace std;

template <class T>
class Bag {
	friend	ostream& operator<<<>(ostream&, const Bag <T>&);
public:
	Bag(int BagSize) :MaxSize(BagSize), name("Bag"), top(0) {
		array = new T[MaxSize];
	};  
	~Bag() {delete[] array; };   // destructor
	virtual int  Size() const { return top; };
	virtual bool IsEmpty() const { return (top == 0); };
	virtual bool IsFull() const { return (top >= MaxSize); };
	virtual void Push(const T& item) {
		// check if full
		if (IsFull()) {
			Full(item);
			return;
		}
		// read data
		array[top++] = item;
	};
	virtual T* Pop(T& dst) { 
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
	};
protected:	
	T *array;	
	int MaxSize;
	int top;   // position of the top element
	char name[20];
	void Empty() { cout <<"   "<<setw(6) << left << name << " is empty. Cannot Pop()\n"; };
	void Full(const T& x) { cout <<"   "<< setw(6) << left << name << " is full for " << x << endl; }
	void message(const T& x) { cout << "     pop " << x << " from " << name << endl; };
};

template <class T>
ostream& operator<<<>(ostream& os, const Bag <T>& b) {
	os << " All Data in " << right << setw(6) << b.name << '[' << b.MaxSize << "] are: ";
	for (int i = 0; i < b.MaxSize; i++)
		os << "(" << i << ") " << b.array[i] << setw(3) << " ";
	os << "\n Valid data among them are: ";
	for (int i = 0; i < b.top; i++)
		os << "(" << i << ") " << b.array[i] << setw(3) << " ";
	cout << "\n\n";
	return os;
}
#endif

