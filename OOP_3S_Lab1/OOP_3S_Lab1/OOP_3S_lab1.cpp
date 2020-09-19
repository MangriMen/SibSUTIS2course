#include <iostream>
#include <ctime>

using namespace std;

void fillMatrix(int** matrix, size_t size, string type) {
	if (type == "rand") {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				matrix[i][j] = rand() % 9;
			}
		}
	}
	else if (type == "dec") {
		int dec = size*size;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				matrix[i][j] = dec;
				dec--;
			}
		}
	}
	else if (type == "inc") {
		int inc = 1;
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				matrix[i][j] = inc;
				inc++;
			}
		}
	}
	else return;
}

void printMatrix(int **matrix, size_t size, bool debug = false) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < _msize(matrix[i]) / sizeof(int); j++) {
			cout << matrix[i][j] << "\t";
		}
		cout << endl << endl << endl << endl;
	}
	if (debug) {
		cout << endl << endl;

		for (int i = 0; i < size; i++) {
			for (int j = 0; j < _msize(matrix[i]) / sizeof(int); j++) {
				cout << i << " " << j << "\t";
			}
			cout << endl << endl << endl << endl;
		}
	}
}

void matrixToArray(int **matrix, size_t size, string task) {
	size_t bufSize = size * size;
	int counter = 0;
	int* arr = new int[bufSize];
	for (int i = 0; i < bufSize; i++) {
		arr[i] = 0;
	}
	if (task == "a") {
		int diagSize = 0;
		for (int i = 0; i < size; i++) {
			int diag = 0;
			for (int j = size - 1 - diagSize; j < size; j++) {
				arr[counter] = matrix[diag][j];
				counter++;
				diag++;
			}
			diagSize++;
		}
		diagSize = 0;
		for (int i = 1 + diagSize; i < size; i++) {
			int diag = i;
			for (int j = 0; j < size - 1 - diagSize; j++) {
				arr[counter] = matrix[diag][j];
				counter++;
				diag++;
			}
			diagSize++;
		}
	}
	else if (task == "b") {
		int i = 0;
		while (i < size && i >= 0) {
			int diag = i;
			for (int j = 0; j < i + 1; j++) {
				arr[counter] = matrix[diag][j];
				counter++;
				diag--;
			}
			i++;
		}
		int diagSize = 1;
		i = size - 1;
		while (diagSize < size) {
			int diag = i;
			int j = diagSize;
			while (j < size) {
				arr[counter] = matrix[diag][j];
				counter++;
				j++;
				diag--;
			}
			diagSize++;
		}
	}
	else if (task == "c") {
		int counter = 0;
		int closedBorder = floor((size + 1) / 2);
		int rightOff = closedBorder, bottomOff = closedBorder, leftOff = closedBorder - 2, topOff = closedBorder - 1;
		int rightBorder = size, bottomBorder = size, leftBorder = 0, topBorder = 0;
		int bottom = topOff;

		arr[counter] = matrix[size / 2][size / 2];
		counter++;

		if (size & 1) {
			while (counter < size * size) {
				int right = leftOff + 2;
				while (counter < size * size && right <= rightOff) {
					arr[counter] = matrix[bottom][right];
					right++;
					counter++;
				}
				right--;
				topOff--;

				int top = size - 1 - bottomOff;
				while (counter < size * size && top >= topOff) {
					arr[counter] = matrix[top][right];
					top--;
					counter++;
				}
				top++;
				bottomOff--;

				int left = rightOff - 1;
				while (counter < size * size && left >= leftOff) {
					arr[counter] = matrix[top][left];
					left--;
					counter++;
				}
				left++;
				rightOff++;

				bottom = topOff + 1;
				while (counter < size * size && bottom <= bottomBorder - bottomOff) {
					arr[counter] = matrix[bottom][left];
					bottom++;
					counter++;
				}
				bottom--;
				leftOff--;
			}
		}
		else {
			leftOff++;
			while (counter < size * size) {
				int right = leftOff + 1 + (counter != 1);

				while (counter < size * size && (counter == 1 ? right < rightOff : right <= rightOff)) {
					arr[counter] = matrix[bottom][right];
					right++;
					counter++;
				}
				right -= (counter != 1);
				topOff--;

				int top = size - 1 - bottomOff;
				while (counter < size * size && top > topOff) {
					arr[counter] = matrix[top][right];
					top--;
					counter++;
				}
				top++;
				bottomOff--;

				int left = rightOff - 1;
				while (counter < size * size && left >= leftOff) {
					arr[counter] = matrix[top][left];
					left--;
					counter++;
				}
				rightOff++;

				left++;
				bottom = topOff + 2;
				while (counter < size * size && bottom <= bottomBorder - bottomOff) {
					arr[counter] = matrix[bottom][left];
					bottom++;
					counter++;
				}
				left--;
				leftOff--;
				bottom--;
			}
		}
	}
	else if (task == "d") {
		int counter = 0;
		int rightOff = 0, bottomOff = 0, leftOff = 0, topOff = 0;
		int rightBorder = size, bottomBorder = size, leftBorder = 0, topBorder = 0;
		while (counter < size * size) {
			int right = leftOff;
			while (right < rightBorder - rightOff) {
				arr[counter] = matrix[topBorder + topOff][right];
				right++;
				counter++;
			}
			right--;
			topOff++;
			int bottom = topOff;
			while (bottom < bottomBorder - bottomOff) {
				arr[counter] = matrix[bottom][right];
				bottom++;
				counter++;
			}
			bottom--;
			rightOff++;
			int left = size - 1 - rightOff;
			while (left >= leftBorder + leftOff) {
				arr[counter] = matrix[bottom][left];
				left--;
				counter++;
			}
			left++;
			bottomOff++;
			int top = size - 1 - bottomOff;
			while (top >= topBorder + topOff) {
				arr[counter] = matrix[top][left];
				top--;
				counter++;
			}
			top++;
			leftOff++;
		}
	}
	else return;

	for (int i = 0; i < bufSize; i++) {
		cout << arr[i] << " ";
	}
	
	delete[] arr;
}

