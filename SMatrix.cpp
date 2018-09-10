/*Name: Saar Weitzman
  ID: 204175137    */

#include <iostream>
#include <string>
#include <assert.h>
#include "SMatrix.h"
using namespace std;

#define ARROW_HEAD "Arrowhead"
#define TOEPLITZ "Toeplitz"
#define ANY "any"


// construct a 'rows X cols' matrix. 
SMatrix:: SMatrix(int rows, int cols, string matType)
{
	if (rows <= 0 || cols <= 0)
	{
		cout << "NA: the matrice cannot have a negative size of rows and/or cols" << endl;
		return;
	}
	if (matType.compare(ANY) && matType.compare(ARROW_HEAD) && matType.compare(TOEPLITZ))   //if(matType != ANY && matType != ARROWHEAD && matType != TOEPLITZ)
	{
		cout << "NA: Undefined matType, please enter valid type of matrix" << endl;
		this->~SMatrix();
		return;
	}
	if (!matType.compare(ARROW_HEAD) && rows != cols)  // ArrowHead can be only rows = cols
	{
		cout << "Invalid input: Arrowhead matrix must have equal rows and cols" << endl;
		this->~SMatrix();
		return;
	}

	initialize(rows, cols, matType);
}

//copy constructor
SMatrix::SMatrix(const SMatrix& other)
{
	initialize(other._rowSize, other._colSize, other._matType);
	this->_elemNum = other._elemNum;
	copy(other);
}

//the method initialize the SMatrix varaiables
void SMatrix::initialize(int rows, int cols, string matType)
{
	this->_rowSize = rows;
	this->_colSize = cols;
	this->_elemNum = 0;
	this->_matType = matType;

	this->_rowHead = new MNode*[_rowSize];
	assert(_rowHead != NULL);
	for (int i = 0; i < _rowSize; i++)
		_rowHead[i] = NULL;
	this->_colHead = new MNode*[_colSize];
	assert(_colHead != NULL);
	for (int i = 0; i < _colSize; i++)
		_colHead[i] = NULL;
}

//the method copy the value of each index in the other SMatrix to the this SMatrix
void SMatrix::copy(const SMatrix& other)
{
	MNode* other_current = other._rowHead[0];
	int i;
	for (i = 0; i < _rowSize; i++)
	{
		other_current = other._rowHead[i];
		while (other_current != NULL && other._rowHead[i]->_indexJ < _colSize)   //the loops runs on each column index in evety row of the SMatrix
		{
			MNode *n = new MNode(other_current->_data, other_current->_indexI, other_current->_indexJ);
			insertNode(n);
			other_current = other_current->_nextRow;
		}
	}
}

/*private method to check if the matrix is 'Arrowhead' type*/
bool SMatrix::isArrowHead()
{
	if (this->_rowSize != this->_colSize)       //'ArrowHead' must be a square matrix
		return false;

	int i;
	MNode* node;

	/*the loops check if there are other numbers than zeroes, in indexes that should be only zeroes*/
	for (i = 1; i < this->_rowSize; i++)
	{
		node = this->_rowHead[i];
		
		while (node != NULL)
		{
			if (node->_indexI != 0 && node->_indexJ != 0 && node->_indexI != node->_indexJ)
				return false;
			else
				node = node->_nextRow;
		}
	}
	return true;
}

/*private method to check if the matrix is 'Toeplitz' type*/
bool SMatrix::isToeplitz()
{
	int i, j;
	for (i = 0, j = 0; i < _rowSize; i++)   //checks the central diagonal and the lower ones
	{
		if (is_diagonal_equal(i, j) == false)
			return false;
	}
	for (i = 0, j = 1; j < _colSize; j++)    //checks the upper diagonals from the central one
	{
		if (is_diagonal_equal(i, j) == false)
			return false;
	}
	return true;
}

