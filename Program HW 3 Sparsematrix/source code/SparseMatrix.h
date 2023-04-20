#include <iostream>
using namespace std;
#ifndef _SPARSE_MATRIX
#define _SPARSE_MATRIX

class SparseMatrix;

class MatrixTerm{
    friend ostream & operator<<(ostream & os, SparseMatrix& m);
    friend istream & operator>>(istream & is, SparseMatrix& m);
    friend class SparseMatrix;
public: 
    void set(int r, int c, int v) {
        row = r, col = c, value = v;
    };
    void set(MatrixTerm& m) {
        row = m.row; col = m.col; value = m.value;
    };
private: int row=-1,col=-1,value=0;
};


class SparseMatrix
{
    friend ostream & operator<<(ostream & os, SparseMatrix& m);
    friend istream & operator>>(istream & is, SparseMatrix& m);
public:
    SparseMatrix(int ncol = 0, int nrow = 0, int nterm = 20);
    void Resize(int ncol = 0, int nrow = 0, int nterm = 20);
    int get(int index);
    int get(int col, int row);
    SparseMatrix Transpose();
    SparseMatrix FastTranspose();
    SparseMatrix & operator+(SparseMatrix& b);
    SparseMatrix & operator*(SparseMatrix& b);  // polynomial multiplication
  
private:
    unsigned int Rows = 0, Cols = 0;
    unsigned int Terms = 0;
    MatrixTerm* smArray = NULL;
};

#endif