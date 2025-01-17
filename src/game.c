#include "game.h"
#include "utils.h"
#include <stdlib.h>

Board* createBoard(const int n_rows, const int n_cols, const int n_mines) {
	Tile **tiles = (Tile**)malloc(n_rows * sizeof(Tile*));
	for(int i = 0; i < n_rows; i++) {
		tiles[i] = (Tile*)malloc(n_cols * sizeof(Tile));
	}

	for(int i = 0; i < n_rows; i++) {
		for(int j = 0; j < n_cols; j++) {
			tiles[i][j].discovered = false;
			tiles[i][j].content = false;
			tiles[i][j].flagged = false;
		}
	}

	Board *board = (Board*)malloc(sizeof(Board));
	board->n_cols = n_cols;
	board->n_rows = n_rows;
	board->n_mines = n_mines;
	board->n_flags = 0;
	board->n_discovered = 0;
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

void populateBoard(Board *board, const int row_start, const int col_start) {
	// populate with mines
	int placed_mines = 0;
	while(placed_mines < board->n_mines) {
		int row = rand() % board->n_rows;
		int col = rand() % board->n_cols;
		Tile *t = &(board->tiles[row][col]);
		
		bool startNeighbor = false;

		for(int i = -1; i <= 1; i++) {
			for(int j = -1; j <= 1; j++) {
				if(row == row_start + i && col == col_start + j) {
					startNeighbor = true;
					break;
				}
			}
		}
		
		if(t->content != MINE && !startNeighbor) {
			t->content = MINE;
			updateNeighbors(board, row, col);
			placed_mines++;
		}
	}
}

bool discoverTile(Board *board, const int row, const int col) {
	if(!board->tiles[row][col].flagged) {
		if(board->tiles[row][col].content == MINE)
			return false;
		if(board->tiles[row][col].discovered)
			return discoverFlagged(board, row, col);
		
		board->tiles[row][col].discovered = 1;
		board->n_discovered++;
		if(board->tiles[row][col].content == 0) {
			discoverNeighbors(board, row, col);
		}
	}

	return true;
}

void discoverNeighbors(Board *board, const int row, const int col) {
	for(int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
			int row_neighbor = row + i;
			int col_neighbor = col + j;
			if(isInBounds(board->n_rows, board->n_cols, row_neighbor, col_neighbor)) {
				if(board->tiles[row_neighbor][col_neighbor].content != MINE && !board->tiles[row_neighbor][col_neighbor].discovered) {
					board->tiles[row_neighbor][col_neighbor].discovered = 1;
					board->n_discovered++;
					if(board->tiles[row_neighbor][col_neighbor].flagged) {
						board->tiles[row_neighbor][col_neighbor].flagged = 0;
						board->n_flags--;
					}
					if(board->tiles[row_neighbor][col_neighbor].content == 0) {
						discoverNeighbors(board, row_neighbor, col_neighbor);
					}
				}
			}
		}
	}
}

void placeFlag(Board *board, const int row, const int col) {
	if(!board->tiles[row][col].discovered) {
		if(board->tiles[row][col].flagged) {
			board->tiles[row][col].flagged = 0;
			board->n_flags--;
		}
		else {
			if(board->n_flags < board->n_mines) {
				board->tiles[row][col].flagged = 1;
				board->n_flags++;
			}
		}
	}
}

bool discoverFlagged(Board *board, const int row, const int col) {
	int neighbor_flags = 0;
	
	// count flags placed in neighborhood
	for(int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
			if(isInBounds(board->n_rows, board->n_cols, row + i, col + j) && board->tiles[row+i][col+j].flagged)
				neighbor_flags++;
		}
	}

	// if the number of flags is correct, check that they are placed on top of mines
	if(board->tiles[row][col].content == neighbor_flags) {
		for(int i = -1; i <= 1; i++) {
			for(int j = -1; j <= 1; j++) {
				if(isInBounds(board->n_rows, board->n_cols, row + i, col + j) && board->tiles[row+i][col+j].flagged) {
					if(board->tiles[row+i][col+j].content != MINE)
						return false;
				}
			}
		}		
		discoverNeighbors(board, row, col);
	}

	return true;
}

bool allClear(Board *board) {
	return board->n_discovered == board->n_rows * board->n_cols - board->n_mines;
}

void discoverAllMines(Board *board) {
	for(int i = 0; i < board->n_rows; i++) {
		for(int j = 0; j < board->n_cols; j++) {
			if(board->tiles[i][j].content == MINE)
				board->tiles[i][j].discovered = 1;
		}
	}
}
