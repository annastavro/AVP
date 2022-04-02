#pragma once
#include <stdlib.h>
#include <time.h>
#include <emmintrin.h>
#include <iostream>

using namespace std;

template <typename T>
class Matrix
{
private:
	int mainWidth;
	int mainHeight;
	int cellWidth;
	int cellHeight;
	T**** Pointer;
public:
	Matrix(int mainWidth, int mainHeight, int cellWidth, int cellHeight)
	{
		this->mainWidth = mainWidth;
		this->mainHeight = mainHeight;
		this->cellWidth = cellWidth;
		this->cellHeight = cellHeight;

		try {
			Pointer = new T * **[mainHeight];

			for (int i = 0; i < mainHeight; i++) {
				Pointer[i] = new T * *[mainWidth];
				for (int j = 0; j < mainWidth; j++) {
					Pointer[i][j] = new T *[cellHeight];
					for (int k = 0; k < cellHeight; k++) {
						Pointer[i][j][k] = new T[cellWidth];
						for (int m = 0; m < cellWidth; m++) {
							Pointer[i][j][k][m] = 0;
						}
					}
				}
			}
		}
		catch (...) {
			Pointer = nullptr;
		}
	}

	Matrix() {}

	Matrix<T> multiplyVectorized(Matrix<T>& matrixB) {
		if (this->mainWidth != matrixB.getMainMatrixHeight() || matrixB.getCellMatrixHeight() != this->cellWidth)
		{
			return matrixB;
		}

		Matrix<T> resultMatrix(this->mainHeight, matrixB.getMainMatrixWidth(), matrixB.getCellMatrixWidth(), this->cellHeight);

		for (int m = 0; m < this->mainHeight; m++) {
			for (int n = 0; n < this->mainWidth; n++) {
				for (int y = 0; y < matrixB.getMainMatrixWidth(); y++) {
					mulMatrixVectorized(matrixB, resultMatrix, m, n, y);
				}
			}
		}

		return resultMatrix;
	}

	Matrix<T> multiplySse(Matrix<T>& matrixB) {
		if (this->mainWidth != matrixB.getMainMatrixHeight()
			|| this->cellWidth != matrixB.getCellMatrixHeight())
		{
			return matrixB;
		}

		Matrix<T> resultMatrix(this->mainHeight, matrixB.getMainMatrixWidth(), matrixB.getCellMatrixWidth(), this->cellHeight);

		for (int m = 0; m < this->mainHeight; m++) {
			for (int n = 0; n < this->mainWidth; n++) {
				for (int y = 0; y < matrixB.getMainMatrixWidth(); y++) {
					mulSse(matrixB, resultMatrix, m, n, y);
				}
			}
		}

		return resultMatrix;
	}

