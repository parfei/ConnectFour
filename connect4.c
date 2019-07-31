/* ENGGEN 131 2018 Project*/
/* Connect Four Variant*/
/* Author: Jennifer Lowe */

#include "connect4.h"

/*
InitialiseBoard function creates an initial 2D array Connect 4 board for any given size
indicated from the input. Each element of the board will be set to zero (which indicates
an empty space) and the middle of the board to three (which indicates a fixed position. If 
the board size is even, there will be four fixed positions on the board). 
*/
void InitialiseBoard(int board[MAX_SIZE][MAX_SIZE], int size)
{
	int i, j, parity = size % 2;

	for (i = 0; i < size; i++) { //Set every element to zero for the given size.
		for (j = 0; j < size; j++) {
			board[i][j] = 0;
		}
	}

	if (parity == 1) {	//If size is odd, it will place one 3 in the middle.
		board[size / 2][size / 2] = 3;
	}
	else {	//If size is even, it will place four 3s in the middle.
		board[size / 2][size / 2] = 3;
		board[size / 2 - 1][size / 2] = 3;
		board[size / 2][size / 2 - 1] = 3;
		board[size / 2 - 1][size / 2 - 1] = 3;
	}
}

/******************************************************/
/*
CheckSpace is a helper function for AddMoveToBoard. It returns true if the token is going into a 
NON-empty space and false if the token is going into an empty space.
*/
int CheckSpace(int board[MAX_SIZE][MAX_SIZE], int row, int col) {
	if (board[row][col] != 0) {
		return 1;
	}
	else {
		return 0;
	}
}

/*
AddMoveToBoard function places a token onto the Connect 4 baord. The player indicates what side and move
they want their token to enter the board from. These are passed as inputs into the function. Two pointer inputs
are passed and motified to indicate the position of the row and column of where the token ended up to be.
An invalid move (i.e. entering a token into a space that is occupied) will motify the pointer inputs to be -1.
*/
void AddMoveToBoard(int board[MAX_SIZE][MAX_SIZE], int size, char side, int move, int player, int *lastRow, int *lastCol)
{
	int i;

	/*If the user picked North side.*/
	if (side == 'N') {
		for (i = 0; i < size; i++) {
			if (CheckSpace(board, i, move)) {
				if (i == 0) { //If a potential move is on the edge when entering from the north, this returns an invalid state.
					*lastRow = -1;
					*lastCol = -1;
					break;
				}
				else {
					board[i - 1][move] = player; //The space before the occupied spot on the board will be the token's position.
					*lastRow = i - 1;
					*lastCol = move;
					break;
				}
			}
			if (i == size - 1) { //If the token reaches the end, the end of the board will be the token's position.
				board[i][move] = player;
				*lastRow = i;
				*lastCol = move;
				break;
			}
		}

		/* If the user picked East side.*/
	}
	else if (side == 'E') {
		for (i = size - 1; i >= 0; i--) {
			if (CheckSpace(board, move, i)) {
				if (i == size - 1) {
					*lastRow = -1;
					*lastCol = -1;
					break;
				}
				else {
					board[move][i + 1] = player;
					*lastRow = move;
					*lastCol = i + 1;
					break;
				}
			}
			if (i == 0) {
				board[move][i] = player;
				*lastRow = move;
				*lastCol = i;
				break;
			}
		}

		/*If the user picked West side.*/
	}
	else if (side == 'W') {
		for (i = 0; i < size; i++) {
			if (CheckSpace(board, move, i)) {
				if (i == 0) {
					*lastRow = -1;
					*lastCol = -1;
					break;
				}
				else {
					board[move][i - 1] = player;
					*lastRow = move;
					*lastCol = i - 1;
					break;
				}

			}
			if (i == size - 1) {
				board[move][i] = player;
				*lastRow = move;
				*lastCol = i;
				break;
			}
		}

		/*If the user picked South side.*/
	}
	else if (side == 'S') {
		for (i = size - 1; i >= 0; i--) {
			if (CheckSpace(board, i, move)) {
				if (i == size - 1) {
					*lastRow = -1;
					*lastCol = -1;
					break;
				}
				else {
					board[i + 1][move] = player;
					*lastRow = i + 1;
					*lastCol = move;
					break;
				}
			}
			if (i == 0) {
				board[i][move] = player;
				*lastRow = i;
				*lastCol = move;
				break;
			}
		}
	}

}

