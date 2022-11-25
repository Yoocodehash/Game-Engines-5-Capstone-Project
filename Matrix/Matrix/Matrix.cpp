#include "pch.h"
#include "Matrix.h"

MATRIX_API Matrix operator+(const Matrix& m1, const Matrix& m2)
{
	if (m1.rows != m2.rows || m1.columns != m2.columns) { DimensionError(); }

	Matrix matrixResult(m1);
	matrixResult += m2;

	return matrixResult;
}

MATRIX_API Matrix operator-(const Matrix& m1, const Matrix& m2)
{
	if (m1.rows != m2.rows || m1.columns != m2.columns) { DimensionError(); }

	Matrix matrixResult(m1);
	matrixResult -= m2;

	return matrixResult;
}

MATRIX_API Matrix operator*(const Matrix& m, const short int& value)
{
	Matrix matrixResult(m);
	matrixResult *= value;

	return matrixResult;
}

MATRIX_API Matrix operator*(const short int& value, const Matrix& m)
{
	return Matrix(m) * value;
}

MATRIX_API std::istream& operator >> (std::istream& in, Matrix& matrix_)
{
	size_t rows;
	size_t columns;

	in >> rows >> columns;

	if (rows <= 0 && columns <= 0) { DimensionError(); }

	if (matrix_.rows != rows || matrix_.columns != columns)
	{
		matrix_ = Matrix(rows, columns);
	}

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			in >> matrix_.matrix[i][j];
		}
	}

	return in;
}

MATRIX_API std::ostream& operator<<(std::ostream& out, const Matrix& matrix_)
{
	size_t rows = matrix_.rows;
	size_t columns = matrix_.columns;

	out << rows << ' ' << columns << '\n';

	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < columns; j++)
		{
			out << matrix_.matrix[i][j];

			if (j != columns - 1) out << ' ';
		}
		if (i != rows - 1) out << '\n';
	}

	return out;
}

Matrix::Matrix()
{
	columns = 0;
	rows = 0;
	matrix = 0;
}

Matrix::Matrix(size_t rows, size_t columns) :rows(rows), columns(columns)
{
	if (rows <= 0 && columns <= 0) { DimensionError(); }

	matrix = new short int* [rows];

	for (size_t i = 0; i < rows; i++)
	{
		matrix[i] = new short int[columns];
	}
}

Matrix::Matrix(std::initializer_list<std::initializer_list<short int>> list)
{
	this->rows = list.size();
	size_t columns = (*list.begin()).size();

	for (std::initializer_list<std::initializer_list<short int>>::iterator it = list.begin(); it != list.end(); it++)
	{
		if ((*it).size() != columns) { DimensionError(); }
	}

	this->columns = columns;

	size_t i = -1;

	matrix = new short int* [this->rows];

	for (std::initializer_list<short int> columns_list : list)
	{
		size_t j = 0;

		matrix[++i] = new short int[this->columns];

		for (short int value : columns_list)
		{
			matrix[i][j++] = value;
		}
	}
}

Matrix::Matrix(const Matrix& matrix_) :Matrix(matrix_.rows, matrix_.columns)
{
	for (size_t i = 0; i < this->rows; i++)
	{
		for (size_t j = 0; j < this->columns; j++)
		{
			this->matrix[i][j] = matrix_.matrix[i][j];
		}
	}
}

Matrix::~Matrix()
{
	for (size_t i = 0; i < rows; i++)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}

size_t Matrix::getRows() const
{
	return rows;
}

void Matrix::setRows(const size_t& rows)
{
	this->rows = rows;
}

size_t Matrix::getColumns() const
{
	return columns;
}

void Matrix::setColumns(const size_t& columns)
{
	this->columns = columns;
}

short int*& Matrix::operator[](const size_t& index) const
{
	return matrix[index];
}

Matrix& Matrix::operator=(const Matrix& matrix_)
{
	if (this != &matrix_)
	{
		this->rows = matrix_.rows;
		this->columns = matrix_.columns;

		this->matrix = new short int* [this->rows];

		for (size_t i = 0; i < this->rows; i++)
		{
			this->matrix[i] = new short int[columns];

			for (size_t j = 0; j < this->columns; j++)
			{
				this->matrix[i][j] = matrix_.matrix[i][j];
			}
		}
	}

	return *this;
}

bool Matrix::operator==(const Matrix& matrix_) const
{
	if (this->rows != matrix_.rows || this->columns != matrix_.columns) return false;

	for (size_t i = 0; i < this->rows; i++)
	{
		for (size_t j = 0; j < this->columns; j++)
		{
			if (this->matrix[i][j] != matrix_.matrix[i][j])
				return false;
		}
	}

	return true;
}

bool Matrix::operator!=(const Matrix& matrix) const
{
	return !(*this == matrix);
}

Matrix& Matrix::operator+=(const Matrix& matrix_)
{
	if (this->rows != matrix_.rows || this->columns != matrix_.columns) { DimensionError(); }

	for (size_t i = 0; i < this->rows; i++)
	{
		for (size_t j = 0; j < this->columns; j++)
		{
			this->matrix[i][j] += matrix_.matrix[i][j];
		}
	}

	return *this;
}

Matrix& Matrix::operator-=(const Matrix& matrix_)
{
	if (this->rows != matrix_.rows || this->columns != matrix_.columns) { DimensionError(); }

	for (size_t i = 0; i < this->rows; i++)
	{
		for (size_t j = 0; j < this->columns; j++)
		{
			this->matrix[i][j] -= matrix_.matrix[i][j];
		}
	}

	return *this;
}

Matrix& Matrix::operator*=(const short int& value)
{
	for (size_t i = 0; i < this->rows; i++)
	{
		for (size_t j = 0; j < this->columns; j++)
		{
			this->matrix[i][j] *= value;
		}
	}

	return *this;
}