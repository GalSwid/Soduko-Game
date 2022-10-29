#include "AuxiliaryFunctions.h"
#include "Program.h"

// --------------------------PART 1-------------------------------------------
Array*** PossibleDigits(short** sudokuBoard)
{
	/*the function build array of Possible Digits */

	Array*** possibleBoard = (Array***)malloc(9 * sizeof(Array**)); /*allocate memory to possible board*/

	for (short row = 0; row < 9; row++)
	{
		possibleBoard[row] = (Array**)malloc(9 * sizeof(Array*));
		checkAllocation(*(possibleBoard + row));
		for (short col = 0; col < 9; col++)
		{
			possibleBoard[row][col] = (Array*)malloc(sizeof(Array));
			checkAllocation(possibleBoard);
			createArrOfPossibilities(possibleBoard[row][col], sudokuBoard, row, col);
		}
	}
	return possibleBoard;
}
void createArrOfPossibilities(Array* possArray, short** sudokuBoard, short row, short col)
{
	/*the function create Array Of Possibilities */

	if (sudokuBoard[row][col] != -1)
	{
		possArray->size = 0;
		possArray->arr = NULL;
	}
	else
	{
		possArray->size = 9;
		possArray->arr = (short*)malloc(9 * sizeof(short));
		checkAllocation(possArray->arr);

		//init array
		for (short i = 0; i < 9; i++)
		{
			(possArray->arr)[i] = i + 1;
		}
		//update arr - delete unnecessary options
		checkPossibles(sudokuBoard, row, col, &possArray);
	}
}
void checkPossibles(short** sudokuBoard, short row, short col, Array** possible)
{
	/*the function check possiblity for each cell */
	unsigned short newSize = (*possible)->size;
	checkRow(sudokuBoard, row, col, possible, &newSize);
	checkCol(sudokuBoard, row, col, possible, &newSize);
	checkSubSquare(sudokuBoard, row, col, possible, &newSize);

	//bubble sort max -> min by old size
	BubbleSort((*possible)->arr, (*possible)->size);

	//realloc by new size
	updateAndReallocArray(*possible, newSize);
}
void checkRow(short** sudokuBoard, short row, short col, Array** possible, short* newSize)
{
	/*the function check possiblity for each cell by his row*/

	for (short c = 0; c < (*possible)->size; c++)
	{
		if (sudokuBoard[row][c] != -1)
		{
			updateArr(possible, sudokuBoard[row][c], newSize);
		}
	}
}
void checkCol(short** sudokuBoard, short row, short col, Array** possible, short* newSize)
{
	/*the function check possiblity for each cell by his col*/

	for (short r = 0; r < (*possible)->size; r++)
	{
		if (sudokuBoard[r][col] != -1)
		{
			updateArr(possible, sudokuBoard[r][col], newSize);
		}
	}
}
void checkSubSquare(short** sudokuBoard, short row, short col, Array** possible, short* newSize)
{
	/*the function check possiblity for each cell by his SubSquare*/

	short SubSquareRow = (row / 3) * 3;
	short SubSquareCol = (col / 3) * 3;

	for (short r = SubSquareRow; r < SubSquareRow + 3; r++)
	{
		for (short c = SubSquareCol; c < SubSquareCol + 3; c++)
		{
			if (sudokuBoard[r][c] != -1)
			{
				updateArr(possible, sudokuBoard[r][c], newSize);
			}
		}
	}
}
void updateArr(Array** possible, short val, short* newSize)
{
	for (short i = 0; i < (*possible)->size; i++)
	{
		if ((*possible)->arr[i] == val)
		{
			((*possible)->arr)[i] = 0;
			(*newSize)--;
		}
	}
}
void updateAndReallocArray(Array* myArray, short newSize)
{
	myArray->size = newSize;
	if (newSize != 0)
	{
		myArray->arr = (short*)realloc(myArray->arr, newSize * sizeof(short));
		checkAllocation(myArray->arr);
	}
	else
	{
		free(myArray->arr);
		myArray->arr = NULL;

	}
	checkAllocation(myArray);
}
void fillSquare(short** sudokuBoard, Array*** possibilities, short row, short col)
{
	Array* boardCell = possibilities[row][col];
	sudokuBoard[row][col] = boardCell->arr[0];

}
void printOptions(Array*** possibilities, short x, short y)
{
	Array* boardCell = possibilities[x][y];

	printf("Cell number [%d,%d] currently holds the minimum number of possible values, select one of the below:\n", x, y);
	for (short i = 0; i < boardCell->size; i++)
	{
		short size = boardCell->size;
		printf("%d. %d\n", i + 1, boardCell->arr[size - 1 - i]);
	}
}