/******************************************************/
/*
ExceedBoardCheck is a helper function for various other functions in this source code. It checks if the input index
is smaller than zero (returns a 0) or larger than the largest index (returns size - 1). This prevents unauthorised
indexing into foridden areas of the 2D Connect 4 board.
*/
int ExceedBoardCheck(int index, int size) {

	size = size - 1;

	if (index < 0) {
		return 0; //If the max/min index exceeds, set index as 0.
	}
	else if (index > size) {
		return size; //If the max/min index exceeds, set index as the board size - 1.
	}
	else {
		return index; //If it does not exceed at all, the original index is returned.
	}
}

/*
abs is a helper function for CheckGameOver that returns the absolute value of the input.
*/
int abs(int value) {
	if (value < 0) {
		return value * -1;
	}

	return value;
}

/*
ScanLimit is a helper function CheckGameOver that returns a value of action (see CheckGameOver).
This is used mainly for determining the min and max index of the board when scanning for diagonal
4-in-a-row patterns. It will receive two actions for how much to add/minus for each row and col index.
The function then checks whether these actions exceed the board. The function will eventually return a value that indicates the value of the action 
to CheckGameOver function.
*/
int ScanLimit(int row, int col, int size, int actionr, int actionc) {

	int changer, changec;

	//If the action to the row index exceeds the board, the row's new action value changes.
	if (ExceedBoardCheck(row + actionr, size) != row + actionr) { 
		changer = ExceedBoardCheck(row + actionr, size) - row;
	}
	else {
		changer = actionr; //Else the row's action value stays the same.
	}

	if (ExceedBoardCheck(col + actionc, size) != col + actionc) { //Action for col undergoes the same check.
		changec = ExceedBoardCheck(col + actionc, size) - col;
	}
	else {
		changec = actionc;
	}

	if (abs(changer) < abs(changec)) { //It will return the smaller value of action between the row and col action values.
		return abs(changer);
	}
	else {
		return abs(changec);
	}
}

/******************************************************/
/*
CheckGameOver function checks the input board to see whether the game is over or not.
It will return a 0 if the game is still continuing, or the winning player value.
There are three conditions:
	- If there are no more possible moves to be made, the game is over and the function returns the 
	player who placed the token last as they are now the winner.
	- If the last token forms a 4-in-a-row, the player who placed the token wins and the player's
	value is returned.
	- If row and col is -1, this indicates an invalid move has been made, so the game is not over.
*/
int CheckGameOver(int board[MAX_SIZE][MAX_SIZE], int size, int player, int row, int col)
{
	int i, j, count = 0;
	int top, bot;

	/*Check if row and col is -1, indicating an invalid move had been made.*/
	if ((row == -1) && (col == -1)) {
		return 0;
	}

	/*Check if board cannot take in any more moves. */
	for (i = 0; i < size; i++) { //Checking top length.
		if (board[0][i] == 0) {
			count++;
		}
	}
	for (i = 0; i < size; i++) { //Bottom length.
		if (board[size - 1][i] == 0) {
			count++;
		}
	}
	for (i = 0; i < size; i++) { //Left length.
		if (board[i][0] == 0) {
			count++;
		}
	}
	for (i = 0; i < size; i++) { //Right length.
		if (board[i][size - 1] == 0) {
			count++;
		}
	}

	if (!count) {
		return player; //Returns current player if no more moves can be made onto the board.
	}


	/***Checking for 4-in-a-row.***/

	/*Horizontal check*/
	count = 0;
	for (i = ExceedBoardCheck(col - 3, size); i <= ExceedBoardCheck(col + 3, size); i++) {
		if (board[row][i] == player) { //If the current position is the same as the player value, count increases.
			count++;
		}
		else {
			count = 0; //Reset count to 0 if there was something else other than the player in the 4-in-a-row check.
		}

		if (count == 4) { //If count is 4, return player as the winner.
			return player;
		}
	}

	/*Vertical check.*/
	count = 0;
	for (i = ExceedBoardCheck(row - 3, size); i <= ExceedBoardCheck(row + 3, size); i++) {
		if (board[i][col] == player) {
			count++;
		}
		else {
			count = 0;
		}

		if (count == 4) {
			return player;
		}
	}

	/*Right diagonal check (Top right to bottom left).*/
	count = 0;
	top = ScanLimit(row, col, size, -3, 3); //Retrieve the upper limit for the diagonal scan.
	bot = ScanLimit(row, col, size, 3, -3); //Retrieve the bottom limit for the diagonal scan.

	j = col + top;
	for (i = row - top; i <= row + bot; i++) {

		if (board[i][j] == player) {
			count++;
		}
		else {
			count = 0;
		}
		if (count == 4) {
			return player;
		}
		j--;
	}

	/*Left diagonal check (Top left to bottom right).*/
	count = 0;
	top = ScanLimit(row, col, size, -3, -3);
	bot = ScanLimit(row, col, size, 3, 3);

	j = col - top;
	for (i = row - top; i <= row + bot; i++) {

		if (board[i][j] == player) {
			count++;
		}
		else {
			count = 0;
		}

		if (count == 4) {
			return player;
		}
		j++;
	}

	return 0; //Else, return 0 if the game is still ongoing.
}

