#include <Windows.h>
#include <iostream>
#include "Matrix.h"
using namespace std;

typedef double MyType;

//#define MAIN_WIDTH 352
//#define MAIN_HEIGHT 352
//#define MATRIX_A_CELL_WIDTH 4
//#define MATRIX_A_CELL_HEIGHT 4
//#define MATRIX_B_CELL_WIDTH 8
//#define MATRIX_B_CELL_HEIGHT 4

#define MAIN_WIDTH 96
#define MAIN_HEIGHT 96
#define MATRIX_A_CELL_WIDTH 16
#define MATRIX_A_CELL_HEIGHT 16
#define MATRIX_B_CELL_WIDTH 32
#define MATRIX_B_CELL_HEIGHT 16
const int BLOCK_SIZE = 32;

int main() {
	Matrix<MyType> matrixA(MAIN_WIDTH, MAIN_HEIGHT, MATRIX_A_CELL_WIDTH, MATRIX_A_CELL_HEIGHT),
		matrixB(MAIN_WIDTH, MAIN_HEIGHT, MATRIX_B_CELL_WIDTH, MATRIX_B_CELL_HEIGHT),
		matrixC, matrixD, matrixF;

	matrixA.generateValues();
	matrixB.generateValues();

	ULONGLONG resultTimeSseCache, resultTimeVectorizedCache, resultTimeSse, resultTimeVectorized,
		resultTimeNotVectorized, resultTimeNotVectorizedCache, endTime, startTime;

	startTime = GetTickCount64();
	matrixC = matrixA.multiplyVectorized(matrixB);
	endTime = GetTickCount64();
	resultTimeVectorized = endTime - startTime;

	
	startTime = GetTickCount64();
	matrixC = matrixA.multiplyVectorizedCache(matrixB, BLOCK_SIZE);
	endTime = GetTickCount64();
	resultTimeVectorizedCache = endTime - startTime;

	startTime = GetTickCount64();
	matrixD = matrixA.multiplySse(matrixB);
	endTime = GetTickCount64();
	resultTimeSse = endTime - startTime;

	startTime = GetTickCount64();
	matrixD = matrixA.multiplyNotVectorized(matrixB);
	endTime = GetTickCount64();
	resultTimeNotVectorized = endTime - startTime;

	if (matrixC.equals(matrixD)) {
		cout << "Matrices by notvec and by Vectorization are equal." << endl << endl;
	}
	else {
		cout << "Matrices by notvec and by Vectorization are not equal." << endl << endl;
	}

	startTime = GetTickCount64();
	matrixD = matrixA.multiplySseCache(matrixB, BLOCK_SIZE);
	endTime = GetTickCount64();
	resultTimeSseCache = endTime - startTime;



	cout << "Not Vectorized Ticks: " << resultTimeNotVectorized << "." << endl;
	cout << "Vectorization Ticks: " << resultTimeVectorized << "." << endl;
	cout << "SSE Ticks: " << resultTimeSse << "." << endl << endl;

	cout << "VectorizationCache Ticks: " << resultTimeVectorizedCache << "." << endl;
	cout << "SSECache Ticks: " << resultTimeSseCache << "." << endl << endl;

	cout << "Vectorized speed up: x" << (double)resultTimeNotVectorized / (double)resultTimeVectorized << "." << endl;
	cout << "SSE speed up: x" << (double)resultTimeNotVectorized / (double)resultTimeSse << "." << endl << endl;


	cout << "VectorizedCache speed up: x" << (double)resultTimeNotVectorized / (double)resultTimeVectorizedCache << "." << endl;
	cout << "SSECache speed up: x" << (double)resultTimeNotVectorized / (double)resultTimeSseCache << "." << endl << endl;

	if (matrixC.equals(matrixD)) {
		cout << "Matrices by SSE and by Vectorization are equal." << endl << endl;
	}
	else {
		cout << "Matrices by SSE and by Vectorization are not equal." << endl << endl;
	}
	cout << endl << endl;

	/*matrixC.outpt();
	cout << endl << endl;
	matrixD.outpt();*/

	system("pause");
	return 0;
}