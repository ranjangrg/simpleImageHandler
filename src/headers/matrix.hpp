#ifndef __MATRIX_H
#define __MATRIX_H

#include <iostream>
#include <iomanip>	// for setw()
#include <vector>
#include <exception>
#include <initializer_list>
#include <limits>	// to check overflows

#define USE_S_INT_MATRIX
#define USE_U_INT_MATRIX
#define USE_U_CHAR_MATRIX

// defining struct for a Matrix object/instance
namespace Matrix {
	template <typename T>
	struct MatrixStruct{
		std::size_t nRows, nCols;
		std::vector<T> elements;

		// operator symbol overload/overriding
		T& operator () (const size_t& rowIdx, const size_t& colIdx) const;
		MatrixStruct<T> operator + (const MatrixStruct<T>& rhs) const;
		MatrixStruct<T> operator - (const MatrixStruct<T>& rhs) const;
		MatrixStruct<T> operator * (const T& scalar) const;
		MatrixStruct<T> operator * (const MatrixStruct<T>& rhs) const;
		bool operator == (const MatrixStruct<T>& rhs) const;
	};

}

// matrix struct operations/methods
namespace Matrix {
	template <typename T>
	MatrixStruct<T> createMatrix(
		const std::size_t& nRows, 
		const std::size_t& nCols);

	template <typename T>
	MatrixStruct<T> createMatrix(
		const std::size_t& nRows, 
		const std::size_t& nCols, 
		const std::vector<T>& elements);

	template <typename T>
	MatrixStruct<T> createMatrix(
		const std::size_t& nRows, 
		const std::size_t& nCols, 
		const std::initializer_list<T>& elements);

	// getter methods
	template <typename T>
	std::vector<T> getRow(const MatrixStruct<T>& matrix, const size_t& rowIdx);
	template <typename T>
	std::vector<T> getCol(const MatrixStruct<T>& matrix, const size_t& colIdx);

	template <typename T, typename U>
	MatrixStruct<T> convoluteMatrixUsingKernel(
		const MatrixStruct<T>& mainMatrix,
		const MatrixStruct<U>& kernelMatrix);
		
	template <typename T>
	MatrixStruct<T> transposeMatrix(const MatrixStruct<T>& matrix);

	template <typename T>
	void dumpMatrixInfo(const MatrixStruct<T>& matrix, const size_t& printWidth = 3, const bool& castToInt = false);

	// overflow check for (num1 + num2); 
	// num1's data type must have larger max/min limit than num2
	template <typename T, typename U>
	bool checkAdditionOverflow(const T& numWithLargerDatatype, const U& numOfSmallerDatatype);
	template <typename T, typename U>
	bool checkMultiplicationOverflow(const T& numWithLargerDatatype, const U& numOfSmallerDatatype);

}

// matrix exceptions
namespace Matrix {
	namespace Exception {
		struct dimensionAndElementsMismatchException : public std::exception {
			const char* what() const throw () {
				return "[ERR] [Matrix] n(rows) * n(columns) != n(elements).";
			}
		};
		struct dimensionMismatchException : public std::exception {
			const char* what() const throw () {
				return "[ERR] [Matrix] Dimensions do not match among the matrices.";
			}
		};
		struct dataOverflowDuringAdditionException : public std::exception {
			const char* what() const throw () {
				return "[ERR] [Matrix] Data value overflow/underflow detected during addition.";
			}
		};
		struct dataOverflowDuringMultiplicationException : public std::exception {
			const char* what() const throw () {
				return "[ERR] [Matrix] Data value overflow/underflow detected during multiplication.";
			}
		};
		struct dimensionMismatchForMultiplicationException : public std::exception {
			const char* what() const throw () {
				return "[ERR] [Matrix] Matrix dimensions not suitable for multiplication.";
			}
		};
	}
}


#endif