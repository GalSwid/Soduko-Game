#include "Program.h"
#include "AuxiliaryFunctions.h"

//---------------------------------MANAGE----------------------------------------------
void startGame()
{
	int numOfPlayer;
	printf("please enter number of players:\n");
	scanf("%d", &numOfPlayer);
	char ch = getchar();

	PlayerList* listOfPlayer = createListOfPlayers(numOfPlayer);

	printf("-----The Players are:-----\n");
	printPlayerList(listOfPlayer);

	PlayerListNode** playersArr = buildArrFromPlayerList(listOfPlayer, numOfPlayer);

	Tree playersTree = buildTreeFromArray(playersArr, numOfPlayer);

	PlayerList* winnersList = (PlayerList*)malloc(sizeof(PlayerList)); // allocate new Players List 
	checkAllocation(winnersList);
	makeEmptyPlayersList(winnersList); // make empty list

	scanTreeInOrder(playersTree.root, listOfPlayer, winnersList);

	printf("-----The Winners are:-----\n");
	printPlayerList(winnersList);

	freeTree(&playersTree); 
	freePlayerList(listOfPlayer);
	freePlayerList(winnersList);

}

//-------------------------------GENERAL-----------------------------------------
void BubbleSort(short* arr, unsigned short size)
{
	unsigned short i, j;

	for (i = 0; i < size - 1; i++)
		for (j = 0; j < size - i - 1; j++)
			if (arr[j] < arr[j + 1])
				swap(arr, j, j + 1);
}
void swap(short* arr, unsigned short i, unsigned short j)
{
	short tmp = arr[i];
	arr[i] = arr[j];
	arr[j] = tmp;
}
void checkAllocation(void* arr)
{
	if (arr == NULL)
	{
		printf("Memory allocation failure!!!\n");
		exit(1);
	}
}
void printSodukoBoard(short** board, char* playerName)
{
	printf("#####----%s's board----#####\n\n", playerName);
	printf("  ");

	for (unsigned short i = 0; i < 9; i++)
	{
		if (i % 3 == 0) {
			printf("|");
		}
		printf("%*d", 2, i);
	}
	printf("\n-----------------------\n");
	for (int row = 0; row < 9; row++)
	{
		printf("%*d|", 2, row);
		for (int col = 0; col < 9; col++)
		{

			if (board[row][col] == EMPTY_CELL)
			{
				printf("  ");
			}
			else
			{
				printf("%*d", 2, board[row][col]);
			}

			if (col % 3 == 2)
			{
				printf("|");
			}
		}
		printf("\n");

		if (row % 3 == 2)
		{
			printf("-----------------------\n");
		}
	}
	printf("\n\n\n");
}

//---------------------------------BOARD----------------------------------------------
short** createStartBoard() // allocate initial game board (type short**)
{
	short** board = (short**)malloc(9 * sizeof(short*)); // allocate rows
	checkAllocation(board);
	for (short row = 0; row < 9; row++)
	{
		board[row] = (short*)malloc(9 * sizeof(short)); // allocate cols 
		checkAllocation(board[row]);

		for (int i = 0; i < 9; i++)
		{
			board[row][i] = -1;
		}

	}
	return board; // return 9*9 board
}
RandomBoard* createRandomBoard()
{
	RandomBoard* randomBoard = (RandomBoard*)malloc(sizeof(RandomBoard)); // allocate new randomBoard

	randomBoard->board = createStartBoard(); // create and set new startboard (type short**) 
	randomBoard->emptyCells = createStartList(); // create list of all the empty cells 
	initBoardGame(randomBoard); // init randomBoard
	return randomBoard;
}
void initBoardGame(RandomBoard* randBoard)
{
	ListNode* newNext, * temp, * curr;
	Array* options = NULL;
	short k = -1, n, len;
	short row = 0, col = 0;

	srand(time(NULL));
	n = 1 + rand() % 20;
	for (short i = 0; i < n; ) // fill n cells on the board
	{
		len = lenOfPositionList(randBoard->emptyCells); // calculate length of empty cells list in order to set range for K

		k = 1 + rand() % len; // 1<=k<=length of list 

		curr = randBoard->emptyCells->head;
		for (short index = 1; index < k - 1; index++) // get the k node 
		{
			curr = curr->next;
		}
		row = curr->data->row; // get row of k node 
		col = curr->data->col; // get col of k node 

		deletePositionFromPositionList(randBoard->emptyCells, row, col); // remove position [row][col] from list
		getOptionsArr(&options, randBoard->board, row, col); // get Array from possible board from [row][col] cell

		if (options->size != 0)
		{
			setValue(randBoard->board, row, col, options); // לוודא את עניין הסייז שווה 0 
			i++;
		}
	}
	freeArray(options);

}

