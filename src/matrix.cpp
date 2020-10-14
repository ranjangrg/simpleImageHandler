#include "./headers/matrix.hpp"

template <typename T>
T& Matrix::MatrixStruct<T>::operator () (const size_t& rowIdx, const size_t& colIdx) const {
	T value = this->elements.at(rowIdx * this->nCols + colIdx);
	T& valueRef = value;
	return valueRef;
}

template <typename T>
Matrix::MatrixStruct<T> Matrix::MatrixStruct<T>::operator + (const Matrix::MatrixStruct<T>& rhsMatrix) const {
	const Matrix::MatrixStruct<T>* lhs = this;
	const Matrix::MatrixStruct<T>* rhs = &rhsMatrix;
	if ( (lhs->nCols != rhs->nCols) || (lhs->nRows != rhs->nRows) ) {
		throw Matrix::Exception::dimensionMismatchException();
	}
	Matrix::MatrixStruct<T> sumMatrix = Matrix::createMatrix<T>(this->nRows, this->nRows);
	for (size_t idx = 0; idx < sumMatrix.elements.size(); ++idx) {
		// !IMP: better to check overflow here
		if ( Matrix::checkAdditionOverflow(lhs->elements.at(idx), rhs->elements.at(idx)) ) {
			throw Matrix::Exception::dataOverflowDuringAdditionException();
		}
		sumMatrix.elements.at(idx) = lhs->elements.at(idx) + rhs->elements.at(idx);
	}
	return sumMatrix;
}

template <typename T>
Matrix::MatrixStruct<T> Matrix::MatrixStruct<T>::operator - (const Matrix::MatrixStruct<T>& rhsMatrix) const {
	const Matrix::MatrixStruct<T>* lhs = this;
	const Matrix::MatrixStruct<T>* rhs = &rhsMatrix;
	if ( (lhs->nCols != rhs->nCols) || (lhs->nRows != rhs->nRows) ) {
		throw Matrix::Exception::dimensionMismatchException();
	}
	Matrix::MatrixStruct<T> sumMatrix = Matrix::createMatrix<T>(this->nRows, this->nRows);
	for (size_t idx = 0; idx < sumMatrix.elements.size(); ++idx) {
		// !IMP: better to check overflow here
		if ( Matrix::checkAdditionOverflow(lhs->elements.at(idx), (-1) * rhs->elements.at(idx)) ) {
			throw Matrix::Exception::dataOverflowDuringAdditionException();
		}
		sumMatrix.elements.at(idx) = lhs->elements.at(idx) - rhs->elements.at(idx);
	}
	return sumMatrix;
}

template <typename T>
Matrix::MatrixStruct<T> Matrix::MatrixStruct<T>::operator * (const T& scalar) const {
	const Matrix::MatrixStruct<T>* lhs = this;
	Matrix::MatrixStruct<T> resultMatrix = Matrix::createMatrix<T>(this->nRows, this->nRows);
	for (size_t idx = 0; idx < resultMatrix.elements.size(); ++idx) {
		// TODO:
		// !IMP: better to check overflow here: lhs->elements.at(idx) * scalar
		resultMatrix.elements.at(idx) = lhs->elements.at(idx) * scalar;
	}
	return resultMatrix;
}

template <typename T>
Matrix::MatrixStruct<T> Matrix::MatrixStruct<T>::operator * (const Matrix::MatrixStruct<T>& rhsMatrix) const {
	const Matrix::MatrixStruct<T>* lhsMatrix = this;
	// check if matrices can actually multiply with each other i.e. (m x n) x (n x p)
	if ( lhsMatrix->nCols != rhsMatrix.nRows ) {
		throw Exception::dimensionMismatchForMultiplicationException();
	}
	Matrix::MatrixStruct<T> resultMatrix = Matrix::createMatrix<T>(lhsMatrix->nRows, rhsMatrix.nCols);
	T value;
	std::vector<T> currRow, currCol;

	for (size_t resultRowIdx = 0; resultRowIdx < resultMatrix.nRows; ++resultRowIdx) {
		for (size_t resultColIdx = 0; resultColIdx < resultMatrix.nCols; ++resultColIdx) { 
			value = T();
			currRow = getRow(*lhsMatrix, resultRowIdx);
			currCol = getCol(rhsMatrix, resultColIdx);
			// calculate dot product of lhs's resultRowIdx row and rhs's resultColIdx col
			for (size_t currIdx = 0; currIdx < currRow.size(); ++currIdx) {
				// check for possible overflows:
				// overflow check for (currRow.at(currIdx) * currCol.at(currIdx)) AND/OR
				// overflow check for (value + (currRow.at(currIdx) * currCol.at(currIdx))): addition
				if ( Matrix::checkMultiplicationOverflow(currRow.at(currIdx), currCol.at(currIdx)) ) {
					throw Matrix::Exception::dataOverflowDuringMultiplicationException();
				}
				if ( Matrix::checkAdditionOverflow(value, (currRow.at(currIdx) * currCol.at(currIdx))) ) {
					throw Matrix::Exception::dataOverflowDuringAdditionException();
				}
				value += currRow.at(currIdx) * currCol.at(currIdx);
			}
			resultMatrix.elements.at(resultRowIdx * resultMatrix.nCols + resultColIdx) = value;
		}
	}

	return resultMatrix;
}

