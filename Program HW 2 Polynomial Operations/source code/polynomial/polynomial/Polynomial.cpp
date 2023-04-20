#include "Polynomial.h"

// this function perform cout << poly operation
std::ostream& operator <<(std::ostream& os, Polynomial& s) {
    os << "(";
    polynomial_symbol *p = s.symbols;
    bool firstVal = true;
    while (p != NULL) {
        if(p->coef > 0.0f && (!firstVal))
            os << "+";
        if (p->coef != 0.0f) {
            if (p->vari == 1.0f) { // dont print exp when it equal to 1
                os << p->coef << "x";
            }
            else if (p->vari == 0.0f) {
                os << p->coef;
            }
            else if (p->vari > 0.0f) {
                os << p->coef << "x^" << p->vari;
            }
            else if (p->vari < 0.0f) {
                os << p->coef << "x^(" << p->vari << ")";
            }
            firstVal = false;
        }

        p = p->next;
        if (p == s.symbols) break;
    }
    if (firstVal) os << "0";
    os << ")";
    return os;
}

void Polynomial::getdata(char c) {
    // read data from stdin
    float coef;
    int exp;
    name = c;
    while (std::cin >> coef) {
        std::cin >> exp;
        set(coef, exp);
    }
    std::cin.clear();
    std::cin.ignore(100, '\n');
    std::cout << std::setw(4) << ' ' << "Polynomial " << c << ": " << *this << std::endl;
};

// Polynomial construct
Polynomial::Polynomial() {
    // class initialize
    Polynomial::numSymbols = 1;
    Polynomial::symbols = new polynomial_symbol(0, 0);
    Polynomial::symbols->next = Polynomial::symbols->prev = Polynomial::symbols;
}
// 
void Polynomial::Delete() {
    polynomial_symbol* p = Polynomial::symbols;
    for (unsigned int i = 0; i < Polynomial::numSymbols; i++) {
        polynomial_symbol* deleteNode = p;
        p = p->next;
        delete deleteNode;
    }
}


void Polynomial::set(float a, int b) { // function to set one term (a*x^b)
    // do not insert if coefficient is zero
    if (a == 0) return;

    polynomial_symbol* newSymbol = new polynomial_symbol(a, b);

    // if there is no element in polynomial
    if (Polynomial::symbols == NULL) {
        // never process to here
        Polynomial::symbols = newSymbol;
        newSymbol->next = newSymbol->prev = newSymbol;
        return;
    }

    // insert coef to the list
    polynomial_symbol* p = Polynomial::symbols;
    if (p->prev->vari < newSymbol->vari) { // check if it's exp is greater then the other in polynomial
        // search position to insert
        while (p != NULL) {
            if (p->vari >= newSymbol->vari) break;

            p = p->next;
            if (p == Polynomial::symbols) break;
        }
    }
    else {
        Polynomial::numSymbols = Polynomial::numSymbols + 1;
        newSymbol->next = p;
        newSymbol->prev = p->prev;
        p->prev = newSymbol;
        newSymbol->prev->next = newSymbol;
        Polynomial::symbols = Polynomial::symbols->prev;
        return;
    }


    if (p->vari == newSymbol->vari) {
        // there are existed a non-zero coef
        // replace the coef
        p->coef = newSymbol->coef;
        delete newSymbol;
        return;
    }
    else {
        // insert coef to the list
        Polynomial::numSymbols = Polynomial::numSymbols + 1;
        newSymbol->next = p;
        newSymbol->prev = p->prev;
        p->prev = newSymbol;
        newSymbol->prev->next = newSymbol;
    }
}