//---------------------------------LIST----------------------------------------------
PositionList* createStartList()
{
	//insert all position on board into startlist 
	Position* pos;
	PositionList* startList = (PositionList*)malloc(sizeof(PositionList)); // allocate List
	checkAllocation(startList);
	makeEmptyPositionList(startList);

	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			pos = createPosition(row, col); // create position 
			insertDataToEndOfPositionList(startList, pos); // add position to the end of the list 
		}
	}

	return startList;
}

ListNode* createNewPositionListNode(Position* data, ListNode* next)
{
	ListNode* res = (ListNode*)malloc(sizeof(ListNode)); // allocate list node 
	checkAllocation(res);
	res->data = data; // set data 
	res->next = next; // set next 
	return res;
}
PlayerListNode* createNewPlayersListNode(Player* data, PlayerListNode* next)
{
	PlayerListNode* res = (PlayerListNode*)malloc(sizeof(PlayerListNode)); // allocate list node 
	checkAllocation(res);

	res->data = data; // set data 
	res->next = next; // set next 
	return res;
}

bool isEmptyPositionList(PositionList* lst)
{
	//check if list is empty
	if (lst->head == NULL)
		return true;
	else
		return false;
}
bool isEmptyPlayersList(PlayerList* lst)
{
	if (lst->head == NULL) // if there is at least one node (head)
		return true;
	else
		return false;
}

int lenOfPositionList(PositionList* lst)
{
	//calculate length of list 
	int counter = 0;
	ListNode* curr = lst->head;
	while (curr != NULL)
	{
		counter++;
		curr = curr->next;
	}
	return counter;
}
int lenOfPlayerList(PlayerList* lst)
{
	int counter = 0;
	PlayerListNode* curr = lst->head;
	while (curr != NULL)
	{
		counter++; // count number of PlayerListNode in list 
		curr = curr->next;
	}
	return counter;
}

void makeEmptyPositionList(PositionList* lst)
{
	// init head and tail with NULL 
	lst->head = lst->tail = NULL;
}
void makeEmptyPlayersList(PlayerList* lst)
{
	// init head and tail with NULL 
	lst->head = lst->tail = NULL;
}

void insertDataToEndOfPositionList(PositionList* lst, Position* data)
{
	ListNode* newTail;
	newTail = createNewPositionListNode(data, NULL); // allocate new tail
	insertNodeToEndOfPositionList(lst, newTail); // insert newTail to the end of the list 
}
void insertDataToEndOfPlayersList(PlayerList* lst, Player* data)
{
	PlayerListNode* newTail;
	newTail = createNewPlayersListNode(data, NULL); // create node 
	insertNodeToEndOfPlayersList(lst, newTail); // add node to end of list 
}

void insertNodeToEndOfPositionList(PositionList* lst, ListNode* node)
{
	if (isEmptyPositionList(lst) == true) // if the list is empty, the 'node' will be haed and tail
		lst->head = lst->tail = node;
	else// not empty list. insert node to the end and update the tail
	{
		lst->tail->next = node;
		lst->tail = node;
	}
	node->next = NULL;
}
void insertNodeToEndOfPlayersList(PlayerList* lst, PlayerListNode* tail)
{
	if (isEmptyPlayersList(lst) == true)
		lst->head = lst->tail = tail;
	else
	{
		lst->tail->next = tail;
		lst->tail = tail;
	}
	tail->next = NULL;
}

