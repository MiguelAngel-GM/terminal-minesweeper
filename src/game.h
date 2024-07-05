#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdbool.h>

#define MINE 0xFF

struct Tile {
	bool discovered;
	uint8_t content;
    bool flagged;
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

bool discoverTile(Board *board, const int row, const int col);

void discoverNeighbors(Board *board, const int row, const int col);

void placeFlag(Board *board, const int row, const int col);

bool discoverFlagged(Board *board, const int row, const int col);

bool allClear(Board *board);

void discoverAllMines(Board *board);

#endif