/*private method that gets the start indexes of a diagonal in the matrix, and checks if the 'data' in it is equal*/
bool SMatrix::is_diagonal_equal(int iRow, int jCol)
{
	int i = iRow, j = jCol;
	double data = 0;
	if (IsExist(i, j) == true)
		data = getElement(i, j);

	for (int i = iRow + 1, j = jCol + 1; i < _rowSize && j < _colSize; i++, j++)
	{
		if (IsExist(i, j) == true)
		{
			if (data != getElement(i, j))
				return false;
		}
		else if (data != 0)
			return false;
	}
	return true;
}

/*checks if a matrix is from the matType it gets as an input*/
bool SMatrix::isA(string matType)
{
	std::cout.setf(std::ios::boolalpha);
	if (_matType.compare(ANY) && _matType.compare(ARROW_HEAD) && _matType.compare(TOEPLITZ)) // check if the matrix is initialized
	{
		cout << "NA - The matrix should be initialized first" << endl;
		return false;
	}

	if (!matType.compare(ARROW_HEAD))
		return isArrowHead();
	else if (!matType.compare(TOEPLITZ))
		return isToeplitz();
	else if (!matType.compare(ANY))
		return true;
	else
	{
		cout << "ERROR: The input matrix type is undefined" << endl;
		return false;
	}
}


// remove element with (i,j) index from row and column
// separate cases: first element in list or in the middle
void SMatrix::removeElement(int i, int j)
{
	MNode *prev = NULL;
	MNode *colPtr = _colHead[j];               // keep the column linked list
	MNode *rowPtr = _rowHead[i];               // keep the row linked list

	if (_colHead[j]->_indexI == i)             // if element is the first in column
		_colHead[j] = _colHead[j]->_nextCol;

	else                                       // if element is not the first in column
	{
		while (colPtr->_indexI != i)           // find prev element on the column
		{
			prev = colPtr;
			colPtr = colPtr->_nextCol;
		}
		prev->_nextCol = colPtr->_nextCol;     // connect the prev with the next
	}

	if (_rowHead[i]->_indexJ == j)             // if element is the first in row
	{
		rowPtr = _rowHead[i];                  //pointer to the element that is going to be removed, so we can delete it from the heap memory
		_rowHead[i] = _rowHead[i]->_nextRow;
	}

	else                                      // if element is not the first in row
	{
		while (rowPtr->_indexJ != j)          // find prev element on the row
		{
			prev = rowPtr;
			rowPtr = rowPtr->_nextRow;
		}
		prev->_nextRow = rowPtr->_nextRow;    // connect the prev with the next
	}

	delete rowPtr;                            // delete the element only once
}

//add new node to both row and column
// separate cases: list is empty or not empty
void SMatrix::insertNode(MNode* n)
{
	MNode* p = _rowHead[n->_indexI];  // the i row
	if (p != NULL)                    // there are elements in i row
	{
		if (p->_indexJ > n->_indexJ) // need to insert n at start, before existing elements
		{
			n->_nextRow = p;
			_rowHead[n->_indexI] = n;
		}
		else                          // need to inserst in the middle of the list or at end
		{
			while (p->_nextRow && p->_nextRow->_indexJ < n->_indexJ)
				p = p->_nextRow;
			n->_nextRow = p->_nextRow;
			p->_nextRow = n;
		}
	}
	else                              // row is empty
		_rowHead[n->_indexI] = n;

	p = _colHead[n->_indexJ];         // the j column
	if (p != NULL)                    // there are elements in j col
	{
		if (p->_indexI > n->_indexI)  // need to insert n at start, before existing elements
		{
			n->_nextCol = p;
			_colHead[n->_indexJ] = n;
		}
		else                          // need to inserst in the middle of the list or at end
		{
			while (p->_nextCol && p->_nextCol->_indexI < n->_indexI)
				p = p->_nextCol;
			n->_nextCol = p->_nextCol;
			p->_nextCol = n;
		}
	}
	else                            // column is empty
		_colHead[n->_indexJ] = n;
}

// set value to an exist element (i,j)
void SMatrix::setValue(int i, int j, double data)
{
	MNode *ptr = _rowHead[i];

	while (ptr->_indexJ != j)          // find the element on the row, by running on the J indexes
		ptr = ptr->_nextRow;
	ptr->_data = data;
}