Polynomial& Polynomial::operator+(Polynomial& b){
    Polynomial c;
    Polynomial& a = *this;
    
    polynomial_symbol* a_p = a.symbols;
    unsigned int aCnt = 0;
    polynomial_symbol* b_p = b.symbols;
    unsigned int bCnt = 0;
    while(aCnt < a.numSymbols && bCnt < b.numSymbols){
        if (a_p->vari == b_p->vari) {
            float coef = a_p->coef + b_p->coef;
            if (coef != 0)  c.set(coef, a_p->vari);
            a_p = a_p->next;    aCnt++;
            b_p = b_p->next;    bCnt++;
            continue;
        }
        if (a_p->vari < b_p->vari) {
            c.set(a_p->coef, a_p->vari);
            a_p = a_p->next;    aCnt++;
            continue;
        }
        if (b_p->vari < a_p->vari) {
            c.set(b_p->coef, b_p->vari);
            b_p = b_p->next;    bCnt++;
            continue;
        }
    }
    while (aCnt < a.numSymbols) {
        c.set(a_p->coef, a_p->vari);
        a_p = a_p->next;    aCnt++;
    }
    while (bCnt < b.numSymbols) {
        c.set(b_p->coef, b_p->vari);
        b_p = b_p->next;    bCnt++;
    }

    return c;
}

// similar as operator+
Polynomial Polynomial::operator-(Polynomial& b){
    Polynomial c;
    Polynomial& a = *this;

    polynomial_symbol* a_p = a.symbols;
    unsigned int aCnt = 0;
    polynomial_symbol* b_p = b.symbols;
    unsigned int bCnt = 0;
    while (aCnt < a.numSymbols && bCnt < b.numSymbols) {
        if (a_p->vari == b_p->vari) {
            float coef = a_p->coef - b_p->coef;
            if (coef != 0)  c.set(coef, a_p->vari);
            a_p = a_p->next;    aCnt++;
            b_p = b_p->next;    bCnt++;
            continue;
        }
        if (a_p->vari < b_p->vari) {
            c.set(a_p->coef, a_p->vari);
            a_p = a_p->next;    aCnt++;
            continue;
        }
        if (b_p->vari < a_p->vari) {
            c.set(-b_p->coef, b_p->vari);
            b_p = b_p->next;    bCnt++;
            continue;
        }
    }
    while (aCnt < a.numSymbols) {
        c.set(a_p->coef, a_p->vari);
        a_p = a_p->next;    aCnt++;
    }
    while (bCnt < b.numSymbols) {
        c.set(-b_p->coef, b_p->vari);
        b_p = b_p->next;    bCnt++;
    }

    return c;
}

Polynomial& Polynomial::operator*(Polynomial& b)  // polynomial multiplication
{
    Polynomial c;
    Polynomial& a = *this;

    polynomial_symbol* a_p = a.symbols;
    polynomial_symbol* b_p = b.symbols;
    for (unsigned int i = 0; i < b.numSymbols; i++) {
        Polynomial newP;
        a_p = a.symbols;

        for (unsigned int j = 0; j < a.numSymbols; j++) {
            newP.set(b_p->coef * a_p->coef, b_p->vari + a_p->vari);
            a_p = a_p->next;
        }

        c = c + newP;
        b_p = b_p->next;
    }

    return c;
}

// using C(2) instead of C.evaluate(2)
// to evaluate the polynomial when x=2
float Polynomial::operator()(float x) {
    float p = evaluate(x);
    return p;
}

// Horner's method
float Polynomial::evaluate(float x) {
    // colculate the part that have non-negedge exponent
    float result0 = 0.0f;
    polynomial_symbol* p;
    p = Polynomial::symbols->prev;
    for (int i = Polynomial::symbols->prev->vari; i >= 0; i--) {
        result0 = result0 * x;
        if (p->vari == i) {
            result0 += p->coef;
            p = p->prev;
        }
    }

    // colculate the part that have negedge exponent
    float result1 = 0.0f;
    p = Polynomial::symbols;
    for (int i = Polynomial::symbols->vari; i < 0; i++) {
        if (p->vari == i) {
            result1 = result1 + p->coef;
            p = p->prev;
        }
        result1 = result1 / x;
    }

    // return the sum of two part
    return result0 + result1;
}

Polynomial Polynomial::differentiate()
{
    Polynomial deriv;  // = new Polynomial ( 0, deg - 1 );
    polynomial_symbol* p = Polynomial::symbols;
    for (unsigned int i = 0; i < Polynomial::numSymbols; i++) {
        if(p->vari != 0)
            deriv.set(p->vari * p->coef, p->vari - 1);
        p = p->next;
    }
    return deriv;
}