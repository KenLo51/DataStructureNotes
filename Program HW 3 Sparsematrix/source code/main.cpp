#include "SparseMatrix.h"
#include <fstream>
#include <iostream>

int main(int argc, char* argv[])
{
	SparseMatrix A, B, C, D;
	ifstream fin;
	if (argc == 1) fin.open("sparse.txt");  // open the default sparsematrix data 
	else fin.open(argv[1]);
	if (!fin) { // check whether fin is correct or not
		cout << "the input file [" << "sparse2.txt" << "] open error\n"; exit(1);
	}
	else { 
		fin >> A >> B>> C >> D;
	}
	fin.close();
	cout << "Matrix A\n" << A << "Matrix B\n" << B;
	cout << "Matrix A + B\n" <<  A + B;
	cout << "Matrix C\n" << C << "Matrix D\n" << D;
	cout << "Matrix C * D\n" <<  C * D;
	cout << "C + D\n" << C + D;
	cout << "B * C\n" << B * C << endl ;
	system("PAUSE");
	return 0;
}