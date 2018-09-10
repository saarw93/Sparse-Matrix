/*Name: Saar Weitzman
ID: 204175137    */

#ifndef _SMATRIX_H
#include <iostream>
#include <string>
#include <assert.h>
using namespace std;

#define ARROW_HEAD "Arrowhead"
#define TOEPLITZ "Toeplitz"
#define ANY "any"

// A node of a sparse matrix.  
class MNode {

public:
	double _data;
	int _indexI, _indexJ; // the place of the node in the matrix  
	MNode* _nextRow, *_nextCol;    // clarification: _nextRow is a pointer to the next columns in the row. _nextCol- pointer to the next rows in the column

	MNode(double data, int i, int j)
	{
		this->_data = data;
		this->_indexI = i;
		this->_indexJ = j;
		this->_nextRow = NULL;
		this->_nextCol = NULL;
	}
};


// A sparse matrix. a mathematical matrix object which is implemented
// in a way that saves memory if the matrix is sparse. The implementation
// consists of linked lists of elements which are not zero. 
class SMatrix {
private:
	MNode** _rowHead, **_colHead; // _rowHead : array of head-pointers for the rows lists. 
	int _rowSize, _colSize; // _rowSize - number of rows
	int _elemNum; // number of entries not containing zero 
	string _matType;

	void setValue(int, int, double);
	void removeElement(int, int);
	void insertNode(MNode*);
	bool IsExist(int, int);

	void initialize(int rows, int cols, string matType);
	bool isToeplitz();
	bool is_diagonal_equal(int iRow, int jCol);
	bool isArrowHead();
	void deleteNodes();
	void setElement_no_break_check(int i, int j, double data);
	void copy(const SMatrix& other);

public:

	// construct a 'rows X cols' smatrix.
	SMatrix(int rows, int cols, string matType);

	SMatrix(const SMatrix& other);


	// set the (i,j) element to be 'data' 
	void setElement(int i, int j, double data);
	//returns the 'data' in the [i,j] place of the matrix
	double getElement(int i, int j);
	bool isA(string matType);
	// rowShift     
	void rowShift(const int shiftSize);
	// colShift     
	void colShift(const int shiftSize);
	// //calculate the size of the object SMatrix by bytes
	void sizeInBytes();
	// operator + for SMatrix
	SMatrix operator+(SMatrix &other);
	// operator = for SMatrix
	SMatrix& operator=(SMatrix &other);
	// print rows as linked lists
	void printRowsIndexes();
	// print columns as linked lists
	void printColumnsIndexes();
	// destroy this matrix. 
	~SMatrix();

	friend std::ostream& operator<<(std::ostream& os, const SMatrix& mat);
};

#endif 	_SMATRIX_H
