#include "utils.h"
#include <ncurses.h>

int isInBounds(const int n_rows, const int n_cols, const int row, const int col) {
	return (row >= 0) && (col >= 0) && (row < n_rows) && (col < n_cols);
}

void boardToString(Board *board, char *buffer) {

	for(int i = 0; i < board->n_rows; i++) {
		for(int j = 0; j < board->n_cols; j++) {
			if(board->tiles[i][j].discovered) {
				if(board->tiles[i][j].content == 0) {
					buffer[i*board->n_cols+j] = ' ';
				}
				else if(board->tiles[i][j].content != MINE) {
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

int clickToBoardCoordinates(Board *board, const int x, const int y, int *row, int *col) {
    int win_rows, win_cols;
    getmaxyx(stdscr, win_rows, win_cols);

    int x0 = win_cols / 2 - board->n_cols / 2;
    int y0 = win_rows / 2 - board->n_rows / 2;

    *col = x - x0;
    *row = y - y0;

    return isInBounds(board->n_rows, board->n_cols, *row, *col);
}