template <typename T>
bool Matrix::MatrixStruct<T>::operator == (const Matrix::MatrixStruct<T>& rhsMatrix) const {
	bool areSame = false;
	const Matrix::MatrixStruct<T>* lhs = this;
	const Matrix::MatrixStruct<T>* rhs = &rhsMatrix;
	if ( (lhs->nCols == rhs->nCols) && (lhs->nRows == rhs->nRows) ) {
		areSame = true;
		for (size_t idx = 0; (idx < lhs->elements.size()) && areSame; ++idx) {
			areSame = lhs->elements.at(idx) == rhs->elements.at(idx);
		}
	}
	return areSame;
}

template <typename T>
Matrix::MatrixStruct<T> Matrix::createMatrix(const std::size_t& nRows, const std::size_t& nCols) {
	Matrix::MatrixStruct<T> matrix;
	matrix.nRows = nRows;
	matrix.nCols = nCols;
	matrix.elements = std::vector<T> (nRows * nCols, T());	// default val = constructor of each T class
	return matrix;
}

template <typename T>
Matrix::MatrixStruct<T> Matrix::createMatrix(
		const std::size_t& nRows, 
		const std::size_t& nCols, 
		const std::vector<T>& elements
	) {
	if (elements.size() != (nRows * nCols)) {
		throw Matrix::Exception::dimensionAndElementsMismatchException();
	}	
	Matrix::MatrixStruct<T> matrix;
	matrix.nRows = nRows;
	matrix.nCols = nCols;
	matrix.elements = elements;
	return matrix;
}

template <typename T>
Matrix::MatrixStruct<T> Matrix::createMatrix(
		const std::size_t& nRows, 
		const std::size_t& nCols, 
		const std::initializer_list<T>& elements
	) {
	if (elements.size() != (nRows * nCols)) {
		throw Matrix::Exception::dimensionAndElementsMismatchException();
	}	
	Matrix::MatrixStruct<T> matrix;
	std::vector<T> elementsAsVector = elements;
	matrix = Matrix::createMatrix<T>(nRows, nCols, elementsAsVector);
	return matrix;
}

template <typename T>
std::vector<T> Matrix::getRow(const Matrix::MatrixStruct<T>& matrix, const size_t& rowIdx) {
	size_t startIdxOffset = rowIdx * matrix.nCols;
	size_t endIdxOffset = startIdxOffset + matrix.nCols;
	std::vector<T> row(matrix.elements.begin() + startIdxOffset, matrix.elements.begin() + endIdxOffset);
	return row;
}

template <typename T>
std::vector<T> Matrix::getCol(const Matrix::MatrixStruct<T>& matrix, const size_t& colIdx) {
	std::vector<T> col(matrix.nRows);
	for (size_t rowIdx = 0; rowIdx < matrix.nRows; ++rowIdx) {
		col.at(rowIdx) = matrix.elements.at(rowIdx * matrix.nCols + colIdx);
	}
	return col;
}

