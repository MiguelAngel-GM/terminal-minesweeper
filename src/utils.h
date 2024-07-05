#ifndef UTILS_H
#define UTILS_H

#include "game.h"

bool isInBounds(const int n_rows, const int n_cols, const int row, const int col);

void boardToString(Board *board, char *buffer);

bool clickToBoardCoordinates(Board *board, const int x, const int y, int *row, int *col);

#endif