/******************************************************/
/*
GetDisplayBoardString takes the board input and turns it into a pretty Connect 4 board that
has the sides and possible moves labelled at the edges of the board.
It returns a motified pointer string with the characters that make up of the board. It replaces:
	- 0 with .
	- 3 with #
	- 1 with X
	- 2 with O
*/
void GetDisplayBoardString(int board[MAX_SIZE][MAX_SIZE], int size, char *boardString)
{
	int i, j, currentNewline, boardr = 0, boardc = 0;
	int addition = size + 5; //Amount of elements in one row of any specified size Connect 4 board.
	char baseNo = 48; //This variable contains the number 0 in char form.

	/*Top row of the board, N signs to indicate the north side.*/
	boardString[0] = '-';
	boardString[1] = '-';

	for (i = 2; i < size + 2; i++) {
		boardString[i] = 'N';
	}

	currentNewline = size + 4; //currentNewLine variable keeps track of the latest newline index in the string.

	boardString[currentNewline - 2] = '-';
	boardString[currentNewline - 1] = '-';
	boardString[currentNewline] = '\n';

	/*Possible moves listed out in the second row of the board.*/
	boardString[currentNewline + 1] = '-';
	boardString[currentNewline + 2] = '-';

	for (i = currentNewline + 3; i < currentNewline + 3 + size; i++) {
		boardString[i] = baseNo;
		baseNo++; //Next iteration will print out the next number.
	}

	currentNewline = currentNewline * 2 + 1;

	boardString[currentNewline - 2] = '-';
	boardString[currentNewline - 1] = '-';
	boardString[currentNewline] = '\n';

	/*West side of the board.*/
	baseNo = 48;
	i = currentNewline;
	while (i < currentNewline + size * addition) {
		boardString[i + 1] = 'W';
		boardString[i + 2] = baseNo;
		baseNo++;
		i = i + addition;
	}

	/*East side of the board.*/
	baseNo = 48;
	i = currentNewline;
	while (i < currentNewline + size * addition) {
		boardString[i + size + 3] = baseNo;
		boardString[i + size + 4] = 'E';
		baseNo++;
		i = i + addition;
	}

	/*The playable Connect 4 board.*/
	i = currentNewline;
	while (i < currentNewline + size * addition) {
		for (j = i + 3; j < i + 3 + size; j++) {
			if (board[boardr][boardc] == 0) { //Replace 0 with .
				boardString[j] = 46;
			}
			else if (board[boardr][boardc] == 3) { //Replace 3 with #
				boardString[j] = 35;
			}
			else if (board[boardr][boardc] == 1) { //Replace 1 with X
				boardString[j] = 88;
			}
			else {
				boardString[j] = 79; //Otherwise replace 2 with O.
			}
			boardc++;
		}
		boardr++; //New row of the board, row index update.
		boardc = 0; //Reset col index back to zero.
		i = i + addition;
	}

	i = currentNewline;
	while (i <= currentNewline + size * addition) { //Inserting the playable Connect 4 board newlines.
		boardString[i] = '\n';
		i = i + addition;
	}
	/*End of playable Connect 4 board.*/


	/*Possible move numbers listed in the second to last row of the board.*/
	baseNo = 48;
	currentNewline = currentNewline + size * addition;

	boardString[currentNewline + 1] = '-';
	boardString[currentNewline + 2] = '-';

	for (i = currentNewline + 3; i < currentNewline + 3 + size; i++) {
		boardString[i] = baseNo;
		baseNo++;
	}

	currentNewline += addition;

	boardString[currentNewline - 2] = '-';
	boardString[currentNewline - 1] = '-';
	boardString[currentNewline] = '\n';

	/*Last row of the board, S signs to indicate the south side of the board.*/
	boardString[currentNewline + 1] = '-';
	boardString[currentNewline + 2] = '-';

	for (i = 3; i < size + 3; i++) {
		boardString[currentNewline + i] = 'S';
	}

	currentNewline += addition;

	boardString[currentNewline - 2] = '-';
	boardString[currentNewline - 1] = '-';
	boardString[currentNewline] = '\n';
	boardString[currentNewline + 1] = '\0'; //End the string.

}