template <typename T, typename U>
Matrix::MatrixStruct<T> Matrix::convoluteMatrixUsingKernel(
	const Matrix::MatrixStruct<T>& mainMatrix,
	const Matrix::MatrixStruct<U>& kernelMatrix
	) {

	bool ignoreOverflow = true;

	Matrix::MatrixStruct<T> resultantMatrix = Matrix::createMatrix<T>(mainMatrix.nRows, mainMatrix.nCols);
	// variable declarations
	T value, valueAtMat, valueAtKernel;
	signed int rowOffset, colOffset, currRowIdx, currColIdx;

	for (unsigned int mainRowIdx = 0; mainRowIdx < mainMatrix.nRows; ++mainRowIdx) {
		for (unsigned int mainColIdx = 0; mainColIdx < mainMatrix.nCols; ++mainColIdx) {
			// loop through the kernel now
			value = T();
			for (unsigned int kernelRowIdx = 0; kernelRowIdx < kernelMatrix.nRows; ++kernelRowIdx) {
				for (unsigned int kernelColIdx = 0; kernelColIdx < kernelMatrix.nCols; ++kernelColIdx) {
					rowOffset = (-1 * int((kernelMatrix.nRows + 1) / 2)) + 1;
					colOffset = (-1 * int((kernelMatrix.nCols + 1) / 2)) + 1;
					valueAtKernel = kernelMatrix.elements[kernelRowIdx * kernelMatrix.nCols + kernelColIdx];
					currRowIdx = mainRowIdx + kernelRowIdx + rowOffset;
					currColIdx = mainColIdx + kernelColIdx + colOffset;
					if ( 
						(currRowIdx >= 0) && (currColIdx >= 0) &&
						(currRowIdx < mainMatrix.nRows) && (currColIdx < mainMatrix.nCols)
					) {
						// proceed only if row/col indices are valid(+ve) and within range
						valueAtMat = mainMatrix.elements[(currRowIdx * mainMatrix.nCols) + currColIdx];
						
						if (!ignoreOverflow) {
							// check for possible overflows:
							// overflow check for (valueAtMat * valueAtKernel) AND/OR
							// overflow check for (value + (valueAtMat * valueAtKernel)): addition
							if ( Matrix::checkMultiplicationOverflow(valueAtMat, valueAtKernel) ) {
								throw Matrix::Exception::dataOverflowDuringMultiplicationException();
							}
							if ( Matrix::checkAdditionOverflow(value, valueAtMat * valueAtKernel) ) {
								throw Matrix::Exception::dataOverflowDuringAdditionException();
							}
						}
						value = value + (valueAtMat * valueAtKernel);
					}
				}
			}
			resultantMatrix.elements[mainRowIdx * mainMatrix.nCols + mainColIdx] = value;
		}
	}
	return resultantMatrix;
}

template <typename T>
Matrix::MatrixStruct<T> Matrix::transposeMatrix(const Matrix::MatrixStruct<T>& matrix) {
	Matrix::MatrixStruct<T> transposed = Matrix::createMatrix<T>(matrix.nCols, matrix.nRows);
	size_t origIdx, transIdx;
	for (size_t rowIdx = 0; rowIdx < matrix.nRows; ++rowIdx) {
		for (size_t colIdx = 0; colIdx < matrix.nCols; ++colIdx) {
			origIdx = rowIdx * matrix.nCols + colIdx;
			transIdx = colIdx * transposed.nCols + rowIdx;
			transposed.elements.at(transIdx) = matrix.elements.at(origIdx);
		}
	}
	return transposed;
}

template <typename T>
void Matrix::dumpMatrixInfo(const Matrix::MatrixStruct<T>& matrix, const size_t& printWidth, const bool& castToInt) {
	for (size_t rowIdx = 0; rowIdx < matrix.nRows; ++rowIdx) {
		for (size_t colIdx = 0; colIdx < matrix.nCols; ++colIdx) {
			if (castToInt) {
				std::cout << std::setw(printWidth) << int(matrix.elements.at(rowIdx * matrix.nCols + colIdx)) << ' ';
			} else {
				std::cout << std::setw(printWidth) << matrix.elements.at(rowIdx * matrix.nCols + colIdx) << ' ';
			}
		}
		std::cout << std::endl;
	}
}

template <typename T, typename U>
bool Matrix::checkAdditionOverflow(const T& numWithLargerDatatype, const U& numOfSmallerDatatype) {
	// assuming T has bigger limits (max and min)
	T maxDatatypeValue = std::numeric_limits<T>::max();
	T minDatatypeValue = std::numeric_limits<T>::min();
	
	bool overflowDetected = (
		(
			( numWithLargerDatatype > T() ) &&
			( numOfSmallerDatatype > (maxDatatypeValue - numWithLargerDatatype) )
		) || (
			( numWithLargerDatatype < T() ) &&
			( numOfSmallerDatatype < (minDatatypeValue - numWithLargerDatatype) )
		)
	);
	return overflowDetected;
}

