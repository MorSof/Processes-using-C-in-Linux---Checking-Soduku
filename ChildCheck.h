/*
 * ChildCheck.h
 *
 *  Created on: 3 May 2019
 *      Author: mor
 */

#ifndef CHILDCHECK_H_
#define CHILDCHECK_H_


void readSodukusFromPipe(char* task);
void manageSodukuFromPipe(char* task);
int checkRows(int* mat);
int checkOneRow(int* row);
int checkCols(int* mat);
int checkOneCol(int* mat, int j);
int checkSquares(int* mat);
int checkOneSquare(int* mat);
void printMatrix(int* mat);
void cleanBuffer();






#endif /* CHILDCHECK_H_ */