/******************************************************/
/*
CheckWinningMove is a helper function for the bot functions that checks whether the potential move 
(indicated by the inputs row and col) would result in a 4-in-a-row win.
*/
int CheckWinningMove(int board[MAX_SIZE][MAX_SIZE], int size, int player, int row, int col) {
	int temp = board[row][col], result; //Current position value of the board is stored for safety.

	if (board[row][col] == 0) {

		board[row][col] = player; //Temporarily replace the current position to the player value.
		result = CheckGameOver(board, size, player, row, col);
		board[row][col] = temp; //Replace back with the original value.

		return result;
	}

	return 0;
}

/*
CheckValid is a helper function for the bot functions that checks whether the side and move made
by the bot will result in an invalid move or not.
*/
int CheckValid(int board[MAX_SIZE][MAX_SIZE], int size, char side, int move) {

	if (side == 'W') {
		if (board[move][0] != 0) { //If the edge position is occupied, the function returns a false.
			return 0;
		}
	}
	else if (side == 'N') {
		if (board[0][move] != 0) {
			return 0;
		}
	}
	else if (side == 'E') {
		if (board[move][size - 1] != 0) {
			return 0;
		}
	}
	else if (side == 'S') {
		if (board[size - 1][move] != 0) {
			return 0;
		}
	}

	return 1;

}

/*
GetMoveBot1 function contains the logic for a bot to play Connect 4 with. The bot will
always return a VALID move and it will be aggressive always (i.e. if it can make a winning move,
it will).
*/
void GetMoveBot1(int board[MAX_SIZE][MAX_SIZE], int size, int player, char *side, int *move)
{
	int iteration, i, j, nextRow, nextCol, row, col;

	/***Check the board for possible winning moves for the current player.***/
	for (iteration = 0; iteration < 2; iteration++) {

		//Checking west side for potential winning moves.
		for (i = 0; i < size; i++) {
			j = 0;
			nextRow = 0; //While this remains false, the function will continue checking the current column for winning moves.
			while ((j < size) && (!nextRow)) {

				if ((board[i][j] != 0) || (j == size - 1)) { //If it sees a spot on the board to NOT be a empty space...

					row = i; //row and col variables are used to store the index information of the current checking position.
					col = ExceedBoardCheck(j - 1, size);
					if (j == size - 1) { //If the token can be placed at the end of the board, the col index is the end of the board index.
						col = size - 1;
					}

					if (CheckWinningMove(board, size, player, row, col) && CheckValid(board, size, 'W', i)) { //Potential move has to be winning & valid.
						*side = 'W';
						*move = i;
						return;
					}
					nextRow = 1;
				}
				j++; //Next column of the board to check.
			}
		}

		//Checking north side.
		for (j = 0; j < size; j++) {
			i = 0;
			nextCol = 0; //While this remains false, the function will continue checking the current row for winning moves.
			while ((i < size) && (!nextCol)) {

				if ((board[i][j] != 0) || (i == size - 1)) {

					row = ExceedBoardCheck(i - 1, size);
					col = j;
					if (i == size - 1) {
						row = size - 1;
					}
					if (CheckWinningMove(board, size, player, row, col) && CheckValid(board, size, 'N', j)) {
						*side = 'N';
						*move = j;
						return;
					}
					nextCol = 1;
				}
				i++; //Next row of the board to check.
			}
		}

		//Checking east side.
		for (i = 0; i < size; i++) {
			j = size - 1;
			nextRow = 0;
			while ((j >= 0) && (!nextRow)) {

				if ((board[i][j] != 0) || (j == 0)) {

					row = i;
					col = ExceedBoardCheck(j + 1, size);
					if (j == 0) {
						col = 0;
					}

					if (CheckWinningMove(board, size, player, row, col) && CheckValid(board, size, 'E', i)) {
						*side = 'E';
						*move = i;
						return;
					}
					nextRow = 1;
				}
				j--;
			}
		}

		//Checking south side.
		for (j = 0; j < size; j++) {
			i = size - 1;
			nextCol = 0;
			while ((i >= 0) && (!nextCol)) {

				if ((board[i][j] != 0) || (i == 0)) {

					row = ExceedBoardCheck(i + 1, size);
					col = j;
					if (i == 0) {
						row = 0;
					}

					if (CheckWinningMove(board, size, player, row, col) && CheckValid(board, size, 'S', j)) {
						*side = 'S';
						*move = j;
						return;
					}
					nextCol = 1;
				}
				i--;
			}

		}

		/*On the next iteration, the bot will check whether the opponent has potential winning moves or not.
		If it does, the bot will place a valid token to block the oppoent.*/
		if (player == 1) { 
			player = 2;
		}
		else {
			player = 1;
		}
	}

	/*If no winning moves are in sight, then pick a valid side and move for the token.*/
	for (i = 0; i < size; i++) {

		if (CheckValid(board, size, 'W', i)) {
			*side = 'W';
			*move = i;
			break;
		}
		else if (CheckValid(board, size, 'N', i)) {
			*side = 'N';
			*move = i;
			break;
		}
		else if (CheckValid(board, size, 'E', i)) {
			*side = 'E';
			*move = i;
			break;
		}
		else if (CheckValid(board, size, 'S', i)) {
			*side = 'S';
			*move = i;
			break;
		}

	}
	return;
}