template <typename T, typename U>
bool Matrix::checkMultiplicationOverflow(const T& numWithLargerDatatype, const U& numOfSmallerDatatype) {
	// assuming T has bigger limits (max and min)
	T maxDatatypeValue = std::numeric_limits<T>::max();
	T minDatatypeValue = std::numeric_limits<T>::min();
	
	bool overflowDetected = ( 
		(numWithLargerDatatype > T()) && (	// basically to prevent div by zero
			(numOfSmallerDatatype > maxDatatypeValue / numWithLargerDatatype) ||
			(numOfSmallerDatatype < minDatatypeValue / numWithLargerDatatype)
		)
	) || (
		(numWithLargerDatatype < T()) && (	// basically != 0; to prevent ??/0 below
			(numOfSmallerDatatype < maxDatatypeValue / numWithLargerDatatype) ||
			(numOfSmallerDatatype > minDatatypeValue / numWithLargerDatatype)
		)
	) || (
		(int(numOfSmallerDatatype) == -1) &&
		(numWithLargerDatatype == minDatatypeValue) 
	) || (
		(int(numWithLargerDatatype) == -1) &&
		(numOfSmallerDatatype == minDatatypeValue) 
	);
	/*
	if (overflowDetected) {
		std::cout << "Values: " << numWithLargerDatatype << " and " << numOfSmallerDatatype << std::endl;
	}
	*/
	return overflowDetected;
}

// explicit instantiations for templates
#ifdef USE_S_INT_MATRIX
template Matrix::MatrixStruct<int> Matrix::createMatrix<int>(const std::size_t&, const std::size_t&);
template Matrix::MatrixStruct<int> Matrix::createMatrix<int>(const std::size_t&, const std::size_t&, const std::vector<int>&);
template Matrix::MatrixStruct<int> Matrix::createMatrix<int>(const std::size_t&, const std::size_t&, const std::initializer_list<int>&);

template std::vector<int> Matrix::getRow(const Matrix::MatrixStruct<int>&, const size_t&);
template std::vector<int> Matrix::getCol(const Matrix::MatrixStruct<int>&, const size_t&);

template Matrix::MatrixStruct<int> Matrix::convoluteMatrixUsingKernel(const Matrix::MatrixStruct<int>&, const Matrix::MatrixStruct<int>&);
template Matrix::MatrixStruct<int> Matrix::transposeMatrix(const Matrix::MatrixStruct<int>&);

template int& Matrix::MatrixStruct<int>::operator () (const size_t& rowIdx, const size_t& colIdx) const;
template Matrix::MatrixStruct<int> Matrix::MatrixStruct<int>::operator + (const Matrix::MatrixStruct<int>& rhs) const;
template Matrix::MatrixStruct<int> Matrix::MatrixStruct<int>::operator - (const Matrix::MatrixStruct<int>& rhs) const;
template Matrix::MatrixStruct<int> Matrix::MatrixStruct<int>::operator * (const int& scalar) const;
template Matrix::MatrixStruct<int> Matrix::MatrixStruct<int>::operator * (const Matrix::MatrixStruct<int>& rhs) const;
template bool Matrix::MatrixStruct<int>::operator == (const Matrix::MatrixStruct<int>& rhs) const;

template void Matrix::dumpMatrixInfo<int>(const Matrix::MatrixStruct<int>&, const size_t&, const bool&);
template bool Matrix::checkAdditionOverflow(const int&, const int&);
template bool Matrix::checkMultiplicationOverflow(const int&, const int&);
#endif

#ifdef USE_U_INT_MATRIX
template Matrix::MatrixStruct<unsigned int> Matrix::createMatrix<unsigned int>(const std::size_t&, const std::size_t&);
template Matrix::MatrixStruct<unsigned int> Matrix::createMatrix<unsigned int>(const std::size_t&, const std::size_t&, const std::vector<unsigned int>&);
template Matrix::MatrixStruct<unsigned int> Matrix::createMatrix<unsigned int>(const std::size_t&, const std::size_t&, const std::initializer_list<unsigned int>&);