//-------------------------------PART 2------------------------------------------
short OneStage(Player* player, short* x, short* y)
{
	Array*** possibilities =player->possiblesBoard;
	short** sudokuBoard = player->gameBoard->board;
	PositionList* emptyCellsList = player->gameBoard->emptyCells;
	Array* boardCell;
	short flag = NOT_FINISH, minSize = 10, row, col;
	
	for (row = 0; row < 9 && flag != FINISH_FAILURE; row++)
	{
		for (col = 0; (col < 9) && flag != FINISH_FAILURE; col++)
		{
			boardCell = possibilities[row][col];
			if (boardCell->size == 1)
			{
				sudokuBoard[row][col] = boardCell->arr[0];
				deletePositionFromPositionList(emptyCellsList, row, col);

				printf("update value of [%d,%d]\n", row, col);
				free(boardCell->arr);
				boardCell->arr = NULL;
				boardCell->size = 0;
				UpdatePossiblesArr(player, row, col, sudokuBoard[row][col]);
			}

			if (checkIfFailure(sudokuBoard, row, col))
			{
				flag = FINISH_FAILURE;
			}

			else if (boardCell->size >= 2 && sudokuBoard[row][col] == -1)
			{
				flag = NOT_FINISH;
				if (boardCell->size < minSize)
				{
					*x = row;
					*y = col;
					minSize = boardCell->size;
				}
			}
		}
	}
	if (isEmptyPositionList(emptyCellsList))
		flag = FINISH_SUCCESS;

	return flag;
}

bool checkIfFailure(short** sudokuBoard, short row, short col)
{
	return (isDupCol(sudokuBoard, row, col) || isDupRow(sudokuBoard, row, col) || isDupSubsquare(sudokuBoard, row, col));
}

bool isDupRow(short** sudokuBoard, short row, short col)
{
	short counter = 0, curCell = sudokuBoard[row][col];

	if (curCell == -1)
		return false;

	for (short c = 0; c < 9; c++)
	{
		if (sudokuBoard[row][c] == curCell)
			counter++;
	}
	if (counter > 1)
		return true;
	return false;
}

bool isDupCol(short** sudokuBoard, short row, short col) /*the function return true if found duplicate value in the current col*/
{
	short counter = 0, curCell = sudokuBoard[row][col];

	if (curCell == -1)
		return false;

	for (short r = 0; r < 9; r++)
	{
		if (sudokuBoard[r][col] == curCell)
			counter++;
	}
	if (counter > 1)
		return true;

	return false;
}
bool isDupSubsquare(short** sudokuBoard, short row, short col) /*the function return true if found duplicate value in the current sub-square*/
{
	short counter = 0;
	short SubSquareRow = (row / 3) * 3;
	short SubSquareCol = (col / 3) * 3;
	short curCell = sudokuBoard[row][col];

	if (curCell == -1)
		return false;

	for (short r = SubSquareRow; r < SubSquareRow + 3; r++)
	{
		for (short c = SubSquareCol; (c < SubSquareCol + 3); c++)
		{
			if (sudokuBoard[r][c] == curCell)
				counter++;
		}

	}
	if (counter > 1)
		return true;
	return false;
}