/******************************************************/
/*
GetMoveBot2 function has the exact same logic as GetMoveBot1.
*/
void GetMoveBot2(int board[MAX_SIZE][MAX_SIZE], int size, int player, char *side, int *move)
{
	int iteration, i, j, nextRow, nextCol, row, col;

	for (iteration = 0; iteration < 2; iteration++) {

		for (i = 0; i < size; i++) {
			j = 0;
			nextRow = 0;
			while ((j < size) && (!nextRow)) {

				if ((board[i][j] != 0) || (j == size - 1)) {

					row = i;
					col = ExceedBoardCheck(j - 1, size);
					if (j == size - 1) {
						col = size - 1;
					}

					if (CheckWinningMove(board, size, player, row, col) && CheckValid(board, size, 'W', i)) {
						*side = 'W';
						*move = i;
						return;
					}
					nextRow = 1;
				}
				j++;
			}
		}

		for (j = 0; j < size; j++) {
			i = 0;
			nextCol = 0;
			while ((i < size) && (!nextCol)) {

				if ((board[i][j] != 0) || (i == size - 1)) {

					row = ExceedBoardCheck(i - 1, size);
					col = j;
					if (i == size - 1) {
						row = size - 1;
					}

					if (CheckWinningMove(board, size, player, row, col) && CheckValid(board, size, 'N', j)) {
						*side = 'N';
						*move = j;
						return;
					}
					nextCol = 1;
				}
				i++;
			}
		}

		for (i = 0; i < size; i++) {
			j = size - 1;
			nextRow = 0;
			while ((j >= 0) && (!nextRow)) {

				if ((board[i][j] != 0) || (j == 0)) {

					row = i;
					col = ExceedBoardCheck(j + 1, size);
					if (j == 0) {
						col = 0;
					}

					if (CheckWinningMove(board, size, player, row, col) && CheckValid(board, size, 'E', i)) {
						*side = 'E';
						*move = i;
						return;
					}
					nextRow = 1;
				}
				j--;
			}
		}

		for (j = 0; j < size; j++) {
			i = size - 1;
			nextCol = 0;
			while ((i >= 0) && (!nextCol)) {

				if ((board[i][j] != 0) || (i == 0)) {

					row = ExceedBoardCheck(i + 1, size);
					col = j;
					if (i == 0) {
						row = 0;
					}

					if (CheckWinningMove(board, size, player, row, col) && CheckValid(board, size, 'S', j)) {
						*side = 'S';
						*move = j;
						return;
					}
					nextCol = 1;
				}
				i--;
			}

		}

		if (player == 1) {
			player = 2;
		}
		else {
			player = 1;
		}
	}

	for (i = 0; i < size; i++) {

		if (CheckValid(board, size, 'W', i)) {
			*side = 'W';
			*move = i;
			break;
		}
		else if (CheckValid(board, size, 'N', i)) {
			*side = 'N';
			*move = i;
			break;
		}
		else if (CheckValid(board, size, 'E', i)) {
			*side = 'E';
			*move = i;
			break;
		}
		else if (CheckValid(board, size, 'S', i)) {
			*side = 'S';
			*move = i;
			break;
		}

	}

	return;
}