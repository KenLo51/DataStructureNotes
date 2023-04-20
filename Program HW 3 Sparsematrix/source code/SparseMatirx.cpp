#include <fstream>
#include <iomanip>
#include "SparseMatrix.h"

char compare(int a, int b) {
	if (a > b) return '>';
	else if (a < b) return '<';
	else if (a == b) return '=';
};

SparseMatrix::SparseMatrix(int ncol, int nrow, int nterm) {
	SparseMatrix::Resize(ncol, nrow, nterm);
}

void SparseMatrix::Resize(int ncol, int nrow, int nterm) {
	if (SparseMatrix::Terms == nterm) return;

	int minTerm = (nterm > SparseMatrix::Terms) ? SparseMatrix::Terms : nterm;
	MatrixTerm* newArray = new MatrixTerm[nterm];
	if (SparseMatrix::smArray != NULL) {
		for (unsigned int i = 0; i < minTerm; i++)
			newArray[i] = SparseMatrix::smArray[i];
		delete[] SparseMatrix::smArray;
	}
	SparseMatrix::smArray = newArray;

	SparseMatrix::Rows = nrow;
	SparseMatrix::Cols = ncol;
	SparseMatrix::Terms = nterm;
}

SparseMatrix SparseMatrix::Transpose()
{  
	SparseMatrix bt(this->Cols, this->Rows, this->Terms);  
	unsigned int bCnt = 0;
	if (this->Terms > 0) {
		for (unsigned int col = 0; col < bt.Cols; col++) {
			for (unsigned int row = 0; row < bt.Rows; row++) {
				int val = SparseMatrix::get(col, row);
				if (val == 0) continue;
				// save the value to c
				bt.smArray[bCnt++].set(row, col, val);

				// if smArray does not have enought space
				if (bCnt >= bt.Terms) {
					int maxTerms = bt.Cols * bt.Rows;
					int extendStep = 10;
					int nextTerms = bt.Terms + 10;
					if (nextTerms > maxTerms) nextTerms = maxTerms;
					// extand the c's smArray
					bt.Resize(bt.Cols, bt.Rows, nextTerms);
				}
			}
		}
	}
	
	return bt; // or return *b 
}

// Fast Transpose
SparseMatrix SparseMatrix::FastTranspose()
{//  O(terms + cols) time complexity	
	SparseMatrix b(Cols, Rows, Terms);
	
	int* rowSize = new int[Cols];
	for (unsigned int i = 0; i < Cols; i++)
		rowSize[i] = 0;

	for (unsigned int i = 0; i < Terms; i++) {
		if (SparseMatrix::smArray[i].row == -1) break;
		if (SparseMatrix::smArray[i].col == -1) break;
		rowSize[SparseMatrix::smArray[i].col]++;
	}

	int* rowStart = new int[Cols];
	rowStart[0] = 0;
	for (unsigned int i = 1; i < Cols; i++)
		rowStart[i] = rowStart[i-1] + rowSize[i-1];

	for (unsigned int i = 0; i < Terms; i++) {
		if (SparseMatrix::smArray[i].row == -1) break;
		if (SparseMatrix::smArray[i].col == -1) break;

		int row = SparseMatrix::smArray[i].row;
		int col = SparseMatrix::smArray[i].col;
		int val = SparseMatrix::smArray[i].value;

		b.smArray[rowStart[col]].value = val;
		b.smArray[rowStart[col]].row = col;
		b.smArray[rowStart[col]].col = row;
		rowStart[col]++;
	}

	return b;
}

istream& operator>>(istream& is, SparseMatrix& m)
// read in a matrix and set up its linked representation.
// An auxiliary array head is used
{
	is >> m.Rows >> m.Cols >> m.Terms; // matrix dimensions
	is.clear(); is.ignore(100, '\n');
	int CurrentA = 0;
	for (int i = 0; i < m.Terms; i++)
	{
		is >> m.smArray[i].row >> m.smArray[i].col >> m.smArray[i].value;
		is.clear(); is.ignore(100, '\n');
	}
	return is;
}

ostream& operator<<(ostream& os, SparseMatrix& m)
{   if(m.Rows)
	if (m.Rows*m.Cols<30) {
		int i, j;
		int** A = new int* [m.Rows];
		for (i = 0; i < m.Rows; i++)
			A[i] = new int[m.Cols];

		for (i = 0; i < m.Rows; i++)
			for (j = 0; j < m.Cols; j++)
				A[i][j] = 0;
		//printf("newArray with size %d\n", m.Terms);
		MatrixTerm* p = m.smArray;
		for (i = 0; i < m.Terms; i++) {
			if (p[i].row == -1 || p[i].col == -1)
				continue;
			A[p[i].row][p[i].col] = p[i].value;
		}
		for (i = 0; i < m.Rows; i++) {
			for (j = 0; j < m.Cols; j++)
				os << setw(4) << A[i][j];
			os << endl;
		}
		os << endl;
	}
	else {
		for (int i = 0; i < m.Terms; i++)
			os << "(" << setw(4)<<m.smArray[i].row << "," <<setw(4)<< m.smArray[i].col << "," <<setw(4)<< m.smArray[i].value << ")" << endl;
		os << endl;
	}
	return os;
}

