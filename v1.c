#include "v1.h"
#include "v1.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include "ChildCheck.h"
#define BUF_SIZE 10
#define SIZE_ROWS  9
#define SIZE_COLS  9

#define NUM_OF_PIPES 4
#define FREE_CHANNEL_WRITE 11
#define FREE_CHANNEL_READ 12
int pfd1[2];
int pfd2[2];
int pfd3[2];
int pfd4[2];

int main(int argc, char* argv[]) {

	const char* TASK_ONE = "0";
	const char* TASK_TWO = "1";
	const char* TASK_THREE = "2";

	printf("\n");

	//Creating the pipes
	createPipe(pfd1);
	createPipe(pfd2);
	createPipe(pfd3);
	createPipe(pfd4);

	dup2(STDOUT_FILENO, FREE_CHANNEL_WRITE); //now channel 11 is navigate to console write
	dup2(STDIN_FILENO, FREE_CHANNEL_READ); //now channel 12 is navigate to console read

	//Create and Navigate Children
	childNavigation(pfd1[1], pfd1[0], TASK_ONE);
	childNavigation(pfd2[1], pfd2[0], TASK_TWO);
	childNavigation(pfd3[1], pfd3[0], TASK_THREE);

	sendSodukusToChildren(argc, argv);

	return 0;

}

void childNavigation(int pfdWrite, int pfdRead, const char* task) {

	int pid;

	const int PFD_READ4 = 9;
	const int PFD_WRITE4 = 10;

	switch (pid = fork()) {
	case -1:
		printf("error: fork call\n");
		exit(1);
	case 0: //Child is in the house!

		dup2(pfdRead, STDIN_FILENO); //STDIN is now only to PIPE
		close(pfdRead);
		close(pfdWrite);
		dup2(PFD_WRITE4, STDOUT_FILENO); //STDOUT is now only to pipe
		close(PFD_READ4);
		if (execlp("./ChildCheck", task, NULL) == -1) {
			dup2(FREE_CHANNEL_WRITE, STDOUT_FILENO);
			printf("ERROR in execlp\n");
			exit(1);
		}
		break;
	}

}

void sendSodukusToChildren(int argc, char* argv[]) {

	int answer1, answer2, answer3;

	if (argc < 2) {

		gettingSodukuFromUser(&answer1, &answer2, &answer3);

	} else {

		gettingSodukuFromFiles(argv, &answer1, &answer2, &answer3);

	}
	dup2(FREE_CHANNEL_WRITE, STDOUT_FILENO);
	dup2(FREE_CHANNEL_READ, STDIN_FILENO);
	closeAllPipes();
	printf("\n");

}

void gettingSodukuFromUser(int* answer1, int* answer2, int* answer3) {

	int soduku[SIZE_ROWS][SIZE_COLS];

	insertNumberToSoduku((int*) soduku);

	sendSodukusToChildrenHelper((int*) soduku, answer1, answer2, answer3);

	dup2(FREE_CHANNEL_WRITE, STDOUT_FILENO);
	if (*answer1 == 1 && *answer2 == 1 && *answer3 == 1) {
		printf("soduku is legal\n");
	} else {
		printf("soduku is not legal\n");
	}

	signalNoSoduku(pfd1[1], pfd1[0]);
	signalNoSoduku(pfd2[1], pfd2[0]);
	signalNoSoduku(pfd3[1], pfd3[0]);

}

void gettingSodukuFromFiles(char* argv[], int* answer1, int* answer2,
		int* answer3) {

	int i = 1;

	while (argv[i] != NULL) {

		int soduku[SIZE_ROWS][SIZE_COLS];

		readSodukuFromFile(argv[i], (int*) soduku);
		sendSodukusToChildrenHelper((int*) soduku, answer1, answer2, answer3);

		dup2(FREE_CHANNEL_WRITE, STDOUT_FILENO);
		if (*answer1 == 1 && *answer2 == 1 && *answer3 == 1) {
			printf("%s is legal\n", argv[i]);
		} else {
			printf("%s is not legal\n", argv[i]);
		}

		i++;

	}

	signalNoSoduku(pfd1[1], pfd1[0]);
	signalNoSoduku(pfd2[1], pfd2[0]);
	signalNoSoduku(pfd3[1], pfd3[0]);

}

void sendSodukusToChildrenHelper(int* soduku, int* answer1, int* answer2,
		int* answer3) {

	sendOneSodukuToChild(soduku, pfd1[1], pfd1[0]);
	sendOneSodukuToChild(soduku, pfd2[1], pfd2[0]);
	sendOneSodukuToChild(soduku, pfd3[1], pfd3[0]);

	dup2(pfd4[0], STDIN_FILENO); //Now STDIN is only to pipe
	close(pfd4[0]);
	close(pfd4[1]);

	scanf("%d", answer1); //first answer
	scanf("%d", answer2); //second answer
	scanf("%d", answer3); //third answer

}

void signalNoSoduku(int pfdWrite, int pfdRead) {

	const int NO_SODUKU = -1;
	dup2(pfdWrite, STDOUT_FILENO);
	close(pfdRead);
	printf("%d\n", NO_SODUKU);
}

void sendOneSodukuToChild(int* soduku, int pfdWrite, int pfdRead) {

	const int SODUKU_ON_THE_WAY = 0;
	int i, j;

	dup2(pfdWrite, STDOUT_FILENO);
	close(pfdRead);

	printf("%d\n", SODUKU_ON_THE_WAY);

	for (i = 0; i < SIZE_ROWS; i++) {
		for (j = 0; j < SIZE_COLS; j++) {
			printf("%d\n", *(soduku + i * SIZE_COLS + j));
		}
	}

}

void readSodukuFromFile(char* fileName, int* soduku) {

	FILE *fp;

	int i, j;

	fp = fopen(fileName, "rt");

	for (i = 0; i < SIZE_ROWS; i++) {
		for (j = 0; j < SIZE_COLS; j++) {
			fscanf(fp, "%d", soduku + i * SIZE_COLS + j);
		}
	}

}

void createPipe(int* pfd) {

	if (pipe(pfd) == -1)
		printf("error");

}

void insertNumberToSoduku(int* soduku) {

	int i, j;

	printf(
			"Please enter numbers for your soduku, press enter after every number:\n");

	for (i = 0; i < SIZE_ROWS; i++) {
		for (j = 0; j < SIZE_COLS; j++) {
			scanf("%d", soduku + i * SIZE_COLS + j);

		}
	}

}

void closeAllPipes() {
	close(pfd1[0]);
	close(pfd1[1]);
	close(pfd2[0]);
	close(pfd2[1]);
	close(pfd3[0]);
	close(pfd3[1]);
	close(pfd4[0]);
	close(pfd4[1]);
}
