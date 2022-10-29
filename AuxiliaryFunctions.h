#ifndef AUXILIARYFUNCTIONS_H
#define AUXILIARYFUNCTIONS_H

#include "Program.h"

#define NOT_FINISH -2
#define FINISH_SUCCESS -3
#define FINISH_FAILURE -4
#define SUDOKO_FULL_SIZE 81
#define EMPTY_CELL -1
#define FILLED -3
#define FAIL -1


// --------------------------PART 1----------------------------------------------

Array*** PossibleDigits(short** sudokuBoard);
void createArrOfPossibilities(Array* possArray, short** sudokuBoard, short row, short col);
void checkPossibles(short** sudokuBoard, short row, short col, Array** possible);
void checkRow(short** sudokuBoard, short row, short col, Array** possible, short* newSize);
void checkCol(short** sudokuBoard, short row, short col, Array** possible, short* newSize);
void checkSubSquare(short** sudokuBoard, short row, short col, Array** possible, short* newSize);
void updateArr(Array** possible, short val, short* newSize);

// --------------------------PART 2----------------------------------------------
short OneStage(Player* player, short* x, short* y);
bool checkIfFailure(short** sudokuBoard, short row, short col);
bool isDupRow(short** sudokuBoard, short row, short col);
bool isDupCol(short** sudokuBoard, short row, short col);
bool isDupSubsquare(short** sudokuBoard, short row, short col);
void UpdatePossiblesArr(Player* player, short row, short col, short saver);
void UpdateCol(Player* player, short col, short currDelete);
void UpdateRow(Player* player, short row, short currDelete);
void UpdateSubSquare(Player* player, short currDelete, short SubSquareRow, short SubSquareCol);
void fillSquare(short** sudokuBoard, Array*** possibilities, short row, short col);
// --------------------------PART 3----------------------------------------------
short fillBoard(Player* player);
void printOptions(Array*** possibilities, short x, short y);
void deletePositionFromPositionList(PositionList* lst, short x, short y);


#endif