void deletePlayerFromPlayerList(Player* data, PlayerList* listOfPlayer)
{
	PlayerListNode* temp, * curr, * newNext;
	if (listOfPlayer->head == listOfPlayer->tail) // == node)
		listOfPlayer->head = listOfPlayer->tail = NULL;
	else if (listOfPlayer->head->data == data) // or node->prev == NULL
	{
		temp = listOfPlayer->head;
		listOfPlayer->head = listOfPlayer->head->next;
		free(temp);
	}
	else if (listOfPlayer->tail->data == data) // or node->next == NULL
	{
		temp = listOfPlayer->tail;
		curr = listOfPlayer->head;
		while (curr->next->next != NULL)
			curr = curr->next;
		listOfPlayer->tail = curr;
		curr->next = NULL;
		free(temp);
	}
	else
	{
		curr = listOfPlayer->head;
		while (curr->next->data != data)
		{
			curr = curr->next;
		}
		newNext = curr->next->next;
		temp = curr->next;
		curr->next = newNext;

		free(temp);
	}

}
void deletePositionFromPositionList(PositionList* lst, short x, short y)
{
	ListNode* temp, * curr, * newNext;

	if (lst->head->data->row == x && lst->head->data->col == y) //if posToDelete is head
	{
		temp = lst->head;
		lst->head = lst->head->next;
		freePositionListNode(temp);
	}
	else if (lst->tail->data->row == x && lst->tail->data->col == y) //if posToDelete is tail
	{
		temp = lst->tail;
		curr = lst->head;
		while (curr->next != lst->tail)
		{
			curr = curr->next;
		}
		curr->next = NULL;
		lst->tail = curr;
		freePositionListNode(temp);
	}
	else
	{
		curr = lst->head;
		while (curr->next != NULL && (curr->next->data->row != x || curr->next->data->col != y))
		{
			curr = curr->next;
		}
		newNext = curr->next->next;
		temp = curr->next;
		curr->next = newNext;
		freePositionListNode(temp);
	}
}

void printPlayerList(PlayerList* lst)
{
	PlayerListNode* curr = lst->head;
	if (curr == NULL)
	{
		printf("There are no winners\n"); // in case there are no winners

	}
	while (curr != NULL)
	{
		printf("%s\n", curr->data->name); // print player's name
		curr = curr->next;
	}
	printf("---------------------------\n");

}

//-------------------------------ARR------------------------------------------------
Position* createPosition(short row, short col)
{
	Position* newPosition = (Position*)malloc(sizeof(Position)); // allocate Position
	checkAllocation(newPosition);

	newPosition->row = row; // set row
	newPosition->col = col; // set col

	return newPosition;
}

PlayerListNode** buildArrFromPlayerList(PlayerList* listOfPlayer, int numOfPlayer)
{
	PlayerListNode** arrOfPlayerListNode = (PlayerListNode**)malloc(sizeof(PlayerListNode*) * numOfPlayer);
	checkAllocation(arrOfPlayerListNode);

	PlayerListNode* curr = listOfPlayer->head;
	int index = 0;

	while (curr != NULL) // copy into every cells in the arrOfPlayerListNode the ptr to node in listOfPlayer
	{
		arrOfPlayerListNode[index] = curr;
		curr = curr->next;
		index++;
	}

	sortArrOfPointers(arrOfPlayerListNode, index); // sort arr 
	return arrOfPlayerListNode;
}

PlayerListNode** merge(PlayerListNode** a1, int size1, PlayerListNode** a2, int size2)
{
	int ind1, ind2, resInd, compare;
	PlayerListNode** res = (PlayerListNode**)malloc((size1 + size2) * sizeof(PlayerListNode*));
	checkAllocation(res);

	if (res)
	{
		ind1 = ind2 = resInd = 0;

		while ((ind1 < size1) && (ind2 < size2))
		{
			compare = compareFullCell(a1[ind1]->data->gameBoard->board, a2[ind2]->data->gameBoard->board);
			//if emptyCells list of a1 board is shorter than a2 board, 'compare' will be negative number. 
			if (compare < 0)
			{
				res[ind1 + ind2] = a1[ind1++];

			}
			// if emptyCells list of a2 board is shorter than a1 board, 'compare' will be negative number. 
			else if (compare > 0)
			{
				res[ind1 + ind2] = a2[ind2++];

			}
			//if the length of both lists is equal(compare==0), we'll sort by lexicographic order
			else
			{

				if (strcmp(a1[ind1]->data->name, a2[ind2]->data->name) < 0)
				{
					res[ind1 + ind2] = a1[ind1++];
				}

				else
				{
					res[ind1 + ind2] = a2[ind2++];
				}
			}
		}
		while (ind1 < size1) {
			res[ind1 + ind2] = a1[ind1++];

		}
		while (ind2 < size2) {
			res[ind1 + ind2] = a2[ind2++];

		}
		return res;

	}
}

int compareFullCell(short** board1, short** board2)
{
	// scan the whole board, calculates the number of empty cells and returns the difference
	int countFullCells1 = 0, countFullCells2 = 0;
	short row, col;
	for (row = 0; row < 9; row++)
	{
		for (col = 0; col < 9; col++)
		{
			if (board1[row][col] != -1)
			{
				countFullCells1++;
			}
			if (board2[row][col] != -1)
			{
				countFullCells2++;
			}
		}
	}
	return (countFullCells1 - countFullCells2);
}