// check if element (i,j) exists
bool SMatrix::IsExist(int i, int j)
{
	MNode *ptr = _rowHead[i];

	if (ptr == NULL)
		return false;

	while (ptr && ptr->_indexJ < j)
		ptr = ptr->_nextRow;

	if (ptr && ptr->_indexJ == j)
		return true;

	return false;
}

// set the (i,j) element to be 'data' 
void SMatrix::setElement(int i, int j, double data)
{
	if (i < 0 || i >= _rowSize || j < 0 || j >= _colSize)
	{
		cout << "NA: setElement out of range" << endl;
		return;
	}

	SMatrix copy_mat(*this);

	setElement_no_break_check(i, j, data);

	if (isA(this->_matType) == true)        //the set did not break the matType, so it can be done
		return;
	else
	{
		this->operator= (copy_mat);        //the set did break the matType, we copy back the original SMatrix to this
		cout << "NA" << endl;
	}
}

void SMatrix::setElement_no_break_check(int i,int j, double data) //makes the setElement without making checks on the matType after (for the shifts)
{
	if (i < 0 || i >= _rowSize || j < 0 || j >= _colSize)
	{
		cout << "NA: setElement out of range" << endl;
		return;
	}

	bool found = IsExist(i, j);
	if (data == 0 && found == true)  //change data different from zero to zero, need to remove that data (zero is null in SMatrix)
	{
		_elemNum--;
		removeElement(i, j);
	}
	if (data == 0 && found == false)  //need to change zero to zero, so do not need to do anything
	{
		return;
	}
	if (data != 0 && found == true)   //need to change non-zero data with non-zero data, so need to update the value
	{
		setValue(i, j, data);
	}
	if (data != 0 && found == false)  //need to change a zero data with non-zero data, so need to make a new MNode and insert it to the SMatrix 
	{
		MNode *n = new MNode(data, i, j);
		_elemNum++;
		insertNode(n);
	}
}

//returns the 'data' in the [i,j] place of the matrix
double SMatrix::getElement(int i, int j)
{
	if (i < 0 || i >= _rowSize || j < 0 || j >= _colSize)
	{
		cout << "getElement out of range";
		exit(-1);
	}

	if (_rowHead[i] != NULL)
	{
		MNode* ptr = _rowHead[i];
		while (ptr != NULL && ptr->_indexJ < j)
			ptr = ptr->_nextRow;
		if (ptr->_indexJ == j)
			return ptr->_data;
	}
	return 0;
}

// rowShift method. possitive numbers- shift down, negative numbers-shift up    
void SMatrix::rowShift(const int shiftSize)
{
	if (_matType.compare(ANY) && _matType.compare(ARROW_HEAD) && _matType.compare(TOEPLITZ)) // check if the matrix is initialized
	{
		cout << "NA - The matrix should be initialized first" << endl;
		return;
	}

	if (shiftSize == 0 || shiftSize%_rowSize == 0 || (-1 * shiftSize) % _rowSize == 0)     //in those cases the matrix shifts don't change the order of the rows
		return;

	SMatrix temp_mat(_rowSize, _colSize, _matType); 

	int i, j;
	MNode* row_ptr = NULL;
	for (i = 0; i < this->_rowSize; i++)
	{
		row_ptr = _rowHead[i];
		for (j = 0; j < this->_colSize; j++)
		{
			if (IsExist(i, j))    //if there is no element in the (i,j) place, no need to shift (temp_mat initialized to be zero mat, so the wanted place after shift is zero anyway)
			{
				if (shiftSize > 0)
				{
					int k = (i + shiftSize) % _rowSize;                               // k is the rowIndex that the data is need to be shifted
					temp_mat.setElement_no_break_check(k, j, row_ptr->_data);         //put the element in it's place in the temp_mat
					row_ptr = row_ptr->_nextRow;
				}
				else       // (shiftSize < 0)
				{
					int k = (((_rowSize - ((shiftSize*-1) % _rowSize)) + i) % _rowSize);      // k is the rowIndex that the data is need to be shifted
					temp_mat.setElement_no_break_check(k, j, row_ptr->_data);                //put the element in it's place in the temp_mat
					row_ptr = row_ptr->_nextRow;
				}
			}
		}
	}

	/*check if temp_mat is from the same matType the SMatrix was before the shift*/
	if (temp_mat.isA(_matType) == false)
	{
		cout << "NA" << endl;
	}
	else
	{
		this->operator= (temp_mat);
	}
}

