#pragma once

#include <windows.h>
#include <fstream>
#include <sdkddkver.h>

#ifdef MATRIX_EXPORTS
#define MATRIX_API __declspec(dllexport)
#else
#define MATRIX_API __declspec(dllimport)
#endif

struct MATRIX_API DimensionError
{
	char message[80];
	DimensionError() { strcpy_s(message, "Bad dimensions"); }
};

// This class is exported from the Matrix.dll
class MATRIX_API Matrix {
private:
	size_t rows;
	size_t columns;
	short int** matrix;

	Matrix(void);
public:
	Matrix(size_t rows, size_t columns);
	Matrix(std::initializer_list<std::initializer_list<short int>> list);
	Matrix(const Matrix& matrix);
	virtual ~Matrix();
	// TODO: add your methods here.

	size_t getRows() const;
	void setRows(const size_t& rows);

	size_t getColumns() const;
	void setColumns(const size_t& columns);

	short int*& operator[] (const size_t& index) const;

	Matrix& operator=(const Matrix& matrix);

	bool operator==(const Matrix& matrix) const;
	bool operator!=(const Matrix& matrix) const;

	Matrix& operator+=(const Matrix& matrix);
	Matrix& operator-=(const Matrix& matrix);

	Matrix& operator*=(const short int& value);

	friend MATRIX_API Matrix operator+(const Matrix& m1, const Matrix& m2);
	friend MATRIX_API Matrix operator-(const Matrix& m1, const Matrix& m2);

	friend MATRIX_API Matrix operator*(const Matrix& m, const short int& value);
	friend MATRIX_API Matrix operator*(const short int& value, const Matrix& m);

	friend MATRIX_API std::istream& operator>>(std::istream& in, Matrix& matrix);
	friend MATRIX_API std::ostream& operator<<(std::ostream& out, const Matrix& matrix);
};