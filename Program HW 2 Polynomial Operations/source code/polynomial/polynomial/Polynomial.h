#pragma once
#include <iostream>
#include <iomanip>
#include <string>

struct polynomial_symbol {
    float coef;
    int vari;
    polynomial_symbol* prev;
    polynomial_symbol* next;

    polynomial_symbol() : prev(NULL), next(NULL), coef(0), vari(0) {}
    polynomial_symbol(float a, int b) : prev(NULL), next(NULL), coef(a), vari(b) {}
    polynomial_symbol(float a, int b, polynomial_symbol* prev, polynomial_symbol* next) : prev(prev), next(next), coef(a), vari(b) {}
};
class Polynomial {
    friend std::ostream& operator <<(std::ostream& os, Polynomial& s);

public:
    polynomial_symbol* symbols;
    unsigned int numSymbols;
    std::string name;

    //define public member functions
public:
    Polynomial();// Polynomial construct
    void Delete();// 
    void getdata(char c);           // get polynomial (coeff, order)s data
    void set(float a, int b);
    int degree();                   // find the deg of a polynomial
    float operator()(float x);
    float evaluate(float x);
    Polynomial differentiate();     // differentiate this polynomial and return it
    Polynomial& operator+(Polynomial& b);
    Polynomial operator-(Polynomial& b);
    Polynomial& operator*(Polynomial& b);  // polynomial multiplication
};