// colShift method. possitive numbers- shift right, negative numbers-shift left     
void SMatrix::colShift(const int shiftSize)
{

	if (_matType.compare(ANY) && _matType.compare(ARROW_HEAD) && _matType.compare(TOEPLITZ)) // check if the matrix is initialized
	{
		cout << "NA - The matrix should be initialized first" << endl;
		return;
	}

	if (shiftSize == 0 || shiftSize%_colSize == 0 || (-1 * shiftSize) % _colSize == 0)   //in those cases the matrix shifts don't change the order of the cols
		return;

	SMatrix temp_mat(_rowSize, _colSize, _matType);  //make the new mat, to make the changes on it

	int i, j;
	MNode* col_ptr = NULL;
	for (j = 0; j < this->_colSize; j++)
	{
		col_ptr = _colHead[j];
		for (i = 0; i < this->_rowSize; i++)
		{
			if (IsExist(i, j))        //if there is no element in the (i,j) place, no need to shift (temp_mat initialized to be zero mat, so the wanted place after shift is zero anyway)
			{
				if (shiftSize > 0)
				{
					int k = (j + shiftSize) % _colSize;                             // k is the colIndex that the data is need to be shifted
						temp_mat.setElement_no_break_check(i, k, col_ptr->_data);   //put the element in it's place in the temp_mat
						col_ptr = col_ptr->_nextCol;
				}
				else       //(shiftSize < 0)
				{
					int k = (((_colSize - ((shiftSize*-1) % _colSize)) + j) % _colSize);   // k is the colIndex that the data is need to be shifted
					temp_mat.setElement_no_break_check(i, k, col_ptr->_data);              //put the element in it's place in the temp_mat
					col_ptr = col_ptr->_nextCol;
				}
			}
		}
	}

	/*check if temp_mat is from the same matType the SMatrix was before the shift*/
	if (temp_mat.isA(_matType) == false)       //if return false, do not do the shifts
	{
		cout << "NA" << endl;
	}
	else
	{
		this->operator= (temp_mat);       //if return true, copy temp_mat to the original object by operator=
	}
}

//calculate the size of the object SMatrix by bytes
void SMatrix::sizeInBytes()
{
	int sizeInBytes =  sizeof(SMatrix) + _rowSize * sizeof(_rowHead[0]) + _colSize * sizeof(_colHead[0]) + _elemNum * sizeof(MNode);
	cout << sizeInBytes << endl;
}


// operator + for SMatrix
SMatrix SMatrix::operator+(SMatrix &other)
{
	if (_matType.compare(ANY) && _matType.compare(ARROW_HEAD) && _matType.compare(TOEPLITZ)) // check if the matrix is initialized
	{
		cout << "NA - The matrix should be initialized first" << endl;
		exit(1);
	}

	if (other._rowSize != this->_rowSize || other._colSize != this->_colSize)     //the matrices have different size, so the add function cannot be done
	{
		cout << "NA: try to add 2 matrices on different sizes" << endl;
		exit(1);
	}

	if (other._matType != this->_matType)        //cannot add 2 matrices from different 'matType'
	{
		cout << "NA: cannot add 2 matrices from different matType " << endl;
		exit(1);
	}
	if (!isA(other._matType))        //the 'other' matrix is not really from the matType it says it is.
	{
		cout << "NA" << endl;
		exit(1);
	}

	int i, j;
	for (i = 0; i < _rowSize; i++)
	{
		MNode* this_ptr = _rowHead[i];
		MNode* other_ptr = other._rowHead[i];
		j = 0;
		while(this_ptr != NULL || other_ptr != NULL && other_ptr->_indexJ < _colSize)    //do the loop as long as there are elements in one of the both matrices row at least
		{
			if (this->IsExist(i, j) && other.IsExist(i, j))           //in both matrices there are elements in the (i,j) place, so sum them up
			{
				double data = this->getElement(i, j) + other.getElement(i, j);
				this->setElement(i, j, data);
			}
			if(this->IsExist(i,j))
			   this_ptr = this_ptr->_nextRow;

			if (!this->IsExist(i, j) && other.IsExist(i, j))   //there is element only in the 'other' matrix, so put that element in the 'this' matrix
			{
				double data = other.getElement(i, j);
				this->setElement(i, j, data);
			}
            
			if(other.IsExist(i,j))
			   other_ptr = other_ptr->_nextRow;

			j++;
		}
	}
	return *this;
}