void setValue(short** sudokuBoard, short row, short col, Array* options)
{
	short index = rand() % options->size; // get random index in the arr of options
	sudokuBoard[row][col] = (options->arr)[index]; // set value
}

void getOptionsArr(Array** options, short** sudokuBoard, short row, short col)
{
	if (*options == NULL) // if options is null, means it is not exist so we'll allocate Array
	{
		*options = (Array*)malloc(sizeof(Array));
		checkAllocation(*options);

		(*options)->size = 9;
		(*options)->arr = (short*)malloc(9 * sizeof(short));
		checkAllocation(options);
	}
	else // we already allocate the options array, so we just need to realloc it and update size to 9
	{
		(*options)->arr = (short*)realloc((*options)->arr, 9 * sizeof(short));
		checkAllocation((*options)->arr);

		(*options)->size = 9;
	}

	//init array
	for (short i = 0; i < 9; i++)
	{
		((*options)->arr)[i] = i + 1;
	}
	//update arr - delete unnecessary options
	checkPossibles(sudokuBoard, row, col, options);
}

void sortArrOfPointers(PlayerListNode** arr, int size)
{
	// sort arr of pointers from min to max of the full cells in the board player
	// if there are more than one player, the sort will be by the names of the players (lexicographic sorting)
	mergeSort(arr, size);
}

void mergeSort(PlayerListNode** arr, int size)
{
	PlayerListNode** tmpArr = NULL;
	if (size <= 1)
		return;

	mergeSort(arr, size / 2);// left half
	mergeSort(arr + size / 2, size - size / 2);//right half
	tmpArr = merge(arr, size / 2, arr + size / 2, size - size / 2);
	if (tmpArr)
	{
		copyArr(arr, tmpArr, size); // copy values from tmpArr to arr
		free(tmpArr);
	}
	else
	{
		printf("Memory allocation failure!!!\n");
		exit(1);	// end program immediately with code 1 (indicating an error)
	}
}

void copyArr(PlayerListNode** dest, PlayerListNode** src, int size)
{
	int i;

	for (i = 0; i < size; i++)
	{
		dest[i] = src[i];
	}
}


//-------------------------------TREE-----------------------------------------------

Tree buildTreeFromArray(PlayerListNode** arr, int size)
{
	Tree tr;

	//---------------    Formula x->n   -------------------
	int p = ceil(log((double)size + 1) / log(2));
	int n = pow(2, p) - 1;
	//-----------------------------------------------------

	PlayerListNode** newArr = (PlayerListNode**)realloc(arr, sizeof(PlayerListNode*) * n);
	checkAllocation(newArr);
	for (int i = size; i < n; i++)
	{
		newArr[i] = NULL; // set null in cells added
	}
	tr=buildTreeFromArrayHelper(newArr, n); // build Tree from Array
	return tr;

}

Tree buildTreeFromArrayHelper(PlayerListNode** arr, int size)
{
	// local Trees
	Tree resTree;
	Tree left;
	Tree right;
	int halfSize;

	// allocate the root of the resTree
	resTree.root = (TreeNode*)malloc(sizeof(TreeNode));
	checkAllocation(resTree.root);

	if (size == 1)//BASE CASE: if there is only one PlayerNode in arr
	{
		//set data. no children
		resTree.root->dataListNode = arr;
		resTree.root->left = NULL;
		resTree.root->right = NULL;
	}
	else
	{ // there is more than one PlayerNode in arr
		halfSize = size / 2; // calc root index 
		resTree.root->dataListNode = arr+halfSize; // set arr[halfSize] value as root data

		left = buildTreeFromArrayHelper(arr, halfSize); // build left sub tree
		resTree.root->left = left.root; // set left.root as resTree.root left son 

		right = buildTreeFromArrayHelper(arr + halfSize + 1, halfSize);// build right sub tree
		resTree.root->right = right.root; // set right.root as resTree.root right son 

	}
	return resTree;
}