int SparseMatrix::get(int index) {
	return SparseMatrix::smArray[index].value;
}
int SparseMatrix::get(int col, int row) {
	for (unsigned int i = 0; i < SparseMatrix::Terms; i++) {
		if (SparseMatrix::smArray[i].row == row && SparseMatrix::smArray[i].col == col)
			return SparseMatrix::smArray[i].value;
	}
	return 0;
}

SparseMatrix &SparseMatrix::operator*(SparseMatrix &b)
{
	if (this->Cols != b.Rows) {
		printf("Can't Multipy, invalid matrix dimensions for (%d*%d) and (%d*%d)\n", this->Rows, this->Cols, b.Rows, b.Cols);
		SparseMatrix* c = new SparseMatrix(0, 0, 0);
		return *c;
	}
	SparseMatrix bt = b.Transpose();    
	SparseMatrix *c = new SparseMatrix(this->Rows, b.Cols, this->Terms);
	
	// DO IT BY YOURSELF
	unsigned int cCnt = 0;
	for (unsigned int row = 0; row < c->Rows; row++) {
		for (unsigned int col = 0; col < c->Cols; col++) {
			int val = 0;
			for (unsigned int i = 0; i < this->Cols; i++) {
				val += this->get(i, row) * b.get(col, i);
			}

			// save the value to c
			c->smArray[cCnt++].set(row, col, val);

			// if smArray does not have enought space
			if (cCnt >= c->Terms) {
				int maxTerms = c->Cols * c->Rows;
				int extendStep = 10;
				int nextTerms = c->Terms + 10;
				if (nextTerms > maxTerms) nextTerms = maxTerms;
				// extand the c's smArray
				c->Resize(c->Cols, c->Rows, nextTerms);
			}
		}
	}

	return *c;
}
SparseMatrix& SparseMatrix::operator+(SparseMatrix& b){
	if (this->Rows != b.Rows || this->Cols != b.Cols) {
		printf("Can't Addition, invalid matrix dimensions for (%d*%d) and (%d*%d)\n", this->Rows, this->Cols, b.Rows, b.Cols);
		SparseMatrix* c = new SparseMatrix(0, 0, 0);
		return *c;
	}

	SparseMatrix *c=new SparseMatrix(this->Rows, this->Cols, this->Terms);
	
	MatrixTerm* aArr = SparseMatrix::smArray;
	MatrixTerm* bArr = b.smArray;
	unsigned int aCnt = 0;
	unsigned int bCnt = 0;
	unsigned int cCnt = 0;
	while (1) {
		int row=0, col=0, val=0;

		// get the next value
		int aIdx = -1;
		if (aCnt < SparseMatrix::Terms) {
			int aRow = SparseMatrix::smArray[aCnt].row;
			int aCol = SparseMatrix::smArray[aCnt].col;
			if (aRow >= 0 && aCol >= 0) aIdx = SparseMatrix::Cols * aRow + aCol;
		}
		
		int bIdx = -1;
		if (bCnt < b.Terms) {
			int bRow = b.smArray[bCnt].row;
			int bCol = b.smArray[bCnt].col;
			if (bRow >= 0 && bCol >= 0) bIdx = b.Cols * bRow + bCol;
		}

		// find the value with minimum index in a and b
		if (aIdx == -1 && bIdx == -1) break;
		else if (aIdx != -1 && bIdx == -1) {
			row = aIdx / SparseMatrix::Cols;
			col = aIdx % SparseMatrix::Cols;
			val = SparseMatrix::smArray[aCnt].value;
			aCnt++;
		}
		else if (aIdx == -1 && bIdx != -1) {
			row = bIdx / b.Cols;
			col = bIdx % b.Cols;
			val = b.smArray[bCnt].value;
			bCnt++;
		}
		else if (aIdx == bIdx) {
			row = aIdx / SparseMatrix::Cols;
			col = aIdx % SparseMatrix::Cols;
			val = SparseMatrix::smArray[aCnt].value + b.smArray[bCnt].value;
			aCnt++; bCnt++;
		}
		else if (aIdx < bIdx) {
			row = aIdx / SparseMatrix::Cols;
			col = aIdx % SparseMatrix::Cols;
			val = SparseMatrix::smArray[aCnt].value;
			aCnt++;
		}
		else if(aIdx > bIdx) {
			row = bIdx / b.Cols;
			col = bIdx % b.Cols;
			val = b.smArray[bCnt].value;
			bCnt++;
		}

		// save the value to c
		c->smArray[cCnt++].set(row, col, val);

		// if smArray does not have enought space
		if (cCnt >= c->Terms) {
			int maxTerms = c->Cols * c->Rows;
			int extendStep = 10;
			int nextTerms = c->Terms + 10;
			if (nextTerms > maxTerms) nextTerms = maxTerms;
			// extand the c's smArray
			c->Resize(c->Cols, c->Rows, nextTerms);
		}
	}
	
	return *c;
}