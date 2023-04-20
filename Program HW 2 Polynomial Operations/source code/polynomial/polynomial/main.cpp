#include <iomanip>
#include <string>


#include "Polynomial.h"

int main()
{
	Polynomial a, b, c, d;
	std::cout << "(1) Input Data\n";

	a.getdata('A');
	b.getdata('B');

	//a.set(1, 1); //1 x^1
	//a.set(2, -2); //2 x^2
	//a.name = std::string("A");
	//b.set(1, 1); //5 x^0
	//b.set(2, 2); //3 x^3
	////b.set(4, 4);   // 4 x^4
	//b.name = std::string("B");

	std::cout << a.name << "(x)=" << a << std::endl;
	std::cout << b.name << "(x)=" << b << std::endl;
	std::cout << "\n(2) Perform Polynomial Operations (+, -, *):\n";
	c = a + b;   //   operator overloading of "+"
	std::cout << std::setw(5) << a.name << "+" << b.name << "=";
	std::cout << a << "+" << b << "=" << c << std::endl;    // operator overloading of "+"		
	c = a - b;      //  operator overloading of "-", a.minus(b)
	std::cout << std::setw(5) << a.name << "-" << b.name << "=";
	std::cout << a << "-" << b << "=" << c << std::endl;    // print out the c polynomial

	c = a * b; // operator overloading of "*" or c=a.times(b)
	std::cout << std::setw(5) << a.name << "*" << b.name << "=";
	std::cout << a << "*" << b << "=" << c << std::endl;  //  operator overloading for output polynomial

	std::cout << "\n(3) Perform Derivations D'(x)\n";
	d = c.differentiate().differentiate();  d.name = 'D';
	std::cout << "    Differentiate " << d.name << "(x) = " << c << " two times lead to: \n"
		<< std::setw(5) << d.name << "(x) = " << d << std::endl;   // operator overloading <<

	int x = 2;
	std::cout << "\n(4) Setting x = " << x << std::endl;
	std::cout << "    Evaluate the polynomial C(x) = " << c
		<< " with x=" << x << std::endl
		<< "    i.e., C(" << x << "), lead to a numerical value : "
		<< c(x) << std::endl; // evaluate the polynomial with x=2 by horner's method, 
	// i.e., a.evaluate(x=2) is replace with a(2)

	a.Delete();
	b.Delete();
	c.Delete();
	d.Delete();

	std::cin.get();   // wait for you to press a key to finish the program

	return 0;
}





