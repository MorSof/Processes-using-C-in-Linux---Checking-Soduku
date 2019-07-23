/*
 * Main.c
 *
 * Created on: 5 May 2019
 * Author: mor
 */

#include "v2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <bits/mman.h>
#include <unistd.h>
#include <fcntl.h>

#define SIZE_ROWS 9
#define SIZE_COLS 9
#define NUMBER_OF_CHILDEREN 3
#define MAP_ANONYMOUS 32

int main(int argc, char* argv[]) {
	int i;
	int task[] = { 1, 2, 3 };

	printf("\n");

	//Open shared memory
	Shared_Memory* sharedMemory = mmap(NULL, sizeof(int),
	PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (sharedMemory == MAP_FAILED) {
		printf("Error creating shared memory");
		exit(EXIT_FAILURE);
	}
	sharedMemory->isSodukusLeft = 1;
	resetStatus(sharedMemory->status);
	//Create childs
	for (i = 0; i < NUMBER_OF_CHILDEREN; i++) {
		createChild(task[i], sharedMemory);
	}
	if (argc < 2) {
		gettingSodukuFromUserQ2(sharedMemory);
	} else {
		gettingSodukuFromFilesQ2(sharedMemory, argv);
	}

	if (munmap(sharedMemory, sizeof(Shared_Memory)) == -1) {
		printf("munmap Error");
		exit(EXIT_FAILURE);
	}

	printf("\n");

	exit(EXIT_SUCCESS);
}

void createChild(int task, Shared_Memory* sharedMemory) {
	int pid;
	switch (pid = fork()) {
	case -1:
		printf("error: fork call\n");
		exit(EXIT_FAILURE);
	case 0: //Child is in the house!

		childBusyWaiting(sharedMemory);

		while (sharedMemory->isSodukusLeft) {

			childNavigator(task, sharedMemory);
			childBusyWaiting(sharedMemory);

		}

		if (munmap(sharedMemory, sizeof(Shared_Memory)) == -1) {
			printf("munmap Error");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);

	}
}

void childNavigator(int task, Shared_Memory* sharedMemory) {

	switch (task) {

	case 1:

		checkRowsQ2(sharedMemory);

		break;
	case 2:

		checkColsQ2(sharedMemory);
		break;
	case 3:

		checkSquaresQ2(sharedMemory);
		break;
	}

}

void checkRowsQ2(Shared_Memory* sharedMemory) {

	const int ROWS_STATUS = 0;
	int i, sum = 0;

	for (i = 0; i < SIZE_ROWS; i++) {
		sum += checkOneRowQ2(sharedMemory->soduku + i * SIZE_COLS);
	}
	if (sum == 9)
		sharedMemory->status[ROWS_STATUS] = 1;
	else
		sharedMemory->status[ROWS_STATUS] = 0;
}

int checkOneRowQ2(int* row) {

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

void checkColsQ2(Shared_Memory* sharedMemory) {
	const int COLS_STATUS = 1;
	int j, sum = 0;
	for (j = 0; j < SIZE_COLS; j++) {
		sum += checkOneColQ2(sharedMemory->soduku, j);
	}

	if (sum == 9)
		sharedMemory->status[COLS_STATUS] = 1;
	else
		sharedMemory->status[COLS_STATUS] = 0;
}

int checkOneColQ2(int* mat, int j) {
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

void checkSquaresQ2(Shared_Memory* sharedMemory) {

	const int SUB_MAT_STATUS = 2;
	int i, j, sum = 0;
	for (i = 0; i < SIZE_ROWS; i += 3) {
		for (j = 0; j < SIZE_COLS; j += 3) {
			sum += checkOneSquareQ2(sharedMemory->soduku + i * SIZE_COLS + j);
		}
	}

	if (sum == 9)
		sharedMemory->status[SUB_MAT_STATUS] = 1;
	else
		sharedMemory->status[SUB_MAT_STATUS] = 0;

}
int checkOneSquareQ2(int* mat) {

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

void gettingSodukuFromFilesQ2(Shared_Memory* sharedMemory, char* argv[]) {
	int i = 1;
	while (argv[i] != NULL) {

		readSodukuFromFileQ2(argv[i], sharedMemory->soduku);
		//if all status -2 it means soduku is ready for childs
		readyStatus(sharedMemory->status);
		fatherBusyWating(sharedMemory);
		if (checkStatus(sharedMemory->status)) {
			printf("%s is legal\n", argv[i]);
		} else {
			printf("%s is not legal\n", argv[i]);
		}

		//resetStatus(sharedMemory->status);
		i++;
	}

	sharedMemory->isSodukusLeft = 0;
}

void readSodukuFromFileQ2(char* fileName, int* soduku) {

	FILE *fp;

	int i, j;

	fp = fopen(fileName, "rt");

	for (i = 0; i < SIZE_ROWS; i++) {
		for (j = 0; j < SIZE_COLS; j++) {
			fscanf(fp, "%d", soduku + i * SIZE_COLS + j);
		}
	}

}

void gettingSodukuFromUserQ2(Shared_Memory* sharedMemory) {
	insertNumberToSodukuQ2(sharedMemory->soduku);
	readyStatus(sharedMemory->status);
	fatherBusyWating(sharedMemory);
	if (checkStatus(sharedMemory->status)) {
		printf("Soduku is legal\n");
	} else {
		printf("Soduku is not legal\n");
	}
	sharedMemory->isSodukusLeft = 0;
}

void insertNumberToSodukuQ2(int* soduku) {

	int i, j;

	printf(
			"Please enter numbers for your soduku, press enter after every number:\n");

	for (i = 0; i < SIZE_ROWS; i++) {
		for (j = 0; j < SIZE_COLS; j++) {
			scanf("%d", soduku + i * SIZE_COLS + j);

		}
	}

}

int checkStatus(int* status) {

	if (status[0] == 1 && status[1] == 1 && status[2] == 1) {
		return 1;
	}
	return 0;

}

void fatherBusyWating(Shared_Memory* sharedMemory) {

	while (sharedMemory->status[0] < 0 || sharedMemory->status[1] < 0
			|| sharedMemory->status[2] < 0) {
		//Waiting...
	}
}

void childBusyWaiting(Shared_Memory* sharedMemory) {

	while (sharedMemory->status[0] != -2 && sharedMemory->status[1] != -2
			&& sharedMemory->status[2] != -2 && sharedMemory->isSodukusLeft) {
		//Waiting...
	}

}

void readyStatus(int* status) {
	int i;

	for (i = 0; i < NUMBER_OF_CHILDEREN; i++) {
		status[i] = -2;

	}
}

void resetStatus(int* status) {

	int i;

	for (i = 0; i < NUMBER_OF_CHILDEREN; i++) {
		status[i] = -1;

	}
}
