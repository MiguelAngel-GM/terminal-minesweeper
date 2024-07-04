#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define MINE 0xFF

struct Tile {
	uint8_t discovered;
	uint8_t content;
    uint8_t flagged;
} typedef Tile;

struct Board {
	int n_cols;
	int n_rows;
	int n_mines;
	int n_flags;
	int n_discovered;
	Tile **tiles;
} typedef Board;

Board* createBoard(const int n_rows, const int n_cols, const int n_mines);

void destroyBoard(Board *board);

void updateNeighbors(Board *board, const int row, const int col);

void populateBoard(Board *board, const int row_start, const int col_start);

int discoverTile(Board *board, const int row, const int col);

void discoverNeighbors(Board *board, const int row, const int col);

void placeFlag(Board *board, const int row, const int col);

int discoverFlagged(Board *board, const int row, const int col);

int allClear(Board *board);

void discoverAllMines(Board *board);

#endif