// operator = for SMatrix
SMatrix& SMatrix::operator=(SMatrix &other)
{
	if (this != &other)
	{
		deleteNodes();
		initialize(other._rowSize, other._colSize, other._matType);
		this->_elemNum = other._elemNum;
		copy(other);
	}
		return *this;
}

//delete all nodes inside linked lists
void SMatrix::deleteNodes()
{
	for (int i = 0; i < _rowSize; i++)
	{
		MNode* p = _rowHead[i], *p2;
		while (p)
		{
			p2 = p;            //for not losing the next MNodes in the linked list
			p = p->_nextRow;
			delete p2;
		}
	}
}

// print rows as linked lists
void SMatrix:: printRowsIndexes()
{
	for (int i = 0; i < _rowSize; i++)
	{
		MNode* ptr = _rowHead[i];
		cout << i << ": ";
		while (ptr != NULL)
		{
			cout << "(" << ptr->_indexJ << "," << ptr->_data << ")->";
			ptr = ptr->_nextRow;
		}
		cout << endl;
	}
}

// print columns as linked lists
void SMatrix:: printColumnsIndexes()
{
		for (int j = 0; j < _colSize; j++)
		{
			MNode* ptr = _colHead[j];
			cout << j << ": ";
			while (ptr != NULL)
			{
				cout << "(" << ptr->_indexI << "," << ptr->_data << ")->";
				ptr = ptr->_nextCol;
			}
			cout << endl;
		}
}


// destroy this matrix. 
SMatrix::~SMatrix()
{
	if (_elemNum != 0)
	{
		//delete all nodes inside linked lists
		deleteNodes();
	}
	//delete the arrays of linked lists
	delete[] _colHead;
	delete[] _rowHead;
}


// print zero value 'num' times
void printZero(int num1, int num2)
{
	if (num1 != num2)   //means the data '0' is not the last in the row
	{
		for (int i = 0; i < num1; i++)
			cout << "0,";
	}
	else               //the data '0' is going to be the last in the row
	{
		for (int i = 0; i < num1; i++)
		{
			if (i == num1-1)          //the last zero that needs to be printed, so print it without a comma
				cout << "0";
			else
			    cout << "0,";        //not the last zero in the row yet, so print with a comma
		}
	}
}

// print operator for SMatrix class
ostream& operator<<(ostream& os, const SMatrix& mat)
{
	int curIndex, lastIndex;
	for (int i = 0; i < mat._rowSize; i++)
	{
		lastIndex = -1;
		MNode* p = mat._rowHead[i];
		while (p)
		{
			curIndex = p->_indexJ;
			printZero(curIndex - lastIndex - 1, -1);
			if (p->_indexJ+1 == mat._colSize)       //the last J index in the row is now, so print with a comma
				os << p->_data;
			else
			    os << p->_data << ",";              //not the last J index, so print with a comma
			lastIndex = p->_indexJ;
			p = p->_nextRow;
		}
		printZero((mat._colSize - lastIndex - 1), (mat._colSize - lastIndex - 1));

		os << endl;
	}
	return os;
}