	Matrix<T> multiplyNotVectorized(Matrix<T>& matrixB) {
		if (this->mainWidth != matrixB.getMainMatrixHeight()
			|| matrixB.getCellMatrixHeight() != this->cellWidth)
		{
			return matrixB;
		}

		Matrix<T> resultMatrix(this->mainHeight, matrixB.getMainMatrixWidth(), matrixB.getCellMatrixWidth(), this->cellHeight);

		for (int m = 0; m < this->mainHeight; m++) {
			for (int n = 0; n < this->mainWidth; n++) {
				for (int y = 0; y < matrixB.getMainMatrixWidth(); y++) {
					mulMatrixNotVectorized(matrixB, resultMatrix, m, n, y);
				}
			}
		}

		return resultMatrix;
	}
	Matrix<T> multiplyNotVectorizedCache(Matrix<T>& matrixB) {
		if (this->mainWidth != matrixB.getMainMatrixHeight()
			|| matrixB.getCellMatrixHeight() != this->cellWidth)
		{
			return matrixB;
		}

		Matrix<T> resultMatrix(this->mainHeight, matrixB.getMainMatrixWidth(), matrixB.getCellMatrixWidth(), this->cellHeight);
		int block_size = 32;
		for (int ii = 0; ii < this->mainHeight; ii += block_size) {
			for (int jj = 0; jj < this->mainWidth; jj += block_size) {
				for (int yy = 0; yy < matrixB.getMainMatrixWidth(); yy += block_size) {
					int i_start = ii;    // индекс i для блока принимает значения [ii, ii + block_size)
					int i_end = ii + block_size;
					int j_start = jj;    // индекс j для блока принимает значения [jj, jj + block_size)
					int j_end = jj + block_size;
					int y_start = yy;    // индекс j для блока принимает значения [jj, jj + block_size)
					int y_end = yy + block_size;

					for (int m = i_start; m < i_end; m++) {
						for (int n = j_start; n < j_end; n++) {
							for (int y = y_start; y < y_end; y++) {
								mulMatrixNotVectorized(matrixB, resultMatrix, m, n, y);
							}
						}
					}
				}
			}
		}
		return resultMatrix;
	}
	Matrix<T> multiplyVectorizedCache(Matrix<T>& matrixB, int block_size) {
		if (this->mainWidth != matrixB.getMainMatrixHeight() || matrixB.getCellMatrixHeight() != this->cellWidth)
		{
			return matrixB;
		}

		Matrix<T> resultMatrix(this->mainHeight, matrixB.getMainMatrixWidth(), matrixB.getCellMatrixWidth(), this->cellHeight);
		//int block_size = 32;
		for (int ii = 0; ii < this->mainHeight; ii += block_size) {
			for (int jj = 0; jj < this->mainWidth; jj += block_size) {
				for (int yy = 0; yy < matrixB.getMainMatrixWidth(); yy += block_size) {
					int i_start = ii;    // индекс i для блока принимает значения [ii, ii + block_size)
					int i_end = ii + block_size;
					int j_start = jj;    // индекс j для блока принимает значения [jj, jj + block_size)
					int j_end = jj + block_size;
					int y_start = yy;    // индекс j для блока принимает значения [jj, jj + block_size)
					int y_end = yy + block_size;

					for (int m = i_start; m < i_end; m++) {
						for (int n = j_start; n < j_end; n++) {
							for (int y = y_start; y < y_end; y++) {
								mulMatrixVectorized(matrixB, resultMatrix, m, n, y);
							}
						}
					}
				}
			}
		}
		return 	resultMatrix;
	}

	Matrix<T> multiplySseCache(Matrix<T>& matrixB, int block_size) {
		if (this->mainWidth != matrixB.getMainMatrixHeight()
			|| this->cellWidth != matrixB.getCellMatrixHeight())
		{
			return matrixB;
		}

		Matrix<T> resultMatrix(this->mainHeight, matrixB.getMainMatrixWidth(), matrixB.getCellMatrixWidth(), this->cellHeight);
		//int block_size = 32;
		for (int ii = 0; ii < this->mainHeight; ii += block_size) {
			for (int jj = 0; jj < this->mainWidth; jj += block_size) {
				for (int yy = 0; yy < matrixB.getMainMatrixWidth(); yy += block_size) {
					int i_start = ii;    // индекс i для блока принимает значения [ii, ii + block_size)
					int i_end = ii + block_size;
					int j_start = jj;    // индекс j для блока принимает значения [jj, jj + block_size)
					int j_end = jj + block_size;
					int y_start = yy;    // индекс j для блока принимает значения [jj, jj + block_size)
					int y_end = yy + block_size;

					for (int m = i_start; m < i_end; m++) {
						for (int n = j_start; n < j_end; n++) {
							for (int y = y_start; y < y_end; y++) {
								mulSse(matrixB, resultMatrix, m, n, y);
							}
						}
					}
				}
			}
		}
		return resultMatrix;
	}

