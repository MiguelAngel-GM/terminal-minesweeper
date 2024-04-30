#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define MINE 0xFF

struct Tile {
	uint8_t discovered;
	uint8_t content;
    uint8_t flagged;
} typedef Tile;

Tile** createBoard(const int n_rows, const int n_cols);

void destroyBoard(Tile **board, const int n_rows);

int isInBounds(const int n_rows, const int n_cols, const int row, const int col);

void updateNeighbors(Tile **board, const int n_rows, const int n_cols, const int row, const int col);

void populateBoard(Tile **board, const int n_rows, const int n_cols, const int n_mines);

#endif