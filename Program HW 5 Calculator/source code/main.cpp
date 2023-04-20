#include "stdio.h"
#include <string>
#include <iostream>
#include <iomanip>

#include "queue.h"

#include "Calculator.h"

int main() {
	char cont;
	do {
		std::string infix;
		std::cout << std::left << std::setw(10) << "\nINFIX:    ";
		std::getline(std::cin >> std::ws, infix);

		double result = Calculator::Calculate(infix.c_str());
		std::cout << "Evaluate: " << infix << " = " << result << std::endl;
		std::cout << "Continue? (Y/N) ";
		std::cin >> cont; std::cout << (char)toupper(cont) << std::endl;
	} while (cont == 'Y' || cont == 'y');

	//char form[] = { "-3*5" };
	//double result = Calculator::Calculate(form);
	//printf("\n\nresult : %f\n", result);

	//Queue<char> q;
	//q.Push('(', -1);
	//q.Push('(', -1);
	//q.Push('+', -1);
	//printf("%c%c%c\n", q[0], q[1], q[2]);


	return 0;
}