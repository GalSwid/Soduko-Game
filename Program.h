#ifndef PROGRAM_H
#define PROGRAM_H
#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
//#include "AuxiliaryFunctions.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <crtdbg.h>

#define MAX_LEN 100

typedef struct position
{
	short row, col;
}Position;

typedef struct listNode
{
	Position* data;
	struct listNode* next;
}ListNode;

typedef struct list
{
	ListNode* head, * tail;
}PositionList;

typedef struct _randomBoard
{
	short** board;
	PositionList* emptyCells;
}RandomBoard;

typedef struct _Array
{
	short* arr;
	unsigned short size;
}Array;

typedef struct _Player
{
	char name[MAX_LEN];
	RandomBoard* gameBoard;
	Array*** possiblesBoard;
}Player;

typedef struct playerlistNode
{
	Player* data;
	struct playerlistNode* next;
}PlayerListNode;

typedef struct playerList
{
	PlayerListNode* head, * tail;
}PlayerList;

typedef struct treeNode
{
	PlayerListNode** dataListNode;
	struct treeNode *right, *left;
}TreeNode;

typedef struct tree
{
	TreeNode* root;
}Tree;


//---------------------------------MANAGE----------------------------------------------
void startGame();
// --------------------------GENERAL---------------------------------------------------
void BubbleSort(short* arr, unsigned short size);
void swap(short* arr, unsigned short i, unsigned short j);
void checkAllocation(void* arr);
void printSodukoBoard(short** board, char* playerName);
void updateAndReallocArray(Array* myArray, short newSize);
void printPossSizeBoard(Array*** board);
//---------------------------------BOARD----------------------------------------------
RandomBoard* createRandomBoard();
short** createStartBoard();
void initBoardGame(RandomBoard* randBoard);
//---------------------------------LIST----------------------------------------------
PositionList* createStartList();
ListNode* createNewPositionListNode(Position* data, ListNode* next);
PlayerListNode* createNewPlayersListNode(Player* data, PlayerListNode* next);
bool isEmptyPositionList(PositionList* lst);
bool isEmptyPlayersList(PlayerList* lst);
int lenOfPositionList(PositionList* lst);
int lenOfPlayerList(PlayerList* lst);
void makeEmptyPlayersList(PlayerList* lst);
void makeEmptyPositionList(PositionList* lst);
void insertDataToEndOfPositionList(PositionList* lst, Position* data);
void insertDataToEndOfPlayersList(PlayerList* lst, Player* data);
void insertNodeToEndOfPositionList(PositionList* lst, ListNode* node);
void insertNodeToEndOfPlayersList(PlayerList* lst, PlayerListNode* tail);
void deletePlayerFromPlayerList(Player* data, PlayerList* listOfPlayer);
void printPlayerList(PlayerList* lst);
//-------------------------------ARR------------------------------------------------
Position* createPosition(short row, short col);
PlayerListNode** buildArrFromPlayerList(PlayerList* listOfPlayer, int numOfPlayer);
PlayerListNode** merge(PlayerListNode** a1, int size1, PlayerListNode** a2, int size2);
int compareFullCell(short** board1, short** board2);
void setValue(short** sudokuBoard, short row, short col, Array* options);
void getOptionsArr(Array** options, short** sudokuBoard, short row, short col);
void sortArrOfPointers(PlayerListNode** arr, int size);
void mergeSort(PlayerListNode** arr, int size);
void copyArr(PlayerListNode** dest, PlayerListNode** src, int size);
//-------------------------------TREE------------------------------------------------
Tree buildTreeFromArray(PlayerListNode** arr, int size);
Tree buildTreeFromArrayHelper(PlayerListNode** arr, int size);
void scanTreeInOrder(TreeNode* root, PlayerList* listOfPlayer, PlayerList* winnersList);
//-------------------------------PLAYER---------------------------------------------
Player* createNewPlayer(int index);
PlayerList* createListOfPlayers(int numOfPlayer);
//-------------------------------FREE---------------------------------------------
void freePlayerList(PlayerList* lst);
void freePositionList(PositionList* lst);
void freePositionListNode(ListNode* nodeToDelete); 
void freeRandomBoard(RandomBoard* randBoard);
void freePossBoard(Array*** possBoard);
void freeSodukoBoard(short** board);
void freePlayer(Player* player);
void freeArray(Array* options);
void freeTree(Tree* tr);
void freeTreeHelper(TreeNode* root);
void freePlayersArr(PlayerListNode** playersArr);

#endif 