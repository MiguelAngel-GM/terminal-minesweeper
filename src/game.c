#include "game.h"
#include <stdlib.h>

Board* createBoard(const int n_rows, const int n_cols) {
	Tile **tiles = (Tile**)malloc(n_rows * sizeof(Tile*));
	for(int i = 0; i < n_rows; i++) {
		tiles[i] = (Tile*)malloc(n_cols * sizeof(Tile));
	}

	for(int i = 0; i < n_rows; i++) {
		for(int j = 0; j < n_cols; j++) {
			tiles[i][j].discovered = 0;
			tiles[i][j].content = 0;
			tiles[i][j].flagged = 0;
		}
	}

	Board *board = (Board*)malloc(sizeof(Board));
	board->n_cols = n_cols;
	board->n_rows = n_rows;
	board->tiles = tiles;

	return board;
}

void destroyBoard(Board *board) {
	for(int i = 0; i < board->n_rows; i++) {
		free(board->tiles[i]);
	}

	free(board->tiles);
	free(board);
}

int isInBounds(const int n_rows, const int n_cols, const int row, const int col) {
	return (row >= 0) && (col >= 0) && (row < n_rows) && (col < n_cols);
}

void updateNeighbors(Board *board, const int row, const int col) {
	for(int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
			int row_neighbor = row + i;
			int col_neighbor = col + j;
			if(isInBounds(board->n_rows, board->n_cols, row_neighbor, col_neighbor)) {
				if(board->tiles[row_neighbor][col_neighbor].content != MINE) {
					board->tiles[row_neighbor][col_neighbor].content++;
				}
			}
		}
	}
}

void populateBoard(Board *board, const int n_mines, const int row_start, const int col_start) {
	// populate with mines
	int placed_mines = 0;
	while(placed_mines < n_mines) {
		int row = rand() % board->n_rows;
		int col = rand() % board->n_cols;
		Tile *t = &(board->tiles[row][col]);
		if(t->content != MINE && (row != row_start || col != col_start)) {
			t->content = MINE;
			updateNeighbors(board, row, col);
			placed_mines++;
		}
	}
}

void board_to_string(Board *board, char *buffer) {

	for(int i = 0; i < board->n_rows; i++) {
		for(int j = 0; j < board->n_cols; j++) {
			if(board->tiles[i][j].discovered) {
				if(board->tiles[i][j].content != MINE) {
					buffer[i*board->n_cols+j] = board->tiles[i][j].content + '0';
				}
				else {
					buffer[i*board->n_cols+j] = '*';
				}
			}
			else {
				buffer[i*board->n_cols+j] = '#';
			}
		}
	}
}