template std::vector<unsigned int> Matrix::getRow(const Matrix::MatrixStruct<unsigned int>&, const size_t&);
template std::vector<unsigned int> Matrix::getCol(const Matrix::MatrixStruct<unsigned int>&, const size_t&);

template Matrix::MatrixStruct<unsigned int> Matrix::convoluteMatrixUsingKernel(const Matrix::MatrixStruct<unsigned int>&, const Matrix::MatrixStruct<unsigned int>&);
template Matrix::MatrixStruct<unsigned int> Matrix::transposeMatrix(const Matrix::MatrixStruct<unsigned int>&);

template unsigned int& Matrix::MatrixStruct<unsigned int>::operator () (const size_t& rowIdx, const size_t& colIdx) const;
template Matrix::MatrixStruct<unsigned int> Matrix::MatrixStruct<unsigned int>::operator + (const Matrix::MatrixStruct<unsigned int>& rhs) const;
template Matrix::MatrixStruct<unsigned int> Matrix::MatrixStruct<unsigned int>::operator - (const Matrix::MatrixStruct<unsigned int>& rhs) const;
template Matrix::MatrixStruct<unsigned int> Matrix::MatrixStruct<unsigned int>::operator * (const unsigned int& scalar) const;
template Matrix::MatrixStruct<unsigned int> Matrix::MatrixStruct<unsigned int>::operator * (const Matrix::MatrixStruct<unsigned int>& rhs) const;
template bool Matrix::MatrixStruct<unsigned int>::operator == (const Matrix::MatrixStruct<unsigned int>& rhs) const;

template void Matrix::dumpMatrixInfo<unsigned int>(const Matrix::MatrixStruct<unsigned int>&, const size_t&, const bool&);
template bool Matrix::checkAdditionOverflow(const unsigned int&, const unsigned int&);
template bool Matrix::checkMultiplicationOverflow(const unsigned int&, const unsigned int&);
#endif

#ifdef USE_U_CHAR_MATRIX
template Matrix::MatrixStruct<unsigned char> Matrix::createMatrix<unsigned char>(const std::size_t&, const std::size_t&);
template Matrix::MatrixStruct<unsigned char> Matrix::createMatrix<unsigned char>(const std::size_t&, const std::size_t&, const std::vector<unsigned char>&);
template Matrix::MatrixStruct<unsigned char> Matrix::createMatrix<unsigned char>(const std::size_t&, const std::size_t&, const std::initializer_list<unsigned char>&);

template std::vector<unsigned char> Matrix::getRow(const Matrix::MatrixStruct<unsigned char>&, const size_t&);
template std::vector<unsigned char> Matrix::getCol(const Matrix::MatrixStruct<unsigned char>&, const size_t&);

template Matrix::MatrixStruct<unsigned char> Matrix::convoluteMatrixUsingKernel(const Matrix::MatrixStruct<unsigned char>&, const Matrix::MatrixStruct<unsigned char>&);
template Matrix::MatrixStruct<unsigned char> Matrix::transposeMatrix(const Matrix::MatrixStruct<unsigned char>&);

template unsigned char& Matrix::MatrixStruct<unsigned char>::operator () (const size_t& rowIdx, const size_t& colIdx) const;
template Matrix::MatrixStruct<unsigned char> Matrix::MatrixStruct<unsigned char>::operator + (const Matrix::MatrixStruct<unsigned char>& rhs) const;
template Matrix::MatrixStruct<unsigned char> Matrix::MatrixStruct<unsigned char>::operator - (const Matrix::MatrixStruct<unsigned char>& rhs) const;
template Matrix::MatrixStruct<unsigned char> Matrix::MatrixStruct<unsigned char>::operator * (const unsigned char& scalar) const;
template Matrix::MatrixStruct<unsigned char> Matrix::MatrixStruct<unsigned char>::operator * (const Matrix::MatrixStruct<unsigned char>& rhs) const;
template bool Matrix::MatrixStruct<unsigned char>::operator == (const Matrix::MatrixStruct<unsigned char>& rhs) const;

template void Matrix::dumpMatrixInfo<unsigned char>(const Matrix::MatrixStruct<unsigned char>&, const size_t&, const bool&);
template bool Matrix::checkAdditionOverflow(const unsigned char&, const unsigned char&);
template bool Matrix::checkMultiplicationOverflow(const unsigned char&, const unsigned char&);
#endif