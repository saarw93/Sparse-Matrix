Name: Saar Weitzman
ID: 204175137

This program makes sparse matrices. It does that by the class SMatrix and class MNode. Each sparse matrix being build by the number of rows * number of columns
the user put as an input, and also has a type the user input. The sparse matrix use memory only for the elements in it which are not zero. Each place in the
sparse matrix is allocate by the MNode class. The SMatrix is build from 2 arrays, 1 for the rows and 1 for the columns. The MNodes are connected as a linked list
to both arrays. Each sparse matrix the user makes is initialized to be zeroes matrix at the start (means the pointers in the rows and cols arrays are pointing to NULL)).
The matrix type can be 1 of 3 types actually: "Arrowhead", "Toeplitz" or "any". Each one of the types is different, and has certain rules that the matrix has to handle.
There are several methods which can be performed on the sparse matrices, and they are programmed to check that every change which is being performed on the sparse
matrix does not break it's type. If a method break the matrix type, the program will not change the sparse matrix by it. Those methods are basic methods which can be done
on matrices, like shifting rows, shifting columns, sum matrices (the operator+ method) etc. There are also some private methods which were made to helps the
public methods do the work.

As I said at the start, the user construct the sparse matrix he wants by using the class SMatrix, and calling the constructor that gets 3 varaiables (rows, cols, matType).
The rows and cols (columns) are integers (cannot be a negative number ofcourse) and the matType (sparse matrix type) is a string (one of the 3 types).
There is also a copy constructor if the user wants to make a copy of an existing constructor. By those ways the user is able to make his sparse matrix and can activate
the sparse matrix methods on it, in condition the method does not break the sparse matrix type, which in that case the change will not happen. 


Program SMatrix:
Contains class SMatrix (in SMatrix.h) that has in it the signatures of the public and the private methods.
There are 6 varaiables of class SMatrix: _rowHead- array of pointer to MNodes by the rows, _colHead- array of pointer to MNodes by the cols,
_rowSize- the smatrix number of rows, _colSize- the smatrix number of columns, _matType- the type of the smatrix, _elemNum- number of elements
(MNodes in the smatrix).
The program also contains the class MNode (in SMatrix.h) that has the constructor and the private varaiables of the class.
it has 5 varaiables in it: _data- number of type double, _indexI- index i of the element, indexJ- index j of the element, _nextRow- pointer to
the next columns in the row, _nextCol- pointer to the next rows in the column.

Functions:
public:
1. SMatrix(int rows, int cols, string matType) = construct a 'row * cols smatrix
2. SMatrix(const SMatrix& other) = copy constructor
3. setElement =  set the input 'data' to be in the [i,j] place of the smatrix, in case it's not ruin the smatrix 'matType.
4. getElement = returns the 'data' in the [i,j] place of the smatrix.
5. operator+ = sum 2 sparse matrices.
6. isA = checks if a smatrix is from the matType it gets as an input.
7. rowShift = shift the rows of the smatrix "shiftsize" times. 
8. colShift = shift the cols of the smatrix "shiftsize" times.
9. friend std::ostream& operator<< = prints the smatrix on the screen.
10. operator= = change one smatrix to be another smatrix.
11. sizeInBytes = get the size of the smatrix in bytes.
12. printRowsIndexes = print the rows indexes and data as a linked list.
13. printColsIndexes = print the cols indexes and data as a linked list.
14. ~SMatrix = destructor of the smatrix made by the constructor.
private:
15. isArrowHead = check if the smatrix is 'Arrowhead' type.
16. is_diagonal_equal =  gets the start indexes of a diagonal in the smatrix, and checks if the 'data' in it is equal.
17. isToepplitz = check if the smatrix is 'Toeplitz' type.
18. setValue = set a value in the smatrix.
19. removeElement = remove the wanted element
20. insertNode = insert the wanted MNode to the smatrix.
21. IsExit = checks if the [i,j] place is exist (means in bounds of the smatrix and the 'data' is not 0).
22. initialize = initialize the varaiables of the smatrix.
23. deleteNodes = delete all the MNoeds in the smatrix.
24. setElement_no_break_check = makes the set of the element, without checking if the matType was breaked by it (especially for the shifts).
25. copy = copy the MNodes of one smatrix to another.

Program files:
SMatrix.cpp- the file contains all the implementations of the function of the class SMatrix.
SMatrix.h- an header file ,contains the class SMatrix with it's varaiables and declerations of functions in the class. Contains the
class MNode with the constructor and the varaiables.

Input:
1. Call the constructor with 3 varaiables (rows, cols, matType)
2. activate a method on the SMatrix the user construct ( for example, the user construct SMatrix mat(3,3,"Arrowhead"),
can activate rowShift method by doing mat.rowShift()).

Output:
For operator<<: the smatrix itself.
For isA(matType): true\false 
For getElement: the 'data' in the wanted index of the matrix.
For sizeInBytes: the size of the matrix in bytes.
For printRowsIndexes: linked list of the rows.
For printColsIndexes: linked list of the cols
If there are errors of input:
For SMatrix (if row and/or cols are negative): NA: the matrice cannot have a negative size of rows and/or cols.
For getElement: NA: getElement out of range.
For setElement: NA: setElement out of range.
For invalid matType: NA: Undefined matType, please enter valid type of matrix.
For not square smatrix to construct "Arrowhead" matrix: Invalid input: Arrowhead matrix must have equal rows and cols.
For indexes of the smatrix out of bounds: NA.
For trying activate a method on not initialized smatrix: NA - The matrix should be initialized first.
For trying put 'data' in not available index (out of bounds): NA.
For triying to add 2 smatrices with different sizes: NA: cannot add 2 matrices from different matType.
