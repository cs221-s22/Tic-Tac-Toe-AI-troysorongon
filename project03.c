#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

typedef char tictac_board[3][3];

// struct to get the row and col of a move, used for displaying where O is played
struct Move{
	int row, col;	
};

//sets the board from the commmand line  arguments 
void set_board(tictac_board b, char **argv){
	int n = 0;
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			b[r][c] = argv[n++][0];		// sets position on board to the first character of the char*
		}
	}
}

//sets a move on the board
bool set_move(tictac_board b, int r, int c, char turn){
	if(!isdigit(r) && !isdigit(c) && (r > 2 || r < 0) && (c > 2 || c < 0)){		// if the inputs are not a number or are out of range, returns false
		return false;
	}
	if(b[r][c] == '?'){		// if space is empty, the spot is filled with char
		b[r][c] = turn;
		return true;
	}

	return false;	// returns false if spot is already played
}

//prints board
void print_board(tictac_board b){

	printf("%c | %c | %c\n", b[0][0], b[0][1], b[0][2]);
	printf("--+---+--\n");
	printf("%c | %c | %c\n", b[1][0], b[1][1], b[1][2]);
	printf("--+---+--\n");
	printf("%c | %c | %c\n", b[2][0], b[2][1], b[2][2]);

}

//checks for row win
bool check_row_win(tictac_board b, char turn, int r){
	for(int c = 0; c < 3; c++){
		char ch = b[r][c];
		if(ch != turn){
			return false;
		}
	}
	return true;
}

//checks for column win
bool check_col_win(tictac_board b, char turn, int c){
	for(int r = 0; r < 3; r++){
		char ch = b[r][c];
		if(ch != turn){
			return false;
		}
	}
	return true;
}

//checks for diagonal win --> top left to bottom right
bool check_diag1_win(tictac_board b, char turn){
	if(b[0][0] == turn && b[1][1] == turn && b[2][2] == turn){
		return true;
	}

	return false;

}

//checks for diagonal win --> bottom left to top right
bool check_diag2_win(tictac_board b, char turn){
	if(b[2][0] == turn && b[1][1] == turn && b[0][2] == turn){
		return true;
	}

	return false;
}

//checks if there is a winning scenario on the board
bool check_win(tictac_board b, char turn){
	for(int r = 0; r < 3; r++){
		if(check_row_win(b, turn, r)){
			return true;
		}
	}

	for(int c = 0; c < 3; c++){
		if(check_col_win(b, turn, c)){
			return true;
		}
	}

	if(check_diag1_win(b, turn)){
		return true;
	}

	if(check_diag2_win(b, turn)){
		return true;
	}

	return false;	
}

//cheks if there is a draw 
bool check_draw(tictac_board b){
	for(int r = 0; r < 3; r++){
		for(int c = 0; c < 3; c++){
			if(b[r][c] == '?'){		// empty space means the game is not finished
				return false;
			}
		}
	}

	return true;	// if there is not winner, game result is a tie
}

//gets the value of the maximizer and minimizer
int getMinMaxVal(tictac_board board){
	char turn = 'X';
	if(check_win(board, turn)){		// if X wins, returns 10: maximizer value
		return 10;
	}
	else{
		turn = 'O';
		if(check_win(board, turn)){		// if O wins, returns -10: minimizer value
			return -10;
		}
	}

	return 0;
}

//goes through every available spot available on the board and decides which move is most optimal based on score
int minimax(tictac_board board, int depth, bool isMaximizer){
	int score = getMinMaxVal(board);	

	if(score == 10){	// scenario where the Maximizer wins, returns score
		return score;
	}

	if(score == -10){	// scenario where the Minimizer wins, returns score
		return score;
	}

	if(check_draw(board)){	// if there is a draw, returns 0.
		return 0;
	}

	if(isMaximizer){	// looks at X move
	
		int bestScore = -1000;	

		// goes through each available spot on the board and calculates the best score based on the effectiveness of a move
		for(int i = 0; i < 3; i++){		
			for(int j = 0; j < 3; j++){
				if(board[i][j] == '?'){
					board[i][j] = 'X';	// places X if spot on board is empty so it can be evaluated
					int score = minimax(board, depth + 1, false);	// calls minimax to find the final score of that move. See's how many moves it would take for game to finish
					board[i][j] = '?';	// after score is calculated, position on board is reset to an empty spot 
					if(score > bestScore){	
						bestScore = score;	// bestScore is the largest score found from all the possible moves
					}
				}
			}
		}

		return bestScore; 
		
	}
	else{	// looks at O move

		int bestScore = 1000;

		// goes through each available spot on the board and calculates the best score based on the effectiveness of a move
		for(int i = 0; i < 3; i++){
			for(int j = 0; j < 3; j++){
				if(board[i][j] == '?'){
					board[i][j] = 'O';	// places O if spot on board is empty so it can be evaluated
					int score = minimax(board, depth + 1, true);		// calls minimax to find the final score of that move. See's how many moves it would take for game to finish
					board[i][j] = '?';	// after score is calculated, position on board is reset to an empty spot 
					if(score < bestScore){
						bestScore = score;	// bestScore is the smallest score found from all the possible moves
					}
				}
			}
		}

		return bestScore; 

	}	
}

