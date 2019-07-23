
#ifndef V2_H_
#define V2_H_

typedef struct {

	int soduku[81];
	int  status[3];
	int  isSodukusLeft;
} Shared_Memory;


void createChild(int task, Shared_Memory* sharedMemory);
void childNavigator(int task, Shared_Memory* sharedMemory);
void checkRowsQ2(Shared_Memory* sharedMemory);
int checkOneRowQ2(int* row);
void checkColsQ2(Shared_Memory* sharedMemory);
int checkOneColQ2(int* mat, int j);
void checkSquaresQ2(Shared_Memory* sharedMemory);
int checkOneSquareQ2(int* mat);
void gettingSodukuFromFilesQ2(Shared_Memory* sharedMemory, char* argv[]);
void readSodukuFromFileQ2(char* fileName, int* soduku);
void gettingSodukuFromUserQ2(Shared_Memory* sharedMemory);
void insertNumberToSodukuQ2(int* soduku);
int checkStatus(int* status);
void fatherBusyWating(Shared_Memory* sharedMemory);
void childBusyWaiting(Shared_Memory* sharedMemory);
void readyStatus(int* status);
void resetStatus(int* status);

#endif /* V2_H_ */