void scanTreeInOrder(TreeNode* root, PlayerList* listOfPlayer, PlayerList* winnersList)
{
	short x = -1, y = -1;
	int fillBoardFlag;
	if (root == NULL) //BASE CASE
	{
		return;
	}

	scanTreeInOrder(root->left, listOfPlayer, winnersList); //recursive call by the left side of the tree

	if ((*root->dataListNode) != NULL) // if there is data to root
	{
		int res = OneStage((*root->dataListNode)->data, &x, &y);
		if (res == FINISH_FAILURE)
		{
			deletePlayerFromPlayerList((*root->dataListNode)->data, listOfPlayer);
			(*root->dataListNode)->data = NULL;
		}
		else if (res == FINISH_SUCCESS)
		{
			insertDataToEndOfPlayersList(winnersList, (*root->dataListNode)->data);
			deletePlayerFromPlayerList((*root->dataListNode)->data, listOfPlayer);
			(*root->dataListNode)->data = NULL;
		}
		else if (res == NOT_FINISH)
		{
			fillBoardFlag = fillBoard((*root->dataListNode)->data);
			if (fillBoardFlag == FILLED)
			{
				printf("Board filled successfully\n");
				insertDataToEndOfPlayersList(winnersList, (*root->dataListNode)->data);
				deletePlayerFromPlayerList((*root->dataListNode)->data, listOfPlayer);
			}
			else
			{
				printf("User's selections led to duplications\n");
			}
		}
	}

	scanTreeInOrder(root->right, listOfPlayer, winnersList); // recursive call by the right side of the tree
}

//-------------------------------PLAYER---------------------------------------------

Player* createNewPlayer(int index)
{
	Player* newPlayer = (Player*)malloc(sizeof(Player)); // allocate new player
	checkAllocation(newPlayer);
	printf("please enter name of player number %d:\n ", index + 1);
	scanf("%s", newPlayer->name);
	char ch = getchar();

	newPlayer->gameBoard = createRandomBoard(); // create randomBoard (struct)
	printSodukoBoard(newPlayer->gameBoard->board, newPlayer->name);
	newPlayer->possiblesBoard = PossibleDigits(newPlayer->gameBoard->board); // build possibleBoard according to sudoku board 

	return newPlayer;
}

PlayerList* createListOfPlayers(int numOfPlayer)
{
	Player* player;
	PlayerList* playerList = (PlayerList*)malloc(sizeof(PlayerList)); // allocate new Players List 
	checkAllocation(playerList);
	makeEmptyPlayersList(playerList); // make empty list

	for (int i = 0; i < numOfPlayer; i++) // add numOfPlayers players to the list 
	{
		player = createNewPlayer(i); // create new player
		insertDataToEndOfPlayersList(playerList, player); // add new player to end of list
	}

	return playerList;
}

//-------------------------------FREE---------------------------------------------

void freePlayerList(PlayerList* lst)
{
	PlayerListNode* curr = lst->head;
	PlayerListNode* tmp;
	while (curr != NULL)
	{
		tmp = curr;
		curr = curr->next;
		freePlayer(tmp->data);
		free(tmp); // free node
	}
	free(lst);
}

void freePositionList(PositionList* lst)
{
	ListNode* curr = lst->head;
	ListNode* tmp;
	while (curr != NULL)
	{
		tmp = curr;
		curr = curr->next;
		freePositionListNode(tmp);
	}
	free(lst);
}

void freePositionListNode(ListNode* nodeToDelete)
{
	free(nodeToDelete->data);
	free(nodeToDelete);
}

void freeRandomBoard(RandomBoard* randBoard)
{
	freePositionList(randBoard->emptyCells);// free empty cells list 
	freeSodukoBoard(randBoard->board); // free the board 
	free(randBoard);
}

void freePossBoard(Array*** possBoard)
{
	for (short row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			if (possBoard[row][col] != NULL)
				freeArray(possBoard[row][col]);
		}
		free(possBoard[row]);
	}
	free(possBoard);
}

void freeSodukoBoard(short** board)
{
	for (int row = 0; row < 9; row++)
	{
		free(board[row]); // free row
	}
	free(board); // free board 
}

void freePlayer(Player* player)
{
	freeRandomBoard(player->gameBoard); // free random board
	freePossBoard(player->possiblesBoard);
	free(player);
	player = NULL;
}

void freeArray(Array* options)
{

	free(options->arr); // free arr
	free(options);
	options->arr = NULL;
	options->size = 0;
}

void freeTree(Tree* tr)
{
	freeTreeHelper(tr->root); // free tree
	tr->root = NULL;
}

void freeTreeHelper(TreeNode* root)
{
	if (root == NULL)//BASE CASE	
	{
		return;
	}
	else
	{
		freeTreeHelper(root->left); // recursive call - left side
		freeTreeHelper(root->right);// recursive call - right side
		free(root); // free root 
	}

}

void freePlayersArr(PlayerListNode** playersArr)
{
	free(playersArr);
}