int** getMatrixRandomColumns(size_t size) {
	int** matrix = new int*[size];

	for (int i = 0; i < size; i++) {
		int rowSize = rand() % 8 + 2;
		matrix[i] = new int[rowSize];
		for (int j = 0; j < rowSize; ++j) {
			matrix[i][j] = rand() % 9;
		}
	}

	return matrix;
}

int main() {
	srand(unsigned int(time(NULL)));
	system("color F0");

	int side;
	cout << "Enter lenght of matrix side: ";
	cin >> side;
	cout << endl;

	int** matrix = new int* [side];
	for (int i = 0; i < side; i++) {
		matrix[i] = new int[side];
	}

	int** matrixRand = nullptr;

	fillMatrix(matrix, side, "rand");
	cout << "Default matrix: " << endl << endl;
	printMatrix(matrix, side, false);

	cout << endl;

	cout << "Diagonal right: ";
	matrixToArray(matrix, side, "a");
	cout << endl << endl;
	cout << "Diagonal left: ";
	matrixToArray(matrix, side, "b");
	cout << endl << endl;
	cout << "Spiral inner to outer: ";
	matrixToArray(matrix, side, "c");
	cout << endl << endl;
	cout << "Spiral outer to inner: ";
	matrixToArray(matrix, side, "d");
	cout << endl << endl;

	cout << "Random columns lenght matrix: " << endl << endl;
	matrixRand = getMatrixRandomColumns(side);
	printMatrix(matrixRand, side, false);

	cout << endl << endl;

	for (int i = 0; i < side; i++)
	{
		delete[] matrixRand[i];
	}

	delete[] matrixRand;

	for (int i = 0; i < side; i++)
	{
		delete[] matrix[i];
	}

	delete[] matrix;
}