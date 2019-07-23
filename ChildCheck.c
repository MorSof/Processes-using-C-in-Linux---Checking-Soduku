#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include "ChildCheck.h"

#include "v1.h"

#define SIZE_ROWS  9
#define SIZE_COLS  9

int main(int argc, char* argv[]) {
	if (argc < 1) {
		dup2(11, STDOUT_FILENO);
		printf("Error no input for childCheck...\n");
		exit(1);
	}
	readSodukusFromPipe(argv[0]);
}

void readSodukusFromPipe(char* task) {
	int sodukuOnTheWay;
	scanf("%d", &sodukuOnTheWay);
	cleanBuffer();
	while (sodukuOnTheWay == 0) {
		// if you are here there is another Soduku to read
		manageSodukuFromPipe(task);
		scanf("%d", &sodukuOnTheWay);
	}
	cleanBuffer();
}

void cleanBuffer() {
	dup2(11, STDOUT_FILENO);
	printf("\n"); //clean the buffer
	dup2(10, STDOUT_FILENO);
}

void manageSodukuFromPipe(char* task) {

	int num;
	int soduku[9][9];
	int i, j;
	int ans;
	int test;

	for (i = 0; i < SIZE_ROWS; i++) {
		for (j = 0; j < SIZE_COLS; j++) {
			scanf("%d", &num);
			soduku[i][j] = num;
		}
	}

	test = atoi(task);

	switch (test) {

	case 0:

		ans = checkRows((int*) soduku);
		//here we write the answer to pipe
		printf("%d\n", ans);
		break;

	case 1:

		ans = checkCols((int*) soduku);
		//here we write the answer to pipe
		printf("%d\n", ans);
		break;

	case 2:

		ans = checkSquares((int*) soduku);
		//here we write the answer to pipe
		printf("%d\n", ans);
		break;

	}

}

int checkRows(int* mat) {

	int i, sum = 0;

	for (i = 0; i < SIZE_ROWS; i++) {
		sum += checkOneRow(mat + i * SIZE_COLS);
	}
	if (sum == 9)
		return 1;
	else
		return 0;
}

int checkOneRow(int* row) {

	int res[9] = { 0 };
	int i;

	for (i = 0; i < 9; i++) {
		int n = (row[i]) - 1;
		if (res[n] == 0) {
			res[n]++;
		} else {
			return 0;
		}
	}
	return 1;
}

int checkCols(int* mat) {
	int j, sum = 0;
	for (j = 0; j < SIZE_COLS; j++) {
		sum += checkOneCol(mat, j);
	}

	if (sum == 9)
		return 1;
	else
		return 0;
}

int checkOneCol(int* mat, int j) {
	int i;
	int res[9] = { 0 };

	for (i = 0; i < SIZE_ROWS; i++) {
		int n = *(mat + i * SIZE_COLS + j) - 1;
		if (res[n] == 0) {
			res[n]++;
		} else {
			return 0;
		}
	}

	return 1;

}

int checkSquares(int* mat) {

	int i, j, sum = 0;
	for (i = 0; i < SIZE_ROWS; i += 3) {
		for (j = 0; j < SIZE_COLS; j += 3) {
			sum += checkOneSquare(mat + i * SIZE_COLS + j);
		}
	}

	if (sum == 9)
		return 1;
	else
		return 0;

}
int checkOneSquare(int* mat) {

	int i, j;
	int res[9] = { 0 };
	for (i = 0; i < SIZE_ROWS / 3; i++) {
		for (j = 0; j < SIZE_COLS / 3; j++) {
			if (res[*(mat + (SIZE_ROWS * i + j)) - 1] == 0) {
				res[*(mat + (SIZE_COLS * i + j)) - 1]++;
			} else {
				return 0;
			}
		}

	}
	return 1;
}

void printMatrix(int* mat) {

	int i, j;
	printf("\n");

	for (i = 0; i < SIZE_ROWS; i++) {
		for (j = 0; j < SIZE_COLS; j++) {
			printf("%d   ", *(mat + i * SIZE_COLS + j));
		}
		printf("\n");
	}
	printf("\n\n");
}
