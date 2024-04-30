#include "game.h"
#include <stdlib.h>

Tile** createBoard(const int n_rows, const int n_cols) {
	Tile **board = (Tile**)malloc(n_rows * sizeof(Tile*));
	for(int i = 0; i < n_rows; i++) {
		board[i] = (Tile*)malloc(n_cols * sizeof(Tile));
	}

	for(int i = 0; i < n_rows; i++) {
		for(int j = 0; j < n_cols; j++) {
			board[i][j].discovered = 0;
			board[i][j].content = 0;
			board[i][j].flagged = 0;
		}
	}

	return board;
}

void destroyBoard(Tile **board, const int n_rows) {
	for(int i = 0; i < n_rows; i++) {
		free(board[i]);
	}

	free(board);
}

int isInBounds(const int n_rows, const int n_cols, const int row, const int col) {
	return (row >= 0) && (col >= 0) && (row < n_rows) && (row < n_cols);
}

void updateNeighbors(Tile **board, const int n_rows, const int n_cols, const int row, const int col) {
	for(int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
			if(isInBounds(n_rows, n_cols, row + i, col + j)) {
				if(board[row + i][col + j].content != MINE) {
					board[row + i][col + j].content++;
				}
			}
		}
	}
}

void populateBoard(Tile **board, const int n_rows, const int n_cols, const int n_mines) {
	// populate with mines
	int placed_mines = 0;
	while(placed_mines < n_mines) {
		int row = rand() % n_rows;
		int col = rand() % n_cols;
		Tile *t = &board[row][col];
		if(t->content != MINE) {
			t->content = MINE;
			updateNeighbors(board, n_rows, n_cols, row, col);
			placed_mines++;
		}
	}
}