	void mulMatrixNotVectorized(Matrix<T>& matrixB, Matrix<T>& resultMatrix, int m, int n, int y)
	{
		T* __restrict resultMatrixCellRow = nullptr;
		T* __restrict matrixBCellRow = nullptr;
		double a;
		for (int i = 0; i < this->cellHeight; i++) {
			resultMatrixCellRow = resultMatrix.getMatrixPointer()[m][n][i];
			for (int j = 0; j < this->cellWidth; j++) {
				matrixBCellRow = matrixB.getMatrixPointer()[m][y][j];
				a = this->Pointer[m][n][i][j];
#pragma loop(no_vector)
				for (int k = 0; k < matrixB.getCellMatrixWidth(); k++) {
					resultMatrixCellRow[k] += a * matrixBCellRow[k];
				}
			}
		}
	}

	void mulMatrixVectorized(Matrix<T>& matrixB, Matrix<T>& resultMatrix, int m, int n, int y)
	{
		T* __restrict resultMatrixCellRow = nullptr;
		T* __restrict matrixBCellRow = nullptr;
		double a;
		for (int i = 0; i < this->cellHeight; i++) {
			resultMatrixCellRow = resultMatrix.getMatrixPointer()[m][n][i];

			for (int j = 0; j < this->cellWidth; j++) {
				matrixBCellRow = matrixB.getMatrixPointer()[m][y][j];
				a = this->Pointer[m][n][i][j];

				for (int k = 0; k < matrixB.getCellMatrixWidth(); k++) {
					resultMatrixCellRow[k] += a * matrixBCellRow[k];
				}
			}
		}
	}

	void mulSse(Matrix<T>& matrixB, Matrix<T>& resultMatrix, int m, int n, int y)
	{
		T* __restrict resultMatrixCellRow = nullptr;
		T* __restrict matrixBCellRow = nullptr;

		__m128d resultMatrixReg;
		__m128d valueFromMatrixA;
		__m128d matrixBRowReg;

		for (int i = 0; i < this->cellHeight; i++)
		{
			resultMatrixCellRow = resultMatrix.getMatrixPointer()[m][n][i];
			for (int j = 0; j < this->cellWidth; j++)
			{
				matrixBCellRow = matrixB.getMatrixPointer()[m][y][j];
				valueFromMatrixA = _mm_set1_pd(this->Pointer[m][n][i][j]);
				for (int k = 0; k < matrixB.getCellMatrixWidth(); k += 2)
				{
					resultMatrixReg = _mm_load_pd(resultMatrixCellRow + k);
					matrixBRowReg = _mm_load_pd(matrixBCellRow + k);
					resultMatrixReg = _mm_add_pd(resultMatrixReg, _mm_mul_pd(valueFromMatrixA, matrixBRowReg));
					_mm_store_pd(resultMatrixCellRow + k, resultMatrixReg);
				}
			}
		}
	}

	bool equals(Matrix<T>& matrix) {
		for (int i = 0; i < mainHeight; i++) {
			for (int j = 0; j < mainWidth; j++) {
				for (int k = 0; k < cellHeight; k++) {
					for (int l = 0; l < cellWidth; l++) {
						if (this->Pointer[i][j][k][l] != matrix.getMatrixPointer()[i][j][k][l]) {
							return false;
						}
					}
				}
			}
		}
		return true;
	}

	bool generateValues() {
		srand((unsigned)time(NULL));

		try {
			for (int i = 0; i < this->mainHeight; i++) {
				for (int j = 0; j < mainWidth; j++) {
					for (int k = 0; k < cellHeight; k++) {
						for (int m = 0; m < cellWidth; m++) {
							Pointer[i][j][k][m] = (T)(rand() % 1000);
						}
					}
				}
			}
		}
		catch (...) {
			return false;
		}

		return true;
	}
	void outpt() {




		for (int k = 0; k < cellHeight; k++) {
			cout << endl;
			for (int m = 0; m < cellWidth; m++) {
				cout << this->Pointer[0][0][k][m] << " ";
			}
		}


	}

	T**** getMatrixPointer()
	{
		return this->Pointer;
	}

	int getMainMatrixWidth()
	{
		return this->mainWidth;
	}

	int getMainMatrixHeight()
	{
		return this->mainHeight;
	}

	int getCellMatrixWidth()
	{
		return this->cellWidth;
	}

	int getCellMatrixHeight()
	{
		return this->cellHeight;
	}
};