// calculates the best move for player or computer 
struct Move getBestMove(tictac_board board){
	int bestScore = 10000;
	struct Move bestMove;	// creates a new struct to store row and column values
	bestMove.row = -1;	// assigned -1 because it is not a possible move on the board
	bestMove.col = -1;	// ^^

	// goes through each available spot on the board and calculates the best move for Minimizer
	for(int i = 0; i < 3; i++){
		for(int n = 0; n < 3; n++){
			if(board[i][n] == '?'){
				board[i][n] = 'O';
				int val = minimax(board, 0, true);
				board[i][n] = '?';
				if(val < bestScore){	
					bestMove.row = i;	// assigns the row value based on value calculated from minimax
					bestMove.col = n;	// assigns the column value based on value calculated from minimax
					bestScore = val;	// bestScore is the smallest score found from all possible moves
				}
			}
		}
	}

	if(bestMove.row == -1 && bestMove.col == -1){	// 
		for(int i = 0; i < 3; i++){
			for(int n = 0; n < 3; n++){
				if(board[i][n] == '?'){
					board[i][n] = 'X';
					int val = minimax(board, 0, false);
					board[i][n] = '?';
					if(val > bestScore){
						bestMove.row = i;
						bestMove.col = n;
						bestScore = val;
					}
				}
			}
		}
	}

	return bestMove;
}


int main(int argc, char* argv[]){
	
	if(argc == 1){
		tictac_board board = {{'?', '?', '?'}, {'?', '?', '?'},  {'?', '?', '?'}};	// initializes an empty board
		char turn = 'X';	// first turn is the player: X
		print_board(board);
		
		while(1){	// infinie loop until broken by win or draw
			int r, c;
			printf("%c: ", turn);
			scanf("%d %d", &r, &c);	// receives the move from player
			
			
			if(set_move(board, r, c, turn)){	// checks if move is legal
				if(check_win(board, turn)){		// checks if there is a win
					print_board(board);
					printf("%c wins\n", turn);
					break;	// ends infinite loop
				}
				else if(check_draw(board)){	// checks if there is a draw
					print_board(board);
					printf("draw\n");
					break;	// ends infinite loop
				}

				if(turn == 'X'){	// swithces turn to computer: O
					turn = 'O';
				}

				if(turn == 'O'){	
					struct Move theMove;	// creates struct to get row and column
					theMove = getBestMove(board);	// loops for best move 
					int row = theMove.row;	
					int col = theMove.col;
					if(set_move(board, row, col, turn)){	// checks if move is legal
						printf("%c moves to %d %d\n", turn, row, col);	// statement to show where O moved
						if(check_win(board, turn)){		// checks if there is a win
							print_board(board);
							printf("%c wins\n", turn);
							break;	// breaks infinite loop
						}
						else if(check_draw(board)){		// checks if there is a draw
							print_board(board);
							printf("draw\n");	
							break;	// breaks infinite loop
						}

						print_board(board);	// prints updated board
					}

					turn = 'X'; 	// switches turn to player: X
				}
			}
			else{
				printf("Illegal Move\n");	// if it not a legal move, prints message and gets a new input from player
			}	
		}
	}
	
	if(argc > 2){	// if board is partially filled 
		tictac_board board;		// creates a new board
		set_board(board, &argv[1]);		// sets the value from command line to their positions on board
		print_board(board);

		
		struct Move theMove;	// creates struct to get row and column
		theMove = getBestMove(board);	// loops for best move
		int row = theMove.row;		
		int col = theMove.col;
		if(set_move(board, row, col, 'O')){		// sets O on the board after best move is calculated 
			printf("%c: %d %d\n", 'O', row, col);
		}
	}
}