void UpdatePossiblesArr(Player* player, short row, short col, short saver)
{
	short SubSquareRow = (row / 3) * 3;
	short SubSquareCol = (col / 3) * 3;
	
	UpdateCol(player, col, saver);
	UpdateRow(player, row, saver);
	UpdateSubSquare(player, saver, SubSquareRow, SubSquareCol);
}
void UpdateCol(Player* player, short col, short currDelete)
{
	unsigned short tempSize;
	Array* boardCell;

	for (short row = 0; row < 9; row++)
	{
		boardCell = player->possiblesBoard[row][col];
		if (player->gameBoard->board[row][col] == -1)
		{
			if (boardCell->size > 1)
			{
				for (short i = 0; i < boardCell->size; i++)
				{
					if (boardCell->arr[i] == currDelete)
					{
						boardCell->arr[i] = 0;
						tempSize = boardCell->size - 1;
						BubbleSort(boardCell->arr, boardCell->size);
						updateAndReallocArray(boardCell, tempSize);
					}
				}
			}
			if (boardCell->size == 1)
			{
				fillSquare(player->gameBoard->board, player->possiblesBoard, row, col);
				deletePositionFromPositionList(player->gameBoard->emptyCells, row, col);
				updateAndReallocArray(boardCell, 0);
				UpdatePossiblesArr(player, row, col, player->gameBoard->board[row][col]);

			}
		}
	}
}
void UpdateRow(Player* player, short row, short currDelete)
{
	unsigned short tempSize;
	Array* boardCell;

	for (short col = 0; col < 9; col++)
	{
		boardCell = player->possiblesBoard[row][col];
		if (player->gameBoard->board[row][col] == -1)
		{
			for (short i = 0; i < boardCell->size; i++)
			{
				if (boardCell->arr[i] == currDelete)
				{
					boardCell->arr[i] = 0;
					tempSize = boardCell->size - 1;
					BubbleSort(boardCell->arr, boardCell->size);
					updateAndReallocArray(boardCell, tempSize);
				}
			}
		}
		if (boardCell->size == 1)
		{
			fillSquare(player->gameBoard->board, player->possiblesBoard, row, col);
			deletePositionFromPositionList(player->gameBoard->emptyCells, row, col);
			updateAndReallocArray(boardCell, 0);
			UpdatePossiblesArr(player, row, col, player->gameBoard->board[row][col]);

		}

	}
}
void UpdateSubSquare(Player* player, short currDelete, short SubSquareRow, short SubSquareCol)
{
	unsigned short tempSize;
	Array* boardCell;

	for (short r = SubSquareRow; r < SubSquareRow + 3; r++)
	{
		for (short c = SubSquareCol; c < SubSquareCol + 3; c++)
		{
			boardCell = player->possiblesBoard[r][c];
			if (player->gameBoard->board[r][c] == -1)
			{
				for (short i = 0; i < boardCell->size; i++)
				{
					if (boardCell->arr[i] == currDelete)
					{
						boardCell->arr[i] = 0;
						tempSize = boardCell->size - 1;
						BubbleSort(boardCell->arr, boardCell->size);
						updateAndReallocArray(boardCell, tempSize);
					}
				}
			}
			if (boardCell->size == 1)
			{
				fillSquare(player->gameBoard->board, player->possiblesBoard, r, c);
				deletePositionFromPositionList(player->gameBoard->emptyCells, r, c);
				updateAndReallocArray(boardCell, 0);
				UpdatePossiblesArr(player, r, c, player->gameBoard->board[r][c]);

			}
		}
	}
}

//-------------------------------PART 3------------------------------------------

short fillBoard(Player* player)
{
	short x = -1, y = -1, val, flag;

	flag = OneStage(player, &x, &y);
	while (flag == NOT_FINISH)
	{
		if (isEmptyPositionList(player->gameBoard->emptyCells))
		{
			flag = FINISH_SUCCESS;
			break;
		}
		printSodukoBoard(player->gameBoard->board, player->name);
		if (x != -1 && y != -1)
		{
			printOptions(player->possiblesBoard, x, y);
			scanf("%hi", &val);

		}
		else
		{
			flag = OneStage(player, &x, &y);
			break;
		}

		if (flag == NOT_FINISH)
		{
			player->gameBoard->board[x][y] = val;
			player->possiblesBoard[x][y]->arr = NULL; 
			player->possiblesBoard[x][y]->size = 0; 
			deletePositionFromPositionList(player->gameBoard->emptyCells, x, y);
			UpdatePossiblesArr(player, x, y, val);
		}
		if (flag == FINISH_FAILURE)
		{
			return FAIL;
		}
		x = -1;
		y = -1;
		flag = OneStage(player, &x, &y);
	}

	return flag;
}


