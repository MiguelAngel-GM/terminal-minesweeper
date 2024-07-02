#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define MINE 0xFF

enum GameState {
    MENU,
    PLAYING
};

struct Tile {
	uint8_t discovered;
	uint8_t content;
    uint8_t flagged;
} typedef Tile;

struct Board {
	int n_cols;
	int n_rows;
	Tile **tiles;
} typedef Board;

Board* createBoard(const int n_rows, const int n_cols);

void destroyBoard(Board *board);

int isInBounds(const int n_rows, const int n_cols, const int row, const int col);

void updateNeighbors(Board *board, const int row, const int col);

void populateBoard(Board *board, const int n_mines, const int row_start, const int col_start);

void board_to_string(Board *board, char *buffer);

#endif